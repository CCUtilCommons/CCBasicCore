#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// =================== pack 函数 ===================
static void* int_copier(const void* src) {
	int* dst = malloc(sizeof(int));
	*dst = *(int*)src;
	return dst;
}
static int int_freer(void* data) {
	free(data);
	return 1;
}
static int int_compare(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}

// =================== 边缘接口测试 ===================
static void test_list_edge_cases() {
	printf("==== 边缘测试: 空链表/单元素/重复操作 ====\n");
	CCList* list = CCBasicCore_initCCList(sizeof(int));

	// 空链表操作
	int* val = CCBasicCore_CCListPopFront(list);
	printf("PopFront on empty -> %s\n", val ? "NOT NULL (BUG)" : "NULL (OK)");
	val = CCBasicCore_CCListPopBack(list);
	printf("PopBack on empty -> %s\n", val ? "NOT NULL (BUG)" : "NULL (OK)");

	// 单元素操作
	int x = 42;
	CCBasicCore_CCListPushBack(list, &x, sizeof(x));
	printf("PushBack single element, size=%zu\n", CCBasicCore_CCListSize(list));
	val = CCBasicCore_CCListPopFront(list);
	printf("PopFront single element -> %d\n", *val);

	// 重复插入/删除
	int y = 99;
	for (int i = 0; i < 5; i++)
		CCBasicCore_CCListPushBack(list, &y, sizeof(y));
	for (int i = 0; i < 5; i++)
		val = CCBasicCore_CCListPopBack(list);
	printf("Repeated insert/pop test size=%zu\n", CCBasicCore_CCListSize(list));

	CCBasicCore_destroyCCList(list);
}

// =================== 压力接口测试 ===================
static void stress_test_list_interfaces(int count) {
	printf("\n==== 压力测试: Push/Pop/Insert/Erase (%d elements) ====\n", count);

	CCSpecialDefinedPack pack = { int_copier, int_freer, int_compare };
	CCList* list = CCBasicCore_initSpecialCCList(sizeof(int), &pack);

	clock_t start = clock();

	// PushBack
	for (int i = 0; i < count; i++) {
		int* val = malloc(sizeof(int));
		*val = i;
		CCBasicCore_CCListPushBack(list, val, sizeof(int));
	}

	// PushFront
	for (int i = 0; i < count; i++) {
		int* val = malloc(sizeof(int));
		*val = i + 1000000;
		CCBasicCore_CCListPushFront(list, val, sizeof(int));
	}

	// Insert at head repeatedly
	struct CCListNode* head = CCBasicCore_CCListHead(list);
	for (int i = 0; i < 1000; i++) {
		int* val = malloc(sizeof(int));
		*val = i + 2000000;
		CCBasicCore_CCListInsert(list, head, val, sizeof(int));
	}

	// PopFront
	for (int i = 0; i < count / 2; i++) {
		int* val = CCBasicCore_CCListPopFront(list);
		free(val);
	}

	// PopBack
	for (int i = 0; i < count / 2; i++) {
		int* val = CCBasicCore_CCListPopBack(list);
		free(val);
	}

	// Erase some nodes in middle
	head = CCBasicCore_CCListHead(list);
	struct CCListNode* node = head;
	for (int i = 0; i < 10 && node; i++, node = CCBasicCore_ListNodeNext(node)) {
		CCBasicCore_CCListErase(list, node);
	}

	clock_t end = clock();
	printf("压力测试结束, size=%zu, 耗时: %.3f 秒\n",
	       CCBasicCore_CCListSize(list),
	       (double)(end - start) / CLOCKS_PER_SEC);

	// 清理剩余
	while (!CCBasicCore_CCListEmpty(list)) {
		int* val = CCBasicCore_CCListPopFront(list);
		free(val);
	}

	CCBasicCore_destroyCCList(list);
}

// =================== 随机操作压力测试 ===================
static void random_ops_list_test(int count) {
	printf("\n==== 随机操作压力测试 (%d ops) ====\n", count);

	CCList* list = CCBasicCore_initCCList(sizeof(int));
	srand((unsigned)time(NULL));

	int* pool = malloc(sizeof(int) * count);
	for (int i = 0; i < count; i++)
		pool[i] = i;

	for (int i = 0; i < count; i++) {
		int op = rand() % 4;
		int* val = &pool[i];
		switch (op) {
		case 0:
			CCBasicCore_CCListPushBack(list, val, sizeof(int));
			break;
		case 1:
			CCBasicCore_CCListPushFront(list, val, sizeof(int));
			break;
		case 2:
			if (!CCBasicCore_CCListEmpty(list))
				CCBasicCore_CCListPopBack(list);
			break;
		case 3:
			if (!CCBasicCore_CCListEmpty(list))
				CCBasicCore_CCListPopFront(list);
			break;
		}
	}

	printf("随机操作测试完成, 链表大小=%zu\n", CCBasicCore_CCListSize(list));

	// 清理
	while (!CCBasicCore_CCListEmpty(list))
		CCBasicCore_CCListPopFront(list);

	CCBasicCore_destroyCCList(list);
	free(pool);
}

// =================== 主函数 ===================
int main() {
	test_list_edge_cases();
	stress_test_list_interfaces(1000000); // 压力测试 100 万
	random_ops_list_test(1000000); // 随机操作 100 万

	printf("\n==== 所有 CCList 接口压力 & 边缘测试完成 ✅ ====\n");
	return 0;
}
