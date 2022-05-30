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
#include "AQM0802A.h"
#include "microSD.h"
#include "ICM20648.h"
#include "MemorryTrace.h"
//====================================//
// グローバル変数の宣言
//====================================//
// 走行パターン関連
char		pattern = 0;	// パターン番号

// モード関連
char		modeCurve;		// カーブ判定	0:カーブ以外	1:カーブ走行中
char		modeError;		// 0: 通常走行 1:距離停止 2:センサ全灯 3:センサ全消灯 4:エンコーダ停止 5:ジャイロ反応
char 		modeAutoMotor;	// 0: switch文でサーボ、駆動モータのPWM出力を指定する 1: Timer関数内で自動的にPWM出力を実行

// タイマ関連
// 1msタイマ
unsigned int 		cnt1;		// 走行用タイマカウント
unsigned short	 	cntOut1;	// コースアウト判定用タイマ
unsigned short	 	cntOut2;	// コースアウト判定用タイマ2
unsigned short	 	cntOut3;	// コースアウト判定用タイマ3
unsigned short	 	cntOut4;	// コースアウト判定用タイマ4
static char			Timer10;	// 1msカウント用
//====================================//
// プロトタイプ宣言
//====================================//
void initParameter ( bool lcd );
//====================================//
// メインプログラム
//====================================//
void main(void){
	char		countdown = 0x0;
	short i, j, angleAfter, angleCenter;
	double y1;
	unsigned int ui;
	
	//=================================//
	// 初期化
	//=================================//
	ledOut(0); 		// LED消灯
	L_Sen_ON;		// センサ点灯
	intiLcd();		// LCD初期化
	
	motorPwmOut(0, 0, 0, 0);	// モーター停止
	servoPwmOut( 0 );
	
	modePushcart = 0;		// 手押しモードoff
	modeSlope = 0;			// 上り坂チェック
	modeAngle = 0;			// 白線トレース
	modeAutoMotor = 0;		// 自動PWM出力停止
	start = 0;				// ゲートスタート
	
	//IMU初期化
	initIMU();
	// フラッシュ初期化
	// データフラッシュから前回パラメータを読み込む
	if( !initFlash() ) readFlashSetup( 1, 1, 1 ,1 ,1 ,1 ,1);
	// MicroSDカード初期化
	if( !initMicroSD() ) msdset = 1;
	else msdset = 0;

	// 電源電圧の確認
	if (Voltage < LOWVOLTAGE ) {
		cnt1=0;
		while( cnt1 < 1500){
			lcdRowPrintf(UPROW, "LOW BAT ");
			lcdRowPrintf(LOWROW, "  %05.2fV",Voltage);
			ledOut(LED_R);
			// while(1);
		}
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
		ledOut(LED_G);
	}
	
	while(1){
		__setpsw_i();
		if( pattern >= 11 && pattern <= 99 ) {
			if( !modePushcart ) {		
				// 手押しモードOFF
				if( cnt1 >= 100 ) {		// 動き出してから
					if ( EncoderTotal >= ( PALSE_METER * stopping_meter ) ) { // 距離超過の場合
						modeError = 1;
					} else if ( cntOut1 >= STOP_SENSOR1 ) {	// センサ全灯
						modeError = 2;
					} else if ( cntOut2 >= STOP_SENSOR2 ) {	// センサ全消灯
						modeError = 3;
					} else if ( cntOut3 >= STOP_ENCODER ) {	// エンコーダ停止(ひっくり返った？)
						modeError = 4;
					} else if( cntOut4 >= STOP_GYRO ) {	// マイナスの加速度検知(コースから落ちた？)
						modeError = 5;	
					}
					// // Buletoothで外部から停止
					// if ( stopWord == 1 ) {
					// 	modeError = 6;
					// }
					// 一定時間で停止
					// if( cntStable >= STOP_COUNT ) {
					// 	modeError = 7;
					// }
					if (modeError > 0) {
						pattern = 101;
						modeAutoMotor = 0;
						ui = cnt1;	// 走行時間取得
						LEDR_OFF;
						LEDG_OFF;
						LEDB_OFF;
					}
				}
			} else {			
				// 手押しモードON

				// 距離測定
				// lcdRowPrintf(UPROW, "mm%4.1f", (double)EncoderTotal/PALSE_MILLIMETER);
				// lcdRowPrintf(LOWROW, " %7d", EncoderTotal);

				lcdRowPrintf(UPROW, "now  %3d", pattern);
				lcdRowPrintf(LOWROW, " %7d", enc1);
			}
			// スイッチで停止
			if ( cnt1 >= 1000 && taswGet() == SW_PUSH ) {
				modeError = 6;
				pattern = 101;
			}
		} else if ( pattern >= 100 ) {
			modeLCD = 1;
			// 速度ゲイン調整用
			// lcdRowPrintf(UPROW, "TIME%4d", cntStable);
			// lcdRowPrintf(LOWROW, "%6.1f", (double)encStable/PALSE_MILLIMETER);

			// エラーモード確認
			lcdRowPrintf(UPROW, "MODE   %1d", modeError);
			lcdRowPrintf(LOWROW, "    %4d",angleAfter);
		}
		
	switch( pattern ) {
		//-------------------------------------------------------------------
		// 【000】スタート処理
		//-------------------------------------------------------------------
		case 0:
			// スタート前設定
			setup();
			if ( start && !modePushcart ) {
				demo = 0;		// デモモード解除
				modeAngle = 0;	// 白線トレース
				Int = 0;			// 積分リセット
				txt= txtData;		// 受信配列リセット
				cntByte = 0;		// 受信カウントリセット
				
				if ( msdset ) init_log();	// ログ記録準備
				
				if ( !fixSpeed ) writeFlashBeforeStart(1, 0, 1, 1, 1, 1);	// 速度パラメータをデータフラッシュに保存
				else writeFlashBeforeStart(0, 0, 1, 1, 1, 1);		// 速度パラメータ以外を保存
				
				//if (IMUSet) caribrateIMU();
				
				waitLcd(500);		// 500ms待つ
				cnt1 = 0;
				pattern = 1;
				break;
			} else if ( start && modePushcart ) {
				// 手押しモードの場合すぐに通常トレース
				if ( msdset ) init_log();	// ログ記録準備
				
				// 白線トレース用PIDゲイン保存
				// 角度制御用PIDゲイン保存
				writeFlashBeforeStart(0, 0, 1, 1, 0, 0);
				// 変数初期化
				initParameter( 1 );
				break;
			}
			break;
			
		case 1:
			servoPwmOut( ServoPwm );
			if ( start == START_COUNT ) {
				// カウントダウンスタート
				if ( cnt1 >= 3000 ) {	
					// 変数初期化
					initParameter( 0 );
					break;
				} else if ( !(cnt1 % 1000) ) {
					ledOut( countdown );
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
				initParameter( 0 );
				break;
			}
			// LED点滅処理
			if ( cnt1 >= 2000 ) cnt1 = 0;
			if ( cnt1 < 1000 ) {
				ledOut( LED_R );
			} else {
				ledOut( LED_B  );
			}
			break;
		//-------------------------------------------------------------------
		// 【010】トレース処理
		//-------------------------------------------------------------------
		case 11:
			targetSpeed = speed_straight * SPEED_CURRENT;
			i = getServoAngle();
			ledOut( 0x00 );
			
			// クロスラインチェック
			if ( checkCrossLine() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// 右ハーフラインチェック
	   		if ( checkRightLine() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if ( checkLeftLine() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if( checkSlope() == 1 || checkSlope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
			// カーブチェック
			if ( i >=  CURVE_R600_START || i <= -CURVE_R600_START ) {
				enc1 = 0;
				modeCurve = 1;
				pattern = 12;
				break;
			}
			break;
			
		case 12:
			// カーブブレーキ
			targetSpeed = speed_curve_brake * SPEED_CURRENT;
			ledOut( LED_R );
			i = getServoAngle();
			
			if ( enc1 > encMM( 60 ) ) {		// 60mm進む
				enc1 = 0;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 13;
				break;
			}
			
			// クロスラインチェック
			if ( checkCrossLine() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// 右ハーフラインチェック
	   		if ( checkRightLine() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if ( checkLeftLine() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if ( checkSlope() == 1 || checkSlope() == -1 ) {
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
			if ( checkCrossLine() ) {
				enc1 = 0;
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
	   		if ( checkRightLine() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if ( checkLeftLine() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if ( checkSlope() == 1 || checkSlope() == -1 ) {
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
			if ( checkCrossLine() ) {
				enc1 = 0;
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
	   		if ( checkRightLine() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if ( checkLeftLine() ) {
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
			
			if ( enc1 >= encMM( 300 ) ) {		// 300mm進む
				enc1 = 0;
				modeCurve = 0;
				pattern = 11;
				break;
			}
			
			// クロスラインチェック
			if ( checkCrossLine() ) {
				enc1 = 0;
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
   			if ( checkRightLine() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
   			if ( checkLeftLine() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if ( checkSlope() == 1 || checkSlope() == -1 ) {
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
			ledOut( LED_G );
			
			if( enc1 > encMM( 90 ) ) {		// 60mm進む
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
				ledOut( LED_R | LED_G);
				SetAngle = angle_rightclank;
				modeAngle = 1;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 31;
				break;
			}
			// 左クランクチェック
			if( sensor_inp() ==  0x6 ) {
				enc1 = 0;
				ledOut( LED_G | LED_B );
				SetAngle = angle_leftclank;
				modeAngle = 1;
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
			
			if (sensor_inp() == 0x2 && enc1 >= encMM( 50 ) ) {
				y1 = getLinePositionNow( getServoAngle(), TurningAngleIMU);
				enc1 = 0;
				Int = 0;			// 積分リセット
				pattern = 33;
				break;
			}
			break;
			
		case 33:
			SetAngle = angle_rightclank;
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x4 ) {
				angleAfter = getReturnAngle( TurningAngleIMU, y1);
				enc1 = 0;
				pattern = 34;
				break;
			}
			break;
			
		case 34:
			// 角度維持
			SetAngle = -angleAfter * DEG2AD;	// ラインからの角度10°
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x2 && enc1 >= encMM( 100 ) ) {
				enc1 = 0;
				modeAngle = 0;
				Int = 0;			// 積分リセット
				pattern = 35;
				break;
			}
			break;
			
		case 35:
			// 復帰
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;

			if( enc1 >= encMM( 600 ) ) {		// 安定するまで待つ(600mm)
				enc1 = 0;
				ledOut( 0x0 );
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
			
			if (sensor_inp() == 0x2 && enc1 >= encMM( 50 ) ) {
				y1 = getLinePositionNow( getServoAngle(), TurningAngleIMU);
				enc1 = 0;
				Int = 0;			// 積分リセット
				pattern = 43;
				break;
			}
			break;
			
		case 43:
			SetAngle = angle_leftclank;
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x1 ) {
				angleAfter = getReturnAngle( TurningAngleIMU, y1);
				enc1 = 0;
				pattern = 44;
				break;
			}
			break;
			
		case 44:
			// 角度維持
			SetAngle = angleAfter * DEG2AD;	// ラインからの角度10°
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x2 && enc1 >= encMM( 100 ) ) {
				enc1 = 0;
				modeAngle = 0;
				Int = 0;			// 積分リセット
				pattern = 45;
				break;
			}
			break;
			
		case 45:
			// 復帰
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;

			if( enc1 >= encMM( 600 ) ) {		// 安定するまで待つ(600mm)
				enc1 = 0;
				ledOut( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【050】右レーンチェンジ処理
		//-------------------------------------------------------------------
		case 51:
			targetSpeed = speed_halfine * SPEED_CURRENT;
			
			if( enc1 > encMM( 60 ) ) {
				enc1 = 0;
				modeAngle = 0;
				Int = 0;			// 積分リセット
				pattern = 52;
				break;
			}
			
			if( checkCrossLine() ) {		// クロスラインチェック
				enc1 = 0;
				pattern = 21;
				break;
			}
			break;
			
		case 52:
			targetSpeed = speed_rightchange_trace * SPEED_CURRENT;
			ledOut( LED_R );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				modeAngle = 1;
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
				modeAngle = 0;
				modeAutoMotor = 0;
				Int = 0;			// 積分リセット
				pattern = 55;
				break;
			}
			break;
			
		case 55:
			servoPwmOut( 90 );
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x2 ) {
				angleCenter = getServoAngle();
				enc1 = 0;
				modeAutoMotor = 1;
				pattern = 56;
				break;
			}
			break;
			
		case 56:
			targetSpeed = speed_rightchange_escape * SPEED_CURRENT;

			if( enc1 >= encMM( 600 ) ) {
				enc1 = 0;
				ledOut( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//【060】左レーンチェンジ処理
		//-------------------------------------------------------------------
		case 61:
			targetSpeed = speed_halfine * SPEED_CURRENT;
			
			if( enc1 > encMM( 60 ) ) {
				enc1 = 0;
				modeAngle = 0;
				Int = 0;			// 積分リセット
				pattern = 62;
				break;
			}
			if( checkCrossLine() ) {		// クロスラインチェック
				enc1 = 0;
				pattern = 21;
				break;
			}
			break;
			
		case 62:
			targetSpeed = speed_leftchange_trace * SPEED_CURRENT;
			ledOut( LED_B );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				modeAngle = 1;
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
				modeAngle = 0;
				modeAutoMotor = 0;
				Int = 0;			// 積分リセット
				pattern = 65;
				break;
			}
			break;
			
		case 65:
			servoPwmOut( -90 );
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x2 ) {
				angleCenter = getServoAngle();
				enc1 = 0;
				modeAutoMotor = 1;
				pattern = 66;
				break;
			}
			break;
			
		case 66:
			targetSpeed = speed_leftchange_escape * SPEED_CURRENT;

			if( enc1 >= encMM( 600 ) ) {
				enc1 = 0;
				ledOut( 0x0 );
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
			if( checkSlope() == 1 ) {
				if( modeSlope == 0 ) {
					// 上り始め
					modeSlope = 1;
					enc1 = 0;
					ledOut( LED_R | LED_B );
					pattern = 72;
					break;
				} else if ( modeSlope == 2 && enc_slope >= encMM( 600 ) ) {
					// 下り終わり
					modeSlope = 3;
					enc1 = 0;
					ledOut( LED_R | LED_G | LED_B);
					pattern = 74;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
			} else if ( checkSlope() == -1 ) {
				if( modeSlope == 1 && enc_slope >= encMM( 1000 ) ) {
					// 上り終わり、下り始め
					modeSlope = 2;
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
			
			if( enc1 >= encMM( 1200 ) ) {
				enc1 = 0;
				pattern = 73;
				break;
			}
			break;
			
		case 73:
			// 上り坂終点ブレーキ 全力ブレーキ
			targetSpeed = 0;

			if( enc1 >= encMM( 50 ) ) {
				enc1 = 0;
				pattern = 75;
				break;
			}
			break;
			
		case 74:
			// 下り坂終点ブレーキ
			targetSpeed = speed_slope_brake * SPEED_CURRENT;
			if( enc1 >= encMM( 40 ) ) {
				enc1 = 0;
				pattern = 75;
				break;
			}
			break;
			
		case 75:
			// ジャイロセンサが安定するまで読み飛ばし
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			
			if( enc1 >= encMM( 150 ) ) {
				enc1 = 0;
				pattern = 76;
				break;
			}
			break;
			
		case 76:
			// ジャイロセンサが安定するまで読み飛ばし
			if ( modeSlope == 3 ) {
				targetSpeed = speed_straight * SPEED_CURRENT;
			} else {
				targetSpeed = speed_slope_trace * SPEED_CURRENT;
			}
			
			// クロスラインチェック
			if( checkCrossLine() ) {
				enc1 = 0;
				pattern = 21;
				break;
      			}
			// 右ハーフラインチェック
   			if( checkRightLine() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// 左ハーフラインチェック
   			if( checkLeftLine() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			if( modeSlope  == 0 ) {
				if( enc1 >= encMM( 1000 ) ) {
					enc1 = 0;
					enc_slope = 0;
					pattern = 11;
					break;
				}
			} else if ( modeSlope == 3 ) {
				if( enc1 >= encMM( 500 ) ) {
					enc1 = 0;
					enc_slope = 0;
					pattern = 11;
					break;
				}
			} else {
				if( enc1 >= encMM( 400 ) ) {
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
			// 減速処理
			servoPwmOut( ServoPwm );
			targetSpeed = 0;
			motorPwmOut(motorPwm, motorPwm, motorPwm, motorPwm);
			
			if( Encoder <= 3 && Encoder >= -1 ) {
				enc1 = 0;
				pattern = 102;
				break;
			}
			break;
			
		case 102:
			// 車体停止処理
			servoPwmOut( ServoPwm );
			motorPwmOut(0, 0, 0, 0);
			
			if( Encoder <= 5 && Encoder >= -1 ) {
				servoPwmOut( 0 );
				R_PG_IO_PORT_Write_PE6( 0 );	//サーボモータ freeモード
				if( msdset == 1 ) {
					// microSDの動作が有効なとき
					pattern = 103;
					cnt1 = 0;
					break;
				} else {
					// microSDの動作が無効なとき
					pattern = 106;
					break;
				}
			}
			break;
			
		case 103:
			// 最後のデータが書き込まれるまで待つ
			if ( cnt1 <= 1000 ) {	// 1000ms待つ
				if( checkMicroSDProcess() == 11 ) {
					msdFlag = 0;			// ログ記録終了
					microSDProcessEnd();    // microSDProcess終了処理
					pattern = 104;
					break;
				}
			} else {			// 1000ms以上経過したら書き込みを強制終了
				pattern = 106;
				break;
			}
			break;
			
		case 104:
			// microSDの書き込み終了処理が終わるまで待つ
			if( checkMicroSDProcess() == 0 ) {
				// MicroSD最終書き込みアドレス保存
				flashDataBuff[ 0 ] = msdWorkaddress >> 16;
				flashDataBuff[ 1 ] = msdWorkaddress & 0xffff;	// 終了アドレス
				writeFlashData( MSD_STARTAREA, MSD_ENDAREA, MSD_DATA, 2 );
				pattern = 105;
				break;
			}
			break;
			
		case 105:
			// microSD書き込み成功
			// LED点滅処理
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				LEDB_ON;
			} else {
				LEDB_OFF;
			}
			break;
			
		case 106:
			// mMicroSD書き込み失敗
			// LED点滅処理
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				LEDR_ON;
			} else {
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
	__setpsw_i();	// 多重割り込み許可

	//　タイマカウント
	if ( pattern >= 11 ) {
		if ( pattern <= 99 ) {
			if ( pattern != 21 ) {				// クロスライン通過時は無視
				if ( sensor_inp() == 0x7 || sensor_inp() == 0x5 ) {	// センサ全灯
					cntOut1++;	
				} else {
					cntOut1 = 0;
				}
			}
			if ( sensor_inp() == 0x0 && pattern != 53 && pattern != 63 ) cntOut2++;	// センサ全消灯
			else cntOut2 = 0;
			if ( Encoder <= 1 && Encoder >= -1 ) cntOut3++;		// エンコーダ停止(ひっくり返った？)
			else cntOut3 = 0;
			if ( (short)RollAngleIMU >= 5 || (short)RollAngleIMU <= -5 ) cntOut4++;
			else	cntOut4 = 0;
		}
	} else if ( pattern < 11 ) {
		cntSetup1++;
		cntSetup2++;
		cntSetup3++;
		cnt_flash++;
	}
	cnt1++;
	cntGyro++;
			
	// LCD表示
	if ( modeLCD ) lcdShowProcess();

	// エンコーダカウント取得
	getEncoder();
	// 速度ゲイン調整用
	// if (pattern > 10 && pattern < 100) stableSpeedDistance();

	// PID制御値算出
	if ( modeAngle ) servoControlAngle();	// 角度
	else servoControlTrace();		// 白線
	motorControl();		// モータ

	// 走行中のPWM出力
	if ( modeAutoMotor ) {
		if ( modeAngle ) servoPwmOut( ServoPwm2 );	// 角度
		else servoPwmOut( ServoPwm );	// 白線
		if (!modePushcart) {
			diff( motorPwm ); // 駆動輪モータPWM出力
		}
	}
	
	// MicroSD書き込み
	microSDProcess();
	if ( msdFlag ) sendLog( 8, 9, 3
					// char
					, pattern
					, motorPwm
					, sensor_inp()
					, modeSlope
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
					, targetSpeed
					// unsigned int
					, EncoderTotal
					, encStable
					, cnt_log
					);
	
	Timer10++;
	
	// 通信
	// 加速度及び角速度を取得
	
		
	// 10ｍごとに実行
	switch ( Timer10 ) {	
	case 1:
		getSwitch();		// スイッチ読み込み
		getVoltage();		// 電源電圧取得
		break;
	case 2:
		readGyroData();
		readAccelData();
		getTurningAngleIMU();
		getPichAngleIMU();
		getRollAngleIMU();
		if (cntGyro > 200) {
			RollAngleIMU = 0;
			PichAngleIMU = 0;
			cntGyro  = 0;
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
// モジュール名 initParameter
// 処理概要     変数の初期化
// 引数         lcd: 1 lcd表示  0 lcd非表示
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void initParameter ( bool lcd ) {
	cntmpattern2 = 0;	// 記録走行カウントリセット
	EncoderTotal = 10;	// 総走行距離
	cnt1 = 0;			// タイマリセット
	enc1 = 0;			// 区間距離リセット
	modeLCD = lcd;		// LCD表示OFF
	modeAutoMotor = 1; // PWM出力
	msdFlag = 1;		// データ記録開始
	targetSpeed = speed_straight * SPEED_CURRENT; // 目標速度設定
	
	// 角度積算値リセット
	TurningAngleIMU = 0;
	RollAngleIMU = 0;
	PichAngleIMU = 0;
	pattern = 11;		// 通常走行
}