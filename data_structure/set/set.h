#ifndef CC_SET_STANDALONE_H
#define CC_SET_STANDALONE_H

#include "utils_def.h"
#include <stddef.h>
#include <stdint.h>

typedef struct CCSetNode CCSetNode;
typedef struct CCSet {
	CCSetNode* root;
	size_t key_size;
	CCSpecialDefinedPack* key_op;
	size_t sz;
} CCSet;

/* Create / destroy */
CCSet* CCSet_Create(size_t key_size, CCSpecialDefinedPack* key_op);
int CCSet_Destroy(CCSet* s);

/* insert: return 1 if newly inserted, 0 if already existed or error */
int CCSet_Insert(CCSet* s, const void* key);

/* contains: 1 if exists, 0 otherwise */
int CCSet_Contains(CCSet* s, const void* key);

/* erase: 1 if erased, 0 if not found or error */
int CCSet_Erase(CCSet* s, const void* key);

/* size: number of elements */
static inline size_t CCSet_Size(CCSet* s) {
	if (!s)
		return 0;
	return s->sz;
}

#endif /* CC_SET_STANDALONE_H */
