
#ifndef STACK_H
#define STACK_H 

#include <stdint.h>

typedef void OBJECT;

typedef struct STACK_T
{
	int top; // index of top element in the stack
	uint8_t size; //size of the stack

	OBJECT* objects; // bytes ( machine instructions )
} STACK;

STACK* stack_new( uint8_t size );
uint8_t stack_push( STACK* stack , OBJECT object );
OBJECT stack_pop( STACK* stack );
OBJECT stack_peek( STACK* stack );

#endif