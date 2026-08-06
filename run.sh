#!/bin/bash
set -euo pipefail

# ============================================================
# run.sh —— 编译 <name>.cpp，可选立刻跑一次并跟期望输出 diff。
#
# 完整用法说明：./run.sh -h  （或者直接往下看 print_usage() 里那段）
#
# 分成几个阶段，从上到下：解析参数 -> 校验源文件 -> 选编译器 -> 拼编译参数
# -> (可选)配置 sanitizer 环境 -> 定义 compile()/run_bin() -> 主流程按顺序调用。
# 每个阶段是一个函数，但函数里的赋值不加 `local` 就是全局的——所以拆成函数
# 只是换了个组织方式，compile()/run_bin() 依然能看到这些变量，行为不变。
# ============================================================

print_usage() {
    cat <<USAGE
用法: $0 <name> [in.txt] [out.txt]

  编译 <name>.cpp 为可执行文件 <name>。只编译不会跑，要跑得加 RUN=1。

位置参数（in.txt/out.txt 可省略，省略就用默认值）：
  name       必填，源文件名，不带 .cpp 后缀
  in.txt     跑的时候喂给程序的输入文件          默认: in.txt
  out.txt    对拍用的期望输出文件                 默认: out.txt

环境变量开关（默认都是空/关闭；写成 X=1 打开，非空即算打开）：
  RUN=1          编译后立即跑一次。不加这个只编译，不执行。
  SAN=1          编译加 -fsanitize=address,undefined（ASan+UBSan）。
                 默认关闭：默认走 -O0 调试构建，配 lldb 调更常用。
  PERF=1         切成性能构建：-O3 -march=native，不带调试信息/sanitizer。
                 跟 SAN=1 一起给的话 SAN 会被忽略（性能构建那条分支不看 SANITIZER）。
  DEBUG=1        编译后不跑，直接进 lldb。跟下面两个互斥，优先级最高。
  INTERACTIVE=1  不喂 in.txt，直接接键盘输入交互跑。
                 效果和"in.txt 不存在时"完全一样，只是提示语不同。
  NO_DIFF=1      跑完只打印程序输出，不跟 out.txt 做 diff。
                 只在真的读了 in.txt 去跑的那个分支下才有意义。
  LLVM=1         编译器换成 Homebrew 装的独立 LLVM（绝对路径，不依赖 PATH）。
                 默认走 xcrun，跟随 xcode-select 选中的 Xcode 工具链；只有
                 Apple clang 自己又出问题时才需要这个应急开关。

  ACTUAL_OUT           程序实际输出存去哪个文件          默认: result.txt
  ASAN_OPTIONS         SAN=1 时生效，可覆盖默认值 detect_leaks=0:symbolize=1
  LSAN_OPTIONS         SAN=1 时生效，默认套 lsan-macos.supp 过滤 macOS 系统噪音
  NO_LSAN_SUPPRESS=1   跳过上面这个默认 suppressions，看原始 LSan 输出

常见组合：
  ./run.sh main                      只编译，不跑
  RUN=1 ./run.sh main                编译 + 用 in.txt 跑一次 + diff out.txt
  RUN=1 SAN=1 ./run.sh main          开 ASan/UBSan 跑一次
  RUN=1 PERF=1 ./run.sh main         性能构建跑一次（不带 sanitizer/调试信息）
  DEBUG=1 ./run.sh main              编译后直接进 lldb
  RUN=1 INTERACTIVE=1 ./run.sh main  不读 in.txt，手动敲输入交互跑
  RUN=1 NO_DIFF=1 ./run.sh main      跑完只看程序输出，不 diff
  RUN=1 LLVM=1 ./run.sh main         应急换成 Homebrew LLVM 编译
USAGE
}

# ---------- 阶段 1：解析命令行参数 + 环境变量默认值 ----------
parse_args() {
    case "${1:-}" in
        -h|--help)
            print_usage
            exit 0
            ;;
    esac

    # 参数：第一个为源文件名（不带 .cpp），第二个可选输入文件，第三个可选输出说明
    FILE=${1:?用法: $0 <name> [in.txt] [out.txt]（完整说明: $0 -h）}
    IN=${2:-"in.txt"}
    OUT=${3:-"out.txt"}
    ACTUAL_OUT=${ACTUAL_OUT:-"result.txt"}
    SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

    FILENAME="$FILE.cpp"
    BINARY="$FILE"
}

# ---------- 阶段 2：源文件存在性校验 ----------
check_source_exists() {
    if [ ! -f "$FILENAME" ]; then
        echo "错误: $FILENAME 不存在"
        exit 1
    fi
}

# ---------- 阶段 3：选编译器 ----------
setup_toolchain() {
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
}

# ---------- 阶段 4：编译参数 ----------
setup_compile_flags() {
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
}

# ---------- 阶段 5：只有开了 sanitizer 才需要的环境变量 ----------
setup_sanitizer_env() {
    [ -n "$SANITIZER" ] || return 0

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
}

# ---------- 阶段 6a：编译 ----------
# 用临时文件收集 stderr，跑完统一 cat + awk，不用 `2> >(...)` 进程替换——
# 那种写法会派生一个不跟脚本主进程生命周期绑定的后台子进程，`set -e` 提前
# 结束脚本时它可能还没退出、还占着 stderr 管道的写端，让只认"管道 EOF"的
# 外部工具（IDE 运行面板/CI 日志采集）误判还在跑。
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

# ---------- 阶段 6b：运行 + 对拍 ----------
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

# ============================================================
# 主流程：一眼看出跑了哪几步，代码顺序就是实际执行顺序
# ============================================================
parse_args "$@"
check_source_exists
setup_toolchain
setup_compile_flags
setup_sanitizer_env

if compile; then
    if [ -n "${RUN:-}" ]; then
        run_bin
        exit $?
    fi
else
    echo "编译失败。"
    exit 1
fi
