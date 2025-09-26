#include "memory_allocation.h"
#include "memory_allocation_tools.h"
#include "mutex_impl.h"
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct CCMutex_Impl {
	pthread_mutex_t mutex;
} CCMutex_Impl;

CCMutex_Impl* CCCoreBasicMutexIMPL_createMutex() {
	CCMutex_Impl* mutex = allocate_one(CCMutex_Impl);
	pthread_mutex_init(&mutex->mutex, NULL);
	return mutex;
}

void CCCoreBasicMutexIMPL_lockMutex(CCMutex_Impl* impl) {
	pthread_mutex_lock(&impl->mutex);
}

void CCCoreBasicMutexIMPL_unlockMutex(CCMutex_Impl* impl) {
	pthread_mutex_unlock(&impl->mutex);
}

int CCCoreBasicMutexIMPL_trylockMutex(CCMutex_Impl* impl) {
	return pthread_mutex_trylock(&impl->mutex) != EBUSY;
}

int CCCoreBasicMutexIMPL_FreeMutex(CCMutex_Impl* impl) {
	pthread_mutex_destroy(&impl->mutex);
	free_memory(impl);
	return 1;
}