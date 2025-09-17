#include "limits.h"
#include "set.h"
#include "utils_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 200000 // 压力测试规模

// 整数比较函数
static int int_cmp(const void* a, const void* b) {
	int ia = *(const int*)a;
	int ib = *(const int*)b;
	return (ia > ib) - (ia < ib);
}

int main() {
	printf("===== CCSet Combined Test =====\n");

	CCSpecialDefinedPack key_ops = {
		.copier = NULL,
		.freer = NULL,
		.compared = int_cmp
	};

	CCSet* set = CCSet_Create(sizeof(int), &key_ops);
	if (!set) {
		printf("Failed to create CCSet\n");
		return 1;
	}

	srand((unsigned)time(NULL));

	/* ---------------- 压力测试 + 性能 ---------------- */
	printf("\n--- Stress & Performance Test ---\n");

	clock_t start = clock();
	for (int i = 0; i < N; i++) {
		int key = rand();
		CCSet_Insert(set, &key);
	}
	clock_t end = clock();
	printf("[Insert] %d elements, time = %.3f sec, size = %zu\n",
	       N, (double)(end - start) / CLOCKS_PER_SEC,
	       CCSet_Size(set));

	// 查找测试
	int found = 0;
	start = clock();
	for (int i = 0; i < N / 10; i++) {
		int key = rand();
		if (CCSet_Contains(set, &key)) {
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
		if (CCSet_Erase(set, &key)) {
			erased++;
		}
	}
	end = clock();
	printf("[Erase] %d attempts, erased = %d, time = %.3f sec, final size = %zu\n",
	       N / 20, erased, (double)(end - start) / CLOCKS_PER_SEC,
	       CCSet_Size(set));

	/* ---------------- 边界情况测试 ---------------- */
	printf("\n--- Edge Case Test ---\n");

	int key = 42;
	CCSet_Insert(set, &key);
	CCSet_Insert(set, &key); // 重复插入
	printf("Duplicate insert(42): %s\n",
	       CCSet_Contains(set, &key) ? "FOUND" : "NOT FOUND");

	int not_exist = 99;
	int erased2 = CCSet_Erase(set, &not_exist);
	printf("Erase non-existing(99): %s\n", erased2 ? "ERASED" : "NOT FOUND");

	int null_erase = CCSet_Erase(NULL, &key);
	printf("Erase on NULL set: %d\n", null_erase);

	int null_contains = CCSet_Contains(NULL, &key);
	printf("Contains on NULL set: %d\n", null_contains);

	int extreme1 = INT_MIN;
	int extreme2 = INT_MAX;
	CCSet_Insert(set, &extreme1);
	CCSet_Insert(set, &extreme2);
	printf("Find INT_MIN: %s\n",
	       CCSet_Contains(set, &extreme1) ? "FOUND" : "NOT FOUND");
	printf("Find INT_MAX: %s\n",
	       CCSet_Contains(set, &extreme2) ? "FOUND" : "NOT FOUND");

	/* ---------------- 结束清理 ---------------- */
	CCSet_Destroy(set);
	printf("\nAll tests finished, memory released.\n");

	return 0;
}