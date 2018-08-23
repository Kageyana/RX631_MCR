//////////////////////////////////////////////////////////////////////////
//									//
//  FILE	:Klic_trace.c						//
//  DATE	:Thr, Dec 21, 2017					//
//  DESCRIPTION :走行プログラム						//
//  CPU TYPE	:RX631							//
//									//
//////////////////////////////////////////////////////////////////////////
//======================================//
// インクルード                         //
//======================================//
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "SetUp.h"
#include "SCI.h"
#include "E2dataFlash.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "I2C_MPU-9255.h"
#include <stdio.h>

//======================================//
// グローバル変数の宣言                 //
//======================================//
// 走行パターン関連
char 	pattern = 0;	// パターン番号
char 	countdown = 4;
short 	angle_center;

// モード関連
char	curve_moed;	// カーブ判定	0:カーブ以外	1:カーブ走行中
char	error_mode;	// 0:距離停止 1:センサ全灯 2:センサ全消灯 3:エンコーダ停止 4:ジャイロ反応

// タイマ関連
// 1msタイマ
unsigned int 		cnt1;		// 走行用タイマカウント
unsigned short	 	cnt_out;	// コースアウト判定用タイマ
unsigned short	 	cnt_out2;	// コースアウト判定用タイマ2
unsigned short	 	cnt_out3;	// コースアウト判定用タイマ3
unsigned short	 	cnt_out4;	// コースアウト判定用タイマ4
static char		Timer10;	// 1msカウント用

