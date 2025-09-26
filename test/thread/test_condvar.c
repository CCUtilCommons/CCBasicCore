#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "cond_var.h" // 您给的接口头文件

// -------------------- 工具函数 --------------------
static double now_sec() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1e6;
}

// -------------------- 基本功能测试 --------------------
static int g_ready = 0;
static CConditionalVariable* g_cv = NULL;

void* worker_wait(void* arg) {
	printf("[worker] waiting...\n");
	CCCoreBasicConditionalVariable_Wait(g_cv);
	printf("[worker] wakeup, g_ready=%d\n", g_ready);
	return NULL;
}

void test_basic() {
	printf("=== Basic Functionality Test ===\n");
	g_cv = CCCoreBasicConditionalVariable_Create();

	pthread_t th;
	pthread_create(&th, NULL, worker_wait, NULL);

	sleep(1);
	g_ready = 42;
	printf("[main] notify one...\n");
	CCCoreBasicConditionalVariable_NotifyOne(g_cv);

	pthread_join(th, NULL);
	CCCoreBasicConditionalVariableFree(g_cv);
	printf("Basic test done.\n\n");
}

// -------------------- NotifyAll 测试 --------------------
static int g_counter = 0;

void* worker_wait_all(void* arg) {
	CCCoreBasicConditionalVariable_Wait(g_cv);
	__sync_fetch_and_add(&g_counter, 1);
	return NULL;
}

void test_notify_all(int nthreads) {
	printf("=== NotifyAll Test with %d threads ===\n", nthreads);
	g_cv = CCCoreBasicConditionalVariable_Create();
	g_counter = 0;

	pthread_t* ths = malloc(sizeof(pthread_t) * nthreads);
	for (int i = 0; i < nthreads; i++) {
		pthread_create(&ths[i], NULL, worker_wait_all, NULL);
	}

	sleep(1);
	printf("[main] notify all...\n");
	CCCoreBasicConditionalVariable_NotifyAll(g_cv);

	for (int i = 0; i < nthreads; i++) {
		pthread_join(ths[i], NULL);
	}
	printf("All workers awakened, g_counter=%d\n\n", g_counter);

	free(ths);
	CCCoreBasicConditionalVariableFree(g_cv);
}

// -------------------- 性能测试 --------------------
static int perf_loops = 100000;
void* worker_perf(void* arg) {
	for (int i = 0; i < perf_loops; i++) {
		CCCoreBasicConditionalVariable_Wait(g_cv);
	}
	return NULL;
}

void test_perf() {
	printf("=== Perf Test (NotifyOne × %d) ===\n", perf_loops);
	g_cv = CCCoreBasicConditionalVariable_Create();

	pthread_t th;
	pthread_create(&th, NULL, worker_perf, NULL);
	sleep(1); // 确保线程进入等待

	double t0 = now_sec();
	for (int i = 0; i < perf_loops; i++) {
		CCCoreBasicConditionalVariable_NotifyOne(g_cv);
	}
	pthread_join(th, NULL);
	double t1 = now_sec();

	printf("Total time=%.6f s, avg=%.1f us per notify\n\n",
	       t1 - t0, (t1 - t0) * 1e6 / perf_loops);

	CCCoreBasicConditionalVariableFree(g_cv);
}

// -------------------- 压力测试 --------------------
#define STRESS_THREADS 8
#define STRESS_LOOPS 50000

static int stress_counter = 0;
void* worker_stress(void* arg) {
	for (int i = 0; i < STRESS_LOOPS; i++) {
		CCCoreBasicConditionalVariable_Wait(g_cv);
		__sync_fetch_and_add(&stress_counter, 1);
	}
	return NULL;
}

void test_stress() {
	printf("=== Stress Test: %d threads × %d waits ===\n",
	       STRESS_THREADS, STRESS_LOOPS);
	g_cv = CCCoreBasicConditionalVariable_Create();
	stress_counter = 0;

	pthread_t ths[STRESS_THREADS];
	for (int i = 0; i < STRESS_THREADS; i++) {
		pthread_create(&ths[i], NULL, worker_stress, NULL);
	}
	sleep(1);

	double t0 = now_sec();
	for (int i = 0; i < STRESS_THREADS * STRESS_LOOPS; i++) {
		CCCoreBasicConditionalVariable_NotifyOne(g_cv);
	}
	for (int i = 0; i < STRESS_THREADS; i++) {
		pthread_join(ths[i], NULL);
	}
	double t1 = now_sec();

	printf("Expected=%d, Actual=%d, Time=%.3f s\n\n",
	       STRESS_THREADS * STRESS_LOOPS, stress_counter, t1 - t0);

	CCCoreBasicConditionalVariableFree(g_cv);
}

// -------------------- main --------------------
int main() {
	test_basic();
	test_notify_all(4);
	test_perf();
	test_stress();
	printf("All condvar tests finished.\n");
	return 0;
}
