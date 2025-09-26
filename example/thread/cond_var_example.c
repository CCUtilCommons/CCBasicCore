#include "cond_var.h"
#include "mutex.h"
#include "thread.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// -------------------- 全局共享 --------------------
#define BUFFER_SIZE 5
int buffer = 0;
CConditionalVariable* cv_empty;
CConditionalVariable* cv_full;
CCMutex* mutex;

// -------------------- 生产者 --------------------
void* producer(void* arg) {
	for (int i = 1; i <= 10; i++) {
		CCCoreBasicMutex_lockMutex(mutex);
		while (buffer >= BUFFER_SIZE) {
			// 缓冲区满，等待消费者
			CCCoreBasicConditionalVariable_Wait(cv_empty);
		}
		buffer++;
		printf("[Producer] produced, buffer=%d\n", buffer);
		CCCoreBasicMutex_unlockMutex(mutex);

		// 通知消费者
		CCCoreBasicConditionalVariable_NotifyOne(cv_full);
		CCBasicCoreThread_SleepMS(100);
	}
	return NULL;
}

// -------------------- 消费者 --------------------
void* consumer(void* arg) {
	for (int i = 1; i <= 10; i++) {
		CCCoreBasicMutex_lockMutex(mutex);
		while (buffer <= 0) {
			// 缓冲区空，等待生产者
			CCCoreBasicConditionalVariable_Wait(cv_full);
		}
		buffer--;
		printf("[Consumer] consumed, buffer=%d\n", buffer);
		CCCoreBasicMutex_unlockMutex(mutex);

		// 通知生产者
		CCCoreBasicConditionalVariable_NotifyOne(cv_empty);
		CCBasicCoreThread_SleepMS(150);
	}
	return NULL;
}

// -------------------- 主函数 --------------------
int main() {
	mutex = CCCoreBasicMutex_createMutex();
	cv_empty = CCCoreBasicConditionalVariable_Create();
	cv_full = CCCoreBasicConditionalVariable_Create();

	CCThread* prod_thread = CCBasicCore_CreateThread(producer, NULL, 0, NULL, NULL);
	CCThread* cons_thread = CCBasicCore_CreateThread(consumer, NULL, 0, NULL, NULL);

	CCBasicCoreThread_JoinThread(prod_thread);
	CCBasicCoreThread_JoinThread(cons_thread);

	

	CCCoreBasicConditionalVariableFree(cv_empty);
	CCCoreBasicConditionalVariableFree(cv_full);
	CCCoreBasicMutex_freeMutex(mutex);

	printf("Example finished.\n");
	return 0;
}
