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
struct ITreeNode {
    const SpatialInstance* instance; // Dữ liệu của node
    ITreeNode* parent;               // Con trỏ cha (để hỗ trợ RemoveAncestors)
    std::vector<ITreeNode*> children;// Danh sách con

    ITreeNode(const SpatialInstance* inst = nullptr, ITreeNode* par = nullptr)
        : instance(inst), parent(par) {
    }
};

/**
 * @brief Cấu trúc Root để hỗ trợ cú pháp "iTree.Root.AddHeadNode(s)"
 */
struct ITreeRoot {
    std::vector<ITreeNode*> heads; // Các node cấp 1 (Head Nodes)

    /**
     * @brief Tạo một node head mới từ instance s
     * Tương ứng dòng 4: headNode = iTree.Root.AddHeadNode(s)
     */
    ITreeNode* AddHeadNode(const SpatialInstance* s);
};

/**
 * @brief Cấu trúc cây iTree
 * Tương ứng biến iTree trong mã giả
 */
struct ITree {
    ITreeRoot Root; // Để gọi iTree.Root...

    /**
     * @brief Thêm danh sách children vào currNode
     * Tương ứng dòng 13: iTree.AddNodes(currNode, childrenNodes)
     */
    void AddNodes(ITreeNode* currNode, const std::vector<ITreeNode*>& childrenNodes);

    // Hàm dọn dẹp bộ nhớ (không có trong mã giả nhưng cần cho C++)
    ~ITree();
};

/**
 * @brief Wrapper cho Queue để khớp cú pháp "queue.In" và "queue.Out"
 */
struct NodeQueue {
    std::queue<ITreeNode*> q;

    // Tương ứng: queue = Initialize_queue() (Constructor)
    NodeQueue() {}

    // Tương ứng: queue.In(node)
    void In(ITreeNode* node) { q.push(node); }

    // Overload cho vector (dòng 14: queue.In(childrenNodes))
    void In(const std::vector<ITreeNode*>& nodes) {
        for (auto* n : nodes) q.push(n);
    }

    // Tương ứng: currNode = queue.Out
    ITreeNode* Out() {
        ITreeNode* front = q.front();
        q.pop();
        return front;
    }

    // Tương ứng: NotEmpty(queue)
    bool NotEmpty() const { return !q.empty(); }
};



class IDSMiner {
private:
    // --- Input Data ---
    // nbs: neighbor list (bao gồm BNs của mỗi instance) - Dòng Input
    const std::unordered_map<instanceID, NeighborList>& nbs;

    // --- Internal State (Biến dùng trong thuật toán) ---
    ITree iTree;        // Dòng 1
    NodeQueue queue;    // Dòng 3

    // --- Output Data ---
    // Cls: list of I-cliques - Dòng Output
    std::vector<ColocationInstance> Cls;

    // ========================================================================
    // CÁC HÀM HELPERS (Tuân thủ tên trong Algorithm 2)
    // ========================================================================

    /**
     * @brief Khởi tạo cây
     * Dòng 1: iTree = Initialize_Itree()
     */
    ITree Initialize_Itree();

    /**
     * @brief Khởi tạo hàng đợi
     * Dòng 3: queue = Initialize_queue()
     * (Trong C++ có thể chỉ cần clear queue cũ)
     */
    NodeQueue Initialize_queue();

    /**
     * @brief Lấy các node con thỏa mãn Lemma 3
     * Dòng 8: childrenNodes = GetChildren(currNode)
     * * Logic chính: Kiểm tra các instance trong BNs của currNode có kết nối với tất cả ancestors của currNode hay không.
     */
    std::vector<ITreeNode*> GetChildren(ITreeNode* currNode);

    /**
     * @brief Kiểm tra danh sách node con có rỗng không
     * Dòng 9: IsEmpty(childrenNodes)
     */
    bool IsEmpty(const std::vector<ITreeNode*>& nodes) const;

    /**
     * @brief Trích xuất clique từ node lá
     * Dòng 10: GetClique(currNode)
     * * Logic: Duyệt ngược từ currNode lên root để lấy toàn bộ instance.
     */
    ColocationInstance GetClique(ITreeNode* currNode);

    /**
     * @brief Xóa node và tổ tiên của nó (Cắt tỉa cây)
     * Dòng 11: RemoveAncestors(currNode)
     */
    void RemoveAncestors(ITreeNode* currNode);

public:
    /**
     * @brief Constructor
     * @param neighborMap Map nbs đầu vào từ thuật toán 1
     */
    IDSMiner(const std::unordered_map<instanceID, NeighborList>& neighborMap);

    /**
     * @brief Hàm chạy chính (Main Loop)
     * Thực hiện từ dòng 1 đến dòng 17 trong ảnh.
     * @param S Tập hợp các instance (set of instances)
     * @return std::vector<ColocationInstance> Danh sách Cls kết quả
     */
    std::vector<ColocationInstance> run(const std::vector<SpatialInstance>& S);
};