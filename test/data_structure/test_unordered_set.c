#include "unorder_set.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 200000 // 压力测试规模，按需调大

// 整数哈希函数（Knuth hash）
static size_t int_hash(const void* a) {
	return (*(const int*)a) * 2654435761u;
}

// 整数比较函数
static int int_cmp(const void* a, const void* b) {
	int ia = *(const int*)a;
	int ib = *(const int*)b;
	return (ia > ib) - (ia < ib);
}

int main() {
	printf("===== CCUnorderedSet Combined Test =====\n");

	CCSpecialDefinedPack key_ops = {
		.copier = NULL,
		.freer = NULL,
		.compared = int_cmp
	};

	CCUnorderedSet* set = CCBasicCoreUnorderedSet_Create(sizeof(int), &key_ops, int_hash);
	if (!set) {
		printf("Failed to create set\n");
		return 1;
	}

	srand((unsigned)time(NULL));

	/* ---------------- 压力测试 + 性能 ---------------- */
	printf("\n--- Stress & Performance Test ---\n");

	clock_t start = clock();
	for (int i = 0; i < N; i++) {
		int key = rand();
		CCBasicCoreUnorderedSet_Insert(set, &key);
	}
	clock_t end = clock();
	printf("[Insert] %d elements, time = %.3f sec, size = %zu\n",
	       N, (double)(end - start) / CLOCKS_PER_SEC,
	       CCBasicCoreUnorderedSet_Size(set));

	// 查找测试
	int found = 0;
	start = clock();
	for (int i = 0; i < N / 10; i++) {
		int key = rand();
		if (CCBasicCoreUnorderedSet_Contains(set, &key)) {
			found++;
		}
	}
	end = clock();
	printf("[Contains] %d queries, found = %d, time = %.3f sec\n",
	       N / 10, found, (double)(end - start) / CLOCKS_PER_SEC);

	// 删除测试
	int erased = 0;
	start = clock();
	for (int i = 0; i < N / 20; i++) {
		int key = rand();
		if (CCBasicCoreUnorderedSet_Erase(set, &key)) {
			erased++;
		}
	}
	end = clock();
	printf("[Erase] %d attempts, erased = %d, time = %.3f sec, final size = %zu\n",
	       N / 20, erased, (double)(end - start) / CLOCKS_PER_SEC,
	       CCBasicCoreUnorderedSet_Size(set));

	/* ---------------- 边界情况测试 ---------------- */
	printf("\n--- Edge Case Test ---\n");

	int key = 42;
	CCBasicCoreUnorderedSet_Insert(set, &key);
	CCBasicCoreUnorderedSet_Insert(set, &key); // 重复插入
	printf("Duplicate insert(42): %s\n",
	       CCBasicCoreUnorderedSet_Contains(set, &key) ? "FOUND" : "NOT FOUND");

	int not_exist = 99;
	int erased2 = CCBasicCoreUnorderedSet_Erase(set, &not_exist);
	printf("Erase non-existing(99): %s\n", erased2 ? "ERASED" : "NOT FOUND");

	int null_erase = CCBasicCoreUnorderedSet_Erase(NULL, &key);
	printf("Erase on NULL set: %d\n", null_erase);

	int null_contains = CCBasicCoreUnorderedSet_Contains(NULL, &key);
	printf("Contains on NULL set: %d\n", null_contains);

	int extreme1 = INT_MIN;
	int extreme2 = INT_MAX;
	CCBasicCoreUnorderedSet_Insert(set, &extreme1);
	CCBasicCoreUnorderedSet_Insert(set, &extreme2);
	printf("Find INT_MIN: %s\n",
	       CCBasicCoreUnorderedSet_Contains(set, &extreme1) ? "FOUND" : "NOT FOUND");
	printf("Find INT_MAX: %s\n",
	       CCBasicCoreUnorderedSet_Contains(set, &extreme2) ? "FOUND" : "NOT FOUND");

	/* ---------------- 结束清理 ---------------- */
	CCBasicCoreUnorderedSet_Destroy(set);
	printf("\nAll tests finished, memory released.\n");

	return 0;
}
