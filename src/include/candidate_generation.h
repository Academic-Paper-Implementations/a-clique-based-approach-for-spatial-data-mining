/**
 * @file candidate_generation.h
 * @brief Candidate Generation (Algorithm 4)
 *
 * File này định nghĩa cấu trúc CHash và lớp xử lý để tuân thủ tuyệt đối
 * các bước và tên hàm trong Algorithm 4.
 */

#pragma once

#include "types.h"
#include <vector>
#include <map>
#include <string>
#include "ids_tree.h"

 // Key cho bảng băm: Là tập hợp các Feature đã sắp xếp (ví dụ: {A, B, C}
using PatternKey = std::vector<FeatureType>;

// Cấu trúc lưu trữ dữ liệu cho một mẫu (Key) cụ thể
// Tương ứng với cấu trúc bên trong chash[newKey]
struct PatternInstanceTable {
    // Map từ Feature -> Danh sách các Instance ID (Cột)
    // Ví dụ: Feature A -> [A1, A10, A20], Feature B -> [B2, B11, B21]
    std::map<FeatureType, std::vector<SpatialInstance>> feature_columns;

    // Dòng 6: chash[newKey][f].AddInstances(cl)
    // Hàm này thêm instance ID thích hợp vào cột tương ứng với feature f
    void AddInstance(FeatureType f, SpatialInstance instance) {
        feature_columns[f].push_back(instance);
    }
};

using CHashStructure = std::map<PatternKey, PatternInstanceTable>;

class CandidateGenerator{
private:
    PatternKey GetFeatures(const std::vector<SpatialInstance>& clique);
public:
    CHashStructure Candidate_generation(const std::vector<std::vector<SpatialInstance>>& cls);
};