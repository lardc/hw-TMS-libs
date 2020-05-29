// ----------------------------------------
// Device object dictionary
// ----------------------------------------

#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H


// ACTIONS
//
#define ACT_1
//
#define ACT_SAVE_TO_ROM				200	// Save parameters to EEPROM module
#define ACT_RESTORE_FROM_ROM		201	// Restore parameters from EEPROM module
#define ACT_RESET_TO_DEFAULT		202	// Reset parameters to default values (only in controller memory)
#define ACT_LOCK_NV_AREA			203	// Lock modifications of parameters area
#define ACT_UNLOCK_NV_AREA			204	// Unlock modifications of parameters area (password-protected)
//
#define ACT_FL_ERASE_C				302	// Erase sector C
#define ACT_FL_ERASE_D				303	// Erase sector D
#define ACT_FL_ERASE_E				304	// Erase sector E
#define ACT_FL_ERASE_F				305	// Erase sector F
#define ACT_FL_ERASE_G				306	// Erase sector G
#define ACT_FL_ERASE_H				307	// Erase sector H
#define ACT_FL_RESET_COUNTERS		308	// Reset checksum counters
#define ACT_FL_SET_WR_ADDRESS		309	// Set start flash address for firmware upload
#define ACT_FL_WRITE				310	// Begin flash write
#define ACT_FL_VALIDATE				311	// Validate flash write
#define ACT_FL_CLR_REQUEST			312	// Clear request flag for firmware upload
#define ACT_FL_RESET				313	// Force reset
//
#define ACT_FL_SET_REQUEST			320	// Set request flag for firmware upload


// REGISTERS
//
// No NVRAM section
//
#define REG_SP__1					1
//
// ----------------------------------------
//
#define REG_ADDRESS_L				2	// Start address low part of 32bit
#define REG_ADDRESS_H				3	// Start address high part of 32bit
#define REG_XOR_PC					4	// PC calculated XOR value
//
#define REG_PWD_1					5	// Unlock password location 1
#define REG_PWD_2					6	// Unlock password location 2
#define REG_PWD_3					7	// Unlock password location 3
#define REG_PWD_4					8	// Unlock password location 4
//
#define REG_SP__2					9
//
// ----------------------------------------
//
#define REG_XOR_TMS					10	// TMS calculated XOR value
//
#define REG_CAN_BUSOFF_COUNTER		18	// Counter of bus-off states
#define REG_CAN_STATUS_REG			19	// CAN status register (32 bit)
#define REG_CAN_STATUS_REG_32		20
#define REG_CAN_DIAG_TEC			21	// CAN TEC
#define REG_CAN_DIAG_REC			22	// CAN REC


// ENDPOINTS
//
#define EP16_WR						1	// Endpoint for firmware value sequence


// ENDPOINTS MASKS
//
#define EP16_WR_MASK				BIT0


// USER ERROR CODES
//
#define ERR_NONE					0	// No error
#define ERR_CONFIGURATION_LOCKED	1	// Device is locked for writing
#define ERR_OPERATION_BLOCKED		2	// Operation can't be done due to current device state
#define ERR_DEVICE_NOT_READY		3	// Device isn't ready to switch state
#define ERR_WRONG_PWD				4	// Wrong password - unlock failed
#define ERR_SELF_TEST_NOT_DONE		5	// Self-test should be done before making tests

#endif // __DEV_OBJ_DIC_H
