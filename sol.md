当然可以！以下是清晰、可复制的 Markdown 公式推导：

---

我们希望将以下更新

$$
F_v^{\text{new}}(d) = F_v^{\text{old}}(d) + \texttt{cur} \cdot F_u(d)
$$

用仿射形式高效实现，其中当前状态为

$$
F_v^{\text{old}}(d) = \texttt{base}_v[i] \cdot \texttt{ml}_v + \texttt{ad}_v, \quad i = \max(0, L_v - d - 1).
$$

令总方案数 $ T = F_u(\infty) $，并定义

$$
\texttt{tot} = \texttt{cur} \cdot T.
$$

将新增项拆分为常数部分与修正部分：

$$
\texttt{cur} \cdot F_u(d) = \texttt{tot} + \big( \texttt{cur} \cdot F_u(d) - \texttt{tot} \big).
$$

于是

$$
F_v^{\text{new}}(d) = F_v^{\text{old}}(d) + \texttt{tot} + \big( \texttt{cur} \cdot F_u(d) - \texttt{tot} \big).
$$

**第一步：处理常数项**  
直接更新加数：

$$
\texttt{ad}_v \gets \texttt{ad}_v + \texttt{tot},
$$

此时中间状态为

$$
F_v^{\text{temp}}(d) = \texttt{base}_v[i] \cdot \texttt{ml}_v + (\texttt{ad}_v + \texttt{tot}) = F_v^{\text{old}}(d) + \texttt{tot}.
$$

**第二步：修正小深度误差**  
我们需满足

$$
F_v^{\text{new}}(d) = F_v^{\text{temp}}(d) + \big( \texttt{cur} \cdot F_u(d) - \texttt{tot} \big).
$$

代入仿射形式：

$$
\texttt{base}'_v[i] \cdot \texttt{ml}_v + (\texttt{ad}_v + \texttt{tot}) = \texttt{base}_v[i] \cdot \texttt{ml}_v + (\texttt{ad}_v + \texttt{tot}) + \big( \texttt{cur} \cdot F_u(d) - \texttt{tot} \big).
$$

消去公共项，得

$$
\texttt{base}'_v[i] \cdot \texttt{ml}_v = \texttt{base}_v[i] \cdot \texttt{ml}_v + \big( \texttt{cur} \cdot F_u(d) - \texttt{tot} \big).
$$

两边同乘 $ \texttt{rev} = \texttt{ml}\_v^{-1} $，得到 base 的更新规则：

$$
\texttt{base}'_v[i] = \texttt{base}_v[i] + \big( \texttt{cur} \cdot F_u(d) - \texttt{tot} \big) \cdot \texttt{rev}.
$$

**因此，等价操作为：**

1. `ad_v += tot`
2. 对每个相关深度索引 `j`：
   ```cpp
   base_v[j] += (cur * F_u(j) - tot) * rev;
   ```

这完全等价于原始更新 $ F_v^{\text{new}}(d) = F_v^{\text{old}}(d) + \texttt{cur} \cdot F_u(d) $。
