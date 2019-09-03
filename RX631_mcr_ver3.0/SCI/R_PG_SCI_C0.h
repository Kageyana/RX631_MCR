/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_SCI_C0.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 12.05.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_SCI_Set_C0(void);
bool R_PG_SCI_SendAllData_C0(uint8_t * data, uint16_t count);
bool R_PG_SCI_GetSentDataCount_C0(uint16_t * count);
bool R_PG_SCI_ReceiveAllData_C0(uint8_t * data, uint16_t count);
bool R_PG_SCI_StopCommunication_C0(void);
bool R_PG_SCI_GetReceivedDataCount_C0(uint16_t * count);
bool R_PG_SCI_GetReceptionErrorFlag_C0(bool * parity, bool * framing, bool * overrun);
bool R_PG_SCI_ClearReceptionErrorFlag_C0(void);
bool R_PG_SCI_GetTransmitStatus_C0(bool * complete);
bool R_PG_SCI_StopModule_C0(void);



