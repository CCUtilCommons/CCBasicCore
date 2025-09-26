#ifndef CCoreBasicMUTEX_H
#define CCoreBasicMUTEX_H

typedef void* CCMutex_Impl_t;

typedef struct {
	CCMutex_Impl_t implClass;
} CCMutex;

CCMutex* CCCoreBasicMutex_createMutex();
void CCCoreBasicMutex_lockMutex(CCMutex* mutex);
void CCCoreBasicMutex_unlockMutex(CCMutex* mutex);
void CCCoreBasicMutex_trylockMutex(CCMutex* mutex);
int CCCoreBasicMutex_freeMutex(CCMutex* mutex);

#endif