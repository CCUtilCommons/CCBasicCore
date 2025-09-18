#include "utils_def.h"
#include "memory_allocation.h"

int CCCoreBasic_SimpleFreeBuffer(void* element) {
	return free_memory(element); ///< calling the default free
}

CCSpecialDefinedPack CCSpecialDefinedDefaultPack = {
    CCCoreBasic_SimpleCopyBuffer,
    CCCoreBasic_SimpleFreeBuffer,
    CCCoreBasic_SimpleCompareBuffer
};


CCSpecialDefinedPack* CCBasicCoreCCSpecialDefinedPack_AssignedDefault(
    CCSpecialDefinedPack* pack) {
	pack->compared = CCCoreBasic_SimpleCompareBuffer;
	pack->copier = CCCoreBasic_SimpleCopyBuffer;
	pack->freer = CCCoreBasic_SimpleFreeBuffer;
	return pack;
}