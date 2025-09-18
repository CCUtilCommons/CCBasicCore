#ifndef CCTHREAD_IMPL_HELPER_H
#define CCTHREAD_IMPL_HELPER_H
#include "thread_impl_interface.h"

static inline CCThreadID_t __ccthread_from_ptr_to_id(void* __impl) {
	return (CCThreadID_t)__impl;
}

#endif