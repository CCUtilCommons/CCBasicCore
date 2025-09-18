#include "thread.h"
#include "memory_allocation.h"
#include "memory_allocation_tools.h"
#include "thread_impl_interface.h"
CCThread* CCBasicCore_CreateThread(
    CCThreadTask task,
    CCThreadArg_t args,
    size_t arg_size,
    CCSpecialDefinedPack* args_process_package,
    CCThreadSettings* settings) {
	CCThread* t = allocate_one(CCThread);
	t->implClass = CCBasicCoreThreadIMPL_Create(
	    task, args, arg_size, args_process_package, settings);
	if (!t->implClass) {
		free_memory(t);
		return NULL;
	}
	return t;
}

int CCBasicCoreThread_Joinable(CCThread* impl) {
	return CCBasicCoreThreadIMPL_Joinable(impl->implClass);
}

int CCBasicCoreThread_JoinThread(
    CCThread* impl) {
	return CCBasicCoreThreadIMPL_JoinThread(impl->implClass);
}

int CCBasicCoreThread_DetachThread(
    CCThread* impl) {
	return CCBasicCoreThreadIMPL_DetachThread(impl->implClass);
}

CCThreadID_t CCBasicCoreThread_RequestID(CCThread* impl) {
	return CCBasicCoreThreadIMPL_RequestID(impl->implClass);
}

/**
 * @brief Get current Thread ID
 *
 * @return CCThreadID_t
 */
CCThreadID_t CCBasicCoreThread_RequestCurrentID(void) {
	return CCBasicCoreThreadIMPL_RequestCurrentID();
}

void* CCBasicCoreThread_NativeHandle(CCThread* impl) {
	return CCBasicCoreThreadIMPL_NativeHandle(impl->implClass);
}

void CCBasicCoreThread_SleepMS(uint32_t ms) {
	CCBasicCoreThreadIMPL_SleepMS(ms);
}

void CCBasicCoreThread_Yield(void) {
	CCBasicCoreThreadIMPL_Yield();
}

// default is Join the Thread, if joinable!
void CCBasicCoreThread_Release(CCThread* impl) {
	if (!impl)
		return;
	CCBasicCoreThreadIMPL_Release(impl->implClass);
	free_memory(impl);
}