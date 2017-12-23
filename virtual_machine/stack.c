#include "stack.h"

STACK* stack_new( uint8_t size )
{
	STACK *stack;

	stack = ( STACK * )malloc( sizeof( STACK ) );

	stack->top = 0;
	stack->size = size;
	stack->objects = (uint8_t*)malloc( sizeof( OBJECT ) * size );

	return stack;
}

uint8_t stack_push( STACK* stack , OBJECT* object )
{
	stack->objects[ stack->top++ ] = *object;
	return stack->top;
}

OBJECT stack_pop( STACK* stack )
{
	return stack->objects[ --stack->top ];
}

OBJECT stack_peek( STACK* stack )
{
	return stack->objects[ stack->top - 1 ];
}
