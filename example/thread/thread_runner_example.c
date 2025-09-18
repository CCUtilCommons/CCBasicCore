#include "thread.h"
#include <stdio.h>
#include <string.h>

void* thread_func(void* arg) {
	int val = *(int*)arg;
	printf("[thread] running with arg = %d\n", val);

	for (int i = 0; i < 3; i++) {
		printf("[thread] id=%llu, step=%d\n",
		       (unsigned long long)CCBasicCoreThread_RequestCurrentID(), i);
		CCBasicCoreThread_SleepMS(500);
	}

	return NULL;
}

int main() {
	int arg = 42;

	CCThread* t = CCBasicCore_CreateThread(
	    thread_func,
	    &arg,
	    sizeof(arg),
	    NULL, // 默认的 args_process_package
	    NULL // 默认 settings
	);

	if (!t) {
		printf("thread create failed!\n");
		return -1;
	}

	printf("[main] thread created, id=%llu, joinable=%d\n",
	       (unsigned long long)CCBasicCoreThread_RequestID(t),
	       CCBasicCoreThread_Joinable(t));

	// 测试 yield
	printf("[main] yielding CPU...\n");
	CCBasicCoreThread_Yield();

	// 测试 join
	CCBasicCoreThread_JoinThread(t);

	// 测试 release
	CCBasicCoreThread_Release(t);

	printf("[main] done.\n");
	return 0;
}
