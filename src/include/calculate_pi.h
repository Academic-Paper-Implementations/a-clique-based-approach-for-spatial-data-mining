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

 // ============================================================================
 // CẤU TRÚC BỔ TRỢ (HELPER STRUCTURES)
 // Giúp cú pháp code giống hệt mã giả: Ins[f].Add(...)
 // ============================================================================

 /**
  * @brief Tập hợp các instance duy nhất của một Feature
  * Tương ứng với một phần tử trong 'Ins' tại index f
  */
struct InstanceSet {
    // Sử dụng Set để tự động loại bỏ các instance trùng lặp khi Add
    std::unordered_set<const SpatialInstance*> distinctInstances;

    /**
     * @brief Thêm các instance từ FeatureBucket vào tập hợp
     * Tương ứng dòng 4: .Add(chash.cp[f])
     * Logic: Duyệt qua FeatureBucket và insert từng instance vào set.
     */
    void Add(const FeatureBucket& bucket) {
        for (const auto& colocationInstance : bucket.instances) {
            for (const auto* instance : colocationInstance) {
                // Logic thực tế cần lọc đúng loại feature, 
                // ở đây giả định bucket đã chứa đúng feature f tương ứng
                distinctInstances.insert(instance);
            }
        }
    }

    // Lấy số lượng phần tử (|Ins[f]|)
    size_t size() const {
        return distinctInstances.size();
    }
};

/**
 * @brief Wrapper cho biến 'Ins' trong thuật toán
 * Hỗ trợ cú pháp: Ins[f]
 */
struct InstanceAccumulator {
    std::map<FeatureType, InstanceSet> data;

    /**
     * @brief Truy cập InstanceSet theo feature type
     * Tương ứng dòng 4: Ins[f]...
     */
    InstanceSet& operator[](const FeatureType& f) {
        return data[f];
    }
};

// ============================================================================
// LỚP TÍNH TOÁN PI
// ============================================================================

class PICalculator {
private:
    // Dữ liệu toàn cục cần thiết cho phép tính dòng 8: / |f|
    // Key: FeatureType, Value: Tổng số lượng instance của loại đó trong toàn bộ dữ liệu S
    std::map<FeatureType, int> globalFeatureCounts;

    // ========================================================================
    // CÁC HÀM HELPERS (Tuân thủ logic Algorithm 6)
    // ========================================================================

    /**
     * @brief Lấy các siêu tập (supersets) của candidate pattern từ C-hash
     * Tương ứng dòng 1: supersets = GetSuperSets(c-hash, cp)
     * * @param chash Cấu trúc C-Hash chứa dữ liệu
     * @param cp Candidate pattern hiện tại
     * @return std::vector<Colocation> Danh sách các pattern là superset của cp
     */
    std::vector<Colocation> GetSuperSets(const CHash& chash, const Colocation& cp) const;

public:
    /**
     * @brief Constructor
     * @param counts Map chứa số lượng tổng của từng FeatureType (cần cho bước chia |f|)
     */
    PICalculator(const std::map<FeatureType, int>& counts);

    /**
     * @brief Thực thi thuật toán tính PI (Algorithm 6)
     * * Input:
     * - cp: candidate pattern to be calculated
     * - chash: c-hash structure
     * Output:
     * - pi: the PI value of candidate cp
     * * @param cp Pattern cần tính
     * @param chash Cấu trúc dữ liệu chứa các instances
     * @return double Giá trị PI
     */
    double Calculate(const Colocation& cp, const CHash& chash);
};