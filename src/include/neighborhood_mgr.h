#pragma once
#include types.h
#include <vector>
#include <unordered_map>
#include <cmath>

class NeighborhoodMgr {
private:
    // Output: nbs (neighbor list)
    std::unordered_map<instanceID, NeighborList> neighborMap;

    // Input param: min_dist
    double distanceThreshold;

    /**
     * @brief Bước 1: DivideSpace(min_dist, S)
     * Chia không gian thành các ô lưới dựa trên ngưỡng khoảng cách.
     * * @param instances Tập dữ liệu đầu vào (S)
     * @return GridMap Cấu trúc grids chứa các instance đã được phân chia
     */
    GridMap divideSpace(const std::vector<SpatialInstance>& instances) const;

    /**
     * @brief Bước 3: GetNeighborGrids(g)
     * Lấy danh sách các ô lưới lân cận của ô hiện tại (bao gồm cả chính nó).
     * * @param currentGridKey Key của ô lưới hiện tại (g)
     * @return std::vector<std::string> Danh sách Key của các ô lân cận (ngrids)
     */
    std::vector<std::string> getNeighborGrids(const std::string& currentGridKey) const;

    /**
     * @brief Bước 6: Is_Neighbor(s, s', min_distance)
     * Kiểm tra khoảng cách Euclide giữa 2 điểm.
     * * @param s Instance 1
     * @param s_prime Instance 2
     * @return true Nếu khoảng cách <= distanceThreshold
     */
    bool isNeighbor(const SpatialInstance* s, const SpatialInstance* s_prime) const;

    /**
     * @brief Hàm hỗ trợ: Tính Key của Grid từ tọa độ
     * Giúp xác định xem một instance thuộc về ô lưới nào (dùng trong DivideSpace).
     */
    std::string getGridKey(double x, double y) const;
public:
    /**
     * @brief Constructor
     * @param threshold Ngưỡng khoảng cách (min_dist)
     */
    NeighborhoodMgr(double threshold);

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
    void materialize(const std::vector<SpatialInstance>& instances);
        
    /**
     * @brief Lấy kết quả danh sách hàng xóm của một instance cụ thể
     * @param id ID của instance
     */
    const NeighborList& getNeighbors(const instanceID& id) const;

    /**
     * @brief Lấy toàn bộ map hàng xóm
     */
    const std::unordered_map<instanceID, NeighborList>& getAllNeighbors() const;
};