/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_Clock.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 01.02.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_Clock_Set(void);
bool R_PG_Clock_WaitSet(double wait_time);
bool R_PG_Clock_Start_LOCO(void);
bool R_PG_Clock_Stop_LOCO(void);
bool R_PG_Clock_Start_MAIN(void);
bool R_PG_Clock_Stop_MAIN(void);
bool R_PG_Clock_Enable_MAIN_ForcedOscillation(void);
bool R_PG_Clock_Disable_MAIN_ForcedOscillation(void);
bool R_PG_Clock_Enable_MAIN_StopDetection(void);
bool R_PG_Clock_Disable_MAIN_StopDetection(void);
bool R_PG_Clock_ClearFlag_MAIN_StopDetection(void);
bool R_PG_Clock_Start_PLL(void);
bool R_PG_Clock_Stop_PLL(void);
bool R_PG_Clock_GetSelectedClockSource(uint8_t* clock);
bool R_PG_Clock_GetClocksStatus(bool* pll, bool* main, bool* sub, bool* loco, bool* iwdt, bool* hoco);
bool R_PG_Clock_GetHOCOPowerStatus(bool* power);
bool R_PG_Clock_GetFlag_MAIN_StopDetection(bool* stop);



