#!/bin/bash
set -euo pipefail

# 参数：第一个为源文件名（不带 .cpp），第二个可选输入文件，第三个可选输出说明
FILE=${1:?用法: $0 <name> [in.txt] [out.txt]}
IN=${2:-"in.txt"}
OUT=${3:-"out.txt"}
ACTUAL_OUT=${ACTUAL_OUT:-"result.txt"}
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

FILENAME="$FILE.cpp"
BINARY="$FILE"

if [ ! -f "$FILENAME" ]; then
    echo "错误: $FILENAME 不存在"
    exit 1
fi

# 默认走 xcrun 解析当前 `xcode-select` 激活的 Xcode 工具链，不依赖 PATH 顺序——
# 之前排查过一次因为 PATH 悄悄退回/错配编译器版本，导致 SAN=1 在 dyld 初始化时死锁的坑
# （工具链 ABI 不匹配触发的已知 ASan bug，同类分析见 LLVM #200447 / FastLED #3642+#3644），
# Apple 在 Xcode 26.5 修了这个问题，Xcode 26.6 起默认可用，不再需要手动切换 PATH。
# LLVM=1 保留作应急开关：万一 Apple clang 又出问题，显式切到 Homebrew 装的独立 LLVM
# （固定绝对路径，不依赖 PATH，避免重蹈"PATH 顺序被 dotfiles 清理悄悄改掉"的坑）。
if [ -n "${LLVM:-}" ]; then
    if [ -x "/opt/homebrew/opt/llvm/bin/clang++" ]; then
        COMPILER="/opt/homebrew/opt/llvm/bin/clang++"
    else
        echo "警告: LLVM=1 但没找到 /opt/homebrew/opt/llvm/bin/clang++，回退到默认编译器" >&2
        COMPILER="xcrun clang++"
    fi
else
    # 注意：必须用 `xcrun clang++` 当包装器调用，不能先 `xcrun -f clang++` 解析出绝对路径
    # 再单独调用那个路径——Xcode.app 工具链目录里的 clang++ 是个裸二进制，脱离 xcrun
    # 帮它注入 SDK/isysroot 直接调用会报 "ld: library 'c++' not found"（踩过这个坑）。
    COMPILER="xcrun clang++"
fi
CPPFLAGS_STDCXX_H='-I/usr/local/include'
STD_CPP='-std=c++26 -Wall -Wextra -pedantic-errors -Wfatal-errors -DLOCAL' # -Werror
PERF_OPT='-DNDEBUG -march=native -fomit-frame-pointer'
DEBUG_INFO='-g'
FRAME_POINTER='-fno-omit-frame-pointer'
# ASan + UBSan：默认关闭，需要时用 SAN=1 显式开启。
if [ -n "${SAN:-}" ]; then
    SANITIZER='-fsanitize=address,undefined'
else
    SANITIZER=''
fi
STATIC="time -l"
DIFF=TRUE

# 下面这一整块只在真的启用 sanitizer 时才有意义，收进这个分支里，
# 不带 SAN=1 跑的时候不会顺手 export 一堆跟这次编译无关的环境变量。
if [ -n "$SANITIZER" ]; then
    # Homebrew LLVM：让 ASan/UBSan 报告里的栈带 file:line（需编译带 -g）
    if [ -x "/opt/homebrew/opt/llvm/bin/llvm-symbolizer" ]; then
        export ASAN_SYMBOLIZER_PATH="/opt/homebrew/opt/llvm/bin/llvm-symbolizer"
        export UBSAN_SYMBOLIZER_PATH="${ASAN_SYMBOLIZER_PATH}"
    elif [ -x "/usr/local/opt/llvm/bin/llvm-symbolizer" ]; then
        export ASAN_SYMBOLIZER_PATH="/usr/local/opt/llvm/bin/llvm-symbolizer"
        export UBSAN_SYMBOLIZER_PATH="${ASAN_SYMBOLIZER_PATH}"
    fi
    # 默认关闭 leak 检测（噪音大）。想开就在调用前自己覆盖，比如：
    # ASAN_OPTIONS='detect_leaks=1:halt_on_error=0' SAN=1 ./run.sh main
    export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=0:symbolize=1}"

    # macOS 下 LSan 常会误报 libobjc/libxpc/dyld 等系统运行时常驻分配；
    # 默认启用 suppressions 过滤噪声（只有你自己把 detect_leaks 打开时才用得上）。
    if [ "$(uname -s)" = "Darwin" ] && [ -z "${LSAN_OPTIONS:-}" ] && [ -z "${NO_LSAN_SUPPRESS:-}" ]; then
        export LSAN_OPTIONS="suppressions=${SCRIPT_DIR}/lsan-macos.supp:print_suppressions=0"
    fi
