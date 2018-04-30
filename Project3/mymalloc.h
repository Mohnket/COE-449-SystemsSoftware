////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// File Name: mymalloc.h
///
/// @par Full Description
/// COE 449 Project 3
/// Declares a replacement of malloc
///
/// @version
/// 25-Mar-2018 Tyler Mohnke    Initial implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYMALLOC_H
#define MYMALLOC_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: my_worstfit_malloc
///
/// @par Full Description
/// A worst fit implementation of malloc
///
/// @param size     The number of bytes to allocate on the hep
///
/// @return void*   A pointer to the memory block allocated or f no memory could be allocated null
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* my_worstfit_malloc(int size);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: my_free
///
/// @par Full Description
/// 
///
/// @param ptr      A pointer to a block of memory allocated by my_worstfit_malloc
///
/// @return void*   A pointer to the memory block allocated or f no memory could be allocated null
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void my_free(void *ptr);

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
