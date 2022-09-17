#ifndef MTU_H_
#define MTU_H_
//====================================//
// インクルード
//====================================//
#include "R_PG_IGC-P8080_v1.h"
//====================================//
// シンボル定義
//====================================//
#define SERVO_CENTER		2045		// サーボセンターのAD値
#define SERVO_LIMIT		1500		// サーボリミットAD値±
#define TGR_MOTOR			1582		// ジェネラルレジスタ初期値(駆動モータ) TGRA初期値-1
#define TGR_SERVO			1582		// ジェネラルレジスタ初期値(サーボ)	 TGRA初期値-1
#define PALSE_METER		14590	    // 1mのパルス
#define PALSE_MILLIMETER	14.59F	    // 1mmのパルス
#define SPEED_CURRENT		15		    // 1m/sの時　1msのパルス
/*************************************** 自動生成関数 *************************************/
//センサLED
#define L_Sen_ON		R_PG_Timer_SetTGR_D_MTU_U0_C4( TGR_MOTOR+2 );
#define L_Sen_OFF		R_PG_Timer_SetTGR_D_MTU_U0_C4( 0 );
// エンコーダ
#define ENCODER_COUNT	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );	// カウント取得
// モータ
// MTU初期化(PWM機能、エンコーダ
#define SET_MTU_C0	R_PG_Timer_Set_MTU_U0_C0();		// ch0
#define SET_MTU_C1	R_PG_Timer_Set_MTU_U0_C1();		// ch1
#define SET_MTU_C2	R_PG_Timer_Set_MTU_U0_C2();		// ch2
#define SET_MTU_C3	R_PG_Timer_Set_MTU_U0_C3();		// ch3
#define SET_MTU_C4	R_PG_Timer_Set_MTU_U0_C4();		// ch4
// MTU0,1,2,3,4のカウント開始
#define START_MTU		R_PG_Timer_SynchronouslyStartCount_MTU_U0( 1, 1, 1, 1, 1);

// 左前輪
#define DIR_FL_FOR		R_PG_IO_PORT_Write_PE3( 1 );		// モータ回転方向(正転)
#define DIR_FL_REV		R_PG_IO_PORT_Write_PE3( 0 );		// モータ回転方向(逆転）
#define PWM_FL_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C4( abs(pwmfl) );	// PWM出力
// 右前輪
#define DIR_FR_FOR		R_PG_IO_PORT_Write_PE7( 0 );
#define DIR_FR_REV		R_PG_IO_PORT_Write_PE7( 1 );
#define PWM_FR_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C0( abs(pwmfr) );
// 左後輪
#define DIR_RL_FOR		R_PG_IO_PORT_Write_PC4( 1 );
#define DIR_RL_REV		R_PG_IO_PORT_Write_PC4( 0 );
#define PWM_RL_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C3( abs(pwmrl) );
// 右後輪
#define DIR_RR_FOR		R_PG_IO_PORT_Write_PB2( 0 );
#define DIR_RR_REV		R_PG_IO_PORT_Write_PB2( 1 );
#define PWM_RR_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C2( abs(pwmrr) );
// サーボ1
#define DIR_SERVO_FOR		R_PG_IO_PORT_Write_PE5( 1 );
#define DIR_SERVO_REV		R_PG_IO_PORT_Write_PE5( 0 );
#define PWM_SERVO_OUT		R_PG_Timer_SetTGR_D_MTU_U0_C0( abs(pwm) );
// サーボ2
#define DIR_LANCER_FOR		R_PG_IO_PORT_Write_PC2( 0 );
#define DIR_LANCER_REV		R_PG_IO_PORT_Write_PC2( 1 );
#define PWM_LANCER_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C3( abs(pwml) );

/******************************************************************************************/

//====================================//
// グローバル変数の宣言
//====================================//
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
// プロトタイプ宣言
//====================================//
// エンコーダ関連
void getEncoder (void);

// モーター関連
void motorPwmOut( signed char accelefL, signed char accelefR, signed char accelerL, signed char accelerR );
// サーボ関連
void servoPwmOut( signed char pwm );

#endif // MTU_H_