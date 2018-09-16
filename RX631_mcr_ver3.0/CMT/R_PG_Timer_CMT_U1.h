/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_Timer_CMT_U1.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 16.09.2018 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_Timer_Set_CMT_U1_C2(void);
bool R_PG_Timer_StartCount_CMT_U1_C2(void);
bool R_PG_Timer_HaltCount_CMT_U1_C2(void);
bool R_PG_Timer_GetCounterValue_CMT_U1_C2(uint16_t * counter_val);
bool R_PG_Timer_SetCounterValue_CMT_U1_C2(uint16_t counter_val);
bool R_PG_Timer_SetConstantRegister_CMT_U1_C2(uint16_t constant_val);
bool R_PG_Timer_StopModule_CMT_U1(void);



