#ifndef ADCONVERTER_H_
#define ADCONVERTER_H_
//====================================//
// インクルード
//====================================//
#include "R_PG_IGC-P8080_v1.h"
//====================================//
// シンボル定義
//====================================//
#define GATE_VAL			190		    // ゲートセンサしきい値
#define DEGPERAD            40.55       // 1度あたりのAD値 サーボ最大切れ角時のAD値[]/サーボ最大切れ角[°]
#define LOWVOLTAGE          10.5        // 最低動作電圧
/*************************************** 自動生成関数 *************************************/
// ADコンバータ
#define SET_ADC		R_PG_ADC_12_Set_S12AD0(); 				// 12ビットA/Dコンバータ(S12AD0)を設定
#define START_ADC	R_PG_ADC_12_StartConversionSW_S12AD0();	// A/D変換開始
#define GET_ADC		R_PG_ADC_12_GetResult_S12AD0( result );	// AD値を取得
/******************************************************************************************/

//====================================//
// グローバル変数の宣言
//====================================//
// センサ関連
extern short		sensorR;		// 右アナログセンサ
extern short		sensorL;		// 左アナログセンサ
extern short		sensorG;		// ゲートセンサ
extern short		sensorG_th;	// ゲートセンサ
extern short		sensorC;		// 中心アナログセンサ
extern short		sensorLL;		// 最左端アナログセンサ
extern short		sensorRR;		// 最右端アナログセンサ

extern short		L_sencnt;

extern double		Voltage;        //電圧チェッカー

extern short		Angle0;		    // サーボセンター値
//====================================//
// プロトタイプ宣言
//====================================//
// センサ関連
short getServoAngle(void);
short getAnalogSensor( void );
unsigned char sensor_inp( void );
unsigned char startbar_get( void );

#endif // ADCONVERTER_H_