#ifndef CC_UNORDERED_SET_STANDALONE_H
#define CC_UNORDERED_SET_STANDALONE_H

#include "utils_def.h" /* CCSpecialDefinedPack */
#include <stddef.h>
#include <stdint.h>

typedef struct _BucketNode BucketNode;

typedef struct CCUnorderedSet {
	BucketNode** buckets;
	size_t bucket_count;
	size_t key_size;
	CCSpecialDefinedPack* key_op;
	size_t (*hash_fn)(const void*);
	size_t sz;
	double max_load;
} CCUnorderedSet;

/**
 * @brief Create an unordered set.
 * @param key_size size of key in bytes (used for copier)
 * @param key_op pointer to CCSpecialDefinedPack providing copier/freer/compare
 * @param hash_fn pointer to hash function: size_t hash(const void* key)
 * @return pointer to CCUnorderedSet or NULL on error
 */
CCUnorderedSet* CCBasicCoreUnorderedSet_Create(
    size_t key_size,
    CCSpecialDefinedPack* key_op,
    size_t (*hash_fn)(const void*));

/** Destroy set and free all memory. Returns 1 on success. */
int CCBasicCoreUnorderedSet_Destroy(CCUnorderedSet* s);

/** Insert key. Return 1 if newly inserted, 0 if already existed or error. */
int CCBasicCoreUnorderedSet_Insert(CCUnorderedSet* s, const void* key);

/** Return 1 if key exists, 0 otherwise. */
int CCBasicCoreUnorderedSet_Contains(CCUnorderedSet* s, const void* key);

/** Erase key. Return 1 if erased, 0 if not found or error. */
int CCBasicCoreUnorderedSet_Erase(CCUnorderedSet* s, const void* key);

/** Number of elements. */
static inline size_t CCBasicCoreUnorderedSet_Size(CCUnorderedSet* s) {
	if (!s)
		return 0;
	return s->sz;
}

#endif /* CC_UNORDERED_SET_STANDALONE_H */
