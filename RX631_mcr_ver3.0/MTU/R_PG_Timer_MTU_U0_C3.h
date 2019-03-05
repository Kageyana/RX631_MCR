/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_Timer_MTU_U0_C3.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 06.03.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_Timer_Set_MTU_U0_C3(void);
bool R_PG_Timer_StartCount_MTU_U0_C3(void);
bool R_PG_Timer_HaltCount_MTU_U0_C3(void);
bool R_PG_Timer_GetCounterValue_MTU_U0_C3(uint16_t * counter_val);
bool R_PG_Timer_SetCounterValue_MTU_U0_C3(uint16_t counter_val);
bool R_PG_Timer_GetTGR_MTU_U0_C3(uint16_t * tgr_a_val, uint16_t * tgr_b_val, uint16_t * tgr_c_val, uint16_t * tgr_d_val, uint16_t * tgr_e_val, uint16_t * tgr_f_val);
bool R_PG_Timer_SetTGR_A_MTU_U0_C3(uint16_t tgr_a_val);
bool R_PG_Timer_SetTGR_B_MTU_U0_C3(uint16_t tgr_b_val);
bool R_PG_Timer_SetTGR_C_MTU_U0_C3(uint16_t tgr_c_val);
bool R_PG_Timer_SetTGR_D_MTU_U0_C3(uint16_t tgr_d_val);
bool R_PG_Timer_GetRequestFlag_MTU_U0_C3(bool * cm_ic_a, bool * cm_ic_b, bool * cm_ic_c, bool * cm_ic_d, bool * cm_e, bool * cm_f, bool * ov, bool * un);



