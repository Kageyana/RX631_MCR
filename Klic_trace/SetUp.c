//====================================//
// インクルード									//
//====================================//
#include "SetUp.h"
//====================================//
// グローバル変数の宣言							//
//====================================//
char 	start;		// 0:セットアップ中	1:セットアップ完了

// タイマ関連
unsigned short 		cnt_setup;		// セットアップで使用
unsigned short 		cnt_setup2;	// セットアップで使用
unsigned short 		cnt_setup3;	// セットアップで使用
short				cnt_swR;		// スイッチ長押し判定用右
short				cnt_swL;		// スイッチ長押し判定用左

// スイッチ関連
signed char pushL = 0;
signed char pushR = 0;

// パターン関連
char push = 0;
char push1 = 0;
char pattern_sensor = 1;
char pattern_parameter = 1;
char pattern_parameter2 = 1;
char pattern_parameter3 = 1;
char pattern_parameter4 = 1;
char pattern_gain = 1;
char pattern_gain2 = 1;
char pattern_gain3 = 1;
char pattern_speedseting = 1;
char pattern_msd = 1;
char pattern_flash = 1;
char setting_1meter;
char setting_2meter;
char setting_3meter;
char motor_test = 0;
char servo_test = 0;
char servo_test2 = 0;
char fixSpeed = 0;

//====================================//
// プロトタイプ宣言								//
//====================================//
void data_select ( char *data , char button );
void data_tuning ( void *data, char add , char direction );
void wait2 ( int waittime );

