//====================================//
// インクルード									//
//====================================//
#include "SetUp.h"
//====================================//
// グローバル変数の宣言								//
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
// プロトタイプ宣言									//
//====================================//
void data_select ( char *data , char button );
void data_tuning ( void *data, char add , char direction );
void wait2 ( int waittime );

char ble;
///////////////////////////////////////////////////////////////
// モジュール名 setup							//
// 処理概要     走行前設定						//
// 引数         なし								//
// 戻り値       なし 							//
///////////////////////////////////////////////////////////////
void setup( void )
{
	char cnt_led;
	
	// ディップスイッチで項目選択
	switch ( dipsw_get() ) {
		//------------------------------------------------------------------
		// 【0x0】スタート待ち
		//------------------------------------------------------------------
		case 0x0:
			lcdPosition( 0, 0 );
			lcdPrintf("START   ");
			lcdPosition( 0, 1 );
			lcdPrintf("STOP %2dm", stopping_meter );
			
			// ゲート検出
			if ( startbar_get() ) led_out(LED_B);
			else 	led_out(LED_R);
			// 停止距離入力
			data_tuning ( &stopping_meter, 1, UD );
			
			angle_mode = 0;
			data_select( &servo_test, SW_RIGHT );
			if ( servo_test == 1 ) servoPwmOut( ServoPwm );
			else servoPwmOut( 0 );
			
			// プッシュスイッチ押下待ち
			if ( tasw_get() == SW_PUSH ) {
				start = START_COUNT;
			} else if ( tasw_get() == SW_LEFT ) {
				start = START_GATE;
			} else if ( tasw_get() == SW_RIGHT ) {
				pushcart_mode = 1;
				start = START_COUNT;
			}
			
			break;
		//------------------------------------------------------------------
		// 【0x1】手押しモード選択
		//------------------------------------------------------------------
		case 0x1:
			lcdPosition( 0, 0 );
			lcdPrintf("Voltage ");
			lcdPosition( 0, 1 );
			lcdPrintf("  %05.2fV",Voltage);
			break;
		//------------------------------------------------------------------
		// 【0x3】パラメータ調整(通常トレース)
		//------------------------------------------------------------------
		case 0x2:
			data_tuning ( &pattern_parameter, 1, LR );
			
			if ( pattern_parameter == 6 ) pattern_parameter = 1;
			else if ( pattern_parameter == 0 ) pattern_parameter = 5;
			
			switch( pattern_parameter ) {
				case 1:
					// 通常走行速度
					lcdPosition( 0, 0 );
					lcdPrintf("STRAIGHT");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_straight / 10 );
					
					data_tuning ( &speed_straight, 1, UD );
					break;
			}
			break;
			
		//------------------------------------------------------------------
		// 【0x4】パラメータ調整(クランク)
		//------------------------------------------------------------------
		case 0x3:
			data_tuning ( &pattern_parameter2, 1, LR );
			
			if ( pattern_parameter2 == 7 ) pattern_parameter2 = 1;
			else if ( pattern_parameter2 == 0 ) pattern_parameter2 = 6;
			
			switch( pattern_parameter2 ) {
				case 1:
					// クロスライン通過時
					lcdPosition( 0, 0 );
					lcdPrintf("CROSLINE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_crossline / 10 );
					
					data_tuning ( &speed_crossline, 1, UD );
					break;
				case 2:
					// クランク前トレース
					lcdPosition( 0, 0 );
					lcdPrintf("CL_TRACE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_ckank_trace / 10 );
					
					data_tuning ( &speed_ckank_trace, 1, UD );
					break;
				case 3:
					// 右クランク旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("RCL_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightclank_curve / 10 );
					
					data_tuning ( &speed_rightclank_curve, 1, UD );
					break;
				case 4:
					// 右クランク復帰速度
					lcdPosition( 0, 0 );
					lcdPrintf("RCL_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightclank_escape / 10 );
					
					data_tuning ( &speed_rightclank_escape, 1, UD );
					break;
					
				case 5:
					// 左クランク旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("LCL_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftclank_curve / 10 );
					
					data_tuning ( &speed_leftclank_curve, 1, UD );
					break;
					
				case 6:
					// 左クランク復帰速度
					lcdPosition( 0, 0 );
					lcdPrintf("LCL_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftclank_escape / 10 );
					
					data_tuning ( &speed_leftclank_escape, 1, UD );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0x5】パラメータ調整(レーンチェンジ)
		//------------------------------------------------------------------
		case 0x4:
			data_tuning ( &pattern_parameter3, 1, LR );
			
			if ( pattern_parameter3 == 8 ) pattern_parameter3 = 1;
			else if ( pattern_parameter3 == 0 ) pattern_parameter3 = 7;
			
			switch( pattern_parameter3 ) {
				case 1:
					// ハーフライン通過時
					lcdPosition( 0, 0 );
					lcdPrintf("HALFLINE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_halfine / 10 );
					
					data_tuning ( &speed_halfine, 1, UD );
					break;
				case 2:
					// 右レーンチェンジ進入速度
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_STR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_trace / 10 );
					
					data_tuning ( &speed_rightchange_trace, 1, UD );
					break;
				case 3:
					// 右レーンチェンジ旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_curve / 10 );
					
					data_tuning ( &speed_rightchange_curve, 1, UD );
					break;
				case 4:
					// 右レーンチェンジ復帰速度
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_escape / 10 );
					
					data_tuning ( &speed_rightchange_escape, 1, UD );
					break;
				case 5:
					// 左レーンチェンジ進入速度
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_STR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_trace / 10 );
					
					data_tuning ( &speed_leftchange_trace, 1, UD );
					break;
				case 6:
					// 左レーンチェンジ旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_curve / 10 );
					
					data_tuning ( &speed_leftchange_curve, 1, UD );
					break;
				case 7:
					// 左レーンチェンジ旋回速度
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_escape / 10 );
					
					data_tuning ( &speed_leftchange_escape, 1, UD );
					break;
			}
			break;
		
		//------------------------------------------------------------------
		// 【0x6】パラメータ調整(角度)
		//------------------------------------------------------------------
		case 0x5:
			data_tuning ( &pattern_parameter4, 1, LR );
			
			if ( pattern_parameter4 == 7 ) pattern_parameter4 = 1;
			else if ( pattern_parameter4 == 0 ) pattern_parameter4 = 6;
			
			switch( pattern_parameter4 ) {
				case 3:
					// 右クランク旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_RCR ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_rightclank );
					
					data_tuning ( &angle_rightclank, 1, UD );
					break;
				case 4:
					// 左クランク旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_LCL ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_leftclank );
					
					data_tuning ( &angle_leftclank, 1, UD );
					break;
				case 5:
					// 右レーンチェンジ旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_RLC ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_rightchange );
					
					data_tuning ( &angle_rightchange, 1, UD );
					break;
				case 6:
					// 左レーンチェンジ旋回角度
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_LLC ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_leftchange );
					
					data_tuning ( &angle_leftchange, 1, UD );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0x7】ゲイン調整(サーボ)
		//------------------------------------------------------------------
		case 0x6:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			data_select( &servo_test, SW_PUSH );
			angle_mode = 0;
			if ( servo_test == 1 ) servoPwmOut( ServoPwm );
			else servoPwmOut( 0 );
			
			data_tuning ( &pattern_gain, 1, LR );
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
					
					data_tuning ( &kp_buff, 1, UD );
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
					
					data_tuning ( &ki_buff, 1, UD );
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
					
					data_tuning ( &kd_buff, 1, UD );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0x8】ゲイン調整(角度)
		//------------------------------------------------------------------
		case 0x7:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			data_select( &servo_test2, SW_PUSH );
			angle_mode = 1;
			if ( servo_test == 1 ) servoPwmOut( ServoPwm2 );
			else servoPwmOut( 0 );
			if ( servo_test2 == 1 ) SetAngle = 340;
			else SetAngle = -340;
			
			data_tuning ( &pattern_gain2, 1, LR );
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
					
					data_tuning ( &kp2_buff, 1, UD );
					break;
				case 2:
					// ki2
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) lcdPrintf("%2d    %2d", kp2_buff, kd2_buff);
					else				lcdPrintf("%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					
					data_tuning ( &ki2_buff, 1, UD );
					break;
				case 3:
					// kd2
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 )	lcdPrintf("%2d %2d   ", kp2_buff, ki2_buff);
					else				lcdPrintf("%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					
					data_tuning ( &kd2_buff, 1, UD );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0x9】ゲイン調整(速度)
		//------------------------------------------------------------------
		case 0x8:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			data_tuning ( &pattern_gain3, 1, LR );
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
					
					data_tuning ( &kp3_buff, 1, UD );
					break;
				case 2:
					// ki3
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) lcdPrintf("%2d    %2d", kp3_buff, kd3_buff );
					else				lcdPrintf("%2d %2d %2d", kp3_buff, ki3_buff, kd3_buff );
					
					data_tuning ( &ki3_buff, 1, UD );
					break;
					
				case 3:
					// kd3
					//値を点滅
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) lcdPrintf("%2d %2d   ", kp3_buff, ki3_buff );
					else 				lcdPrintf("%2d %2d %2d", kp3_buff, ki3_buff, kd3_buff );
					
					data_tuning ( &kd3_buff, 1, UD );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 【0xa】プリセットパラメータ
		//------------------------------------------------------------------
		case 0x9:
			data_tuning ( &pattern_speedseting, 1, LR );
				
			if ( pattern_speedseting == 4 ) pattern_speedseting = 1;
			else if ( pattern_speedseting == 0 ) pattern_speedseting = 3;
			
			switch( pattern_speedseting ) {
				case 1:
					lcdPosition( 0, 0 );
					lcdPrintf("1METER %d", setting_1meter);
					lcdPosition( 0, 1 );
					lcdPrintf("SETTING ");
					data_select( &setting_1meter, SW_PUSH );
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
					
					data_select( &setting_2meter, SW_PUSH );
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
					
					data_select( &setting_3meter, SW_PUSH );
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
		case 0xa:
			data_tuning ( &pattern_sensor, 1, LR );
			angle_mode = 0;
			
			if ( pattern_sensor == 8 ) pattern_sensor = 1;
			else if ( pattern_sensor == 0 ) pattern_sensor = 7;
			
			switch( pattern_sensor ) {
				case 1:
					// LED
					lcdPosition( 0, 0 );
					lcdPrintf("LED     ");
					lcdPosition( 0, 1 );
					lcdPrintf("        ");
					cnt_led = 0x00;
					if ( tasw_get() == SW_PUSH) {
						while(1){
						while( cnt_led <= 0x7 ) {
							led_out( cnt_led );
							cnt_led++;
							wait_lcd(200);
 						}
						cnt_led = 0x00;
						}
					}
					break;
					
				case 2:
					// エンコーダ
					lcdPosition( 0, 0 );
					lcdPrintf("Encoder ");
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						lcdPrintf("  %4.1f",(double)EncoderTotal/SPEED_CURRENT);
					}
					break;
							
				case 3:
					// ポテンションメーター
					lcdPosition( 0, 0 );
					lcdPrintf("Angle   ");
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						lcdPrintf("   %4d",getServoAngle());
					}
					break;
					
				case 4:
					// アナログセンサ
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("R   %4d",sensorR);
						lcdPosition( 0,1 );
						lcdPrintf("L   %4d",sensorL);
					}
					break;
					
				case 5:
					// デジタル風センサ, ゲートセンサ
					motor_test = 0;
					data_tuning ( &sensorG_th, 1, UD );
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						startbar_get();
						lcdPosition( 0, 0 );
						lcdPrintf("G%3d%4d", sensorG_th, sensorG);
						lcdPosition( 0, 1 );
						lcdPrintf("D    0x%x", sensor_inp());
					}
					break;
					
				case 6:
					// モーターテスト
					lcdPosition( 0, 0 );
					lcdPrintf("Motortes");
					lcdPosition( 0, 1 );
					lcdPrintf("     30%%");
					if ( motor_test == 1 ) {
						motor_f( 30, 30 );
						motor_r( 30, 30 );
					} else {
						motor_f( 0, 0 );
						motor_r( 0, 0 );
					}
					
					data_select( &motor_test, SW_PUSH );
					break;
				case 7:
					// サーボテスト
					lcdPosition( 0, 0 );
					lcdPrintf("Servo   ");
					lcdPosition( 0, 1 );
					lcdPrintf("        ");
					if ( motor_test == 1 ) servoPwmOut( 20 );
					else				servoPwmOut( 0 );
					
					data_select( &motor_test, SW_PUSH );
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
// 処理概要     タクトスイッチで0,1に変化させる							//
// 引数         data: 変化させる変数 button: どのスイッチで変化させるか			//
// 戻り値       なし												//
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
		}
	} else {
		push = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
// モジュール名 data_tuning										//
// 処理概要     タクトスイッチでdataを加減する							//
// 引数         data: 加減させる変数 add: 0: 変化量 dir: 0:上下 1:左右			//
// 戻り値       なし												//
///////////////////////////////////////////////////////////////////////////////////////
void data_tuning ( void *data, char add , char dir )
{
	short *data2 = (short*)data;	// short型ポインタにキャスト
	
	if ( dir == LR ) { //左右　
		if ( tasw_get() == SW_RIGHT || tasw_get() == SW_LEFT ) {
			if ( tasw_get() == SW_RIGHT && pushL == 0 ) {
				pushL = 1;
				*data2 += add;
			} else if ( tasw_get() == SW_LEFT && pushL == 0 ) {
				pushL = -1;
				*data2 -= add;
			}
		} else {
			pushL = 0;
			cnt_swL = 0;
		}
	} else {	// 上下
		if ( tasw_get() == SW_TOP || tasw_get() == SW_DOWN ) {
			if ( tasw_get() == SW_TOP && pushR == 0 ) {
				pushR = 1;
				*data2 += add;
			} else if ( tasw_get() == SW_DOWN && pushR == 0 ) {
				pushR = -1;
				*data2 -= add;
			}
		} else {
			pushR = 0;
			cnt_swR = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////
// モジュール名 fix_speedsetting								//
// 処理概要     速度パラメータを固定値にする							//
// 引数         なし											//
// 戻り値       0: 速度一定にしない　1: 速度一定にする					//
/////////////////////////////////////////////////////////////////////////////////
char fix_speedsetting ( void )
{
	char ret = 0;
	
	if ( setting_1meter == 1 ) {
		speed_straight		= 10;
		
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
		
		fixSpeed = 1;
		ret = 1;
	} else if ( setting_2meter == 1 ) {
		speed_straight		= 20;
		
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
		
		fixSpeed = 1;
		ret = 1;
	} else if ( setting_3meter == 1 ) {
		speed_straight		= 30;
		
		speed_crossline		= 25;
		speed_ckank_trace	= 20;
		speed_rightclank_curve	= 18;
		speed_rightclank_escape	= 30;
		speed_leftclank_curve	= 18;
		speed_leftclank_escape	= 30;
		
		speed_halfine		= 30;
		speed_rightchange_trace = 30;
		speed_rightchange_curve	= 30;
		speed_rightchange_escape= 30;
		speed_leftchange_trace 	= 30;
		speed_leftchange_curve	= 30;
		speed_leftchange_escape	= 30;
		
		fixSpeed = 1;
		ret = 1;
	}
	
	return ret;
}