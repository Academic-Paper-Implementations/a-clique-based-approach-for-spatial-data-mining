#include <iostream>
#include "miner.h"
#include <unordered_map>
#include <set>
#include <algorithm>
#include "calculate_pi.h"
#include "candidate_generation.h"

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
    double minPrev
) {

}