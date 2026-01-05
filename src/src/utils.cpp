#include "utils.h"
#include <algorithm>
#include <vector>

std::vector<InstanceId> getIntersection(const std::vector<InstanceId>& v1, const std::vector<InstanceId>& v2) {
    // Make copies to sort
    std::vector<InstanceId> sorted_v1 = v1;
    std::vector<InstanceId> sorted_v2 = v2;

    std::sort(sorted_v1.begin(), sorted_v1.end());
    std::sort(sorted_v2.begin(), sorted_v2.end());

    std::vector<InstanceId> intersection;
    // Pre-allocate worst case size is technically min(v1, v2) but usually small
    intersection.reserve(std::min(sorted_v1.size(), sorted_v2.size()));

    std::set_intersection(sorted_v1.begin(), sorted_v1.end(),
                          sorted_v2.begin(), sorted_v2.end(),
                          std::back_inserter(intersection));

    return intersection;
}
}

#include <iostream>

std::vector<SpatialInstance> createSampleData() {
    std::vector<SpatialInstance> data;
    // Giả lập dữ liệu không gian: Type, ID, X, Y
    // Tạo một số pattern colocation tiềm năng gần nhau

    // Cụm 1: Có A, B, C gần nhau
    data.push_back({ "A", "A1", 1.0, 1.0 });
    data.push_back({ "B", "B1", 1.2, 1.1 });
    data.push_back({ "C", "C1", 1.1, 1.3 });

    // Cụm 2: Có A, B gần nhau (không có C)
    data.push_back({ "A", "A2", 5.0, 5.0 });
    data.push_back({ "B", "B2", 5.1, 5.2 });

    // Cụm 3: Có B, C gần nhau
    data.push_back({ "B", "B3", 10.0, 10.0 });
    data.push_back({ "C", "C2", 10.2, 10.1 });

    // Nhiễu (Noise): Đứng một mình
    data.push_back({ "A", "A3", 20.0, 20.0 });
    data.push_back({ "D", "D1", 50.0, 50.0 });

    return data;
}

void printPattern(const Colocation& pattern) {
    std::cout << "{ ";
    for (size_t i = 0; i < pattern.size(); ++i) {
        std::cout << pattern[i] << (i < pattern.size() - 1 ? ", " : " ");
    }
    std::cout << "}";
}

// ==================================================================================
// ALGORITHM 2 HELPERS IMPLEMENTATION
// ==================================================================================

void deleteTree(IDSNode* node) {
    if (!node) return;
    deleteTree(node->first_child);
    deleteTree(node->right_sibling);
    delete node;
}

void Initialize_Itree(IDSNode*& root) {
    if (root) {
        deleteTree(root);
    }
    // Root node ảo, không chứa instance cụ thể (id = -1 hoặc 0 tùy quy ước)
    root = new IDSNode("ROOT"); // Changed from -1 to string for InstanceId compatibility
}

IDSNode* AddHeadNode(IDSNode* root, InstanceId s) {
    // Head node là con trực tiếp của Root
    IDSNode* newNode = new IDSNode(s);
    newNode->parent = root; // Parent là Root
    
    // Thêm vào đầu danh sách con của Root
    newNode->right_sibling = root->first_child;
    root->first_child = newNode;

    return newNode;
}

std::vector<InstanceId> GetChildren(IDSNode* currNode, IDSNode* root, const NeighborhoodMgr& neighbors_mgr) {
    // Lemma 3:
    // 1. If currNode is a head-node (parent is root): Children are BNs(s)
    // 2. Otherwise (non-root): Children are BNs(s) ∩ RS(s)

    std::vector<InstanceId> bns = neighbors_mgr.getBigNeighbors(currNode->instance_id);

    if (currNode->parent == root) {
        // Case 1: Head-node
        return bns;
    } else {
        // Case 2: Non-head node
        // Lấy danh sách Right Siblings (RS) của currNode trên cây
        std::vector<InstanceId> rs;
        IDSNode* sibling = currNode->right_sibling;
        while (sibling != nullptr) {
            rs.push_back(sibling->instance_id);
            sibling = sibling->right_sibling;
        }
        
        // Sử dụng hàm tiện ích chung
        return getIntersection(bns, rs);
    }
}

std::vector<InstanceId> GetClique(IDSNode* currNode, IDSNode* root) {
    std::vector<InstanceId> clique;
    IDSNode* node = currNode;
    while (node != nullptr && node != root) {
        clique.push_back(node->instance_id);
        node = node->parent;
    }
    std::reverse(clique.begin(), clique.end());
    return clique;
}

void RemoveAncestors(IDSNode* currNode, IDSNode* root) {
    if (currNode == nullptr || currNode == root) return;

    IDSNode* parent = currNode->parent;
    
    // Prune currNode
    if (parent) {
        if (parent->first_child == currNode) {
            parent->first_child = currNode->right_sibling;
        } else {
            IDSNode* temp = parent->first_child;
            while (temp && temp->right_sibling != currNode) {
                temp = temp->right_sibling;
            }
            if (temp) {
                temp->right_sibling = currNode->right_sibling;
            }
        }
    }
    
    // Xóa khỏi bộ nhớ
    delete currNode;

    // Recursive check parent
    if (parent != root && parent->first_child == nullptr) {
        RemoveAncestors(parent, root);
    }
}

void AddNodes(IDSNode* currNode, const std::vector<InstanceId>& childrenIds) {
    IDSNode* lastChild = nullptr;
    
    for (const auto& id : childrenIds) {
        IDSNode* newNode = new IDSNode(id);
        newNode->parent = currNode;
        newNode->right_sibling = nullptr;

        if (currNode->first_child == nullptr) {
            currNode->first_child = newNode;
        } else {
            if (lastChild) {
                lastChild->right_sibling = newNode;
            }
        }
        lastChild = newNode;
    }
}
