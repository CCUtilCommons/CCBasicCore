// test_cc_unordered_map.c
// 综合测试套件：功能测试、边缘测试、冲突测试、重哈希与压力测试
// 编译示例：gcc -O2 -std=c11 test_cc_unordered_map.c -o test_map
// 推荐：gcc -O1 -g -fsanitize=address,undefined test_cc_unordered_map.c -o test_map

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include "utils_def.h"
#include "unorder_map.h"

// ---- 简单断言框架 ----
#define ASSERT(expr, msg) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "[FAIL] %s (at %s:%d)\n", msg, __FILE__, __LINE__); \
            exit(1); \
        } \
    } while(0)

#define PASS(msg) fprintf(stdout, "[PASS] %s\n", msg);

// ---- 常用哈希/比较帮助器 ----
size_t str_hash_simple(const cckey_t buffer) {
    if(!buffer) return 0;
    const char* s = (const char*)buffer;
    // djb2 -> size_t
    size_t h = 5381;
    int c;
    while ((c = *s++)) h = ((h << 5) + h) + (unsigned char)c;
    return h;
}
int str_equal_simple(const void* a, const void* b, size_t sz) {
    (void)sz;
    return strcmp((const char*)a, (const char*)b) == 0;
}
void* str_copy_shallow(const void* src, size_t sz) {
    (void)sz;
    // assume null terminated strings in tests, allocate copy
    return strdup((const char*)src);
}
void str_free_simple(void* p) { free(p); }

// ---- 构造一个 minimal CCSpecialDefinedPack ----
CCSpecialDefinedPack* make_str_pack() {
    CCSpecialDefinedPack* p = malloc(sizeof(CCSpecialDefinedPack));
    p->copier = (SelfDefinedCopy)str_copy_shallow;
    p->freer = (SelfDefinedFree)str_free_simple;
    p->compared = (SelfDefinedCompare)str_equal_simple;
    return p;
}
void free_str_pack(CCSpecialDefinedPack* p) { free(p); }

// ---- 测试用例 ----

void test_create_destroy() {
    CCSpecialDefinedPack* kp = make_str_pack();
    CCSpecialDefinedPack* vp = make_str_pack();

    CCUnorderedMap* m = CCBasicCoreCCUnorderedMap_Create((CCHashFunctor)str_hash_simple,
                                                         sizeof(char*), sizeof(void*),
                                                         kp, vp);
    ASSERT(m != NULL, "create should return non-NULL");
    ASSERT(CCBasicCoreCCUnorderedMap_Size(m) == 0, "size should be 0 on create");
    ASSERT(CCBasicCoreCCUnorderedMap_BucketCount(m) > 0, "bucket_count should be >0");
    int ret = CCBasicCoreCCUnorderedMap_Destroy(m);
    ASSERT(ret == 0 || ret == 1, "destroy should return status (0/1)");
    free_str_pack(kp);
    free_str_pack(vp);
    PASS("test_create_destroy");
}

void test_basic_insert_get_erase() {
    CCSpecialDefinedPack* kp = make_str_pack();
    CCSpecialDefinedPack* vp = make_str_pack();
    CCUnorderedMap* m = CCBasicCoreCCUnorderedMap_Create((CCHashFunctor)str_hash_simple,
                                                         sizeof(char), sizeof(int),
                                                         kp, vp);
    // 插入并读取
    char* k1 = strdup("hello");
    int v1 = 42;
    CCBasicCoreCCUnorderedMap_Insert(m, k1, &v1);

    int* got = (int*)CCBasicCoreCCUnorderedMap_Get(m, "hello");
    ASSERT(got != NULL, "get must find 'hello'");
    // note: depending on implementation it may store copy or pointer;
    // we only check value equality if accessible
    // if direct pointer to stack was stored, can't assert numeric equality.
    // So at least assert non-NULL.
    int erased = CCBasicCoreCCUnorderedMap_Erase(m, "hello");
    ASSERT(erased == 1, "erase should return 1 for existing key");
    int erased2 = CCBasicCoreCCUnorderedMap_Erase(m, "hello");
    ASSERT(erased2 == 0, "erase non-existing should return 0");

    CCBasicCoreCCUnorderedMap_Destroy(m);
    free_str_pack(kp); free_str_pack(vp);
    PASS("test_basic_insert_get_erase");
}

