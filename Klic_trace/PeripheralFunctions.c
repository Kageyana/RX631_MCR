//======================================//
// インクルード                         //
//======================================//
#include "R_PG_RX631_mcr_ver3.0.h"
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "iodefine.h"
#include "SCI.h"
#include "I2C_MPU-9255.h"
#include <stdio.h>
#include <string.h>

//======================================//
// グローバル変数の宣言                 //
//======================================//
// タイマ関連
// 100usタイマ
unsigned int		cnt0;		// 関数用タイマカウント
unsigned int 		cnt1;		// 走行用タイマカウント
unsigned short	 	cnt_out;	// コースアウト判定用タイマカウント
unsigned short	 	cnt_out2;	// コースアウト判定用タイマカウント2
unsigned short	 	cnt_out3;	// コースアウト判定用タイマカウント3
unsigned short	 	cnt_out4;	// コースアウト判定用タイマカウント4
unsigned short 		cnt_setup;	// セットアップで使用
unsigned short 		cnt_setup2;	// セットアップで使用
static char		ADTimer10;	// AD変換カウント用
// 1msタイマ
unsigned short		cnt_flash;	// フラッシュ用タイマカウント
unsigned short 		cnt_gyro;	// 角度計算用タイマカウント
static char		Timer10;	// 1msカウント用
short			cnt_swR;	// スイッチ長押し判定用右
short			cnt_swL;	// スイッチ長押し判定用左
// 2msタイマ
unsigned int		cnt_log = 0;	// ログ漏れ用カウント

// スイッチ関連
static unsigned char 	dpsw_d[4];	// ディップスイッチの格納先
static unsigned char	tasw_d[4];	// スイッチ値の格納先

// センサ関連
static unsigned short 	result[21]; 	// 12bitA/D変換結果の格納先
static short		senR[10];	// 右アナログセンサAD値
static short		senL[10];	// 左アナログセンサAD値
static short		senG[10];	// ゲートセンサAD値
static short		senC[10];	// 中心アナログセンサAD値
static short		senLL[10];	// 最左端アナログセンサAD値
static short		senRR[10];	// 最右端アナログセンサAD値
static short		gy[10];		// ジャイロセンサAD値
static short		pot[10];	// ポテンションメーターAD値
short			sensorR;	// 右アナログセンサAD値平均値
short			sensorL;	// 左アナログセンサAD値平均値
short			sensorG;	// ゲートセンサAD値平均値
short			sensorC;	// 中心アナログセンサAD値平均値
short			sensorLL;	// 最左端アナログセンサAD値平均値
short			sensorRR;	// 最右端アナログセンサAD値平均値
short			Angle0;		// サーボセンター値

// ブザー関連
unsigned short 		BeepPattern;
unsigned short 		BeepTimer;
static char		BeepMode;

// エンコーダ関連
static unsigned short 	cnt_Encoder;	// エンコーダ値の格納先
static unsigned short	encbuff;	// 前回のエンコーダ値
short			Encoder;	// 1msごとのパルス
unsigned int		EncoderTotal;	// 総走行距離
unsigned int		enc1;		// 走行用距離カウント
unsigned int		enc_slope;	// 坂上距離カウント

// モーター関連
signed char		accele_fR;	// 右前モーターPWM値
signed char		accele_fL;	// 左前モーターPWM値
signed char		accele_rR;	// 右後モーターPWM値
signed char		accele_rL;	// 左後モーターPWM値
signed char		sPwm;		// サーボモーターPWM値