//======================================//
// メインプログラム	                //
//======================================//
void main(void){
	short i, j, rpwm, lpwm;
	unsigned int ui;
	
	//======================================//
	// 初期化		                //
	//======================================//
	inti_lcd();			// LCD初期化
	lcdPosition( 0, 0 );
	lcdPrintf("INITIALI");
	lcdPosition( 0, 1 );
	lcdPrintf("    ZING");
	
	motor_f( 0, 0 );		// モーター停止
	motor_r( 0, 0 );
	servoPwmOut( 0 );
	
	pushcart_mode = 0;		// 手押しモードoff
	slope_mode = 0;			// 上り坂チェック
	angle_mode = 0;			// 白線トレース
	start = 0;			// ゲートスタート
	
	init_BeepS();			// ブザー初期化
	
	// SCI1初期化
	if( tasw_get() == 0x2 ) {
		init_SCI1(RATE_230400);
		IMUSet = 0;
	} else {
		R_PG_SCI_Set_C1();
		init_IMU();
		IMUSet = 1;
	}
	
	// フラッシュ初期化
	if( initFlash() == 0 ) {
		setBeepPatternS( 0x8000 );
		readFlashSetup();	// データフラッシュから前回パラメータを読み込む
	} else{
		setBeepPatternS( 0xaa00 );
	}
	
	// MicroSDカード初期化
	if( init_msd() == 0 ) {
		setBeepPatternS( 0x4000 );
		msdset = 1;
	} else {
		setBeepPatternS( 0xaa00 );
		msdset = 0;
	}

	while(1){
		if( pattern >= 11 && pattern <= 99 ) {
			if( !pushcart_mode ) {		
				// 手押しモードOFF
				if( cnt1 >= 500 ) {		// 動き出してから
					if( EncoderTotal >= ( PALSE_METER * stopping_meter ) ) { // 距離超過の場合
						error_mode = 0;
						pattern = 101;
					} else if( cnt_out >= STOP_SENSOR1 ) {	// センサ全灯
						error_mode = 1;
						pattern = 101;
					} else if( cnt_out2 >= STOP_SENSOR2 ) {	// センサ全消灯
						error_mode = 2;
						pattern = 101;
					} else if( cnt_out3 >= STOP_ENCODER ) {	// エンコーダ停止(ひっくり返った？)
						error_mode = 3;
						//pattern = 101;
					} else if( cnt_out4 >= STOP_GYRO ) {	// マイナスの加速度検知(コースから落ちた？)
						error_mode = 4;	
						pattern = 101;
					} else if ( stopWord == 1 ) {
						error_mode = 5;
						pattern = 101;
					}
					
					if ( stopWord == 1 ) {
						error_mode = 5;
						pattern = 101;
					}
					/*
					if( cnt1 >= STOP_COUNT ) {
						pattern = 101;
					}
					*/
				}
			} else {			
				// 手押しモードON
				lcdPosition( 0, 0 );
				lcdPrintf("case %3d", pattern);
				lcdPosition( 0, 1 );
				lcdPrintf("%4d", (short)TurningAngleIMU);
			}
		} else if( pattern >= 100 ) {
			lcd_mode = 1;
			lcdPosition( 0, 0 );
			lcdPrintf("TIME  %d", error_mode);
			lcdPosition( 0, 1 );
			lcdPrintf(" %5dms", ui);
		}
		
	switch( pattern ) {
		//-------------------------------------------------------------------
		// 【000】スタート処理
		//-------------------------------------------------------------------
		case 0:
			// スタート前設定
			setup();
			if( start >= 1 && !pushcart_mode ) {
				demo = 0;		// デモモード解除
				angle_mode = 0;		// 白線トレース
				txt= txt_data;		// 受信配列リセット
				cnt_byte = 0;		// 受信カウントリセット
				if( msdset == 1 ) init_log();	// ログ記録準備
				if( fixSpeed == 0 ) writeFlashBeforeStart();	// 速度パラメータをデータフラッシュに保存
				
				// 白線トレース用PIDゲイン保存
				flashDataBuff[ 0 ] = kp_buff;
				flashDataBuff[ 1 ] = ki_buff;
				flashDataBuff[ 2 ] = kd_buff;
				writeFlashData( PID_STARTAREA, PID_ENDAREA, PID_DATA, 3 );
				// 角度制御用PIDゲイン保存
				flashDataBuff[ 0 ] = kp2_buff;
				flashDataBuff[ 1 ] = ki2_buff;
				flashDataBuff[ 2 ] = kd2_buff;
				writeFlashData( PID2_STARTAREA, PID2_ENDAREA, PID2_DATA, 3 );
				// 速度制御用PIDゲイン保存
				flashDataBuff[ 0 ] = kp3_buff;
				flashDataBuff[ 1 ] = ki3_buff;
				flashDataBuff[ 2 ] = kd3_buff;
				writeFlashData( PID3_STARTAREA, PID3_ENDAREA, PID3_DATA, 3 );
				// 停止距離保存
				flashDataBuff[ 0 ] = stopping_meter;
				writeFlashData( STOPMETER_STARTAREA, STOPMETER_ENDAREA, STOPMETER_DATA, 1 );
				
				targetSpeed = speed_straight * SPEED_CURRENT; // 目標速度設定
				cnt1 = 0;		// タイマリセット
				pattern = 1;
				break;
			} else if ( start >= 1 && pushcart_mode ) {
				// 手押しモードの場合すぐに通常トレース
				// 白線トレース用PIDゲイン保存
				flashDataBuff[ 0 ] = kp_buff;
				flashDataBuff[ 1 ] = ki_buff;
				flashDataBuff[ 2 ] = kd_buff;
				writeFlashData( PID_STARTAREA, PID_ENDAREA, PID_DATA, 3 );
				// 角度制御用PIDゲイン保存
				flashDataBuff[ 0 ] = kp2_buff;
				flashDataBuff[ 1 ] = ki2_buff;
				flashDataBuff[ 2 ] = kd2_buff;
				writeFlashData( PID2_STARTAREA, PID2_ENDAREA, PID2_DATA, 3 );
				
				lcdPosition( 0, 1 );
				lcdPrintf("        ");
				setBeepPatternS( 0xfff0 );
				
				EncoderTotal = 10;		// 総走行距離
				cnt1 = 0;			// タイマリセット
				lcd_mode = 1;			// LCD表示ON
				pattern = 11;
				break;
			}
			break;
			
		case 1:
			servoPwmOut( ServoPwm );
			if( start == 1 ) {
				// カウントダウンスタート
				if( cnt1 >= 1500 && countdown == 4 ) {
					countdown = 3;
					lcdPosition( 0, 1 );
					lcdPrintf("       3");
					setBeepPatternS( 0x8000 );
					led_out( 0x10 );
				}
				if( cnt1 >= 2500 && countdown == 3 ) {
					countdown = 2;
					lcdPosition( 0, 1 );
					lcdPrintf("       2");
					setBeepPatternS( 0x8000 );
					led_out( 0x04 );
				}
				if( cnt1 >= 3500 && countdown == 2 ) {
					countdown = 1;
					lcdPosition( 0, 1 );
					lcdPrintf("       1");
					setBeepPatternS( 0x8000 );
					led_out( 0x01 );
				}
				if( cnt1 >= 4500 && countdown == 1 ) {
					countdown = 0;
					lcdPosition( 0, 1 );
					lcdPrintf("        ");
					setBeepPatternS( 0xfff0 );
						
					EncoderTotal = 10;	// 総走行距離
					cnt1 = 0;		// タイマリセット
					lcd_mode = 0;		// LCD表示OFF
					msdFlag = 1;		// データ記録開始
					pattern = 11;
					break;
				}
			} else if ( start == 2 ) {
				// スタートゲート開放スタート
				pattern = 2;
				break;
			}
			break;
			
		case 2:
			servoPwmOut( ServoPwm );
			// スタートバー開閉待ち
			if( !startbar_get() ) {
				EncoderTotal = 10;		// 総走行距離
				cnt1 = 0;			// タイマリセット
				lcd_mode = 1;			// LCD表示OFF
				msdFlag = 1;                    // データ記録開始
				pattern = 11;
				break;
			}
			// LED点滅処理
			if( cnt1 >= 2000 ) cnt1 = 0;
			if( cnt1 < 1000 ) {
				led_out( 0x04 );
			}else{
				led_out( 0x08 );
			}
			break;
		//-------------------------------------------------------------------
		// 【010】トレース処理
		//-------------------------------------------------------------------
		case 11:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_straight * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			led_out( 0x00 );
			
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// 右ハーフラインチェック
	   		if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// カーブチェック
			if( i >=  CURVE_R600_START || i <= -CURVE_R600_START ) {
				enc1 = 0;
				curve_moed = 1;
				pattern = 12;
				break;
			}
			break;
			
		case 12:
			// カーブブレーキ
			servoPwmOut( ServoPwm );
			targetSpeed = speed_curve_brake * SPEED_CURRENT;
			led_out( 0x1e );
			diff( motorPwm );
			
			if( enc1 > enc_mm( 60 ) ) {		// 60mm進む
				enc1 = 0;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 13;
				break;
			}
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// 右ハーフラインチェック
	   		if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// カーブ継ぎ目チェック
			if( i <  CURVE_R600_START && i > -CURVE_R600_START ) {
				enc1 = 0;
				pattern = 11;
				break;
			}
			break;
			
		case 13:
			// R600カーブ走行
			servoPwmOut( ServoPwm );
			targetSpeed = speed_curve_r600 * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
	   		if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// R450チェック
			if( i >= CURVE_R450_START || i <= -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 14;
				break;
			}
			// カーブ継ぎ目チェック
			if( i <  CURVE_R600_START && i > -CURVE_R600_START ) {
				if ( enc1 >= enc_mm(50) ) {
					enc1 = 0;
					pattern = 15;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
			}
			break;
			
		case 14:
			// R450カーブ走行
			servoPwmOut( ServoPwm );
			targetSpeed = speed_curve_r450 * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
	   		if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// R600チェック
			if( i < CURVE_R450_START && i > -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 13;
				break;
			}
			break;
		
		case 15:
			// カーブ継ぎ目走行
			servoPwmOut( ServoPwm );
			targetSpeed = speed_curve_straight * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			if( enc1 >= enc_mm( 300 ) ) {		// 300mm進む
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				curve_moed = 0;
				pattern = 11;
				break;
			}
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// カーブチェック
			if( i >=  CURVE_R600_START || i <= - CURVE_R600_START ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 13;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【020】クランク検出処理
		//-------------------------------------------------------------------
		case 21:
			servoPwmOut( 0 );
			targetSpeed = speed_crossline* SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x03 );
			
			if( enc1 > enc_mm( 90 ) ) {		// 60mm進む
				enc1 = 0;
				pattern = 22;
				break;
			}
			break;
			
		case 22:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_ckank_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x06 );
			
			// 右クランクチェック
			if( sensor_inp() ==  0x3 ) {
				enc1 = 0;
				led_out( 0x01 );
				setBeepPatternS( 0xa000 );
				SetAngle = angle_rightclank;
				angle_mode = 1;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 31;
				break;
			}
			// 左クランクチェック
			if( sensor_inp() ==  0x6 ) {
				enc1 = 0;
				led_out( 0x02 );
				setBeepPatternS( 0xa800 );
				SetAngle = angle_leftclank;
				angle_mode = 1;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 41;
				break;
			}
			
	        break;
		//-------------------------------------------------------------------
		//【030】右クランク処理
		//-------------------------------------------------------------------
		case 31:
			SetAngle = angle_rightclank;
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			i = (Encoder * 10) - targetSpeed;	// 目標値との偏差
			j = getAnalogSensor();
			diff( motorPwm );
			/*if( i >= 200 && enc1 <= enc_mm( 40 ) ) {
				// 速度超過なら急ブレーキ
				motor_f( -100, 0 );
				motor_r( -100, 0 );
			} else {
				// 左を加速、右を減速のみに使用する
				if( motorPwm > 0 ) rpwm = 0;		// 加速時　右0
				else rpwm = motorPwm;			// 減速時　右減速
				if( motorPwm > 0 ) lpwm = motorPwm;	// 加速時　左加速
				else lpwm = 0;				// 減速時　左0
				motor_f( lpwm, rpwm);
				motor_r( lpwm, rpwm);	
			}*/
			if( -TurningAngleIMU <= 30 ) {
				if( sensor_inp() == 0x2 ) {
					enc1 = 0;
					angle_mode = 0;
					pattern = 36;
					break;
				}
			} else if ( -TurningAngleIMU >= 20 ) {
				if( j <= -1800 ) {
					enc1 = 0;
					i = (short)-TurningAngleIMU;
					pattern = 34;
					break;
				}
			}
			if ( sensor_inp() == 0x4 ) {
				enc1 = 0;
				pattern = 32;
				break;
			}
			break;
			
		case 32:
			// 外線読み飛ばし
			SetAngle = angle_rightclank;
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			// 左を減速、右を加速のみに使用する
			/*
			if( motorPwm > 0 ) rpwm = 0;		// 加速時　右0
			else rpwm = motorPwm;			// 減速時　右減速
			if( motorPwm > 0 ) lpwm = motorPwm;	// 加速時　左加速
			else lpwm = 0;				// 減速時　左0
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			*/
			
			//if( -TurningAngleIMU <= 90 && -TurningAngleIMU >= 40) {
				if( j <= -1800 ) {
					enc1 = 0;
					i = (short)TurningAngleIMU;
					i = -i;
					pattern = 34;
					break;
				}
			//}
			break;
			
		case 34:
			// 角度維持
			// sensor_inp() == 2を読んだあとに実行
			SetAngle = -( 90 - 10 - i ) * (435/35);	// ラインからの角度10°
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			// 左を減速、右を加速のみに使用する
			/*
			if( motorPwm > 0 ) rpwm = 0;		// 加速時　右0
			else rpwm = motorPwm;			// 減速時　右減速
			if( motorPwm > 0 ) lpwm = motorPwm;	// 加速時　左加速
			else lpwm = 0;				// 減速時　左0
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			led_out(0x18);
			*/
			
			if( sensor_inp() == 0x2 && j >= -1800) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 36;
				break;
			}
			break;
			
		case 36:
			// 復帰
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
			diff( motorPwm );
			led_out(0x06);
			
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
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
			SetAngle = angle_leftclank;
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			i = (Encoder * 10) - targetSpeed;	// 目標値との偏差
			j = getAnalogSensor();
			diff( motorPwm );
			/*if( i >= 200 && enc1 <= enc_mm( 40 ) ) {
				// 速度超過なら急ブレーキ
				motor_f( -100, 0 );
				motor_r( -100, 0 );
			} else {
				// 左を加速、右を減速のみに使用する
				if( motorPwm < 0 ) rpwm = 0;
				else rpwm = motorPwm;
				if( motorPwm > 0 ) lpwm = 0;
				else lpwm = motorPwm;
				motor_f( lpwm, rpwm);
				motor_r( lpwm, rpwm);	
			}*/
			if( TurningAngleIMU <= 30 ) {
				if( sensor_inp() == 0x2 ) {
					enc1 = 0;
					angle_mode = 0;
					pattern = 46;
					break;
				}
			} else if ( TurningAngleIMU >= 20 ) {
				if( j >= 1800 ) {
					enc1 = 0;
					i = TurningAngleIMU;
					pattern = 44;
					break;
				}
			}
			if ( sensor_inp() == 0x1 ) {
				enc1 = 0;
				pattern = 42;
				break;
			}
			break;
			
		case 42:
			// 外線読み飛ばし
			SetAngle = angle_leftclank;
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			// 左を減速、右を加速のみに使用する
			/*
			if( motorPwm < 0 ) rpwm = 0;
			else rpwm = motorPwm;
			if( motorPwm > 0 ) lpwm = 0;
			else lpwm = motorPwm;
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			*/
			
			//if( TurningAngleIMU <= 90 && TurningAngleIMU >= 40) {
				if( j >= 1800 ) {
					enc1 = 0;
					i = TurningAngleIMU;
					pattern = 44;
					break;
				}
			//}
			break;
			
		case 44:
			// 角度維持
			// sensor_inp() == 2を読んだあとに実行
			SetAngle = ( 90 - 10 - i ) * (435/35);	// ラインからの角度10°
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			// 左を減速、右を加速のみに使用する
			/*
			if( motorPwm < 0 ) rpwm = 0;
			else rpwm = motorPwm;
			if( motorPwm > 0 ) lpwm = 0;
			else lpwm = motorPwm;
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			led_out(0x18);
			*/
			
			if( sensor_inp() == 0x2 && j <= 1800) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 46;
				break;
			}
			break;
			
		case 46:
			// 復帰
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			diff( motorPwm );
			led_out(0x06);
			
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
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
			servoPwmOut( 0 );
			targetSpeed = speed_halfine * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				angle_mode = 0;
				setBeepPatternS( 0xe000 );
				pattern = 52;
				break;
			}
			/*if( enc1 <= 20 * PALSE_MILLIMETER ) {
				if( sensor_inp() == 0x2 ) {
					enc1 = 0;
					pattern = 11;
					break;
				}
			}*/
			if( check_crossline() ) {		// クロスラインチェック
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
			}
			break;
			
		case 52:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightchange_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x04 );
			
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
			diff( motorPwm );
			
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
				pattern = 57;
				break;
			}
			break;
			
		case 55:
			servoPwmOut( 90 );
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			diff( motorPwm );
			
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
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 10 ) ) {
				angle_center = getServoAngle();
				enc1 = 0;
				angle_mode = 0;
				pattern = 57;
				break;
			}
			break;
			
		case 57:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
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
			diff( motorPwm );
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				setBeepPatternS( 0xe400 );
				angle_mode = 0;
				pattern = 62;
				break;
			}
			/*if( enc1 <= 20 * PALSE_MILLIMETER ) {
				if( sensor_inp() == 0x2 ) {
					enc1 = 0;
					pattern = 11;
					break;
				}
			}*/
			if( check_crossline() ) {		// クロスラインチェック
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
			}
			break;
			
		case 62:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftchange_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x08 );
			
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
			diff( motorPwm );
			
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
				pattern = 67;
				break;
			}
			break;
			
		case 65:
			servoPwmOut( -90 );
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			diff( motorPwm );
			
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
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 10 ) ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 67;
				break;
			}
			break;
			
		case 67:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			if( enc1 >= enc_mm( 600 ) ) {
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【070】坂道処理
		//-------------------------------------------------------------------
		case 71:
			// 誤検知判断
			servoPwmOut( ServoPwm );
			//targetSpeed = ( speed_slope_trace / 10 ) * SPEED_CURRENT;
			diff( motorPwm );
			if( check_slope() == 1 ) {
				if( slope_mode == 0 ) {
					// 上り始め
					enc1 = 0;
					led_out( 0x18 );
					setBeepPatternS( 0xe000 );
					pattern = 72;
					break;
				} else if ( slope_mode == 2 && enc_slope >= enc_mm( 600 ) ) {
					// 下り終わり
					enc1 = 0;
					led_out( 0x05 );
					setBeepPatternS( 0xe000 );
					slope_mode = 0;
					pattern = 74;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
			} else if ( check_slope() == -1 ) {
				if( slope_mode == 1 && enc_slope >= enc_mm( 1000 ) ) {
					// 上り終わり、下り始め
					enc1 = 0;
					led_out( 0x05 );
					setBeepPatternS( 0xe000 );
					slope_mode = 2;
					pattern = 75;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
				break;
			}else{
				enc1 = 0;
				pattern = 11;
				break;
			}
			break;
			
		case 72:
			// 坂頂点まで走行
			servoPwmOut( ServoPwm );
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 1200 ) ) {
				enc1 = 0;
				setBeepPatternS( 0xe000 );
				led_out( 0x03 );
				pattern = 73;
				break;
			}
			break;
			
		case 73:
			// 上り坂終点ブレーキ
			servoPwmOut( ServoPwm );
			motor_f( -100, -100 );
			motor_r( -100, -100 );
			
			if( enc1 >= enc_mm( 50 ) ) {
				enc1 = 0;
				led_out( 0x10 );
				pattern = 75;
				break;
			}
			break;
			
		case 74:
			// 下り坂終点ブレーキ
			servoPwmOut( ServoPwm );
			targetSpeed = speed_slope_brake * SPEED_CURRENT;
			diff( motorPwm );
			if( enc1 >= enc_mm( 40 ) ) {
				enc1 = 0;
				led_out( 0x0f );
				pattern = 75;
				break;
			}
			break;
			
		case 75:
			// ジャイロセンサが安定するまで読み飛ばし
			servoPwmOut( ServoPwm );
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 150 ) ) {
				enc1 = 0;
				pattern = 76;
				break;
			}
			break;
			
		case 76:
			// ジャイロセンサが安定するまで読み飛ばし
			servoPwmOut( ServoPwm );
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			diff( motorPwm );
			
			// クロスラインチェック
			if( check_crossline() ) {
				enc1 = 0;
				led_out( 0x03 );
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			if( slope_mode  == 0 ) {
				if( enc1 >= enc_mm( 1000 ) ) {
					enc1 = 0;
					enc_slope = 0;
					slope_mode = 1;
					pattern = 11;
					break;
				}
			}else{
				if( enc1 >= enc_mm( 400 ) ) {
					enc1 = 0;
					enc_slope = 0;
					pattern = 11;
					break;
				}
			}
			break;
		//-------------------------------------------------------------------
		//【100】停止処理
		//-------------------------------------------------------------------
		case 101:
			enc1 = 0;	
			ui = cnt1;	// 走行時間取得
			pattern = 102;
			break;
			
		case 102:
			servoPwmOut( ServoPwm );
			targetSpeed = 0;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );
			
			if( Encoder <= 1 && Encoder >= -1 ) {
				enc1 = 0;
				pattern = 103;
				break;
			}
			break;
			
		case 103:
			servoPwmOut( 0 );
			motor_f( 0, 0 );
			motor_r( 0, 0 );
			
			if( msdset == 1 ) {
				pattern = 104;
				break;
			}else{
				setBeepPatternS( 0xaa00 );
				pattern = 106;
				break;
			}
			break;
			
		case 104:
			// 最後のデータが書き込まれるまで待つ
			printf("case 104\n");
			if( checkMicroSDProcess() == 11 ) {
				msdFlag = 0;	// ログ記録終了
				printf("microSDProcessEndNOW\n");
				microSDProcessEnd();        // microSDProcess終了処理
				pattern = 105;
				break;
			}else if( checkMicroSDProcess() == 0 ) {
				setBeepPatternS( 0xf0f0 );
				pattern = 106;
				break;
			}
			break;
			
		case 105:
			// 終了処理が終わるまで待つ
			if( checkMicroSDProcess() == 0 ) {
				// MicroSD最終書き込みアドレス保存
				flashDataBuff[ 0 ] = msdStartAddress >> 16;
				flashDataBuff[ 1 ] = msdStartAddress & 0xffff;	// 開始アドレス
				flashDataBuff[ 2 ] = msdWorkAddress >> 16;
				flashDataBuff[ 3 ] = msdWorkAddress & 0xffff;	// 終了アドレス
				writeFlashData( MSD_STARTAREA, MSD_ENDAREA, MSD_DATA, 4 );
				printf("msdStartAddress = %d\n", msdStartAddress);
				printf("msdEndAddress = %d\n", msdWorkAddress);
				pattern = 106;
				setBeepPatternS( 0xa8a8 );
				break;
			}
			break;
			
		case 106:
			// LED点滅処理
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				led_out( 0x1f );
			}else{
				led_out( 0x00 );
			}
			break;
			
		default:
			pattern = 101;
			break;
			
	} // end of "switch ( pattern )"
	} // end of "while ( 1 )"
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 Timer							//
// 処理概要     1msごとにタイマ割り込み					//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void Timer (void) {
	short s;
	
	__setpsw_i();
	//　タイマカウント
	if ( pattern >= 11 && pattern <= 99 ) {	
		if ( pattern != 21 ) {				// クロスライン通過時は無視
			if ( sensor_inp() == 0x7 || sensor_inp() == 0x5 ) {	// センサ全灯
				cnt_out++;	
			} else {
				cnt_out = 0;
			}
		}
		if ( sensor_inp() == 0x0 && pattern != 53 && pattern != 63 ) cnt_out2++;	// センサ全消灯
		else cnt_out2 = 0;
		if ( Encoder == 0 ) cnt_out3++;		// エンコーダ停止(ひっくり返った？)
		else cnt_out3 = 0;
		s = (short)RollAngleIMU;
		if ( s >= 5 || s <= -5 ) cnt_out4++;
		else	cnt_out4 = 0;
	} else if ( pattern < 11 ) {
		cnt_setup++;
		cnt_setup2++;
		cnt_setup3++;
		cnt_swR++;
		cnt_swL++;
		cnt_flash++;
	}
	cnt1++;
	cnt_gyro++;
			
	// LCD表示
	if ( lcd_mode ) {
		lcdShowProcess();
	}

	// エンコーダカウント
	getEncoder();

	// PID制御値算出
	if ( angle_mode == 0 ) {
		servoControl();
	} else {
		servoControl2();
	}
	motorControl();
	
	// 角度計算
	getDegrees();
	getTurningAngleEnc();
	getTurningAngleIMU();
	getRollAngleIMU();
	if( cnt_gyro == INTEGRAL_LIMIT ) cnt_gyro = 0;

	// MicroSD書き込み
	microSDProcess();
	if( msdFlag == 1 ) {
		sendLog();
	}

	if ( IMUSet == 0 ) {
		// UART受信
		commandSCI1();
	} else {
		// 加速度及び角速度を取得
		IMUProcess();
	}
	
	Timer10++;
	// 10ｍごとに実行
	switch ( Timer10 ) {	
	case 1:
		// ブザー
		beepProcessS();
		break;
	case 2:
		// スイッチ読み込み
		getSwitch();
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