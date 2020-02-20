//====================================//
// インクルード									//
//====================================//
#include "ADC.h"
//====================================//
// グローバル変数の宣言								//
//====================================//
// タイマ関連
static char			ADTimer10;	// AD変換カウント用
// センサ関連
static unsigned short 	result[14]; 	// 12bitA/D変換結果の格納先
static int			senR;	// 右アナログセンサ積算AD値
static int			senL;		// 左アナログセンサ積算AD値
static int			senG;	// ゲートセンサ積算AD値
static int			senC;	// 中心アナログセンサ積算AD値
static int			senLL;	// 最左端アナログセンサ積算AD値
static int			senRR;	// 最右端アナログセンサ積算AD値
static int			VolC;		// 電圧チェッカーAD値
static int			pot;		// ポテンションメーター積算AD値
short 			Angle;	// ポテンションメーター平均AD値
short				sensorR;	// 右アナログセンサ平均AD値
short				sensorL;	// 左アナログセンサ平均AD値
short				sensorG;	// ゲートセンサ平均AD値
short				sensorG_th = GATE_VAL;	// ゲート開放しきい値
short				sensorC;	// 中心アナログセンサ平均AD値
short				sensorLL;	// 最左端アナログセンサ平均AD値
short				sensorRR;	// 最右端アナログセンサ平均AD値
short				VoltageC;	// 電圧チェッカーAD値平均値
short				Angle0;	// サーボセンター値

double		Voltage;
/////////////////////////////////////////////////////////////////////
// モジュール名 ADconverter							//
// 処理概要     AD変換割り込み						//
// 引数         なし									//
// 戻り値       なし									//
/////////////////////////////////////////////////////////////////////
void ADconverter ( void )
{
	__setpsw_i();
	GET_ADC
	
	ADTimer10++;
	if ( ADTimer10 == 10 ) {
		ADTimer10 = 0;
		
		Angle = pot / 10;
		sensorR = senR / 10;
		sensorL = senL / 10;
		sensorG = senG / 10;
		sensorC = senC / 10;
		sensorLL = senLL / 10;
		sensorRR = senRR / 10;
		VoltageC = VolC /10;
		
		senR = 0;
		senL = 0;
		senG = 0;
		senC = 0;
		senLL = 0;
		senRR = 0;
		
		VolC = 0;
		pot = 0;
	}
	
	// AD変換値をバッファに格納
	pot += result[3];
	senG += result[4];
	senLL += result[5];
	senL += result[6];
	senC += result[7];
	senRR += result[8];
	senR += result[9];
	VolC += result[11];
	
}
/////////////////////////////////////////////////////////////////////
// モジュール名 get_voltage							//
// 処理概要     電圧の取得							//
// 引数         なし									//
// 戻り値       なし									//
/////////////////////////////////////////////////////////////////////
void get_voltage ( void )
{
	Voltage = VoltageC * 5.05 * 3.94 / 4096;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getServoAngle							//
// 処理概要     ポテンションメーターのアナログ値で取得			//
// 引数         なし									//
// 戻り値       センサ値								//
/////////////////////////////////////////////////////////////////////
short getServoAngle(void) 
{	
	return  ( Angle0 - Angle );
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getAnalogSensor						//
// 処理概要     アナログセンサのアナログ値で取得				//
// 引数         なし									//
// 戻り値       センサ値								//
/////////////////////////////////////////////////////////////////////
short getAnalogSensor(void) 
{
	return sensorR - sensorL;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 sensor_inp							//
// 処理概要     デジタルセンサの値を16進数で取得				//
// 引数         なし									//
// 戻り値       センサ値0～7							//
/////////////////////////////////////////////////////////////////////
unsigned char sensor_inp(void) 
{
	char l, c, r;
	
	if (sensorRR < 500 ) r = 0x1;
	else r = 0;
	if (sensorC < 500 ) c = 0x2;
	else c = 0;
	if (sensorLL < 500 ) l = 0x4;
	else l = 0;
	
	return l+c+r;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 startbar_get							//
// 処理概要     スタートゲートの開閉の確認					//
// 引数         なし									//
// 戻り値       0; 閉じている 1; 開いている					//
/////////////////////////////////////////////////////////////////////
unsigned char startbar_get(void) 
{
	char ret;
	
	if ( sensorG <= sensorG_th )	ret = 1;
	else			ret = 0;
	
	return ret;
}