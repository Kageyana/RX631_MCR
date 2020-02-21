///////////////////////////////////////////////////////////////////////////
//												//
//  FILE	:Klic_trace.c								//
//  DATE	:Thr, Dec 21, 2017							//
//  DESCRIPTION :走行プログラム							//
//  CPU TYPE	:RX631								//
//												//
///////////////////////////////////////////////////////////////////////////
//====================================//
// インクルード									//
//====================================//
#include "ADC.h"
#include "IO.h"
#include "MOTOR.h"
#include "Rotaryencoder.h"
#include "Timer.h"
#include "Control.h"
#include "SetUp.h"
#include "SCI.h"
#include "I2C_LCD.h"
//====================================//
// グローバル変数の宣言								//
//====================================//
// 走行パターン関連
char		pattern = 0;	// パターン番号
char		countdown = 0x0;
short 	angle_center;

//====================================//
// プロトタイプ宣言									//
//====================================//
void initStatus ( bool lcd );
//====================================//
// メインプログラム									//
//====================================//
void main(void){
	//=================================//
	// 初期化									//
	//=================================//
	L_Sen_ON;		//センサ点灯
	inti_lcd();			// LCD初期化
	lcd_mode = 1;
	
	motor_f( 0, 0 );		// モーター停止
	motor_r( 0, 0 );
	servoPwmOut( 0 );
	
	pushcart_mode = 0;	// 手押しモードoff
	angle_mode = 0;	// 白線トレース
	start = 0;			// ゲートスタート
	
	initParameter();		// 各パラメータ初期化
	
	led_out(0);
	
	while(1){
	__setpsw_i();	// 多重割り込み許可
	switch( pattern ) {
		//-------------------------------------------------------------------
		// 【000】スタート処理
		//-------------------------------------------------------------------
		case 0:
			// スタート前設定
			setup();
			if ( start && !pushcart_mode ) {
				cnt1 = 0;
				pattern = 1;
				break;
			} else if ( start && pushcart_mode ) {
				// 手押しモードの場合すぐに通常トレース
				// 変数初期化
				initStatus( 1 );
				pattern = 11;		// 通常走行
				break;
			}
			break;
			
		case 1:
			servoPwmOut( ServoPwm );
			// スタートバー開閉待ち
			if ( !startbar_get() ) {
				// 変数初期化
				initStatus( 0 );
				break;
			}
			// LED点滅処理
			if ( cnt1 >= 2000 ) cnt1 = 0;
			if ( cnt1 < 1000 ) {
				led_out( LED_R );
			} else {
				led_out( LED_B  );
			}
			break;
		//-------------------------------------------------------------------
		// 【010】トレース処理
		//-------------------------------------------------------------------
		case 11:
			servoPwmOut( ServoPwm );		// サーボ制御
			targetSpeed = speed_straight * SPEED_CURRENT;	// 目標速度
			motor_f(motorPwm, motorPwm);	// 駆動モータ制御
			motor_r(motorPwm, motorPwm);			
			led_out( 0x00 );					// LED消灯
			
			// クロスラインチェック
			if ( check_crossline() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// 右ハーフラインチェック
	   		if ( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if ( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【020】クランク検出処理
		//-------------------------------------------------------------------
		case 21:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_crossline* SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			led_out( LED_G );
			
			if( enc1 > enc_mm( 90 ) ) {		// 90mm進む
				enc1 = 0;
				pattern = 22;
				break;
			}
			break;
			
		case 22:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_ckank_trace * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			// 右クランクチェック
			if( sensor_inp() ==  0x3 ) {
				enc1 = 0;
				led_out( LED_R | LED_G);
				SetAngle = angle_rightclank;
				angle_mode = 1;
				pattern = 31;
				break;
			}
			// 左クランクチェック
			if( sensor_inp() ==  0x6 ) {
				enc1 = 0;
				led_out( LED_G | LED_B );
				SetAngle = angle_leftclank;
				angle_mode = 1;
				pattern = 41;
				break;
			}
			
	        break;
		//-------------------------------------------------------------------
		//【030】右クランク処理
		//-------------------------------------------------------------------
		case 31:
			SetAngle = angle_rightclank;		// 目標角度
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if ( sensor_inp() == 0x2 ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 32;
				break;
			}
			break;
			
		case 32:
			// 復帰
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);

			if( enc1 >= enc_mm( 600 ) ) {		// 安定するまで待つ(600mm)
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【040】左クランク処理
		//-------------------------------------------------------------------
		case 41:
			SetAngle = angle_leftclank;		// 目標角度
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if ( sensor_inp() == 0x2 ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 42;
				break;
			}
			break;
			
		case 42:
			// 復帰
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);

			if( enc1 >= enc_mm( 600 ) ) {		// 安定するまで待つ(600mm)
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【050】右レーンチェンジ処理
		//-------------------------------------------------------------------
		case 51:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_halfine * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 52;
				break;
			}
			if( check_crossline() ) {		// クロスラインチェック
				enc1 = 0;
				pattern = 21;
				break;
			}
			break;
			
		case 52:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightchange_trace * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			led_out( LED_R );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				angle_mode = 1;
				pattern = 53;
				break;
			}
			break;
			
		case 53:
			SetAngle = angle_rightchange;
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( sensor_inp() == 0x1 ) {
				enc1 = 0;
				pattern = 54;
				break;
			}
			break;
			
		case 54:
			SetAngle = 0;
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );
			
			if( sensor_inp() == 0x2 && getAnalogSensor() < 1500 && getAnalogSensor() > -1500 ) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// 積分リセット
				pattern = 57;
				break;
			}
			break;
			
		case 55:
			servoPwmOut( 90 );
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( sensor_inp() == 0x2 ) {
				angle_center = getServoAngle();
				enc1 = 0;
				pattern = 56;
				break;
			}
			break;
			
		case 56:
			SetAngle = angle_center;
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_rightchange_escape * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( enc1 >= enc_mm( 10 ) ) {
				angle_center = getServoAngle();
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// 積分リセット
				pattern = 57;
				break;
			}
			break;
			
		case 57:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightchange_escape * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);

			if( enc1 >= enc_mm( 600 ) ) {
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【060】左レーンチェンジ処理
		//-------------------------------------------------------------------
		case 61:
			servoPwmOut( 0 );
			targetSpeed = speed_halfine * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 62;
				break;
			}
			if( check_crossline() ) {		// クロスラインチェック
				enc1 = 0;
				pattern = 21;
				break;
			}
			break;
			
		case 62:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftchange_trace * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			led_out( LED_B );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				angle_mode = 1;
				pattern = 63;
				break;
			}
			break;
			
		case 63:
			SetAngle = angle_leftchange;
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( sensor_inp() == 0x4 ) {
				enc1 = 0;
				pattern = 64;
				break;
			}
			break;
			
		case 64:
			SetAngle = 0;
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );

			if( sensor_inp() == 0x2 && getAnalogSensor() < 1500 && getAnalogSensor() > -1500 ) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// 積分リセット
				pattern = 67;
				break;
			}
			break;
			
		case 65:
			servoPwmOut( -90 );
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( sensor_inp() == 0x2 ) {
				angle_center = getServoAngle();
				enc1 = 0;
				pattern = 66;
				break;
			}
			break;
			
		case 66:
			SetAngle = angle_center;
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_leftchange_escape * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( enc1 >= enc_mm( 10 ) ) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// 積分リセット
				pattern = 67;
				break;
			}
			break;
			
		case 67:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftchange_escape * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);

			if( enc1 >= enc_mm( 600 ) ) {
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【100】停止処理
		//-------------------------------------------------------------------
		case 101:
			enc1 = 0;	
			
			LEDR_OFF;
			LEDG_OFF;
			LEDB_OFF;
	
			pattern = 102;
			break;
			
		case 102:
			servoPwmOut( ServoPwm );
			targetSpeed = 0;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );
			
			if( Encoder <= 0 && Encoder >= -1 ) {
				enc1 = 0;
				pattern = 103;
				break;
			}
			break;
			
		case 103:
			servoPwmOut( ServoPwm );
			motor_f( 0, 0 );
			motor_r( 0, 0 );
			
			pattern = 106;
			break;
			
		case 106:
			servoPwmOut( 0 );
			// LED点滅処理
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				LEDB_ON;
			}else{
				LEDB_OFF;
			}
			break;
			
		default:
			pattern = 101;
			break;
			
	} // end of "switch ( pattern )"
	} // end of "while ( 1 )"
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 init_Parameter							//
// 処理概要     変数の初期化								//
// 引数         lcd: 1 lcd表示  0 lcd非表示						//
// 戻り値       なし										//
///////////////////////////////////////////////////////////////////////////
void initStatus ( bool lcd ) {
	EncoderTotal = 10;	// 総走行距離
	cnt1 = 0;			// タイマリセット
	enc1 = 0;			// 区間距離リセット
	lcd_mode = lcd;		// LCD表示OFF
	angle_mode = 0;	// 白線トレース
	targetSpeed = speed_straight * SPEED_CURRENT; // 目標速度設定
}