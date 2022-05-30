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
* File Name    : R_PG_Clock.c
* Version      : 1.00
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
* Limitations  : 
******************************************************************************
* History : 17.05.2022 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_pdl_cgc.h"
#include "r_pdl_definitions.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Set(void)
*
* Function Name: R_PG_Clock_Set
*
* Description  : クロックの設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*                   : R_CGC_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Set(void)
{
	bool res;

	res = R_CGC_Set(
		PDL_CGC_CLK_LOCO,
		PDL_CGC_BCLK_DISABLE | PDL_CGC_RTC_SRC_NONE,
		125000,
		125000.000000,
		125000.000000,
		125000.000000,
		125000.000000,
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_NO_DATA
	);

	if( !res ){
		return res;
	}

	res = R_CGC_Set(
		PDL_CGC_CLK_MAIN,
		PDL_CGC_BCLK_DISABLE,
		12000000,
		3000000.000000,
		3000000.000000,
		3000000.000000,
		3000000.000000,
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_NO_DATA
	);

	if( !res ){
		return res;
	}

	res = R_CGC_Set(
		PDL_CGC_CLK_PLL,
		PDL_CGC_BCLK_DISABLE,
		192000000,
		96000000.000000,
		96000000.000000,
		48000000.000000,
		48000000.000000,
		PDL_NO_DATA,
		PDL_NO_DATA,
		48000000.000000,
		PDL_NO_DATA
	);

	if( !res ){
		return res;
	}

	return R_CGC_Control(
		PDL_CGC_CLK_PLL,
		PDL_CGC_OSC_STOP_DISABLE,
		PDL_CGC_RTC_NOT_USE
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_WaitSet(double wait_time)
*
* Function Name: R_PG_Clock_WaitSet
*
* Description  : クロックの設定(発振安定待機時間の設定あり)
*
* Arguments    : double wait_time : 発振安定待機時間(秒)
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*                   : R_CGC_Set
*                   : rpdl_wait_time
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_WaitSet(double wait_time)
{
	bool res;

	res = R_CGC_Set(
		PDL_CGC_CLK_LOCO,
		PDL_CGC_BCLK_DISABLE | PDL_CGC_RTC_SRC_NONE,
		125000,
		125000.000000,
		125000.000000,
		125000.000000,
		125000.000000,
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_NO_DATA
	);

	if( !res ){
		return res;
	}

	res = R_CGC_Set(
		PDL_CGC_CLK_MAIN,
		PDL_CGC_BCLK_DISABLE,
		12000000,
		3000000.000000,
		3000000.000000,
		3000000.000000,
		3000000.000000,
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_NO_DATA
	);

	if( !res ){
		return res;
	}

	res = R_CGC_Set(
		PDL_CGC_CLK_PLL,
		PDL_CGC_BCLK_DISABLE,
		192000000,
		96000000.000000,
		96000000.000000,
		48000000.000000,
		48000000.000000,
		PDL_NO_DATA,
		PDL_NO_DATA,
		48000000.000000,
		PDL_NO_DATA
	);

	if( !res ){
		return res;
	}

	rpdl_wait_time( wait_time );

	return R_CGC_Control(
		PDL_CGC_CLK_PLL,
		PDL_CGC_OSC_STOP_DISABLE,
		PDL_CGC_RTC_NOT_USE
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Start_LOCO(void)
*
* Function Name: R_PG_Clock_Start_LOCO
*
* Description  : 低速オンチップオシレータ(LOCO)の動作
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Start_LOCO(void)
{
	return R_CGC_Control(
		PDL_NO_DATA,
		PDL_CGC_LOCO_ENABLE,
		PDL_NO_DATA
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Stop_LOCO(void)
*
* Function Name: R_PG_Clock_Stop_LOCO
*
* Description  : 低速オンチップオシレータ(LOCO)の停止
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Stop_LOCO(void)
{
	return R_CGC_Control(
		PDL_NO_DATA,
		PDL_CGC_LOCO_DISABLE,
		PDL_NO_DATA
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Start_MAIN(void)
*
* Function Name: R_PG_Clock_Start_MAIN
*
* Description  : メインクロック発振器の動作
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Start_MAIN(void)
{
	return R_CGC_Control(
		PDL_NO_DATA,
		PDL_CGC_MAIN_ENABLE,
		PDL_NO_DATA
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Stop_MAIN(void)
*
* Function Name: R_PG_Clock_Stop_MAIN
*
* Description  : メインクロック発振器の停止
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Stop_MAIN(void)
{
	return R_CGC_Control(
		PDL_NO_DATA,
		PDL_CGC_MAIN_DISABLE,
		PDL_NO_DATA
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Enable_MAIN_ForcedOscillation(void)
*
* Function Name: R_PG_Clock_Enable_MAIN_ForcedOscillation
*
* Description  : メインクロック発振器強制発振の有効化
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Enable_MAIN_ForcedOscillation(void)
{
	return R_CGC_Control(
		PDL_NO_DATA,
		PDL_CGC_MAIN_FORCED_ENABLE,
		PDL_NO_DATA
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Disable_MAIN_ForcedOscillation(void)
*
* Function Name: R_PG_Clock_Disable_MAIN_ForcedOscillation
*
* Description  : メインクロック発振器強制発振の無効化
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Disable_MAIN_ForcedOscillation(void)
{
	return R_CGC_Control(
		PDL_NO_DATA,
		PDL_CGC_MAIN_FORCED_DISABLE,
		PDL_NO_DATA
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Start_PLL(void)
*
* Function Name: R_PG_Clock_Start_PLL
*
* Description  : PLL回路の動作
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Start_PLL(void)
{
	return R_CGC_Control(
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_CGC_PLL_ENABLE
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_Stop_PLL(void)
*
* Function Name: R_PG_Clock_Stop_PLL
*
* Description  : PLL回路の停止
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_CGC_Control
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_Stop_PLL(void)
{
	return R_CGC_Control(
		PDL_NO_DATA,
		PDL_NO_DATA,
		PDL_CGC_PLL_DISABLE
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_GetSelectedClockSource(uint8_t* clock)
*
* Function Name: R_PG_Clock_GetSelectedClockSource
*
* Description  : 内部クロックソースの取得
*
* Arguments    : uint8_t* clock : 選択クロックソース格納先
*
* Return Value : true  : 取得に成功した場合
*              : false : 取得に失敗した場合
*
* Calling Functions : R_CGC_GetStatus
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_GetSelectedClockSource(uint8_t* clock)
{
	uint16_t data;
	bool res;

	res = R_CGC_GetStatus( &data );

	if( clock ){
		*clock = (data >> 4) &0x07;
	}

	return res;
}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_GetClocksStatus(bool* pll, bool* main, bool* sub, bool* loco, bool* iwdt, bool* hoco)
*
* Function Name: R_PG_Clock_GetClocksStatus
*
* Description  : 各クロック状態取得
*
* Arguments    : bool* pll : PLL停止状態格納先
*              : bool* main : メインクロック停止状態格納先
*              : bool* sub : サブクロック停止状態格納先
*              : bool* loco : LOCO停止状態格納先
*              : bool* iwdt : IWDTクロック停止状態格納先
*              : bool* hoco : HOCO停止状態格納先
*
* Return Value : true  : 取得に成功した場合
*              : false : 取得に失敗した場合
*
* Calling Functions : R_CGC_GetStatus
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_GetClocksStatus(bool* pll, bool* main, bool* sub, bool* loco, bool* iwdt, bool* hoco)
{
	uint16_t data;
	bool res;

	res = R_CGC_GetStatus( &data );

	if( pll ){
		*pll = (data >> 8) &0x01;
	}
	if( main ){
		*main = (data >> 9) &0x01;
	}
	if( sub ){
		*sub = (data >> 10) &0x01;
	}
	if( loco ){
		*loco = (data >> 11) &0x01;
	}
	if( iwdt ){
		*iwdt = (data >> 12) &0x01;
	}
	if( hoco ){
		*hoco = (data >> 13) &0x01;
	}

	return res;
}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_GetHOCOPowerStatus(bool* power)
*
* Function Name: R_PG_Clock_GetHOCOPowerStatus
*
* Description  : 高速オンチップオシレータ(HOCO)電源状態取得
*
* Arguments    : bool* power : HOCO電源状態格納先
*
* Return Value : true  : 取得に成功した場合
*              : false : 取得に失敗した場合
*
* Calling Functions : R_CGC_GetStatus
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_GetHOCOPowerStatus(bool* power)
{
	uint16_t data;
	bool res;

	res = R_CGC_GetStatus( &data );

	if( power ){
		*power = (data >> 14) &0x01;
	}

	return res;
}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Clock_GetFlag_MAIN_StopDetection(bool* stop)
*
* Function Name: R_PG_Clock_GetFlag_MAIN_StopDetection
*
* Description  : メインクロック発振器停止検出フラグ取得
*
* Arguments    : bool* stop : 発振停止状態格納先
*
* Return Value : true  : フラグの取得が正しく行われた場合
*              : false : フラグの取得に失敗した場合
*
* Calling Functions : R_CGC_GetStatus
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_Clock_GetFlag_MAIN_StopDetection(bool* stop)
{
	uint16_t data;
	bool res;

	res = R_CGC_GetStatus( &data );

	if( stop ){
		*stop = data &0x01;
	}

	return res;
}



