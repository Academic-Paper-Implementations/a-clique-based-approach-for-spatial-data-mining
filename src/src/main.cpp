/**
 * @file main.cpp
 * @brief Chương trình chính kiểm thử thuật toán Clique-based Colocation Mining
 *
 * Quy trình:
 * 1. Load Config & Data
 * 2. Neighborhood Materialization (Algo 1)
 * 3. IDS - Instance Driven Search (Algo 2)
 * 4. Candidate Generation (Algo 4)
 * 5. Prevalent Filtering (Algo 5 & 6)
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>

 // Include các header đã định nghĩa
#include "config.h"
#include "types.h"
#include "neighborhood_mgr.h"
#include "ids.h"
#include "candidate_generation.h"
#include "miner.h"

// ============================================================================
// HÀM HỖ TRỢ: TẠO DỮ LIỆU MẪU (DUMMY DATA)
// ============================================================================
std::vector<SpatialInstance> createSampleData() {
    std::vector<SpatialInstance> data;
    // Giả lập dữ liệu không gian: Type, ID, X, Y
    // Tạo một số pattern colocation tiềm năng gần nhau

    // Cụm 1: Có A, B, C gần nhau
    data.push_back({ "A", "A1", 1.0, 1.0 });
    data.push_back({ "B", "B1", 1.2, 1.1 });
    data.push_back({ "C", "C1", 1.1, 1.3 });

    // Cụm 2: Có A, B gần nhau (không có C)
    data.push_back({ "A", "A2", 5.0, 5.0 });
    data.push_back({ "B", "B2", 5.1, 5.2 });

    // Cụm 3: Có B, C gần nhau
    data.push_back({ "B", "B3", 10.0, 10.0 });
    data.push_back({ "C", "C2", 10.2, 10.1 });

    // Nhiễu (Noise): Đứng một mình
    data.push_back({ "A", "A3", 20.0, 20.0 });
    data.push_back({ "D", "D1", 50.0, 50.0 });

    return data;
}

// Hàm hỗ trợ in kết quả pattern (Colocation là vector<string>)
void printPattern(const Colocation& pattern) {
    std::cout << "{ ";
    for (size_t i = 0; i < pattern.size(); ++i) {
        std::cout << pattern[i] << (i < pattern.size() - 1 ? ", " : " ");
    }
    std::cout << "}";
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    try {
        std::cout << "=== STARTING CLIQUE-BASED COLOCATION MINING ===\n" << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 0: Cấu hình và Dữ liệu
        // ---------------------------------------------------------
        AppConfig config;
        config.neighborDistance = 2.0; // Khoảng cách để A1, B1, C1 là hàng xóm
        config.minPrev = 0.3;          // Ngưỡng phổ biến

        std::cout << "Loading data..." << std::endl;
        // Trong thực tế: data = loadFromCSV(config.datasetPath);
        std::vector<SpatialInstance> data = createSampleData();
        std::cout << "Loaded " << data.size() << " spatial instances.\n" << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 1: Neighborhood Materialization (Algorithm 1)
        // ---------------------------------------------------------
        std::cout << ">>> Step 1: Running Neighborhood Materialization..." << std::endl;
        NeighborhoodMgr neighborMgr(config.neighborDistance);

        // Gọi hàm materialize để tính toán BNs, SNs
        neighborMgr.materialize(data);

        std::cout << "Neighborhoods materialized." << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 2: IDS Algorithm (Algorithm 2)
        // ---------------------------------------------------------
        std::cout << "\n>>> Step 2: Running IDS (Instance-Driven Search)..." << std::endl;

        // Khởi tạo IDSMiner với map hàng xóm từ bước 1
        IDSMiner idsMiner(neighborMgr.getAllNeighbors());

        // Chạy thuật toán tìm Row-instances cliques (I-Cliques)
        std::vector<ColocationInstance> cliques = idsMiner.run(data);

        std::cout << "Found " << cliques.size() << " cliques (row instances)." << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 3: Candidate Generation (Algorithm 4)
        // ---------------------------------------------------------
        std::cout << "\n>>> Step 3: Generating Candidates..." << std::endl;
        CandidateGenerator candidateGen;

        // Chuyển đổi từ I-Cliques sang cấu trúc C-Hash
        CHash cHash = candidateGen.generate(cliques);

        std::cout << "C-Hash structure built. Keys generated: " << cHash.data.size() << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 4: Prevalent Co-locations Filtering (Algorithm 5)
        // ---------------------------------------------------------
        std::cout << "\n>>> Step 4: Filtering Prevalent Co-locations..." << std::endl;

        /* Lưu ý: Để tính PI chính xác (Algorithm 6), Miner cần biết tổng số lượng
           của từng Feature (Global Counts) để chia mẫu số.
           Trong file miner.h bạn định nghĩa chỉ nhận minPrev, nên ở đây ta giả định
           CoLocationMiner sẽ tự đếm hoặc được set dữ liệu này bên trong.
        */
        CoLocationMiner miner(config.minPrev);

        // Thực hiện lọc và tính PI
        ResultList results = miner.filter(cHash);

        // ---------------------------------------------------------
        // KẾT QUẢ
        // ---------------------------------------------------------
        std::cout << "\n=== FINAL RESULTS (Prevalent Co-locations) ===" << std::endl;
        if (results.results.empty()) {
            std::cout << "No prevalent patterns found." << std::endl;
        }
        else {
            for (const auto& res : results.results) {
                // res.first là Colocation (vector<string>), res.second là PI (double)
                std::cout << "Pattern: ";
                printPattern(res.first);
                std::cout << " | PI: " << res.second << std::endl;
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}