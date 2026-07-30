#!/usr/bin/env python3
"""Generate a Run-1 ("first") input for the Ja/Quack communication problem.

Output format matches exactly what the jury feeds Ja on the first run:
    first
    t
    n_1
    a_1_1 ... a_1_n1
    n_2
    ...

Usage:
    python3 gen.py --seed 1 > test1.txt
    python3 gen.py --seed 1 --force-n 1000 > big_single_case.txt
    python3 gen.py --seed 1 --edge > edge_values.txt
"""
import argparse
import random


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--seed", type=int, default=0)
    ap.add_argument("--max-total-n", type=int, default=1000, help="sum of n over test cases (problem caps this at 1000)")
    ap.add_argument("--max-t", type=int, default=100)
    ap.add_argument("--min-a", type=int, default=1)
    ap.add_argument("--max-a", type=int, default=10**6)
    ap.add_argument("--force-n", type=int, default=None, help="single test case with exactly this n")
    ap.add_argument("--edge", action="store_true", help="bias values toward 1 / 10**6 / small primes to stress edge cases")
    args = ap.parse_args()

    rng = random.Random(args.seed)

    if args.force_n is not None:
        ns = [args.force_n]
    else:
        ns = []
        remaining = args.max_total_n
        t = rng.randint(1, args.max_t)
        for i in range(t):
            slots_left = t - i
            if remaining <= 0:
                break
            hi = max(1, remaining - (slots_left - 1))
            n = rng.randint(1, hi)
            ns.append(n)
            remaining -= n
        if not ns:
            ns = [1]

    lines = ["first", str(len(ns))]
    small_pool = [1, 2, 3, 4, 5, 7, args.max_a, args.max_a - 1, args.max_a // 2]
    for n in ns:
        lines.append(str(n))
        if args.edge:
            vals = [rng.choice(small_pool) for _ in range(n)]
        else:
            vals = [rng.randint(args.min_a, args.max_a) for _ in range(n)]
        lines.append(" ".join(map(str, vals)))

    print("\n".join(lines))


if __name__ == "__main__":
    main()
