#pragma once

#if defined Uses_dyn_mms_arr || !defined __COMPILING

// Warning . After realloc, the memory block may move, so any previously stored pointer to its data becomes invalid
// zero block at the begining

// not THREAD SAFE

typedef struct
{
	void ** data;      // raw buffer
	size_t item_size;  // size of one element
	size_t capacity;   // allocated slots
	size_t count;      // number of used items
	size_t growStep;
} dyn_mms_arr; // memory move safe array

// Initialize memmove safe dynamic array with given item_size
status mms_array_init( dyn_mms_arr * arr , size_t item_size/*size of one element*/ , size_t init_capacity/*allocated slots*/ ,
	size_t growStep , size_t init_occopied_count/*occupied count*/ );

// Free resources
void mms_array_free( dyn_mms_arr * arr );

// Add item (auto-expands capacity if needed)
status mms_array_add( dyn_mms_arr * arr , void * item /*one item*/ );

// just return empty item that currently could be used
// Warning . After realloc, the memory block may move, so any previously stored pointer to its data becomes invalid
status mms_array_get_one_available_unoccopied_item( dyn_mms_arr * arr , _NEW_OUT_P void ** item );

// pointer to container keep data. it used to assign manual allocated unit to keep in arr
status mms_array_get_one_available_unoccopied_item_holder( dyn_mms_arr * arr , _NEW_OUT_P void *** item /*add you struct pointer*/ );

// Delete item at index (shift remaining)
status mms_array_delete( dyn_mms_arr * arr , size_t index );

// Resize to at least new_capacity
status mms_array_resize( dyn_mms_arr * arr , size_t new_capacity , size_t new_used_count /*=0 to just reserve expand reserve capacity*/ );

// Get number of active items
size_t mms_array_get_count( dyn_mms_arr * arr );

Boolean mms_array_idx_exist_b( dyn_mms_arr * arr , size_t idx );
status mms_array_idx_exist_s( dyn_mms_arr * arr , size_t idx );

// Get pointer to item at index (NULL if out of range)
_MEMMOVE_UNSAFE_FXN void * mms_array_get( dyn_mms_arr * arr , size_t index );

// securly Get pointer to item at index
// Warning . After realloc, the memory block may move, so any previously stored pointer to its data becomes invalid
_MEMMOVE_UNSAFE_FXN status mms_array_get_s( dyn_mms_arr * arr , size_t index , void ** pitem );

// just could use temporarily . get unsafe pointer so be careful
_MEMMOVE_UNSAFE_FXN status mms_array_get_us( dyn_mms_arr * arr , size_t index , void *** ppitem );

// Set item at index (returns -1 if out of range)
status mms_array_set( dyn_mms_arr * arr , size_t index , void * item );

#endif
