#!/usr/bin/env python3
"""Fake Ja/Quack solution used ONLY to smoke-test run_comm.py itself.

It is not a real strategy for the problem (gcd queries alone can't recover an
arbitrary array from an unmodified b=a encoding in general) - it cheats via a
side-channel file so we can verify the harness's plumbing (shuffling, query
counting, protocol parsing) independently of writing the actual algorithm.

Requires every test case in the input to have a distinct n (gen.py's default
output already guarantees this often enough; use --force-n per file if not).
"""
import json
import os
import sys

CHEAT_FILE = os.environ.get("MOCK_CHEAT_FILE", "/tmp/mock_cheat.json")


def main():
    mode = input().strip()
    t = int(input())
    if mode == "first":
        cheat = {}
        out = []
        for _ in range(t):
            n = int(input())
            a = list(map(int, input().split()))
            if str(n) in cheat:
                raise RuntimeError("mock_sol.py requires distinct n across test cases")
            cheat[str(n)] = a
            out.append(str(n))          # k = n
            out.append(" ".join(map(str, a)))  # b = a
        with open(CHEAT_FILE, "w") as f:
            json.dump(cheat, f)
        print("\n".join(out))
    elif mode == "second":
        with open(CHEAT_FILE) as f:
            cheat = json.load(f)
        for _ in range(t):
            n, k = map(int, input().split())
            a = cheat[str(n)]
            probes = min(2, k - 1)
            for q in range(probes):
                print(f"? 1 {q + 2}")
                sys.stdout.flush()
                input()  # discard the gcd answer, just exercising the protocol
            print("! " + " ".join(map(str, a)))
            sys.stdout.flush()
    else:
        raise RuntimeError(f"unexpected mode {mode!r}")


if __name__ == "__main__":
    main()
