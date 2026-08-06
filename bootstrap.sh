#!/bin/bash
# One-time (idempotent) local setup after cloning this repo.
set -euo pipefail
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

chmod +x .githooks/pre-commit
git config core.hooksPath .githooks
echo "hooks: core.hooksPath -> .githooks (pre-commit blocks binaries/oversized files)"

SCRATCH_FILES=(scratch/main.cpp scratch/std.cpp scratch/in.txt scratch/out.txt)
git update-index --skip-worktree "${SCRATCH_FILES[@]}"
echo "skip-worktree set on: ${SCRATCH_FILES[*]}"
