//====================================//
// インクルード
//====================================//
#include "io.h"
#include "mtu.h"
#include "ADconverter.h"

#include "control.h"
#include "setup.h"
#include "sci.h"
#include "E2dataFlash.h"
#include "AQM1602Y.h"
#include "microSD.h"
#include "ICM20648.h"
#include "MemorryTrace.h"
//====================================//
// グローバル変数の宣言
//====================================//
// 走行パターン関連
char		pattern = 0;	// パターン番号
char		countdown = 0x0;
short 		angle_center;

// モード関連
char		mode_curve;		// カーブ判定	0:カーブ以外	1:カーブ走行中
char		mode_error;		// 0:距離停止 1:センサ全灯 2:センサ全消灯 3:エンコーダ停止 4:ジャイロ反応
char 		mode_autoMotor;	// 0: switch文でサーボ、駆動モータのPWM出力を指定する 1: Timer関数内で自動的にPWM出力を実行

// タイマ関連
// 1msタイマ
unsigned int 		cnt1;		// 走行用タイマカウント
unsigned short	 	cnt_out;	// コースアウト判定用タイマ
unsigned short	 	cnt_out2;	// コースアウト判定用タイマ2
unsigned short	 	cnt_out3;	// コースアウト判定用タイマ3
unsigned short	 	cnt_out4;	// コースアウト判定用タイマ4
static char			Timer10;	// 1msカウント用
//====================================//
// プロトタイプ宣言
//====================================//
void init_Parameter ( bool lcd );
//====================================//
// メインプログラム
//====================================//
void main(void){
	short i, j;
	unsigned int ui;
	
	//=================================//
	// 初期化
	//=================================//
	L_Sen_ON;		//センサ点灯
	inti_lcd();		// LCD初期化
	
	motorPwmOut(0, 0, 0, 0);	// モーター停止
	servoPwmOut( 0 );
	
	mode_pushcart = 0;		// 手押しモードoff
	mode_slope = 0;			// 上り坂チェック
	mode_angle = 0;			// 白線トレース
	mode_autoMotor = 0;		// 自動PWM出力停止
	start = 0;				// ゲートスタート
	
	//IMU初期化
	IMU_init();
	// フラッシュ初期化
	// データフラッシュから前回パラメータを読み込む
	if( !initFlash() ) readFlashSetup( 1, 1, 1 ,1 ,1 ,1 ,1);
	// MicroSDカード初期化
	if( !init_msd() ) msdset = 1;
	else msdset = 0;

	// 電源電圧の確認
	if (Voltage < LOWVOLTAGE ) {
		lcdRowPrintf(UPROW, "LOW BAT ");
		lcdRowPrintf(LOWROW, "  %05.2fV",Voltage);
		led_out(LED_R);
		while(1);
	} else {
		cnt1=0;
		while( cnt1 < 1500){
			lcdRowPrintf(UPROW, "Voltage ");
			lcdRowPrintf(LOWROW, "  %05.2fV",Voltage);
		}
	}

	cnt1=0;
	while(msdset == 1 && cnt1 < 1500){
		lcdRowPrintf(UPROW, " SYSTEM ");
		lcdRowPrintf(LOWROW, "ALLGREEN");
		led_out(LED_G);
	}
	led_out(0);
	
	while(1){
		__setpsw_i();
		if( pattern >= 11 && pattern <= 99 ) {
			if( !mode_pushcart ) {		
				// 手押しモードOFF
				if( cnt1 >= 100 ) {		// 動き出してから
					if ( EncoderTotal >= ( PALSE_METER * stopping_meter ) ) { // 距離超過の場合
						mode_error = 0;
						pattern = 101;
						mode_autoMotor = 0;
					} else if ( cnt_out >= STOP_SENSOR1 ) {	// センサ全灯
						mode_error = 1;
						pattern = 101;
						mode_autoMotor = 0;
					} else if ( cnt_out2 >= STOP_SENSOR2 ) {	// センサ全消灯
						mode_error = 2;
						pattern = 101;
						mode_autoMotor = 0;
					} else if ( cnt_out3 >= STOP_ENCODER ) {	// エンコーダ停止(ひっくり返った？)
						mode_error = 3;
						pattern = 101;
						mode_autoMotor = 0;
					} else if( cnt_out4 >= STOP_GYRO ) {	// マイナスの加速度検知(コースから落ちた？)
						mode_error = 4;	
						pattern = 101;
						mode_autoMotor = 0;
					}
					/*
					// Buletoothで外部から停止
					if ( stopWord == 1 ) {
						mode_error = 5;
						pattern = 101;
					}
					*/
					/*
					// 一定時間で停止
					if( cnt1 >= STOP_COUNT ) {
						pattern = 101;
					}
					*/
				}
			} else {			
				// 手押しモードON
				lcdPosition( 0, 0 );
				lcdPrintf("now %3d", pattern);
				lcdPosition( 0, 1 );
				lcdPrintf("log %3d", logmeter());
			}
			// スイッチで停止
			if ( cnt1 >= 1000 && tasw_get() == SW_PUSH ) {
				mode_error = 6;
				pattern = 101;
			}
		} else if ( pattern >= 100 ) {
			mode_lcd = 1;
			lcdPosition( 0, 0 );
			lcdPrintf("TIME  %d", mode_error);
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
			if ( start && !mode_pushcart ) {
				demo = 0;		// デモモード解除
				mode_angle = 0;	// 白線トレース
				Int = 0;			// 積分リセット
				txt= txt_data;		// 受信配列リセット
				cnt_byte = 0;		// 受信カウントリセット
				
				if ( msdset ) init_log();	// ログ記録準備
				
				if ( !fixSpeed ) writeFlashBeforeStart(1, 0, 1, 1, 1, 1);	// 速度パラメータをデータフラッシュに保存
				else writeFlashBeforeStart(0, 0, 1, 1, 1, 1);		// 速度パラメータ以外を保存
				
				//if (IMUSet) caribrateIMU();
				
				wait_lcd(500);		// 500ms待つ
				cnt1 = 0;
				pattern = 1;
				break;
			} else if ( start && mode_pushcart ) {
				// 手押しモードの場合すぐに通常トレース
				if ( msdset ) init_log();	// ログ記録準備
				
				// 白線トレース用PIDゲイン保存
				// 角度制御用PIDゲイン保存
				writeFlashBeforeStart(0, 0, 1, 1, 0, 0);
				// 変数初期化
				init_Parameter( 1 );
				break;
			}
			break;
			
		case 1:
			servoPwmOut( ServoPwm );
			if ( start == START_COUNT ) {
				// カウントダウンスタート
				if ( cnt1 >= 3000 ) {	
					// 変数初期化
					init_Parameter( 0 );
					break;
				} else if ( !(cnt1 % 1000) ) {
					led_out( countdown );
					countdown = countdown << 1;
					break;
				}
			} else if ( start == START_GATE ) {
				// スタートゲート開放スタート
				pattern = 2;
				break;
			}
			break;
			
		case 2:
			servoPwmOut( ServoPwm );
			// スタートバー開閉待ち
			if ( !startbar_get() ) {
				// 変数初期化
				init_Parameter( 0 );
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
			targetSpeed = speed_straight * SPEED_CURRENT;
			i = getServoAngle();
			led_out( 0x00 );
			
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
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
			// カーブチェック
			if ( i >=  CURVE_R600_START || i <= -CURVE_R600_START ) {
				enc1 = 0;
				mode_curve = 1;
				pattern = 12;
				break;
			}
			break;
			
		case 12:
			// カーブブレーキ
			targetSpeed = speed_curve_brake * SPEED_CURRENT;
			led_out( LED_R );
			i = getServoAngle();
			
			if ( enc1 > enc_mm( 60 ) ) {		// 60mm進む
				enc1 = 0;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 13;
				break;
			}
			
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
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
			if ( memory_mode ) {
				enc1 = 0;
				pattern = 16;
				break;
			}
			// 直線チェック
			if ( i <  CURVE_R600_START && i > -CURVE_R600_START ) {
				enc1 = 0;
				pattern = 11;
				break;
			}
			break;
			
		case 13:
			// R600カーブ走行
			targetSpeed = speed_curve_r600 * SPEED_CURRENT;
			i = getServoAngle();
			
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
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
			// R450チェック
			if ( i >= CURVE_R450_START || i <= -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 14;
				break;
			}
			// カーブ継ぎ目チェック
			if ( i <  CURVE_R600_START && i > -CURVE_R600_START ) {
				enc1 = 0;
				pattern = 15;
				break;
			}
			break;
			
		case 14:
			// R450カーブ走行
			targetSpeed = speed_curve_r450 * SPEED_CURRENT;
			i = getServoAngle();
			
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
			// R600チェック
			if ( i < CURVE_R450_START && i > -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 13;
				break;
			}
			break;
		
		case 15:
			// カーブ継ぎ目走行
			targetSpeed = speed_curve_straight * SPEED_CURRENT;
			i = getServoAngle();
			
			if ( enc1 >= enc_mm( 300 ) ) {		// 300mm進む
				enc1 = 0;
				mode_curve = 0;
				pattern = 11;
				break;
			}
			
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
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
			// カーブチェック
			if( i >=  CURVE_R600_START || i <= - CURVE_R600_START ) {
				enc1 = 0;
				pattern = 13;
				break;
			}
			break;
			
		case 16:
			targetSpeed = speed_straight * SPEED_CURRENT;
			
			break;
		//-------------------------------------------------------------------
		//【020】クランク検出処理
		//-------------------------------------------------------------------
		case 21:
			targetSpeed = speed_crossline* SPEED_CURRENT;
			led_out( LED_G );
			
			if( enc1 > enc_mm( 90 ) ) {		// 60mm進む
				enc1 = 0;
				pattern = 22;
				break;
			}
			break;
			
		case 22:
			targetSpeed = speed_ckank_trace * SPEED_CURRENT;
			
			// 右クランクチェック
			if( sensor_inp() ==  0x3 ) {
				enc1 = 0;
				led_out( LED_R | LED_G);
				SetAngle = angle_rightclank;
				mode_angle = 1;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 31;
				break;
			}
			// 左クランクチェック
			if( sensor_inp() ==  0x6 ) {
				enc1 = 0;
				led_out( LED_G | LED_B );
				SetAngle = angle_leftclank;
				mode_angle = 1;
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
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			i = -TurningAngleIMU;
			j = getAnalogSensor();
			
			if (sensor_inp() == 0x2) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// 積分リセット
				pattern = 36;
				break;
			}
			break;
			
		case 32:
			// 外線読み飛ばし
			SetAngle = angle_rightclank;
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			j = getAnalogSensor();
			
			if( -TurningAngleIMU <= 90 && -TurningAngleIMU >= 40) {
				if( j <= -1800 && sensor_inp() == 0x2 ) {
					enc1 = 0;
					i = (short)TurningAngleIMU;
					i = -i;
					j = getServoAngle();
					pattern = 36;
					break;
				}
			}
			break;
			
		case 34:
			// 角度維持
			// sensor_inp() == 2を読んだあとに実行
			SetAngle = -( 90 - 10 - i ) * DEGPERAD;	// ラインからの角度10°
			//SetAngle = angle_rightclank + 160;
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
			j = getAnalogSensor();
			
			if( sensor_inp() == 0x2 && j >= -1800 ) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// 積分リセット
				pattern = 36;
				break;
			}
			break;
			
		case 36:
			// 復帰
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;

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
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			i = -TurningAngleIMU;
			j = getAnalogSensor();
			
			if ( i >= 20 ) {
				if( j >= 1800 && sensor_inp() == 0x2 ) {
					enc1 = 0;
					i = TurningAngleIMU;
					pattern = 44;
					break;
				}
			}
			break;
			
		case 42:
			// 外線読み飛ばし
			SetAngle = angle_leftclank;
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			j = getAnalogSensor();
			
			if( TurningAngleIMU <= 90 && TurningAngleIMU >= 40) {
				if( j >= 1800 && sensor_inp() == 0x2 ) {
					enc1 = 0;
					i = TurningAngleIMU;
					pattern = 46;
					break;
				}
			}
			break;
			
		case 44:
			// 角度維持
			// sensor_inp() == 2を読んだあとに実行
			SetAngle = ( 90 - 10 - i ) * DEGPERAD;	// ラインからの角度10°
			//SetAngle = angle_leftclank - 160;
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			j = getAnalogSensor();
			
			if( sensor_inp() == 0x2 && j <= 1800) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// 積分リセット
				pattern = 46;
				break;
			}
			break;
			
		case 46:
			// 復帰
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			
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
			targetSpeed = speed_halfine * SPEED_CURRENT;
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// 積分リセット
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
			targetSpeed = speed_rightchange_trace * SPEED_CURRENT;
			led_out( LED_R );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				mode_angle = 1;
				pattern = 53;
				break;
			}
			break;
			
		case 53:
			SetAngle = angle_rightchange;
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x1 ) {
				enc1 = 0;
				pattern = 54;
				break;
			}
			break;
			
		case 54:
			SetAngle = 0;
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x4 ) {
				enc1 = 0;
				mode_angle = 0;
				mode_autoMotor = 0;
				Int = 0;			// 積分リセット
				pattern = 55;
				break;
			}
			break;
			
		case 55:
			servoPwmOut( 90 );
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x2 ) {
				angle_center = getServoAngle();
				enc1 = 0;
				mode_autoMotor = 1;
				pattern = 56;
				break;
			}
			break;
			
		case 56:
			targetSpeed = speed_rightchange_escape * SPEED_CURRENT;

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
			targetSpeed = speed_halfine * SPEED_CURRENT;
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// 積分リセット
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
			targetSpeed = speed_leftchange_trace * SPEED_CURRENT;
			led_out( LED_B );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				mode_angle = 1;
				pattern = 63;
				break;
			}
			break;
			
		case 63:
			SetAngle = angle_leftchange;
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x4 ) {
				enc1 = 0;
				pattern = 64;
				break;
			}
			break;
			
		case 64:
			SetAngle = 0;
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;

			if( sensor_inp() == 0x1 ) {
				enc1 = 0;
				mode_angle = 0;
				mode_autoMotor = 0;
				Int = 0;			// 積分リセット
				pattern = 65;
				break;
			}
			break;
			
		case 65:
			servoPwmOut( -90 );
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x2 ) {
				angle_center = getServoAngle();
				enc1 = 0;
				mode_autoMotor = 1;
				pattern = 66;
				break;
			}
			break;
			
		case 66:
			targetSpeed = speed_leftchange_escape * SPEED_CURRENT;

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
			// 目標速度変えない
			if( check_slope() == 1 ) {
				if( mode_slope == 0 ) {
					// 上り始め
					mode_slope = 1;
					enc1 = 0;
					led_out( LED_R | LED_B );
					pattern = 72;
					break;
				} else if ( mode_slope == 2 && enc_slope >= enc_mm( 600 ) ) {
					// 下り終わり
					mode_slope = 3;
					enc1 = 0;
					led_out( LED_R | LED_G | LED_B);
					pattern = 74;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
			} else if ( check_slope() == -1 ) {
				if( mode_slope == 1 && enc_slope >= enc_mm( 1000 ) ) {
					// 上り終わり、下り始め
					mode_slope = 2;
					enc1 = 0;
					pattern = 75;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
				break;
			} else {
				enc1 = 0;
				pattern = 11;
				break;
			}
			break;
			
		case 72:
			// 坂頂点まで走行
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			
			if( enc1 >= enc_mm( 1200 ) ) {
				enc1 = 0;
				pattern = 73;
				break;
			}
			break;
			
		case 73:
			// 上り坂終点ブレーキ 全力ブレーキ
			targetSpeed = 0;

			if( enc1 >= enc_mm( 50 ) ) {
				enc1 = 0;
				pattern = 75;
				break;
			}
			break;
			
		case 74:
			// 下り坂終点ブレーキ
			targetSpeed = speed_slope_brake * SPEED_CURRENT;
			if( enc1 >= enc_mm( 40 ) ) {
				enc1 = 0;
				pattern = 75;
				break;
			}
			break;
			
		case 75:
			// ジャイロセンサが安定するまで読み飛ばし
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			
			if( enc1 >= enc_mm( 150 ) ) {
				enc1 = 0;
				pattern = 76;
				break;
			}
			break;
			
		case 76:
			// ジャイロセンサが安定するまで読み飛ばし
			if ( mode_slope == 3 ) {
				targetSpeed = speed_straight * SPEED_CURRENT;
			} else {
				targetSpeed = speed_slope_trace * SPEED_CURRENT;
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
			if( mode_slope  == 0 ) {
				if( enc1 >= enc_mm( 1000 ) ) {
					enc1 = 0;
					enc_slope = 0;
					pattern = 11;
					break;
				}
			} else if ( mode_slope == 3 ) {
				if( enc1 >= enc_mm( 500 ) ) {
					enc1 = 0;
					enc_slope = 0;
					pattern = 11;
					break;
				}
			} else {
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
			
			LEDR_OFF;
			LEDG_OFF;
			LEDB_OFF;
	
			pattern = 102;
			break;
			
		case 102:
			servoPwmOut( ServoPwm );
			targetSpeed = 0;
			motorPwmOut(motorPwm, motorPwm, motorPwm, motorPwm);
			
			if( Encoder <= 0 && Encoder >= -1 ) {
				enc1 = 0;
				pattern = 103;
				break;
			}
			break;
			
		case 103:
			servoPwmOut( ServoPwm );
			motorPwmOut(0, 0, 0, 0);
			
			if( msdset == 1 ) {
				pattern = 104;
				cnt1 = 0;
				break;
			}else{
				pattern = 106;
				break;
			}
			break;
			
		case 104:
			servoPwmOut( ServoPwm );
			// 最後のデータが書き込まれるまで待つ
			if ( cnt1 <= 1000 ) {	// 500ms待つ
				if( checkMicroSDProcess() == 11 ) {
					msdFlag = 0;			// ログ記録終了
					microSDProcessEnd();        // microSDProcess終了処理
					pattern = 105;
					break;
				}
			} else {			// 500ms以上経過したら終了
				servoPwmOut( 0 );
				R_PG_IO_PORT_Write_PE6( 0 );	//サーボモータ freeモード
				pattern = 107;
				break;
			}
			break;
			
		case 105:
			servoPwmOut( ServoPwm );
			// 終了処理が終わるまで待つ
			if( checkMicroSDProcess() == 0 ) {
				// MicroSD最終書き込みアドレス保存
				flashDataBuff[ 0 ] = msdWorkaddress >> 16;
				flashDataBuff[ 1 ] = msdWorkaddress & 0xffff;	// 終了アドレス
				writeFlashData( MSD_STARTAREA, MSD_ENDAREA, MSD_DATA, 2 );
				servoPwmOut( 0 );
				R_PG_IO_PORT_Write_PE6( 0 );	//サーボモータ freeモード
				pattern = 106;
				break;
			}
			break;
			
		case 106:
			// mMicroSD書き込み成功
			// LED点滅処理
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				LEDB_ON;
			}else{
				LEDB_OFF;
			}
			break;
			
		case 107:
			// mMicroSD書き込み失敗
			// LED点滅処理
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				LEDR_ON;
			}else{
				LEDR_OFF;
			}
			break;
			
		default:
			pattern = 101;
			break;
			
	} // end of "switch ( pattern )"
	} // end of "while ( 1 )"
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 Timer
// 処理概要     1msごとにタイマ割り込み
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void Timer (void) {
	short s;
	
	__setpsw_i();
	//　タイマカウント
	if ( pattern >= 11 ) {
		if ( pattern <= 99 ) {
			if ( pattern != 21 ) {				// クロスライン通過時は無視
				if ( sensor_inp() == 0x7 || sensor_inp() == 0x5 ) {	// センサ全灯
					cnt_out++;	
				} else {
					cnt_out = 0;
				}
			}
			if ( sensor_inp() == 0x0 && pattern != 53 && pattern != 63 ) cnt_out2++;	// センサ全消灯
			else cnt_out2 = 0;
			if ( Encoder <= 1 && Encoder >= -1 ) cnt_out3++;		// エンコーダ停止(ひっくり返った？)
			else cnt_out3 = 0;
			s = (short)RollAngleIMU;
			if ( s >= 5 || s <= -5 ) cnt_out4++;
			else	cnt_out4 = 0;
		}
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
	if ( mode_lcd ) lcdShowProcess();

	// エンコーダカウント
	getEncoder();

	// PID制御値算出
	if ( mode_angle ) servoControl2();	// 角度
	else servoControl();		// 白線
	motorControl();		// モータ

	// 走行中のPWM出力
	if ( mode_autoMotor ) {
		if ( mode_angle ) servoPwmOut( ServoPwm2 );	// 角度
		else servoPwmOut( ServoPwm );	// 白線
		if (!mode_pushcart) {
			diff( motorPwm ); // 駆動輪モータPWM出力
		}
	}
	
	// MicroSD書き込み
	microSDProcess();
	if ( msdFlag ) sendLog( 8, 8, 3
					// char
					, pattern
					, motorPwm
					, sensor_inp()
					, mode_slope
					, (char)Encoder
					, sPwm
					, (char)(PichAngleIMU*10)
					, (char)(RollAngleIMU*10)
					//short
					, (short)(TurningAngleIMU*10)
					, xg
					, yg
					, zg
					, getServoAngle()
					, SetAngle
					, getAnalogSensor()
					, (short)(Voltage*100)
					// unsigned int
					, EncoderTotal
					, enc1
					, cnt_log
					);
	
	Timer10++;
	
	// 通信
	// 加速度及び角速度を取得
	
		
	// 10ｍごとに実行
	switch ( Timer10 ) {	
	case 1:
		getSwitch();		// スイッチ読み込み
		get_voltage();		// 電源電圧取得
		break;
	case 2:
		read_gyro_data();
		read_accel_data();
		getTurningAngleIMU();
		getPichAngleIMU();
		getRollAngleIMU();
		if (cnt_gyro > 200) {
			RollAngleIMU = 0;
			PichAngleIMU = 0;
			cnt_gyro  = 0;
		}
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

///////////////////////////////////////////////////////////////////////////
// モジュール名 init_Parameter
// 処理概要     変数の初期化
// 引数         lcd: 1 lcd表示  0 lcd非表示
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void init_Parameter ( bool lcd ) {
	cntmpattern2 = 0;	// 記録走行カウントリセット
	EncoderTotal = 10;	// 総走行距離
	cnt1 = 0;			// タイマリセット
	enc1 = 0;			// 区間距離リセット
	mode_lcd = lcd;		// LCD表示OFF
	mode_autoMotor = 1; // PWM出力
	msdFlag = 1;		// データ記録開始
	targetSpeed = speed_straight * SPEED_CURRENT; // 目標速度設定
	
	// 角度積算値リセット
	TurningAngleIMU = 0;
	RollAngleIMU = 0;
	PichAngleIMU = 0;
	pattern = 11;		// 通常走行
}