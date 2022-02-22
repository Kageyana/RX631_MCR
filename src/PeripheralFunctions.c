//====================================//
// インクルード
//====================================//
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include <stdio.h>
#include <string.h>

//====================================//
// グローバル変数の宣言
//====================================//
// タイマ関連
volatile unsigned short	cnt0;		// 関数用タイマ
static char				ADTimer10;	// AD変換カウント用

// スイッチ関連
static unsigned char 	dpsw_d[4];	// ディップスイッチの格納先
static unsigned char	tasw_d[5];	// スイッチ値の格納先

// センサ関連
static unsigned short 	result[14]; 	// 12bitA/D変換結果の格納先
static int			senR;		// 右アナログセンサ積算AD値
static int			senL;		// 左アナログセンサ積算AD値
static int			senG;		// ゲートセンサ積算AD値
static int			senC;		// 中心アナログセンサ積算AD値
static int			senLL;		// 最左端アナログセンサ積算AD値
static int			senRR;		// 最右端アナログセンサ積算AD値
static int			VolC;		// 電圧チェッカーAD値
static int			pot;		// ポテンションメーター積算AD値
short 				Angle;		// ポテンションメーター平均AD値
short				sensorR;	// 右アナログセンサ平均AD値
short				sensorL;	// 左アナログセンサ平均AD値
short				sensorG;	// ゲートセンサ平均AD値
short				sensorG_th = GATE_VAL;	// ゲート開放しきい値
short				sensorC;	// 中心アナログセンサ平均AD値
short				sensorLL;	// 最左端アナログセンサ平均AD値
short				sensorRR;	// 最右端アナログセンサ平均AD値
short				VoltageC;	// 電圧チェッカーAD値平均値
short				Angle0;		// サーボセンター値

double		Voltage;

// エンコーダ関連
static unsigned short 	cnt_Encoder;	// エンコーダ値の格納先
static unsigned short	encbuff;		// 前回のエンコーダ値
short				Encoder;			// 1msごとのパルス
unsigned int		EncoderTotal;		// 総走行距離
unsigned int		enc1;				// 走行用距離カウント
unsigned int		enc_slope;			// 坂上距離カウント

// モーター関連
signed char		accele_fR;		// 右前モーターPWM値
signed char		accele_fL;		// 左前モーターPWM値
signed char		accele_rR;		// 右後モーターPWM値
signed char		accele_rL;		// 左後モーターPWM値
signed char		sPwm;			// サーボモーターPWM値

