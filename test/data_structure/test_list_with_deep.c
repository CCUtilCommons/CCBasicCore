#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

// 打印整数
static void print_int(void* elem) {
    printf("%d ", *(int*)elem);
}

// pack 测试
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

// 基础 push/pop 测试
void test_basic() {
    printf("===== 基础测试: Push/Pop =====\n");
    CCList* list = CCBasicCore_initCCList(sizeof(int));

    int a = 1, b = 2, c = 3;
    CCBasicCore_CCListPushBack(list, &a, sizeof(a));
    CCBasicCore_CCListPushBack(list, &b, sizeof(b));
    CCBasicCore_CCListPushFront(list, &c, sizeof(c));

    printf("当前 size: %zu\n", CCBasicCore_CCListSize(list));
    CCBasicCore_CCListRunIterate(list, print_int);
    printf("\n");

    int* x = (int*)CCBasicCore_CCListPopFront(list);
    printf("PopFront: %d\n", *x);
    x = (int*)CCBasicCore_CCListPopBack(list);
    printf("PopBack: %d\n", *x);

    printf("剩余 size: %zu\n", CCBasicCore_CCListSize(list));
    CCBasicCore_destroyCCList(list);
}

// Insert / Erase 测试
void test_insert_erase() {
    printf("\n===== 插入/删除测试 =====\n");
    CCList* list = CCBasicCore_initCCList(sizeof(int));
    int vals[] = {10, 20, 30, 40};
    for(int i=0;i<4;i++) CCBasicCore_CCListPushBack(list, &vals[i], sizeof(int));

    printf("初始链表: ");
    CCBasicCore_CCListRunIterate(list, print_int);
    printf("\n");

    struct CCListNode* head = CCBasicCore_CCListHead(list);
    struct CCListNode* inserted = CCBasicCore_CCListInsert(list, head, &vals[3], sizeof(int));

    printf("插入后: ");
    CCBasicCore_CCListRunIterate(list, print_int);
    printf("\n");

    CCBasicCore_CCListErase(list, inserted);
    printf("删除后: ");
    CCBasicCore_CCListRunIterate(list, print_int);
    printf("\n");

    CCBasicCore_destroyCCList(list);
}

// pack 测试
void test_pack() {
    printf("\n===== 带 pack 的深拷贝测试 =====\n");
    CCSpecialDefinedPack pack = {
        .copier = int_copier,
        .freer = int_freer,
        .compared = int_compare
    };
    CCList* list = CCBasicCore_initSpecialCCList(sizeof(int), &pack);

    int* heap_num = malloc(sizeof(int));
    *heap_num = 100;
    CCBasicCore_CCListPushBack(list, heap_num, sizeof(int));

    printf("深拷贝测试: ");
    CCBasicCore_CCListRunIterate(list, print_int);
    printf("\n");

    CCBasicCore_destroyCCList(list); // 会调用 int_freer 释放元素
}

// 边界测试
void test_edge() {
    printf("\n===== 边界情况测试 =====\n");
    CCList* list = CCBasicCore_initCCList(sizeof(int));

    printf("空表 size=%zu\n", CCBasicCore_CCListSize(list));
    printf("PopFront(NULL)=%p\n", CCBasicCore_CCListPopFront(list));
    printf("PopBack(NULL)=%p\n", CCBasicCore_CCListPopBack(list));

    int val = 999;
    CCBasicCore_CCListPushBack(list, &val, sizeof(int));
    printf("单节点表 size=%zu\n", CCBasicCore_CCListSize(list));

    int* x = CCBasicCore_CCListPopFront(list);
    printf("PopFront 单节点: %d\n", *x);

    CCBasicCore_destroyCCList(list);
}

// 压力测试
void stress_test(int count) {
    printf("\n===== 压力测试 =====\n");
    CCList* list = CCBasicCore_initCCList(sizeof(int));

    clock_t start = clock();

    // PushBack
    for(int i=0;i<count;i++) {
        int* val = malloc(sizeof(int));
        *val = i;
        CCBasicCore_CCListPushBack(list, val, sizeof(int));
    }

    // PopFront
    long long sum = 0;
    for(int i=0;i<count;i++) {
        int* val = CCBasicCore_CCListPopFront(list);
        sum += *val;
        free(val);
    }

    clock_t end = clock();
    printf("压测完成，sum=%lld, 链表大小=%zu\n", sum, CCBasicCore_CCListSize(list));
    printf("耗时: %.3f 秒\n", (double)(end - start) / CLOCKS_PER_SEC);

    CCBasicCore_destroyCCList(list);
}

// 随机操作测试
void random_ops_test(int count) {
    printf("\n===== 随机操作测试 =====\n");
    CCList* list = CCBasicCore_initCCList(sizeof(int));
    srand((unsigned)time(NULL));

    int* pool = malloc(sizeof(int)*count);

    for(int i=0;i<count;i++) pool[i] = i;

    for(int i=0;i<count;i++) {
        int op = rand() % 4;
        int* val = &pool[i];
        switch(op){
            case 0: CCBasicCore_CCListPushBack(list, val, sizeof(int)); break;
            case 1: CCBasicCore_CCListPushFront(list, val, sizeof(int)); break;
            case 2: if(!CCBasicCore_CCListEmpty(list)) CCBasicCore_CCListPopBack(list); break;
            case 3: if(!CCBasicCore_CCListEmpty(list)) CCBasicCore_CCListPopFront(list); break;
        }
    }

    printf("随机操作完成, 链表大小=%zu\n", CCBasicCore_CCListSize(list));

    while(!CCBasicCore_CCListEmpty(list)) {
        CCBasicCore_CCListPopFront(list);
    }

    CCBasicCore_destroyCCList(list);
    free(pool);
}

int main() {
    test_basic();
    test_insert_erase();
    test_pack();
    test_edge();

    stress_test(1000000); // 压测 1M 次
    random_ops_test(1000000); // 随机操作 1M 次

    printf("\n所有测试完成 ✅\n");
    return 0;
}
