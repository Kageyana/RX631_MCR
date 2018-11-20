/************************************************************************
*
* Device     : RX/RX600/RX63N,RX631
*
* File Name  : hwsetup.c
*
* Abstract   : Hardware Setup file.
*
* History    : 0.10  (2011-02-21)  [Hardware Manual Revision : 0.01]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2011 Renesas Electronics Corporation.
* and Renesas Solutions Corp.
*
************************************************************************/

#include "iodefine.h"
#include "PeripheralFunctions.h"
#include "MicroSD.h"
#include "I2C_LCD.h"
#include "SCI.h"

extern void HardwareSetup(void);

// IDコード設定(45ffffffffffffffffffffffffffffff)
#pragma address id_code=0xffffffa0 // ID codes (Default)
const unsigned long id_code[4] = {
        0x45ffffff,
        0xffffffff,
        0xffffffff,
        0xffffffff,
};

void HardwareSetup(void)
{
	R_PG_IO_PORT_SetPortNotAvailable();	// 存在しないポートを設定
	R_PG_Clock_WaitSet(0.01); 		// クロックを設定し0.01秒後にクロックソース切り替え
	
	SET_MTU_C0		// マルチファンクションタイマを設定
	SET_MTU_C1
	SET_MTU_C2
	SET_MTU_C3
	
	SET_SCI_C1
	SET_SCI_C5 		// シリアルI/Oチャネルを設定(SPI)
	SET_SCI_C12 		// シリアルI/Oチャネルを設定(I2C)
	
	SET_CMT_C0		// コンペアマッチタイマを設定(ch0)
	SET_CMT_C2		// コンペアマッチタイマを設定(ch2)
	
	init_IO();			// IOポートの初期化
	
	SET_ADC			// 12ビットA/Dコンバータ(S12AD0)を設定
	
	START_MTU		// MTU0,2,3,4のカウント開始
	
	START_ADC		// A/D変換開始
	START_CMT_C0 	// カウントスタート(ch0)
	START_CMT_C2 	// カウントスタート(ch2)
}
