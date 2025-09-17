#include "compares.h"
#include "result_pack.h"
#include "utils_def.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============ 定义字符串专用操作 ============

void* string_copier(const void* elem) {
	const char* src = *(const char**)elem;
	return strdup(src);
}

int string_freer(void* elem) {
	return 1;
}

CompareResult string_compare(const void* a, const void* b) {
	const char* sa = (const char*)a;
	const char* sb = (const char*)b;
	return strcmp(sa, sb);
}

CCSpecialDefinedPack string_pack = {
	.copier = string_copier,
	.freer = string_freer,
	.compared = string_compare
};

// ============ 测试工具函数 ============
static void print_result(const char* test_name, int passed) {
	printf("[TEST] %-50s : %s\n", test_name, passed ? "PASSED" : "FAILED");
}

// ============ 功能测试 ============
static void test_string_vector_basic() {
	CCVector* v = CCBasicCoreVector_CreateDefinedVector(sizeof(char*), &string_pack);

	char* s1 = "hello";
	char* s2 = "world";

	CCBasicCoreVector_PushBack(v, &s1);
	CCBasicCoreVector_PushBack(v, &s2);

	struct ResultPack* r0 = CCBasicCoreVector_Get(v, 0);
	struct ResultPack* r1 = CCBasicCoreVector_Get(v, 1);

	print_result("PushBack + Get index 0", strcmp(*(char**)r0->result, "hello") == 0);
	print_result("PushBack + Get index 1", strcmp(*(char**)r1->result, "world") == 0);

	// 测试 Set
	char* new_val = "new";
	CCBasicCoreVector_Set(v, 1, &new_val);
	struct ResultPack* r2 = CCBasicCoreVector_Get(v, 1);
	print_result("Set index 1", strcmp(*(char**)r2->result, "new") == 0);

	// PopFront / PopBack
	CCBasicCoreVector_PopFront(v);
	print_result("PopFront size check", v->current_size == 1);
	CCBasicCoreVector_PopBack(v);
	print_result("PopBack empty check", v->current_size == 0);

	CCBasicCoreVector_FreeVector(v);
}

// ============ 性能压力测试 ============
static void stress_test_strings() {
	CCVector* v = CCBasicCoreVector_CreateDefinedVector(sizeof(char*), &string_pack);

	const size_t N = 1000000; // 100 万
	char buf[32];
	clock_t start, end;
	double elapsed;

	// PushBack 性能
	start = clock();
	for (size_t i = 0; i < N; i++) {
		snprintf(buf, sizeof(buf), "hello_%zu", i);
		char* tmp = buf;
		__CCBasicCoreVector_PushBack(v, &tmp, sizeof(char*));
	}
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] PushBack %zu strings took %.4f sec\n", N, elapsed);

	// Get 性能
	start = clock();
	for (size_t i = 0; i < N; i++) {
		struct ResultPack* rp = CCBasicCoreVector_Get(v, i);
		(void)rp;
	}
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] Get %zu strings took %.4f sec\n", N, elapsed);

	// Set 性能
	start = clock();
	for (size_t i = 0; i < N; i++) {
		snprintf(buf, sizeof(buf), "set_%zu", i);
		char* tmp = buf;
		CCBasicCoreVector_Set(v, i, &tmp);
	}
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] Set %zu strings took %.4f sec\n", N, elapsed);

	// Resize 测试
	start = clock();
	CCBasicCoreVector_ResizeVector(v, N / 2);
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] Resize to %zu strings took %.4f sec\n", N / 2, elapsed);

	CCBasicCoreVector_FreeVector(v);
}

// ============ 边界 / 鲁棒性测试 ============
static void test_edge_strings() {
	CCVector* v = CCBasicCoreVector_CreateDefinedVector(sizeof(char*), &string_pack);
	char* dummy = "dummy";

	// Pop 空 vector
	print_result("PopBack empty vector", CCBasicCoreVector_PopBack(v) != 0);
	print_result("PopFront empty vector", CCBasicCoreVector_PopFront(v) != 0);

	// Get / Set 越界
	print_result("Get out of range", CCBasicCoreVector_Get(v, 100)->result == NULL);
	print_result("Set out of range", CCBasicCoreVector_Set(v, 100, &dummy) != 0);

	// Push NULL
	print_result("PushBack NULL element", __CCBasicCoreVector_PushBack(v, NULL, sizeof(char*)) != 0);
	print_result("PushFront NULL element", __CCBasicCoreVector_PushFront(v, NULL, sizeof(char*)) != 0);

	// Resize 0
	print_result("Resize to 0", CCBasicCoreVector_ResizeVector(v, 0) == 0 && v->current_size == 0);

	// 空指针安全
	print_result("NULL FreeVector safe", CCBasicCoreVector_FreeVector(NULL) != 0);
	print_result("NULL PushBack safe", CCBasicCoreVector_PushBack(NULL, &dummy) != 0);
	print_result("NULL PushFront safe", CCBasicCoreVector_PushFront(NULL, &dummy) != 0);
	print_result("NULL PopBack safe", CCBasicCoreVector_PopBack(NULL) != 0);
	print_result("NULL PopFront safe", CCBasicCoreVector_PopFront(NULL) != 0);
	print_result("NULL Get safe", CCBasicCoreVector_Get(NULL, 0) == NULL);
	print_result("NULL Set safe", CCBasicCoreVector_Set(NULL, 0, &dummy) != 0);

	CCBasicCoreVector_FreeVector(v);
}

// ============ 主函数 ============
int main(void) {
	printf("======== String Vector Functional Test ========\n");
	test_string_vector_basic();

	printf("\n======== String Vector Stress / Performance Test ========\n");
	stress_test_strings();

	printf("\n======== String Vector Edge / Robustness Test ========\n");
	test_edge_strings();

	printf("\n======== All String Vector Tests Completed ========\n");
	return 0;
}
