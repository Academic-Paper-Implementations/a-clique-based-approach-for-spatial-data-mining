#include "neighborhood_mgr.h"
#include <iostream>


std::vector<Grid> NeighborhoodMgr::divideSpace(double distanceThreshold, const std::vector<SpatialInstance>& instances){
    // Early exit if there are no instances to process.
    if (instances.empty()) {
        return {};
    }

    // 1. Calculate the bounding box (min/max coordinates) of the dataset.
    double min_x = instances[0].x;
    double min_y = instances[0].y;
    double max_x = instances[0].x;
    double max_y = instances[0].y;

    for (const auto& inst : instances) {
        if (inst.x < min_x) min_x = inst.x;
        if (inst.y < min_y) min_y = inst.y;
        if (inst.x > max_x) max_x = inst.x;
        if (inst.y > max_y) max_y = inst.y;
    }

    // 2. Determine grid dimensions based on the distance threshold.
    this->gridCellsX = static_cast<size_t>(std::ceil((max_x - min_x) / distanceThreshold));
    this->gridCellsY = static_cast<size_t>(std::ceil((max_y - min_y) / distanceThreshold));

    // Ensure at least one cell exists (handles cases where min == max).
    if (gridCellsX == 0) gridCellsX = 1;
    if (gridCellsY == 0) gridCellsY = 1;

    // 3. Initialize the grid cells with IDs.
    std::vector<Grid> dividedSpace(gridCellsX * gridCellsY);
    for (size_t i = 0; i < dividedSpace.size(); ++i) {
        dividedSpace[i].grid_id = i;
    }

    // 4. Assign each instance to the corresponding grid cell.
    for (const auto& inst : instances) {
        size_t gridX = static_cast<size_t>((inst.x - min_x) / distanceThreshold);
        size_t gridY = static_cast<size_t>((inst.y - min_y) / distanceThreshold);

        // Clamp indices to prevent out-of-bounds access (e.g., when val == max).
        if (gridX >= gridCellsX) gridX = gridCellsX - 1;
        if (gridY >= gridCellsY) gridY = gridCellsY - 1;

        // Calculate linear index (row-major order) and store the instance pointer.
        size_t gridID = gridY * gridCellsX + gridX;
        dividedSpace[gridID].instances.push_back(&inst);
    }

    return dividedSpace;
};


std::vector<Grid*> NeighborhoodMgr::getNeighborGrids(Grid& g, std::vector<Grid>& all_grids) const {
    std::vector<Grid*> neighbor_grids;
    neighbor_grids.reserve(9);

	// 1. Get grid dimensions
    long long width = static_cast<long long>(this->gridCellsX);
    long long height = static_cast<long long>(this->gridCellsY);

	// 2. Calculate current grid's 2D coordinates
    long long curX = g.grid_id % width;
    long long curY = g.grid_id / width;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
			// Calculate neighbor coordinates
            long long nx = curX + dx;
            long long ny = curY + dy;

			// 3. CHECK LOGIC: Check bounds
            if (nx >= 0 && nx < width &&
                ny >= 0 && ny < height) {

				// 4. Calculate neighbor grid ID
                size_t nid = ny * width + nx;

				// Add neighbor grid pointer to the list
                neighbor_grids.push_back(&all_grids[nid]);
            }
        }
    }
    return neighbor_grids;
}


bool NeighborhoodMgr::isNeighbor(const SpatialInstance* s, const SpatialInstance* s_prime, double distanceThreshold) const {
    double dx = s->x - s_prime->x;
    double dy = s->y - s_prime->y;
    double dist_sq = dx * dx + dy * dy;
	return dist_sq <= (distanceThreshold * distanceThreshold);
};


void NeighborhoodMgr::materialize(const std::vector<SpatialInstance>& instances, const double& distanceThreshold) {
    allNeighbors.clear();

	std::vector<Grid> grids = divideSpace(distanceThreshold, instances);
    for (auto& grid : grids) {
        std::vector<Grid*> ngrids = getNeighborGrids(grid, grids);
        for (const auto* s : grid.instances) {
            for (const auto* ngrid : ngrids) {
                for (const auto* s_prime : ngrid->instances) {
                    if (s == s_prime) continue;  // Skip self-comparison
                    if (isNeighbor(s, s_prime, distanceThreshold)) {
                        if (s->type < s_prime->type) {
                            this->allNeighbors[s].addBN(s_prime);
                        } else if (s->type > s_prime->type) {
                            this->allNeighbors[s].addSN(s_prime);
                        }
                    }
				}
            }

        }
	}
};


const std::unordered_map<const SpatialInstance*, NeighborList>& NeighborhoodMgr::getAllNeighbors() const {
	return this->allNeighbors;
};


void NeighborhoodMgr::printResults() const {
    std::cout << "\n--- KET QUA NEIGHBORHOOD ---" << std::endl;
    for (const auto& pair : allNeighbors) {
        const SpatialInstance* s = pair.first;
        const NeighborList& nl = pair.second;

        std::cout << "ID: " << s->id << " | Type: " << s->type
            << " | Pos: (" << s->x << ", " << s->y << ")\n";

        std::cout << "  -> BN (>=): ";
        if (nl.BNs.empty()) std::cout << "None";
        for (const auto* n : nl.BNs) std::cout << n->id << " ";
        std::cout << "\n";

        std::cout << "  -> SN (<):  ";
        if (nl.SNs.empty()) std::cout << "None";
        for (const auto* n : nl.SNs) std::cout << n->id << " ";
        std::cout << "\n";
    }
}

std::vector<InstanceId> NeighborhoodMgr::getBigNeighbors(const InstanceId& id) const {
    const SpatialInstance* key = nullptr;
    // Linear search for the instance string ID
    for (const auto& pair : allNeighbors) {
        if (pair.first->id == id) {
            key = pair.first;
            break;
        }
    }

    if (!key) return {};

    const auto& bns = allNeighbors.at(key).BNs;
    std::vector<InstanceId> res;
    res.reserve(bns.size());
    for (const auto* n : bns) {
        res.push_back(n->id);
    }
    return res;
}