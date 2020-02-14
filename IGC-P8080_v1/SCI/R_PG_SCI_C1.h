/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_SCI_C1.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 15.02.2020 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_SCI_Set_C1(void);
bool R_PG_SCI_SendAllData_C1(uint8_t * data, uint16_t count);
bool R_PG_SCI_GetSentDataCount_C1(uint16_t * count);
bool R_PG_SCI_ReceiveAllData_C1(uint8_t * data, uint16_t count);
bool R_PG_SCI_StopCommunication_C1(void);
bool R_PG_SCI_GetReceivedDataCount_C1(uint16_t * count);
bool R_PG_SCI_GetReceptionErrorFlag_C1(bool * parity, bool * framing, bool * overrun);
bool R_PG_SCI_ClearReceptionErrorFlag_C1(void);
bool R_PG_SCI_GetTransmitStatus_C1(bool * complete);
bool R_PG_SCI_StopModule_C1(void);



