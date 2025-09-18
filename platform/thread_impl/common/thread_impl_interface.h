#ifndef CCTHREAD_COMMON_INTERFACE_H
#define CCTHREAD_COMMON_INTERFACE_H
#include "thread_defs.h"
#include "utils_def.h"


/**
 * @brief Impl Types for the CCThreadImpl
 *
 */
typedef struct CCThreadImpl CCThreadImpl;

/**
 * @brief
 *
 * @return CCThreadImpl
 */
CCThreadImpl* CCBasicCoreThreadIMPL_Create(
    CCThreadTask task,
    CCThreadArg_t args,
    size_t arg_size,
    CCSpecialDefinedPack* args_process_package,
    CCThreadSettings* settings);

int CCBasicCoreThreadIMPL_Joinable(CCThreadImpl* impl);

int CCBasicCoreThreadIMPL_JoinThread(
    CCThreadImpl* impl);

int CCBasicCoreThreadIMPL_DetachThread(
    CCThreadImpl* impl);

CCThreadID_t CCBasicCoreThreadIMPL_RequestID(CCThreadImpl* impl);

/**
 * @brief Get current Thread ID
 *
 * @return CCThreadID_t
 */
CCThreadID_t CCBasicCoreThreadIMPL_RequestCurrentID(void);

void* CCBasicCoreThreadIMPL_NativeHandle(CCThreadImpl* impl);

void CCBasicCoreThreadIMPL_SleepMS(uint32_t ms);
void CCBasicCoreThreadIMPL_Yield(void);

// default is Join the Thread, if joinable!
void CCBasicCoreThreadIMPL_Release(CCThreadImpl* impl);

#endif