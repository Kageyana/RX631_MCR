#ifndef LINECHASE_H_
#define LINECHASE_H_
//======================================//
// シンボル定義                         //
//======================================//
// 緊急停止
#define	STOPPING_METER		23		// 停止距離

// 各セクションでの目標速度　x/10[m/s]
#define SPEED_STRAIGHT			44	// 通常トレース
#define SPEED_CURVE_BRAKE		38	// カーブブレーキ
#define SPEED_CURVE_R600		38	// R600カーブ速度
#define SPEED_CURVE_R450		34	// R450カーブ速度
#define SPEED_CURVE_STRAIGHT		38	// S字カーブ直線速度

#define SPEED_CROSSLINE			12	// クロスライン進入速度
#define SPEED_CLANK_TRACE		30	// クランク進入速度
#define SPEED_RIGHTCLANK_CURVE		22	// 右クランク旋回速度
#define SPEED_RIGHTCLANK_ESCAPE		36	// 右クランク復帰速度
#define SPEED_LEFTCLANK_CURVE		22	// 左クランク旋回速度
#define SPEED_LEFTCLANK_ESCAPE		36	// 左クランク復帰速度

#define SPEED_HALFLINE			30	// ハーフライン進入速度
#define SPEED_RIGHTCHANGE_TRACE		42	// 右レーンチェンジ進入速度
#define SPEED_RIGHTCHANGE_CURVE		42	// 右レーンチェンジ旋回速度
#define SPEED_RIGHTCHANGE_ESCAPE	42	// 右レーンチェンジ復帰速度
#define SPEED_LEFTCHANGE_TRACE		42	// 左レーンチェンジ進入速度
#define SPEED_LEFTCHANGE_CURVE		42	// 左レーンチェンジ旋回速度
#define SPEED_LEFTCHANGE_ESCAPE		42	// 左レーンチェンジ復帰速度

#define SPEED_SLOPE_BRAKE		20	// 下り坂終点速度
#define SPEED_SLOPE_TRACE		36	// 坂読み飛ばし速度
// 角度
#define ANGLE_RIGHTCLANK		-420	// 右クランク旋回角度
#define ANGLE_LEFTCLANK			420	// 左クランク旋回角度
#define ANGLE_RIGHTCHANGE		-160	// 右レーンチェンジ旋回角度
#define ANGLE_LEFTCHANGE		160	// 右レーンチェンジ旋回角度

// カーブ関連
#define CURVE_R600_START	40		// R600開始AD値
#define CURVE_R450_START	140		// R450開始AD値

// ジャイロ関連
#define AD_3V3VOLTAGE		0.806		// 3V時の1AD値あたりの電圧[mV]
#define AD_5VOLTAGE		1.22		// 5V時の1AD値あたりの電圧[mV]
#define GYROVOLTAGE		0.67		// 電圧毎角加速度[mV/deg/s]
#define SLOPEUPPERLINE		17		// 上り坂検出角度
#define SLOPELOWERLINE		-14		// 下り坂検出角度

#define PI			3.141592	// 円周率
#define RIGHTCURVE_ENCODER	78.5		// 右輪中心からエンコーダーの中心までの距離
#define LEFTCURVE_ENCODER	74.5		// 左輪中心からエンコーダーの中心までの距離

// PIDゲイン関連
//白線トレース
#define KP			20//30
#define KI			2//41
#define KD			58//27

// 角度制御
#define KP2			9
#define KI2			90
#define KD2			90

// 速度制御
#define KP3			6
#define KI3			20
#define KD3			0

// 緊急停止関連
#define STOP_SENSOR1		60		// センサ全灯
#define STOP_SENSOR2		800		// センサ全消灯
#define STOP_ENCODER		10		// エンコーダ停止(ひっくり返った？)
#define STOP_GYRO		30		// マイナスの加速度検知(コースから落ちた？)
#define STOP_COUNT		10000		// 時間停止
//======================================//
// グローバル変数の宣言                 //
//======================================//
// パターン、モード関連
extern char 	pattern;			// パターン番号
extern char	lcd_mode;			// LCD表示選択
extern char	slope_mode;			// 坂チェック		0:上り坂始め	1:上り坂終わり	2:下り坂始め	3:下り坂終わり
extern char	angle_mode;
extern char	pushcart_mode;			// 手押しモード
extern char	msdset;				// MicroSDが初期化されたか
extern char	IMUSet;				// IMUが初期化されたか	0: 初期化失敗	1:初期化成功


