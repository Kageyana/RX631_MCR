#ifndef SETUP_H_
#define SETUP_H_
//======================================//
// インクルード
//======================================//
#include "io.h"
#include "mtu.h"
#include "ADconverter.h"
#include "control.h"
#include "E2dataFlash.h"
#include "AQM0802A.h"
#include "MicroSD.h"
#include "sci.h"
#include "ICM20648.h"
#include "MemorryTrace.h"
#include <stdio.h>
//======================================//
// シンボル定義
//======================================//
#define UD	0
#define LR	1

#define START_COUNT	    1
#define START_GATE		2

//======================================//
// グローバル変数の宣言
//======================================//
// パターン関連
extern char		start;

// タイマ関連
extern unsigned short 	cntSetup1;
extern unsigned short 	cntSetup2;
extern unsigned short 	cntSetup3;	
extern short		cntSwitchUD;	// スイッチ長押し判定用右
extern short		cntSwitchLR;	// スイッチ長押し判定用左

// パラメータ関連
extern char fixSpeed;

//======================================//
// プロトタイプ宣言
//======================================//
void setup(void);
char fixSpeedSetting ( void );

#endif /* SCI_H_ */