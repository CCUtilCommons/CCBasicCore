#include "map.h"
#include "Error.h"
#include "compares.h"
#include "map_node.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"
/**
 * @brief create a map
 *
 * @param key_size key size
 * @param value_size value size
 * @param key_op op of key
 * @param value_op value of key
 * @return CCMap*
 */
CCMap* CCBasicCoreCCMap_Create(
    size_t key_size,
    size_t value_size,
    CCSpecialDefinedPack* key_op,
    CCSpecialDefinedPack* value_op) {
	CCMap* map = allocate_one(CCMap);
	map->key_op = key_op;
	map->value_op = value_op;
	map->value_size = value_size;
	map->key_size = key_size;
	map->root = NULL;
	return map;
}

/**
 * @brief insert a node
 *
 * @param map
 * @param key
 * @param value
 */
void CCBasicCoreCCMap_Insert(CCMap* map, cckey_t key, ccvalue_t value) {
	if (!map)
		return;
	CCMapNode* mapping_node = CCBasicCoreCCMapNode_Create(
	    key, map->key_size,
	    map->key_op,
	    value, map->value_size,
	    map->value_op);

	map->root = CCBasicCoreCCMapNode_Insert(map->root, mapping_node);
}

/**
 * @brief find a value by key
 *
 * @param map
 * @param key
 * @return value_t
 */
ccvalue_t CCBasicCoreCCMap_Find(CCMap* map, cckey_t key) {
	if (!map)
		return NULL;
	CCMapNode* mapNode = map->root;
	while (mapNode) {
		CompareResult result = map->key_op->compared(mapNode->key, key);
		switch (result) {
		case CCBASICCORE_COMPARERESULT_SMALLER:
			mapNode = mapNode->leftNode;
			break;
		case CCBASICCORE_COMPARERESULT_LARGE:
			mapNode = mapNode->rightNode;
			break;
		case CCBASICCORE_COMPARERESULT_EQUAL:
			return mapNode->value;
			break;
		};
	}
	return NULL;
}

/**
 * @brief erase a node by key
 *
 * @param map
 * @param key
 * @return int 1 if erased successfully, 0 if not found
 */
int CCBasicCoreCCMap_Erase(CCMap* map, cckey_t key) {
	if (!map || !map->root)
		return CCBasicCore_MEMORY_NULL_ADDR;
	map->root = CCBasicCoreCCMapNode_Delete(map->root, key);
	return CCBasicCore_SUCCESS;
}

/**
 * @brief destroy a map
 *
 * @param map
 * @return int
 */
int CCBasicCoreCCMap_Destroy(CCMap* map) {
	if (!map)
		return CCBasicCore_MEMORY_NULL_ADDR;
	int result = CCBasicCoreCCMapNode_Erase(map->root);
	if (result != CCBasicCore_SUCCESS)
		return result;
	free_memory(map);
	return result;
}