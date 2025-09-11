#include "result_pack.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"
/**
 * @brief assigned pack for the usage
 *
 * @param result the sources address, you must promised it available or not
 * @param code the result code status, if is OK
 * @return ResultPack* pack for the result
 */
ResultPack* CCBasicCoreResultPack_AssignedResultPack(
    Result_t result, StatusCode_t code)
{
    ResultPack* pack = allocate_one(ResultPack);
    pack->result = result;
    pack->status_code = code;
    return pack;
}

/**
 * @brief depack the result
 *
 * @param pack
 * @return Result_t
 */
Result_t CCBasicCoreResultPack_DepackResult(ResultPack* pack){
    return pack->result;
}

/**
 * @brief   depack the result for fetch the code, we recommend using
 *          the interface to query the code
 *
 * @param pack
 * @return StatusCode_t
 */
StatusCode_t CCBasicCoreResultPack_GetCode(ResultPack* pack){
    return pack->status_code;
}

/**
 * @brief free the pack
 *
 * @param pack
 * @return int
 */
int CCBasicCoreResultPack_freePack(ResultPack* pack){
    return free_memory(pack);
}