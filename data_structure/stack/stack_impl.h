#ifndef CCSTACK_IMPL_H
#define CCSTACK_IMPL_H

/**
 * @brief   impl type code, code should be used in determine
 *          what the actual type it is
 * @note    default library we support vector impl and stack impl
 *          if you owns your data structure, make sure you impl
 *          the relavent interfaces acquired, see stack_interface.h
 *          for details
 */
typedef enum CCStackImplCode {
	CCSTACK_VECTOR_IMPL_CODE,
	CCSTACK_LIST_IMPL_CODE
	// you can add here
} CCStackImplCode_t;




#endif