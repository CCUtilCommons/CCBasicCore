#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "result_pack.h"
#include "Error.h"

#define N 1000000  // 压测元素数量

void basic_test() {
    printf("==== Basic Function Test ====\n");

    CCVector* v = CCBasicCoreVector_CreateVector(sizeof(int));

    // push 10 elements
    for (int i = 0; i < 10; i++) {
        __CCBasicCoreVector_PushBack(v, &i, sizeof(int));
    }

    // check size
    printf("Size after 10 push: %zu\n", v->current_size);

    // get and print
    for (int i = 0; i < 10; i++) {
        struct ResultPack* res = CCBasicCoreVector_Get(v, i);
        if(res->status_code == CCBasicCore_SUCCESS){
            printf("%d ", *(int*)res->result);
        } else {
            printf("[ERR]");
        }
    }
    printf("\n");

    // set element 5 -> 999
    int val = 999;
    __CCBasicCoreVector_Set(v, 5, &val, sizeof(int));

    struct ResultPack* res = CCBasicCoreVector_Get(v, 5);
    printf("Element[5] after set = %d\n", *(int*)res->result);

    // pop elements
    for (int i = 0; i < 5; i++) {
        CCBasicCoreVector_PopBack(v);
    }
    printf("Size after 5 pop: %zu\n", v->current_size);

    CCBasicCoreVector_FreeVector(v);
    printf("==== Basic Test Done ====\n\n");
}

void stress_test() {
    printf("==== Stress Test ====\n");

    CCVector* v = CCBasicCoreVector_CreateVector(sizeof(int));

    clock_t start = clock();

    // push N elements
    for (int i = 0; i < N; i++) {
        __CCBasicCoreVector_PushBack(v, &i, sizeof(int));
    }

    clock_t mid = clock();

    // pop N elements
    for (int i = 0; i < N; i++) {
        CCBasicCoreVector_PopBack(v);
    }

    clock_t end = clock();

    double push_time = (double)(mid - start) / CLOCKS_PER_SEC;
    double pop_time  = (double)(end - mid) / CLOCKS_PER_SEC;

    printf("Push %d elements: %.3f sec\n", N, push_time);
    printf("Pop  %d elements: %.3f sec\n", N, pop_time);
    printf("Final size: %zu\n", v->current_size);

    CCBasicCoreVector_FreeVector(v);
    printf("==== Stress Test Done ====\n\n");
}

void perf_compare_array() {
    printf("==== Performance Compare (Vector vs Raw Array) ====\n");

    clock_t start, end;

    // vector push
    CCVector* v = CCBasicCoreVector_CreateVector(sizeof(int));
    start = clock();
    for (int i = 0; i < N; i++) {
        __CCBasicCoreVector_PushBack(v, &i, sizeof(int));
    }
    end = clock();
    double vector_time = (double)(end - start) / CLOCKS_PER_SEC;

    // raw array
    int* arr = (int*)malloc(N * sizeof(int));
    start = clock();
    for (int i = 0; i < N; i++) {
        arr[i] = i;
    }
    end = clock();
    double raw_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Vector push %d elements: %.3f sec\n", N, vector_time);
    printf("Raw array write %d elements: %.3f sec\n", N, raw_time);

    CCBasicCoreVector_FreeVector(v);
    free(arr);
    printf("==== Performance Compare Done ====\n\n");
}

int main() {
    basic_test();
    stress_test();
    perf_compare_array();
    return 0;
}
