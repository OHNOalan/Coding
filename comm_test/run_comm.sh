#!/bin/bash
set -euo pipefail

# 用法(和 run.sh 保持一致的调用习惯，在仓库根目录执行):
#   ./comm_test/run_comm.sh <name> [in.txt] [-- 额外传给 run_comm.py 的参数]
# 例:
#   ./comm_test/run_comm.sh sol                     # 用 in.txt
#   ./comm_test/run_comm.sh sol my_case.txt -- -v    # 指定输入 + 打印交互过程
#
# in.txt 必须是 Run 1 (Ja) 收到的原始 jury 输入，即以 "first" 开头、
# 包含 t 和每组 n/a 的那份数据 —— 这题没有独立的"第二份输入文件"，
# Run 2 的 n/k 和每次 gcd 回复都是由本脚本现算的（取决于你的解自己
# 生成的 b），也没有独立的 out.txt：正确答案就是 in.txt 里的原始 a，
# 由 run_comm.py 在协议重放结束后逐位核对。

FILE=${1:?"用法: $0 <name> [in.txt] [-- extra run_comm.py args]"}
IN=${2:-"in.txt"}
shift $(( $# >= 2 ? 2 : 1 ))
if [ "${1:-}" = "--" ]; then shift; fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

if [ ! -f "$IN" ]; then
    echo "错误: $IN 不存在（应为 Run 1 的原始 jury 输入，以 'first' 开头）"
    exit 1
fi

echo ">>> 编译 $FILE.cpp（复用 run.sh，只编译不单独跑）..."
RUN= "$REPO_ROOT/run.sh" "$FILE"

echo ">>> 用 $IN 跑两次交互并核对最终答案..."
python3 "$SCRIPT_DIR/run_comm.py" --test "$IN" --sol "$REPO_ROOT/$FILE" "$@"
