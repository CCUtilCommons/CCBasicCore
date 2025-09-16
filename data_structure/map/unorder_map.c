#include "unorder_map.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"
#include "memory_behave/memory_behave.h"
#include <string.h>


static const size_t prime_list[] = {
    11, 23, 53, 97, 193, 389, 769,
    1543, 3079, 6151, 12289, 24593,
    49157, 98317, 196613, 393241,
    786433, 1572869, 0
};

static size_t query_next_prime(size_t n){
    for (int i = 0; prime_list[i]; i++) {
        if (prime_list[i] >= n) return prime_list[i];
    }
    return n * 2 + 1;
}


typedef struct CCHashNode {
    cckey_t key;
    ccvalue_t value;
    struct CCHashNode* next_node;
} CCHashNode;


CCUnorderedMap* CCBasicCoreCCUnorderedMap_Create(
    CCHashFunctor hashy, // hash function
    size_t key_size,
    size_t value_size,
    CCSpecialDefinedPack* key_packs,
    CCSpecialDefinedPack* value_packs) // package
{
    CCUnorderedMap* unorder_map = allocate_one(CCUnorderedMap);
    unorder_map->bucket_count = prime_list[0];
    allocate_zero_arraymemory(unorder_map->nodes_arrays, unorder_map->bucket_count, CCHashNode*);
    unorder_map->size = 0;
    unorder_map->max_load_factor = 0.75f;
    unorder_map->key_size = key_size;
    unorder_map->value_size = value_size;
    unorder_map->value_packs = value_packs;
    unorder_map->key_packs = key_packs;
    unorder_map->hash_functor = hashy;

    return unorder_map;
}

static void __force_rehash_map(CCUnorderedMap* map, const size_t new_bucket_count)
{
    size_t aligned_count = query_next_prime(new_bucket_count);
    CCHashNode** new_buckets = NULL;
    allocate_zero_arraymemory(new_buckets, aligned_count, CCHashNode*);
    
    for(size_t i = 0; i < map->bucket_count; i++)
    {
        CCHashNode* node = map->nodes_arrays[i];
        while(node){
            CCHashNode* next_node = node->next_node;
            const size_t new_hash_val = map->hash_functor(node->key) % aligned_count;
            node->next_node = new_buckets[new_hash_val];
            new_buckets[new_hash_val] = node;
            node = next_node;
        }
    }

    free_memory(map->nodes_arrays);
    map->nodes_arrays = new_buckets;
    map->bucket_count = aligned_count;
}


void CCBasicCoreCCUnorderedMap_Insert(
    CCUnorderedMap* map, const cckey_t key, const ccvalue_t value)
{
    if(!key){
        return; // Key should never be NULL
    }
    const size_t index = map->hash_functor(key) % map->bucket_count;
    CCHashNode* node = map->nodes_arrays[index];

    while (node) {
        if(map->key_packs->compared(node->key, key)){
            // Ok, these fucks should be reset
            release_memory(node->value, map->value_packs);
            node->value = ccmemory_copy(value, map->value_size, 
                map->value_packs);
            return; 
        }
        node = node->next_node;
    }

    CCHashNode* new_node = allocate_one(CCHashNode);
    new_node->key = ccmemory_copy(key, map->key_size, map->key_packs);
    new_node->value = ccmemory_copy(value, map->value_size, map->value_packs);

    new_node->next_node = map->nodes_arrays[index];
    map->nodes_arrays[index] = new_node;
    map->size++;


    if ((float)map->size / map->bucket_count > map->max_load_factor) {
        __force_rehash_map(map, map->bucket_count * 2);
    }
}


ccvalue_t CCBasicCoreCCUnorderedMap_Get(CCUnorderedMap* map, const cckey_t key)
{
    const size_t index = map->hash_functor(key) % map->bucket_count;
    CCHashNode* current = map->nodes_arrays[index];

    while(current){
        if(map->key_packs->compared(current->key, key)){
            return current->value;
        }
        current = current->next_node;
    }

    return NULL;
}

int CCBasicCoreCCUnorderedMap_Erase(CCUnorderedMap* map, const cckey_t key)
{
    const size_t index = map->hash_functor(key) % map->bucket_count;
    CCHashNode* current = map->nodes_arrays[index];
    CCHashNode* prev = NULL;

    while(current){
        if(!map->key_packs->compared(current->key, key)){
            // Not these one
            prev = current;
            current = current->next_node;
        }else{
            // place the nodes
            if(prev) {
                prev->next_node = current->next_node;
            }else {
                map->nodes_arrays[index] = current->next_node; 
            }

            // release the node
            release_memory(current->key, map->key_packs);
            release_memory(current->value, map->value_packs);
            free_memory(current);            
            return 1;
        }
    }

    return 0;
}


int CCBasicCoreCCUnorderedMap_Destroy(CCUnorderedMap* unordered_map)
{
    for(size_t i = 0; i < unordered_map->bucket_count; i++){
        CCHashNode* node = unordered_map->nodes_arrays[i];
        while(node){
            CCHashNode* this_node = node;
            node = node->next_node;
            release_memory(this_node->key, unordered_map->key_packs);
            release_memory(this_node->value, unordered_map->value_packs);
            free_memory(this_node);
        }
    }

    free_memory(unordered_map->nodes_arrays);
    free_memory(unordered_map);
    
    return 1;
}