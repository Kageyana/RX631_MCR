#ifndef SETUP_H_
#define SETUP_H_
//======================================//
// インクルード
//======================================//
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "E2dataFlash.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "SCI.h"
#include "SPI_ICM20648.h"
#include "MemorryTrace.h"
#include <stdio.h>
//======================================//
// シンボル定義
//======================================//
#define UD	0
#define LR		1

#define START_COUNT	1
#define START_GATE		2

//======================================//
// グローバル変数の宣言
//======================================//
// パターン関連
extern char		start;

// タイマ関連
extern unsigned short 	cnt_setup;
extern unsigned short 	cnt_setup2;
extern unsigned short 	cnt_setup3;	
extern short		cnt_swR;	// スイッチ長押し判定用右
extern short		cnt_swL;	// スイッチ長押し判定用左

// パラメータ関連
extern char fixSpeed;

//======================================//
// プロトタイプ宣言
//======================================//
void setup(void);
char fix_speedsetting ( void );

#endif /* SCI_H_ */