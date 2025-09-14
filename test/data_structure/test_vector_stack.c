// test_vector_stack.c
// Interface / edge / stress tests for Vector-backed CCStack ONLY.
// Each CCStack instance is created with one elem_size and only used with that size.
//
// Compile example:
//   gcc -std=c11 -O2 -I. test_vector_stack.c stack_impl.o -o test_vector_stack
//
// Run:
//   ./test_vector_stack

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stack_vector.h"
#include "stack_interface.h"
#define OK(fmt, ...)  do{ printf("[OK]  " fmt "\n", ##__VA_ARGS__); }while(0)
#define FAIL(fmt, ...) do{ printf("[FAIL] " fmt "\n", ##__VA_ARGS__); failures++; }while(0)

static int failures = 0;

struct big_struct {
    int a;
    double b;
    char buf[64];
};

static void run_basic_int_tests_vector(void) {
    printf("\n=== VectorStack<int> tests ===\n");
    // create a stack that stores ints (elem_size fixed to sizeof(int))
    CCStack* st = CCBasicCore_CreateVectorStack(sizeof(int));
    if (!st) { printf("[ERROR] create vector stack failed\n"); return; }

    int v1 = 1, v2 = 2, v3 = 3;
    CCBasicCore_CCStackPush(st, &v1, sizeof(v1));
    CCBasicCore_CCStackPush(st, &v2, sizeof(v2));
    CCBasicCore_CCStackPush(st, &v3, sizeof(v3));

    if (CCBasicCore_CCStackSize(st) != 3) FAIL("size != 3 after pushes");
    else OK("size == 3 after pushes");

    int *ptop = (int*)CCBasicCore_CCStackTop(st);
    if (!ptop) FAIL("top returned NULL");
    else OK("top returned %d", *ptop);

    // mutate original v3 to check copy vs pointer semantics
    v3 = 999;
    int *ptop2 = (int*)CCBasicCore_CCStackTop(st);
    if (!ptop2) FAIL("top returned NULL after mutate");
    else {
        if (*ptop2 == 999) OK("VectorStack<int> appears to store pointer (top reflected mutation)");
        else if (*ptop2 == 3) OK("VectorStack<int> appears to store copy (top did not change)");
        else FAIL("VectorStack<int> top has unexpected value %d", *ptop2);
    }

    // pop order check LIFO
    int expected[] = {3,2,1};
    for (int i = 0; i < 3; ++i) {
        int *pv = (int*)CCBasicCore_CCStackPop(st);
        if (!pv) { FAIL("pop returned NULL at step %d", i); break; }
        if (*pv != expected[i]) FAIL("pop expected %d got %d", expected[i], *pv);
        else OK("pop returned %d", *pv);
    }

    // empty pop/top
    if (CCBasicCore_CCStackPop(st) == NULL) OK("pop on empty returned NULL (acceptable)");
    else OK("pop on empty returned non-NULL (implementation-defined)");

    if (CCBasicCore_CCStackTop(st) == NULL) OK("top on empty returned NULL (acceptable)");
    else OK("top on empty returned non-NULL (implementation-defined)");

    CCBasicCore_CCStackDestroy(st);
}

static void run_struct_tests_vector(void) {
    printf("\n=== VectorStack<big_struct> tests ===\n");
    CCStack* st = CCBasicCore_CreateVectorStack(sizeof(struct big_struct));
    if (!st) { printf("[ERROR] create vector stack for struct failed\n"); return; }

    struct big_struct s = { .a = 7, .b = 1.23 };
    strcpy(s.buf, "hello");

    CCBasicCore_CCStackPush(st, &s, sizeof(s));
    // mutate original s
    s.a = 77; strcpy(s.buf, "world");

    struct big_struct *top = (struct big_struct*)CCBasicCore_CCStackTop(st);
    if (!top) { FAIL("top returned NULL for struct"); }
    else {
        if (top->a == 7 && strcmp(top->buf, "hello") == 0) OK("struct was copied on push");
        else if (top->a == 77 && strcmp(top->buf, "world") == 0) OK("struct stored by pointer (mutation visible)");
        else FAIL("struct top has unexpected contents a=%d buf=%s", top->a, top->buf);
    }

    CCBasicCore_CCStackPop(st); // cleanup
    CCBasicCore_CCStackDestroy(st);
}

static double timespec_diff_seconds(const struct timespec *a, const struct timespec *b){
    double A = a->tv_sec + a->tv_nsec * 1e-9;
    double B = b->tv_sec + b->tv_nsec * 1e-9;
    return A - B;
}

static void run_stress_vector(size_t N) {
    printf("\n=== VectorStack<int> stress test N=%zu ===\n", N);
    CCStack* st = CCBasicCore_CreateVectorStack(sizeof(int));
    if (!st) { printf("[ERROR] create vector stack failed\n"); return; }

    struct timespec t0, t1;
    int val;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (size_t i = 0; i < N; ++i) {
        val = (int)i;
        CCBasicCore_CCStackPush(st, &val, sizeof(val));
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double push_time = timespec_diff_seconds(&t1, &t0);
    printf("push: %.6f s (%.3f ops/s)\n", push_time, N / push_time);

    clock_gettime(CLOCK_MONOTONIC, &t0);
    size_t popped = 0;
    while (!CCBasicCore_CCStackEmpty(st)) {
        void* p = CCBasicCore_CCStackPop(st);
        if (p) popped++;
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double pop_time = timespec_diff_seconds(&t1, &t0);
    printf("pop: %.6f s (%.3f ops/s) popped=%zu\n", pop_time, popped / pop_time, popped);

    CCBasicCore_CCStackDestroy(st);
}

int main(void) {
    printf("=== VectorStack tests start ===\n");
    run_basic_int_tests_vector();
    run_struct_tests_vector();

    // stress: default 1e6, but you can reduce
    size_t N = 1000000;
    run_stress_vector(N);

    printf("\n=== VectorStack tests finished. failures=%d ===\n", failures);
    return (failures==0) ? 0 : 1;
}
