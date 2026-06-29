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
STD_CPP='-std=c++26 -Wall -Wextra -Werror -pedantic-errors -Wfatal-errors'
OPTIMIZATION='-O3'
PERF_OPT='-DNDEBUG -march=native -fomit-frame-pointer'
DEBUG_INFO='-g -gsplit-dwarf'
FRAME_POINTER='-fno-omit-frame-pointer'
# ASan + UBSan；仅调试构建启用（PERF=1 时关闭）
SANITIZER='-fsanitize=address,undefined'
# SANITIZER=''
STATIC="time -l"


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
        echo ">>> [性能模式] 编译 ($OPTIMIZATION, 无 Sanitizer)..."
        $COMPILER $CPPFLAGS_STDCXX_H $STD_CPP $OPTIMIZATION $PERF_OPT "$FILENAME" -o "$BINARY"
    else
        echo ">>> [调试模式] 编译 ($OPTIMIZATION + ASan/UBSan + $DEBUG_INFO)..."
        $COMPILER $CPPFLAGS_STDCXX_H $STD_CPP $OPTIMIZATION $DEBUG_INFO $FRAME_POINTER $SANITIZER "$FILENAME" -o "$BINARY"
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
            } 2> >(tee /dev/stderr | awk '/maximum resident set size/ {printf "峰值内存: %.2f MB\n", $1/1024/1024}')

            echo ">>> 程序输出已保存: $ACTUAL_OUT"
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
        else
            echo ">>> 提示: 无 $IN，直接运行..."
            $STATIC "./$BINARY"
        fi
    fi
}

if compile; then
    if [ -z "${PERF:-}" ] && [ -n "${ASAN_SYMBOLIZER_PATH:-}" ]; then
        echo ">>> ASAN_SYMBOLIZER_PATH=${ASAN_SYMBOLIZER_PATH}"
    fi
    if [ -n "${RUN:-}" ]; then
        run_bin
    fi
else
    echo "编译失败。"
    exit 1
fi
