#include "utils_def.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ====== 辅助函数 ====== */
static CompareResult int_compare(const void* a, const void* b) {
    return CCCoreBasic_SimpleCompareInts(a, b);
}

static void* int_copy(const void* src) {
    if(!src) return NULL;
    int* dst = malloc(sizeof(int));
    *dst = *(int*)src;
    return dst;
}

static int int_free(void* p) {
    free(p);
    return 1;
}

static CCSpecialDefinedPack int_pack = {
    .copier = int_copy,
    .freer = int_free,
    .compared = int_compare
};

/* ====== 功能测试 ====== */
static void test_basic_insert_find() {
    printf("==== 基础测试 ====\n");
    CCMap* map = CCBasicCoreCCMap_Create(sizeof(int), sizeof(int), &int_pack, &int_pack);

    int k1=1, v1=100;
    int k2=2, v2=200;
    int k3=3, v3=300;

    CCBasicCoreCCMap_Insert(map, &k1, &v1);
    CCBasicCoreCCMap_Insert(map, &k2, &v2);
    CCBasicCoreCCMap_Insert(map, &k3, &v3);

    int* res = (int*)CCBasicCoreCCMap_Find(map, &k2);
    printf("查找 key=2 -> %d (期望200)\n", res ? *res : -1);

    // 覆盖测试
    int v2_new = 222;
    CCBasicCoreCCMap_Insert(map, &k2, &v2_new);
    res = (int*)CCBasicCoreCCMap_Find(map, &k2);
    printf("覆盖 key=2 -> %d (期望222)\n", res ? *res : -1);

    CCBasicCoreCCMap_Destroy(map);
}

/* ====== 边缘测试 ====== */
static void test_edge_cases() {
    printf("\n==== 边缘测试 ====\n");

    CCMap* map = CCBasicCoreCCMap_Create(sizeof(int), sizeof(int), &int_pack, &int_pack);

    int k1 = 42, v1 = 999;

    // 空查找
    int* res = (int*)CCBasicCoreCCMap_Find(map, &k1);
    printf("空map查找 -> %s\n", res ? "非空(错误)" : "NULL(正确)");

    // 插入 NULL key/value（看实现是否安全）
    CCBasicCoreCCMap_Insert(map, NULL, NULL);
    printf("插入NULL key/value 测试通过 (未崩溃)\n");

    // 重复key多次插入
    for (int i=0;i<5;i++) {
        CCBasicCoreCCMap_Insert(map, &k1, &v1);
    }
    res = (int*)CCBasicCoreCCMap_Find(map, &k1);
    printf("重复key插入 -> %d (期望999)\n", res ? *res : -1);

    CCBasicCoreCCMap_Destroy(map);
}

/* ====== 性能测试 ====== */
static void test_performance(size_t n) {
    printf("\n==== 性能测试 (插入%zu个元素) ====\n", n);

    CCMap* map = CCBasicCoreCCMap_Create(sizeof(int), sizeof(int), &int_pack, &int_pack);

    clock_t start = clock();
    for (size_t i = 0; i < n; i++) {
        int key = (int)i;
        int value = (int)(i * 2);
        CCBasicCoreCCMap_Insert(map, &key, &value);
    }
    clock_t end = clock();
    double insert_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("插入耗时: %.4f 秒\n", insert_time);

    start = clock();
    size_t hit = 0;
    for (size_t i = 0; i < n; i++) {
        int key = (int)i;
        int* val = (int*)CCBasicCoreCCMap_Find(map, &key);
        if (val && *val == (int)(i*2)) hit++;
    }
    end = clock();
    double find_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("查找耗时: %.4f 秒 命中率: %.2f%%\n",
           find_time, (double)hit/n*100.0);

    CCBasicCoreCCMap_Destroy(map);
}

int main() {
    test_basic_insert_find();
    test_edge_cases();
    test_performance(100000);  // 10万次插入查找性能测试
    return 0;
}
