#include "unorder_map.h"
#include "utils_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ================= 辅助函数 =================

// 简单整数 hash
static size_t int_hash(const cckey_t buffer) {
	if (!buffer)
		return 0;
	int key = *(int*)buffer;
	return (size_t)key;
}

static CompareResult int_compare(const void* a, const void* b) {
	if (!a || !b)
		return -1;
	int ia = *(int*)a;
	int ib = *(int*)b;
	return (ia > ib) - (ia < ib);
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

// ================= 测试工具函数 =================
static void print_result(const char* test_name, int passed) {
	printf("[TEST] %-50s : %s\n", test_name, passed ? "PASSED" : "FAILED");
}

// ================= 功能测试 =================
static void test_unordered_map_basic() {
	printf("==== CCUnorderedMap 基础功能测试 ====\n");

	CCUnorderedMap* map = CCBasicCoreCCUnorderedMap_Create(
	    int_hash, sizeof(int), sizeof(int), &int_pack, &int_pack);

	int k1 = 1, v1 = 100;
	int k2 = 2, v2 = 200;
	int k3 = 3, v3 = 300;

	CCBasicCoreCCUnorderedMap_Insert(map, &k1, &v1);
	CCBasicCoreCCUnorderedMap_Insert(map, &k2, &v2);
	CCBasicCoreCCUnorderedMap_Insert(map, &k3, &v3);

	int* res = (int*)CCBasicCoreCCUnorderedMap_Get(map, &k2);
	print_result("查找 key=2", res && *res == 200);

	// 覆盖测试
	int v2_new = 222;
	CCBasicCoreCCUnorderedMap_Insert(map, &k2, &v2_new);
	res = (int*)CCBasicCoreCCUnorderedMap_Get(map, &k2);
	print_result("覆盖 key=2", res && *res == 222);

	// Erase 测试
	int erase_ret = CCBasicCoreCCUnorderedMap_Erase(map, &k2);
	res = (int*)CCBasicCoreCCUnorderedMap_Get(map, &k2);
	print_result("Erase key=2", erase_ret == 0 && res == NULL);

	CCBasicCoreCCUnorderedMap_Destroy(map);
}

// ================= 边界 / 鲁棒性测试 =================
static void test_unordered_map_edge() {
	printf("\n==== CCUnorderedMap 边界/鲁棒性测试 ====\n");

	CCUnorderedMap* map = CCBasicCoreCCUnorderedMap_Create(
	    int_hash, sizeof(int), sizeof(int), &int_pack, &int_pack);

	int key = 42, val = 999;

	// 空查找
	int* res = (int*)CCBasicCoreCCUnorderedMap_Get(map, &key);
	print_result("空 map 查找", res == NULL);

	// 插入 NULL key/value
	CCBasicCoreCCUnorderedMap_Insert(map, NULL, NULL);
	print_result("插入 NULL key/value (未崩溃)", 1);

	// Erase 空 key
	int ret = CCBasicCoreCCUnorderedMap_Erase(map, NULL);
	print_result("Erase NULL key (安全)", ret != -1);

	// 空指针安全
	res = (int*)CCBasicCoreCCUnorderedMap_Get(NULL, &key);
	print_result("NULL map Get 安全", res == NULL);
	CCBasicCoreCCUnorderedMap_Insert(NULL, &key, &val);
	print_result("NULL map Insert 安全", 1);
	CCBasicCoreCCUnorderedMap_Erase(NULL, &key);
	print_result("NULL map Erase 安全", 1);
	CCBasicCoreCCUnorderedMap_Destroy(NULL);

	CCBasicCoreCCUnorderedMap_Destroy(map);
}

// ================= 性能压力测试 =================
static void test_unordered_map_performance(size_t n) {
	printf("\n==== CCUnorderedMap 性能压力测试 (插入%zu个元素) ====\n", n);

	CCUnorderedMap* map = CCBasicCoreCCUnorderedMap_Create(
	    int_hash, sizeof(int), sizeof(int), &int_pack, &int_pack);

	clock_t start, end;
	double elapsed;

	// 插入性能
	start = clock();
	for (size_t i = 0; i < n; i++) {
		int key = (int)i;
		int val = (int)(i * 2);
		CCBasicCoreCCUnorderedMap_Insert(map, &key, &val);
	}
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] 插入 %zu 个元素耗时: %.4f 秒\n", n, elapsed);

	// 查找性能
	size_t hit = 0;
	start = clock();
	for (size_t i = 0; i < n; i++) {
		int key = (int)i;
		int* val = (int*)CCBasicCoreCCUnorderedMap_Get(map, &key);
		if (val && *val == (int)(i * 2))
			hit++;
	}
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] 查找 %zu 个元素耗时: %.4f 秒, 命中率: %.2f%%\n",
	       n, elapsed, (double)hit / n * 100.0);

	CCBasicCoreCCUnorderedMap_Destroy(map);
}

// ================= 主函数 =================
int main(void) {
	printf("======== CCUnorderedMap 功能测试 ========\n");
	test_unordered_map_basic();

	printf("\n======== CCUnorderedMap 边界/鲁棒性测试 ========\n");
	test_unordered_map_edge();

	printf("\n======== CCUnorderedMap 性能压力测试 ========\n");
	test_unordered_map_performance(100000); // 10 万元素插入查找

	printf("\n======== CCUnorderedMap 测试完成 ========\n");
	return 0;
}
