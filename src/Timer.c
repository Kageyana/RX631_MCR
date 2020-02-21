//====================================//
// インクルード									//
//====================================//
#include "Timer.h"
//====================================//
// グローバル変数の宣言								//
//====================================//
volatile unsigned short	cnt0;		// 関数用タイマ
unsigned int 			cnt1;		// 走行用タイマカウント
unsigned short	 		cnt_out;	// コースアウト判定用タイマ
unsigned short	 		cnt_out2;	// コースアウト判定用タイマ2
unsigned short	 		cnt_out3;	// コースアウト判定用タイマ3
unsigned short	 		cnt_out4;	// コースアウト判定用タイマ4
static char				Timer10;	// 1msカウント用
///////////////////////////////////////////////////////////////////////////
// モジュール名 Timer									//
// 処理概要     1msごとにタイマ割り込み						//
// 引数         なし										//
// 戻り値       なし										//
///////////////////////////////////////////////////////////////////////////
void Timer (void) {
	__setpsw_i();
	//　タイマカウント
	if ( pattern < 11 ) {
		cnt_setup++;
		cnt_setup2++;
		cnt_setup3++;
		cnt_swR++;
		cnt_swL++;
	}
	cnt0++;
	cnt1++;
			
	// LCD表示
	if ( lcd_mode ) lcdShowProcess();

	// エンコーダカウント
	getEncoder();

	// PID制御値算出
	if ( angle_mode ) servoControl2();	// 角度
	else 			servoControl();		// 白線
	motorControl();		// モータ
	
	
	Timer10++;
	// 10ｍごとに実行
	switch ( Timer10 ) {	
	case 1:
		getSwitch();		// スイッチ読み込み
		get_voltage();		// 電源電圧取得
		break;
	case 2:
		break;
	case 3:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		Timer10 = 0;
		break;
	default:
		break;
	}
}