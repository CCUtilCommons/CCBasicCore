#ifndef MEMORY_ALLOCATION_TOOLS_H
#define MEMORY_ALLOCATION_TOOLS_H

#define allocate_one(ElementType) (ElementType*)allocate_memory(sizeof(ElementType))

#define get_operating_offset(base_addr, element_size, element_num) \
	((char*)base_addr + element_size * element_num)

#define allocate_zero_memory(buffer_ptr, buffer_size) \
	do {                                              \
		buffer_ptr = allocate_memory(buffer_size);    \
		memset(buffer_ptr, 0, buffer_size);           \
	} while (0);

#define allocate_array_memory(elem_count, elem_type) \
	allocate_memory(elem_count * sizeof(elem_type));

#define allocate_zero_arraymemory(buffer_ptr, elem_count, elem_type) \
	do {                                                             \
		buffer_ptr = allocate_array_memory(elem_count, elem_type);   \
		memset(buffer_ptr, 0, (elem_count * sizeof(elem_type)));     \
	} while (0);

#endif