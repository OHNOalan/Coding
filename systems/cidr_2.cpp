#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum class Status { NONE, ALLOW, BLOCK, MIXED };

std::string statusToString(Status s) {
    switch (s) {
        case Status::NONE:
            return "NONE";
        case Status::ALLOW:
            return "ALLOW";
        case Status::BLOCK:
            return "BLOCK";
        case Status::MIXED:
            return "MIXED";
    }
    return "?";
}

struct Node {
    Node *children[2] = {nullptr, nullptr};
    Status rule = Status::NONE; // Explicit rule set by user input
    Status subtreeStatus = Status::NONE; // Aggregated status (Allow, Block, or Mixed)
    ~Node() {
        delete children[0];
        delete children[1];
    }
};

class Firewall {
    Node *root;

    unsigned ipToU(const std::string &ipStr) {
        unsigned ip = 0;
        std::stringstream ss(ipStr);
        std::string segment;
        while (std::getline(ss, segment, '.')) {
            ip = (ip << 8) | std::stoi(segment);
        }
        return ip;
    }

    std::pair<unsigned, int> parseCIDR(const std::string &cidr) {
        auto slashPos = cidr.find('/');
        std::string ipPart = cidr.substr(0, slashPos);
        int len = std::stoi(cidr.substr(slashPos + 1));
        return {ipToU(ipPart), len};
    }

public:
    Firewall() { root = new Node(); }

    ~Firewall() { delete root; }

    void addRule(const std::string &cidr, Status action) {
        auto [ip, len] = parseCIDR(cidr);
        Node *curr = root;
        for (int i = 31; i >= 32 - len; --i) {
            int bit = (ip >> i) & 1;
            if (!curr->children[bit]) {
                curr->children[bit] = new Node();
            }
            curr = curr->children[bit];
        }
        curr->rule = action;
    }

    // Post-Order Traversal to populate subtreeStatus
    Status optimize(Node *node = nullptr, Status parentStatus = Status::BLOCK) {
        if (!node) node = root;

        // 1. Determine effective status for this node
        Status effectiveStatus = (node->rule != Status::NONE) ? node->rule : parentStatus;

        // 2. Recurse
        Status leftRes = effectiveStatus;
        Status rightRes = effectiveStatus;

        if (node->children[0]) leftRes = optimize(node->children[0], effectiveStatus);
        if (node->children[1]) rightRes = optimize(node->children[1], effectiveStatus);

        // 3. Aggregate
        if (leftRes == rightRes) {
            node->subtreeStatus = leftRes;
        } else {
            node->subtreeStatus = Status::MIXED;
        }
        return node->subtreeStatus;
    }

    // === FIX IS HERE ===
    Status checkRange(const std::string &cidr) {
        auto [ip, len] = parseCIDR(cidr);
        Node *curr = root;

        // Track the rule inherited from above
        Status effectiveRule = Status::BLOCK; // Default deny

        for (int i = 31; i >= 32 - len; --i) {
            // Update the effective rule as we descend
            if (curr->rule != Status::NONE) {
                effectiveRule = curr->rule;
            }

            int bit = (ip >> i) & 1;
            if (!curr->children[bit]) {
                // If the node implies by this CIDR doesn't exist,
                // it is fully covered by the last effective rule.
                return effectiveRule;
            }
            curr = curr->children[bit];
        }

        // If we reach here, the node exists. Return its aggregated status.
        return curr->subtreeStatus;
    }

    Status checkIP(const std::string &ipStr) {
        // checkIP is actually just checkRange with /32, but for clarity:
        return checkRange(ipStr + "/32");
    }
};

int main() {
    Firewall fw;

    // 10.0.0.0/8   -> ALLOW
    // 10.1.0.0/16  -> BLOCK
    fw.addRule("10.0.0.0/8", Status::ALLOW);
    fw.addRule("10.1.0.0/16", Status::BLOCK);

    fw.optimize();

    std::cout << "--- IP Checks ---" << std::endl;
    std::cout << "10.0.0.5:      " << statusToString(fw.checkIP("10.0.0.5")) << " (Expect ALLOW)" << std::endl;
    std::cout << "10.1.0.5:      " << statusToString(fw.checkIP("10.1.0.5")) << " (Expect BLOCK)" << std::endl;

    std::cout << "\n--- Range Checks ---" << std::endl;

    // 1. Root /8 contains both Allow and Block -> MIXED
    std::cout << "10.0.0.0/8:    " << statusToString(fw.checkRange("10.0.0.0/8")) << " (Expect MIXED)" << std::endl;

    // 2. Specific range 10.2... falls inside 10/8 (Allow) and has no sub-rules -> ALLOW
    std::cout << "10.2.0.0/16:   " << statusToString(fw.checkRange("10.2.0.0/16")) << " (Expect ALLOW)" << std::endl;

    // 3. Specific range 10.1... is explicitly blocked -> BLOCK
    std::cout << "10.1.0.0/16:   " << statusToString(fw.checkRange("10.1.0.0/16")) << " (Expect BLOCK)" << std::endl;

    return 0;
}
