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
* File Name    : R_PG_Timer_MTU_U0.c
* Version      : 1.00
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
* Limitations  : 
******************************************************************************
* History : 19.02.2020 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_pdl_mtu2.h"
#include "r_pdl_definitions.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_SynchronouslyStartCount_MTU_U0(bool ch0, bool ch1, bool ch2, bool ch3, bool ch4)
*
* Function Name: R_PG_Timer_SynchronouslyStartCount_MTU_U0
*
* Description  : MTUのカウントを同時に開始
*
* Arguments    : bool ch0 : MTU0.TCNTのシンクロスタート設定 (1:する  0:しない)
*              : bool ch1 : MTU1.TCNTのシンクロスタート設定 (1:する  0:しない)
*              : bool ch2 : MTU2.TCNTのシンクロスタート設定 (1:する  0:しない)
*              : bool ch3 : MTU3.TCNTのシンクロスタート設定 (1:する  0:しない)
*              : bool ch4 : MTU4.TCNTのシンクロスタート設定 (1:する  0:しない)
*
* Return Value : true  : カウント動作の再開が正しく行われた場合
*              : false : カウント動作の再開に失敗した場合
*
* Calling Functions : R_MTU2_ControlUnit
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_SynchronouslyStartCount_MTU_U0(bool ch0, bool ch1, bool ch2, bool ch3, bool ch4)
{
	R_MTU2_ControlUnit_structure parameters;

	parameters.simultaneous_control = PDL_NO_DATA;
	parameters.output_control = PDL_NO_DATA;
	parameters.buffer_control = PDL_NO_DATA;
	parameters.brushless_DC_motor_control = PDL_NO_DATA;
	parameters.general_control = PDL_NO_DATA;
	parameters.register_selection = PDL_NO_DATA;
	parameters.TDDR_value = PDL_NO_DATA;
	parameters.TCDR_value = PDL_NO_DATA;
	parameters.TCBR_value = PDL_NO_DATA;

	if( ch0 ){
		parameters.simultaneous_control |= PDL_MTU2_START_CH_0;
	}
	if( ch1 ){
		parameters.simultaneous_control |= PDL_MTU2_START_CH_1;
	}
	if( ch2 ){
		parameters.simultaneous_control |= PDL_MTU2_START_CH_2;
	}
	if( ch3 ){
		parameters.simultaneous_control |= PDL_MTU2_START_CH_3;
	}
	if( ch4 ){
		parameters.simultaneous_control |= PDL_MTU2_START_CH_4;
	}

	return R_MTU2_ControlUnit(
		0,
		&parameters
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_StopModule_MTU_U0(void)
*
* Function Name: R_PG_Timer_StopModule_MTU_U0
*
* Description  : MTUのユニットを停止
*
* Arguments    : なし
*
* Return Value : true  : 停止に成功した場合
*              : false : 停止に失敗した場合
*
* Calling Functions : R_MTU2_Destroy
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_StopModule_MTU_U0(void)
{
	return R_MTU2_Destroy( 0 );

}



