#ifndef PERIPHERALFUNCTIONS_H_
#define PERIPHERALFUNCTIONS_H_
//====================================//
// インクルード									//
//====================================//
#include "R_PG_RX631_mcr_ver3.0.h"
//====================================//
// シンボル定義									//
//====================================//
#define SERVO_CENTER		2010		// サーボセンターのAD値
#define SERVO_LIMIT		430		// サーボリミットAD値±
#define TGR_MOTOR			4798		// ジェネラルレジスタ初期値(駆動モータ)
#define TGR_SERVO			2878		// ジェネラルレジスタ初期値(サーボ)
#define PALSE_METER		24750	// 1mのパルス
#define PALSE_MILLIMETER	24.75F	// 1mmのパルス
#define SPEED_CURRENT		25		// 1m/sの時　1msのパルス

/*************************************** 自動生成関数 *************************************/
// タイマ割り込み
#define SET_CMT_C0		R_PG_Timer_Set_CMT_U0_C0();			// コンペアマッチタイマ初期化(ch0)
#define START_CMT_C0	R_PG_Timer_StartCount_CMT_U0_C0();	// カウントスタート(ch0)

// エンコーダ
#define ENCODER_COUNT	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );	// カウント取得

// タクトスイッチ
// デジタルインプット
#define TACTSWITCH2	R_PG_IO_PORT_Read_PC5(&tasw_d[0]);
#define TACTSWITCH1	R_PG_IO_PORT_Read_PC4(&tasw_d[1]);
#define TACTSWITCH3	R_PG_IO_PORT_Read_PC6(&tasw_d[2]);
#define TACTSWITCH4	R_PG_IO_PORT_Read_P50(&tasw_d[3]);
// ディップスイッチ
#define DIPSWITCH1		R_PG_IO_PORT_Read_PC3(&dpsw_d[0]);
#define DIPSWITCH2		R_PG_IO_PORT_Read_PC2(&dpsw_d[1]);
#define DIPSWITCH3		R_PG_IO_PORT_Read_PC1(&dpsw_d[2]);
#define DIPSWITCH4		R_PG_IO_PORT_Read_PC0(&dpsw_d[3]);

// ADコンバータ
#define SET_ADC		R_PG_ADC_12_Set_S12AD0(); 				// 12ビットA/Dコンバータ(S12AD0)を設定
#define START_ADC		R_PG_ADC_12_StartConversionSW_S12AD0();	// A/D変換開始
#define GET_ADC		R_PG_ADC_12_GetResult_S12AD0( result );	// AD値を取得

// ディレイ
#define DELAY			R_PG_IO_PORT_Write_P27(0);		// 未接続の端子を割り当てる

// LED
#define LED_OUT		R_PG_IO_PORT_Write_P5( led2 );	// LED点灯

// ブザー
// デジタルアウト
#define BEEP_ON		R_PG_IO_PORT_Write_P23(1);
#define BEEP_OFF		R_PG_IO_PORT_Write_P23(0);

// モータ
// MTU初期化(PWM機能、エンコーダ)
#define SET_MTU_C0		R_PG_Timer_Set_MTU_U0_C0();	// ch0
#define SET_MTU_C1		R_PG_Timer_Set_MTU_U0_C1();	// ch1
#define SET_MTU_C2		R_PG_Timer_Set_MTU_U0_C2();	// ch2
#define SET_MTU_C3		R_PG_Timer_Set_MTU_U0_C3();	// ch3
// MTU0,1,2,3のカウント開始
#define START_MTU		R_PG_Timer_SynchronouslyStartCount_MTU_U0( 1, 1, 1, 1, 0);

// 左前輪
#define DIR_FL_FOR		R_PG_IO_PORT_Write_PB2( 0 );		// モータ回転方向(正転)
#define DIR_FL_REV		R_PG_IO_PORT_Write_PB2( 1 );		// モータ回転方向(正転)
#define PWM_FL_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C0( pwmfl );	// PWM出力
// 右前輪
#define DIR_FR_FOR		R_PG_IO_PORT_Write_PB4( 0 );
#define DIR_FR_REV		R_PG_IO_PORT_Write_PB4( 1 );
#define PWM_FR_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C0( pwmfr );
// 左後輪
#define DIR_RL_FOR		R_PG_IO_PORT_Write_P20( 0 );
#define DIR_RL_REV		R_PG_IO_PORT_Write_P20( 1 );
#define PWM_RL_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C3( pwmrl );
// 右後輪
#define DIR_RR_FOR		R_PG_IO_PORT_Write_P15( 0 );
#define DIR_RR_REV		R_PG_IO_PORT_Write_P15( 1 );
#define PWM_RR_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C3( pwmrr );
// サーボ
#define DIR_SERVO_FOR	R_PG_IO_PORT_Write_PB6( 0 );
#define DIR_SERVO_REV	R_PG_IO_PORT_Write_PB6( 1 );
#define PWM_SERVO_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C2( pwm );

/******************************************************************************************/

//====================================//
// グローバル変数の宣言							//
//====================================//
// タイマ関連
extern volatile unsigned short		cnt0;		// 関数用タイマ

// センサ関連
extern short		sensorR;		// 右アナログセンサ
extern short		sensorL;		// 左アナログセンサ
extern short		sensorG;		// 左アナログセンサ
extern short		sensorC;		// 中心アナログセンサ
extern short		sensorLL;		// 最左端アナログセンサ
extern short		sensorRR;		// 最右端アナログセンサ

extern short		Angle0;		// サーボセンター値

// エンコーダ関連
extern unsigned int	EncoderTotal;	// 総走行距離
extern signed short	Encoder;		// 1msごとのパルス
extern unsigned int	enc1;		// 走行用距離カウント
extern unsigned int	enc_slope;		// 坂上距離カウント

// モーター関連
extern signed char	accele_fR;		// 右前モーターPWM値
extern signed char	accele_fL;		// 左前モーターPWM値
extern signed char	accele_rR;		// 右後モーターPWM値
extern signed char	accele_rL;		// 左後モーターPWM値
extern signed char	sPwm;		// サーボモーターPWM値

//====================================//
// プロトタイプ宣言								//
//====================================//
// IOポート
void init_IO(void);

// タイマ関連
void delay( unsigned short delaytime );

// エンコーダ関連
void getEncoder (void);

// LED関連
void led_out( unsigned char led );

// スイッチ関連
void getSwitch(void);
unsigned char tasw_get ( void );
unsigned char dipsw_get( void );

// センサ関連
short getGyro( void );
short getServoAngle(void);
short getAnalogSensor( void );
unsigned char sensor_inp( void );
unsigned char startbar_get( void );

//ブザー関連
void init_BeepS( void );
void setBeepPatternS( unsigned short Beep );
void beepProcessS( void );

// モーター関連
void motor_f( signed char accelefL, signed char accelefR );
void motor_r( signed char accelerL, signed char accelerR );

// サーボ関連
void servoPwmOut( signed char pwm );

// 比較関連
int short_sort( const void* a, const void* b );

#endif // PERIPHERALFUNCTIONS_H_