#include "thread.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STRESS_THREAD_COUNT 1000

// ----------------- 测试线程函数 -----------------
void* test_thread_func(void* arg) {
	int val = arg ? *(int*)arg : 0;
	printf("[thread] running with arg = %d, id=%llu\n",
	       val, (unsigned long long)CCBasicCoreThread_RequestCurrentID());

	for (int i = 0; i < 3; i++) {
		printf("[thread] id=%llu, step=%d\n",
		       (unsigned long long)CCBasicCoreThread_RequestCurrentID(), i);
		CCBasicCoreThread_SleepMS(100); // 模拟耗时任务
	}
	return NULL;
}

// ----------------- 边缘测试 -----------------
void edge_tests() {
	printf("=== Edge Tests ===\n");

	// NULL 参数
	CCThread* t1 = CCBasicCore_CreateThread(NULL, NULL, 0, NULL, NULL);
	printf("CreateThread(NULL) returned: %p\n", (void*)t1);

	// arg_size = 0
	int dummy = 123;
	CCThread* t2 = CCBasicCore_CreateThread(test_thread_func, &dummy, 0, NULL, NULL);
	if (t2) {
		CCBasicCoreThread_JoinThread(t2);
		CCBasicCoreThread_Release(t2);
	}

	CCThread* t3 = CCBasicCore_CreateThread(test_thread_func, &dummy, sizeof(dummy), NULL, NULL);
	if (t3) {
		CCBasicCoreThread_Release(t3);
	}
}

// ----------------- 功能测试 -----------------
void functionality_tests() {
	printf("\n=== Functionality Tests ===\n");

	int arg = 42;
	CCThread* t = CCBasicCore_CreateThread(test_thread_func, &arg, sizeof(arg), NULL, NULL);
	if (!t) {
		printf("thread create failed!\n");
		return;
	}

	printf("[main] thread created, id=%llu, joinable=%d\n",
	       (unsigned long long)CCBasicCoreThread_RequestID(t),
	       CCBasicCoreThread_Joinable(t));

	void* handle = CCBasicCoreThread_NativeHandle(t);
	printf("[main] native handle: %p\n", handle);

	CCBasicCoreThread_Yield();
	CCBasicCoreThread_JoinThread(t);
	CCBasicCoreThread_Release(t);
}

// ----------------- 压力/并发测试 -----------------
void stress_test() {
	printf("\n=== Stress Test ===\n");

	CCThread* threads[STRESS_THREAD_COUNT];
	int args[STRESS_THREAD_COUNT];

	for (int i = 0; i < STRESS_THREAD_COUNT; i++) {
		args[i] = i;
		threads[i] = CCBasicCore_CreateThread(test_thread_func, &args[i], sizeof(int), NULL, NULL);
		if (!threads[i]) {
			printf("thread %d create failed!\n", i);
		}
	}

	for (int i = 0; i < STRESS_THREAD_COUNT; i++) {
		if (threads[i])
			CCBasicCoreThread_JoinThread(threads[i]);
		if (threads[i])
			CCBasicCoreThread_Release(threads[i]);
	}

	printf("[stress_test] all threads finished\n");
}

// ----------------- 性能测试 -----------------
void performance_test() {
	printf("\n=== Performance Test ===\n");

	clock_t start = clock();
	CCThread* t = CCBasicCore_CreateThread(test_thread_func, NULL, 0, NULL, NULL);
	if (t) {
		CCBasicCoreThread_JoinThread(t);
		CCBasicCoreThread_Release(t);
	}
	clock_t end = clock();
	double ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
	printf("[perf_test] single thread create + join + release = %.3f ms\n", ms);
}

// ----------------- 主函数 -----------------
int main() {
	edge_tests();
	functionality_tests();
	performance_test();
	stress_test();

	printf("\nAll tests done.\n");
	return 0;
}
