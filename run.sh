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

COMPILER="clang++"
CPPFLAGS_STDCXX_H='-I/usr/local/include'
STD_CPP='-std=c++26 -Wall -Wextra -pedantic-errors -Wfatal-errors -DLOCAL' # -Werror 
OPTIMIZATION='-O3'
PERF_OPT='-DNDEBUG -march=native -fomit-frame-pointer'
DEBUG_INFO='-g'
FRAME_POINTER='-fno-omit-frame-pointer'
# ASan + UBSan：默认关闭。当前 Homebrew LLVM 21 + macOS 26 上，
# -fsanitize=address,undefined 编译出的二进制会在 dyld 加载期卡死
# （sample 显示全部时间停在 dyld `start`，从未进入 main），并非代码或输入的问题。
# 需要时可用 SAN=1 ./run.sh ... 显式开启。
export ASAN_OPTIONS=detect_leaks=0
if [ -n "${SAN:-}" ]; then
    SANITIZER='-fsanitize=address,undefined'
    # SANITIZER=''
else
    SANITIZER=''
fi
STATIC="time -l"
DIFF=TRUE


# Homebrew LLVM：让 ASan/UBSan 报告里的栈带 file:line（需编译带 -g）
if [ -x "/opt/homebrew/opt/llvm/bin/llvm-symbolizer" ]; then
    export ASAN_SYMBOLIZER_PATH="/opt/homebrew/opt/llvm/bin/llvm-symbolizer"
    export UBSAN_SYMBOLIZER_PATH="${ASAN_SYMBOLIZER_PATH}"
elif [ -x "/usr/local/opt/llvm/bin/llvm-symbolizer" ]; then
    export ASAN_SYMBOLIZER_PATH="/usr/local/opt/llvm/bin/llvm-symbolizer"
    export UBSAN_SYMBOLIZER_PATH="${ASAN_SYMBOLIZER_PATH}"
fi
# 可按需覆盖：ASAN_OPTIONS='detect_leaks=1:halt_on_error=0' ./run.sh main
export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:symbolize=1}"

# macOS 下 LSan 常会误报 libobjc/libxpc/dyld 等系统运行时常驻分配；
# 默认启用 suppressions 过滤噪声，仍保留用户代码的 leak 检测。
if [ "$(uname -s)" = "Darwin" ] && [ -z "${LSAN_OPTIONS:-}" ] && [ -z "${NO_LSAN_SUPPRESS:-}" ]; then
    export LSAN_OPTIONS="suppressions=${SCRIPT_DIR}/lsan-macos.supp:print_suppressions=0"
fi

compile() {
    if [ -n "${PERF:-}" ]; then
        echo ">>> [性能模式] 编译 (-O3, 无 Sanitizer)..."
        {
            $STATIC $COMPILER $CPPFLAGS_STDCXX_H $STD_CPP -O3 $PERF_OPT "$FILENAME" -o "$BINARY"
        } 2> >( tee /dev/stderr | awk '/maximum resident set size/ {printf "编译峰值内存: %.2f MB\n", $1/1024/1024}')
    else
        if [ -n "$SANITIZER" ]; then
            echo ">>> [调试模式] 编译 (-O0 + ASan/UBSan + $DEBUG_INFO)..."
        else
            echo ">>> [调试模式] 编译 (-O0 + $DEBUG_INFO, 无 Sanitizer；SAN=1 可开启)..."
        fi
        {
            $STATIC $COMPILER $CPPFLAGS_STDCXX_H $STD_CPP -O0 $DEBUG_INFO $FRAME_POINTER $SANITIZER "$FILENAME" -o "$BINARY"
        } 2> >( tee /dev/stderr | awk '/maximum resident set size/ {printf "编译峰值内存: %.2f MB\n", $1/1024/1024}')
    fi
}

run_bin() {
    if [ -n "${DEBUG:-}" ]; then
        echo ">>> 启动 LLDB..."
        lldb "./$BINARY"
    elif [ -n "${INTERACTIVE:-}" ]; then
        echo ">>> 交互运行..."
        $STATIC "./$BINARY"
    else
        if [ -f "$IN" ]; then
            echo ">>> 输入: $IN"
            {
                $STATIC "./$BINARY" < "$IN" > "$ACTUAL_OUT"
            } 2> >(tee /dev/stderr | awk '/maximum resident set size/ {printf "运行峰值内存: %.2f MB\n", $1/1024/1024}')

            echo ">>> 程序输出已保存: $ACTUAL_OUT"
            if [ -n "${NO_DIFF:-}" ]; then
                echo ">>> 已开启 NO_DIFF，跳过 diff，直接输出程序结果："
                cat "$ACTUAL_OUT"
                return
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
        else
            echo ">>> 提示: 无 $IN，直接运行..."
            $STATIC "./$BINARY"
        fi
    fi
    echo "退出码: $?"
}

if compile; then
    # if [ -z "${PERF:-}" ] && [ -n "${ASAN_SYMBOLIZER_PATH:-}" ]; then
    #     echo ">>> ASAN_SYMBOLIZER_PATH=${ASAN_SYMBOLIZER_PATH}"
    # fi
    if [ -n "${RUN:-}" ]; then
        run_bin
    fi
else
    echo "编译失败。"
    exit 1
fi
