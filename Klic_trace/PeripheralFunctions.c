//====================================//
// インクルード									//
//====================================//
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "iodefine.h"
#include "SCI.h"
#include "I2C_MPU-9255.h"
#include <stdio.h>
#include <string.h>

//====================================//
// グローバル変数の宣言							//
//====================================//
// タイマ関連
volatile unsigned short	cnt0;		// 関数用タイマ
static char			ADTimer10;	// AD変換カウント用

// スイッチ関連
static unsigned char 	dpsw_d[4];	// ディップスイッチの格納先
static unsigned char	tasw_d[4];	// スイッチ値の格納先

// センサ関連
static unsigned short 	result[14]; 	// 12bitA/D変換結果の格納先
static int			senR;	// 右アナログセンサ積算AD値
static int			senL;		// 左アナログセンサ積算AD値
static int			senG;	// ゲートセンサ積算AD値
static int			senC;	// 中心アナログセンサ積算AD値
static int			senLL;	// 最左端アナログセンサ積算AD値
static int			senRR;	// 最右端アナログセンサ積算AD値
static int			gy;		// ジャイロセンサ積算AD値
short 			gyro;		// ジャイロセンサ平均AD値
static int			pot;		// ポテンションメーター積算AD値
short 			Angle;	// ポテンションメーター平均AD値
short				sensorR;	// 右アナログセンサ平均AD値
short				sensorL;	// 左アナログセンサ平均AD値
short				sensorG;	// ゲートセンサ平均AD値
short				sensorC;	// 中心アナログセンサ平均AD値
short				sensorLL;	// 最左端アナログセンサ平均AD値
short				sensorRR;	// 最右端アナログセンサ平均AD値
short				Angle0;	// サーボセンター値

// ブザー関連
unsigned short 		BeepPattern;	// ビープ音の出力パターン
unsigned short 		BeepTimer;	// 50msごとのタイマ
static char			BeepMode;	// swich用変数

// エンコーダ関連
static unsigned short 	cnt_Encoder;	// エンコーダ値の格納先
static unsigned short	encbuff;		// 前回のエンコーダ値
short				Encoder;		// 1msごとのパルス
unsigned int		EncoderTotal;	// 総走行距離
unsigned int		enc1;		// 走行用距離カウント
unsigned int		enc2;		// コース記憶走行用距離カウント
unsigned int		enc_slope;		// 坂上距離カウント

// モーター関連
signed char		accele_fR;		// 右前モーターPWM値
signed char		accele_fL;		// 左前モーターPWM値
signed char		accele_rR;		// 右後モーターPWM値
signed char		accele_rL;		// 左後モーターPWM値
signed char		sPwm;		// サーボモーターPWM値