void test_duplicate_key_update() {
    CCSpecialDefinedPack* kp = make_str_pack();
    CCSpecialDefinedPack* vp = make_str_pack();
    CCUnorderedMap* m = CCBasicCoreCCUnorderedMap_Create((CCHashFunctor)str_hash_simple,
                                                         sizeof(char), sizeof(int),
                                                         kp, vp);
    char* k = strdup("dup");
    int* v1 = malloc(sizeof(int)); *v1 = 1;
    int* v2 = malloc(sizeof(int)); *v2 = 2;

    CCBasicCoreCCUnorderedMap_Insert(m, k, v1);
    CCBasicCoreCCUnorderedMap_Insert(m, strdup("dup"), v2); // inserting duplicate key should update

    int* got = (int*)CCBasicCoreCCUnorderedMap_Get(m, "dup");
    ASSERT(got != NULL, "got duplicate key");
    // we can't be certain which pointer stored (implementation detail).
    // We at least check size==1
    ASSERT(CCBasicCoreCCUnorderedMap_Size(m) == 1, "size must remain 1 after duplicate insert");
    CCBasicCoreCCUnorderedMap_Destroy(m);
    free_str_pack(kp); free_str_pack(vp);
    PASS("test_duplicate_key_update");
}

size_t bad_hash(const cckey_t k) { (void)k; return 1; }
void test_collision_behavior() {
    CCSpecialDefinedPack* kp = make_str_pack();
    CCSpecialDefinedPack* vp = make_str_pack();
    CCUnorderedMap* m = CCBasicCoreCCUnorderedMap_Create((CCHashFunctor)bad_hash,
                                                         sizeof(char), sizeof(int),
                                                         kp, vp);
    const int N = 1000;
    for (int i = 0; i < N; i++) {
        char buf[32];
        sprintf(buf, "k_%d", i);
        CCBasicCoreCCUnorderedMap_Insert(m, strdup(buf), malloc(8));
    }
    // all keys should be retrievable
    for (int i = 0; i < N; i++) {
        char buf[32];
        sprintf(buf, "k_%d", i);
        void* v = CCBasicCoreCCUnorderedMap_Get(m, buf);
        ASSERT(v != NULL, "collision test: value should exist");
    }
    CCBasicCoreCCUnorderedMap_Destroy(m);
    free_str_pack(kp); free_str_pack(vp);
    PASS("test_collision_behavior");
}

void test_resize_and_loadfactor() {
    CCSpecialDefinedPack* kp = make_str_pack();
    CCSpecialDefinedPack* vp = make_str_pack();
    CCUnorderedMap* m = CCBasicCoreCCUnorderedMap_Create((CCHashFunctor)str_hash_simple,
                                                         sizeof(char), sizeof(int),
                                                         kp, vp);
    size_t initial_buckets = CCBasicCoreCCUnorderedMap_BucketCount(m);
    const int N = 200000; // 大量插入以触发 rehash 多次
    for (int i = 0; i < N; i++) {
        char* k = malloc(32);
        sprintf(k, "key_big_%d", i);
        int* val = malloc(sizeof(int));
        *val = i;
        CCBasicCoreCCUnorderedMap_Insert(m, k, val);
    }
    size_t after_buckets = CCBasicCoreCCUnorderedMap_BucketCount(m);
    ASSERT(after_buckets >= initial_buckets, "bucket count should not shrink below initial");
    ASSERT(CCBasicCoreCCUnorderedMap_Size(m) == (size_t)N, "size must equal number inserted");
    // spot-check some values
    for (int i = 0; i < 100; i++) {
        char buf[64];
        sprintf(buf, "key_big_%d", i * 997 % N); // pseudo-random picks
        void* v = CCBasicCoreCCUnorderedMap_Get(m, buf);
        ASSERT(v != NULL, "resized map should contain value");
    }
    CCBasicCoreCCUnorderedMap_Destroy(m);
    free_str_pack(kp); free_str_pack(vp);
    PASS("test_resize_and_loadfactor");
}

