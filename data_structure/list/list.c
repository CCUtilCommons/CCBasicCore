#include "list.h"
#include "memory_allocation.h"
#include "memory_allocation_tools.h"
#include "utils_def.h"
#include <alloca.h>
#include <stddef.h>
#include <stddef.h>
#include <string.h>

struct CCListNode {
    void* element_itself;
    size_t size;
    struct CCListNode* next;
    struct CCListNode* prev;
};

typedef struct CCListNode CCListNode;

static CCListNode* createNodeStandAlone(void* elem, size_t size, CCSpecialDefinedPack* pack){
    CCListNode* node = allocate_one(CCListNode);
    void* mem = NULL;
    if(CCBasicCoreCCSpecialDefinedPack_OwnsSpecialCopy(pack)){
        mem = pack->copier(elem);
    }else{
        mem = allocate_memory(size);
        memcpy(mem, elem, size);
    }
    node->element_itself = mem;
    node->size = size;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

static inline void freeNodeOnly(CCListNode* node){
    free_memory(node);
}

void* CCBasicCore_GetListNode(struct CCListNode* node){
    return node->element_itself;
}

int __CCBasicCore_SetListNode(struct CCListNode* node,
                              void* element,
                              size_t element_size){
    if(element_size != node->size) 
        return 0;
    node->element_itself = element;
    return 1;
}

struct CCListNode* CCBasicCore_ListNodeNext(struct CCListNode* node)
{
    return node->next;
}

struct CCListNode* CCBasicCore_ListNodePrev(struct CCListNode* node)
{
    return node->prev;
}

CCList* CCBasicCore_initCCList(size_t element_size)
{
    CCList* list = allocate_one(CCList);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->pack = NULL;
    list->element_size = element_size;
    return list;
}

CCList* CCBasicCore_initSpecialCCList(
    size_t element_size, CCSpecialDefinedPack* pack)
{
    CCList* list = allocate_one(CCList);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->pack = pack;
    list->element_size = element_size;
    return list;
}

int CCBasicCore_destroyCCList(CCList* list)
{
    if(!list) return 1;

    if(list->head && list->size > 0){
        CCListNode* node = list->head;
        size_t count = list->size;
        for(size_t i = 0; i < count; ++i){
            CCListNode* next = node->next;

            if(list->pack && CCBasicCoreCCSpecialDefinedPack_OwnsSpecialFree(list->pack)){
                list->pack->freer(node->element_itself);
            } else {
                free_memory(node->element_itself);
            }

            freeNodeOnly(node);
            node = next;
        }
    }

    free_memory(list);
    return 1;
}

int CCBasicCore_CCListPushFront(CCList* list, void* data, size_t element_size)
{
    if(list->element_size != element_size) 
        return 0;
    CCListNode* node = createNodeStandAlone(data, element_size, list->pack);
    
    // Ok, we get empty
    if(!list->head){
        list->tail = list->head = node;
        list->size = 1;
        return 1;
    }

    // modified the head element
    node->next = list->head;
    list->head->prev = node;
    
    // modified the tail element
    node->prev = list->tail;
    list->tail->next = node;

    // Ok, assigned finished
    list->head = node;
    list->size++;
    return 1;
}

int CCBasicCore_CCListPushBack(CCList* list, void* data, size_t element_size)
{
    if(list->element_size != element_size) 
        return 0;
    CCListNode* node = createNodeStandAlone(data, element_size, list->pack);
    
        // Ok, we get empty
    if(!list->head){
        list->tail = list->head = node;
        list->size = 1;
        return 1;
    }

    // modified the head element
    node->next = list->head;
    list->head->prev = node;
    
    // modified the tail element
    node->prev = list->tail;
    list->tail->next = node;

    // Ok, assigned finished
    list->tail = node;
    list->size++;
    return 1;
}

void* CCBasicCore_CCListPopFront(CCList* list)
{
    if(!list->head) return NULL;

    CCListNode* del_head = list->head;
    void* elem_cont = del_head->element_itself;

    if(list->head == list->tail){
        list->head = list->tail = NULL;
    } else {
        list->head = del_head->next;
        list->tail->next = list->head;
        list->head->prev = list->tail;
    }

    list->size--;
    freeNodeOnly(del_head);
    return elem_cont;
}

void* CCBasicCore_CCListPopBack(CCList* list)
{
    if(!list->head) return NULL;

    CCListNode* del_tail = list->tail;
    void* elem_cont = del_tail->element_itself;

    if(list->head == list->tail){
        list->head = list->tail = NULL;
    } else {
        list->tail = del_tail->prev;
        list->tail->next = list->head;
        list->head->prev = list->tail;
    }

    list->size--;
    freeNodeOnly(del_tail);
    return elem_cont;
}

struct CCListNode* CCBasicCore_CCListInsert(
    CCList* list,
    struct CCListNode* pos,
    void* data,
    size_t element_size)
{
    if(!pos) return NULL;

    // See if we need to modify the head session
    if(pos == list->tail){
        // Insert front
        CCBasicCore_CCListPushBack(list, data, element_size);
        return list->tail;
    }

    CCListNode* next_one = pos->next;
    CCListNode* current_node = createNodeStandAlone(data, element_size, list->pack);
    current_node->next = next_one;
    current_node->prev = pos;
    pos->next = current_node;
    next_one->prev = current_node;

    return current_node;
}

void CCBasicCore_CCListErase(CCList* list, struct CCListNode* pos)
{
    if(pos == list->head) 
        list->head = pos->next;
    if(pos == list->tail) 
        list->tail = pos->prev;

    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;

    if(CCBasicCoreCCSpecialDefinedPack_OwnsSpecialFree( list->pack)){
        list->pack->freer(pos->element_itself);
    }else{
        free_memory(pos->element_itself);
    }
    freeNodeOnly(pos);
}


void CCBasicCore_CCListRunIterate(CCList* list, ContainerIterator iterator){
    if(!list || !list->head || list->size == 0) return;
    CCListNode* session = list->head;
    size_t count = list->size;
    for(size_t i = 0; i < count; ++i){
        iterator(session->element_itself);
        session = session->next;
    }
}
