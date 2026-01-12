#include "ids_tree.h"
#include "utils.h"
#include <algorithm>
#include <iostream>

IDSTree::IDSTree(const NeighborhoodMgr& neighbors_mgr, const std::vector<Instance>& instances)
    : neighbors_mgr_(neighbors_mgr), instances_(instances), root_(nullptr) {
}

IDSTree::~IDSTree() {
    deleteTree(root_);
}



void IDSTree::deleteTree(IDSNode* node) {
    if (!node) return;
    deleteTree(node->first_child);
    deleteTree(node->right_sibling);
    delete node;
}

// ==================================================================================
// ALGORITHM 2: IDS algorithm
// ==================================================================================
std::vector<std::vector<InstanceId>> IDSTree::run() {
    std::vector<std::vector<InstanceId>> Cls; // Result: list of I-cliques

    // ============== Step 1: Initialize_Itree ==============
    Initialize_Itree(root_);

    // ============== Step 2: For Each instance s In S Do ==============
    // Duyệt qua tất cả instances để tìm các clique bắt đầu bằng s
    // (Trong thực tế có thể tối ưu bằng cách chỉ duyệt các instance có BNs không rỗng)
    for (const auto& instance : instances_) {
        InstanceId s = instance.id;

        // ============== Step 3: queue = Initialize_queue() ==============
        std::queue<IDSNode*> queue;

        // ============== Step 4: headNode = iTree.Root.AddHeadNode(s) ==============
        IDSNode* headNode = AddHeadNode(root_, s);

        // ============== Step 5: queue.In(headNode) ==============
        queue.push(headNode);

        // ============== Step 6: While NotEmpty(queue) Do ==============
        while (!queue.empty()) {
            // ============== Step 7: currNode = queue.Out ==============
            IDSNode* currNode = queue.front();
            queue.pop();

            // ============== Step 8: childrenNodes = GetChildren(currNode) ==============
            std::vector<InstanceId> childrenIds = GetChildren(currNode, root_, neighbors_mgr_);

            // ============== Step 9: If IsEmpty(childrenNodes) Then ==============
            if (childrenIds.empty()) {
                // ============== Step 10: Cls.Add(GetClique(currNode)) ==============
                Cls.push_back(GetClique(currNode, root_));

                // ============== Step 11: RemoveAncestors(currNode) ==============
                RemoveAncestors(currNode, root_);
            } else {
                // ============== Step 12: Else ==============
                
                // ============== Step 13: iTree.AddNodes(currNode, childrenNodes) ==============
                AddNodes(currNode, childrenIds);

                // ============== Step 14: queue.In(childrenNodes) ==============
                // Add tất cả các children vừa tạo vào queue
                IDSNode* child = currNode->first_child;
                while (child != nullptr) {
                    queue.push(child);
                    child = child->right_sibling;
                }
            }
            // ============== Step 15: End If ==============
        }
        // ============== Step 16: End While ==============

        // Cleanup: Sau khi xử lý xong một instance head-node, ta có thể clear cây con của nó
        // hoặc để RemoveAncestors tự xử lý. Trong cài đặt này, RemoveAncestors đã xử lý pruning.
        // Tuy nhiên, để đảm bảo cây sạch sẽ cho vòng lặp sau (mặc dù thuật toán bảo tạo cây mới, 
        // nhưng ta có thể tái sử dụng root hoặc prune hết).
        // Với IDS, các cây con là độc lập, nhưng root node dùng chung.
        // Thực tế Step 11 đã prune dần dần.
    }
    // ============== Step 17: End For ==============

    return Cls;
}
