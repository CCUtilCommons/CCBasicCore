#include "deque_interface.h"
#include "deque_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static void test_basic_vector_deque() {
    printf("===== Basic Test: VectorDeque =====\n");
    CCDeque* dq = CCBasicCore_CCVectorDequeCreate(sizeof(int));
    assert(CCBasicCore_CCDequeEmpty(dq) == 1);

    int a = 10, b = 20, c = 30;
    CCBasicCore_CCDequePushBack(dq, &a, sizeof(int));
    CCBasicCore_CCDequePushBack(dq, &b, sizeof(int));
    CCBasicCore_CCDequePushFront(dq, &c, sizeof(int));

    assert(CCBasicCore_CCDequeSize(dq) == 3);

    int* front = (int*)CCBasicCore_CCDequeFront(dq);
    assert(*front == 30);

    int* pop1 = (int*)CCBasicCore_CCDequePopFront(dq);
    assert(*pop1 == 30);

    int* pop2 = (int*)CCBasicCore_CCDequePopBack(dq);
    assert(*pop2 == 20);

    int* pop3 = (int*)CCBasicCore_CCDequePopFront(dq);
    assert(*pop3 == 10);

    assert(CCBasicCore_CCDequeEmpty(dq) == 1);

    CCBasicCore_CCDequeDestroy(dq);
    printf("VectorDeque basic test PASSED.\n");
}

static void test_stress_vector_deque() {
    printf("===== Stress Test: VectorDeque (100000 elements) =====\n");
    CCDeque* dq = CCBasicCore_CCVectorDequeCreate(sizeof(int));
    int n = 100000;

    for (int i = 0; i < n; i++) {
        CCBasicCore_CCDequePushBack(dq, &i, sizeof(int));
    }
    assert(CCBasicCore_CCDequeSize(dq) == (size_t)n);

    for (int i = 0; i < n; i++) {
        int* val = (int*)CCBasicCore_CCDequePopFront(dq);
        if (*val != i) {
            printf("Error: expected %d, got %d\n", i, *val);
            exit(1);
        }
    }
    assert(CCBasicCore_CCDequeEmpty(dq) == 1);

    CCBasicCore_CCDequeDestroy(dq);
    printf("VectorDeque stress test PASSED.\n");
}

int main() {
    test_basic_vector_deque();
    test_stress_vector_deque();
    return 0;
}
