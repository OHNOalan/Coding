#!/bin/bash

# set -x

FILE=$1
IN="in.txt"
OPTIMIZE="-O1"

if [ -f "$2" ]; then
    IN=$2
fi

g++ -lcurl -std=c++20 -DTIMER -fsanitize=address,undefined -g $FILE.cpp -o $FILE && \
    if [ -n "$DEBUG" ]; then
        lldb $FILE
    elif [ -n "$INTERACTIVE" ]; then
        time ./$FILE
    else
        time ./$FILE < $IN
    fi