//////////////////////////////////////////////////////////////////////////
// モジュール名 Timer							//
// 処理概要     1msごとにタイマ割り込み					//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void Timer (void) {
	short s,i;
	
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
		if ( slope_mode != 0 || slope_mode != 1 ) {
			if ( check_slope() == -1 ) cnt_out4++;
			else	cnt_out4 = 0;
		}
	} else if ( pattern < 11 ) {
		cnt0++;
		cnt_setup++;
		cnt_setup2++;
		cnt_flash++;
	}
	cnt1++;
	cnt_swR++;
	cnt_swL++;
			
	// LCD表示
	if ( lcd_mode ) {
		lcdShowProcess();
	}

	// エンコーダカウント
	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );
	Encoder = cnt_Encoder - encbuff;
	EncoderTotal += Encoder;
	enc1 += Encoder;
	enc_slope += Encoder;
	encbuff = cnt_Encoder;

	// PID制御値算出
	if ( angle_mode == 0 ) {
		servoControl();
	} else {
		servoControl2();
	}
	motorControl();
	
	// 角度計算
	get_degrees();
	get_TurningAngle();

	// MicroSD書き込み
	microSDProcess();
	if( msdFlag == 1 ) {
		msdTimer++;
		if( msdTimer == WRITINGTIME ) {
			msdTimer = 0;
			msdBuffPointa = msdBuff + msdBuffAddress;
			send_Char	(	pattern		);
			send_Char	(	motorPwm 	);
			send_Char	(	accele_fL 	);
			send_Char	(	accele_fR 	);
			send_Char	(	accele_rL 	);
			send_Char	(	accele_rR 	);
			send_Char	(	sPwm	 	);
			send_Char	(	ServoPwm 	);
			send_Char	(	ServoPwm2 	);
			send_Char	(	sensor_inp() 	);
			send_Char	( 	slope_mode	);
			send_Char	( 	msdlibError	);
			send_ShortToChar(	getServoAngle()	);
			send_ShortToChar(	SetAngle	);
			send_ShortToChar(	getAnalogSensor());
			send_ShortToChar(	sensorL		);
			send_ShortToChar(	sensorR		);
			send_ShortToChar(	getGyro()	);
			send_ShortToChar(	Degrees		);
			send_ShortToChar(	TurningAngle	);
			send_ShortToChar(	Encoder		);
			send_ShortToChar(	target_speed/10	);
			send_uIntToChar (	EncoderTotal	);
			send_uIntToChar (	enc1		);
			send_uIntToChar (	cnt_log		);
			cnt_log += WRITINGTIME;
			msdBuffAddress += DATA_BYTE;       // RAMの記録アドレスを次へ
			if( msdBuffAddress >= 512 ) {
				msdBuffAddress = 0;
				setMicroSDdata( msdBuff ); 
				msdWorkAddress += 512;
				if( msdWorkAddress >= msdEndAddress ) {
					msdFlag = 0;
				}
			}
		}
	}

	// UART受信
	if ( strcmp( txt_data, txt_stop) == 0 ) {
		stopWord = 1;
	}
	
	if ( IMUSet == 0 ) {
		if ( commandEnd == 1 ) {	// コマンド終了時に実行
			if ( cmderr == 1 ) {
				printf("commandERROR\n");
				commandEnd = 0;
			} else {
				switch ( cmmandMode ) {
				case 1:
					// ボーレート設定(br)
					for ( s = 0; s < 15; s++ ) {
						i = ascii_num[s];
						if ( txt_command[0] == i ) {
							printf("br=%d\n", s);
							init_SCI1( s );
							break;
						}
					}
					break;
					
				default:
					break;
				}
				commandEnd = 0;
			}
		}
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
		// タクトスイッチ読み込み
		R_PG_IO_PORT_Read_PC4(&tasw_d[1]);	// タクトスイッチ右上
		R_PG_IO_PORT_Read_PC5(&tasw_d[0]);	// タクトスイッチ右下
		R_PG_IO_PORT_Read_PC6(&tasw_d[2]);	// タクトスイッチ左上
		R_PG_IO_PORT_Read_P50(&tasw_d[3]);	// タクトスイッチ左下
		break;
	case 3:
		// ディップスイッチ読み込み
		R_PG_IO_PORT_Read_PC3(&dpsw_d[0]);
		R_PG_IO_PORT_Read_PC2(&dpsw_d[1]);
		R_PG_IO_PORT_Read_PC1(&dpsw_d[2]);
		R_PG_IO_PORT_Read_PC0(&dpsw_d[3]);
		break;
	case 5:
		if (SCI1.SSR.BIT.ORER) {
			reverr = 1;
		} else if (SCI1.SSR.BIT.FER) {
			reverr = 2;
		} else if (SCI1.SSR.BIT.PER) {
			reverr = 3;
		} else {
			//reverr = 0;
		}
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
//////////////////////////////////////////////////////////////////////////
// モジュール名 ADconverter						//
// 処理概要     AD変換割り込み						//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void ADconverter ( void )
{
	__setpsw_i();
	R_PG_ADC_12_GetResult_S12AD0( result );
	
	ADTimer10++;
	if ( ADTimer10 == 10 ) {
		ADTimer10 = 0;
	}
	
	// AD変換値をバッファに格納
	senLL[ADTimer10] = result[3];
	senL[ADTimer10] = result[4];
	senG[ADTimer10] = result[5];
	senC[ADTimer10] = result[6];
	senR[ADTimer10] = result[7];
	senRR[ADTimer10] = result[8];
	gy[ADTimer10] = result[12];
	pot[ADTimer10] = result[13];
	
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 delay	                                                //
// 処理概要     遅延処理 1 = 1ms					//
// 引数         delaytime						//
// 戻り値       なし                                                    //
//////////////////////////////////////////////////////////////////////////
void delay(unsigned short delaytime)
{
	cnt0 = 0;
	while( cnt0 <= delaytime ) {
		R_PG_IO_PORT_Write_P27(0);
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 dipsw_get						//
// 処理概要     ディップスイッチ値を16進数で取得			//
// 引数         なし							//
// 戻り値       スイッチ値 0～15					//
//////////////////////////////////////////////////////////////////////////
unsigned char dipsw_get( void ) 
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
//////////////////////////////////////////////////////////////////////////
// モジュール名 led_out							//
// 処理概要     LEDの点灯						//
// 引数         led(対応するLED番号)					//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void led_out ( unsigned char led )
{
	unsigned char led2;

	led2 = led << 1;
	R_PG_IO_PORT_Write_P5( led2 );
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 tasw_get						//
// 処理概要     タクトスイッチ値を16進数で取得				//
// 引数         なし							//
// 戻り値       スイッチ値 0～7						//
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
// モジュール名 getGyro						//
// 処理概要     ジャイロセンサのアナログ値で取得			//
// 引数         なし							//
// 戻り値       センサ値						//
//////////////////////////////////////////////////////////////////////////
short getGyro(void) 
{
	short gyro;
	// 平均値の計算
	gyro =  ( gy[0] + gy[1] + gy[2] + gy[3] + gy[4] + gy[5] + gy[6] + gy[7] + gy[8] + gy[9] ) / 10;
	
	return ( gyro - 1796 );
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 getServoAngle						//
// 処理概要     ポテンションメーターのアナログ値で取得			//
// 引数         なし							//
// 戻り値       センサ値						//
//////////////////////////////////////////////////////////////////////////
short getServoAngle(void) 
{	
	short Angle;
	
	// 平均値の計算
	Angle =  ( pot[0] + pot[1] + pot[2] + pot[3] + pot[4] + pot[5] + pot[6] + pot[7] + pot[8] + pot[9] ) / 10;
	
	return  ( Angle - Angle0 );
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 getAnalogSensor						//
// 処理概要     アナログセンサのアナログ値で取得			//
// 引数         なし							//
// 戻り値       センサ値						//
//////////////////////////////////////////////////////////////////////////
short getAnalogSensor(void) 
{
	// 平均値の計算
	sensorR = ( senR[0] + senR[1] + senR[2] + senR[3] + senR[4] + senR[5] + senR[6] + senR[7] + senR[8] + senR[9] ) / 10;
	//sensorR >>= 1;
	sensorL = ( senL[0] + senL[1] + senL[2] + senL[3] + senL[4] + senL[5] + senL[6] + senL[7] + senL[8] + senL[9] ) / 10;
	//sensorL += 100;

	return sensorR - sensorL;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 sensor_inp						//
// 処理概要     デジタルセンサの値を16進数で取得			//
// 引数         なし							//
// 戻り値       センサ値0～7						//
//////////////////////////////////////////////////////////////////////////
unsigned char sensor_inp(void) 
{
	char l, c, r;
	
	sensorC = ( senC[0] + senC[1] + senC[2] + senC[3] + senC[4] + senC[5] + senC[6] + senC[7] + senC[8] + senC[9] ) / 10;
	sensorLL = ( senLL[0] + senLL[1] + senLL[2] + senLL[3] + senLL[4] + senLL[5] + senLL[6] + senLL[7] + senLL[8] + senLL[9] ) / 10;
	sensorRR = ( senRR[0] + senRR[1] + senRR[2] + senRR[3] + senRR[4] + senRR[5] + senRR[6] + senRR[7] + senRR[8] + senRR[9] ) / 10;
	
	if (sensorRR < 3000 ) r = 0x1;
	else r = 0;
	if (sensorC < 2000 ) c = 0x2;
	else c = 0;
	if (sensorLL < 3000 ) l = 0x4;
	else l = 0;
	
	return l+c+r;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 startbar_get						//
// 処理概要     スタートゲートの開閉の確認				//
// 引数         なし							//
// 戻り値       0; 閉じている 1; 開いている				//
//////////////////////////////////////////////////////////////////////////
unsigned char startbar_get(void) 
{
	char ret;
		
	sensorG = ( senG[0] + senG[1] + senG[2] + senG[3] + senG[4] + senG[5] + senG[6] + senG[7] + senG[8] + senG[9] ) / 10;
	if ( sensorG <= 2000 )	ret = 1;
	else			ret = 0;
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 init_BeepS						//
// 処理概要     ブザー関連初期化					//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void init_BeepS( void )
{
    BeepPattern = 0x0000;
    BeepTimer   = 0;
    BeepMode    = 0;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 setBeepPatternS						//
// 処理概要     ブザー出力パターンセット				//
// 引数         Beep:鳴動パターン					//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void setBeepPatternS( unsigned short Beep )
{
    BeepPattern = Beep;
    BeepTimer   = 0;
    BeepMode    = 1;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 beepProcessS						//
// 処理概要     ブザー処理						//
// 引数         なし							//
// 戻り値       なし							//
// メモ		この関数を1ms毎に実行してください			//
//////////////////////////////////////////////////////////////////////////
void beepProcessS( void )
{
	if ( BeepMode ) {
		if ( BeepTimer % 5 == 0 ) {
			if ( BeepPattern & 0x8000 ) {
				R_PG_IO_PORT_Write_P23(1);
			} else {
				R_PG_IO_PORT_Write_P23(0);
			}
			BeepPattern <<= 1;
		}
		BeepTimer++;
		if ( BeepTimer == 17 * 5 ) {
			BeepMode = 0;
			R_PG_IO_PORT_Write_P23(0);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 motor_f							//
// 処理概要     モーターのPWMの変更					//
// 引数         accelefL, accelefR(PWMを1～100%で指定)			//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void motor_f( signed char accelefL, signed char accelefR ){
	uint16_t pwmfl, pwmfr;
	
	if ( pushcart_mode ) {
		accelefR = 0;
		accelefL = 0;
	}
	
	accele_fR = accelefR;
	accele_fL = accelefL;
	
	pwmfl = TGR_MOTOR * accelefL / 100;
	pwmfr = TGR_MOTOR * accelefR / 100;
	
	if( accelefL >= 0) {					// 正転
		R_PG_IO_PORT_Write_PB2( 0 );
		R_PG_Timer_SetTGR_D_MTU_U0_C0( pwmfl );
	} else if ( accelefL == 100 || accelefL == -100 ) {	// 100%
		if ( accelefL > 0 ) {
			R_PG_IO_PORT_Write_PB2( 0 );
		} else {
			R_PG_IO_PORT_Write_PB2( 1 );
		}
		R_PG_Timer_SetTGR_D_MTU_U0_C0( TGR_MOTOR + 2 );
	} else {						// 逆転
		R_PG_IO_PORT_Write_PB2( 1 );
		R_PG_Timer_SetTGR_D_MTU_U0_C0( -pwmfl );
	}
	
	if( accelefR >= 0) {					// 正転
		R_PG_IO_PORT_Write_PB4( 0 );
		R_PG_Timer_SetTGR_B_MTU_U0_C0( pwmfr );
	} else if ( accelefR == 100 || accelefR == -100 ) {	// 100%
		if ( accelefR > 0 ) {
			R_PG_IO_PORT_Write_PB4( 0 );
		} else {
			R_PG_IO_PORT_Write_PB4( 1 );
		}
		R_PG_Timer_SetTGR_B_MTU_U0_C0( TGR_MOTOR + 2 );
	} else {						// 逆転
		R_PG_IO_PORT_Write_PB4( 1 );
		R_PG_Timer_SetTGR_B_MTU_U0_C0( -pwmfr );
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 motor_r							//
// 処理概要     モーターのPWMの変更					//
// 引数         acelerL, accelerR(PWMを1～100%で指定)			//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void motor_r( signed char accelerL, signed char accelerR ){
	uint16_t pwmrl, pwmrr;
	
	if ( pushcart_mode ) {
		accelerL = 0;
		accelerR = 0;
	}
	
	accele_rR = accelerR;
	accele_rL = accelerL;
	
	pwmrl = TGR_MOTOR * accelerL / 100;
	pwmrr = TGR_MOTOR * accelerR / 100;
	
	if( accelerL >= 0 ) {					// 正転
		R_PG_IO_PORT_Write_P20( 0 );
		R_PG_Timer_SetTGR_B_MTU_U0_C3( pwmrl );
	} else if ( accelerL == 100 || accelerL == -100 ) {	// 100%
		if (accelerL > 0) {
			R_PG_IO_PORT_Write_P20( 0 );
		} else {
			R_PG_IO_PORT_Write_P20( 1 );
		}
		R_PG_Timer_SetTGR_B_MTU_U0_C3( TGR_MOTOR + 2 );
	} else {						// 逆転
		R_PG_IO_PORT_Write_P20( 1 );
		R_PG_Timer_SetTGR_B_MTU_U0_C3( -pwmrl );
	}
	
	if( accelerR >= 0 ) {					// 正転
		R_PG_IO_PORT_Write_P15( 0 );
		R_PG_Timer_SetTGR_D_MTU_U0_C3( pwmrr );
	} else if ( accelerR == 100 || accelerR == -100 ) {	// 100%
		if ( accelerR > 0 ) {
			R_PG_IO_PORT_Write_P15( 0 );
		} else {
			R_PG_IO_PORT_Write_P15( 1 );
		}
		R_PG_Timer_SetTGR_D_MTU_U0_C3( TGR_MOTOR + 2 );
	} else {						// 逆転
		R_PG_IO_PORT_Write_P15( 1 );
		R_PG_Timer_SetTGR_D_MTU_U0_C3( -pwmrr );
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 servoPwmOut						//
// 処理概要     白線トレース時サーボのPWMの変更				//
// 引数         spwm							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void servoPwmOut( signed char servopwm )
{
	uint16_t pwm;
	short angle;
	
	sPwm = servopwm;
	
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
	if( servopwm > 0) {				// 正転
		R_PG_IO_PORT_Write_PB6( 0 );
		R_PG_Timer_SetTGR_B_MTU_U0_C2( pwm );
	} else {				// 逆転
		R_PG_IO_PORT_Write_PB6( 1 );
		R_PG_Timer_SetTGR_B_MTU_U0_C2( -pwm );
	}
}