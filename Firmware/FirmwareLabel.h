﻿#ifndef __FIRMWARE_LABEL_H
#define __FIRMWARE_LABEL_H

// Include
#include "stdinc.h"
#include "LabelIndexSelector.h"

// Definitions
#define LABEL_NAME_MAX_LENGTH		64

// Types
typedef struct __Label
{
	char Name[LABEL_NAME_MAX_LENGTH];
	SelectorIndex Index;
} Label;

// Functions
void FWLB_LoadBoardLabel();
SelectorIndex FWLB_GetSelector();

void FWLB_PrepareLabelRead();
Int16U FWLB_ReadLabelSymbol();

#endif // __FIRMWARE_LABEL_H