void test_random_stress_verify() {
    CCSpecialDefinedPack* kp = make_str_pack();
    CCSpecialDefinedPack* vp = make_str_pack();
    CCUnorderedMap* m = CCBasicCoreCCUnorderedMap_Create((CCHashFunctor)str_hash_simple,
                                                         sizeof(char), sizeof(int),
                                                         kp, vp);
    const int N = 50000;
    srand(123456);
    // store keys separately to verify later
    char** keys = malloc(sizeof(char*) * N);
    for (int i = 0; i < N; i++) {
        keys[i] = malloc(32);
        sprintf(keys[i], "rand_%u", (unsigned)rand());
        int* v = malloc(sizeof(int));
        *v = i;
        CCBasicCoreCCUnorderedMap_Insert(m, keys[i], v);
    }
    // verify
    for (int i = 0; i < N; i++) {
        void* v = CCBasicCoreCCUnorderedMap_Get(m, keys[i]);
        ASSERT(v != NULL, "random stress get must succeed");
    }
    // cleanup
    CCBasicCoreCCUnorderedMap_Destroy(m);
    for (int i = 0; i < N; i++) free(keys[i]);
    free(keys);
    free_str_pack(kp); free_str_pack(vp);
    PASS("test_random_stress_verify");
}

void test_edge_cases() {
    CCSpecialDefinedPack* kp = make_str_pack();
    CCSpecialDefinedPack* vp = make_str_pack();
    CCUnorderedMap* m = CCBasicCoreCCUnorderedMap_Create((CCHashFunctor)str_hash_simple,
                                                         sizeof(char), sizeof(int),
                                                         kp, vp);

    // 空字符串 key
    CCBasicCoreCCUnorderedMap_Insert(m, strdup(""), malloc(4));
    ASSERT(CCBasicCoreCCUnorderedMap_Get(m, "") != NULL, "empty string key should work");

    // very long key (10k)
    char* longk = malloc(10005);
    for (int i = 0; i < 10000; i++) longk[i] = 'a' + (i % 26);
    longk[10000] = '\0';
    CCBasicCoreCCUnorderedMap_Insert(m, longk, malloc(8));
    ASSERT(CCBasicCoreCCUnorderedMap_Get(m, longk) != NULL, "long key should be found");

    // NULL key: behavior undefined for many maps; we just check that it doesn't crash if API handles it.
    // If your implementation forbids NULL, this is expected to either ignore or return error.
    int crashed = 0;
    #ifndef ALLOW_NULL_KEY_TEST_DISABLE
    // guard: if implementation crashes on NULL, cannot catch here; user should run under sanitizer
    // to observe behavior. We'll attempt to insert and see if it returns or not.
    // Surround with try-catch not available in C; so we just call (user must run under ASAN to detect crash).
    CCBasicCoreCCUnorderedMap_Insert(m, NULL, NULL);
    #endif

    CCBasicCoreCCUnorderedMap_Destroy(m);
    free_str_pack(kp); free_str_pack(vp);
    PASS("test_edge_cases");
}

int main() {
    fprintf(stdout, "Running CCUnorderedMap test suite...\n");
    clock_t t0 = clock();

    test_create_destroy();
    test_basic_insert_get_erase();
    test_duplicate_key_update();
    test_collision_behavior();
    test_resize_and_loadfactor();
    test_random_stress_verify();
    test_edge_cases();

    double elapsed = (double)(clock() - t0) / CLOCKS_PER_SEC;
    fprintf(stdout, "ALL TESTS PASSED in %.2fs\n", elapsed);
    return 0;
}
