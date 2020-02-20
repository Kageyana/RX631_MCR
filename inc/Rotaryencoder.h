#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_
//====================================//
// インクルード									//
//====================================//
#include "R_PG_IGC-P8080_v1.h"
//====================================//
// シンボル定義									//
//====================================//
#define SPEED_CURRENT		25		// 1m/sの時　1msのパルス	1000*1回転あたりのパスル数/プーリ外径/pi
/*************************************** 自動生成関数 *************************/
// ADコンバータ
#define ENCODER_COUNT	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );	// カウント取得
/**************************************************************************/
//====================================//
// グローバル変数の宣言								//
//====================================//
// エンコーダ関連
extern unsigned int	EncoderTotal;	// 総走行距離
extern signed short	Encoder;		// 1msごとのパルス
extern unsigned int	enc1;		// 走行用距離カウント
extern unsigned int	enc_slope;		// 坂上距離カウント
//====================================//
// プロトタイプ宣言									//
//====================================//
// エンコーダ関連
void getEncoder (void);
// エンコーダ関連
unsigned int enc_mm( short mm );
#endif // ROTARYENCODER_H_