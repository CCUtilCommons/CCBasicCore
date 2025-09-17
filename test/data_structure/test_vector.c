#include "result_pack.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// ============ 工具函数 ============
static void print_result(const char* test_name, int passed) {
	printf("[TEST] %-40s : %s\n", test_name, passed ? "PASSED" : "FAILED");
}

// ============ 功能测试 ============
static void test_create_and_free() {
	CCVector* v = CCBasicCoreVector_CreateVector(sizeof(int));
	print_result("Create Vector", v != NULL);

	int res = CCBasicCoreVector_FreeVector(v);
	print_result("Free Vector", res == 0);
}

static void test_push_and_get() {
	CCVector* v = CCBasicCoreVector_CreateVector(sizeof(int));
	int a = 10, b = 20;

	CCBasicCoreVector_PushBack(v, &a);
	CCBasicCoreVector_PushBack(v, &b);

	struct ResultPack* rp = CCBasicCoreVector_Get(v, 1);
	int* val = (int*)(rp->result);
	print_result("PushBack + Get", (*val == 20));

	CCBasicCoreVector_FreeVector(v);
}

static void test_pop_and_resize() {
	CCVector* v = CCBasicCoreVector_CreateVector(sizeof(int));
	for (int i = 0; i < 5; i++)
		CCBasicCoreVector_PushBack(v, &i);

	CCBasicCoreVector_PopBack(v);
	print_result("PopBack size check", v->current_size == 4);

	int res = CCBasicCoreVector_ResizeVector(v, 10);
	print_result("Resize Vector", res == 0 && v->capacity >= 10);

	CCBasicCoreVector_FreeVector(v);
}

// ============ 扩展性能测试 ============
static void test_performance_all() {
	const int N = 100000;
	CCVector* v = CCBasicCoreVector_CreateVector(sizeof(int));
	clock_t start, end;
	double elapsed;

	// PushBack 性能
	start = clock();
	for (int i = 0; i < N; i++)
		CCBasicCoreVector_PushBack(v, &i);
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] PushBack %d elems took %.4f sec\n", N, elapsed);

	// PushFront 性能
	start = clock();
	for (int i = 0; i < N; i++)
		CCBasicCoreVector_PushFront(v, &i);
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] PushFront %d elems took %.4f sec\n", N, elapsed);

	// Get 性能
	start = clock();
	for (int i = 0; i < N; i++) {
		struct ResultPack* rp = CCBasicCoreVector_Get(v, i % v->current_size);
		(void)rp;
	}
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] Get %d elems took %.4f sec\n", N, elapsed);

	// Set 性能
	start = clock();
	for (int i = 0; i < N; i++) {
		int val = i;
		CCBasicCoreVector_Set(v, i % v->current_size, &val);
	}
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] Set %d elems took %.4f sec\n", N, elapsed);

	// PopBack 性能
	start = clock();
	for (int i = 0; i < N && v->current_size > 0; i++)
		CCBasicCoreVector_PopBack(v);
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] PopBack %d elems took %.4f sec\n", N, elapsed);

	// PopFront 性能
	for (int i = 0; i < N; i++)
		CCBasicCoreVector_PushBack(v, &i);
	start = clock();
	for (int i = 0; i < N && v->current_size > 0; i++)
		CCBasicCoreVector_PopFront(v);
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] PopFront %d elems took %.4f sec\n", N, elapsed);

	// Resize 性能
	start = clock();
	for (int i = 0; i < 1000; i++) {
		CCBasicCoreVector_ResizeVector(v, i * 10 + 1);
	}
	end = clock();
	elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("[PERF] Resize 1000 times took %.4f sec\n", elapsed);

	CCBasicCoreVector_FreeVector(v);
}

// ============ 扩展边界/鲁棒性测试 ============
static void test_edge_all() {
	CCVector* v = CCBasicCoreVector_CreateVector(sizeof(int));
	int dummy = 0;

	// Pop 空 vector
	print_result("PopBack empty vector", CCBasicCoreVector_PopBack(v) != 0);
	print_result("PopFront empty vector", CCBasicCoreVector_PopFront(v) != 0);

	// Get/Set 越界
	print_result("Get out of range", CCBasicCoreVector_Get(v, 100)->result == NULL);
	print_result("Set out of range", CCBasicCoreVector_Set(v, 100, &dummy) != 0);

	// Push NULL element (应该安全处理)
	print_result("PushBack NULL element", __CCBasicCoreVector_PushBack(v, NULL, sizeof(int)) != 0);
	print_result("PushFront NULL element", __CCBasicCoreVector_PushFront(v, NULL, sizeof(int)) != 0);

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
	printf("======== CCVector Test Begin ========\n");

	test_create_and_free();
	test_push_and_get();
	test_pop_and_resize();
	test_performance_all();
	test_edge_all();

	printf("======== CCVector Test End ==========\n");
	return 0;
}
