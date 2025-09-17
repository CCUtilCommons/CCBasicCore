#include "unorder_set.h"
#include "Error.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_behave/memory_behave.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*
  Simple separate-chaining hash set.
  Uses CCSpecialDefinedPack for key copy/free/compare.
  Resizes when load factor > 0.75.
*/

typedef struct _BucketNode {
	void* key;
	size_t key_size;
	CCSpecialDefinedPack* key_op;
	struct _BucketNode* next;
} BucketNode;

#define INITIAL_BUCKETS 16
#define MAX_LOAD_FACTOR 0.75

/* allocate a bucket node and copy key using key_op->copier */
static BucketNode* _node_create(const void* key, size_t key_size, CCSpecialDefinedPack* key_op) {
	if (!key_op)
		return NULL;
	/* copy key using provided copier */
	void* keycopy = ccmemory_copy((void*)key, key_size, key_op);
	if (!keycopy)
		return NULL;
	BucketNode* n = (BucketNode*)allocate_memory(sizeof(BucketNode));
	if (!n) {
		release_memory(keycopy, n->key_op);
		return NULL;
	}
	n->key = keycopy;
	n->key_size = key_size;
	n->key_op = key_op;
	n->next = NULL;
	return n;
}

static void _node_free_chain(BucketNode* n) {
	while (n) {
		BucketNode* t = n->next;
		release_memory(n->key, n->key_op);
		free_memory(n);
		n = t;
	}
}

/* create set */
CCUnorderedSet* CCBasicCoreUnorderedSet_Create(size_t key_size, CCSpecialDefinedPack* key_op, size_t (*hash_fn)(const void*)) {
	if (!key_op || !hash_fn)
		return NULL;
	CCUnorderedSet* s = (CCUnorderedSet*)allocate_memory(sizeof(CCUnorderedSet));
	if (!s)
		return NULL;
	s->bucket_count = INITIAL_BUCKETS;
	s->buckets = (BucketNode**)allocate_memory(sizeof(BucketNode*) * s->bucket_count);
	if (!s->buckets) {
		free_memory(s);
		return NULL;
	}
	for (size_t i = 0; i < s->bucket_count; ++i)
		s->buckets[i] = NULL;
	s->key_size = key_size;
	s->key_op = key_op;
	s->hash_fn = hash_fn;
	s->sz = 0;
	s->max_load = MAX_LOAD_FACTOR;
	return s;
}

/* destroy */
int CCBasicCoreUnorderedSet_Destroy(CCUnorderedSet* s) {
	if (!s)
		return CCBasicCore_MEMORY_NULL_ADDR;
	for (size_t i = 0; i < s->bucket_count; ++i) {
		_node_free_chain(s->buckets[i]);
	}
	free_memory(s->buckets);
	free_memory(s);
	return CCBasicCore_SUCCESS;
}

/* find: return pointer to node or NULL */
static BucketNode* _find_node_in_bucket(BucketNode* head, const void* key) {
	BucketNode* cur = head;
	while (cur) {
		CompareResult r = (cur->key_op->compared)(cur->key, key);
		if (r == CCBASICCORE_COMPARERESULT_EQUAL)
			return cur;
		cur = cur->next;
	}
	return NULL;
}

/* rehash to new_bucket_count (power-of-two growth expected) */
static int _rehash(CCUnorderedSet* s, size_t new_bucket_count) {
	if (!s || new_bucket_count == 0)
		return 0;
	BucketNode** new_buckets = (BucketNode**)allocate_memory(sizeof(BucketNode*) * new_bucket_count);
	if (!new_buckets)
		return 0;
	for (size_t i = 0; i < new_bucket_count; ++i)
		new_buckets[i] = NULL;

	/* move nodes */
	for (size_t i = 0; i < s->bucket_count; ++i) {
		BucketNode* cur = s->buckets[i];
		while (cur) {
			BucketNode* next = cur->next;
			size_t idx = s->hash_fn(cur->key) % new_bucket_count;
			cur->next = new_buckets[idx];
			new_buckets[idx] = cur;
			cur = next;
		}
	}
	free_memory(s->buckets);
	s->buckets = new_buckets;
	s->bucket_count = new_bucket_count;
	return 1;
}

/* ensure capacity */
static int _ensure_capacity(CCUnorderedSet* s) {
	if (!s)
		return 0;
	double lf = (double)(s->sz + 1) / (double)s->bucket_count;
	if (lf > s->max_load) {
		size_t new_count = s->bucket_count * 2;
		return _rehash(s, new_count);
	}
	return 1;
}

/* Insert key */
int CCBasicCoreUnorderedSet_Insert(CCUnorderedSet* s, const void* key) {
	if (!s || !key)
		return CCBasicCore_MEMORY_NULL_ADDR;
	size_t h = s->hash_fn(key);
	size_t idx = h % s->bucket_count;
	/* if exists, do nothing */
	if (_find_node_in_bucket(s->buckets[idx], key) != NULL)
		return CCBasicCore_UNFIND;
	/* ensure capacity (rehash may change buckets) */
	if (!_ensure_capacity(s))
		return CCBasicCore_MEMORY_ALLOCATION_FAILED;
	/* recompute index after possible rehash */
	idx = s->hash_fn(key) % s->bucket_count;
	BucketNode* n = _node_create(key, s->key_size, s->key_op);
	if (!n)
		return CCBasicCore_MEMORY_ALLOCATION_FAILED;
	n->next = s->buckets[idx];
	s->buckets[idx] = n;
	s->sz++;
	return CCBasicCore_SUCCESS;
}

/* Contains */
int CCBasicCoreUnorderedSet_Contains(CCUnorderedSet* s, const void* key) {
	if (!s || !key)
		return 0;
	size_t idx = s->hash_fn(key) % s->bucket_count;
	return _find_node_in_bucket(s->buckets[idx], key) != NULL;
}

/* Erase */
int CCBasicCoreUnorderedSet_Erase(CCUnorderedSet* s, const void* key) {
	if (!s || !key)
		return 0;
	size_t idx = s->hash_fn(key) % s->bucket_count;
	BucketNode* cur = s->buckets[idx];
	BucketNode* prev = NULL;
	while (cur) {
		CompareResult r = (cur->key_op->compared)(cur->key, key);
		if (r == CCBASICCORE_COMPARERESULT_EQUAL) {
			if (prev)
				prev->next = cur->next;
			else
				s->buckets[idx] = cur->next;
			release_memory(cur->key, cur->key_op);
			free_memory(cur);
			s->sz--;
			return CCBasicCore_SUCCESS;
		}
		prev = cur;
		cur = cur->next;
	}
	return CCBasicCore_UNFIND;
}
