#include <iostream>
#include "miner.h"
#include <unordered_map>
#include <set>
#include <algorithm>
#include "calculate_pi.h"
#include "candidate_generation.h"
#include <ranges>

std::map<FeatureType, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances) {
    std::map<FeatureType, int> featureCount;

    for (const auto& instance : instances) {
        // Extract feature type from instance ID
        // Assumes ID format is: FeatureType + Number (e.g., "A1", "B2")
        // Takes first character as feature type
        FeatureType featureType = instance.id.substr(0, 1);
        featureCount[featureType]++;
    }

    return featureCount;
}

std::unordered_map<PatternKey, double>mineColocations(
    CHashStructure& chash,
    double minPrev,
	const std::vector<SpatialInstance>& instances
) {
	std::unordered_map<PatternKey, double> cs;
    std::map<FeatureType, int> globalFeatureCounts = countInstancesByFeature(instances);
    std::vector<PatternKey> candidates;
	candidates.reserve(chash.size());

    for (const auto& pair : chash) {
        candidates.push_back(pair.first);
    }
    //Sort candidates by size from high to low
    std::sort(candidates.begin(), candidates.end(),
        [](const PatternKey& a, const PatternKey& b) {
            return a.size() > b.size();
        });

    //while not empty candidates
    while (!candidates.empty()) {
        PatternKey currCandidate = candidates.front();
		double pi = CalculatePI::calculatePI(chash, currCandidate, globalFeatureCounts);

        if (pi >= minPrev) {
            std::vector<PatternKey> subsets = GetAllSubsets(currCandidate);
			cs[currCandidate] = pi;
            for (const auto& subset : subsets) {
                double pi_subset = CalculatePI::calculatePI(chash, subset, globalFeatureCounts);
                cs[subset] = pi_subset;
                //Remove all subset from candidates
                std::set<PatternKey> itemsToRemove(subsets.begin(), subsets.end());
                itemsToRemove.insert(currCandidate);
                candidates.erase(
                    std::remove_if(candidates.begin(), candidates.end(),
                        [&](const PatternKey& key) {
                            // Trả về true nếu key nằm trong danh sách cần xóa
                            return itemsToRemove.count(key) > 0;
                        }
                    ),
                    candidates.end()
                );
            }
        }
        else {
            std::vector<PatternKey> directSubs = GetDirectSub(currCandidate); //size-1 subsets
            candidates.erase(candidates.begin());
			AddWithSort(candidates, directSubs);
        }
    }
}

std::vector<PatternKey> GetAllSubsets(const PatternKey& candidate) {
    std::vector<PatternKey> subsets;
    int n = candidate.size();
    int powerSetSize = 1 << n;

    for (int i = 1; i < powerSetSize; ++i) {
        PatternKey sub;
        sub.reserve(n);

        // Kiểm tra từng bit của i
        for (int j = 0; j < n; ++j) {
            if ((i >> j) & 1) {
                sub.push_back(candidate[j]);
            }
        }
        subsets.push_back(sub);
    }

    return subsets;
}

std::vector<PatternKey> GetDirectSub(const PatternKey& candidate) {
    std::vector<PatternKey> directSubsets;

    if (candidate.empty()) return directSubsets;
    for (size_t i = 0; i < candidate.size(); ++i) {
        PatternKey sub = candidate;
        sub.erase(sub.begin() + i);
        directSubsets.push_back(sub);
    }

    return directSubsets;
}

void AddWithSort(std::vector<PatternKey>& candidates, const std::vector<PatternKey>& newPatterns) {
    candidates.insert(candidates.end(), newPatterns.begin(), newPatterns.end());
    std::sort(candidates.begin(), candidates.end(),
        [](const PatternKey& a, const PatternKey& b) {
            return a.size() > b.size();
		});
}