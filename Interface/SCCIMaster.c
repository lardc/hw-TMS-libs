// -----------------------------------------
// SCCI-Master communication interface
// ----------------------------------------

// Header
#include "SCCIMaster.h"
//
// Includes
#include "CRC16.h"


#ifdef SCCI_MASTER

// Forward functions
//
static void SCCIM_SendFrame(pSCCIM_Interface Interface, Int16U Node, Int16U Function, Int16U SubFunction, Int16U DataSize);
static Int16U SCCIM_ReceiveFrame(pSCCIM_Interface Interface, Int16U Node, Int16U Function, Int16U SubFunction, Int16U DataSize);


// Functions
//
void SCCIM_Init(pSCCIM_Interface Interface, pSCCI_IOConfig IOConfig, Int32U MessageTimeoutTicks, volatile Int64U *pTimer)
{
	Int16U i;

	for(i = 0; i < xCCI_BUFFER_SIZE; ++i)
		Interface->MessageBuffer[i] = 0;

	// Save parameters
	Interface->IOConfig = IOConfig;
	Interface->TimeoutValueTicks = MessageTimeoutTicks;
	Interface->pTimerCounter = pTimer;
}
// ----------------------------------------

Int16U SCCIM_Write16(pSCCIM_Interface Interface, Int16U NodeID, Int16U Address, Int16U Value)
{
	Int16U i, err = ERR_NO_ERROR;

	for(i = 0; i < SCCI_MASTER_RETRANSMISSION_COUNT; ++i)
	{
		Interface->MessageBuffer[2] = Address;
		Interface->MessageBuffer[3] = Value;

		SCCIM_SendFrame(Interface, NodeID, FUNCTION_WRITE, SFUNC_16, 2);

		if((err = SCCIM_ReceiveFrame(Interface, NodeID, FUNCTION_WRITE, SFUNC_16, 1)) == ERR_NO_ERROR)
			break;
	}

	return err;
}
// ----------------------------------------

Int16U SCCIM_Read16(pSCCIM_Interface Interface, Int16U NodeID, Int16U Address, pInt16U Value)
{
	Int16U i, err = ERR_NO_ERROR;

	for(i = 0; i < SCCI_MASTER_RETRANSMISSION_COUNT; ++i)
	{
		Interface->MessageBuffer[2] = Address;

		SCCIM_SendFrame(Interface, NodeID, FUNCTION_READ, SFUNC_16, 1);
		err = SCCIM_ReceiveFrame(Interface, NodeID, FUNCTION_READ, SFUNC_16, 2);

		if(err == ERR_NO_ERROR)
		{
			*Value = Interface->MessageBuffer[3];
			break;
		}
	}

	return err;
}
// ----------------------------------------

Int16U SCCIM_ReadArray16(pSCCIM_Interface Interface, Int16U NodeID, Int16U Endpoint, Int16U MaxCount, pInt16U Out,
		pInt16U OutCounter)
{
	return SCCIM_ReadArray16Callback(Interface, NodeID, Endpoint, MaxCount, Out, OutCounter, NULL);
}
// ----------------------------------------

Int16U SCCIM_ReadArray16Callback(pSCCIM_Interface Interface, Int16U NodeID, Int16U Endpoint, Int16U MaxCount,
		pInt16U Out, pInt16U OutCounter, void (*Callback)())
{
	Int16U result;
	Int16U i = 0, j, k = 0;
	Int16U dataCount = 1;

	while((i < MaxCount) && (dataCount > 0))
	{
		Interface->MessageBuffer[2] = Endpoint << 8;

		SCCIM_SendFrame(Interface, NodeID, FUNCTION_READ_BLOCK, SFUNC_16, 1);
		result = SCCIM_ReceiveFrame(Interface, NodeID, FUNCTION_READ_BLOCK, SFUNC_16, SCCI_BLOCK_MAX_VAL_16_R + 1);

		if(result != ERR_NO_ERROR)
			return result;

		dataCount = Interface->MessageBuffer[2] & 0x00FF;
		i += dataCount;

		if (i > MaxCount)
			dataCount = i - MaxCount;

		for (j = 0; j < dataCount; j++)
			Out[k++] = Interface->MessageBuffer[3 + j];

		if (Callback)
			Callback();
	}

	*OutCounter = k;
	return ERR_NO_ERROR;
}
// ----------------------------------------