// パラメータ関連
// 距離
extern short	stopping_meter;			// 停止距離
// 速度
extern short	speed_straight;			// 通常トレース
extern short	speed_curve_brake;		// カーブブレーキ
extern short	speed_curve_r600;		// R600カーブ速度
extern short	speed_curve_r450;		// R450カーブ速度
extern short	speed_curve_straight;		// S字カーブ直線速度

extern short	speed_crossline;		// クロスライン進入速度
extern short	speed_ckank_trace;		// クランク進入速度
extern short	speed_rightclank_curve;		// 右クランク旋回速度
extern short	speed_rightclank_escape;	// 右クランク復帰速度
extern short	speed_leftclank_curve;		// 左クランク旋回速度
extern short	speed_leftclank_escape;		// 左クランク復帰速度

extern short	speed_halfine;			// ハーフライン進入速度
extern short	speed_rightchange_trace;	// 右レーンチェンジ進入速度
extern short	speed_rightchange_curve;	// 右レーンチェンジ旋回速度
extern short	speed_rightchange_escape;	// 右レーンチェンジ復帰速度

extern short	speed_leftchange_trace;		// 左レーンチェンジ進入速度
extern short	speed_leftchange_curve;		// 左レーンチェンジ旋回速度
extern short	speed_leftchange_escape;	// 左レーンチェンジ旋回速度

extern short	speed_slope_brake;		// 下り坂終点速度
extern short	speed_slope_trace;		// 坂読み飛ばし速度

// 角度
extern short	angle_rightclank;		// 右クランク旋回角度
extern short	angle_leftclank;		// 左クランク旋回角度
extern short	angle_rightchange;		// 右レーンチェンジ旋回角度
extern short	angle_leftchange;		// 右レーンチェンジ旋回角度

// タイマ関連
extern unsigned int	cnt1;		// 走行用タイマカウント
extern unsigned int	cnt_log;	// 走行用タイマカウント
extern unsigned short	cnt_out;	// コースアウト判定用タイマカウント
extern unsigned short	cnt_out2;	// コースアウト判定用タイマカウント2
extern unsigned short	cnt_out3;	// コースアウト判定用タイマカウント3
extern unsigned short	cnt_out4;	// コースアウト判定用タイマカウント4
extern unsigned short 	cnt_gyro;	// 角度計算用タイマカウント

// 角度関連
extern short 		Degrees;	// ジャイロセンサから計算した角度(degrees)
extern double		integral_rad;	// 角速度積算値
extern double		angularVelocity;// 理論角速度[rad/s]
extern short 		TurningAngle;	// 旋回角度

// モーター関連
extern signed char 	motorPwm;	// モーター制御PWM
extern short		target_speed;	// 目標速度

// ゲイン関連
extern signed char	kp_buff, ki_buff, kd_buff;
extern signed char	kp2_buff, ki2_buff, kd2_buff;
extern signed char 	kp3_buff, ki3_buff, kd3_buff;

// デモ関連
extern char demo;

// サーボ関連
extern short 		SetAngle;	// 目標角度
extern signed char 	ServoPwm;	// 白線トレースサーボPWM
extern signed char 	ServoPwm2;	// 角度サーボPWM

//======================================//
// プロトタイプ宣言                     //
//======================================//
// マーカー関連
signed char check_crossline( void );
signed char check_rightline( void );
signed char check_leftline( void );
signed char check_slope( void );

// 角度関連
void get_degrees( void );
void get_TurningAngle(void);

// エンコーダ関連
unsigned int enc_mm( short mm );

// モーター関連
void motorControl( void );

// 内輪差関連
void diff ( signed char pwm );

// サーボ関連
void servoControl( void );
void servoControl2( void );

#endif // LINECHASE_H_