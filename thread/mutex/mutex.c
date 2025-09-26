#include "mutex.h"
#include "memory_allocation.h"
#include "memory_allocation_tools.h"
#include "mutex_impl.h"

CCMutex* CCCoreBasicMutex_createMutex() {
	CCMutex* mutex = allocate_one(CCMutex);
	mutex->implClass = CCCoreBasicMutexIMPL_createMutex();
	return mutex;
}

void CCCoreBasicMutex_lockMutex(CCMutex* mutex) {
	CCCoreBasicMutexIMPL_lockMutex(mutex->implClass);
}
void CCCoreBasicMutex_unlockMutex(CCMutex* mutex) {
	CCCoreBasicMutexIMPL_unlockMutex(mutex->implClass);
}
void CCCoreBasicMutex_trylockMutex(CCMutex* mutex) {
	CCCoreBasicMutexIMPL_trylockMutex(mutex->implClass);
}

int CCCoreBasicMutex_freeMutex(CCMutex* mutex) {
	CCCoreBasicMutexIMPL_FreeMutex(mutex->implClass);
	return free_memory(mutex);
}