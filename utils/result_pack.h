#ifndef RESULT_PACKAGE_H
#define RESULT_PACKAGE_H

typedef void* Result_t;
typedef int StatusCode_t;

typedef struct ResultPack {
	Result_t result;
	StatusCode_t status_code;
} ResultPack;

/**
 * @brief assigned pack for the usage
 *
 * @param result the sources address, you must promised it available or not
 * @param code the result code status, if is OK
 * @return ResultPack pack for the result
 */
ResultPack* CCBasicCoreResultPack_AssignedResultPack(Result_t result, StatusCode_t code);

/**
 * @brief depack the result
 *
 * @param pack
 * @return Result_t
 */
Result_t CCBasicCoreResultPack_DepackResult(ResultPack* pack);

/**
 * @brief   depack the result for fetch the code, we recommend using
 *          the interface to query the code
 *
 * @param pack
 * @return StatusCode_t
 */
StatusCode_t CCBasicCoreResultPack_GetCode(ResultPack* pack);

/**
 * @brief free the pack
 *
 * @param pack
 * @return int
 */
int CCBasicCoreResultPack_freePack(ResultPack* pack);

#endif