/////////////////////////////////////////////////////////////////////
// モジュール名 ADconverter						//
// 処理概要     AD変換割り込み						//
// 引数         なし								//
// 戻り値       なし								//
/////////////////////////////////////////////////////////////////////
void ADconverter ( void )
{
	__setpsw_i();
	GET_ADC
	
	ADTimer10++;
	if ( ADTimer10 == 10 ) {
		ADTimer10 = 0;
		
		gyro = gy / 10;
		Angle = pot / 10;
		sensorR = senR / 10;
		sensorL = senL / 10;
		sensorC = senC / 10;
		sensorLL = senLL / 10;
		sensorRR = senRR / 10;
		sensorG = senG / 10;
		
		senLL = 0;
		senL = 0;
		senG = 0;
		senC = 0;
		senR = 0;
		senRR = 0;
		gy = 0;
		pot = 0;
	}
	
	// AD変換値をバッファに格納
	senLL += result[3];
	senL += result[4];
	senG += result[5];
	senC += result[6];
	senR += result[7];
	senRR += result[8];
	gy += result[12];
	pot += result[13];
	
}
/////////////////////////////////////////////////////////////////////
// モジュール名 init_IO								//
// 処理概要     IOポートの初期化						//
// 引数         なし								//
// 戻り値       なし                                                   		//
/////////////////////////////////////////////////////////////////////
void init_IO(void)
{
	// I/Oポートを設定
	R_PG_IO_PORT_Set_P1();
	R_PG_IO_PORT_Set_P2();
	R_PG_IO_PORT_Set_P5();
	R_PG_IO_PORT_Set_PA();
	R_PG_IO_PORT_Set_PB();
	R_PG_IO_PORT_Set_PC();
	R_PG_IO_PORT_Set_PD();
	
	// すべてのIOポートをLOWにする
	R_PG_IO_PORT_Write_P1(0);
	R_PG_IO_PORT_Write_P2(0);
	R_PG_IO_PORT_Write_P5(0);
	R_PG_IO_PORT_Write_PA(0);
	R_PG_IO_PORT_Write_PB(0);
	R_PG_IO_PORT_Write_PC(0);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 led_out							//
// 処理概要     LEDの点灯							//
// 引数         led(対応するLED番号)					//
// 戻り値       なし								//
/////////////////////////////////////////////////////////////////////
void led_out ( unsigned char led )
{
	unsigned char led2;

	led2 = led << 1;
	LED_OUT
}
/////////////////////////////////////////////////////////////////////////////////
// モジュール名 getEncoder									//
// 処理概要     エンコーダのカウントを取得し積算する(1msごとに実行)	//
// 引数         なし										//
// 戻り値       なし										//
/////////////////////////////////////////////////////////////////////////////////
void getEncoder (void)
{
	ENCODER_COUNT			// エンコーダカウント値取得
	Encoder = cnt_Encoder - encbuff;// 現在地から1ms前の値を引いて1ms間のカウントを計算
	
	// 積算
	EncoderTotal += Encoder;
	enc1 += Encoder;
	enc2 += Encoder;
	enc_slope += Encoder;
	
	encbuff = cnt_Encoder;	// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getSwitch								//
// 処理概要     スイッチの読み込み(10msごとに実行)			//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void getSwitch(void)
{
	// タクトスイッチ読み込み
	TACTSWITCH1	// タクトスイッチ右上
	TACTSWITCH2	// タクトスイッチ右下
	TACTSWITCH3	// タクトスイッチ左上
	TACTSWITCH4	// タクトスイッチ左下
	
	// ディップスイッチ読み込み
	DIPSWITCH1
	DIPSWITCH2
	DIPSWITCH3
	DIPSWITCH4
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 dipsw_get								//
// 処理概要     ディップスイッチ値を16進数で取得				//
// 引数         なし									//
// 戻り値       スイッチ値 0～15							//
///////////////////////////////////////////////////////////////////////////
unsigned char dipsw_get(void) 
{
	char	dpsw[4];
	
	if ( dpsw_d[0] == 0 )	dpsw[0] = 0x1;
	else			dpsw[0] = 0x0;
	
	if ( dpsw_d[1] == 0 )	dpsw[1] = 0x2;
	else			dpsw[1] = 0x0;
	
	if ( dpsw_d[2] == 0 )	dpsw[2] = 0x4;
	else			dpsw[2] = 0x0;
	
	if ( dpsw_d[3] == 0 )	dpsw[3] = 0x8;
	else			dpsw[3] = 0x0;

	return ( dpsw[0] + dpsw[1] + dpsw[2] + dpsw[3] );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 tasw_get								//
// 処理概要     タクトスイッチ値を16進数で取得				//
// 引数         なし									//
// 戻り値       スイッチ値 0～7							//
///////////////////////////////////////////////////////////////////////////
unsigned char tasw_get(void) 
{
	char	tasw[4];
	
	if ( tasw_d[0] == 0 )	tasw[0] = 0x1;
	else			tasw[0] = 0x0;
	
	if ( tasw_d[1] == 0 )	tasw[1] = 0x2;	
	else			tasw[1] = 0x0;
	
	if ( tasw_d[2] == 0 )	tasw[2] = 0x4;	
	else			tasw[2] = 0x0;
	
	if ( tasw_d[3] == 0 )	tasw[3] = 0x8;	
	else			tasw[3] = 0x0;

	return ( tasw[0] + tasw[1] + tasw[2] + tasw[3] );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getGyro								//
// 処理概要     ジャイロセンサのアナログ値で取得				//
// 引数         なし									//
// 戻り値       センサ値									//
///////////////////////////////////////////////////////////////////////////
short getGyro(void) 
{
	return ( gyro - 1796 );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getServoAngle							//
// 処理概要     ポテンションメーターのアナログ値で取得			//
// 引数         なし									//
// 戻り値       センサ値									//
///////////////////////////////////////////////////////////////////////////
short getServoAngle(void) 
{	
	return  ( Angle - Angle0 );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getAnalogSensor							//
// 処理概要     アナログセンサのアナログ値で取得				//
// 引数         なし									//
// 戻り値       センサ値									//
///////////////////////////////////////////////////////////////////////////
short getAnalogSensor(void) 
{
	return sensorR - sensorL;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 sensor_inp								//
// 処理概要     デジタルセンサの値を16進数で取得				//
// 引数         なし									//
// 戻り値       センサ値0～7								//
///////////////////////////////////////////////////////////////////////////
unsigned char sensor_inp(void) 
{
	char l, c, r;
	
	if (sensorRR < 3000 ) r = 0x1;
	else r = 0;
	if (sensorC < 3000 ) c = 0x2;
	else c = 0;
	if (sensorLL < 3000 ) l = 0x4;
	else l = 0;
	
	return l+c+r;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 startbar_get							//
// 処理概要     スタートゲートの開閉の確認					//
// 引数         なし									//
// 戻り値       0; 閉じている 1; 開いている					//
///////////////////////////////////////////////////////////////////////////
unsigned char startbar_get(void) 
{
	char ret;
	
	if ( sensorG <= 2900 )	ret = 1;
	else			ret = 0;
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 init_BeepS								//
// 処理概要     ブザー関連初期化							//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void init_BeepS( void )
{
    BeepPattern = 0x0000;
    BeepTimer   = 0;
    BeepMode    = 0;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 setBeepPatternS							//
// 処理概要     ブザー出力パターンセット						//
// 引数         Beep:鳴動パターン							//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void setBeepPatternS( unsigned short Beep )
{
    BeepPattern = Beep;
    BeepTimer   = 0;
    BeepMode    = 1;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 beepProcessS							//
// 処理概要     ブザー処理								//
// 引数         なし									//
// 戻り値       なし									//
// メモ		この関数を1ms毎に実行してください			//
///////////////////////////////////////////////////////////////////////////
void beepProcessS( void )
{
	if ( BeepMode ) {
		if ( BeepTimer % 5 == 0 ) {
			if ( BeepPattern & 0x8000 ) {
				BEEP_ON
			} else {
				BEEP_OFF
			}
			BeepPattern <<= 1;
		}
		BeepTimer++;
		if ( BeepTimer == 17 * 5 ) {
			BeepMode = 0;
			BEEP_OFF
		}
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motor_f								//
// 処理概要     モーターのPWMの変更						//
// 引数         accelefL, accelefR(PWMを1～100%で指定)			//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void motor_f( signed char accelefL, signed char accelefR )
{
	uint16_t pwmfl, pwmfr;
	
	if ( pushcart_mode ) {
		accelefR = 0;
		accelefL = 0;
	}
	
	accele_fR = accelefR;
	accele_fL = accelefL;
	
	pwmfl = TGR_MOTOR * accelefL / 100;
	pwmfr = TGR_MOTOR * accelefR / 100;
	
	// 左前輪
	if( accelefL >= 0) {					
		// 正転
		DIR_FL_FOR
		PWM_FL_OUT
	} else if ( accelefL == 100 || accelefL == -100 ) {	
		// 100%
		if ( accelefL > 0 ) {
			DIR_FL_FOR
		} else {
			DIR_FL_REV
		}
		pwmfl = TGR_MOTOR + 2;
		PWM_FL_OUT
	} else {						
		// 逆転
		pwmfl = -pwmfl;
		DIR_FL_REV
		PWM_FL_OUT
	}
	
	// 右前輪
	if( accelefR >= 0) {					
		// 正転
		DIR_FR_FOR
		PWM_FR_OUT
	} else if ( accelefR == 100 || accelefR == -100 ) {	
		// 100%
		if ( accelefR > 0 ) {
			DIR_FR_FOR
		} else {
			DIR_FR_REV
		}
		pwmfr = TGR_MOTOR + 2;
		PWM_FR_OUT
	} else {						
		// 逆転
		pwmfr = -pwmfr;
		DIR_FR_REV
		PWM_FR_OUT
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motor_r								//
// 処理概要     モーターのPWMの変更						//
// 引数         acelerL, accelerR(PWMを1～100%で指定)			//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void motor_r( signed char accelerL, signed char accelerR )
{
	uint16_t pwmrl, pwmrr;
	
	if ( pushcart_mode ) {
		accelerL = 0;
		accelerR = 0;
	}
	
	accele_rR = accelerR;
	accele_rL = accelerL;
	
	pwmrl = TGR_MOTOR * accelerL / 100;
	pwmrr = TGR_MOTOR * accelerR / 100;
	
	// 左後輪
	if( accelerL >= 0 ) {					
		// 正転
		DIR_RL_FOR
		PWM_RL_OUT
	} else if ( accelerL == 100 || accelerL == -100 ) {	
		// 100%
		if (accelerL > 0) {
			DIR_RL_FOR
		} else {
			DIR_RL_REV
		}
		pwmrl = TGR_MOTOR + 2;
		PWM_RL_OUT
	} else {						
		// 逆転
		pwmrl = -pwmrl;
		DIR_RL_REV
		PWM_RL_OUT
	}
	
	// 右後輪
	if( accelerR >= 0 ) {					
		// 正転
		DIR_RR_FOR
		PWM_RR_OUT
	} else if ( accelerR == 100 || accelerR == -100 ) {	
		// 100%
		if ( accelerR > 0 ) {
			DIR_RR_FOR
		} else {
			DIR_RR_REV
		}
		pwmrr = TGR_MOTOR + 2;
		PWM_RR_OUT
	} else {						
		// 逆転
		pwmrr = -pwmrr;
		DIR_RR_REV
		PWM_RR_OUT
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 servoPwmOut							//
// 処理概要     白線トレース時サーボのPWMの変更				//
// 引数         spwm									//
// 戻り値       なし									//
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
		PWM_SERVO_OUT
	} else {				
		// 逆転
		pwm = -pwm;
		DIR_SERVO_REV
		PWM_SERVO_OUT
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 short_sort								//
// 処理概要     short型変数の比較をする						//
// 引数         比較する配列								//
// 戻り値       -1:a<b 0:a=b 1:a>b						//
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