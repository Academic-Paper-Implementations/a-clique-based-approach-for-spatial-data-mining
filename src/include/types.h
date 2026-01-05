/**
 * @file types.h
 * @brief Core data type definitions for spatial colocation pattern mining
 *
 * This file defines the fundamental data structures and type aliases used throughout
 * the joinless colocation mining algorithm implementation.
 */

#pragma once
#include <string>
#include <unordered_map>
#include <vector>

 // ============================================================================
 // Type Aliases
 // ============================================================================

struct SpatialInstance; // Forward decl

 /** @brief Type alias for feature types (e.g., "Restaurant", "Hotel") */
using FeatureType = std::string;

/** @brief Type alias for instance identifiers (e.g., "A1", "B2") */
using instanceID = std::string;
using InstanceId = instanceID;
using Instance = SpatialInstance;

/** @brief Type alias for a colocation pattern (set of feature types) */
using Colocation = std::vector<FeatureType>;

/** @brief Type alias for a colocation instance (set of spatial instance pointers) */
using ColocationInstance = std::vector<const struct SpatialInstance*>;

/**
 * @brief Định nghĩa kiểu dữ liệu cho một Grid (Ô lưới)
 * Key: ID của ô lưới (ví dụ chuỗi "x_y")
 * Value: Danh sách các con trỏ tới SpatialInstance nằm trong ô đó
 */
struct Grid {
    int grid_id;
	std::vector<const SpatialInstance*> instances;
};

// ============================================================================
// Data Structures
// ============================================================================

/**
 * @brief Structure representing a spatial data instance
 *
 * Each spatial instance has a feature type, unique identifier, and 2D coordinates.
 */
struct SpatialInstance {
    FeatureType type;  ///< Feature type of this instance (e.g., "A", "B")
    instanceID id;     ///< Unique identifier (e.g., "A1", "B2")
    double x, y;       ///< 2D spatial coordinates
};

struct NeighborList {
    // Sử dụng con trỏ để đồng nhất với ColocationInstance và tối ưu bộ nhớ
    std::vector<const SpatialInstance*> BNs;
    std::vector<const SpatialInstance*> SNs;

    // Helper: Thêm vào danh sách Big Neighbors
    void addBN(const SpatialInstance* inst) {
        BNs.push_back(inst);
    }

    // Helper: Thêm vào danh sách Small Neighbors
    void addSN(const SpatialInstance* inst) {
        SNs.push_back(inst);
    }

    // Kiểm tra xem có hàng xóm nào không
    bool isEmpty() const {
        return BNs.empty() && SNs.empty();
    }
};

// Struct đại diện cho một node trong cây I-tree
// Theo Definition 5 trong paper: contains instance-name and node-link
struct IDSNode {
    InstanceId instance_id;     // instance-name
    IDSNode* right_sibling;     // node-link: links to the right sibling node
    IDSNode* first_child;       // Pointer to the first child (để duyệt xuống dưới)
    IDSNode* parent;            // Pointer to parent (để hỗ trợ pruning/RemoveAncestors)

    // Constructor
    IDSNode(InstanceId id)
        : instance_id(id), right_sibling(nullptr), first_child(nullptr), parent(nullptr) {}
};
