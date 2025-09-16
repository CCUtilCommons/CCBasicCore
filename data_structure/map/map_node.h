// map_node.h
#ifndef CCMAP_NODE_TREAP_H
#define CCMAP_NODE_TREAP_H

#include "map.h"
#include "utils_def.h"
#include <stddef.h>
typedef struct CCMapNode {
	cckey_t key;
	ccvalue_t value;
	size_t key_size;
	size_t value_size;

	CCSpecialDefinedPack* key_element_op;
	CCSpecialDefinedPack* value_element_op;

	struct CCMapNode* leftNode;
	struct CCMapNode* rightNode;
#ifdef USE_TREAP_ACC
	int priority; // Treap 的随机优先级
#endif
} CCMapNode;

/**
 * @brief create a map node
 *
 * @param key	key value type
 * @param key_size
 * @param key_element_op
 * @param value
 * @param value_size
 * @param value_element_op
 * @return CCMapNode*
 */
CCMapNode* CCBasicCoreCCMapNode_Create(
    cckey_t key, size_t key_size, CCSpecialDefinedPack* key_element_op,
    ccvalue_t value, size_t value_size, CCSpecialDefinedPack* value_element_op);

/**
 * @brief insert a node into a subtree
 *
 * @param mapNode
 * @param new_node
 * @return CCMapNode*
 */
CCMapNode* CCBasicCoreCCMapNode_Insert(
    CCMapNode* mapNode, CCMapNode* new_node);

/**
 * @brief delete a nodes
 *
 * @param root
 * @param key
 * @return CCMapNode*
 */
CCMapNode* CCBasicCoreCCMapNode_Delete(CCMapNode* root, cckey_t key);

/**
 * @brief erase a subtree
 *
 * @param mapNode
 * @return int
 */
int CCBasicCoreCCMapNode_Erase(CCMapNode* mapNode);

#endif
