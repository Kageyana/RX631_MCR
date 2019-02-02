/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_Timer_CMT_U1.c
* Version      : 1.00
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
* Limitations  : 
******************************************************************************
* History : 01.02.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_pdl_cmt.h"
#include "r_pdl_definitions.h"
#include "R_PG_IntFuncsExtern.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_Set_CMT_U1_C2(void)
*
* Function Name: R_PG_Timer_Set_CMT_U1_C2
*
* Description  : CMTを設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CMT_Create
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_Set_CMT_U1_C2(void)
{
	return R_CMT_Create(
		2,
		PDL_CMT_PCLK_DIV_8 | PDL_CMT_DMAC_DTC_TRIGGER_DISABLE | PDL_CMT_STOP,
		59,
		msd_send_data,
		7
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_StartCount_CMT_U1_C2(void)
*
* Function Name: R_PG_Timer_StartCount_CMT_U1_C2
*
* Description  : CMTのカウントを開始
*
* Arguments    : なし
*
* Return Value : true  : カウント動作の再開が正しく行われた場合
*              : false : カウント動作の再開に失敗した場合
*
* Calling Functions : R_CMT_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_StartCount_CMT_U1_C2(void)
{
	return R_CMT_Control(
		2,
		PDL_CMT_START,
		0
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_HaltCount_CMT_U1_C2(void)
*
* Function Name: R_PG_Timer_HaltCount_CMT_U1_C2
*
* Description  : CMTのカウントを一時停止
*
* Arguments    : なし
*
* Return Value : true  : 停止に成功した場合
*              : false : 停止に失敗した場合
*
* Calling Functions : R_CMT_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_HaltCount_CMT_U1_C2(void)
{
	return R_CMT_Control(
		2,
		PDL_CMT_STOP,
		0
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_GetCounterValue_CMT_U1_C2(uint16_t * counter_val)
*
* Function Name: R_PG_Timer_GetCounterValue_CMT_U1_C2
*
* Description  : CMTのカウンタ値を取得
*
* Arguments    : uint16_t * counter_val : カウンタ値の格納先
*
* Return Value : true  : 取得に成功した場合
*              : false : 取得に失敗した場合
*
* Calling Functions : R_CMT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_GetCounterValue_CMT_U1_C2(uint16_t * counter_val)
{
	if( counter_val == 0 ){ return false; }

	return R_CMT_Read(
		2,
		PDL_NO_PTR,
		counter_val
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_SetCounterValue_CMT_U1_C2(uint16_t counter_val)
*
* Function Name: R_PG_Timer_SetCounterValue_CMT_U1_C2
*
* Description  : CMTのカウンタ値を設定
*
* Arguments    : uint16_t counter_val : カウンタに設定する値
*
* Return Value : true  : カウンタ値の設定に成功した場合
*              : false : カウンタ値の設定に失敗した場合
*
* Calling Functions : R_CMT_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_SetCounterValue_CMT_U1_C2(uint16_t counter_val)
{
	return R_CMT_Control(
		2,
		PDL_CMT_COUNTER,
		counter_val
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_SetConstantRegister_CMT_U1_C2(uint16_t constant_val)
*
* Function Name: R_PG_Timer_SetConstantRegister_CMT_U1_C2
*
* Description  : CMTのコンスタントレジスタ値を設定
*
* Arguments    : uint16_t constant_val : コンスタントレジスタ値の格納先
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CMT_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_SetConstantRegister_CMT_U1_C2(uint16_t constant_val)
{
	return R_CMT_Control(
		2,
		PDL_CMT_CONSTANT,
		constant_val
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_StopModule_CMT_U1(void)
*
* Function Name: R_PG_Timer_StopModule_CMT_U1
*
* Description  : CMTのユニットを停止
*
* Arguments    : なし
*
* Return Value : true  : 停止に成功した場合
*              : false : 停止に失敗した場合
*
* Calling Functions : R_CMT_Destroy
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_StopModule_CMT_U1(void)
{
	return R_CMT_Destroy( 1 );

}



