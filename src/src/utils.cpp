#include "utils.h"
#include <algorithm>
#include <vector>
#include <iostream>

std::vector<instanceID> getIntersection(const std::vector<instanceID>& v1, const std::vector<instanceID>& v2) {
    // Make copies to sort
    std::vector<instanceID> sorted_v1 = v1;
    std::vector<instanceID> sorted_v2 = v2;

    std::sort(sorted_v1.begin(), sorted_v1.end());
    std::sort(sorted_v2.begin(), sorted_v2.end());

    std::vector<instanceID> intersection;
    // Pre-allocate worst case size is technically min(v1, v2) but usually small
    intersection.reserve(std::min(sorted_v1.size(), sorted_v2.size()));

    std::set_intersection(sorted_v1.begin(), sorted_v1.end(),
        sorted_v2.begin(), sorted_v2.end(),
        std::back_inserter(intersection));

    return intersection;
};

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

IDSNode* AddHeadNode(IDSNode* root, instanceID s) {
    // Head node là con trực tiếp của Root
    IDSNode* newNode = new IDSNode(s);
    newNode->parent = root; // Parent là Root
    
    // Thêm vào đầu danh sách con của Root
    newNode->right_sibling = root->first_child;
    root->first_child = newNode;

    return newNode;
}

std::vector<instanceID> GetChildren(IDSNode* currNode, IDSNode* root, const NeighborhoodMgr& neighbors_mgr) {
    // Lemma 3:
    // 1. If currNode is a head-node (parent is root): Children are BNs(s)
    // 2. Otherwise (non-root): Children are BNs(s) ∩ RS(s)

    std::vector<instanceID> bns = neighbors_mgr.getBigNeighbors(currNode->instance_id);

    if (currNode->parent == root) {
        // Case 1: Head-node
        return bns;
    } else {
        // Case 2: Non-head node
        // Lấy danh sách Right Siblings (RS) của currNode trên cây
        std::vector<instanceID> rs;
        IDSNode* sibling = currNode->right_sibling;
        while (sibling != nullptr) {
            rs.push_back(sibling->instance_id);
            sibling = sibling->right_sibling;
        }
        
        // Sử dụng hàm tiện ích chung
        return getIntersection(bns, rs);
    }
}

std::vector<instanceID> GetClique(IDSNode* currNode, IDSNode* root) {
    std::vector<instanceID> clique;
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

void AddNodes(IDSNode* currNode, const std::vector<instanceID>& childrenIds) {
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