fi

# 说明：以前这里用 `2> >(tee /dev/stderr | awk ...)` 这种进程替换来"边输出边统计峰值内存"。
# 问题：进程替换会派生一个不受脚本主进程生命周期约束的后台 tee/awk 子进程；一旦上面
# 被计时的命令因 set -e 触发脚本提前退出（比如程序里手动 exit(N) 提前结束），这个后台
# 子进程可能还没退出、还占着 stderr 管道的写端，导致外部工具（IDE 运行面板/CI 日志采集，
# 只在这条流真正 EOF 才判定"结束"的场景）误以为进程还在跑。
# 现在改用临时文件收集 stderr，跑完了再一次性 cat + awk，没有额外的后台进程，
# 代价是编译/运行时的 stderr 不再是实时流式输出，而是结束后一次性打印。
compile() {
    local tmp_err rc=0
    tmp_err="$(mktemp)"
    if [ -n "${PERF:-}" ]; then
        echo ">>> [性能模式] 编译 (-O3, 无 Sanitizer)..."
        $STATIC $COMPILER $CPPFLAGS_STDCXX_H $STD_CPP -O3 $PERF_OPT "$FILENAME" -o "$BINARY" 2> "$tmp_err" || rc=$?
    else
        if [ -n "$SANITIZER" ]; then
            echo ">>> [调试模式] 编译 (-O0 + ASan/UBSan + $DEBUG_INFO)..."
        else
            echo ">>> [调试模式] 编译 (-O0 + $DEBUG_INFO, 无 Sanitizer；SAN=1 可开启)..."
        fi
        $STATIC $COMPILER $CPPFLAGS_STDCXX_H $STD_CPP -O0 $DEBUG_INFO $FRAME_POINTER $SANITIZER "$FILENAME" -o "$BINARY" 2> "$tmp_err" || rc=$?
    fi
    cat "$tmp_err" >&2
    awk '/maximum resident set size/ {printf "编译峰值内存: %.2f MB\n", $1/1024/1024}' "$tmp_err"
    rm -f "$tmp_err"
    return "$rc"
}

run_bin() {
    local rc=0
    if [ -n "${DEBUG:-}" ]; then
        echo ">>> 启动 LLDB..."
        lldb "./$BINARY" || rc=$?
    elif [ -n "${INTERACTIVE:-}" ] || [ ! -f "$IN" ]; then
        # INTERACTIVE=1 和"没有 $IN 可读"其实是同一件事——不喂文件，直接接键盘输入，
        # 只是提示语不同，合并成一个分支避免重复。
        if [ -n "${INTERACTIVE:-}" ]; then
            echo ">>> 交互运行..."
        else
            echo ">>> 提示: 无 ${IN}，直接运行..."
        fi
        $STATIC "./$BINARY" || rc=$?
        echo "退出码: $rc"
    else
        echo ">>> 输入: $IN"
        local tmp_err
        tmp_err="$(mktemp)"
        $STATIC "./$BINARY" < "$IN" > "$ACTUAL_OUT" 2> "$tmp_err" || rc=$?
        cat "$tmp_err" >&2
        awk '/maximum resident set size/ {printf "运行峰值内存: %.2f MB\n", $1/1024/1024}' "$tmp_err"
        rm -f "$tmp_err"

        echo ">>> 程序输出已保存: $ACTUAL_OUT"
        echo "退出码: $rc"
        if [ -n "${NO_DIFF:-}" ]; then
            echo ">>> 已开启 NO_DIFF，跳过 diff，直接输出程序结果："
            cat "$ACTUAL_OUT"
            return "$rc"
        fi

        if [ "${DIFF:-}" = TRUE ]; then
            if [ -f "$OUT" ]; then
                echo ">>> 对比期望输出: $OUT"
                if diff -u --strip-trailing-cr "$OUT" "$ACTUAL_OUT"; then
                    echo ">>> ✅ 输出一致"
                else
                    echo ">>> ❌ 输出不一致"
                fi
            else
                echo ">>> 提示: 期望输出文件 $OUT 不存在，已跳过 diff"
            fi
        fi
    fi
    return "$rc"
}

if compile; then
    # if [ -z "${PERF:-}" ] && [ -n "${ASAN_SYMBOLIZER_PATH:-}" ]; then
    #     echo ">>> ASAN_SYMBOLIZER_PATH=${ASAN_SYMBOLIZER_PATH}"
    # fi
    if [ -n "${RUN:-}" ]; then
        run_bin
        exit $?
    fi
else
    echo "编译失败。"
    exit 1
fi
