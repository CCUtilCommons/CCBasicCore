#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "mutex.h"

// -------------------- 工具函数 --------------------
static double now_sec() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1e6;
}

// -------------------- 功能测试 --------------------
void test_basic() {
	printf("=== Basic Functionality Test ===\n");
	CCMutex* m = CCCoreBasicMutex_createMutex();

	printf("Locking mutex...\n");
	CCCoreBasicMutex_lockMutex(m);
	printf("Locked.\n");

	printf("Unlocking mutex...\n");
	CCCoreBasicMutex_unlockMutex(m);
	printf("Unlocked.\n");

	printf("Trylock mutex (should succeed)...\n");
	errno = 0;
	CCCoreBasicMutex_trylockMutex(m);
	if (errno == 0) {
		printf("Trylock succeeded.\n");
	} else {
		printf("Trylock failed, errno=%d\n", errno);
	}
	CCCoreBasicMutex_unlockMutex(m);

	CCCoreBasicMutex_freeMutex(m);
	printf("Basic test passed.\n\n");
}

// -------------------- 压力测试：多线程累加 --------------------
static int g_counter = 0;
static CCMutex* g_mutex = NULL;

void* worker_inc(void* arg) {
	int loops = (int)(intptr_t)arg;
	for (int i = 0; i < loops; i++) {
		CCCoreBasicMutex_lockMutex(g_mutex);
		g_counter++;
		CCCoreBasicMutex_unlockMutex(g_mutex);
	}
	return NULL;
}

void test_stress(int nthreads, int loops) {
	printf("=== Stress Test: %d threads × %d loops ===\n", nthreads, loops);
	g_counter = 0;
	g_mutex = CCCoreBasicMutex_createMutex();

	pthread_t* ths = (pthread_t*)malloc(sizeof(pthread_t) * nthreads);


    
	double t0 = now_sec();
	for (int i = 0; i < nthreads; i++) {
		pthread_create(&ths[i], NULL, worker_inc, (void*)(intptr_t)loops);
	}
	for (int i = 0; i < nthreads; i++) {
		pthread_join(ths[i], NULL);
	}
	double t1 = now_sec();

	printf("Expected=%d, Actual=%d, Time=%.3f s\n\n",
	       nthreads * loops, g_counter, t1 - t0);

	CCCoreBasicMutex_freeMutex(g_mutex);
	free(ths);
}

// -------------------- 性能测试：单线程锁开销 --------------------
void test_perf(int iterations) {
	printf("=== Perf Test: %d lock/unlock in 1 thread ===\n", iterations);
	CCMutex* m = CCCoreBasicMutex_createMutex();
	double t0 = now_sec();
	for (int i = 0; i < iterations; i++) {
		CCCoreBasicMutex_lockMutex(m);
		CCCoreBasicMutex_unlockMutex(m);
	}
	double t1 = now_sec();
	double ns_per_op = (t1 - t0) * 1e9 / iterations / 2; // 每次lock或unlock的平均耗时

	printf("Time=%.6f s, avg=%.1f ns per lock/unlock\n\n", t1 - t0, ns_per_op);
	CCCoreBasicMutex_freeMutex(m);
}

// -------------------- 主函数 --------------------
int main() {
	test_basic();

	test_stress(4, 500000); // 4线程，每线程50万次加锁
	test_stress(8, 200000); // 8线程，每线程20万次加锁

	test_perf(2000000); // 单线程200万次lock/unlock

	printf("All tests finished.\n");
	return 0;
}
