#ifndef CONTROL_H_
#define CONTROL_H_
//====================================//
// インクルード									//
//====================================//
#include "Rotaryencoder.h"
//====================================//
// シンボル定義									//
//====================================//
// 緊急停止
#define	STOPPING_METER			40		// 停止距離

// 各セクションでの目標速度　x/10[m/s]
#define SPEED_STRAIGHT				54	// 通常トレース

#define SPEED_CROSSLINE			28	// クロスライン進入速度
#define SPEED_CLANK_TRACE			32	// クランク進入速度
#define SPEED_RIGHTCLANK_CURVE		22	// 右クランク旋回速度
#define SPEED_RIGHTCLANK_ESCAPE		40	// 右クランク復帰速度
#define SPEED_LEFTCLANK_CURVE		22	// 左クランク旋回速度
#define SPEED_LEFTCLANK_ESCAPE		40	// 左クランク復帰速度

#define SPEED_HALFLINE				30	// ハーフライン進入速度
#define SPEED_RIGHTCHANGE_TRACE	46	// 右レーンチェンジ進入速度
#define SPEED_RIGHTCHANGE_CURVE	46	// 右レーンチェンジ旋回速度
#define SPEED_RIGHTCHANGE_ESCAPE	46	// 右レーンチェンジ復帰速度
#define SPEED_LEFTCHANGE_TRACE		46	// 左レーンチェンジ進入速度
#define SPEED_LEFTCHANGE_CURVE		46	// 左レーンチェンジ旋回速度
#define SPEED_LEFTCHANGE_ESCAPE		46	// 左レーンチェンジ復帰速度
// 角度
#define ANGLE_RIGHTCLANK			-400	// 右クランク旋回角度
#define ANGLE_LEFTCLANK			400	// 左クランク旋回角度
#define ANGLE_RIGHTCHANGE			-160	// 右レーンチェンジ旋回角度
#define ANGLE_LEFTCHANGE			160	// 右レーンチェンジ旋回角度

// PIDゲイン関連
//白線トレース
#define KP			11
#define KI			0
#define KD		44

// 角度制御
#define KP2		9
#define KI2		90
#define KD2		90

// 速度制御
#define KP3		6
#define KI3		10
#define KD3		0

// 緊急停止関連
#define STOP_SENSOR1		60		// センサ全灯
#define STOP_SENSOR2		800		// センサ全消灯
#define STOP_ENCODER		100		// エンコーダ停止(ひっくり返った？)
#define STOP_COUNT		10000	// 時間停止
//====================================//
// グローバル変数の宣言								//
//====================================//
// パターン、モード関連
extern char 	pattern;			// パターン番号
extern char	lcd_mode;			// LCD表示選択
extern char	slope_mode;		// 坂チェック		0:上り坂始め	1:上り坂終わり	2:下り坂始め	3:下り坂終わり
extern char	angle_mode;		// サーボPWM変更	0:白線トレース	1:角度制御
extern char	pushcart_mode;		// 手押しモード可否	0:自動走行	1:手押し
extern char	msdset;			// MicroSDが初期化されたか	0:初期化失敗	1:初期化成功
extern char	IMUSet;			// IMUが初期化されたか	0: 初期化失敗	1:初期化成功

// パラメータ関連
// 距離stopping_meter
extern short	stopping_meter;			// 停止距離
// 速度
extern short	speed_straight;			// 通常トレース

extern short	speed_crossline;			// クロスライン進入速度
extern short	speed_ckank_trace;		// クランク進入速度
extern short	speed_rightclank_curve;	// 右クランク旋回速度
extern short	speed_rightclank_escape;	// 右クランク復帰速度
extern short	speed_leftclank_curve;	// 左クランク旋回速度
extern short	speed_leftclank_escape;	// 左クランク復帰速度

extern short	speed_halfine;			// ハーフライン進入速度
extern short	speed_rightchange_trace;	// 右レーンチェンジ進入速度
extern short	speed_rightchange_curve;	// 右レーンチェンジ旋回速度
extern short	speed_rightchange_escape;// 右レーンチェンジ復帰速度

extern short	speed_leftchange_trace;	// 左レーンチェンジ進入速度
extern short	speed_leftchange_curve;	// 左レーンチェンジ旋回速度
extern short	speed_leftchange_escape;	// 左レーンチェンジ旋回速度

// サーボ角度
extern short	angle_rightclank;		// 右クランク旋回角度
extern short	angle_leftclank;			// 左クランク旋回角度
extern short	angle_rightchange;		// 右レーンチェンジ旋回角度
extern short	angle_leftchange;		// 右レーンチェンジ旋回角度

// モーター関連
extern signed char 	motorPwm;	// モーター制御PWM
extern short		targetSpeed;	// 目標速度

// ゲイン関連
extern char	kp_buff, ki_buff, kd_buff;
extern char	kp2_buff, ki2_buff, kd2_buff;
extern char 	kp3_buff, ki3_buff, kd3_buff;

// デモ関連
extern char demo;

// サーボ関連
extern double		Int;			// I成分積算値(白線トレース)
extern short 		SetAngle;		// 目標角度
extern signed char 	ServoPwm;	// 白線トレースサーボPWM
extern signed char 	ServoPwm2;	// 角度サーボPWM

//====================================//
// プロトタイプ宣言									//
//====================================//
// パラメータ関連
void initParameter(void);

// マーカー関連
signed char check_crossline( void );
signed char check_rightline( void );
signed char check_leftline( void );

// モーター関連
void motorControl( void );

// サーボ関連
void servoControl( void );
void servoControl2( void );

#endif // CONTROL_H_