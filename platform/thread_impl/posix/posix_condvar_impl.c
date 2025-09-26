#include "cond_var_impl.h"
#include "memory_allocation.h"
#include "memory_allocation_tools.h"
#include <limits.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct CConditionalVariable_IMPL {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int pending;
} CConditionalVariable_IMPL;

/**
 * @brief Create a impl
 *
 * @return CConditionalVariable_IMPL*
 */
CConditionalVariable_IMPL* CCBasicCoreConditionVariableIMPL_Create() {
	CConditionalVariable_IMPL* impl = allocate_one(CConditionalVariable_IMPL);
	if (pthread_mutex_init(&impl->mutex, NULL) != 0) {
		free_memory(impl);
		return NULL;
	}
	if (pthread_cond_init(&impl->cond, NULL) != 0) {
		pthread_mutex_destroy(&impl->mutex);
		free_memory(impl);
		return NULL;
	}
	impl->pending = 0;
	return impl;
}

/**
 * @brief Wait the conditions
 *
 * @param impl
 */
void CCBasicCoreConditionVariableIMPL_Wait(CConditionalVariable_IMPL* impl) {
	pthread_mutex_lock(&impl->mutex);
	while (impl->pending == 0) {
		pthread_cond_wait(&impl->cond, &impl->mutex);
	}
	impl->pending--; // 消费一个信号
	pthread_mutex_unlock(&impl->mutex);
}

void CCBasicCoreConditionVariableIMPL_NotifyOne(CConditionalVariable_IMPL* impl) {
	pthread_mutex_lock(&impl->mutex);
	impl->pending++;
	pthread_cond_signal(&impl->cond);
	pthread_mutex_unlock(&impl->mutex);
}

void CCBasicCoreConditionVariableIMPL_NotifyAll(CConditionalVariable_IMPL* impl) {
	pthread_mutex_lock(&impl->mutex);
	impl->pending = INT_MAX / 2; // 确保足够多
	pthread_cond_broadcast(&impl->cond);
	pthread_mutex_unlock(&impl->mutex);
}
/**
 * @brief Free a var
 *
 * @param impl
 * @return int
 */
int CCBasicCoreConditionVariableIMPL_Free(CConditionalVariable_IMPL* impl) {
	pthread_mutex_destroy(&impl->mutex);
	pthread_cond_destroy(&impl->cond);
	free_memory(impl);
	return 1;
}