char ble;
//////////////////////////////////////////////////////////////////////////
// モジュール名 setup							//
// 処理概要     走行前設定						//
// 引数         なし							//
// 戻り値       なし 							//
//////////////////////////////////////////////////////////////////////////
void setup( void )
{
	char cnt_led;
	short s;
	
	// LED点滅処理
	
	if ( cnt_setup2 >= 600 ) cnt_setup2 = 0;
	if ( cnt_setup2 < 300 ) {
		if ( EncoderTotal % 12 == 1 ) led_out( 0x12 );
		else led_out( 0x02 );
	} else {
		if ( EncoderTotal % 12 == 1 ) led_out( 0x11 );
		else led_out( 0x01 );
	}
	
	// ディップスイッチで項目選択
	switch ( dipsw_get() ) {
		//------------------------------------------------------------------
		// 【0x0】スタート待ち
		//------------------------------------------------------------------
		case 0x0:
			lcdPosition( 0, 0 );
			lcdPrintf("START   ");
			lcdPosition( 0, 1 );
			lcdPrintf("%d  READY", startbar_get());
			
			//0x8を押すと白線追従
			angle_mode = 0;
			data_select( &servo_test, 8 );
			if ( servo_test == 1 ) servoPwmOut( ServoPwm );
			else servoPwmOut( 0 );
			
			// プッシュスイッチ押下待ち
			if ( tasw_get() == 0x1 ) start = 2;
			else if ( tasw_get() == 0x2 ) start = 1;
			
			break;
		//------------------------------------------------------------------
		// 【0x1】手押しモード選択
		//------------------------------------------------------------------
		case 0x1:
			lcdPosition( 0, 0 );
			lcdPrintf("PUSHCART");
			lcdPosition( 0, 1 );
			if ( pushcart_mode == 1 ) lcdPrintf("      ON");
			else lcdPrintf("     OFF");
			
			data_select ( &pushcart_mode, 1 );
			break;
		//------------------------------------------------------------------
		// 【0x2】停止距離調整
		//------------------------------------------------------------------
		case 0x2:
			lcdPosition( 0, 0 );
			lcdPrintf("STOP    ");
			lcdPosition( 0, 1 );
			lcdPrintf("   %4dm", stopping_meter );
			data_tuning ( &stopping_meter, 1, RIGHT );
			break;
		//------------------------------------------------------------------
		// 【0x3】パラメータ調整(通常トレース)
		//------------------------------------------------------------------
		case 0x3:
			data_tuning ( &pattern_parameter, 1, LEFT );
			
			if ( pattern_parameter == 6 ) pattern_parameter = 1;
			else if ( pattern_parameter == 0 ) pattern_parameter = 5;
			
			switch( pattern_parameter ) {
				case 1:
					// 通常走行速度
					lcdPosition( 0, 0 );
					lcdPrintf("STRAIGHT");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_straight / 10 );
					
					data_tuning ( &speed_straight, 1, RIGHT );
					break;
				case 2:
					// カーブブレーキ
					lcdPosition( 0, 0 );
					lcdPrintf("BRAKE   ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_curve_brake / 10 );
					
					data_tuning ( &speed_curve_brake, 1, RIGHT );
					break;
				case 3:
					// R600カーブ走行速度
					lcdPosition( 0, 0 );
					lcdPrintf("R600    ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_curve_r600 / 10  );
					
					data_tuning ( &speed_curve_r600, 1, RIGHT );
					break;
				case 4:
					// R450カーブ走行速度
					lcdPosition( 0, 0 );
					lcdPrintf("R450    ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_curve_r450 / 10 );
					
					data_tuning ( &speed_curve_r450, 1, RIGHT );
					break;
				case 5:
					// S字カーブ直線速度
					lcdPosition( 0, 0 );
					lcdPrintf("CURVE_ST");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_curve_straight / 10 );
					
					data_tuning ( &speed_curve_straight, 1, RIGHT );
					break;
			}
			break;
			
		//------------------------------------------------------------------
		// 【0x4】パラメータ調整(クランク)
		//------------------------------------------------------------------
		case 0x4:
			data_tuning ( &pattern_parameter2, 1, LEFT );
			
			if ( pattern_parameter2 == 7 ) pattern_parameter2 = 1;
			else if ( pattern_parameter2 == 0 ) pattern_parameter2 = 6;
			
			switch( pattern_parameter2 ) {
				case 1:
					// クロスライン通過時
					lcdPosition( 0, 0 );
					lcdPrintf("CROSLINE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_crossline / 10 );
					
					data_tuning ( &speed_crossline, 1, RIGHT );
					break;
				case 2:
					// クランク前トレース
					lcdPosition( 0, 0 );
					lcdPrintf("CL_TRACE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_ckank_trace / 10 );
					
					data_tuning ( &speed_ckank_trace, 1, RIGHT );
					break;
				case 3:
					// 右クランク旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("RCL_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightclank_curve / 10 );
					
					data_tuning ( &speed_rightclank_curve, 1, RIGHT );
					break;
				case 4:
					// 右クランク復帰速度
					lcdPosition( 0, 0 );
					lcdPrintf("RCL_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightclank_escape / 10 );
					
					data_tuning ( &speed_rightclank_escape, 1, RIGHT );
					break;
					
				case 5:
					// 左クランク旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("LCL_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftclank_curve / 10 );
					
					data_tuning ( &speed_leftclank_curve, 1, RIGHT );
					break;
					
				case 6:
					// 左クランク復帰速度
					lcdPosition( 0, 0 );
					lcdPrintf("LCL_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftclank_escape / 10 );
					
					data_tuning ( &speed_leftclank_escape, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0x5】パラメータ調整(レーンチェンジ)
		//------------------------------------------------------------------
		case 0x5:
			data_tuning ( &pattern_parameter3, 1, LEFT );
			
			if ( pattern_parameter3 == 8 ) pattern_parameter3 = 1;
			else if ( pattern_parameter3 == 0 ) pattern_parameter3 = 7;
			
			switch( pattern_parameter3 ) {
				case 1:
					// ハーフライン通過時
					lcdPosition( 0, 0 );
					lcdPrintf("HALFLINE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_halfine / 10 );
					
					data_tuning ( &speed_halfine, 1, RIGHT );
					break;
				case 2:
					// 右レーンチェンジ進入速度
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_STR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_trace / 10 );
					
					data_tuning ( &speed_rightchange_trace, 1, RIGHT );
					break;
				case 3:
					// 右レーンチェンジ旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_curve / 10 );
					
					data_tuning ( &speed_rightchange_curve, 1, RIGHT );
					break;
				case 4:
					// 右レーンチェンジ復帰速度
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_escape / 10 );
					
					data_tuning ( &speed_rightchange_escape, 1, RIGHT );
					break;
				case 5:
					// 左レーンチェンジ進入速度
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_STR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_trace / 10 );
					
					data_tuning ( &speed_leftchange_trace, 1, RIGHT );
					break;
				case 6:
					// 左レーンチェンジ旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_curve / 10 );
					
					data_tuning ( &speed_leftchange_curve, 1, RIGHT );
					break;
				case 7:
					// 左レーンチェンジ旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_escape / 10 );
					
					data_tuning ( &speed_leftchange_escape, 1, RIGHT );
					break;
			}
			break;
		
		//------------------------------------------------------------------
		// 【0x6】パラメータ調整(坂道、角度)
		//------------------------------------------------------------------
		case 0x6:
			data_tuning ( &pattern_parameter4, 1, LEFT );
			
			if ( pattern_parameter4 == 7 ) pattern_parameter4 = 1;
			else if ( pattern_parameter4 == 0 ) pattern_parameter4 = 6;
			
			switch( pattern_parameter4 ) {
				case 1:
					// 坂道ブレーキ
					lcdPosition( 0, 0 );
					lcdPrintf("SL_BRAKE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_slope_brake / 10 );
					
					data_tuning ( &speed_slope_brake, 1, RIGHT );
					break;
				case 2:
					// 坂読み飛ばし速度
					lcdPosition( 0, 0 );
					lcdPrintf("SL_TRACE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_slope_trace / 10 );
					
					data_tuning ( &speed_slope_trace, 1, RIGHT );
					break;
				case 3:
					// 右クランク旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_RCR ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_rightclank );
					
					data_tuning ( &angle_rightclank, 1, RIGHT );
					break;
				case 4:
					// 左クランク旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_LCL ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_leftclank );
					
					data_tuning ( &angle_leftclank, 1, RIGHT );
					break;
				case 5:
					// 右レーンチェンジ旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_RLC ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_rightchange );
					
					data_tuning ( &angle_rightchange, 1, RIGHT );
					break;
				case 6:
					// 左レーンチェンジ旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_LLC ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_leftchange );
					
					data_tuning ( &angle_leftchange, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0x7】ゲイン調整(サーボ)
		//------------------------------------------------------------------
		case 0x7:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			data_select( &servo_test, 8 );
			angle_mode = 0;
			if ( servo_test == 1 ) servoPwmOut( ServoPwm );
			else servoPwmOut( 0 );
			
			if ( tasw_get() == 0x4 && push1 == 0 ) {
				push1 = 1;
				pattern_gain += 1;
			} else if ( tasw_get() == 0x0 ) {
				push1 = 0;
			}
			if ( pattern_gain == 4 ) pattern_gain = 1;
			else if ( pattern_gain == 0 ) pattern_gain = 3;
			
			switch( pattern_gain ) {
				case 1:
					// kp
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("   %2d %2d", ki_buff, kd_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp_buff, ki_buff, kd_buff);
					}
					
					data_tuning ( &kp_buff, 1, RIGHT );
					break;
				case 2:
					// ki
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("%2d    %2d", kp_buff, kd_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp_buff, ki_buff, kd_buff);
					}
					
					data_tuning ( &ki_buff, 1, RIGHT );
					break;
				case 3:
					// kd
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("%2d %2d   ", kp_buff, ki_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp_buff, ki_buff, kd_buff);
					}
					
					data_tuning ( &kd_buff, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0x8】ゲイン調整(角度)
		//------------------------------------------------------------------
		case 0x8:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			data_select( &servo_test2, 8 );
			angle_mode = 1;
			if ( servo_test == 1 ) servoPwmOut( ServoPwm2 );
			else servoPwmOut( 0 );
			if ( servo_test2 == 1 ) SetAngle = 120;
			else SetAngle = -120;
			
			if ( tasw_get() == 0x4 && push1 == 0 ) {
				push1 = 1;
				pattern_gain2 += 1;
			} else if ( tasw_get() == 0x0 ) {
				push1 = 0;
			}
			if ( pattern_gain2 == 4 ) pattern_gain2 = 1;
			else if ( pattern_gain2 == 0 ) pattern_gain2 = 3;
			
			switch( pattern_gain2 ) {
				case 1:
					// kp2
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) lcdPrintf("   %2d %2d", ki2_buff, kd2_buff);
					else				lcdPrintf("%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					
					data_tuning ( &kp2_buff, 1, RIGHT );
					break;
				case 2:
					// ki2
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) lcdPrintf("%2d    %2d", kp2_buff, kd2_buff);
					else				lcdPrintf("%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					
					data_tuning ( &ki2_buff, 1, RIGHT );
					break;
				case 3:
					// kd2
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 )	lcdPrintf("%2d %2d   ", kp2_buff, ki2_buff);
					else				lcdPrintf("%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					
					data_tuning ( &kd2_buff, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0x9】ゲイン調整(速度)
		//------------------------------------------------------------------
		case 0x9:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			if ( tasw_get() == 0x4 && push1 == 0 ) {
				push1 = 1;
				pattern_gain3 += 1;
			} else if ( tasw_get() == 0x0 ) {
				push1 = 0;
			}
			if ( pattern_gain3 == 4 ) pattern_gain3 = 1;
			else if ( pattern_gain3 == 0 ) pattern_gain3 = 3;
			
			switch( pattern_gain3 ) {
				case 1:
					// kp3
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) lcdPrintf("   %2d %2d", ki3_buff, kd3_buff );
					else				lcdPrintf("%2d %2d %2d", kp3_buff, ki3_buff, kd3_buff );
					
					data_tuning ( &kp3_buff, 1, RIGHT );
					break;
				case 2:
					// ki3
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) lcdPrintf("%2d    %2d", kp3_buff, kd3_buff );
					else				lcdPrintf("%2d %2d %2d", kp3_buff, ki3_buff, kd3_buff );
					
					data_tuning ( &ki3_buff, 1, RIGHT );
					break;
					
				case 3:
					// kd3
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) lcdPrintf("%2d %2d   ", kp3_buff, ki3_buff );
					else 				lcdPrintf("%2d %2d %2d", kp3_buff, ki3_buff, kd3_buff );
					
					data_tuning ( &kd3_buff, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0xa】1m/sでのパラメータ
		//------------------------------------------------------------------
		case 0xa:
			data_tuning ( &pattern_speedseting, 1, LEFT );
				
			if ( pattern_speedseting == 4 ) pattern_speedseting = 1;
			else if ( pattern_speedseting == 0 ) pattern_speedseting = 3;
			
			switch( pattern_speedseting ) {
				case 1:
					lcdPosition( 0, 0 );
					lcdPrintf("1METER %d", setting_1meter);
					lcdPosition( 0, 1 );
					lcdPrintf("SETTING ");
					data_select( &setting_1meter, 1 );
					if ( setting_1meter == 1 ) {
						fix_speedsetting();
						setting_2meter = 0;
						setting_3meter = 0;
					} else {
						fixSpeed = 0;
						setting_2meter = 0;
						setting_3meter = 0;
					}
					break;
					
				case 2:
					lcdPosition( 0, 0 );
					lcdPrintf("2METER %d", setting_2meter);
					lcdPosition( 0, 1 );
					lcdPrintf("SETTING ");
					
					data_select( &setting_2meter, 1 );
					if ( setting_2meter == 1 ) {
						fix_speedsetting();
						setting_1meter = 0;
						setting_3meter = 0;
					} else {
						fixSpeed = 0;
						setting_1meter = 0;
						setting_3meter = 0;
					}
					break;
					
				case 3:
					lcdPosition( 0, 0 );
					lcdPrintf("3METER %d", setting_3meter);
					lcdPosition( 0, 1 );
					lcdPrintf("SETTING ");
					
					data_select( &setting_3meter, 1 );
					if ( setting_3meter == 1 ) {
						fix_speedsetting();
						setting_1meter = 0;
						setting_2meter = 0;
					} else {
						fixSpeed = 0;
						setting_1meter = 0;
						setting_2meter = 0;
					}
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0xb】Motor_test
		//------------------------------------------------------------------
		case 0xb:
			data_tuning ( &pattern_sensor, 1, LEFT );
			angle_mode = 0;
			
			if ( pattern_sensor == 16 ) pattern_sensor = 1;
			else if ( pattern_sensor == 0 ) pattern_sensor = 15;
			
			switch( pattern_sensor ) {
				case 1:
					// LED
					lcdPosition( 0, 0 );
					lcdPrintf("LED     ");
					lcdPosition( 0, 1 );
					lcdPrintf("        ");
					cnt_led = 0x00;
					if ( tasw_get() == 0x1) {
						while( cnt_led <= 0x1f ) {
							led_out( cnt_led );
							cnt_led++;
							wait_lcd(200);
 						}
					}
					break;
					
				case 2:
					// ジャイロ
					if ( tasw_get() == 0x1 ) PichAngleIMU = 0;
					if ( tasw_get() == 0x2 ) RollAngleIMU = 0;
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("Roll%4d", (short)RollAngleIMU);
						lcdPosition( 0, 1 );
						lcdPrintf("Pich%4d",(short)PichAngleIMU);
					}
					break;
					
				case 3:
					// エンコーダ
					lcdPosition( 0, 0 );
					lcdPrintf("Encoder ");
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						lcdPrintf("   %4.1f",(double)EncoderTotal/PALSE_MILLIMETER);
					}
					break;
							
				case 4:
					// ポテンションメーター
					lcdPosition( 0, 0 );
					lcdPrintf("Angle   ");
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						lcdPrintf("    %4d",getServoAngle());
					}
					break;
					
				case 5:
					// アナログセンサ
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("R  %4d ",sensorR);
						lcdPosition( 0,1 );
						lcdPrintf("L  %4d ",sensorL);
					}
					break;
					
				case 6:
					// デジタルセンサ
					motor_test = 0;
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						startbar_get();
						lcdPosition( 0, 0 );
						lcdPrintf("G  %4d", sensorG);
						lcdPosition( 0, 1 );
						lcdPrintf("D    0x%x", sensor_inp());
					}
					break;
					
				case 7:
					// モーターテスト
					lcdPosition( 0, 0 );
					lcdPrintf("Motortes");
					lcdPosition( 0, 1 );
					lcdPrintf("     30%%");
					demo = 0;
					if ( motor_test == 1 ) {
						diff( 30 );
						//motor_f( 30, 30 );
						//motor_r( 30, 30 );
					} else {
						motor_f( 0, 0 );
						motor_r( 0, 0 );
					}
					
					data_select( &motor_test, 1 );
					break;
				case 8:
					// サーボテスト
					lcdPosition( 0, 0 );
					lcdPrintf("Servo   ");
					lcdPosition( 0, 1 );
					lcdPrintf("        ");
					if ( motor_test == 1 ) servoPwmOut( 20 );
					else				servoPwmOut( 0 );
					
					data_select( &motor_test, 1 );
					break;
				case 9:
					// 関数テスト
					lcdPosition( 0, 0 );
					lcdPrintf("gASensor");
					
					motor_test = 0;
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						lcdPrintf("   %5d", getAnalogSensor());
					}
					break;
					
				case 10:
					// Bluetooth
					lcdPosition( 0, 0 );
					lcdPrintf("Text   %d", revErr);
					lcdPosition( 0, 1 );
					lcdPrintf("%s",txt_data);
					break;
					
				case 11:
					// 旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("IMU %4d", (short)TurningAngleIMU);
					lcdPosition( 0, 1 );
					lcdPrintf("Enc %4d", (short)TurningAngleEnc);
					if ( tasw_get() == 0x1 ) TurningAngleEnc = 0;
					if ( tasw_get() == 0x2 ) TurningAngleIMU = 0;
					break;
					
				case 12:
					// IMU
					if ( cnt_setup >= 500 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("xa%2.3f",(double)rawXa / ACCELLSB);
						lcdPosition( 0, 1 );
						lcdPrintf("ya%2.3f",(double)rawYa / ACCELLSB);
					}
					break;
					
				case 13:
					// IMU
					if ( cnt_setup >= 500 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("za%2.3f",(double)rawZa / ACCELLSB);
						lcdPosition( 0, 1 );
						lcdPrintf("xg%2.3f",(double)rawXg/GYROLSB);
					}
					break;
					
				case 14:
					// IMU
					if ( cnt_setup >= 500 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("yg%2.3f",(double)rawYg/GYROLSB);
						lcdPosition( 0, 1 );
						lcdPrintf("zg%2.3f",(double)rawZg/GYROLSB);
					}
					break;
					
				case 15:
					// who am i
					if ( cnt_setup >= 500 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("wai 0x%x", whoami);
						lcdPosition( 0, 1 );
						lcdPrintf("Temp%2.1f", (double)TempIMU);
						if ( tasw_get() == 0x1 ) {
							wait_lcd(1000);
							caribrateIMU();
						}
					}
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0xc】位置固定デモ
		//------------------------------------------------------------------
		case 0xc:
			lcdPosition( 0, 0 );
			lcdPrintf("DEMO%4d",motorPwm);
			targetSpeed  = 0;
			
			data_select ( &demo, 1 );
			if ( demo == 1 ) {
				motor_f( motorPwm, motorPwm );
				motor_r( motorPwm, motorPwm );
				lcdPosition( 0, 1 );
				lcdPrintf("   Start");
			} else {
				lcdPosition( 0, 1 );
				lcdPrintf("    Stop");
			}
			break;
		//------------------------------------------------------------------
		// 【0xd】MicroSD
		//------------------------------------------------------------------
		case 0xd:
			lcdPosition( 0, 0 );
			lcdPrintf("MicroSD%d", msdset);
			
			servo_test = 0;
			angle_mode = 0;
			data_tuning ( &pattern_msd, 1, LEFT );
			if ( pattern_msd == 13 ) pattern_msd = 1;
			else if ( pattern_msd == 0 ) pattern_msd = 12;
			
			switch ( pattern_msd ) {
				case 1:
					msdWorkaddress = msdaddrBuff[1];
					msdWorkaddress2 = msdaddrBuff[0];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("1       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 2:
					msdWorkaddress = msdaddrBuff[3];
					msdWorkaddress2 = msdaddrBuff[2];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("2       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 3:
					msdWorkaddress = msdaddrBuff[5];
					msdWorkaddress2 = msdaddrBuff[4];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("3       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 4:
					msdWorkaddress = msdaddrBuff[7];
					msdWorkaddress2 = msdaddrBuff[6];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("4       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 5:
					msdWorkaddress = msdaddrBuff[9];
					msdWorkaddress2 = msdaddrBuff[8];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("5       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 6:
					msdWorkaddress = msdaddrBuff[11];
					msdWorkaddress2 = msdaddrBuff[10];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("6       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 7:
					msdWorkaddress = msdaddrBuff[13];
					msdWorkaddress2 = msdaddrBuff[12];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("7       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 8:
					msdWorkaddress = msdaddrBuff[15];
					msdWorkaddress2 = msdaddrBuff[14];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("8       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 9:
					msdWorkaddress = msdaddrBuff[17];
					msdWorkaddress2 = msdaddrBuff[16];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("9       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 10:
					msdWorkaddress = msdaddrBuff[19];
					msdWorkaddress2 = msdaddrBuff[18];
					if ( msdWorkaddress == 0 && msdWorkaddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("10      ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
					
				case 11:
				// ログ記録
					if ( msdFlag == 0 ) { 
						lcdPosition( 0, 1 );
						lcdPrintf("LogWrite");
					}
					if ( tasw_get() == 0x1 && push1 == 0 && msdFlag == 0) {
						push1 = 1;
						readFlashSetup( 0, 0, 1 ,0 ,0 ,0 ,0);
						init_log();	// ログ記録準備
						msdFlag = 1;		// データ記録開始
						lcdPosition( 0, 1 );
						lcdPrintf("Logging ");
					} else if ( tasw_get() == 0x2 && push1 == 0 && msdFlag == 1) {
						push1 = 1;
						msdEndLog();		// MicroSDの終了処理
					} else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
					
				case 12:
				// コース解析
					if ( msdFlag == 0 ) { 
						lcdPosition( 0, 1 );
						lcdPrintf("LogRead ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 && msdFlag == 0) {
						//ログ解析
						msdgetData () ;
						setBeepPatternS( 0xc000 );
					} else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0xe】キャリブレーション
		//------------------------------------------------------------------
		case 0xe:
			lcdPosition( 0, 0 );
			lcdPrintf("Angle0  ");
			lcdPosition( 0, 1 );
			lcdPrintf("    %4d", Angle0);
			
			flashDataBuff[0] = Angle0;
			data_tuning ( &Angle0, 1, RIGHT );
			data_select( &servo_test, 8 );
			angle_mode = 1;
			if ( servo_test == 1 ) {
				SetAngle = 0;
				servoPwmOut( ServoPwm2 );
			} else {
				servoPwmOut( 0 );
			}
			if ( tasw_get() == 0x4 && push1 == 0 ) {
				push1 = 1;
				writeFlashData( ANGLE0_STARTAREA, ANGLE0_ENDAREA, ANGLE0_DATA, 1 );
			} else if ( tasw_get() == 0x0 ) {
				push1 = 0;
			}
			break;
		//------------------------------------------------------------------
		// 【0xf】フラッシュ
		//------------------------------------------------------------------
		case 0xf:
			switch( pattern_flash ) {
				case 1:
					lcdPosition( 0, 0 );
					lcdPrintf("Flash   ");
					lcdPosition( 0, 1 );
					lcdPrintf("AllErase");
					
					if ( tasw_get() == 1 && push1 == 0 ) {
						push1 = 1;
						pattern_flash = 2;
						break;
					} else if ( tasw_get() == 8 && push1 == 0 ) {
						push1 = 1;
						ble = 1;
						break;
					} else {
						push1 = 0;
					}
					break;
				case 2:
					lcdPosition( 0, 0 );
					lcdPrintf("Really? ");
					lcdPosition( 0, 1 );
					lcdPrintf("c:Y 2:N ");
					
					if ( tasw_get() == 2 ) {
						pattern_flash = 1;
						break;
					}
					if ( tasw_get() == 0xc ) {
						pattern_flash = 3;
						break;
					}
					break;
				case 3:
					lcdPosition( 0, 0 );
					lcdPrintf("AllErase");
					lcdPosition( 0, 1 );
					lcdPrintf("     Now");
					s = 0;
					while ( s <= 1023 ) {
						eraseE2DataFlash( s );
						s++;
					}
					cnt_setup = 0;
					pattern_flash = 4;
					break;
				case 4:
					lcdPosition( 0, 1 );
					lcdPrintf("complete");
					if ( cnt_setup >= 1500 ) {
						pattern_flash = 1;
						break;
					}
					break;
			}
			break;
	default:
		lcdPosition( 0, 0 );
		lcdPrintf("       ");
		lcdPosition( 0, 1 );
		lcdPrintf("none    ");
		
		servo_test = 0;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
// モジュール名 data_select										//
// 処理概要     タクトスイッチで0,1に変化させる						//
// 引数         data: 変化させる変数 button: どのスイッチで変化させるか		//
// 戻り値       なし											//
///////////////////////////////////////////////////////////////////////////////////////
void data_select ( char *data , char button )
{
	if ( tasw_get() == button ) {
		if ( *data == 1 && push == 0) {
			push = 1;
			*data = 0;
		} else if ( *data == 0 && push == 0) {
			push = 1;
			*data = 1;
			Int = 0;
		}
	} else {
		push = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
// モジュール名 data_tuning										//
// 処理概要     タクトスイッチでdataを加減する						//
// 引数         data: 加減させる変数 add: 0: 変化量 lr: 0:右列 1:左列			//
// 戻り値       なし											//
///////////////////////////////////////////////////////////////////////////////////////
void data_tuning ( void *data, char add , char lr )
{
	short *data2 = (short*)data;	// short型ポインタにキャスト
	
	if ( lr == LEFT ) { //左側列　
		if ( tasw_get() == 0x4 || tasw_get() == 0x8 ) {
			if ( tasw_get() == 0x4 && pushL == 0 ) {
				pushL = 1;
				*data2 += add;
			} else if ( tasw_get() == 0x8 && pushL == 0 ) {
				pushL = -1;
				*data2 -= add;
			} else if ( pushL != 0 ) {
				// 長押しモード
				if ( cnt_swL >= 500 ) {
					if ( ( cnt_setup3 % 200 ) == 0 ) {
						cnt_setup3 = 0;
						if ( pushL == 1) *data2 += add;
						else *data2 -= add;
					}
				}
			}
		} else {
			pushL = 0;
			cnt_swL = 0;
		}
	} else {
		if ( tasw_get() == 0x1 || tasw_get() == 0x2 ) {
			if ( tasw_get() == 0x2 && pushR == 0 ) {
				pushR = 1;
				*data2 += add;
			} else if ( tasw_get() == 0x1 && pushR == 0 ) {
				pushR = -1;
				*data2 -= add;
			} else if ( pushR != 0 ) {
				// 長押しモード
				if ( cnt_swR >= 500 ) {
					if ( ( cnt_setup3 % 200 ) == 0 ) {
						cnt_setup3 = 0;
						if ( pushR == 1) *data2 += add;
						else *data2 -= add;
					}
				}
			}
		} else {
			pushR = 0;
			cnt_swR = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////
// モジュール名 fix_speedsetting								//
// 処理概要     速度パラメータを固定値にする						//
// 引数         なし										//
// 戻り値       0: 速度一定にしない　1: 速度一定にする				//
/////////////////////////////////////////////////////////////////////////////////
char fix_speedsetting ( void )
{
	char ret = 0;
	
	if ( setting_1meter == 1 ) {
		speed_straight		= 10;
		speed_curve_brake	= 10;
		speed_curve_r600	= 10;
		speed_curve_r450	= 10;
		speed_curve_straight	= 10;
		
		speed_crossline		= 10;
		speed_ckank_trace	= 10;
		speed_rightclank_curve	= 10;
		speed_rightclank_escape	= 10;
		speed_leftclank_curve	= 10;
		speed_leftclank_escape	= 10;
		
		speed_halfine		= 10;
		speed_rightchange_trace = 10;
		speed_rightchange_curve	= 10;
		speed_rightchange_escape= 10;
		speed_leftchange_trace 	= 10;
		speed_leftchange_curve	= 10;
		speed_leftchange_escape	= 10;
		
		speed_slope_brake	= 10;
		speed_slope_trace	= 10;
		
		fixSpeed = 1;
		ret = 1;
	} else if ( setting_2meter == 1 ) {
		speed_straight		= 20;
		speed_curve_brake	= 20;
		speed_curve_r600	= 20;
		speed_curve_r450	= 20;
		speed_curve_straight	= 20;
		
		speed_crossline		= 20;
		speed_ckank_trace	= 20;
		speed_rightclank_curve	= 20;
		speed_rightclank_escape	= 20;
		speed_leftclank_curve	= 20;
		speed_leftclank_escape	= 20;
		
		speed_halfine		= 20;
		speed_rightchange_trace = 20;
		speed_rightchange_curve	= 20;
		speed_rightchange_escape= 20;
		speed_leftchange_trace 	= 20;
		speed_leftchange_curve	= 20;
		speed_leftchange_escape	= 20;
		
		speed_slope_brake	= 20;
		speed_slope_trace	= 20;
		
		fixSpeed = 1;
		ret = 1;
	} else if ( setting_3meter == 1 ) {
		speed_straight		= 30;
		speed_curve_brake	= 30;
		speed_curve_r600	= 30;
		speed_curve_r450	= 30;
		speed_curve_straight	= 30;
		
		speed_crossline		= 25;
		speed_ckank_trace	= 27;
		speed_rightclank_curve	= 22;
		speed_rightclank_escape	= 30;
		speed_leftclank_curve	= 22;
		speed_leftclank_escape	= 30;
		
		speed_halfine		= 30;
		speed_rightchange_trace = 30;
		speed_rightchange_curve	= 30;
		speed_rightchange_escape= 30;
		speed_leftchange_trace 	= 30;
		speed_leftchange_curve	= 30;
		speed_leftchange_escape	= 30;
		
		speed_slope_brake	= 20;
		speed_slope_trace	= 30;
		
		fixSpeed = 1;
		ret = 1;
	}
	
	return ret;
}