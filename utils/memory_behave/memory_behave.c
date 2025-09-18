#include "memory_behave.h"
#include "memory_allocation.h"
#include "utils_def.h"
#include <string.h>
void* ccmemory_copy(
    void* elem, size_t elem_size,
    CCSpecialDefinedPack* pack) {
	if (CCBasicCoreCCSpecialDefinedPack_OwnsSpecialCopy(pack)) {
		return pack->copier(elem);
	} else {
		void* result = allocate_memory(elem_size);
		memcpy(result, elem, elem_size);
		return result;
	}
}

int release_memory(void* elem, CCSpecialDefinedPack* pack) {
	if (CCBasicCoreCCSpecialDefinedPack_OwnsSpecialFree(pack)) {
		return pack->freer(elem);
	} else {
		return free_memory(elem);
	}
}
