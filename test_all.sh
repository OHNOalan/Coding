#!/bin/bash

# 测试脚本：编译 2026W/X.cpp 并测试所有 testdata/X.xxx.dat 的用例
# 验证结果与对应的 .diff 文件是否一致
# 输出最长的执行时间，或报告失败
# 用法: ./test_all.sh A   或   ./test_all.sh B   或   ./test_all.sh C

# 获取题号参数（默认为 A）
PROBLEM="${1:-A}"

PROG_PATH="2026W/${PROBLEM}"
TESTDATA_DIR="testdata"
PROG_FILE="${PROG_PATH}.cpp"

# 编译程序
echo "Compiling $PROG_FILE..."
g++ -std=c++26 -DTIMER -fsanitize=address,undefined -g "$PROG_FILE" -o "$PROG_PATH" 2>&1 | grep -v "^[[:space:]]*note:" || true

if [ ! -f "$PROG_PATH" ]; then
    echo "FAILED: Compilation failed"
    exit 1
fi

# 找到所有 X.*.dat 文件并排序
test_files=$(find "$TESTDATA_DIR" -name "${PROBLEM}.*.dat" -type f | sort -V)

if [ -z "$test_files" ]; then
    echo "FAILED: No test files found"
    exit 1
fi

max_time=0
test_count=0
failed=0

# 遍历每个测试文件
while IFS= read -r test_file; do
    test_name=$(basename "$test_file" .dat)
    expected_file="${TESTDATA_DIR}/${test_name}.diff"
    
    if [ ! -f "$expected_file" ]; then
        echo "FAILED: Missing expected output file: $expected_file"
        failed=1
        continue
    fi
    
    output_file="/tmp/${test_name}_output.txt"
    
    # 运行程序并计时
    start_time=$(date +%s%N)
    ./"$PROG_PATH" < "$test_file" > "$output_file" 2>/dev/null
    end_time=$(date +%s%N)
    
    # 计算执行时间（纳秒转秒）
    elapsed_ns=$((end_time - start_time))
    elapsed=$(printf "%.3f" "$(echo "scale=10; $elapsed_ns / 1000000000" | bc)")
    
    # 比较输出
    if ! diff -q "$output_file" "$expected_file" > /dev/null 2>&1; then
        echo "FAILED: Output mismatch for $test_name"
        # cat $test_file
        # cat $output_file
        # cat $expected_file
        failed=1
        rm -f "$output_file"
        break
    fi
    
    echo "$test_name: ${elapsed}s"
    
    # 更新最长时间
    max_time=$(echo "if ($elapsed > $max_time) $elapsed else $max_time" | bc)
    
    test_count=$((test_count + 1))
    rm -f "$output_file"
    
done <<< "$test_files"

# 报告结果
echo "================"
if [ $failed -eq 1 ]; then
    echo "FAILED"
    exit 1
else
    echo "All ${test_count} tests passed."
    echo "Maximum time: ${max_time}s"
    exit 0
fi
