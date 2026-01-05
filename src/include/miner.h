/**
 * @file miner.h
 * @brief Prevalent co-locations filtering (Algorithm 5)
 *
 * File này định nghĩa các cấu trúc và lớp để thực hiện thuật toán lọc
 * các colocation phổ biến, tuân thủ tuyệt đối tên hàm và thứ tự trong ảnh.
 */

#pragma once

#include "calculate_pi.h"
#include "types.h"
#include "candidate_generation.h" // Để sử dụng cấu trúc CHash
#include <vector>
#include <algorithm>
#include "ids_tree.h"

class PrevalentColocationMiner {
private:
	double minPrev;
	NeighborhoodMgr* neighborhoodMgr;
	std::vector<PatternKey> GetAllSubsets(const PatternKey& candidate);
	std::vector<PatternKey> GetDirectSub(const PatternKey& candidate);
public:
	std::unordered_map<PatternKey, double>mineColocations(
		CHashStructure& chash,
		double minPrev,
		const std::vector<SpatialInstance>& instances
	);
};