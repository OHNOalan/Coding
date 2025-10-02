MOD = 10**9 + 7

def main():
    import sys
    input = sys.stdin.read().split()
    ptr = 0
    n = int(input[ptr])
    ptr += 1
    q = int(input[ptr])
    ptr += 1
    nums = list(map(int, input[ptr:ptr+n]))
    ptr += n
    queries = []
    for _ in range(q):
        li = int(input[ptr])
        ri = int(input[ptr+1])
        ki = int(input[ptr+2])
        vi = int(input[ptr+3])
        queries.append((li, ri, ki, vi))
        ptr += 4
    
    B = 300  # 阈值，可根据实际情况调整
    
    # 初始化：mul存储ki > B的查询的乘积因子，is_zero标记是否被置0
    mul = [1] * n
    is_zero = [False] * n
    
    # 初始化factors：对于每个d <= B，每个余数r < d，维护一个因子数组
    factors = {}
    for d in range(1, B + 1):
        factors[d] = []
        for r in range(d):
            # 计算该d和r对应的最大x（0-based）
            max_x = (n - 1 - r) // d if (n - 1 - r) >= 0 else -1
            # 因子数组长度为max_x + 2，确保x_max + 1不越界
            if max_x >= 0:
                factor_arr = [1] * (max_x + 2)
            else:
                factor_arr = [1]  # 不会被使用
            factors[d].append(factor_arr)
    
    # 处理查询
    for li, ri, ki, vi in queries:
        if vi == 0:
            # 暴力处理，置0
            idx = li
            while idx <= ri:
                if 0 <= idx < n and not is_zero[idx]:
                    is_zero[idx] = True
                    mul[idx] = 0
                idx += ki
            continue
        
        # vi != 0，计算逆元
        inv_vi = pow(vi, MOD - 2, MOD)
        
        if ki > B:
            # 暴力处理，乘以vi
            idx = li
            while idx <= ri:
                if 0 <= idx < n and not is_zero[idx]:
                    mul[idx] = (mul[idx] * vi) % MOD
                idx += ki
        else:
            d = ki
            if d not in factors:
                continue  # 不应该发生
            r0 = li % d
            if r0 >= d:
                continue  # 不可能
            
            # 计算x_min和x_max
            m = (li - r0) // d
            x_min = m
            # 确保x_min对应的i >=0
            x_min = max(x_min, 0)
            
            # 计算x_max的初始值
            x_max = (ri - r0) // d
            # 确保x_max对应的i <n
            max_possible_x = (n - 1 - r0) // d if (n - 1 - r0) >= 0 else -1
            x_max = min(x_max, max_possible_x)
            
            if x_min > x_max:
                continue  # 无有效x
            
            # 获取对应的因子数组
            factor_arr = factors[d][r0]
            # 检查x_min是否在因子数组范围内
            if x_min >= len(factor_arr):
                continue
            # 更新x_min位置
            # 更新x_min位置
            factor_arr[x_min] = (factor_arr[x_min] * vi) % MOD
            # 更新x_max +1位置
            x_next = x_max + 1
            if x_next < len(factor_arr):
                factor_arr[x_next] = (factor_arr[x_next] * inv_vi) % MOD
    
    # 预处理prefix数组：计算每个d和r的前缀乘积
    prefix = {}
    for d in factors:
        prefix[d] = []
        for r in range(d):
            factor_arr = factors[d][r]
            m = len(factor_arr)
            pre = [1] * m
            pre[0] = factor_arr[0]
            for x in range(1, m):
                pre[x] = (pre[x-1] * factor_arr[x]) % MOD
            prefix[d].append(pre)
    
    # 计算最终的异或结果
    result_xor = 0
    for i in range(n):
        if is_zero[i]:
            final = 0
        else:
            total = mul[i]
            # 乘以所有d <= B的前缀乘积
            for d in range(1, B + 1):
                if d not in prefix:
                    continue
                r = i % d
                if r >= len(prefix[d]):
                    continue
                pre_r = prefix[d][r]
                x = (i - r) // d
                if x < 0:
                    continue
                if x >= len(pre_r):
                    continue  # 超出范围，乘积为1
                total = (total * pre_r[x]) % MOD
            final = (nums[i] * total) % MOD
        result_xor ^= final
    
    print(result_xor)

if __name__ == "__main__":
    main()
