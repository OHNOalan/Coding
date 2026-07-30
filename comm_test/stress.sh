#!/bin/bash
# Repeatedly generate random cases and run them through run_comm.py until one fails.
# Usage: ./stress.sh ./sol [iterations]
set -uo pipefail

SOL=${1:?"usage: $0 <path-to-solution-binary> [iterations]"}
ITERS=${2:-200}
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CASE_FILE="$DIR/.stress_case.txt"

for i in $(seq 1 "$ITERS"); do
    seed=$RANDOM$RANDOM
    # mix small/edge-biased cases with generic random ones
    if (( i % 3 == 0 )); then
        python3 "$DIR/gen.py" --seed "$seed" --max-total-n 30 --max-t 5 --edge > "$CASE_FILE"
    elif (( i % 3 == 1 )); then
        python3 "$DIR/gen.py" --seed "$seed" --force-n 1 > "$CASE_FILE"
    else
        python3 "$DIR/gen.py" --seed "$seed" --max-total-n 1000 --max-t 100 > "$CASE_FILE"
    fi

    if ! python3 "$DIR/run_comm.py" --test "$CASE_FILE" --sol "$SOL" > /tmp/stress_out.txt 2>&1; then
        echo ">>> FAILED on iteration $i (gen seed $seed)"
        cat /tmp/stress_out.txt
        echo ">>> failing case saved at: $CASE_FILE"
        exit 1
    fi
    echo "iter $i ok"
done

echo "all $ITERS iterations passed"
rm -f "$CASE_FILE"
