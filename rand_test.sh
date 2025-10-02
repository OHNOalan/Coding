#!/bin/bash

# MAIN=$1
# STD=$2
# PYTHON_SCRIPT=$2

MAIN="./main"
STD="./std"
PYTHON_SCRIPT="./test_gen.py"  # 你的测试用例生成器

compile() {
    clear && g++ -std=c++20 -fsanitize=address,undefined -g -O1 $1.cpp -o $1
}

compile $MAIN
compile $STD

check_file_exist() {
    # 检查文件是否存在
    if [ ! -f "$1" ]; then
        echo "错误: 找不到Python脚本 $1"
        exit 1
    fi
}

check_file_exist $MAIN
check_file_exist $STD
check_file_exist $PYTHON_SCRIPT

execution_permission() {
    # 检查文件是否可执行
    if [ ! -x "$1" ]; then
        echo "警告: 二进制文件 $1 不可执行，尝试添加执行权限..."
        chmod +x "$1"
        if [ $? -ne 0 ]; then
            echo "错误: 无法为 $1 添加执行权限"
            exit 1
        fi
    fi
}

execution_permission $MAIN
execution_permission $STD

# ls $MAIN
# ls $STD

# 创建测试结果目录
RESULT_DIR="./test_results"
mkdir -p "$RESULT_DIR"

one_test() {
    # 生成时间戳用于唯一标识测试运行
    TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

    # 运行测试：Python生成测试用例并通过管道传给二进制程序
    # echo "开始测试... (时间戳: $TIMESTAMP)"
    # echo "生成的测试用例: $RESULT_DIR/input_$TIMESTAMP.txt"
    # echo "程序输出: $RESULT_DIR/output_$TIMESTAMP.txt"

    # 执行测试并保存输入输出
    python3 "$PYTHON_SCRIPT" > "$RESULT_DIR/input_$TIMESTAMP.txt"

    cat "$RESULT_DIR/input_$TIMESTAMP.txt" | $STD > "$RESULT_DIR/output_$TIMESTAMP_$(basename "$STD").txt"
    if [ $? -ne 0 ]; then
        echo "std失败"
        exit 1
    fi

    cat "$RESULT_DIR/input_$TIMESTAMP.txt" | $MAIN > "$RESULT_DIR/output_$TIMESTAMP_$(basename "$MAIN").txt"
    if [ $? -ne 0 ]; then
        echo "生成的测试用例: $RESULT_DIR/input_$TIMESTAMP.txt"
        echo "main失败"
        exit 1
    fi

    # 对比输出结果
    diff "$RESULT_DIR/output_$TIMESTAMP_$(basename "$MAIN").txt" "$RESULT_DIR/output_$TIMESTAMP_$(basename "$STD").txt"

    # 对比结果
    if [ $? -ne 0 ]; then
        echo "测试失败"
        exit 1
    fi

    rm "$RESULT_DIR/input_$TIMESTAMP.txt"
    rm "$RESULT_DIR/output_$TIMESTAMP_$(basename "$MAIN").txt"
    rm "$RESULT_DIR/output_$TIMESTAMP_$(basename "$STD").txt"
}

for i in {1..1000}
do
    one_test
done

echo "测试完成"