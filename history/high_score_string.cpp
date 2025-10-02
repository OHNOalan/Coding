#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>

// 计算子串前缀和t1后缀的最长匹配
int max_prefix_t1_suffix(const std::string& sub_str, const std::string& t1) {
    int max_match = 0;
    for (int k = 1; k <= sub_str.size() && k <= t1.size(); ++k) {
        if (sub_str.substr(0, k) == t1.substr(t1.size() - k)) {
            max_match = k;
        }
    }
    return max_match;
}

// 计算子串后缀和t2前缀的最长匹配
int max_suffix_t2_prefix(const std::string& sub_str, const std::string& t2) {
    int max_match = 0;
    for (int k = 1; k <= sub_str.size() && k <= t2.size(); ++k) {
        if (sub_str.substr(sub_str.size() - k) == t2.substr(0, k)) {
            max_match = k;
        }
    }
    return max_match;
}

// 计算高分字符串
std::pair<int, std::string> high_score_string(const std::string& s, const std::string& t1, const std::string& t2) {
    int score = 0;
    std::string ans = "";
    for (size_t i = 0; i < s.size(); ++i) {
        for (size_t j = i; j < s.size(); ++j) {
            std::string sub_str = s.substr(i, j - i + 1);
            int prefix_match = max_prefix_t1_suffix(sub_str, t1);
            int suffix_match = max_suffix_t2_prefix(sub_str, t2);
            int current_score = prefix_match + suffix_match;
            
            if (current_score > score) {
                score = current_score;
                ans = sub_str;
            } else if (current_score == score) {
                if (ans > sub_str) {
                    ans = sub_str;
                }
            }
        }
    }
    return {score, ans};
}

// 生成随机字符串
std::string generate_random_string(int length) {
    const std::string letters = "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += letters[rand() % letters.size()];
    }
    return result;
}

// 生成测试用例
void generate_testcase(std::string& s, std::string& t1, std::string& t2) {
    int len_s = rand() % 100 + 1;
    int len_t1 = rand() % 100 + 1;
    int len_t2 = rand() % 100 + 1;
    s = generate_random_string(len_s);
    t1 = generate_random_string(len_t1);
    t2 = generate_random_string(len_t2);
}

// 调用Python脚本验证结果
bool verify_with_python(const std::string& s, const std::string& t1, const std::string& t2) {
    char command[256];
    snprintf(command, sizeof(command), "python3 /Users/alanlee/Desktop/CodeForce/test_gen.py \"%s\" \"%s\" \"%s\"", s.c_str(), t1.c_str(), t2.c_str());
    FILE* pipe = popen(command, "r");
    if (!pipe) {
        std::cerr << "Failed to run command" << std::endl;
        return false;
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    // 解析Python脚本的输出
    int expected_score;
    std::string expected_ans;
    sscanf(result.c_str(), "Score: %d, String: %s", &expected_score, buffer);
    expected_ans = buffer;

    // 获取C++的结果
    auto cpp_result = high_score_string(s, t1, t2);
    int cpp_score = cpp_result.first;
    std::string cpp_ans = cpp_result.second;

    // 比较结果
    if (cpp_score == expected_score && cpp_ans == expected_ans) {
        return true;
    } else {
        std::cerr << "Mismatch: C++ Score: " << cpp_score << ", C++ String: " << cpp_ans
                  << " | Python Score: " << expected_score << ", Python String: " << expected_ans << std::endl;
        return false;
    }
}

int main() {
    srand(time(0)); // 初始化随机数生成器

    // 测试用例数量
    const int num_tests = 100;

    for (int i = 0; i < num_tests; ++i) {
        std::string s, t1, t2;
        generate_testcase(s, t1, t2);
        if (!verify_with_python(s, t1, t2)) {
            std::cerr << "Test case " << i + 1 << " failed" << std::endl;
            return 1;
        }
    }

    std::cout << "All test cases passed!" << std::endl;
    return 0;
}