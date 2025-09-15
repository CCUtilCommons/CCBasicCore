#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_interface.h"
#include "queue_list.h"
#include "queue_vector.h"

static void test_queue(CCQueue* q, const char* name) {
    printf("===== Testing %s =====\n", name);

    // Basic push
    for (int i = 1; i <= 5; i++) {
        CCBasicCore_CCQueuePush(q, &i, sizeof(int));
        printf("Pushed: %d\n", i);
    }

    // Check front
    int* front = (int*)CCBasicCore_CCQueueFront(q);
    if (front) {
        printf("Front element: %d\n", *front);
    }

    // Pop elements
    while (!CCBasicCore_CCQueueEmpty(q)) {
        int* val = (int*)CCBasicCore_CCQueuePop(q);
        if (val) {
            printf("Popped: %d\n", *val);
        }
    }

    // Edge case: pop from empty
    int* emptyPop = (int*)CCBasicCore_CCQueuePop(q);
    printf("Pop on empty queue -> %s\n", emptyPop ? "NOT NULL (BUG)" : "NULL (OK)");

    CCBasicCore_CCQueueDestroy(q);
    printf("===== Finished %s =====\n\n", name);
}

static void stress_test(CCQueue* q, const char* name, int N) {
    printf("===== Stress Test %s (%d elements) =====\n", name, N);

    // Push N elements
    for (int i = 0; i < N; i++) {
        CCBasicCore_CCQueuePush(q, &i, sizeof(int));
    }

    // Validate size
    size_t sz = CCBasicCore_CCQueueSize(q);
    printf("Size after push: %zu\n", sz);

    // Pop all elements
    int success = 1;
    for (int i = 0; i < N; i++) {
        int* val = (int*)CCBasicCore_CCQueuePop(q);
        if (!val || *val != i) {
            printf("Error: expected %d, got %d\n", i, val ? *val : -1);
            success = 0;
            break;
        }
    }

    if (success) {
        printf("Stress test PASSED.\n");
    } else {
        printf("Stress test FAILED.\n");
    }

    CCBasicCore_CCQueueDestroy(q);
    printf("===== Finished Stress Test %s =====\n\n", name);
}

int main() {
    // Vector-backed queue test
    CCQueue* vectorQueue = CCBasicCore_CCVectorQueueCreate(sizeof(int));
    test_queue(vectorQueue, "VectorQueue");

    // List-backed queue test
    CCQueue* listQueue = CCBasicCore_CCListQueueCreate(sizeof(int));
    test_queue(listQueue, "ListQueue");

    // Stress test for vector
    CCQueue* stressVector = CCBasicCore_CCVectorQueueCreate(sizeof(int));
    stress_test(stressVector, "VectorQueue", 100000);

    // Stress test for list
    CCQueue* stressList = CCBasicCore_CCListQueueCreate(sizeof(int));
    stress_test(stressList, "ListQueue", 100000);

    return 0;
}
