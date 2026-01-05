/**
 * @file calculate_pi.h
 * @brief Calculate PI value (Algorithm 6)
 *
 * File này định nghĩa lớp tính toán PI, tuân thủ tuyệt đối các bước,
 * tên biến và logic trong Algorithm 6.
 */

#pragma once

#include "types.h"
#include "candidate_generation.h" // Để sử dụng CHash và FeatureBucket
#include <vector>
#include <map>
#include <unordered_set>
#include <algorithm>

class CalculatePI {
public:
	static std::vector<PatternKey> GetSuperSets(
		const CHashStructure& chash,
		const PatternKey& candidate
	);

	static double calculatePI(
		const CHashStructure& chash,
		const PatternKey& candidate,
		const std::map<FeatureType, int>& globalFeatureCounts
	);
};