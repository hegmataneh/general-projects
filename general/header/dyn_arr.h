#pragma once

#if defined Uses_dyn_arr || !defined __COMPILING

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
status array_init( dyn_arr * arr , size_t item_size , size_t growStep );

// each item is address of allocated memory and array keep it and should free it at last
// Important: When instances are destroyed, their destructor is not invoked.
// You are responsible for manually deallocating any memory allocated within the class to prevent memory leaks.
//status alc_ptr_array_init( dyn_arr * arr , size_t growStep );

// Free resources
void array_free( dyn_arr * arr );

// Add item (auto-expands capacity if needed)
status array_add( dyn_arr * arr , void * item );

// Delete item at index (shift remaining)
status array_delete( dyn_arr * arr , size_t index );

// Resize to at least new_capacity
status array_resize( dyn_arr * arr , size_t new_capacity );

// Get number of active items
size_t array_get_count( dyn_arr * arr );

// Get pointer to item at index (NULL if out of range)
void * array_get( dyn_arr * arr , size_t index );

// Set item at index (returns -1 if out of range)
status array_set( dyn_arr * arr , size_t index , void * item );

#endif
