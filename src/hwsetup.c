﻿/************************************************************************
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
#include "io.h"
#include "mtu.h"
#include "ADconverter.h"
#include "control.h"
#include "microSD.h"
#include "AQM0802A.h"
#include "ICM20648.h"
#include "sci.h"

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
	SET_MTU_C4
	
	SET_SCI_C2
	SET_SCI_MSD		// シリアルI/Oチャネルを設定(SPI microSd)
	initSCI1( RATE_230400 );		// シリアルI/Oチャネルを設定(UART)
	SET_SCI_LCD 		// シリアルI/Oチャネルを設定(I2C)
	
	SET_CMT_C0		// コンペアマッチタイマを設定(ch0)
	SET_CMT_MSD		// コンペアマッチタイマを設定(ch2)
	
	initIO();			// IOポートの初期化
	
	SET_ADC			// 12ビットA/Dコンバータ(S12AD0)を設定
	
	START_MTU		// MTU0,1,2,3,4のカウント開始
	
	START_ADC		// A/D変換開始
	START_CMT_C0 	// カウントスタート(ch0)
	START_CMT_MSD 	// カウントスタート(ch2)
	
	/*----------0:brake　1:stop----------*/	
	R_PG_IO_PORT_Write_PE4( 0 );	//左前モータ
	R_PG_IO_PORT_Write_PA0( 0 );	//右前モータ
	R_PG_IO_PORT_Write_PC5( 0 );	//左後モータ
	R_PG_IO_PORT_Write_PB4( 0 );	//右後モータ
	R_PG_IO_PORT_Write_PE6( 1 );	//サーボモータ
	R_PG_IO_PORT_Write_PC3( 1 );	//ランサーモータ
}
