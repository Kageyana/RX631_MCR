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
* File Name    : R_PG_IO_PORT_PC.c
* Version      : 1.00
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
* Limitations  : 
******************************************************************************
* History : 17.09.2018 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_pdl_io_port.h"
#include "r_pdl_definitions.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PC(void)
*
* Function Name: R_PG_IO_PORT_Set_PC
*
* Description  : I/Oポートの設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Set_PC(void)
{
	bool res;

	res = R_IO_PORT_Set(
		PDL_IO_PORT_C_0,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_ON
	);

	if( !res ){
		return res;
	}

	res = R_IO_PORT_Set(
		PDL_IO_PORT_C_1,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_ON
	);

	if( !res ){
		return res;
	}

	res = R_IO_PORT_Set(
		PDL_IO_PORT_C_2,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_ON
	);

	if( !res ){
		return res;
	}

	res = R_IO_PORT_Set(
		PDL_IO_PORT_C_3,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_ON
	);

	if( !res ){
		return res;
	}

	res = R_IO_PORT_Set(
		PDL_IO_PORT_C_4,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_OFF
	);

	if( !res ){
		return res;
	}

	res = R_IO_PORT_Set(
		PDL_IO_PORT_C_5,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_OFF
	);

	if( !res ){
		return res;
	}

	res = R_IO_PORT_Set(
		PDL_IO_PORT_C_6,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_OFF
	);

	if( !res ){
		return res;
	}

	return res;
}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PC0(void)
*
* Function Name: R_PG_IO_PORT_Set_PC0
*
* Description  : I/Oポート(1端子)の設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Set_PC0(void)
{
	return R_IO_PORT_Set(
		PDL_IO_PORT_C_0,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_ON
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PC1(void)
*
* Function Name: R_PG_IO_PORT_Set_PC1
*
* Description  : I/Oポート(1端子)の設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Set_PC1(void)
{
	return R_IO_PORT_Set(
		PDL_IO_PORT_C_1,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_ON
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PC2(void)
*
* Function Name: R_PG_IO_PORT_Set_PC2
*
* Description  : I/Oポート(1端子)の設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Set_PC2(void)
{
	return R_IO_PORT_Set(
		PDL_IO_PORT_C_2,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_ON
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PC3(void)
*
* Function Name: R_PG_IO_PORT_Set_PC3
*
* Description  : I/Oポート(1端子)の設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Set_PC3(void)
{
	return R_IO_PORT_Set(
		PDL_IO_PORT_C_3,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_ON
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PC4(void)
*
* Function Name: R_PG_IO_PORT_Set_PC4
*
* Description  : I/Oポート(1端子)の設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Set_PC4(void)
{
	return R_IO_PORT_Set(
		PDL_IO_PORT_C_4,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_OFF
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PC5(void)
*
* Function Name: R_PG_IO_PORT_Set_PC5
*
* Description  : I/Oポート(1端子)の設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Set_PC5(void)
{
	return R_IO_PORT_Set(
		PDL_IO_PORT_C_5,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_OFF
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PC6(void)
*
* Function Name: R_PG_IO_PORT_Set_PC6
*
* Description  : I/Oポート(1端子)の設定
*
* Arguments    : なし
*
* Return Value : true  : 設定が正しく行われた場合
*              : false : 設定に失敗した場合
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Set_PC6(void)
{
	return R_IO_PORT_Set(
		PDL_IO_PORT_C_6,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_OFF
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC
*
* Description  : I/Oポートレジスタからの読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC0(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC0
*
* Description  : I/Oポートレジスタからのビット読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC0(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C_0,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC1(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC1
*
* Description  : I/Oポートレジスタからのビット読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC1(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C_1,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC2(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC2
*
* Description  : I/Oポートレジスタからのビット読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC2(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C_2,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC3(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC3
*
* Description  : I/Oポートレジスタからのビット読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC3(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C_3,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC4(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC4
*
* Description  : I/Oポートレジスタからのビット読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC4(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C_4,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC5(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC5
*
* Description  : I/Oポートレジスタからのビット読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC5(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C_5,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC6(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC6
*
* Description  : I/Oポートレジスタからのビット読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC6(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C_6,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PC7(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PC7
*
* Description  : I/Oポートレジスタからのビット読み出し
*
* Arguments    : uint8_t * data : 読み出した端子状態の格納先
*
* Return Value : true  : 読み出しが正しく行われた場合
*              : false : 読み出しに失敗した場合
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Read_PC7(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_C_7,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PC(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PC
*
* Description  : I/Oポートデータレジスタへの書き込み
*
* Arguments    : uint8_t data : 書き込む値
*
* Return Value : true  : 書き込みに成功した場合
*              : false : 書き込みに失敗した場合
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Write_PC(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_C,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PC0(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PC0
*
* Description  : I/Oポートデータレジスタへのビット書き込み
*
* Arguments    : uint8_t data : 書き込む値
*
* Return Value : true  : 書き込みに成功した場合
*              : false : 書き込みに失敗した場合
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Write_PC0(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_C_0,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PC1(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PC1
*
* Description  : I/Oポートデータレジスタへのビット書き込み
*
* Arguments    : uint8_t data : 書き込む値
*
* Return Value : true  : 書き込みに成功した場合
*              : false : 書き込みに失敗した場合
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Write_PC1(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_C_1,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PC2(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PC2
*
* Description  : I/Oポートデータレジスタへのビット書き込み
*
* Arguments    : uint8_t data : 書き込む値
*
* Return Value : true  : 書き込みに成功した場合
*              : false : 書き込みに失敗した場合
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Write_PC2(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_C_2,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PC3(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PC3
*
* Description  : I/Oポートデータレジスタへのビット書き込み
*
* Arguments    : uint8_t data : 書き込む値
*
* Return Value : true  : 書き込みに成功した場合
*              : false : 書き込みに失敗した場合
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Write_PC3(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_C_3,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PC4(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PC4
*
* Description  : I/Oポートデータレジスタへのビット書き込み
*
* Arguments    : uint8_t data : 書き込む値
*
* Return Value : true  : 書き込みに成功した場合
*              : false : 書き込みに失敗した場合
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Write_PC4(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_C_4,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PC5(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PC5
*
* Description  : I/Oポートデータレジスタへのビット書き込み
*
* Arguments    : uint8_t data : 書き込む値
*
* Return Value : true  : 書き込みに成功した場合
*              : false : 書き込みに失敗した場合
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Write_PC5(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_C_5,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PC6(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PC6
*
* Description  : I/Oポートデータレジスタへのビット書き込み
*
* Arguments    : uint8_t data : 書き込む値
*
* Return Value : true  : 書き込みに成功した場合
*              : false : 書き込みに失敗した場合
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : 詳細についてはリファレンスマニュアルを参照してください。
******************************************************************************/
bool R_PG_IO_PORT_Write_PC6(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_C_6,
		data
	);

}



