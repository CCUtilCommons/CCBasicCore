#include "Error.h"
#include "memory_allocation.h"
#include "memory_allocation_tools.h"
#include "memory_behave/memory_behave.h"
#include "thread_impl_interface.h"
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct CCThreadImpl {
	unsigned char joinable;
	CCThreadID_t id;
	pthread_t native_posix_thread_handle;
	CCThreadArg_t args;
	CCThreadTask task;
	CCSpecialDefinedPack* args_pack;
	char debug_name[CCThread_NameBufferLen];
} CCThreadImpl;

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
    CCThreadSettings* settings) {
	if (!task)
		return NULL;
	CCThreadImpl* impl = allocate_one(CCThreadImpl);
	impl->args = ccmemory_copy(args, arg_size, args_process_package);
	impl->task = task;
	impl->args_pack = args_process_package;
	impl->joinable = 1;
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);

	if (settings) {
		// valid settings
		if (settings->stack_size) {
			pthread_attr_setstacksize(&thread_attr, settings->stack_size);
		}

		memcpy(impl->debug_name, settings->name_buffer, strlen(settings->name_buffer) + 1);
	}

	int result = pthread_create(
	    &impl->id, &thread_attr,
	    impl->task, impl->args);
	// clean the attr
	pthread_attr_destroy(&thread_attr);

	// set the result
	if (result) {
		release_memory(impl->args, impl->args_pack);
		free_memory(impl);
		return NULL;
	}

	return impl;
}

int CCBasicCoreThreadIMPL_Joinable(CCThreadImpl* impl) {
	return impl && impl->joinable;
}

int CCBasicCoreThreadIMPL_JoinThread(
    CCThreadImpl* impl) {
	if (!CCBasicCoreThreadIMPL_Joinable(impl)) {
		return CCBasicCore_ThreadUnJoinable;
	}

	int result = pthread_join(impl->id, NULL);
	if (!result) {
		impl->joinable = 0;
	}
	return CCBasicCore_SUCCESS;
}

int CCBasicCoreThreadIMPL_DetachThread(
    CCThreadImpl* impl) {
	if (!CCBasicCoreThreadIMPL_Joinable(impl)) {
		return CCBasicCore_ThreadUnJoinable;
	}

	int result = pthread_detach(impl->id);
	if (!result) {
		impl->joinable = 0;
	}
	return CCBasicCore_SUCCESS;
}

CCThreadID_t CCBasicCoreThreadIMPL_RequestID(CCThreadImpl* impl) {
	return impl->id;
}

/**
 * @brief Get current Thread ID
 *
 * @return CCThreadID_t
 */
CCThreadID_t CCBasicCoreThreadIMPL_RequestCurrentID(void) {
	return pthread_self();
}

void* CCBasicCoreThreadIMPL_NativeHandle(CCThreadImpl* impl) {
	return (void*)impl->id;
}

void CCBasicCoreThreadIMPL_SleepMS(uint32_t ms) {
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	nanosleep(&ts, NULL);
}

void CCBasicCoreThreadIMPL_Yield(void) {
	sched_yield();
}

void CCBasicCoreThreadIMPL_Release(CCThreadImpl* impl) {
	if (!impl)
		return;
	if (CCBasicCoreThreadIMPL_Joinable(impl)) {
		CCBasicCoreThreadIMPL_JoinThread(impl);
	}

	// OK, release the sources
	release_memory(impl->args, impl->args_pack);
	free_memory(impl);
}

#undef DEBUG_NAME_LENGTH