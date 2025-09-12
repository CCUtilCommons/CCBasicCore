#ifndef CCBASIC_CORE_LIST_H
#define CCBASIC_CORE_LIST_H
#include "utils_def.h"
#include <stddef.h>
///< this is the internal listnode type, which should be iterator only
struct CCListNode;

/**
 * @brief CCBasicCore_GetListNode, unpack node for the data
 *
 * @param node
 * @return void*
 */
void* CCBasicCore_GetListNode(struct CCListNode* node);

/**
 * @brief set the node for data
 *
 * @param node
 * @param element
 * @param element_size
 * @return int if we set success, then issue down
 */
int __CCBasicCore_SetListNode(struct CCListNode* node,
                              void* element,
                              size_t element_size);

#define CCBasicCore_SetListNode(node, element) \
	__CCBasicCore_SetListNode(node, element, sizeof(*element));

/**
 * @brief get the next element of the node
 *
 * @param node
 * @return struct CCListNode*
 */
struct CCListNode* CCBasicCore_ListNodeNext(struct CCListNode* node);

/**
 * @brief get the prev element of the node
 *
 * @param node
 * @return struct CCListNode*
 */
struct CCListNode* CCBasicCore_ListNodePrev(struct CCListNode* node);

/**
 * @brief CCList the type for the list sessions
 *
 */
typedef struct CCList {
	struct CCListNode* head; // head
	struct CCListNode* tail; // tail
	size_t size; // size of total
	CCSpecialDefinedPack* pack;
	size_t element_size;
} CCList;

/**
 * @brief init CCList
 *
 * @param element_size
 * @return CCList*
 */
CCList* CCBasicCore_initCCList(size_t element_size);

/**
 * @brief
 *
 * @param element_size
 * @param pack
 * @return CCList*
 */
CCList* CCBasicCore_initSpecialCCList(size_t element_size, CCSpecialDefinedPack* pack);

/**
 * @brief destroy list
 *
 * @param list
 * @return int
 */
int CCBasicCore_destroyCCList(CCList* list);

/**
 * @brief get the size of the list
 *
 * @param list
 * @return size_t
 */
static inline size_t CCBasicCore_CCListSize(CCList* list) {
	return list->size;
}

/**
 * @brief check if the list is empty
 *
 * @param list
 * @return int
 */
static inline int CCBasicCore_CCListEmpty(CCList* list) {
	return list->size == 0;
}

/**
 * @brief query the head from the list
 *
 * @param list
 * @return struct CCListNode*
 */
static inline struct CCListNode* CCBasicCore_CCListHead(CCList* list) {
	return list->head;
}

/**
 * @brief query the tail from the list
 *
 * @param list
 * @return struct CCListNode*
 */
static inline struct CCListNode* CCBasicCore_CCListTail(CCList* list) {
	return list->tail;
}

/**
 * @brief assigned front of the list
 *
 * @param list
 * @param data
 * @param element_size
 */
int CCBasicCore_CCListPushFront(CCList* list, void* data, size_t element_size);

/**
 * @brief assigned backward of the list
 *
 * @param list
 * @param data
 * @param element_size
 */
int CCBasicCore_CCListPushBack(CCList* list, void* data, size_t element_size);

/**
 * @brief pop front of the list
 *
 * @param list
 * @param data
 * @param element_size
 */
void* CCBasicCore_CCListPopFront(CCList* list);

/**
 * @brief pop back of the list
 *
 * @param list
 * @param data
 * @param element_size
 */
void* CCBasicCore_CCListPopBack(CCList* list);

/**
 * @brief yes the
 *
 * @param list
 * @param pos
 * @param data
 * @param element_size
 * @warning you must assume the pos node belongs to the node
 * @return struct CCListNode*
 */
struct CCListNode* CCBasicCore_CCListInsert(
    CCList* list,
    struct CCListNode* pos,
    void* data,
    size_t element_size);

/**
 * @brief Erase the given pos
 *
 * @param list
 * @param pos
 */
void CCBasicCore_CCListErase(CCList* list, struct CCListNode* pos);

/**
 * @brief iterator the list
 *
 * @param list
 * @param iterator
 */
void CCBasicCore_CCListRunIterate(CCList* list, ContainerIterator iterator);

#endif