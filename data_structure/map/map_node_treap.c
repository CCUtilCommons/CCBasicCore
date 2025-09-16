// map_node.c
#include "utils_def.h"
#ifdef USE_TREAP_ACC
#include "map_node.h"
#include "compares.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"
#include "memory_behave/memory_behave.h"
#include <stdlib.h> 

CCMapNode* CCBasicCoreCCMapNode_Create(
    cckey_t key, size_t key_size, CCSpecialDefinedPack* key_element_op,
    ccvalue_t value, size_t value_size, CCSpecialDefinedPack* value_element_op)
{
    CCMapNode* map_node = allocate_one(CCMapNode);
    map_node->leftNode = NULL;
    map_node->rightNode = NULL;
    map_node->key_element_op = key_element_op;
    map_node->value_element_op = value_element_op;
    map_node->key = ccmemory_copy(key, key_size, key_element_op);
    map_node->value = ccmemory_copy(value, value_size, value_element_op);
    map_node->value_size = value_size;
    map_node->key_size = key_size;
    map_node->priority = rand();

    return map_node;
}

/* ---------------- Treap Helper ---------------- */

/**
 * @brief spin tree right cases
 * 
 * @param y 
 * @return CCMapNode* 
 * @note this function operate the session like this
 *    y               x
 *   / \             / \
 *  x   C   -->     A   y
 * / \                 / \
 *A   B               B   C

 */
static CCMapNode* rotateRight(CCMapNode* y) {
    CCMapNode* x = y->leftNode; // new root
    CCMapNode* T2 = x->rightNode; // new subtree

    x->rightNode = y;
    y->leftNode = T2;
    return x;
}

/**
 * @brief spin tree left cases
 * 
 * @param x 
 * @return CCMapNode* 
 * @note the cases are followings
 *    x               y
 *   / \             / \
 *  A   y   -->     x   C
 *     / \         / \
 *    B   C       A   B
 */
static CCMapNode* rotateLeft(CCMapNode* x) {
    CCMapNode* y = x->rightNode;
    CCMapNode* T2 = y->leftNode;

    y->leftNode = x;
    x->rightNode = T2;
    return y;
}

/* ---------------- Treap 插入 ---------------- */
CCMapNode* CCBasicCoreCCMapNode_Insert(CCMapNode* root, CCMapNode* new_node) {
    if (!root) return new_node;

    CompareResult result = root->key_element_op->compared(root->key, new_node->key);
    if (result == CCBASICCORE_COMPARERESULT_SMALLER) {
        root->leftNode = CCBasicCoreCCMapNode_Insert(root->leftNode, new_node);
        // now if we find the priorty is larger, then we need to spin for
        // the possible unbalanced
        if (root->leftNode->priority > root->priority) {
            root = rotateRight(root);
        }
    }
    else if (result == CCBASICCORE_COMPARERESULT_LARGE) {
        root->rightNode = CCBasicCoreCCMapNode_Insert(root->rightNode, new_node);
        if (root->rightNode->priority > root->priority) {
            root = rotateLeft(root);
        }
    }
    else { 
        if (root->value) {
            release_memory(root->value, root->value_element_op);
        }
        root->value = ccmemory_copy(new_node->value, new_node->value_size, new_node->value_element_op);
        root->value_size = new_node->value_size;
        free_memory(new_node); // 避免泄漏
    }

    return root;
}


CCMapNode* CCBasicCoreCCMapNode_Delete(CCMapNode* root, cckey_t key) {
    if (!root) return NULL;
    CCSpecialDefinedPack* key_op = root->key_element_op;
    CompareResult result = key_op->compared(key, root->key);

    if (result == CCBASICCORE_COMPARERESULT_SMALLER) {
        root->leftNode = CCBasicCoreCCMapNode_Delete(root->leftNode, key);
    }
    else if (result == CCBASICCORE_COMPARERESULT_LARGE) {
        root->rightNode = CCBasicCoreCCMapNode_Delete(root->rightNode, key);
    }
    else {
        // we find the node and about to set the new node as the result
        if (!root->leftNode) {
            CCMapNode* right = root->rightNode;
            release_memory(root->key, root->key_element_op);
            release_memory(root->value, root->value_element_op);
            free_memory(root);
            return right;
        }
        else if (!root->rightNode) {
            CCMapNode* left = root->leftNode;
            release_memory(root->key, root->key_element_op);
            release_memory(root->value, root->value_element_op);
            free_memory(root);
            return left;
        }
        else {
            // 用 Treap 旋转来删除
            if (root->leftNode->priority > root->rightNode->priority) {
                root = rotateRight(root);
                root->rightNode = CCBasicCoreCCMapNode_Delete(root->rightNode, key);
            } else {
                root = rotateLeft(root);
                root->leftNode = CCBasicCoreCCMapNode_Delete(root->leftNode, key);
            }
        }
    }
    return root;
}

int CCBasicCoreCCMapNode_Erase(CCMapNode* mapNode) {
    if (!mapNode) return 1;
    int result = 1;
    result &= CCBasicCoreCCMapNode_Erase(mapNode->leftNode);
    result &= CCBasicCoreCCMapNode_Erase(mapNode->rightNode);

    result &= release_memory(mapNode->key, mapNode->key_element_op);
    result &= release_memory(mapNode->value, mapNode->value_element_op);
    result &= free_memory(mapNode);

    return result;
}
#endif