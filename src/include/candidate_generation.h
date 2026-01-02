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

// Key cho bảng băm: Là tập hợp các Feature đã sắp xếp (ví dụ: {A, B, C})
using PatternKey = std::vector<FeatureType>;

// CHash Structure: Map từ PatternKey -> Map từ Feature -> Danh sách Cliques
// Cấu trúc này khớp với thuật toán: chash[newKey][f].AddInstances(cl)
// - chash[newKey] trả về std::map<FeatureType, std::vector<std::vector<SpatialInstance>>>
// - chash[newKey][f] trả về std::vector<std::vector<SpatialInstance>> (danh sách các cliques)
// - Mỗi cột feature f lưu trữ TẤT CẢ các cliques có chứa feature đó
using CHashStructure = std::map<PatternKey, std::map<FeatureType, std::vector<SpatialInstance>>>;

class CandidateGenerator{
private:
    PatternKey GetFeatures(const std::vector<SpatialInstance>& clique);
public:
    CHashStructure Candidate_generation(const std::vector<std::vector<SpatialInstance>>& cls);
};