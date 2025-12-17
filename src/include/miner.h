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

 // ============================================================================
 // CẤU TRÚC BỔ TRỢ (HELPER STRUCTURES)
 // ============================================================================

 /**
  * @brief Wrapper cho danh sách Candidate để khớp cú pháp mã giả
  * Tương ứng biến 'candidates'
  */
struct CandidateList {
    std::vector<Colocation> items;

    /**
     * @brief Sắp xếp candidate theo kích thước từ lớn xuống nhỏ
     * Tương ứng dòng 2: candidates.Sort()
     */
    void Sort();

    /**
     * @brief Kiểm tra danh sách không rỗng
     * Tương ứng dòng 3: While NotEmpty(candidates)
     */
    bool NotEmpty() const { return !items.empty(); }

    /**
     * @brief Lấy phần tử đầu tiên
     * Tương ứng dòng 4: candidates.First
     */
    Colocation First() const { return items.front(); }

    /**
     * @brief Xóa danh sách các tập con khỏi candidate
     * Tương ứng dòng 10: candidates.Remove(subsets)
     */
    void Remove(const std::vector<Colocation>& subsets);

    /**
     * @brief Xóa một candidate cụ thể
     * Tương ứng dòng 13: candidates.Remove(currCandidate)
     */
    void Remove(const Colocation& currCandidate);

    /**
     * @brief Thêm vào và sắp xếp lại
     * Tương ứng dòng 14: candidates.AddWithSort(directSubs)
     */
    void AddWithSort(const std::vector<Colocation>& directSubs);

    // Helper để nạp dữ liệu ban đầu từ CHash keys
    void loadFromKeys(const std::vector<Colocation>& keys) {
        items = keys;
    }
};

/**
 * @brief Wrapper cho danh sách kết quả
 * Tương ứng biến 'cs' (Output)
 */
struct ResultList {
    // Lưu cặp (Pattern, PI Value)
    std::vector<std::pair<Colocation, double>> results;

    /**
     * @brief Thêm danh sách tập con và PI tương ứng vào kết quả
     * Tương ứng dòng 9: cs.Add(subsets, PIs)
     */
    void Add(const std::vector<Colocation>& subsets, const std::vector<double>& PIs);
};

// ============================================================================
// LỚP THỰC THI THUẬT TOÁN (MINER)
// ============================================================================

class CoLocationMiner {
private:
    // Input data references
    const CHash* pCHash;
    double min_prev;

    // ========================================================================
    // CÁC HÀM HELPERS (Tuân thủ tên trong Algorithm 5)
    // ========================================================================

    /**
     * @brief Tính chỉ số tham gia (PI) của candidate hiện tại
     * Tương ứng dòng 5: CalculatePI(currCandidate, candidates, chash)
     */
    double CalculatePI(const Colocation& currCandidate, const CandidateList& candidates, const CHash& chash) const;

    /**
     * @brief Lấy tất cả tập con của candidate
     * Tương ứng dòng 7: GetAllSubsets(currCandidate)
     */
    std::vector<Colocation> GetAllSubsets(const Colocation& currCandidate) const;

    /**
     * @brief Tính PI cho một danh sách các tập con
     * Tương ứng dòng 8: CalculatePIs(subsets)
     * (Lưu ý: Hàm này ngầm định truy cập vào dữ liệu instances trong chash để tính toán)
     */
    std::vector<double> CalculatePIs(const std::vector<Colocation>& subsets) const;

    /**
     * @brief Lấy các tập con trực tiếp (size - 1)
     * Tương ứng dòng 12: GetDirectSub(currCandidate)
     */
    std::vector<Colocation> GetDirectSub(const Colocation& currCandidate) const;

public:
    /**
     * @brief Constructor
     */
    CoLocationMiner(double minPrevThreshold);

    /**
     * @brief Thực thi thuật toán Prevalent co-locations filtering
     * Input: chash, min_prev (đã set trong constructor hoặc truyền vào)
     * Output: cs (list of prevalent co-locations)
     * * @param chash Cấu trúc C-Hash đầu vào
     * @return ResultList Danh sách kết quả cs
     */
    ResultList filter(const CHash& chash);
};