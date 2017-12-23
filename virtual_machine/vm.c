
#include <stdio.h>
#include <stdint.h>

#define DEBUG 3 // level 3 debug

#include "debug.h"

#define HALT_INSTRUCTION 0x00000000
#define ADD_INSTRUCTION 0x10000001
#define SUBSTRACT_INSTRUCTION 0x10000002
#define MULTIPY_INSTRUCTION 0x10000003
#define DIVED_INSTRUCTION 0x10000004


typedef uint32_t PROGRAM_COUNTER_REGISTER;
typedef int32_t	 ACCUMULATOR_REGISTER;
typedef uint32_t STACK_COUNTER_REGISTER;
typedef uint32_t GENERAL_PUPOSE_REGISTER;

typedef uint32_t (*operation)( uint32_t , uint32_t );

typedef enum OBJECT_TYPE_T
{
	INSTRUCTION,
	DATA
} OBJECT_T;

typedef struct OBJECT_T
{
	uint8_t type;

	union
	{
		uint32_t* data;
		operation op;
	};

}OBJECT;

// pull this out into other file 

typedef struct STACK_T
{
	int top; // index of top element in the stack
	uint8_t size; //size of the stack

	OBJECT* objects; // bytes ( machine instructions )
} STACK;


typedef enum INSTRUCTION_TYPE_T 
{

	HALT 		= 0x0,
	ADD 		= 0x1,
	SUBSTRACT 	= 0x2,
	MULTIPLY 	= 0x3,
	DIVID 		= 0x4

} INSTRUCTION_TYPE;

typedef struct VM_T
{
	PROGRAM_COUNTER_REGISTER pc;
	STACK_COUNTER_REGISTER sc;
	ACCUMULATOR_REGISTER ar;
	GENERAL_PUPOSE_REGISTER gp;

	STACK* stack;

} VM;


VM virtual_machine_new( STACK* stack )
{
	VM vm;	

	vm.pc 		= 0;
	vm.sc 		= 0;
	vm.ar 		= 0;
	vm.gp 		= 0;

	vm.stack 	= stack;

	return vm;
}


STACK* virtual_machine_get_stack( VM* vm )
{
	return vm->stack;
}

uint32_t op_add( uint32_t left_op , uint32_t right_op )
{

	DEBUG_PRINT( "%d + %d operation was triggerd\n" , left_op , right_op );
	return left_op + right_op;
}

uint32_t op_sub( uint32_t left_op , uint32_t right_op )
{

	DEBUG_PRINT("subtract operation was triggerd\n");

	return left_op - right_op;
}

uint32_t op_dev( uint32_t left_op , uint32_t right_op )
{

	DEBUG_PRINT("divition operation was triggerd\n");

	return left_op / right_op;
}

uint32_t op_mult( uint32_t left_op , uint32_t right_op )
{

	DEBUG_PRINT("multiplication operation was triggerd\n");

	return left_op * right_op;
}

uint32_t op_halt( uint32_t left_op , uint32_t right_op )
{
	exit( 1 );

	return 0;
}

uint32_t op_undefined( uint32_t left_op , uint32_t right_op )
{
	DEBUG_PRINT("operation undefined\n");	

	return -1;
}


operation evaluate_operation( uint32_t op )
{
	switch( op )
	{
		case ADD:
			return op_add;
		case SUBSTRACT:
			return op_sub;
		case MULTIPLY:
			return op_mult;
		case DIVID:
			return op_dev;
		default:
			return op_undefined;
	}
}


OBJECT fetch( uint32_t code )
{

	// see if content is instruction or data
	OBJECT object;

	// we reserve 4 bits 
	uint32_t is_instruction = code >> 28;

	if( is_instruction )
	{
		object.type = INSTRUCTION;

		uint32_t op = code << 4;
		op = op >> 4; 

		object.op = evaluate_operation( op );

		return object;
	}

	object.type = DATA;
	object.data = code;

	return object;

}

// stack implementation 



STACK* stack_new( uint8_t size )
{
	STACK *stack;

	stack = ( STACK * )malloc( sizeof( STACK ) );

	stack->top = 0;
	stack->size = size;
	stack->objects = (uint8_t*)malloc( sizeof( OBJECT ) * size );

	return stack;
}

uint8_t stack_push( STACK* stack , OBJECT object )
{
	stack->objects[ stack->top++ ] = object;
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


// end of stack implementation


uint8_t load_program( VM* vm, uint32_t * program )
{
	// we fetch inteal we hit HALT instruction

	const uint32_t* code = program;

	STACK* stack = virtual_machine_get_stack( vm );

	while( *code != HALT )
	{
		OBJECT obj = fetch( *code );
		stack_push( stack , obj );
		vm->sc++;
		code++;
	}

	return 0;

}


void execute( VM* vm )
{

	STACK *stack = virtual_machine_get_stack( vm );

	while( stack->top > 0 )
	{
		OBJECT obj = stack_pop( stack );

		if( obj.type == INSTRUCTION )
		{
			OBJECT left_op  = stack_pop( stack );
			OBJECT right_op = stack_pop( stack );

			vm->ar = obj.op( left_op.data , right_op.data );

			DEBUG_SUCCESS( "%d\n" , vm->ar );

		}
	}

}


int main( int argc , char** argv )
{
	STACK* stack = stack_new( 10 );

	VM vm = virtual_machine_new( stack );

	uint32_t *code = (uint32_t *) malloc( sizeof( uint32_t ) * 100 );

	code[0] = 0x00000008;
	code[1] = 0x00000008;
	code[2] = DIVED_INSTRUCTION;

	code[3] = 0x00000000;

	load_program( &vm , code );

	execute( &vm );


	return 0;

}