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
#include "R_PG_RX631_mcr_ver3.0.h"
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
	R_PG_IO_PORT_SetPortNotAvailable();		// 存在しないポートを設定
	R_PG_Clock_WaitSet(0.01); 			// クロックを設定し0.01秒後にクロックソース切り替え
	
	R_PG_Timer_Set_MTU_U0_C0();			// マルチファンクションタイマを設定
	R_PG_Timer_Set_MTU_U0_C1();
	R_PG_Timer_Set_MTU_U0_C2();
	R_PG_Timer_Set_MTU_U0_C3();
	
	R_PG_SCI_Set_C5(); 				// シリアルI/Oチャネルを設定(SPI)
	R_PG_SCI_Set_C12(); 				// シリアルI/Oチャネルを設定(I2C)
	
	R_PG_Timer_Set_CMT_U0_C0(); 			// コンペアマッチタイマを設定
	R_PG_Timer_Set_CMT_U1_C2(); 			// コンペアマッチタイマを設定
	
	// I/Oポートを設定
	R_PG_IO_PORT_Set_P1();
	R_PG_IO_PORT_Set_P2();
	R_PG_IO_PORT_Set_P5();
	R_PG_IO_PORT_Set_PA();
	R_PG_IO_PORT_Set_PB();
	R_PG_IO_PORT_Set_PC();
	R_PG_IO_PORT_Set_PD();
	
	R_PG_IO_PORT_Write_P1(0);
	R_PG_IO_PORT_Write_P2(0);
	R_PG_IO_PORT_Write_P5(0);
	R_PG_IO_PORT_Write_P5(0);
	R_PG_IO_PORT_Write_PA(0);
	R_PG_IO_PORT_Write_PB(0);
	R_PG_IO_PORT_Write_PC(0);
	
	R_PG_ADC_12_Set_S12AD0(); 			// 12ビットA/Dコンバータ(S12AD0)を設定
	
	R_PG_Timer_SynchronouslyStartCount_MTU_U0(	// MTU0,2,3,4のカウント開始
	1, //ch0
	1, //ch1 
	1, //ch2
	1, //ch3 
	0  //ch4 0固定
	);
	R_PG_ADC_12_StartConversionSW_S12AD0();		// A/D変換開始
	R_PG_Timer_StartCount_CMT_U0_C0(); 		// カウントスタート
	R_PG_Timer_StartCount_CMT_U1_C2(); 		// カウントスタート
}