/////////////////////////////////////////////////////////////////////
// モジュール名 ADconverter
// 処理概要     AD変換割り込み
// 引数         なし
// 戻り値       なし
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
// モジュール名 init_IO
// 処理概要     IOポートの初期化
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void init_IO(void)
{
	// I/Oポートを設定
	R_PG_IO_PORT_Set_P1();
	R_PG_IO_PORT_Set_P2();
	R_PG_IO_PORT_Set_P3();
	R_PG_IO_PORT_Set_P5();
	R_PG_IO_PORT_Set_PA();
	R_PG_IO_PORT_Set_PB();
	R_PG_IO_PORT_Set_PC();
	R_PG_IO_PORT_Set_PE();
	
	// すべてのIOポートをLOWにする
	R_PG_IO_PORT_Write_P1(0);
	R_PG_IO_PORT_Write_P2(0);
	R_PG_IO_PORT_Write_P3(0);
	R_PG_IO_PORT_Write_P5(0);
	R_PG_IO_PORT_Write_PA(0);
	R_PG_IO_PORT_Write_PB(0);
	R_PG_IO_PORT_Write_PC(0);
	R_PG_IO_PORT_Write_PE(0);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 led_out
// 処理概要     LEDの点灯
// 引数         rgb 	0x1:青 0x2緑 0x4
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void led_out ( char rgb )
{
	if ( (rgb & 0x4) == 0x4 ) LEDR_ON
	else LEDR_OFF
	
	if ( (rgb & 0x2) == 0x2 ) LEDG_ON
	else LEDG_OFF
	
	if ( (rgb & 0x1) == 1 ) LEDB_ON
	else LEDB_OFF
}
/////////////////////////////////////////////////////////////////////
// モジュール名 get_voltage
// 処理概要     電圧の取得
// 引数         なし
// 戻り値       な
/////////////////////////////////////////////////////////////////////
void get_voltage ( void )
{
	Voltage = VoltageC * 5.05 * 3.94 / 4096;
}
/////////////////////////////////////////////////////////////////////////////////
// モジュール名 getEncoder
// 処理概要     エンコーダのカウントを取得し積算する(1msごとに実行)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////////////////
void getEncoder (void)
{
	ENCODER_COUNT			// エンコーダカウント値取得
	Encoder = cnt_Encoder - encbuff;// 現在地から1ms前の値を引いて1ms間のカウントを計算
	
	// 積算
	EncoderTotal += Encoder;
	enc1 += Encoder;
	enc_slope += Encoder;
	
	encbuff = cnt_Encoder;	// 次回はこの値が1ms前の値となる
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getSwitch
// 処理概要     スイッチの読み込み(10msごとに実行)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getSwitch(void)
{
	// タクトスイッチ読み込み
	TACTSWITCH1	// タクトスイッチ上
	TACTSWITCH2	// タクトスイッチ左
	TACTSWITCH3	// タクトスイッチ右
	TACTSWITCH4	// タクトスイッチ下
	TACTSWITCH5	// タクトスイッチ押し込み
	
	// ディップスイッチ読み込み
	DIPSWITCH1
	DIPSWITCH2
	DIPSWITCH3
	DIPSWITCH4
}
/////////////////////////////////////////////////////////////////////
// モジュール名 dipsw_get
// 処理概要     ディップスイッチ値を16進数で取得
// 引数         なし
// 戻り値       スイッチ値 0〜15
/////////////////////////////////////////////////////////////////////
unsigned char dipsw_get(void) 
{
	char	dpsw[4];
	
	if ( dpsw_d[0] == 0 )	dpsw[0] = 0x0;
	else			dpsw[0] = 0x1;
	
	if ( dpsw_d[1] == 0 )	dpsw[1] = 0x0;
	else			dpsw[1] = 0x2;
	
	if ( dpsw_d[2] == 0 )	dpsw[2] = 0x0;
	else			dpsw[2] = 0x4;

	if ( dpsw_d[3] == 0 )	dpsw[3] = 0x0;
	else			dpsw[3] = 0x8;

	return ( dpsw[0] + dpsw[1] + dpsw[2] + dpsw[3] );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 tasw_get
// 処理概要     タクトスイッチ値を16進数で取得
// 引数         なし
// 戻り値       スイッチ値 0〜7
///////////////////////////////////////////////////////////////////////////
unsigned char tasw_get(void) 
{
	char	tasw[5];
	
	if ( tasw_d[0] == 0 )	tasw[0] = 0x1;
	else			tasw[0] = 0x0;
	
	if ( tasw_d[1] == 0 )	tasw[1] = 0x2;	
	else			tasw[1] = 0x0;
	
	if ( tasw_d[2] == 0 )	tasw[2] = 0x4;	
	else			tasw[2] = 0x0;
	
	if ( tasw_d[3] == 0 )	tasw[3] = 0x8;	
	else			tasw[3] = 0x0;
	
	if ( tasw_d[4] == 0 )	tasw[4] = 0xf;
	else			tasw[4] = 0x0;

	return ( tasw[0] + tasw[1] + tasw[2] + tasw[3] + tasw[4] );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getServoAngle
// 処理概要     ポテンションメーターのアナログ値で取得
// 引数         なし
// 戻り値       センサ値
///////////////////////////////////////////////////////////////////////////
short getServoAngle(void) 
{	
	return  ( Angle0 - Angle );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getAnalogSensor
// 処理概要     アナログセンサのアナログ値で取得
// 引数         なし
// 戻り値       センサ値
///////////////////////////////////////////////////////////////////////////
short getAnalogSensor(void) 
{
	return sensorR - sensorL;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 sensor_inp
// 処理概要     デジタルセンサの値を16進数で取得
// 引数         なし
// 戻り値       センサ値0〜
///////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
// モジュール名 startbar_get
// 処理概要     スタートゲートの開閉の確認
// 引数         なし
// 戻り値       0; 閉じている 1; 開いてい
///////////////////////////////////////////////////////////////////////////
unsigned char startbar_get(void) 
{
	char ret;
	
	if ( sensorG <= sensorG_th )	ret = 1;
	else			ret = 0;
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motor_f
// 処理概要     モーターのPWMの変更
// 引数         accelefL, accelefR(PWMを1〜100%で指定)
// 戻り値       な
///////////////////////////////////////////////////////////////////////////
void motorPwmOut( signed char accelefL, signed char accelefR, signed char accelerL, signed char accelerR )
{
	uint16_t pwmfl, pwmfr, pwmrl, pwmrr;
	
	if ( mode_pushcart ) {
		accelefR = 0;
		accelefL = 0;
		accelerL = 0;
		accelerR = 0;
	}
	
	accele_fR = accelefR;
	accele_fL = accelefL;
	accele_rR = accelerR;
	accele_rL = accelerL;
	
	pwmfl = TGR_MOTOR * accelefL / 100;
	pwmfr = TGR_MOTOR * accelefR / 100;
	pwmrl = TGR_MOTOR * accelerL / 100;
	pwmrr = TGR_MOTOR * accelerR / 100;
	
	// 左前輪
	if( accelefL >= 0) DIR_FL_FOR
	else DIR_FL_REV
	if ( accelefL == 100 || accelefL == -100 )pwmfl = TGR_MOTOR + 2;
	PWM_FL_OUT
	
	// 右前輪
	if( accelefR >= 0) DIR_FR_FOR
	else DIR_FR_REV
	if ( accelefR == 100 || accelefR == -100 ) pwmfr = TGR_MOTOR + 2;
	PWM_FR_OUT

	// 左後輪
	if( accelerL >= 0 ) DIR_RL_FOR
	else DIR_RL_REV
	if ( accelerL == 100 || accelerL == -100 ) pwmrl = TGR_MOTOR + 2;
	PWM_RL_OUT
	
	// 右後輪
	if( accelerR >= 0 ) DIR_RR_FOR
	else DIR_RR_REV
	if ( accelerR == 100 || accelerR == -100 ) pwmrr = TGR_MOTOR + 2;
	PWM_RR_OUT
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 servoPwmOut
// 処理概要     白線トレース時サーボのPWMの変更
// 引数         spwm
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void servoPwmOut( signed char servopwm )
{
	uint16_t pwm;
	short angle;
	
	sPwm = servopwm;		// ログ用変数に代入
	//servopwm = -servopwm;		// 回転方向を変える
	
	// サーボリミット制御
	angle = getServoAngle();
	
	// 角度によるリミット制御
	if ( angle >= SERVO_LIMIT ) servopwm = -15;
	if ( angle <= -SERVO_LIMIT ) servopwm = 15;
	
	// ポテンションメーターが外れていたら制御しない
	if ( angle > SERVO_LIMIT + 100 ) servopwm = 0;
	if ( angle < -SERVO_LIMIT - 100 ) servopwm = 0;

	pwm = (uint16_t)TGR_SERVO * servopwm / 100;
	// サーボモータ制御
	if( servopwm > 0) {				
		// 正転
		DIR_SERVO_FOR
	} else {				
		// 逆転
		pwm = -pwm;
		DIR_SERVO_REV
	}
	PWM_SERVO_OUT
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 short_sort
// 処理概要     short型変数の比較をする
// 引数         比較する配列
// 戻り値       -1:a<b 0:a=b 1:a>b
///////////////////////////////////////////////////////////////////////////
int short_sort( const void* a, const void* b )
{
	// 引数はvoid*型と規定されているのでint型にcastする
	if( *( short * )a < *( short * )b ) {
		return -1;
	} else
	if( *( short * )a == *( short * )b ) {
		return 0;
		}
	return 1;
}