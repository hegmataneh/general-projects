#pragma once

#if defined Uses_dyn_arr || !defined __COMPILING

// prevent memory fragment
// Memory move unsafe
// Warning . After realloc, the memory block may move, so any previously stored pointer to its data becomes invalid

typedef struct
{
	void * data;        // raw buffer
	size_t item_size;  // size of one element
	size_t capacity;   // allocated slots
	size_t count;      // number of used items
	size_t growStep;
	int allocated_pointer_keeper; // array should delete these pointer
} dyn_arr;

// Initialize with given item_size
status array_init( dyn_arr * arr , size_t item_size , size_t init_capacity , size_t growStep , size_t init_occopied_count );

// each item is address of allocated memory and array keep it and should free it at last
// Important: When instances are destroyed, their destructor is not invoked.
// You are responsible for manually deallocating any memory allocated within the class to prevent memory leaks.
//status alc_ptr_array_init( dyn_arr * arr , size_t growStep );

// Free resources
void array_free( dyn_arr * arr );

// Add item (auto-expands capacity if needed)
status array_add( dyn_arr * arr , void * item );

// just return empty item that currently could be used
_MEMMOVE_UNSAFE_FXN status array_get_one_available_unoccopied_item( dyn_arr * arr , void ** item );

// Delete item at index (shift remaining)
status array_delete( dyn_arr * arr , size_t index );

// Resize to at least new_capacity
status array_resize( dyn_arr * arr , size_t new_capacity , size_t new_used_count /*=0 to just reserve expand reserve capacity*/);

// Get number of active items
size_t array_get_count( dyn_arr * arr );

Boolean array_idx_exist( dyn_arr * arr , size_t idx );

// Get pointer to item at index (NULL if out of range)
_MEMMOVE_UNSAFE_FXN void * array_get( dyn_arr * arr , size_t index );

// securly Get pointer to item at index
_MEMMOVE_UNSAFE_FXN status array_get_s( dyn_arr * arr , size_t index , void ** pitem );

// Set item at index (returns -1 if out of range)
status array_set( dyn_arr * arr , size_t index , void * item );

#endif
