#pragma once
#include "types.h"
#include <vector>
#include <unordered_map>
#include <cmath>

class NeighborhoodMgr {
private:
	std::unordered_map<const SpatialInstance*, NeighborList> allNeighbors;  // Bản đồ tất cả hàng xóm

	size_t gridCellsX;  // Số ô lưới theo chiều X
	size_t gridCellsY;  // Số ô lưới theo chiều Y

    /**
     * @brief Bước 1: DivideSpace(min_dist, S)
     * Chia không gian thành các ô lưới dựa trên ngưỡng khoảng cách.
     * * @param instances Tập dữ liệu đầu vào (S)
     * @return GridMap Cấu trúc grids chứa các instance đã được phân chia
     */
    std::vector<Grid> divideSpace(double distanceThreshold, const std::vector<SpatialInstance>& instances);

    /**
     * @brief Bước 3: GetNeighborGrids(g)
     * Lấy danh sách các ô lưới lân cận của ô hiện tại (bao gồm cả chính nó).
     * * @param currentGridKey Key của ô lưới hiện tại (g)
     * @return std::vector<std::string> Danh sách Key của các ô lân cận (ngrids)
     */
    std::vector<Grid*> getNeighborGrids(Grid&g, std::vector<Grid>& all_grids) const;

    /**
     * @brief Bước 6: Is_Neighbor(s, s', min_distance)
     * Kiểm tra khoảng cách Euclide giữa 2 điểm.
     * * @param s Instance 1
     * @param s_prime Instance 2
     * @return true Nếu khoảng cách <= distanceThreshold
     */
    bool isNeighbor(const SpatialInstance* s, const SpatialInstance* s_prime, double distanceThreshold) const;
public:
    /**
     * @brief Thực thi thuật toán Neighborhood Materialization
     * * Hàm này sẽ ghép nối logic giống hệt mã giả:
     * 1. grids = divideSpace(...)
     * 2. For each grid in grids...
     * 3.   ngrids = getNeighborGrids(...)
     * 4.   For each s in grid...
     * 5.     For each s' in ngrids...
     * 6.       If isNeighbor(...) -> Add to BNs/SNs
     * * @param instances Danh sách tất cả các instance đầu vào (S)
     */
    void materialize(const std::vector<SpatialInstance>& instances, const double& distanceThreshold);

    /**
     * @brief Lấy toàn bộ map hàng xóm
     */
    const std::unordered_map<const SpatialInstance*, NeighborList>& getAllNeighbors() const;

    void printResults() const;

    // Helper for IDSTree
    std::vector<InstanceId> getBigNeighbors(const InstanceId& id) const;
};