# CodeForce

Personal repo for competitive programming practice, plus some C++/Linux systems-programming
experiments. (中文版: [README_ch.md](README_ch.md))

## Layout

- `scratch/` — the throwaway draft for whatever problem is currently open: `main.cpp` /
  `std.cpp` / `in.txt` / `out.txt`. `std.cpp` is the local brute-force/reference solution used
  for stress-testing against `main.cpp`. All four have `git update-index --skip-worktree` set,
  so you can edit them freely and `git status` will never show changes. The cloud copy stays a
  minimal placeholder; that skip-worktree state is local only and doesn't survive a fresh clone —
  run `./bootstrap.sh` once on a new machine to set it back up (it also wires up the pre-commit
  hook, see below).
  Feel free to drop extra one-off files here too (`main2.cpp`, `std3.cpp`, ...) when you want a
  quick side-by-side comparison; once you're done, just `rm scratch/main2.cpp scratch/std3.cpp ...`
  for those extras. Don't delete `main.cpp`/`std.cpp`/`in.txt`/`out.txt` themselves — they're
  skip-worktree, so `git checkout` can't bring them back; you'd have to recreate them by hand and
  re-run `git update-index --skip-worktree`.
- `template/` — algorithm templates (DSU, segment tree, Fenwick tree, Treap, Dinic, Modint, etc.),
  meant to be copy-pasted straight into `scratch/main.cpp`.
- `history/` — archived submissions, organized by platform, filenames keep their original
  platform prefix (e.g. `cf2249A.cpp`, `abc466G.cpp`):
  - `history/codeforces/` — Codeforces rated rounds (Div/Educational Round, etc.).
  - `history/atcoder/` — AtCoder.
  - `history/gym/` — private Codeforces Gym/virtual-participation contests a friend set up
    (`Bround*` = Baozii Round, `Zround*` = Zrnstsr Round, `bcup*` = Baozii Cup) — not rated rounds.
  - `history/dmoj/` — DMOJ.
  - `history/waterloo/` — Waterloo local-contest practice: `w25fall*` = fall 2025,
    `w26local*` = winter 2026.
- `systems/` — C++/Linux systems-programming experiments (concurrency primitives, distributed
  experiments, UDP/CIDR networking experiments, etc.) — not competitive-programming code.
- `comm_test/` — local testing tool for two-program interactive ("run-twice" / communication)
  problems, see below.
- `.vscode/`, `.idea/` — personal editor state, local only, untracked from git (see `.gitignore`).
  `.githooks/` is different: it's shared project infrastructure (the pre-commit hook), stays
  tracked and ships with every clone.

## First clone

```
./bootstrap.sh
```

Points `core.hooksPath` at `.githooks/` (the pre-commit hook refuses to commit build artifacts
or files over 5MB; override with `ALLOW_BINARY=1`/`ALLOW_BIG=1` if one genuinely belongs), and
re-applies `skip-worktree` to `scratch/main.cpp`/`scratch/std.cpp`/`scratch/in.txt`/`scratch/out.txt`.

## Day-to-day practice: `run.sh`

```
./run.sh scratch/main [in.txt] [out.txt]
./run.sh -h                     # full usage (every env-var flag, its default, example combos)
./run.sh clean                  # wipe .build/, source files untouched
```

- Compiles `<name>.cpp` into `.build/<name>`, `-O0 -g` debug build by default. Every build
  artifact (binary, `.dSYM`, program output) lives in `.build/` — nothing spills into the repo
  root, and none of it is tracked by git.
- Environment-variable flags (full list in `./run.sh -h`):
  - `RUN=1` — run once against `in.txt` right after compiling, and diff against `out.txt`.
  - `SAN=1` — turn on ASan/UBSan.
  - `PERF=1` — switch to a `-O3` performance build (no sanitizer).
  - `DEBUG=1` — drop into `lldb` after compiling.
  - `INTERACTIVE=1` — skip file input, run interactively instead.
  - `NO_DIFF=1` — just show the program's output, skip the diff.
  - `LLVM=1` — emergency fallback: compile with the standalone Homebrew LLVM instead.

## Two-program interactive problems: `comm_test/`

Problems that run twice, where the two programs can't talk to each other directly and only
communicate through interactive queries (run-twice / communication problems), don't fit
`run.sh`'s "diff two files" model — see the header comment in `comm_test/run_comm.sh` for why.
Usage:

```
./comm_test/run_comm.sh scratch/sol [in.txt] [-- extra args, e.g. -v]
./comm_test/stress.sh .build/scratch/sol [iterations]   # loop random cases under stress
```

`in.txt` is the raw jury input fed to the first run (starts with `first`); `comm_test/gen.py`
can generate random/edge-case inputs.
