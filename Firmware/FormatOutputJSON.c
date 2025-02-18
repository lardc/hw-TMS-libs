#include "FormatOutputJSON.h"

typedef enum __JSONReadStateMachine
{
	JRSM_Read = 0,
	JRSM_Replace
} JSONReadStateMachine;

JSONReadStateMachine CurrentState = JRSM_Read;
char Parameter[PARAMETER_SIZE];
Int16U TemplatePosition;
Int16U ParameterPosition;
Int16U Index;

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
				itoa(Parameter, *(pInt16U)JSONPointers[Index++]);
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

