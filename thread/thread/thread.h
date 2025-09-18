#ifndef CCTHREAD_H
#define CCTHREAD_H
#include "thread_defs.h"
typedef struct __SpecialDefinedPack CCSpecialDefinedPack;
#include <stddef.h>
#include <stdint.h>

typedef struct CCThread {
	CCThread_t implClass;
} CCThread;

CCThread* CCBasicCore_CreateThread(
    CCThreadTask task,
    CCThreadArg_t args,
    size_t arg_size,
    CCSpecialDefinedPack* args_process_package,
    CCThreadSettings* settings);

int CCBasicCoreThread_Joinable(CCThread* impl);

int CCBasicCoreThread_JoinThread(
    CCThread* impl);

int CCBasicCoreThread_DetachThread(
    CCThread* impl);

CCThreadID_t CCBasicCoreThread_RequestID(CCThread* impl);

/**
 * @brief Get current Thread ID
 *
 * @return CCThreadID_t
 */
CCThreadID_t CCBasicCoreThread_RequestCurrentID(void);

void* CCBasicCoreThread_NativeHandle(CCThread* impl);

void CCBasicCoreThread_SleepMS(uint32_t ms);
void CCBasicCoreThread_Yield(void);

// default is Join the Thread, if joinable!
void CCBasicCoreThread_Release(CCThread* impl);

#endif