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
* File Name    : R_PG_Timer_MTU_U0_C1.c
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
#include "r_pdl_mtu2.h"
#include "r_pdl_intc.h"
#include "r_pdl_definitions.h"
#include "R_PG_IntFuncsExtern.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_Set_MTU_U0_C1(void)
*
* Function Name: R_PG_Timer_Set_MTU_U0_C1
*
* Description  : MTUの設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_MTU2_Create
*                   : R_MTU2_Create_load_defaults
*                   : R_MTU2_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_Set_MTU_U0_C1(void)
{
	bool res;
	R_MTU2_Create_structure parameters;

	R_MTU2_Create_load_defaults( &parameters );

	parameters.channel_mode = PDL_MTU2_MODE_PHASE1;
	parameters.TGR_A_B_operation = PDL_MTU2_A_IC_BOTH_EDGES | PDL_MTU2_B_IC_BOTH_EDGES;
	parameters.noise_filter_operation = PDL_MTU2_NF_A_U_ENABLE | PDL_MTU2_NF_B_V_ENABLE | PDL_MTU2_NF_PCLK_DIV_1;
	parameters.TGRA_TCNTV_value = 0;
	parameters.TGRB_TCNTW_value = 0;

	res = R_MTU2_Set(
		1,
		PDL_MTU2_PIN_1A_P20 | PDL_MTU2_PIN_1B_P21,
		PDL_MTU2_PIN_CLKA_P24 | PDL_MTU2_PIN_CLKB_P25
	);

	if( !res ){
		return res;
	}

	return R_MTU2_Create(
		1,
		&parameters
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_StartCount_MTU_U0_C1(void)
*
* Function Name: R_PG_Timer_StartCount_MTU_U0_C1
*
* Description  : MTUのカウント動作開始
*
* Arguments    : なし
*
* Return Value : true  : カウント動作の再開が正しく行われた場合
*              : false : カウント動作の再開に失敗した場合
*
* Calling Functions : R_MTU2_ControlChannel
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_StartCount_MTU_U0_C1(void)
{
	R_MTU2_ControlChannel_structure parameters;

	parameters.control_setting = PDL_MTU2_START;
	parameters.register_selection = PDL_NO_DATA;
	parameters.TCNT_TCNTU_value = PDL_NO_DATA;
	parameters.TGRA_TCNTV_value = PDL_NO_DATA;
	parameters.TGRB_TCNTW_value = PDL_NO_DATA;
	parameters.TGRC_TGRU_value = PDL_NO_DATA;
	parameters.TGRD_TGRV_value = PDL_NO_DATA;
	parameters.TGRE_TGRW_value = PDL_NO_DATA;
	parameters.TGRF_value = PDL_NO_DATA;
	parameters.TADCOBRA_value = PDL_NO_DATA;
	parameters.TADCOBRB_value = PDL_NO_DATA;

	return R_MTU2_ControlChannel(
		1,
		&parameters
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_HaltCount_MTU_U0_C1(void)
*
* Function Name: R_PG_Timer_HaltCount_MTU_U0_C1
*
* Description  : MTUのカウント動作を一時停止
*
* Arguments    : なし
*
* Return Value : true  : 停止に成功した場合
*              : false : 停止に失敗した場合
*
* Calling Functions : R_MTU2_ControlChannel
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_HaltCount_MTU_U0_C1(void)
{
	R_MTU2_ControlChannel_structure parameters;

	parameters.control_setting = PDL_MTU2_STOP;
	parameters.register_selection = PDL_NO_DATA;
	parameters.TCNT_TCNTU_value = PDL_NO_DATA;
	parameters.TGRA_TCNTV_value = PDL_NO_DATA;
	parameters.TGRB_TCNTW_value = PDL_NO_DATA;
	parameters.TGRC_TGRU_value = PDL_NO_DATA;
	parameters.TGRD_TGRV_value = PDL_NO_DATA;
	parameters.TGRE_TGRW_value = PDL_NO_DATA;
	parameters.TGRF_value = PDL_NO_DATA;
	parameters.TADCOBRA_value = PDL_NO_DATA;
	parameters.TADCOBRB_value = PDL_NO_DATA;

	return R_MTU2_ControlChannel(
		1,
		&parameters
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_GetCounterValue_MTU_U0_C1(uint16_t * counter_val)
*
* Function Name: R_PG_Timer_GetCounterValue_MTU_U0_C1
*
* Description  : MTUのカウンタ値を取得
*
* Arguments    : uint16_t * counter_val : カウンタ値の格納先
*
* Return Value : true  : 取得に成功した場合
*              : false : 取得に失敗した場合
*
* Calling Functions : R_MTU2_ReadChannel
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_GetCounterValue_MTU_U0_C1(uint16_t * counter_val)
{
	if( counter_val == 0 ){ return false; }

	return R_MTU2_ReadChannel(
		1,
		PDL_NO_PTR,
		counter_val,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_SetCounterValue_MTU_U0_C1(uint16_t counter_val)
*
* Function Name: R_PG_Timer_SetCounterValue_MTU_U0_C1
*
* Description  : MTUのカウンタ値を設定
*
* Arguments    : uint16_t counter_val : カウンタに設定する値
*
* Return Value : true  : カウンタ値の設定に成功した場合
*              : false : カウンタ値の設定に失敗した場合
*
* Calling Functions : R_MTU2_ControlChannel
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_SetCounterValue_MTU_U0_C1(uint16_t counter_val)
{
	R_MTU2_ControlChannel_structure parameters;

	parameters.control_setting = PDL_NO_DATA;
	parameters.register_selection = PDL_MTU2_REGISTER_COUNTER;
	parameters.TCNT_TCNTU_value = counter_val;
	parameters.TGRA_TCNTV_value = PDL_NO_DATA;
	parameters.TGRB_TCNTW_value = PDL_NO_DATA;
	parameters.TGRC_TGRU_value = PDL_NO_DATA;
	parameters.TGRD_TGRV_value = PDL_NO_DATA;
	parameters.TGRE_TGRW_value = PDL_NO_DATA;
	parameters.TGRF_value = PDL_NO_DATA;
	parameters.TADCOBRA_value = PDL_NO_DATA;
	parameters.TADCOBRB_value = PDL_NO_DATA;

	return R_MTU2_ControlChannel(
		1,
		&parameters
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_GetTGR_MTU_U0_C1(uint16_t * tgr_a_val, uint16_t * tgr_b_val, uint16_t * tgr_c_val, uint16_t * tgr_d_val, uint16_t * tgr_e_val, uint16_t * tgr_f_val)
*
* Function Name: R_PG_Timer_GetTGR_MTU_U0_C1
*
* Description  : ジェネラルレジスタの値の取得
*
* Arguments    : uint16_t * tgr_a_val : ジェネラルレジスタA値の格納先
*              : uint16_t * tgr_b_val : ジェネラルレジスタB値の格納先
*              : uint16_t * tgr_c_val : ジェネラルレジスタC値の格納先
*              : uint16_t * tgr_d_val : ジェネラルレジスタD値の格納先
*              : uint16_t * tgr_e_val : ジェネラルレジスタE値の格納先
*              : uint16_t * tgr_f_val : ジェネラルレジスタF値の格納先
*
* Return Value : true  : 取得に成功した場合
*              : false : 取得に失敗した場合
*
* Calling Functions : R_MTU2_ReadChannel
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_GetTGR_MTU_U0_C1(uint16_t * tgr_a_val, uint16_t * tgr_b_val, uint16_t * tgr_c_val, uint16_t * tgr_d_val, uint16_t * tgr_e_val, uint16_t * tgr_f_val)
{
	uint16_t * local_a = PDL_NO_PTR;
	uint16_t * local_b = PDL_NO_PTR;

	if( tgr_a_val ){
		local_a = tgr_a_val;
	}
	if( tgr_b_val ){
		local_b = tgr_b_val;
	}

	return R_MTU2_ReadChannel(
		1,
		PDL_NO_PTR,
		PDL_NO_PTR,
		local_a,
		local_b,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_SetTGR_A_MTU_U0_C1(uint16_t tgr_a_val)
*
* Function Name: R_PG_Timer_SetTGR_A_MTU_U0_C1
*
* Description  : ジェネラルレジスタの値の設定 (TGRA)
*
* Arguments    : uint16_t tgr_a_val : ジェネラルレジスタに設定する値
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_MTU2_ControlChannel
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_SetTGR_A_MTU_U0_C1(uint16_t tgr_a_val)
{
	R_MTU2_ControlChannel_structure parameters;

	parameters.control_setting = PDL_NO_DATA;
	parameters.register_selection = PDL_MTU2_REGISTER_TGRA;
	parameters.TCNT_TCNTU_value = PDL_NO_DATA;
	parameters.TGRA_TCNTV_value = tgr_a_val;
	parameters.TGRB_TCNTW_value = PDL_NO_DATA;
	parameters.TGRC_TGRU_value = PDL_NO_DATA;
	parameters.TGRD_TGRV_value = PDL_NO_DATA;
	parameters.TGRE_TGRW_value = PDL_NO_DATA;
	parameters.TGRF_value = PDL_NO_DATA;
	parameters.TADCOBRA_value = PDL_NO_DATA;
	parameters.TADCOBRB_value = PDL_NO_DATA;

	return R_MTU2_ControlChannel(
		1,
		&parameters
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_SetTGR_B_MTU_U0_C1(uint16_t tgr_b_val)
*
* Function Name: R_PG_Timer_SetTGR_B_MTU_U0_C1
*
* Description  : ジェネラルレジスタの値の設定 (TGRB)
*
* Arguments    : uint16_t tgr_b_val : ジェネラルレジスタに設定する値
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_MTU2_ControlChannel
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_SetTGR_B_MTU_U0_C1(uint16_t tgr_b_val)
{
	R_MTU2_ControlChannel_structure parameters;

	parameters.control_setting = PDL_NO_DATA;
	parameters.register_selection = PDL_MTU2_REGISTER_TGRB;
	parameters.TCNT_TCNTU_value = PDL_NO_DATA;
	parameters.TGRA_TCNTV_value = PDL_NO_DATA;
	parameters.TGRB_TCNTW_value = tgr_b_val;
	parameters.TGRC_TGRU_value = PDL_NO_DATA;
	parameters.TGRD_TGRV_value = PDL_NO_DATA;
	parameters.TGRE_TGRW_value = PDL_NO_DATA;
	parameters.TGRF_value = PDL_NO_DATA;
	parameters.TADCOBRA_value = PDL_NO_DATA;
	parameters.TADCOBRB_value = PDL_NO_DATA;

	return R_MTU2_ControlChannel(
		1,
		&parameters
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_GetRequestFlag_MTU_U0_C1(bool * cm_ic_a, bool * cm_ic_b, bool * cm_ic_c, bool * cm_ic_d, bool * cm_e, bool * cm_f, bool * ov, bool * un)
*
* Function Name: R_PG_Timer_GetRequestFlag_MTU_U0_C1
*
* Description  : MTUの割り込み要求フラグの取得とクリア
*
* Arguments    : bool * cm_ic_a : コンペアマッチ/インプットキャプチャAフラグの格納先
*              : bool * cm_ic_b : コンペアマッチ/インプットキャプチャBフラグの格納先
*              : bool * cm_ic_c : コンペアマッチ/インプットキャプチャCフラグの格納先
*              : bool * cm_ic_d : コンペアマッチ/インプットキャプチャDフラグの格納先
*              : bool * cm_e : コンペアマッチEフラグの格納先
*              : bool * cm_f : コンペアマッチFフラグの格納先
*              : bool * ov : オーバフローフラグの格納先
*              : bool * un : アンダフローフラグの格納先
*
* Return Value : true  : 取得に成功した場合
*              : false : 取得に失敗した場合
*
* Calling Functions : R_MTU2_ReadChannel
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Timer_GetRequestFlag_MTU_U0_C1(bool * cm_ic_a, bool * cm_ic_b, bool * cm_ic_c, bool * cm_ic_d, bool * cm_e, bool * cm_f, bool * ov, bool * un)
{
	uint8_t data;
	bool res;

	res = R_MTU2_ReadChannel(
		1,
		&data,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR
	);

	if( cm_ic_a ){
		*cm_ic_a = (data >> 1) & 0x01;
	}
	if( cm_ic_b ){
		*cm_ic_b = (data >> 2) & 0x01;
	}
	if( cm_ic_c ){
		*cm_ic_c = 0x00;
	}
	if( cm_ic_d ){
		*cm_ic_d = 0x00;
	}
	if( cm_e ){
		*cm_e = 0x00;
	}
	if( cm_f ){
		*cm_f = 0x00;
	}
	if( ov ){
		*ov = (data >> 6) & 0x01;
	}
	if( un ){
		*un = (data >> 7) & 0x01;
	}

	return res;
}