Int16U SCCIM_Call(pSCCIM_Interface Interface, Int16U NodeID, Int16U Action)
{
	Int16U i, err = ERR_NO_ERROR;

	for(i = 0; i < SCCI_MASTER_RETRANSMISSION_COUNT; ++i)
	{
		Interface->MessageBuffer[2] = Action;

		SCCIM_SendFrame(Interface, NodeID, FUNCTION_CALL, SFUNC_NONE, 1);

		if((err = SCCIM_ReceiveFrame(Interface, NodeID, FUNCTION_CALL, SFUNC_NONE, 1)) == ERR_NO_ERROR)
			break;
	}

	return err;
}
// ----------------------------------------

static void SCCIM_SendFrame(pSCCIM_Interface Interface, Int16U Node, Int16U Function, Int16U SubFunction, Int16U DataSize)
{
	Interface->MessageBuffer[0] = (START_BYTE << 8) | Node;
	Interface->MessageBuffer[1] = ((Function << 3) | SubFunction) << 8;
	Interface->MessageBuffer[DataSize + 2] = CRC16_ComputeCRC(Interface->MessageBuffer, DataSize + 2);

	Interface->IOConfig->IO_SendArray16(Interface->MessageBuffer, DataSize + 3);
}
// ----------------------------------------

static Int16U SCCIM_ReceiveFrame(pSCCIM_Interface Interface, Int16U Node, Int16U Function, Int16U SubFunction, Int16U DataSize)
{
	Int64U timeout;

	timeout = Interface->TimeoutValueTicks + *(Interface->pTimerCounter);

	while(*(Interface->pTimerCounter) < timeout)
	{
		if(Interface->IOConfig->IO_GetBytesToReceive() >= 2 * 2)
		{
			Int16U startByte = Interface->IOConfig->IO_ReceiveByte();

			if(startByte == START_BYTE)
			{
				Int16U nextByte, code;

				nextByte = Interface->IOConfig->IO_ReceiveByte();
				Interface->MessageBuffer[0] = nextByte | (startByte << 8);

				Interface->IOConfig->IO_ReceiveArray16(Interface->MessageBuffer + 1, 1);

				code = Interface->MessageBuffer[1] >> 8;

				if((Interface->MessageBuffer[0] & 0xFF) == Node)
				{
					if(code & RESPONSE_MASK)
					{
						if((((code & FUNCTION_CODE_MASK) >> 3) == Function)
							&& ((code & FUNCTION_SCODE_MASK) == SubFunction)
							|| (((code & FUNCTION_CODE_MASK) >> 3) == FUNCTION_ERROR))
						{
							if(((code & FUNCTION_CODE_MASK) >> 3) == FUNCTION_ERROR)
								DataSize = 2;

							while(*(Interface->pTimerCounter) < timeout)
							{
								if(Interface->IOConfig->IO_GetBytesToReceive() >= (DataSize + 1) * 2)
								{
									Int16U crc;

									Interface->IOConfig->IO_ReceiveArray16(Interface->MessageBuffer + 2, DataSize + 1);
									crc = CRC16_ComputeCRC(Interface->MessageBuffer, DataSize + 2);

									if(Interface->MessageBuffer[DataSize + 2] != crc)
										return ERR_CRC;

									if(((code & FUNCTION_CODE_MASK) >> 3) == FUNCTION_ERROR)
										return Interface->MessageBuffer[2];

									return ERR_NO_ERROR;
								}
							}

							return ERR_TIMEOUT;
						}

						return ERR_PROTOCOL_MISMATCH;
					}
				}

				return ERR_WRONG_NODE_ID;
			}
		}
	}

	return ERR_TIMEOUT;
}
// ----------------------------------------

#endif // SCCI_MASTER
