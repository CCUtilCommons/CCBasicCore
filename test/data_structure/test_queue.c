#include "queue_interface.h"
#include "queue_list.h"
#include "queue_vector.h"
#include <stdio.h>
#include <stdlib.h>

// =================== 测试工具 ===================
static void print_result(const char* test_name, int passed) {
	printf("[TEST] %-50s : %s\n", test_name, passed ? "PASSED" : "FAILED");
}

// =================== 全面接口测试 ===================
static void test_queue_full(CCQueue* q, const char* name) {
	printf("==== %s 全面接口测试 ====\n", name);

	int success = 1;

	// 1. push 一些元素
	for (int i = 0; i < 10; i++) {
		CCBasicCore_CCQueuePush(q, &i, sizeof(int));
	}
	print_result("Push 10 elements", 1);

	// 2. front 测试
	int* front = (int*)CCBasicCore_CCQueueFront(q);
	print_result("Front after push", front && *front == 0);

	// 3. size 测试
	size_t sz = CCBasicCore_CCQueueSize(q);
	print_result("Size after push", sz == 10);

	// 4. empty 测试
	int isEmpty = CCBasicCore_CCQueueEmpty(q);
	print_result("Empty after push", !isEmpty);

	// 5. pop 测试 & 顺序检查
	success = 1;
	for (int i = 0; i < 10; i++) {
		int* val = (int*)CCBasicCore_CCQueuePop(q);
		if (!val || *val != i) {
			success = 0;
		}
	}
	print_result("Pop 10 elements in order", success);

	// 6. empty 再次检查
	print_result("Empty after pop all", CCBasicCore_CCQueueEmpty(q));

	// 7. front on empty
	front = (int*)CCBasicCore_CCQueueFront(q);
	print_result("Front on empty queue", front == NULL);

	// 8. pop on empty
	int* emptyPop = (int*)CCBasicCore_CCQueuePop(q);
	print_result("Pop on empty queue", emptyPop == NULL);

	// 9. Stress test push/pop
	const int N = 100000;
	success = 1;
	for (int i = 0; i < N; i++) {
		CCBasicCore_CCQueuePush(q, &i, sizeof(int));
	}
	print_result("Stress push 100000 elements", 1);

	// 检查 size
	print_result("Size after stress push", CCBasicCore_CCQueueSize(q) == N);

	// Pop 并验证顺序
	success = 1;
	for (int i = 0; i < N; i++) {
		int* val = (int*)CCBasicCore_CCQueuePop(q);
		if (!val || *val != i) {
			success = 0;
		}
	}
	print_result("Stress pop 100000 elements in order", success);

	// Queue should be empty now
	print_result("Empty after stress pop", CCBasicCore_CCQueueEmpty(q));

	// 10. destroy 空队列
	CCBasicCore_CCQueueDestroy(q);
	print_result("Destroy queue safely", 1);

	printf("==== %s 全面接口测试完成 ====\n\n", name);
}

// =================== 主函数 ===================
int main(void) {
	// Vector-backed queue
	CCQueue* vectorQueue = CCBasicCore_CCVectorQueueCreate(sizeof(int));
	test_queue_full(vectorQueue, "VectorQueue");

	// List-backed queue
	CCQueue* listQueue = CCBasicCore_CCListQueueCreate(sizeof(int));
	test_queue_full(listQueue, "ListQueue");

	printf("==== CCQueue 全接口性能 & 鲁棒性测试完成 ====\n");
	return 0;
}
