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
#include <unordered_map>
 // Include các header đã định nghĩa
#include "miner.h"
#include "config.h"
#include "types.h"
#include "neighborhood_mgr.h"
#include "ids_tree.h"
#include "candidate_generation.h"
#include "calculate_pi.h"
#include "data_loader.h"
#include "utils.h"



// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(int argc, char* argv[]) {
    try {
        std::cout << "=== STARTING CLIQUE-BASED COLOCATION MINING ===\n" << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 0: Cấu hình và Dữ liệu
        // ---------------------------------------------------------
        // Load configurations from file
        // Note: Assuming executable is run from the directory containing config.txt
        AppConfig config = ConfigLoader::load("config.txt");
        
        std::cout << "Configuration Loaded:" << std::endl;
        std::cout << " - Neighbor Distance: " << config.neighborDistance << std::endl;
        std::cout << " - Min Prevalence: " << config.minPrev << std::endl;
        std::cout << " - Dataset Path: " << config.datasetPath << std::endl;

        std::cout << "\nLoading data..." << std::endl;
        std::vector<SpatialInstance> data = DataLoader::load_csv(config.datasetPath);
        if (data.empty()) {
             std::cout << "Warning: No data loaded or file not found at '" << config.datasetPath << "'." << std::endl;
        }
        std::cout << "Loaded " << data.size() << " spatial instances.\n" << std::endl;

        // Tạo map để lookup nhanh từ ID -> SpatialInstance (Cần cho Bước 3)
        std::unordered_map<instanceID, SpatialInstance> instanceLookup;
        for (const auto& inst : data) {
            instanceLookup[inst.id] = inst;
        }

        // ---------------------------------------------------------
        // BƯỚC 1: Neighborhood Materialization (Algorithm 1)
        // ---------------------------------------------------------
        std::cout << ">>> Step 1: Running Neighborhood Materialization..." << std::endl;
        NeighborhoodMgr neighborMgr;

        // Gọi hàm materialize để tính toán BNs, SNs
        neighborMgr.materialize(data, config.neighborDistance);

        std::cout << "Neighborhoods materialized." << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 2: IDS Algorithm (Algorithm 2)
        // ---------------------------------------------------------
        std::cout << "\n>>> Step 2: Running IDS (Instance-Driven Search)..." << std::endl;
        IDSTree idsMiner(neighborMgr, data);

        // Chạy thuật toán tìm Row-instances cliques (I-Cliques)
        std::vector<std::vector<instanceID>> rawCliques = idsMiner.run();

        std::cout << "Found " << rawCliques.size() << " cliques (row instances)." << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 3: Candidate Generation (Algorithm 4)
        // ---------------------------------------------------------
        std::cout << "\n>>> Step 3: Generating Candidates..." << std::endl;

        // Chuyển đổi từ I-Cliques sang cấu trúc C-Hash
        // IDSTree trả về ID (string), nhưng CandidateGenerator cần SpatialInstance (object).
        std::vector<std::vector<SpatialInstance>> objectCliques;
        objectCliques.reserve(rawCliques.size());

        for (const auto& rowIds : rawCliques) {
            std::vector<SpatialInstance> rowObjs;
            rowObjs.reserve(rowIds.size());
            for (const auto& id : rowIds) {
                // Tìm object tương ứng với ID
                if (instanceLookup.find(id) != instanceLookup.end()) {
                    rowObjs.push_back(instanceLookup[id]);
                }
            }
            objectCliques.push_back(rowObjs);
        }

        CandidateGenerator candidateGen;

        // SỬA: Tên hàm là Candidate_generation và trả về CHashStructure
        CHashStructure cHash = candidateGen.Candidate_generation(objectCliques);

        std::cout << "C-Hash structure built. Number of Pattern Keys: " << cHash.size() << std::endl;

        // ---------------------------------------------------------
        // BƯỚC 4: Prevalent Co-locations Filtering (Algorithm 5)
        // ---------------------------------------------------------
        std::cout << "\n>>> Step 4: Filtering Prevalent Co-locations..." << std::endl;

        /* Lưu ý: Để tính PI chính xác (Algorithm 6), Miner cần biết tổng số lượng
           của từng Feature (Global Counts) để chia mẫu số.
           Trong file miner.h bạn định nghĩa chỉ nhận minPrev, nên ở đây ta giả định
           CoLocationMiner sẽ tự đếm hoặc được set dữ liệu này bên trong.
        */
        PrevalentColocationMiner miner;

        // SỬA: Hàm mineColocations nhận (chash, minPrev, instances)
        // Kết quả trả về là map<PatternKey, double>
        // Thực hiện lọc và tính PI
        std::map<PatternKey, double> results = miner.mineColocations(cHash, config.minPrev, data);
        
        // ---------------------------------------------------------
        // KẾT QUẢ
        // ---------------------------------------------------------
        std::cout << "\n=== FINAL RESULTS (Prevalent Co-locations) ===" << std::endl;
        if (results.empty()) {
            std::cout << "No prevalent patterns found." << std::endl;
        }
        else {
            for (const auto& res : results) {
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