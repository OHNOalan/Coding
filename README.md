# CodeForce

竞赛刷题 + 部分 C++/Linux 系统编程实验的个人仓库。

## 目录结构

- `main.cpp` / `std.cpp` / `in.txt` / `out.txt` —— 每道新题的临时草稿。`std.cpp` 是本地对拍用的
  暴力/正确参考解。四个都已 `git update-index --skip-worktree`,内容随便改,`git status`
  永远看不见改动。云端只留一份极简占位符;这个状态是本地的,不会随 clone 同步,换机器
  跑一次 `./bootstrap.sh` 就会重新设置好(顺带把 pre-commit 钩子也接上,见下文)。
- `template/` —— 算法模板(DSU、线段树、Fenwick、Treap、Dinic、Modint 等),直接复制进 `main.cpp` 用。
- `history/` —— 按比赛/题号归档的历史提交(`cf2249A.cpp`、`Bround3D.cpp` 之类)。
- `comm_test/` —— 双程序交互题(run-twice / communication problem)本地测试工具,见下文。
- 根目录其余零散 `.cpp`/`.h` —— C++/Linux 系统编程实验(并发原语、MPI 分布式实验、UDP/CIDR
  网络实验、binary patching 笔记等),不是竞赛代码。

## 首次 clone

```
./bootstrap.sh
```

设置 `core.hooksPath` 指向 `.githooks/`(pre-commit 会拒绝提交编译产物/超过 5MB 的文件,
误加的话用 `ALLOW_BINARY=1`/`ALLOW_BIG=1` 覆盖),并重新给 `main.cpp`/`std.cpp`/`in.txt`/
`out.txt` 打上 `skip-worktree`。

## 日常刷题:`run.sh`

```
./run.sh <name> [in.txt] [out.txt]
```

- 编译 `<name>.cpp` 为可执行文件 `<name>`,默认 `-O0 -g` 调试构建。
- 环境变量开关:
  - `RUN=1` —— 编译后立即用 `in.txt` 跑一次,并和 `out.txt` diff。
  - `SAN=1` —— 开启 ASan/UBSan。
  - `PERF=1` —— 换成 `-O3` 性能构建(不带 sanitizer)。
  - `DEBUG=1` —— 编译后进 `lldb`。
  - `INTERACTIVE=1` —— 不读文件,直接交互式跑。
  - `NO_DIFF=1` —— 只看程序输出,不 diff。

## 双程序交互题:`comm_test/`

跑两次、两个程序间不能直接通信、只能靠交互 query 的题(run-twice / communication
problem)不能用 `run.sh` 的"两个文件 diff"模型——具体原因见 `comm_test/run_comm.sh`
文件头注释。用法:

```
./comm_test/run_comm.sh <name> [in.txt] [-- 额外参数，如 -v]
./comm_test/stress.sh ./<name> [iterations]   # 循环随机测例压测
```

`in.txt` 是 jury 喂给第一次运行的原始输入(以 `first` 开头),`comm_test/gen.py`
可以生成随机/边界测例。
