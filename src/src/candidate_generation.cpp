#include "candidate_generation.h"
#include <algorithm>
#include "types.h"

PatternKey CandidateGenerator::GetFeatures(const std::vector<SpatialInstance>& clique) {
    PatternKey key;
	key.reserve(clique.size());
    for (const auto& instance : clique) {
        key.push_back(instance.type);
    }
    std::sort(key.begin(), key.end());
    key.erase(std::unique(key.begin(), key.end()), key.end());
    return key;
}

CHashStructure CandidateGenerator::Candidate_generation(const std::vector<std::vector<SpatialInstance>>& cls) {
    CHashStructure chash;
    for (const auto& clique : cls) {
        PatternKey newKey = GetFeatures(clique);
        if (chash.find(newKey) == chash.end()) {
            chash[newKey] = std::map<FeatureType, std::vector<SpatialInstance>>();
        }
        for (const auto&instance : clique) {
			FeatureType f = instance.type;
			chash[newKey][f].push_back(instance);
        }
    }
    return chash;
}