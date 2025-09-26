#include "cond_var.h"
#include "thread.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep
static CConditionalVariable* g_cv = NULL;
static int g_ready = 0;

void* worker_thread(void* arg) {
	printf("[worker] waiting for signal...\n");
	CCCoreBasicConditionalVariable_Wait(g_cv);

	printf("[worker] got signal, g_ready=%d\n", g_ready);
	return NULL;
}

int main() {
	CCThread* thread = CCBasicCore_CreateThread(
	    worker_thread, NULL, 0, NULL, NULL);
	g_cv = CCCoreBasicConditionalVariable_Create();
	if (!g_cv) {
		fprintf(stderr, "Failed to create CConditionalVariable\n");
		return EXIT_FAILURE;
	}

	CCBasicCoreThread_SleepMS(2000);

	g_ready = 1;
	printf("[main] notify worker...\n");

	CCCoreBasicConditionalVariable_NotifyOne(g_cv);

	CCBasicCoreThread_JoinThread(thread);

	CCCoreBasicConditionalVariableFree(g_cv);
	g_cv = NULL;

	printf("[main] test finished.\n");
	return EXIT_SUCCESS;
}
