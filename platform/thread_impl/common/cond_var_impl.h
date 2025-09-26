#ifndef CCBasicCore_ConditionVariable_H
#define CCBasicCore_ConditionVariable_H

typedef struct CConditionalVariable_IMPL CConditionalVariable_IMPL;

/**
 * @brief Create a impl
 *
 * @return CConditionalVariable_IMPL*
 */
CConditionalVariable_IMPL* CCBasicCoreConditionVariableIMPL_Create();

/**
 * @brief Wait the conditions
 *
 * @param impl
 */
void CCBasicCoreConditionVariableIMPL_Wait(CConditionalVariable_IMPL* impl);

/**
 * @brief Notify one waiter
 *
 * @param impl
 */
void CCBasicCoreConditionVariableIMPL_NotifyOne(CConditionalVariable_IMPL* impl);

/**
 * @brief Notify all waiter
 *
 * @param impl
 */
void CCBasicCoreConditionVariableIMPL_NotifyAll(CConditionalVariable_IMPL* impl);

/**
 * @brief Free a var
 *
 * @param impl
 * @return int
 */
int CCBasicCoreConditionVariableIMPL_Free(CConditionalVariable_IMPL* impl);

#endif