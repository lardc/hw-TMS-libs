#include "JSONDescription.h"
#include "FormatOutputJSON.h"

// Definitions
#define PARAMETER_SIZE			32
#define JSON_POINTERS_SIZE		32

typedef enum __JSONReadStateMachine
{
	JRSM_Read = 0,
	JRSM_Replace
} JSONReadStateMachine;

// Variables
static JSONReadStateMachine CurrentState = JRSM_Read;
static char Parameter[PARAMETER_SIZE];
static Int16U TemplatePosition, ParameterPosition, Index;
static pInt16U JSONPointers[JSON_POINTERS_SIZE] = {0};

// Forward functions
Int16U itoa(char *dest, Int16U i);

// Functions
//
void JSON_ResetStateMachine()
{
	CurrentState = JRSM_Read;
	TemplatePosition = 0;
	ParameterPosition = 0;
	Index = 0;
}
// ----------------------------------------

Int16U JSON_ReadSymbol()
{
	switch (CurrentState)
	{
		case JRSM_Read:
			if (TemplateJSON[TemplatePosition] == '\0')
				return 0;

			if (TemplateJSON[TemplatePosition] == '$')
			{
				CurrentState = JRSM_Replace;
				TemplatePosition++;

				ParameterPosition = 0;
				itoa(Parameter, *JSONPointers[Index++]);
			}
			else
				return TemplateJSON[TemplatePosition++];

		case JRSM_Replace:
			if (Parameter[ParameterPosition + 1] == '\0')
				CurrentState = JRSM_Read;
			return Parameter[ParameterPosition++];

		default:
			return 0;
	}
}
// ----------------------------------------

Int16U itoa(char *dest, Int16U i)
{
	char *start = dest;
	char buffer[12];
	char *p = buffer;

	do
	{
		*p++ = '0' + (i % 10);
		i /= 10;
	} while (i > 0);

	while (p != buffer)
		*dest++ = *--p;

	*dest = '\0';
	return dest - start;
}
// ----------------------------------------

void JSON_AssignPointer(Int16U Index, pInt16U Pointer)
{
	if (Index < JSON_POINTERS_SIZE)
		JSONPointers[Index] = Pointer;
}
// ----------------------------------------
