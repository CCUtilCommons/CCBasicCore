
#include "map_node.h"
#include "compares.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"
#include "memory_behave/memory_behave.h"
#include <sys/types.h>

CCMapNode* CCBasicCoreCCMapNode_Create(
    cckey_t key, size_t key_size, 
    CCSpecialDefinedPack* key_element_op,
    ccvalue_t value, size_t value_size, 
    CCSpecialDefinedPack* value_element_op)
{
    CCMapNode* map_node = allocate_one(CCMapNode);
    map_node->leftNode = NULL;
    map_node->rightNode = NULL;
    map_node->key_element_op = key_element_op;
    map_node->value_element_op = value_element_op;
    map_node->key = 
        ccmemory_copy(key, key_size, key_element_op);
    map_node->value = 
        ccmemory_copy(value, value_size, value_element_op);
    map_node->value_size = value_size;
    map_node->key_size = key_size;

    return map_node;
}

CCMapNode* CCBasicCoreCCMapNode_Insert(
    CCMapNode* mapNode, CCMapNode* new_node){
    if(!mapNode){
        return new_node;
    }
    CompareResult result = mapNode->key_element_op->compared(
        mapNode->key, new_node->key);
    switch (result) {
    case CCBASICCORE_COMPARERESULT_SMALLER:
    {
        mapNode->leftNode = CCBasicCoreCCMapNode_Insert(
            mapNode->leftNode, new_node);
    }
    break;
    case CCBASICCORE_COMPARERESULT_LARGE:
    {
        mapNode->rightNode = CCBasicCoreCCMapNode_Insert(
            mapNode->rightNode, new_node);
    }
    break;
    case CCBASICCORE_COMPARERESULT_EQUAL:
    {
        if(mapNode->value){
            release_memory(
                mapNode->value, 
                mapNode->value_element_op);
        }
        mapNode->value = ccmemory_copy(
        new_node->value, new_node->value_size, new_node->value_element_op);
        mapNode->value_size = new_node->value_size;
        free_memory(new_node);
    }
    break;
    }
    return mapNode;

}





// BST 删除节点
CCMapNode* CCBasicCoreCCMapNode_Delete(CCMapNode* root, cckey_t key) {
    if (root == NULL) return NULL;

    if (key < root->key) {
        root->leftNode = CCBasicCoreCCMapNode_Delete(root->leftNode, key);
    } else if (key > root->key) {
        root->rightNode = CCBasicCoreCCMapNode_Delete(root->rightNode, key);
    } else {
        // 找到目标节点
        if (root->leftNode == NULL && root->rightNode == NULL) {
            release_memory(root->key, root->key_element_op);
            release_memory(root->value, root->value_element_op);
            free_memory(root);
            return NULL;
        } else if (root->leftNode == NULL) {
            CCMapNode* temp = root->rightNode;
            release_memory(root->key, root->key_element_op);
            release_memory(root->value, root->value_element_op);
            free_memory(root);
            return temp;
        } else if (root->rightNode == NULL) {
            CCMapNode* temp = root->leftNode;
            release_memory(root->key, root->key_element_op);
            release_memory(root->value, root->value_element_op);
            free_memory(root);
            return temp;
        } else {
            CCMapNode* succ = root->rightNode;
            while (succ->leftNode != NULL) {
                succ = succ->leftNode;
            }
            root->key = succ->key;
            root->value = succ->value;
            root->rightNode = CCBasicCoreCCMapNode_Delete(root->rightNode, succ->key);
        }
    }
    return root;
}


int CCBasicCoreCCMapNode_Erase(CCMapNode* mapNode){
    if(!mapNode) return 1;
    int result = 1;
    result &= CCBasicCoreCCMapNode_Erase(mapNode->leftNode);
    result &= CCBasicCoreCCMapNode_Erase(mapNode->rightNode);
    
    result &= release_memory(mapNode->key, mapNode->key_element_op);
    result &= release_memory(mapNode->value, mapNode->value_element_op);
    result &= free_memory(mapNode);

    return result;
}
