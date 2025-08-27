#pragma once

/*
* Lamport's Bakery Algorithm is a mutual exclusion algorithm designed to ensure that only one
* process can enter a critical section at a time in a concurrent environment, even without hardware-level atomic operations
*/

typedef struct BakeryAl
{
	int thread_count;
	volatile int * choosing;
	volatile int * number;
} Ba_al;

status ba_init( Ba_al * pLB , int thread_count );
void ba_destroy( Ba_al * pLB );
void ba_lock( Ba_al * pLB , int idx );
void ba_unlock( Ba_al * pLB , int idx );

/*
* note.
* 14040527 سر پروژه protocol bridge در قسمت آنلاک گیر کرد
*/