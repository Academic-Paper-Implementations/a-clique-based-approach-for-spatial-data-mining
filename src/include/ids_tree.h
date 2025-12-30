#pragma once

#include "types.h"
#include "neighborhood_mgr.h" // Để dùng NeighborList và input nbs
#include <vector>
#include <queue>
#include <unordered_map>


/**
 * @brief Đại diện cho một nút trong cây iTree
 * (Tương ứng với currNode, childrenNodes trong mã giả)
 */
struct Node {
    const SpatialInstance* instance; // Dữ liệu của node
    Node* parent;               // Con trỏ cha (để hỗ trợ RemoveAncestors)
    std::vector<Node*> children;// Danh sách con

    // Constructor
    Node(int id, IDSNode* p = nullptr) : instance_id(id), parent(p) {}

    // Destructor: Có thể đệ quy xóa con nếu cần, nhưng cẩn thận với RemoveAncestors
    ~Node() {
        for (auto child : children) {
            if (child) delete child;
        }
    }
    // Hàm tiện ích để xóa một node con cụ thể khỏi danh sách children (dùng khi cắt tỉa)
    void RemoveChild(Node* childToRemove) {
        auto it = std::remove(children.begin(), children.end(), childToRemove);
        children.erase(it, children.end());
    }
};

/**
 * @brief Cấu trúc cây iTree
 * Tương ứng biến iTree trong mã giả
 */
struct ITree {
    Node* Root; // Để gọi iTree.Root...

    ITree() {
        root = new Node(-1); // Root ảo
    }

    ~ITree() {
        delete Root; // Sẽ kích hoạt destructor đệ quy của IDSNode
    }

    // Dòng 4 Algo 2: Thêm node đầu tiên từ root
    Node* AddHeadNode(const SpatialInstance* s) {
        Node* newNode = new Node(s->id, root);
        root->children.push_back(newNode);
        return newNode;
    }
    // Dòng 13 Algo 2: Thêm danh sách con vào node hiện tại
    void AddNodes(Node* currNode, const std::vector<Node*>& childrenNodes) {
        for (Node* child : childrenNodes) {
            currNode->children.push_back(child);
            // Lưu ý: child->parent đã được gán khi tạo child
        }
    }
};

class IDSTree {
private:
    const std::vector<SpatialInstance>& S;
    const std::unordered_map<SpatialInstance, NeighborList>& nbs;

    // Biến kết quả
    std::vector<std::vector<SpatialInstance>> Cls;

    // Các hàm helper (Private)
    std::vector<Node*> GetChildren(Node* currNode);
    std::vector<SpatialInstance> GetClique(Node* leafNode);
    void RemoveAncestors(Node* currNode);

public:
    // Constructor
    IDSTree(const std::vector<SpatialInstance>& instances, const std::unordered_map<SpatialInstance, NeighborList>& neighbor_list);
    std::vector<std::vector<SpatialInstance>> Run();
}

