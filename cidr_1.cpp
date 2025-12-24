#include <array>
#include <assert.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>


constexpr int B = 31;

// 1. Define the data source ONCE
// Format: X(EnumName, IntegerValue)
#define ALLOWENCE_VALUES                                                                                               \
    X(NONE, 0)                                                                                                         \
    X(BLOCK, 1)                                                                                                        \
    X(ALLOW, 2)                                                                                                        \
    X(MIXED, 3)

// 2. Generate the Enum Class
enum class ALLOWENCE {
#define X(name, val) name = val,
    ALLOWENCE_VALUES
#undef X
};

// 3. Generate the String Conversion (operator<<)
std::ostream &operator<<(std::ostream &os, ALLOWENCE a) {
    switch (a) {
#define X(name, val)                                                                                                   \
    case ALLOWENCE::name:                                                                                              \
        return os << #name;
        ALLOWENCE_VALUES
#undef X
        default:
            return os << "UNKNOWN(" << static_cast<int>(a) << ")";
    }
}

struct Rule {
    std::string cidr;
    ALLOWENCE indicator;
    friend std::ostream &operator<<(std::ostream &os, Rule &obj);
};

std::ostream &operator<<(std::ostream &os, Rule &rule) {
    os << "Rule: " << rule.cidr << ", ALLOWENCE: " << rule.indicator << std::endl;
    return os;
}

struct Node {
    std::array<std::shared_ptr<Node>, 2> child{nullptr, nullptr};
    std::array<bool, 2> rules{false, false}; // rules[0] = fully allowed, rules[1] = fully blocked
    Node() {}
};

unsigned ipToU(std::string_view ips) {
    std::stringstream ss(static_cast<std::string>(ips));
    std::string t;
    unsigned ip = 0;
    for (std::string s; std::getline(ss, s, '.');) {
        uint8_t x = std::stoi(s);
        ip = (ip << 8) + x;
    }
    return ip;
}

// assume CIDR is valid: xxx.xxx.xxx.xxx/xx
std::pair<unsigned, unsigned> extractCIDR(std::string_view cidr) {
    auto lst = cidr.find('/');
    assert(lst != -1);
    std::string_view ips = cidr.substr(0, lst);
    std::string_view len = cidr.substr(lst + 1);
    return {ipToU(ips), std::stoi(static_cast<std::string>(len))};
}

int main() {
    std::vector<Rule> rules{{"0.0.2.126/31", ALLOWENCE::ALLOW}, {"0.0.2.127/32", ALLOWENCE::BLOCK}};
    std::shared_ptr<Node> root = std::make_shared<Node>();
    auto build = [root](unsigned ip, unsigned len, ALLOWENCE indicator) -> void {
        auto cur = root;
        // std::stack<std::shared_ptr<Node>> stk;
        for (int i = 0, b = B; i < len; i++, b--) {
            int x = (ip >> b) & 1;
            auto &child = cur->child[x];
            if (child == nullptr) {
                child = std::make_shared<Node>();
            }
            // stk.push(cur);
            cur = child;
        }
        cur->rules[0] |= static_cast<int>(indicator) & 1;
        cur->rules[1] |= (static_cast<int>(indicator) >> 1) & 1;
        // while (!stk.empty()) {
        //     auto node = stk.top();
        //     stk.pop();
        //     for (int r: {0, 1}) {
        //         cur->rules[r] |=
        //                 (cur->child[0] ? cur->child[0]->rules[r] : 0) & (cur->child[1] ? cur->child[1]->rules[r] :
        //                 0);
        //     }
        // }
    };
    for (auto &rule: rules) {
        std::cout << rule;
        auto [ip, len] = extractCIDR(rule.cidr);
        std::cout << "ip: " << std::bitset<32>(ip) << ", len: " << len << std::endl;
        build(ip, len, rule.indicator);
    }
    auto check = [&root](unsigned ip) -> ALLOWENCE {
        auto cur = root;
        int allowence = 0;
        for (int b = B; b >= 0; b--) {
            int x = (ip >> b) & 1;
            auto &child = cur->child[x];
            if (child == nullptr) {
                break;
            }
            cur = child;
            if (cur->rules[0]) allowence |= 1;
            if (cur->rules[1]) allowence |= 2;
            if (allowence == 3) {
                return ALLOWENCE::MIXED;
            }
        }
        return static_cast<ALLOWENCE>(allowence);
    };
    std::cout << check(ipToU("0.0.0.127")) << std::endl;
    std::cout << check(ipToU("0.0.2.127")) << std::endl;
    std::cout << check(ipToU("0.0.2.126")) << std::endl;
    std::cout << check(ipToU("0.0.2.128")) << std::endl;
    return 0;
}
