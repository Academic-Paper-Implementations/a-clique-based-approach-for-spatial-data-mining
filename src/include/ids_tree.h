#ifndef IDS_TREE_H
#define IDS_TREE_H

#include <vector>
#include <queue>
#include <set>
#include "types.h"
#include "neighborhood_mgr.h"



class IDSTree 
public:
    // Constructor nhận vào dữ liệu cần thiết:
    // - neighbors_mgr: Quản lý thông tin láng giềng (neighborhood list, BNs, SNs)
    // - instances: Tập hợp tất cả các instances (S)
    IDSTree(const NeighborhoodMgr& neighbors_mgr, const std::vector<Instance>& instances);
    
    ~IDSTree();

    // Hàm chính để thực thi thuật toán IDS (Algorithm 2)
    // Trả về danh sách các I-cliques tìm được (mỗi clique là một vector các InstanceId)
    std::vector<std::vector<InstanceId>> run();

private:
    const NeighborhoodMgr& neighbors_mgr_;
    const std::vector<Instance>& instances_;
    IDSNode* root_;

#endif // IDS_TREE_H
