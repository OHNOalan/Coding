# CodeForce

竞赛刷题 + 部分 C++/Linux 系统编程实验的个人仓库。

## 目录结构

- `main.cpp` / `std.cpp` / `in.txt` / `out.txt` —— 每道新题的临时草稿。`std.cpp` 是本地对拍用的
  暴力/正确参考解。四个都已 `git update-index --skip-worktree`,内容随便改,`git status`
  永远看不见改动。云端只留一份极简占位符;这个状态是本地的,不会随 clone 同步,换机器
  跑一次 `./bootstrap.sh` 就会重新设置好(顺带把 pre-commit 钩子也接上,见下文)。
- `template/` —— 算法模板(DSU、线段树、Fenwick、Treap、Dinic、Modint 等),直接复制进 `main.cpp` 用。
- `history/` —— 按平台归档的历史提交,文件名保留原平台前缀(`cf2249A.cpp`、`abc466G.cpp` 之类):
  - `history/codeforces/` —— Codeforces 正式排名赛(Div/Edu Round 等)。
  - `history/atcoder/` —— AtCoder。
  - `history/gym/` —— 朋友私建的 Codeforces Gym/VP 赛题(`Bround*`=Baozii Round、`Zround*`=
    Zrnstsr Round、`bcup*`=Baozii Cup),不是正式排名赛。
  - `history/dmoj/` —— DMOJ。
- `systems/` —— C++/Linux 系统编程实验(并发原语、分布式实验、UDP/CIDR 网络实验等),不是竞赛代码。
- `comm_test/` —— 双程序交互题(run-twice / communication problem)本地测试工具,见下文。

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
./run.sh -h            # 完整用法说明(所有环境变量开关、默认值、组合示例)
./run.sh clean         # 清空 .build/，源码不受影响
```

- 编译 `<name>.cpp` 为可执行文件 `.build/<name>`,默认 `-O0 -g` 调试构建。
  所有编译产物(二进制、`.dSYM`、程序输出)都在 `.build/` 里,不会散落在仓库根目录,
  也不会被 git 跟踪。
- 环境变量开关(完整列表见 `./run.sh -h`):
  - `RUN=1` —— 编译后立即用 `in.txt` 跑一次,并和 `out.txt` diff。
  - `SAN=1` —— 开启 ASan/UBSan。
  - `PERF=1` —— 换成 `-O3` 性能构建(不带 sanitizer)。
  - `DEBUG=1` —— 编译后进 `lldb`。
  - `INTERACTIVE=1` —— 不读文件,直接交互式跑。
  - `NO_DIFF=1` —— 只看程序输出,不 diff。
  - `LLVM=1` —— 应急开关,编译器切到 Homebrew 装的独立 LLVM。

## 双程序交互题:`comm_test/`

跑两次、两个程序间不能直接通信、只能靠交互 query 的题(run-twice / communication
problem)不能用 `run.sh` 的"两个文件 diff"模型——具体原因见 `comm_test/run_comm.sh`
文件头注释。用法:

```
./comm_test/run_comm.sh <name> [in.txt] [-- 额外参数，如 -v]
./comm_test/stress.sh .build/<name> [iterations]   # 循环随机测例压测
```

`in.txt` 是 jury 喂给第一次运行的原始输入(以 `first` 开头),`comm_test/gen.py`
可以生成随机/边界测例。
