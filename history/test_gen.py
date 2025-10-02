import random
import string

def generate_random_string(length):
    """生成指定长度的随机字符串"""
    letters = string.ascii_lowercase  # 包含大小写字母和数字
    return ''.join(random.choice(letters) for _ in range(length))

def generate_testcase():
    """生成包含三个随机字符串的测试用例"""
    # 随机生成字符串的长度
    len_s = random.randint(1, 100)
    len_t1 = random.randint(1, 100)
    len_t2 = random.randint(1, 100)
    
    # 生成随机字符串
    s = generate_random_string(len_s)
    t1 = generate_random_string(len_t1)
    t2 = generate_random_string(len_t2)
    
    return s, t1, t2

def high_score_string(s, t1, t2):
    score = 0
    ans = ""
    for i in range(len(s)):
        for j in range(i, len(s)):
            sub_str = s[i:j+1]
            max_prefix_t1_suffix = 0
            max_suffix_t2_prefix = 0
            
            # 计算子串前缀和t1后缀的最长匹配
            for k in range(1, len(sub_str) + 1):
                if k <= len(t1) and sub_str[:k] == t1[-k:]:
                    max_prefix_t1_suffix = k
            
            # 计算子串后缀和t2前缀的最长匹配
            for k in range(1, len(sub_str) + 1):
                if k <= len(t2) and sub_str[-k:] == t2[:k]:
                    max_suffix_t2_prefix = k
            
            current_score = max_prefix_t1_suffix + max_suffix_t2_prefix
            
            if current_score > score:
                score = current_score
                ans = sub_str
            elif current_score == score:
                if ans > sub_str:
                    ans = sub_str
    return score, ans

# 示例：生成一个测试用例
s, t1, t2 = generate_testcase()
score, ans = high_score_string(s, t1, t2)
print(f"Score: {score}, String: {ans}")