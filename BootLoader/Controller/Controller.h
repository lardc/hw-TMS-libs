// -----------------------------------------
// Logic controller
// ----------------------------------------

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "Global.h"
#include "DeviceObjectDictionary.h"
#include "Flash.h"


// Variables
//
extern volatile Int64U CONTROL_TimeCounter;
extern volatile Boolean WaitForFWUpload;


// Functions
//
// Initialize controller
void CONTROL_Init();
// Do background idle operation
void CONTROL_Idle();
// Notify that CAN system fault occurs
void CONTROL_NotifyCANFault(ZwCAN_SysFlags Flag);


#endif // __CONTROLLER_H
