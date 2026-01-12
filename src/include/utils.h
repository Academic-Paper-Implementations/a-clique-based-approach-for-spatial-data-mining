#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <vector>
#include "neighborhood_mgr.h"


/**
 * @brief Finds the intersection of two vectors of InstanceIds.
 * 
 * Note: This function handles sorting internally to ensure correctness,
 * as set_intersection requires sorted ranges.
 * 
 * @param v1 First vector
 * @param v2 Second vector
 * @return std::vector<instanceID> The intersection (elements present in both)
 */
std::vector<instanceID> getIntersection(const std::vector<instanceID>& v1, const std::vector<instanceID>& v2);




// Helper to print pattern
void printPattern(const Colocation& pattern);

// ==================================================================================
// ALGORITHM 2 HELPERS
// ==================================================================================

void deleteTree(IDSNode* node);

// Step 1: Initialize_Itree
void Initialize_Itree(IDSNode*& root);

// Step 4: AddHeadNode(s)
IDSNode* AddHeadNode(IDSNode* root, instanceID s);

// Step 8: GetChildren(currNode)
std::vector<instanceID> GetChildren(IDSNode* currNode, IDSNode* root, const NeighborhoodMgr& neighbors_mgr);

// Step 10: GetClique(currNode)
std::vector<instanceID> GetClique(IDSNode* currNode, IDSNode* root);

// Step 11: RemoveAncestors(currNode)
void RemoveAncestors(IDSNode* currNode, IDSNode* root);

// Step 13: AddNodes(currNode, childrenNodes)
void AddNodes(IDSNode* currNode, const std::vector<instanceID>& childrenNodes);

#endif // UTILS_H
