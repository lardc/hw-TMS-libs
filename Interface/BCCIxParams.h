// ----------------------------------------
// Parameters of BCCI master and slave interface
// ----------------------------------------

#ifndef __BCCIx_PARAMS_H
#define __BCCIx_PARAMS_H

#define CAN_SLAVE_NID_MASK			0x0003FC00
#define CAN_SLAVE_NID_MPY			(1ul << 10)

#define CAN_MASTER_NID_MASK			0x03FC0000
#define CAN_MASTER_NID_MPY			(1ul << 18)

#define BR_TIMEOUT					100

#endif // __BCCIx_PARAMS_H
