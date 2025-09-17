#include "map.h"
#include "utils_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============ 辅助函数 ============

static CompareResult int_compare(const void* a, const void* b) {
	return CCCoreBasic_SimpleCompareInts(a, b);
}

static void* int_copy(const void* src) {
	if (!src)
		return NULL;
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

// ============ 测试工具函数 ============
static void print_result(const char* test_name, int passed) {
	printf("[TEST] %-50s : %s\n", test_name, passed ? "PASSED" : "FAILED");
}

// ============ 功能测试 ============
static void test_map_basic() {
	printf("==== 基础功能测试 ====\n");
	CCMap* map = CCBasicCoreCCMap_Create(sizeof(int), sizeof(int), &int_pack, &int_pack);

	int k1 = 1, v1 = 100;
	int k2 = 2, v2 = 200;
	int k3 = 3, v3 = 300;

	CCBasicCoreCCMap_Insert(map, &k1, &v1);
	CCBasicCoreCCMap_Insert(map, &k2, &v2);
	CCBasicCoreCCMap_Insert(map, &k3, &v3);

	int* res = (int*)CCBasicCoreCCMap_Find(map, &k2);
	print_result("查找 key=2", res && *res == 200);

	// 覆盖测试
	int v2_new = 222;
	CCBasicCoreCCMap_Insert(map, &k2, &v2_new);
	res = (int*)CCBasicCoreCCMap_Find(map, &k2);
	print_result("覆盖 key=2", res && *res == 222);

	CCBasicCoreCCMap_Destroy(map);
}

// ============ 边界 / 鲁棒性测试 ============
static void test_map_edge() {
	printf("\n==== 边界/鲁棒性测试 ====\n");

	CCMap* map = CCBasicCoreCCMap_Create(sizeof(int), sizeof(int), &int_pack, &int_pack);
	int k1 = 42, v1 = 999;

	// 空查找
	int* res = (int*)CCBasicCoreCCMap_Find(map, &k1);
	print_result("空 map 查找", res == NULL);

	// 插入 NULL key/value
	CCBasicCoreCCMap_Insert(map, NULL, NULL);
	print_result("插入 NULL key/value", 1);

	// 重复 key 多次插入
	for (int i = 0; i < 5; i++)
		CCBasicCoreCCMap_Insert(map, &k1, &v1);
	res = (int*)CCBasicCoreCCMap_Find(map, &k1);
	print_result("重复 key 插入", res && *res == 999);

	// 空指针安全
	res = (int*)CCBasicCoreCCMap_Find(NULL, &k1);
	print_result("NULL map Find 安全", res == NULL);
	CCBasicCoreCCMap_Insert(NULL, &k1, &v1);
	print_result("NULL map Insert 安全", 1);
	CCBasicCoreCCMap_Destroy(NULL); // 应该安全

	CCBasicCoreCCMap_Destroy(map);
}

// ============ 性能压力测试 ============
static void test_map_performance(size_t n) {
	printf("\n==== 性能压力测试 (插入%zu个元素) ====\n", n);

	CCMap* map = CCBasicCoreCCMap_Create(sizeof(int), sizeof(int), &int_pack, &int_pack);

	clock_t start = clock();
	for (size_t i = 0; i < n; i++) {
		int key = (int)i;
		int value = (int)(i * 2);
		CCBasicCoreCCMap_Insert(map, &key, &value);
	}
	clock_t end = clock();
	double insert_time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] 插入耗时: %.4f 秒\n", insert_time);

	start = clock();
	size_t hit = 0;
	for (size_t i = 0; i < n; i++) {
		int key = (int)i;
		int* val = (int*)CCBasicCoreCCMap_Find(map, &key);
		if (val && *val == (int)(i * 2))
			hit++;
	}
	end = clock();
	double find_time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] 查找耗时: %.4f 秒, 命中率: %.2f%%\n",
	       find_time, (double)hit / n * 100.0);

	CCBasicCoreCCMap_Destroy(map);
}

// ============ 主函数 ============
int main(void) {
	printf("======== CCMap 功能测试 ========\n");
	test_map_basic();

	printf("\n======== CCMap 边界/鲁棒性测试 ========\n");
	test_map_edge();

	printf("\n======== CCMap 性能压力测试 ========\n");
	test_map_performance(100000); // 10 万元素插入查找

	printf("\n======== CCMap 测试完成 ========\n");
	return 0;
}
