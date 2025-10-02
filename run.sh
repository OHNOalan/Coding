#!/bin/bash

# set -x

FILE=$1
IN="in.txt"
OPTIMIZE="-O1"

if [ -f "$2" ]; then
    IN=$2
fi

clear && \
    g++ -std=c++20 -fsanitize=address,undefined -g $FILE.cpp -o $FILE && \
    if [ -n "$DEBUG" ]; then
        # 使用lldb替代gdb进行调试（macOS推荐）
        DEBUG=$IN lldb $FILE
    else
        ./$FILE < $IN
    fi
