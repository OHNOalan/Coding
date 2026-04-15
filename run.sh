#!/bin/bash

# 参数处理：第一个参数是文件名（不带.cpp），第二个可选是输入文件
FILE=$1
IN=${2:-"in.txt"}
OUT=${3:-"out.txt"}

FILENAME="$FILE.cpp"
BINARY="$FILE"

# 检查文件是否存在
if [ ! -f "$FILE.cpp" ]; then
    echo "错误: $FILE.cpp 不存在"
    exit 1
fi

clang++ -std=c++23 -O0 -g -fno-omit-frame-pointer main.cpp -o main

# GNU bits/stdc++.h 若在 /usr/local/include/bits/，须加入该目录（Apple clang 默认不搜这里）
COMPILER="clang++"
CPPFLAGS_STDCXX_H='-I/usr/local/include'
STD_CPP='-std=c++26'
OPTIMIZATION='-O0'
SANITIZER='-fsanitize=address,undefined'
DEBUG_INFO='-g'

compile() {
    if [ -n "$PERF" ]; then
        echo ">>> [性能模式] 正在编译 ($OPTIMIZATION, 无 Sanitizer)..."
        # 移除 -g 和 -fsanitize 以获取极致性能
        $COMPILER $CPPFLAGS_STDCXX_H $STD_CPP $OPTIMIZATION $FILENAME -o $BINARY
    else
        echo ">>> [调试模式] 正在编译 ($OPTIMIZATION + Sanitizer + Debug Info)..."
        # 注意：在调试模式下，-O3 可能会让 lldb 的变量跟踪变得困难，
        # 如果需要精准调试，可以将 O3 改为 Og 或 O0
        $COMPILER $CPPFLAGS_STDCXX_H $STD_CPP $OPTIMIZATION $DEBUG_INFO $SANITIZER $FILENAME -o $BINARY
    fi
    
    # 返回上一个命令的执行结果
    return $?
}

run() {
    if [ -n "$DEBUG" ]; then
        echo ">>> 启动 LLDB 调试器..."
        lldb "./$FILE"
    elif [ -n "$INTERACTIVE" ]; then
        echo ">>> 手动输入模式运行..."
        time "./$FILE"
    else
        if [ -f "$IN" ]; then
            echo ">>> 使用输入文件 $IN 运行..."
            time "./$FILE" < "$IN"
        else
            echo "提示: 输入文件 $IN 不存在，切换到交互模式..."
            time "./$FILE"
        fi
    fi
}

# 执行逻辑：如果编译成功，则运行
if compile; then
    run
else
    echo "编译失败，请检查代码。"
    exit 1
fi