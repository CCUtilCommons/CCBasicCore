#include "deque_interface.h"
#include "deque_vector.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "deque_interface.h"
#include "deque_list.h"
#include "deque_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// =================== 边缘接口测试 ===================
static void test_deque_edge(CCDeque* deque, const char* name) {
	printf("==== 边缘测试: %s ====\n", name);

	// 空 deque
	void* val = CCBasicCore_CCDequePopFront(deque);
	printf("PopFront on empty -> %s\n", val ? "NOT NULL (BUG)" : "NULL (OK)");
	val = CCBasicCore_CCDequePopBack(deque);
	printf("PopBack on empty -> %s\n", val ? "NOT NULL (BUG)" : "NULL (OK)");
	printf("Empty check -> %s\n", CCBasicCore_CCDequeEmpty(deque) ? "YES" : "NO");

	// 单元素操作
	int x = 42;
	CCBasicCore_CCDequePushBack(deque, &x, sizeof(int));
	printf("PushBack single element, size=%zu\n", CCBasicCore_CCDequeSize(deque));
	val = CCBasicCore_CCDequeFront(deque);
	printf("Front element: %d\n", val ? *(int*)val : -1);
	val = CCBasicCore_CCDequeBack(deque);
	printf("Back element: %d\n", val ? *(int*)val : -1);
	val = CCBasicCore_CCDequePopFront(deque);
	printf("PopFront single element -> %d\n", val ? *(int*)val : -1);

	printf("Empty after single pop -> %s\n", CCBasicCore_CCDequeEmpty(deque) ? "YES" : "NO");

	CCBasicCore_CCDequeDestroy(deque);
}

// =================== 压力接口测试 ===================
static void stress_test_deque(size_t N, CCDeque* deque, const char* name) {
	printf("\n==== 压力测试: %s (%zu elements) ====\n", name, N);

	clock_t start = clock();

	// PushBack
	for (size_t i = 0; i < N; i++) {
		int* val = malloc(sizeof(int));
		*val = (int)i;
		CCBasicCore_CCDequePushBack(deque, val, sizeof(int));
	}

	// PushFront
	for (size_t i = 0; i < N; i++) {
		int* val = malloc(sizeof(int));
		*val = (int)(i + N);
		CCBasicCore_CCDequePushFront(deque, val, sizeof(int));
	}

	// 随机 PopFront/PopBack
	for (size_t i = 0; i < N; i++) {
		void* val;
		if (rand() % 2)
			val = CCBasicCore_CCDequePopFront(deque);
		else
			val = CCBasicCore_CCDequePopBack(deque);
		if (val)
			free(val);
	}

	clock_t end = clock();
	printf("压力测试结束, size=%zu, 耗时: %.3f 秒\n",
	       CCBasicCore_CCDequeSize(deque),
	       (double)(end - start) / CLOCKS_PER_SEC);

	// 清理剩余
	while (!CCBasicCore_CCDequeEmpty(deque)) {
		void* val = CCBasicCore_CCDequePopFront(deque);
		free(val);
	}

	CCBasicCore_CCDequeDestroy(deque);
}

// =================== 随机操作压力测试 ===================
static void random_ops_deque(size_t N, CCDeque* deque, const char* name) {
	printf("\n==== 随机操作压力测试: %s (%zu ops) ====\n", name, N);
	srand((unsigned)time(NULL));

	for (size_t i = 0; i < N; i++) {
		int op = rand() % 4;
		int val = (int)i;
		switch (op) {
		case 0:
			CCBasicCore_CCDequePushBack(deque, &val, sizeof(int));
			break;
		case 1:
			CCBasicCore_CCDequePushFront(deque, &val, sizeof(int));
			break;
		case 2:
			if (!CCBasicCore_CCDequeEmpty(deque)) {
				void* v = CCBasicCore_CCDequePopBack(deque);
				free(v);
			}
			break;
		case 3:
			if (!CCBasicCore_CCDequeEmpty(deque)) {
				void* v = CCBasicCore_CCDequePopFront(deque);
				free(v);
			}
			break;
		}
	}

	printf("随机操作完成, size=%zu\n", CCBasicCore_CCDequeSize(deque));

	// 清理剩余
	while (!CCBasicCore_CCDequeEmpty(deque)) {
		void* v = CCBasicCore_CCDequePopFront(deque);
		free(v);
	}

	CCBasicCore_CCDequeDestroy(deque);
}

// =================== 主函数 ===================
int main() {
	size_t N = 100000; // 压力测试 10 万元素
	const size_t elem_size = sizeof(int);
	// Vector-backed deque
	CCDeque* vectorDeque = CCBasicCore_CCVectorDequeCreate(elem_size);
	test_deque_edge(vectorDeque, "VectorDeque");
	vectorDeque = CCBasicCore_CCVectorDequeCreate(elem_size);
	stress_test_deque(N, vectorDeque, "VectorDeque");
	vectorDeque = CCBasicCore_CCVectorDequeCreate(elem_size);
	random_ops_deque(N, vectorDeque, "VectorDeque");

	printf("\n==== 所有 CCDeque 接口压力 & 边缘测试完成 ✅ ====\n");
	return 0;
}
