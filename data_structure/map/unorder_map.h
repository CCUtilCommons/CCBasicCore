#ifndef CCUNORDER_MAP_H
#define CCUNORDER_MAP_H
#include "utils_def.h"
#include <stddef.h>
typedef struct CCHashNode CCHashNode;
typedef void* cckey_t;
typedef void* ccvalue_t;
typedef size_t (*CCHashFunctor)(const cckey_t buffer);

/**
 * @brief unordered map settings
 *
 */
typedef struct CCUnorderedMap {
	CCHashNode** nodes_arrays; ///< node arrays
	size_t bucket_count; ///< bucket counts
	size_t size; ///< size of current sessions
	float max_load_factor; ///< load factors to prevent hash collections
	CCHashFunctor hash_functor; ///< hush functors

	size_t key_size;
	CCSpecialDefinedPack* key_packs;
	size_t value_size;
	CCSpecialDefinedPack* value_packs;
} CCUnorderedMap;

/**
 * @brief get the current element counts
 *
 * @param unordered_map
 * @return size_t
 */
static inline size_t CCBasicCoreCCUnorderedMap_Size(CCUnorderedMap* unordered_map) {
	return unordered_map->size;
}

/**
 * @brief get the bucket count
 *
 * @param unordered_map
 * @return size_t
 */
static inline size_t CCBasicCoreCCUnorderedMap_BucketCount(CCUnorderedMap* unordered_map) {
	return unordered_map->bucket_count;
}

/**
 * @brief Create a CCUnorderedMap
 *
 * @param hashy hash functions for creatings
 * @param key_size key size
 * @param value_size value size
 * @param key_packs key packs
 * @param value_packs
 * @return CCUnorderedMap*
 */
CCUnorderedMap* CCBasicCoreCCUnorderedMap_Create(
    CCHashFunctor hashy, // hash function
    size_t key_size,
    size_t value_size,
    CCSpecialDefinedPack* key_packs,
    CCSpecialDefinedPack* value_packs); // package

/**
 * @brief insert a kv pair into an unordered map
 *
 * @param map
 * @param key
 * @param value
 */
void CCBasicCoreCCUnorderedMap_Insert(
    CCUnorderedMap* map, const cckey_t key, const ccvalue_t value);

/**
 * @brief get an element from a map
 *
 * @param map
 * @param key
 * @return ccvalue_t
 */
ccvalue_t CCBasicCoreCCUnorderedMap_Get(CCUnorderedMap* map, const cckey_t key);

/**
 * @brief Erase a mappings in unordered map
 *
 * @param map
 * @param key
 * @return int
 */
int CCBasicCoreCCUnorderedMap_Erase(CCUnorderedMap* map, const cckey_t key);

/**
 * @brief Destroy a map
 *
 * @param unordered_map
 * @return int
 */
int CCBasicCoreCCUnorderedMap_Destroy(CCUnorderedMap* unordered_map);

#endif
