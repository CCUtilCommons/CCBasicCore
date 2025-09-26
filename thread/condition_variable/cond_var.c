#include "cond_var.h"
#include "cond_var_impl.h"
#include "memory_allocation.h"
#include "memory_allocation_tools.h"

/**
 * @brief Create a Mutex object
 *
 * @return CConditionalVariable*
 */
CConditionalVariable* CCCoreBasicConditionalVariable_Create() {
	CConditionalVariable* cond = allocate_one(CConditionalVariable);
	cond->impl = CCBasicCoreConditionVariableIMPL_Create();
	return cond;
}

/**
 * @brief lock A Lock
 *
 * @param impl
 */
void CCCoreBasicConditionalVariable_Wait(CConditionalVariable* impl) {
	CCBasicCoreConditionVariableIMPL_Wait(impl->impl);
}

/**
 * @brief Unlock A Lock
 *
 * @param impl
 */
void CCCoreBasicConditionalVariable_NotifyOne(CConditionalVariable* impl) {
	CCBasicCoreConditionVariableIMPL_NotifyOne(impl->impl);
}

/**
 * @brief Attempt to lock if the lock if lockable
 *        you should always check if is not 0 to process sth
 *
 * @param impl
 */
void CCCoreBasicConditionalVariable_NotifyAll(CConditionalVariable* impl) {
	CCBasicCoreConditionVariableIMPL_NotifyAll(impl->impl);
}

/**
 * @brief Free A Lock
 *
 * @param impl
 * @return int
 */
int CCCoreBasicConditionalVariableFree(CConditionalVariable* impl) {
	CCBasicCoreConditionVariableIMPL_Free(impl->impl);
	return free_memory(impl);
}