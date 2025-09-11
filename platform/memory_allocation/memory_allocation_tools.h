#ifndef MEMORY_ALLOCATION_TOOLS_H
#define MEMORY_ALLOCATION_TOOLS_H

#define allocate_one(ElementType) (ElementType*)allocate_memory(sizeof(ElementType))

#define get_operating_offset(base_addr, element_size, element_num) \
	((char*)base_addr + element_size * element_num)

#endif