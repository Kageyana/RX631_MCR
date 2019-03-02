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
* File Name    : R_PG_Timer_IWDT.c
* Version      : 1.00
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
* Limitations  : 
******************************************************************************
* History : 02.03.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_pdl_iwdt.h"
#include "r_pdl_definitions.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_Start_IWDT(void)
*
* Function Name: R_PG_Timer_Start_IWDT
*
* Description  : IWDTの設定と開始
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IWDT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_Start_IWDT(void)
{
	return R_IWDT_Set( PDL_IWDT_TIMEOUT_1024 | PDL_IWDT_CLOCK_OCO_16 | PDL_IWDT_TIMEOUT_RESET | PDL_IWDT_WIN_START_100 | PDL_IWDT_WIN_END_0 | PDL_IWDT_STOP_DISABLE );

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_RefreshCounter_IWDT(void)
*
* Function Name: R_PG_Timer_RefreshCounter_IWDT
*
* Description  : IWDTのカウンタをリフレッシュ
*
* Arguments    : なし
*
* Return Value : true  : リフレッシュに成功した場合
*              : false : リフレッシュに失敗した場合
*
* Calling Functions : R_IWDT_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_RefreshCounter_IWDT(void)
{
	return R_IWDT_Control( PDL_IWDT_REFRESH );

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_GetStatus_IWDT(uint16_t* counter_val, bool * undf, bool * ref_err)
*
* Function Name: R_PG_Timer_GetStatus_IWDT
*
* Description  : IWDTのステータスフラグとカウント値を取得
*
* Arguments    : uint16_t* counter_val : カウンタ値の格納先
*              : bool * undf : アンダフローフラグの格納先
*              : bool * ref_err : リフレッシュエラーフラグの格納先
*
* Return Value : true  : フラグの取得が正しく行われた場合
*              : false : フラグの取得に失敗した場合
*
* Calling Functions : R_IWDT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_GetStatus_IWDT(uint16_t* counter_val, bool * undf, bool * ref_err)
{
	uint16_t status;
	bool res;

	res = R_IWDT_Read( &status );

	if(counter_val){ *counter_val = status & 0x3fff; }

	return res;
}



