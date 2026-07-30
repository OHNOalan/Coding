#!/usr/bin/env python3
"""Local two-run judge for the Ja-the-Ghost / Quack-the-Duck communication problem.

Simulates the jury end to end against a single executable (same binary plays
both roles, exactly like the real judge):

  Run 1 (Ja):    batch stdin/stdout, no interaction mid-testcase.
  Run 2 (Quack): real interactive session, one query -> one gcd answer at a
                 time, with the test-case order reshuffled (the real judge
                 explicitly warns it may shuffle) so a solution that secretly
                 relies on run-1 order will get caught here too.

Usage:
    python3 run_comm.py --test test1.txt --sol ./sol
    python3 run_comm.py --test test1.txt --ja ./sol --quack ./sol --shuffle-seed 7 -v

Exit code 0 = accepted, 1 = wrong answer / protocol violation / crash / timeout.
"""
import argparse
import random
import select
import shlex
import subprocess
import sys
from math import gcd

TIMEOUT_S = 5.0


class Verdict(Exception):
    def __init__(self, msg):
        self.msg = msg


def read_tests(path):
    with open(path) as f:
        content = f.read()
    it = iter(content.split())
    assert next(it) == "first", "test file must start with 'first'"
    t = int(next(it))
    tests = []
    for _ in range(t):
        n = int(next(it))
        a = [int(next(it)) for _ in range(n)]
        tests.append((n, a))
    return tests


def k_bound(n):
    return -(-10 * n // 9) + 150  # ceil(10n/9) + 150


def query_bound(n):
    return 180 * n + 150


def run_ja(ja_cmd, test_path, tests, verbose):
    with open(test_path) as f:
        stdin_data = f.read()
    try:
        proc = subprocess.run(ja_cmd, input=stdin_data, capture_output=True, text=True, timeout=30)
    except subprocess.TimeoutExpired:
        raise Verdict("Ja run timed out (30s)")
    if proc.returncode != 0:
        sys.stderr.write(proc.stderr)
        raise Verdict(f"Ja exited with code {proc.returncode}")
    if verbose:
        sys.stderr.write(proc.stderr)

    it = iter(proc.stdout.split())
    results = []
    for idx, (n, a) in enumerate(tests):
        try:
            k = int(next(it))
        except StopIteration:
            raise Verdict(f"Ja output ended early at test {idx} (expected k)")
        kmax = k_bound(n)
        if not (1 <= k <= kmax):
            raise Verdict(f"test {idx}: k={k} violates 1 <= k <= {kmax} (n={n})")
        b = []
        for _ in range(k):
            try:
                v = int(next(it))
            except StopIteration:
                raise Verdict(f"test {idx}: Ja output ended early while reading b (got {len(b)}/{k})")
            if not (1 <= v <= 10**6):
                raise Verdict(f"test {idx}: b value {v} out of range [1, 1e6]")
            b.append(v)
        results.append((n, k, b, a))
    leftover = list(it)
    if leftover:
        raise Verdict(f"Ja printed {len(leftover)} extra token(s) after the last test case")
    return results


class Interactor:
    def __init__(self, cmd, verbose):
        self.proc = subprocess.Popen(
            cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=None, text=True, bufsize=1,
        )
        self.verbose = verbose

    def send(self, line):
        if self.verbose:
            sys.stderr.write(f">> {line}\n")
        self.proc.stdin.write(line + "\n")
        self.proc.stdin.flush()

    def recv(self, timeout=TIMEOUT_S):
        r, _, _ = select.select([self.proc.stdout], [], [], timeout)
        if not r:
            self.kill()
            raise Verdict(f"Quack produced no output within {timeout}s (hang?)")
        line = self.proc.stdout.readline()
        if line == "":
            self.kill()
            raise Verdict("Quack closed stdout / crashed mid-interaction")
        line = line.strip()
        if self.verbose:
            sys.stderr.write(f"<< {line}\n")
        return line

    def kill(self):
        try:
            self.proc.kill()
        except ProcessLookupError:
            pass

    def close(self):
        try:
            self.proc.stdin.close()
        except (BrokenPipeError, OSError):
            pass
        try:
            self.proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            self.kill()


def run_quack(quack_cmd, results, shuffle_seed, verbose):
    order = list(range(len(results)))
    rng = random.Random(shuffle_seed)
    rng.shuffle(order)
    if verbose:
        sys.stderr.write(f"shuffle seed={shuffle_seed} order={order}\n")

    inter = Interactor(quack_cmd, verbose)
    try:
        inter.send("second")
        inter.send(str(len(results)))
        for pos, orig_idx in enumerate(order):
            n, k, b, a = results[orig_idx]
            inter.send(f"{n} {k}")
            max_q = query_bound(n)
            used = 0
            while True:
                line = inter.recv()
                parts = line.split()
                if not parts:
                    continue
                if parts[0] == "?":
                    if len(parts) != 3:
                        inter.send("-1")
                        raise Verdict(f"test(orig={orig_idx}): malformed query '{line}'")
                    i, j = int(parts[1]), int(parts[2])
                    if not (1 <= i <= k and 1 <= j <= k) or i == j:
                        inter.send("-1")
                        raise Verdict(f"test(orig={orig_idx}): invalid query indices i={i} j={j} (k={k})")
                    used += 1
                    if used > max_q:
                        inter.send("-1")
                        raise Verdict(f"test(orig={orig_idx}, n={n}): exceeded query budget {max_q}")
                    inter.send(str(gcd(b[i - 1], b[j - 1])))
                elif parts[0] == "!":
                    guess = list(map(int, parts[1:]))
                    if len(guess) != n:
                        raise Verdict(f"test(orig={orig_idx}): answer has {len(guess)} values, expected {n}")
                    if guess != a:
                        raise Verdict(
                            f"test(orig={orig_idx}, n={n}): WRONG ANSWER\n"
                            f"  expected: {a}\n  got:      {guess}"
                        )
                    if verbose:
                        sys.stderr.write(f"test(orig={orig_idx}) OK, used {used}/{max_q} queries\n")
                    break
                else:
                    raise Verdict(f"test(orig={orig_idx}): unexpected line '{line}' (expected '?' or '!')")
    finally:
        inter.close()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--test", required=True, help="path to a gen.py-style 'first' input file")
    ap.add_argument("--sol", help="single executable used for both runs")
    ap.add_argument("--ja", help="executable for run 1 (defaults to --sol)")
    ap.add_argument("--quack", help="executable for run 2 (defaults to --sol)")
    ap.add_argument("--shuffle-seed", type=int, default=None)
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    ja_cmd = args.ja or args.sol
    quack_cmd = args.quack or args.sol
    if not ja_cmd or not quack_cmd:
        ap.error("provide --sol, or both --ja and --quack")

    tests = read_tests(args.test)
    seed = args.shuffle_seed if args.shuffle_seed is not None else random.randrange(2**31)

    try:
        results = run_ja(shlex.split(ja_cmd), args.test, tests, args.verbose)
        run_quack(shlex.split(quack_cmd), results, seed, args.verbose)
    except Verdict as v:
        print(f"WRONG ANSWER / PE: {v.msg}", file=sys.stderr)
        print(f"(reproduce with --shuffle-seed {seed})", file=sys.stderr)
        sys.exit(1)

    print(f"OK: {len(tests)} test case(s) accepted (shuffle seed {seed})")


if __name__ == "__main__":
    main()
