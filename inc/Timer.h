#ifndef TIMER_H_
#define TIMER_H_
//====================================//
// インクルード									//
//====================================//
#include "R_PG_IGC-P8080_v1.h"
#include "ADC.h"
#include "IO.h"
#include "MOTOR.h"
#include "Rotaryencoder.h"
#include "Setup.h"
//====================================//
// シンボル定義									//
//====================================//
/*************************************** 自動生成関数 *************************/
// タイマ割り込み
#define SET_CMT_C0		R_PG_Timer_Set_CMT_U0_C0();			// コンペアマッチタイマ初期化(ch0)
#define START_CMT_C0	R_PG_Timer_StartCount_CMT_U0_C0();	// カウントスタート(ch0)
#define STOP_CMT_C0	R_PG_Timer_HaltCount_CMT_U0_C0();	// カウント一時停止(ch0)// タイマ割り込み
#define SET_CMT_C0		R_PG_Timer_Set_CMT_U0_C0();			// コンペアマッチタイマ初期化(ch0)
#define START_CMT_C0	R_PG_Timer_StartCount_CMT_U0_C0();	// カウントスタート(ch0)
#define STOP_CMT_C0	R_PG_Timer_HaltCount_CMT_U0_C0();	// カウント一時停止(ch0)
/**************************************************************************/
//====================================//
// グローバル変数の宣言								//
//====================================//
// タイマ関連
extern volatile unsigned short		cnt0;		// 関数用タイマ
extern unsigned int 				cnt1;		// 走行用タイマカウント
extern unsigned short	 			cnt_out;	// コースアウト判定用タイマ
extern unsigned short	 			cnt_out2;	// コースアウト判定用タイマ2
extern unsigned short	 			cnt_out3;	// コースアウト判定用タイマ3
extern unsigned short	 			cnt_out4;	// コースアウト判定用タイマ4
//====================================//
// プロトタイプ宣言									//
//====================================//

#endif // TIMER_H_