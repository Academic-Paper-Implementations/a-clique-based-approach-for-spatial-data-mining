#include "calculate_pi.h"
#include <unordered_set>
#include <vector>
#include <map>
#include <algorithm>
#include "candidate_generation.h"
#include <set>


std::vector<PatternKey> CalculatePI::GetSuperSets(
	const CHashStructure& chash,
	const PatternKey& candidate
) {
	std::vector<PatternKey> superSets;
	superSets.reserve(chash.size());
	for (const auto& pair : chash) {
		const PatternKey& key = pair.first;
		if (key.size() >= candidate.size() &&
			std::includes(key.begin(), key.end(), candidate.begin(), candidate.end())) {
			superSets.push_back(key);
		}
	}
	return superSets;
}

double CalculatePI::calculatePI(
	const CHashStructure& chash,
	const PatternKey& candidate,
	const std::map<FeatureType, int>& globalFeatureCounts
) {
	std::map<FeatureType, std::set<SpatialInstance>> Ins;
	std::vector<PatternKey> supersets = GetSuperSets(chash, candidate);
	for (const auto& cp : supersets) {
		// Kiểm tra pattern có tồn tại trong map không
		auto patternIt = chash.find(cp);
		if (patternIt == chash.end()) continue;
		const auto& featureMap = patternIt->second;
		for (const auto& f : candidate) {
			//Add all the instances of each feature in the cp that is the same of feature of candidate of supersets to Ins
			auto it = featureMap.find(f);
			if (it != featureMap.end()) {
				const std::vector<SpatialInstance>& instanceList = it->second;
				Ins[f].insert(instanceList.begin(), instanceList.end());
			}
		}
	}
	// 3. Tính Min PI (Giữ nguyên như trước)
	double minPI = 1.0;
	bool first = true;

	for (const auto& f : candidate) {
		if (globalFeatureCounts.find(f) == globalFeatureCounts.end()) return 0.0;

		double numerator = (double)Ins[f].size();
		double denominator = (double)globalFeatureCounts.at(f);

		if (denominator == 0) return 0.0;

		double pr = numerator / denominator;
		if (first) {
			minPI = pr;
			first = false;
		}
		else {
			minPI = std::min(minPI, pr);
		}
	}
	return minPI;
}