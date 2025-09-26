#ifndef CCBasicCore_CondVar_H
#define CCBasicCore_CondVar_H

typedef void* CConditionalVariableIMPL_t;
typedef struct CConditionalVariable {
	CConditionalVariableIMPL_t impl;
} CConditionalVariable;

/**
 * @brief Create a Cond vars
 *
 * @return CConditionalVariable*
 */
CConditionalVariable* CCCoreBasicConditionalVariable_Create();

/**
 * @brief wait a cv
 *
 * @param impl
 */
void CCCoreBasicConditionalVariable_Wait(CConditionalVariable* impl);

/**
 * @brief notify one cv
 *
 * @param impl
 */
void CCCoreBasicConditionalVariable_NotifyOne(CConditionalVariable* impl);

/**
 * @brief notify all cv
 *
 * @param impl
 */
void CCCoreBasicConditionalVariable_NotifyAll(CConditionalVariable* impl);

/**
 * @brief Free A cv
 *
 * @param impl
 * @return int
 */
int CCCoreBasicConditionalVariableFree(CConditionalVariable* impl);

#endif