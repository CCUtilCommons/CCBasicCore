#ifndef CCMAP_H
#define CCMAP_H
#include "utils_def.h"
#include <stddef.h>
typedef void* cckey_t;
typedef void* ccvalue_t;
typedef struct CCMapNode CCMapNode;

/**
 * @brief CCMap is the map interfaces
 *
 */
typedef struct CCMap {
	CCMapNode* root;
	size_t key_size;
	size_t value_size;
	CCSpecialDefinedPack* key_op;
	CCSpecialDefinedPack* value_op;
} CCMap;

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
    CCSpecialDefinedPack* value_op);

/**
 * @brief insert a node
 *
 * @param map
 * @param key
 * @param value
 */
void CCBasicCoreCCMap_Insert(CCMap* map, cckey_t key, ccvalue_t value);

/**
 * @brief erase a node by key
 *
 * @param map
 * @param key
 * @return int 1 if erased successfully, 0 if not found
 */
int CCBasicCoreCCMap_Erase(CCMap* map, cckey_t key);

/**
 * @brief find a value by key
 *
 * @param map
 * @param key
 * @return value_t
 */
ccvalue_t CCBasicCoreCCMap_Find(CCMap* map, cckey_t key);

/**
 * @brief destroy a map
 *
 * @param map
 * @return int
 */
int CCBasicCoreCCMap_Destroy(CCMap* map);

#endif