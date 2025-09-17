#include "set.h"
#include "Error.h"
#include "compares.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_behave/memory_behave.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* Treap-based standalone CCSet implementation.
   Uses CCSpecialDefinedPack for key copy/free/compare.
*/

struct CCSetNode {
	void* key;
	size_t key_size;
	CCSpecialDefinedPack* key_op;
	CCSetNode* left;
	CCSetNode* right;
	unsigned int priority; /* higher = bubble up */
};

/* Random seed once */
static void _seed_rand_once(void) {
	static int seeded = 0;
	if (!seeded) {
		seeded = 1;
		/* seed with time + address noise */
		unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)&seed;
		srand(seed);
	}
}

/* helpers to create/free node */
static CCSetNode* _node_create(const void* key, size_t key_size, CCSpecialDefinedPack* key_op) {
	if (!key_op)
		return NULL;
	void* keycopy = ccmemory_copy((void*)key, key_size, key_op);
	if (!keycopy)
		return NULL;
	CCSetNode* n = (CCSetNode*)allocate_memory(sizeof(CCSetNode));
	if (!n) {
		release_memory(keycopy, n->key_op);
		return NULL;
	}
	n->key = keycopy;
	n->key_size = key_size;
	n->key_op = key_op;
	n->left = n->right = NULL;
	/* priority: use rand(); combine two rand() calls to widen range */
	unsigned int p = ((unsigned int)rand() << 16) ^ (unsigned int)rand();
	n->priority = p ? p : 1;
	return n;
}

static void _node_free(CCSetNode* n) {
	if (!n)
		return;
	release_memory(n->key, n->key_op);
	free_memory(n);
}

/* rotations */
static CCSetNode* _rotate_right(CCSetNode* y) {
	CCSetNode* x = y->left;
	CCSetNode* T2 = x->right;
	x->right = y;
	y->left = T2;
	return x;
}
static CCSetNode* _rotate_left(CCSetNode* x) {
	CCSetNode* y = x->right;
	CCSetNode* T2 = y->left;
	y->left = x;
	x->right = T2;
	return y;
}

/* compare utility (same semantics as in your map code)
   we compare node->key with target: result = key_op->compared(node_key, target)
   - SMALLER => node_key < target => go left
   - LARGE  => node_key > target => go right
   - EQUAL  => equal
*/
static CompareResult _cmp_node_key(CCSetNode* node, const void* key) {
	return (node->key_op->compared)(node->key, key);
}

/* recursive insert returning new subtree root; sets *inserted = 1 if new inserted */
static CCSetNode* _insert_rec(CCSetNode* root, CCSetNode* newnode, int* inserted) {
	if (!root) {
		*inserted = 1;
		return newnode;
	}
	CompareResult r = (root->key_op->compared)(root->key, newnode->key);
	switch (r) {
	case CCBASICCORE_COMPARERESULT_SMALLER:
		root->left = _insert_rec(root->left, newnode, inserted);
		if (root->left && root->left->priority > root->priority) {
			root = _rotate_right(root);
		}
		break;
	case CCBASICCORE_COMPARERESULT_LARGE:
		root->right = _insert_rec(root->right, newnode, inserted);
		if (root->right && root->right->priority > root->priority) {
			root = _rotate_left(root);
		}
		break;
	case CCBASICCORE_COMPARERESULT_EQUAL:
		/* already exists: free newnode */
		release_memory(newnode->key, newnode->key_op);
		free_memory(newnode);
		*inserted = 0;
		break;
	}
	return root;
}

/* recursive erase; sets *erased = 1 if removed; returns new subtree root */
static CCSetNode* _erase_rec(CCSetNode* root, const void* key, int* erased) {
	if (!root)
		return NULL;
	CompareResult r = (root->key_op->compared)(root->key, key);
	if (r == CCBASICCORE_COMPARERESULT_SMALLER) {
		root->left = _erase_rec(root->left, key, erased);
	} else if (r == CCBASICCORE_COMPARERESULT_LARGE) {
		root->right = _erase_rec(root->right, key, erased);
	} else { /* equal: remove this node */
		*erased = 1;
		if (!root->left && !root->right) {
			_node_free(root);
			return NULL;
		} else if (!root->left) {
			CCSetNode* t = root->right;
			_node_free(root);
			return t;
		} else if (!root->right) {
			CCSetNode* t = root->left;
			_node_free(root);
			return t;
		} else {
			/* rotate the child with higher priority up, then continue deletion */
			if (root->left->priority > root->right->priority) {
				root = _rotate_right(root);
				root->right = _erase_rec(root->right, key, erased);
			} else {
				root = _rotate_left(root);
				root->left = _erase_rec(root->left, key, erased);
			}
		}
	}
	return root;
}

/* find */
static CCSetNode* _find_node(CCSetNode* root, const void* key) {
	CCSetNode* cur = root;
	while (cur) {
		CompareResult r = (cur->key_op->compared)(cur->key, key);
		if (r == CCBASICCORE_COMPARERESULT_EQUAL)
			return cur;
		if (r == CCBASICCORE_COMPARERESULT_SMALLER)
			cur = cur->left;
		else
			cur = cur->right;
	}
	return NULL;
}

/* destroy subtree */
static void _destroy_rec(CCSetNode* n) {
	if (!n)
		return;
	_destroy_rec(n->left);
	_destroy_rec(n->right);
	_node_free(n);
}

/* ---------- public API ---------- */

CCSet* CCSet_Create(size_t key_size, CCSpecialDefinedPack* key_op) {
	if (!key_op)
		return NULL;
	_seed_rand_once();
	CCSet* s = (CCSet*)allocate_memory(sizeof(CCSet));
	if (!s)
		return NULL;
	s->root = NULL;
	s->key_size = key_size;
	s->key_op = key_op;
	s->sz = 0;
	return s;
}

int CCSet_Destroy(CCSet* s) {
	if (!s)
		return CCBasicCore_MEMORY_NULL_ADDR;
	_destroy_rec(s->root);
	free_memory(s);
	return CCBasicCore_SUCCESS;
}

int CCSet_Insert(CCSet* s, const void* key) {
	if (!s || !key)
		return CCBasicCore_MEMORY_NULL_ADDR;
	CCSetNode* newnode = _node_create(key, s->key_size, s->key_op);
	if (!newnode)
		return CCBasicCore_MEMORY_ALLOCATION_FAILED;
	int inserted = 0;
	s->root = _insert_rec(s->root, newnode, &inserted);
	if (inserted)
		s->sz++;
	return inserted ? CCBasicCore_SUCCESS : CCBasicCore_MEMORY_ALLOCATION_FAILED;
}

int CCSet_Contains(CCSet* s, const void* key) {
	if (!s || !key)
		return 0;
	return _find_node(s->root, key) != NULL;
}

int CCSet_Erase(CCSet* s, const void* key) {
	if (!s || !key)
		return CCBasicCore_MEMORY_NULL_ADDR;
	int erased = 0;
	s->root = _erase_rec(s->root, key, &erased);
	if (erased) {
		if (s->sz > 0)
			s->sz--;
		return CCBasicCore_SUCCESS;
	}
	return CCBasicCore_MEMORY_ALLOCATION_FAILED;
}
