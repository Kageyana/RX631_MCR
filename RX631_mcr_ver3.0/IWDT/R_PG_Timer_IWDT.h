/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_Timer_IWDT.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 10.05.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_Timer_Start_IWDT(void);
bool R_PG_Timer_RefreshCounter_IWDT(void);
bool R_PG_Timer_GetStatus_IWDT(uint16_t* counter_val, bool * undf, bool * ref_err);



