//====================================//
// インクルード									//
//====================================//
#include "LineChase.h"
//====================================//
// グローバル変数の宣言							//
//====================================//
// モード関連
char 	lcd_mode = 1;		// LCD表示可否		1:表示		0:消灯		
char 	slope_mode;		// 坂チェック		0:上り坂始め	1:上り坂終わり	2:下り坂始め
char 	angle_mode;		// サーボPWM変更	0:白線トレース	1:角度制御
char	pushcart_mode;		// 手押しモード可否	0:自動走行	1:手押し
char	msdset;			// MicroSDが初期化されたか	0:初期化失敗	1:初期化成功
char	IMUSet = 0;		// IMUが初期化されたか	0: 初期化失敗	1:初期化成功

// パラメータ関連
// 距離
short	stopping_meter;			// 停止距離
// 速度
short	speed_straight;			// 通常トレース
short	speed_curve_brake;		// カーブブレーキ
short	speed_curve_r600;		// R600カーブ速度
short	speed_curve_r450;		// R450カーブ速度
short	speed_curve_straight;		// S字カーブ直線速度

short	speed_crossline;			// クロスライン進入速度
short	speed_ckank_trace;		// クランク進入速度
short	speed_rightclank_curve;	// 右クランク旋回速度
short	speed_rightclank_escape;	// 右クランク復帰速度
short	speed_leftclank_curve;	// 左クランク旋回速度
short	speed_leftclank_escape;	// 左クランク復帰速度

short	speed_halfine;			// ハーフライン進入速度
short	speed_rightchange_trace;	// 右レーンチェンジ進入速度
short	speed_rightchange_curve;	// 右レーンチェンジ旋回速度
short	speed_rightchange_escape;// 右レーンチェンジ復帰速度

short	speed_leftchange_trace;	// 左レーンチェンジ進入速度
short	speed_leftchange_curve;	// 左レーンチェンジ旋回速度
short	speed_leftchange_escape;	// 左レーンチェンジ旋回速度

short	speed_slope_brake;		// 下り坂終点速度
short	speed_slope_trace;		// 坂読み飛ばし速度

// サーボ角度
short	angle_rightclank;		// 右クランク旋回角度
short	angle_leftclank;			// 左クランク旋回角度
short	angle_rightchange;		// 右レーンチェンジ旋回角度
short	angle_leftchange;		// 右レーンチェンジ旋回角度

// タイマ関連
short			cnt_gyro;				// 角度計算用カウンタ

// 角度関連
double 		TurningAngleEnc;	// エンコーダから求めた旋回角度

// サーボ関連
// 白線トレース
signed char	ServoPwm;	// 白線トレースサーボPWM
short 		SensorBefore;	// 1ms前のセンサ値
char			DevBefore;		// I成分リセット用
double		Int;			// I成分積算値(白線トレース)
// 角度制御
signed char	ServoPwm2;		// 角度サーボPWM
short 		SetAngle;		// 目標角度
short			SetAngleBefore;		// 1ms前の目標角度
short 		AngleBefore2;	// 1ms前の角度
char			AngleBefore3;		// I成分リセット用
double		Int2;			// I成分積算値(角度制御)

// モーター関連
signed char 	motorPwm;	// モーター制御PWM
char 			AccelefBefore;		// I成分リセット用
short			EncoderBefore;		// 1ms前の速度
int 			targetSpeedBefore;	// 1ms前の目標速度	
double 		Int3;			// I成分積算値(速度制御)
short			targetSpeed;		// 目標速度

// デモ関連
char 	demo;

// ゲイン関連
signed char	kp_buff, ki_buff, kd_buff;
signed char	kp2_buff, ki2_buff, kd2_buff;
signed char 	kp3_buff, ki3_buff, kd3_buff;

///////////////////////////////////////////////////////////////////////////
// モジュール名 check_crossline							//
// 処理概要     クロスライン検知							//
// 引数         なし									//
// 戻り値       0:クロスラインなし 1:あり					//
///////////////////////////////////////////////////////////////////////////
signed char check_crossline( void )
{
	unsigned char digital_sensor;
	char ret = 0;

	digital_sensor = sensor_inp();
	if ( digital_sensor == 0x7 ) ret = 1;
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 check_rightline							//
// 処理概要     右ハーフライン検出処理						//
// 引数         なし									//
// 戻り値       0:右ハーフラインなし 1:あり					//
///////////////////////////////////////////////////////////////////////////
signed char check_rightline( void )
{
	unsigned char digital_sensor;
	char ret = 0;

	digital_sensor = sensor_inp();
	if ( digital_sensor == 0x3 ) ret = 1;
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 check_leftline							//
// 処理概要     左ハーフライン検出処理						//
// 引数         なし									//
// 戻り値       0:左ハーフラインなし 1:あり					//
///////////////////////////////////////////////////////////////////////////
signed char check_leftline( void )
{
	unsigned char digital_sensor;
	char ret = 0;

	digital_sensor = sensor_inp();
	if ( digital_sensor == 0x6 ) {
		ret = 1;
	}
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 check_slope								//
// 処理概要     ジャイロセンサの値から坂道検出				//
// 引数         なし									//
// 戻り値       0:坂道なし 1:上り坂　-1:下り坂					//
///////////////////////////////////////////////////////////////////////////
signed char check_slope( void )
{
	short deg, upperline, lowerline;
	signed char ret = 0;

	deg = PichAngleIMU;
	
	upperline = SLOPEUPPERLINE;
	lowerline = SLOPELOWERLINE;
	
	if ( deg >= upperline ) ret = 1;
	if ( deg <= lowerline ) ret = -1;
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 enc_mm								//
// 処理概要     mmをエンコーダのパルス数に変換して返す			//
// 引数         mm:変換する長さ[mm]						//
// 戻り値       変換したパルス数							//
///////////////////////////////////////////////////////////////////////////
unsigned int enc_mm( short mm )
{
	return PALSE_MILLIMETER * mm;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 servoControl							//
// 処理概要     ライントレース時サーボのPWMの計算				//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void servoControl( void )
{
	int iP, iD, iI, iRet;
	short Dev, Dif;
	
	//サーボモータ用PWM値計算
	Dev = getAnalogSensor();
	// 目標値を変更したらI成分リセット
	if ( Dev >= 0 && DevBefore == 1 ) Int = 0;
	else if ( Dev < -0 && DevBefore == 0 ) Int = 0;
	
	Int += (double)Dev * 0.001;
	Dif = ( Dev - SensorBefore ) * 1;	// dゲイン1/1000倍

	iP = (int)kp_buff * Dev;		// 比例
	iI = (double)ki_buff * Int;		// 積分
	iD = (int)kd_buff * Dif;		// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 10;

	// PWMの上限の設定(安定したら70程度に)
	if ( iRet >  70 ) iRet =  70;		// マイコンカーが安定したら
	if ( iRet <  -70 ) iRet = -70;	// 上限を90くらいにしてください
	if ( sensor_inp() == 0x1 ) iRet = -70;
	else if ( sensor_inp() == 0x4 ) iRet = 70;
	
	if ( Dev >= 0 )	DevBefore = 0;
	else			DevBefore = 1;
	ServoPwm = iRet;
	SensorBefore = Dev;				// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 servoControl2							//
// 処理概要     角度制御時サーボのPWMの計算					//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void servoControl2( void )
{
	short i, j, Dev, Dif;
	int iP, iD, iI, iRet;
	
	// 目標値、現在値取得
	i = SetAngle;
	j = getServoAngle();
	
	//サーボモータ用PWM値計算
	Dev = i - j;
		
	// 目標値を超えたらI成分リセット
	if ( Dev >= 0 && AngleBefore3 == 1 ) Int2 = 0;
	else if ( Dev < 0 && AngleBefore3 == 0 ) Int2 = 0;
	
	// 目標値を変更したらI成分リセット
	if ( !(i == SetAngleBefore) ) Int2 = 0;
	
	Int2 += (double)Dev * 0.001;
	Dif = ( Dev - AngleBefore2 ) * 1;		// dゲイン1/1000倍

	iP = (int)kp2_buff * Dev;		// 比例
	iI = (double)ki2_buff * Int2;		// 積分
	iD = (int)kd2_buff * Dif;		// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 4;		// PWMを0〜100の間に収める

	// PWMの上限の設定(安定したら70程度に)
	if ( iRet >  90 ) iRet =  90;		// マイコンカーが安定したら
	if ( iRet <  -90 ) iRet = -90;	// 上限を90くらいにしてください
	
	if ( Dev >= 0 ) 	AngleBefore3 = 0;
	else 			AngleBefore3 = 1;
	SetAngleBefore = i;
	ServoPwm2 = iRet;
	AngleBefore2 = Dev;			// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 diff									//
// 処理概要   	R1,R2,R3及びR4の計算					//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void diff ( signed char pwm )
{
	const char rev_difference_B[] = {       // 角度から内輪、外輪回転差計算	
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 99, 100, 99, 99, 100, 99, 99, 100, 99, 
		99, 100, 99, 99, 100, 99, 99, 100, 99, 99, 100, 99, 
		98, 100, 99, 98, 100, 98, 98, 100, 98, 98, 100, 98, 
		98, 100, 98, 98, 100, 98, 98, 100, 98, 98, 100, 98, 
		97, 100, 97, 97, 100, 97, 97, 100, 97, 97, 100, 97, 
		97, 100, 97, 97, 100, 97, 97, 100, 97, 97, 100, 97, 
		96, 100, 96, 96, 100, 96, 96, 100, 96, 96, 100, 96, 
		96, 100, 96, 96, 100, 96, 96, 100, 96, 96, 100, 96, 
		95, 100, 96, 95, 100, 95, 95, 100, 95, 95, 100, 95, 
		95, 100, 95, 95, 100, 95, 95, 100, 95, 94, 100, 95, 
		94, 100, 95, 94, 100, 94, 94, 100, 94, 94, 100, 94, 
		94, 100, 94, 94, 100, 94, 94, 100, 94, 93, 100, 94, 
		93, 100, 94, 93, 100, 93, 93, 100, 93, 93, 100, 93, 
		93, 100, 93, 93, 100, 93, 93, 100, 93, 92, 100, 93, 
		92, 100, 93, 92, 100, 93, 92, 100, 92, 92, 100, 92, 
		92, 100, 92, 92, 100, 92, 92, 100, 92, 91, 100, 92, 
		91, 100, 92, 91, 100, 92, 91, 100, 91, 91, 100, 91, 
		91, 100, 91, 91, 100, 91, 91, 100, 91, 90, 100, 91, 
		90, 99, 91, 90, 99, 91, 90, 99, 91, 90, 99, 90, 
		90, 99, 90, 90, 99, 90, 89, 99, 90, 89, 99, 90, 
		89, 99, 90, 89, 99, 90, 89, 99, 90, 89, 99, 90, 
		89, 99, 89, 89, 99, 89, 88, 99, 89, 88, 99, 89, 
		88, 99, 89, 88, 99, 89, 88, 99, 89, 88, 99, 89, 
		88, 99, 89, 88, 99, 88, 87, 99, 88, 87, 99, 88, 
		87, 99, 88, 87, 99, 88, 87, 99, 88, 87, 99, 88, 
		87, 99, 88, 87, 99, 88, 86, 99, 87, 86, 99, 87, 
		86, 99, 87, 86, 99, 87, 86, 99, 87, 86, 99, 87, 
		86, 99, 87, 86, 99, 87, 85, 99, 87, 85, 99, 87, 
		85, 99, 86, 85, 99, 86, 85, 99, 86, 85, 99, 86, 
		85, 99, 86, 85, 99, 86, 84, 99, 86, 84, 99, 86, 
		84, 99, 86, 84, 99, 85, 84, 99, 85, 84, 99, 85, 
		84, 99, 85, 84, 99, 85, 83, 99, 85, 83, 99, 85, 
		83, 99, 85, 83, 99, 85, 83, 99, 85, 83, 99, 84, 
		83, 98, 84, 83, 98, 84, 82, 98, 84, 82, 98, 84, 
		82, 98, 84, 82, 98, 84, 82, 98, 84, 82, 98, 84, 
		82, 98, 84, 82, 98, 83, 81, 98, 83, 81, 98, 83, 
		81, 98, 83, 81, 98, 83, 81, 98, 83, 81, 98, 83, 
		81, 98, 83, 81, 98, 83, 80, 98, 83, 80, 98, 83, 
		80, 98, 82, 80, 98, 82, 80, 98, 82, 80, 98, 82, 
		80, 98, 82, 80, 98, 82, 79, 98, 82, 79, 98, 82, 
		79, 98, 82, 79, 98, 82, 79, 98, 81, 79, 98, 81, 
		79, 98, 81, 79, 98, 81, 79, 98, 81, 78, 98, 81, 
		78, 98, 81, 78, 98, 81, 78, 98, 81, 78, 98, 81, 
		78, 98, 81, 78, 98, 80, 78, 98, 80, 77, 98, 80, 
		77, 97, 80, 77, 97, 80, 77, 97, 80, 77, 97, 80, 
		77, 97, 80, 77, 97, 80, 77, 97, 80, 76, 97, 80, 
		76, 97, 79, 76, 97, 79, 76, 97, 79, 76, 97, 79, 
		76, 97, 79, 76, 97, 79, 76, 97, 79, 75, 97, 79, 
		75, 97, 79, 75, 97, 79, 75, 97, 79, 75, 97, 78, 
		75, 97, 78, 75, 97, 78, 75, 97, 78, 74, 97, 78, 
		74, 97, 78, 74, 97, 78, 74, 97, 78, 74, 97, 78, 
		74, 97, 78, 74, 97, 78, 74, 97, 78, 74, 97, 77, 
		73, 97, 77, 73, 97, 77, 73, 97, 77, 73, 97, 77, 
		73, 97, 77, 73, 97, 77, 73, 96, 77, 73, 96, 77, 
		72, 96, 77, 72, 96, 77, 72, 96, 76, 72, 96, 76, 
		72, 96, 76, 72, 96, 76, 72, 96, 76, 72, 96, 76, 
		71, 96, 76, 71, 96, 76, 71, 96, 76, 71, 96, 76, 
		71, 96, 76, 71, 96, 76, 71, 96, 76, 71, 96, 75, 
		71, 96, 75, 70, 96, 75, 70, 96, 75, 70, 96, 75, 
		70, 96, 75, 70, 96, 75, 70, 96, 75, 70, 96, 75, 
		70, 96, 75, 69, 96, 75, 69, 96, 75, 69, 96, 74, 
		69, 96, 74, 69, 96, 74, 69, 96, 74, 69, 95, 74, 
		69, 95, 74, 68, 95, 74, 68, 95, 74, 68, 95, 74, 
		68, 95, 74, 68, 95, 74, 68, 95, 74, 68, 95, 74, 
		68, 95, 73, 68, 95, 73, 67, 95, 73, 67, 95, 73, 
		67, 95, 73, 67, 95, 73, 67, 95, 73, 67, 95, 73, 
		67, 95, 73, 67, 95, 73, 66, 95, 73, 66, 95, 73, 
		66, 95, 73, 66, 95, 72, 66, 95, 72, 66, 95, 72, 
		66, 95, 72, 66, 95, 72, 65, 95, 72, 65, 95, 72, 
		65, 95, 72, 65, 95, 72, 65, 94, 72, 65, 94, 72, 
		65, 94, 72, 65, 94, 72, 65, 94, 72, 64, 94, 71, 
		64, 94, 71, 64, 94, 71, 64, 94, 71, 64, 94, 71, 
		64, 94, 71, 64, 94, 71, 64, 94, 71, 63, 94, 71, 
		63, 94, 71, 63, 94, 71, 63, 94, 71, 63, 94, 71, 
		63, 94, 71, 63, 94, 70, 63, 94, 70, 63, 94, 70, 
		62, 94, 70, 62, 94, 70, 62, 94, 70, 62, 94, 70, 
		62, 94, 70, 62, 94, 70, 62, 93, 70, 62, 93, 70, 
		61, 93, 70, 61, 93, 70, 61, 93, 70, 61, 93, 70, 
		61, 93, 69, 61, 93, 69, 61, 93, 69, 61, 93, 69, 
		60, 93, 69, 60, 93, 69, 60, 93, 69, 60, 93, 69, 
		60, 93, 69, 60, 93, 69, 60, 93, 69, 60, 93, 69, 
		60, 93, 69, 59, 93, 69, 59, 93, 69, 59, 93, 68, 
		59, 93, 68, 59, 93, 68, 59, 93, 68, 59, 93, 68, 
		59, 93, 68, 58, 92, 68, 58, 92, 68, 58, 92, 68, 
		58, 92, 68, 58, 92, 68, 58, 92, 68, 58, 92, 68, 
		58, 92, 68, 57, 92, 68, 57, 92, 68, 57, 92, 67, 
		57, 92, 67, 57, 92, 67, 57, 92, 67, 57, 92, 67, 
		57, 92, 67, 57, 92, 67, 56, 92, 67, 56, 92, 67, 
		56, 92, 67, 56, 92, 67, 56, 92, 67, 56, 92, 67, 
		56, 92, 67, 56, 92, 67, 55, 91, 67, 55, 91, 66, 
		55, 91, 66, 55, 91, 66, 55, 91, 66, 55, 91, 66, 
		55, 91, 66, 55, 91, 66, 54, 91, 66, 54, 91, 66, 
		54, 91, 66, 54, 91, 66, 54, 91, 66, 54, 91, 66, 
		54, 91, 66, 54, 91, 66, 54, 91, 66, 53, 91, 66, 
		53, 91, 65, 53, 91, 65, 53, 91, 65, 53, 91, 65, 
		53, 91, 65, 53, 91, 65, 53, 90, 65, 52, 90, 65, 
		52, 90, 65, 52, 90, 65, 52, 90, 65, 52, 90, 65, 
		52, 90, 65, 52, 90, 65, 52, 90, 65, 51, 90, 65, 
		51, 90, 65, 51, 90, 65, 51, 90, 64, 51, 90, 64, 
		51, 90, 64, 51, 90, 64, 51, 90, 64, 50, 90, 64, 
		50, 90, 64, 50, 90, 64, 50, 90, 64, 50, 90, 64, 
		50, 89, 64, 50, 89, 64, 50, 89, 64, 49, 89, 64, 
		49, 89, 64, 49, 89, 64, 49, 89, 64, 49, 89, 64, 
		49, 89, 64, 49, 89, 63, 49, 89, 63, 48, 89, 63, 
		48, 89, 63, 48, 89, 63, 48, 89, 63, 48, 89, 63, 
		48, 89, 63, 48, 89, 63, 48, 89, 63, 47, 89, 63, 
		47, 89, 63, 47, 88, 63, 47, 88, 63, 47, 88, 63, 
		47, 88, 63, 47, 88, 63, 47, 88, 63, 46, 88, 63, 
		46, 88, 63, 46, 88, 62, 46, 88, 62, 46, 88, 62, 
		46, 88, 62, 46, 88, 62, 46, 88, 62, 45, 88, 62, 
		45, 88, 62, 45, 88, 62, 45, 88, 62, 45, 88, 62, 
		45, 88, 62, 45, 87, 62, 44, 87, 62, 44, 87, 62, 
		44, 87, 62, 44, 87, 62, 44, 87, 62, 44, 87, 62, 
		44, 87, 62, 44, 87, 62, 43, 87, 62, 43, 87, 61, 
		43, 87, 61, 43, 87, 61, 43, 87, 61, 43, 87, 61, 
		43, 87, 61, 43, 87, 61, 42, 87, 61, 42, 87, 61, 
		42, 86, 61, 42, 86, 61, 42, 86, 61, 42, 86, 61, 
		42, 86, 61, 41, 86, 61, 41, 86, 61, 41, 86, 61, 
		41, 86, 61, 41, 86, 61, 41, 86, 61, 41, 86, 61, 
		40, 86, 61, 40, 86, 61, 40, 86, 60, 40, 86, 60, 
		40, 86, 60, 40, 85, 60, 40, 85, 60, 39, 85, 60, 
		39, 85, 60, 39, 85, 60, 39, 85, 60, 39, 85, 60, 
		39, 85, 60, 39, 85, 60, 38, 85, 60, 38, 85, 60, 
		38, 85, 60, 38, 85, 60, 38, 85, 60, 38, 85, 60
		};

	const char rev_difference_D[] = {       // 角度から内輪、外輪回転差計算	
		70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 
		70, 70, 70, 70, 70, 70, 69, 70, 69, 69, 70, 69, 
		69, 70, 69, 69, 70, 69, 69, 70, 69, 69, 70, 69, 
		69, 70, 69, 69, 70, 69, 69, 70, 69, 69, 70, 69, 
		69, 70, 69, 68, 70, 69, 68, 70, 68, 68, 70, 68, 
		68, 70, 68, 68, 70, 68, 68, 70, 68, 68, 70, 68, 
		68, 70, 68, 68, 70, 68, 68, 70, 68, 68, 70, 68, 
		67, 70, 68, 67, 70, 67, 67, 70, 67, 67, 70, 67, 
		67, 70, 67, 67, 70, 67, 67, 70, 67, 67, 70, 67, 
		67, 70, 67, 67, 70, 67, 67, 70, 67, 66, 70, 67, 
		66, 70, 67, 66, 70, 66, 66, 70, 66, 66, 70, 66, 
		66, 70, 66, 66, 70, 66, 66, 70, 66, 66, 70, 66, 
		66, 70, 66, 66, 70, 66, 66, 70, 66, 65, 70, 66, 
		65, 70, 66, 65, 70, 65, 65, 70, 65, 65, 70, 65, 
		65, 70, 65, 65, 70, 65, 65, 70, 65, 65, 70, 65, 
		65, 70, 65, 65, 70, 65, 64, 70, 65, 64, 70, 65, 
		64, 70, 65, 64, 70, 64, 64, 70, 64, 64, 70, 64, 
		64, 70, 64, 64, 70, 64, 64, 70, 64, 64, 70, 64, 
		64, 70, 64, 63, 70, 64, 63, 70, 64, 63, 70, 64, 
		63, 70, 64, 63, 70, 63, 63, 70, 63, 63, 70, 63, 
		63, 70, 63, 63, 70, 63, 63, 70, 63, 63, 70, 63, 
		62, 70, 63, 62, 70, 63, 62, 70, 63, 62, 70, 63, 
		62, 70, 63, 62, 70, 63, 62, 70, 62, 62, 70, 62, 
		62, 69, 62, 62, 69, 62, 62, 69, 62, 61, 69, 62, 
		61, 69, 62, 61, 69, 62, 61, 69, 62, 61, 69, 62, 
		61, 69, 62, 61, 69, 62, 61, 69, 62, 61, 69, 61, 
		61, 69, 61, 61, 69, 61, 61, 69, 61, 60, 69, 61, 
		60, 69, 61, 60, 69, 61, 60, 69, 61, 60, 69, 61, 
		60, 69, 61, 60, 69, 61, 60, 69, 61, 60, 69, 61, 
		60, 69, 61, 60, 69, 60, 59, 69, 60, 59, 69, 60, 
		59, 69, 60, 59, 69, 60, 59, 69, 60, 59, 69, 60, 
		59, 69, 60, 59, 69, 60, 59, 69, 60, 59, 69, 60, 
		59, 69, 60, 59, 69, 60, 58, 69, 59, 58, 69, 59, 
		58, 69, 59, 58, 69, 59, 58, 69, 59, 58, 69, 59, 
		58, 69, 59, 58, 69, 59, 58, 69, 59, 58, 69, 59, 
		58, 69, 59, 57, 69, 59, 57, 69, 59, 57, 69, 59, 
		57, 69, 59, 57, 69, 58, 57, 69, 58, 57, 69, 58, 
		57, 69, 58, 57, 69, 58, 57, 69, 58, 57, 69, 58, 
		57, 69, 58, 56, 69, 58, 56, 69, 58, 56, 69, 58, 
		56, 69, 58, 56, 69, 58, 56, 69, 58, 56, 69, 57, 
		56, 69, 57, 56, 69, 57, 56, 69, 57, 56, 69, 57, 
		55, 69, 57, 55, 69, 57, 55, 68, 57, 55, 68, 57, 
		55, 68, 57, 55, 68, 57, 55, 68, 57, 55, 68, 57, 
		55, 68, 57, 55, 68, 57, 55, 68, 56, 55, 68, 56, 
		54, 68, 56, 54, 68, 56, 54, 68, 56, 54, 68, 56, 
		54, 68, 56, 54, 68, 56, 54, 68, 56, 54, 68, 56, 
		54, 68, 56, 54, 68, 56, 54, 68, 56, 54, 68, 56, 
		53, 68, 56, 53, 68, 56, 53, 68, 55, 53, 68, 55, 
		53, 68, 55, 53, 68, 55, 53, 68, 55, 53, 68, 55, 
		53, 68, 55, 53, 68, 55, 53, 68, 55, 52, 68, 55, 
		52, 68, 55, 52, 68, 55, 52, 68, 55, 52, 68, 55, 
		52, 68, 55, 52, 68, 55, 52, 68, 54, 52, 68, 54, 
		52, 68, 54, 52, 68, 54, 52, 68, 54, 51, 68, 54, 
		51, 68, 54, 51, 68, 54, 51, 68, 54, 51, 68, 54, 
		51, 68, 54, 51, 68, 54, 51, 68, 54, 51, 68, 54, 
		51, 67, 54, 51, 67, 54, 51, 67, 54, 50, 67, 53, 
		50, 67, 53, 50, 67, 53, 50, 67, 53, 50, 67, 53, 
		50, 67, 53, 50, 67, 53, 50, 67, 53, 50, 67, 53, 
		50, 67, 53, 50, 67, 53, 50, 67, 53, 49, 67, 53, 
		49, 67, 53, 49, 67, 53, 49, 67, 53, 49, 67, 53, 
		49, 67, 53, 49, 67, 52, 49, 67, 52, 49, 67, 52, 
		49, 67, 52, 49, 67, 52, 49, 67, 52, 48, 67, 52, 
		48, 67, 52, 48, 67, 52, 48, 67, 52, 48, 67, 52, 
		48, 67, 52, 48, 67, 52, 48, 67, 52, 48, 67, 52, 
		48, 67, 52, 48, 67, 52, 48, 67, 52, 47, 67, 51, 
		47, 67, 51, 47, 67, 51, 47, 67, 51, 47, 67, 51, 
		47, 67, 51, 47, 67, 51, 47, 67, 51, 47, 66, 51, 
		47, 66, 51, 47, 66, 51, 47, 66, 51, 46, 66, 51, 
		46, 66, 51, 46, 66, 51, 46, 66, 51, 46, 66, 51, 
		46, 66, 51, 46, 66, 51, 46, 66, 50, 46, 66, 50, 
		46, 66, 50, 46, 66, 50, 46, 66, 50, 45, 66, 50, 
		45, 66, 50, 45, 66, 50, 45, 66, 50, 45, 66, 50, 
		45, 66, 50, 45, 66, 50, 45, 66, 50, 45, 66, 50, 
		45, 66, 50, 45, 66, 50, 45, 66, 50, 44, 66, 50, 
		44, 66, 50, 44, 66, 50, 44, 66, 49, 44, 66, 49, 
		44, 66, 49, 44, 66, 49, 44, 66, 49, 44, 66, 49, 
		44, 66, 49, 44, 66, 49, 44, 66, 49, 43, 66, 49, 
		43, 65, 49, 43, 65, 49, 43, 65, 49, 43, 65, 49, 
		43, 65, 49, 43, 65, 49, 43, 65, 49, 43, 65, 49, 
		43, 65, 49, 43, 65, 49, 43, 65, 49, 42, 65, 48, 
		42, 65, 48, 42, 65, 48, 42, 65, 48, 42, 65, 48, 
		42, 65, 48, 42, 65, 48, 42, 65, 48, 42, 65, 48, 
		42, 65, 48, 42, 65, 48, 42, 65, 48, 41, 65, 48, 
		41, 65, 48, 41, 65, 48, 41, 65, 48, 41, 65, 48, 
		41, 65, 48, 41, 65, 48, 41, 65, 48, 41, 65, 48, 
		41, 65, 48, 41, 65, 47, 40, 65, 47, 40, 65, 47, 
		40, 65, 47, 40, 65, 47, 40, 64, 47, 40, 64, 47, 
		40, 64, 47, 40, 64, 47, 40, 64, 47, 40, 64, 47, 
		40, 64, 47, 40, 64, 47, 39, 64, 47, 39, 64, 47, 
		39, 64, 47, 39, 64, 47, 39, 64, 47, 39, 64, 47, 
		39, 64, 47, 39, 64, 47, 39, 64, 47, 39, 64, 47, 
		39, 64, 46, 39, 64, 46, 38, 64, 46, 38, 64, 46, 
		38, 64, 46, 38, 64, 46, 38, 64, 46, 38, 64, 46, 
		38, 64, 46, 38, 64, 46, 38, 64, 46, 38, 64, 46, 
		38, 64, 46, 38, 64, 46, 37, 64, 46, 37, 64, 46, 
		37, 64, 46, 37, 63, 46, 37, 63, 46, 37, 63, 46, 
		37, 63, 46, 37, 63, 46, 37, 63, 46, 37, 63, 46, 
		37, 63, 46, 37, 63, 45, 36, 63, 45, 36, 63, 45, 
		36, 63, 45, 36, 63, 45, 36, 63, 45, 36, 63, 45, 
		36, 63, 45, 36, 63, 45, 36, 63, 45, 36, 63, 45, 
		36, 63, 45, 35, 63, 45, 35, 63, 45, 35, 63, 45, 
		35, 63, 45, 35, 63, 45, 35, 63, 45, 35, 63, 45, 
		35, 63, 45, 35, 63, 45, 35, 63, 45, 35, 63, 45, 
		35, 63, 45, 34, 62, 45, 34, 62, 45, 34, 62, 45, 
		34, 62, 44, 34, 62, 44, 34, 62, 44, 34, 62, 44, 
		34, 62, 44, 34, 62, 44, 34, 62, 44, 34, 62, 44, 
		33, 62, 44, 33, 62, 44, 33, 62, 44, 33, 62, 44, 
		33, 62, 44, 33, 62, 44, 33, 62, 44, 33, 62, 44, 
		33, 62, 44, 33, 62, 44, 33, 62, 44, 32, 62, 44, 
		32, 62, 44, 32, 62, 44, 32, 62, 44, 32, 62, 44, 
		32, 62, 44, 32, 62, 44, 32, 61, 44, 32, 61, 44, 
		32, 61, 44, 32, 61, 43, 32, 61, 43, 31, 61, 43, 
		31, 61, 43, 31, 61, 43, 31, 61, 43, 31, 61, 43, 
		31, 61, 43, 31, 61, 43, 31, 61, 43, 31, 61, 43, 
		31, 61, 43, 30, 61, 43, 30, 61, 43, 30, 61, 43, 
		30, 61, 43, 30, 61, 43, 30, 61, 43, 30, 61, 43, 
		30, 61, 43, 30, 61, 43, 30, 61, 43, 30, 61, 43, 
		29, 61, 43, 29, 60, 43, 29, 60, 43, 29, 60, 43, 
		29, 60, 43, 29, 60, 43, 29, 60, 43, 29, 60, 43, 
		29, 60, 43, 29, 60, 42, 29, 60, 42, 28, 60, 42, 
		28, 60, 42, 28, 60, 42, 28, 60, 42, 28, 60, 42, 
		28, 60, 42, 28, 60, 42, 28, 60, 42, 28, 60, 42, 
		28, 60, 42, 27, 60, 42, 27, 60, 42, 27, 60, 42, 
		27, 60, 42, 27, 60, 42, 27, 59, 42, 27, 59, 42, 
		27, 59, 42, 27, 59, 42, 27, 59, 42, 26, 59, 42, 
		};



	signed char R1, R2, R3, R4;
	short angle, angle2, r1, r2, r3, pa_number;
	
	angle = getServoAngle();
	
	if ( angle < 20 && angle > -20 ) {
		motor_f( pwm, pwm );
		motor_r( pwm, pwm );
	} else {
		angle2 = angle;
		// 負なら正にする
		if ( angle2 < 0 ) angle2 = -angle2;
		pa_number = angle2 * 3;		// 配列を3の倍数から始めたい
		
		if ( pwm >= 0 ) {
			r1 = rev_difference_D[ pa_number ];
			r2 = rev_difference_D[ pa_number + 1 ];
			r3 = rev_difference_D[ pa_number + 2 ];
			
			R1 = r1 * pwm / 100;
			R2 = r2 * pwm / 100;
			R3 = r3 * pwm / 100;
			R4 = pwm;
			
			if ( angle >= 0 ) {
				motor_f( R1, R3 );
				motor_r( R2, R4 );
			} else {
				motor_f( R3, R1 );
				motor_r( R4, R2 );
			}
		} else {
			r1 = rev_difference_B[ pa_number ];
			r2 = rev_difference_B[ pa_number + 1 ];
			r3 = rev_difference_B[ pa_number + 2 ];
			
			R1 = r1 * pwm / 100;
			R2 = r2 * pwm / 100;
			R3 = r3 * pwm / 100;
			R4 = pwm;
			
			if ( angle >= 0 ) {
				motor_f( R3, R4 );
				motor_r( R1, R2 );
			} else {
				motor_f( R4, R3 );
				motor_r( R2, R1 );
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getTurningAngleEnc						//
// 処理概要   	ポテンションメータとエンコーダから旋回角度の計算	//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void getTurningAngleEnc(void)
{
	const unsigned int rev_radius[] = {       // 旋回半径テーブル　エンコーダまでの距離を足す	
		0, 193477, 73051, 45009, 32516, 25447, 20899, 17727, 
		15390, 13595, 12175, 11022, 10067, 9264, 8579, 7988, 
		7473, 7019, 6618, 6259, 5937, 5646, 5382, 5141, 
		4921, 4719, 4532, 4360, 4200, 4051, 3912, 3782, 
		3661, 3547, 3440, 3338, 3243, 3153, 3067, 2986, 
		2910, 2836, 2767, 2700, 2637, 2577, 2519, 2463, 
		2410, 2360, 2311, 2264, 2219, 2175, 2134, 2093, 
		2055, 2017, 1981, 1946, 1913, 1880, 1848, 1818, 
		1788, 1760, 1732, 1705, 1679, 1653, 1629, 1605, 
		1581, 1559, 1536, 1515, 1494, 1474, 1454, 1435, 
		1416, 1397, 1379, 1362, 1345, 1328, 1312, 1296, 
		1280, 1265, 1250, 1236, 1221, 1207, 1194, 1180, 
		1167, 1154, 1142, 1130, 1118, 1106, 1094, 1083, 
		1072, 1061, 1050, 1040, 1029, 1019, 1009, 999, 
		990, 980, 971, 962, 953, 944, 935, 927, 
		919, 910, 902, 894, 886, 879, 871, 864, 
		856, 849, 842, 835, 828, 821, 814, 808, 
		801, 795, 788, 782, 776, 770, 764, 758, 
		752, 746, 741, 735, 730, 724, 719, 713, 
		708, 703, 698, 693, 688, 683, 678, 673, 
		669, 664, 659, 655, 650, 646, 641, 637, 
		633, 628, 624, 620, 616, 612, 608, 604, 
		600, 596, 592, 588, 585, 581, 577, 574, 
		570, 567, 563, 560, 556, 553, 549, 546, 
		543, 539, 536, 533, 530, 526, 523, 520, 
		517, 514, 511, 508, 505, 502, 499, 497, 
		494, 491, 488, 485, 483, 480, 477, 475, 
		472, 469, 467, 464, 462, 459, 457, 454, 
		452, 449, 447, 444, 442, 440, 437, 435, 
		433, 430, 428, 426, 424, 421, 419, 417, 
		415, 413, 411, 408, 406, 404, 402, 400, 
		398, 396, 394, 392, 390, 388, 386, 384, 
		382, 380, 379, 377, 375, 373, 371, 369, 
		368, 366, 364, 362, 360, 359, 357, 355, 
		354, 352, 350, 348, 347, 345, 344, 342, 
		340, 339, 337, 335, 334, 332, 331, 329, 
		328, 326, 325, 323, 322, 320, 319, 317, 
		316, 314, 313, 311, 310, 308, 307, 306, 
		304, 303, 301, 300, 299, 297, 296, 295, 
		293, 292, 291, 289, 288, 287, 286, 284, 
		283, 282, 280, 279, 278, 277, 275, 274, 
		273, 272, 271, 269, 268, 267, 266, 265, 
		263, 262, 261, 260, 259, 258, 257, 255, 
		254, 253, 252, 251, 250, 249, 248, 247, 
		245, 244, 243, 242, 241, 240, 239, 238, 
		237, 236, 235, 234, 233, 232, 231, 230, 
		229, 228, 227, 226, 225, 224, 223, 222, 
		221, 220, 219, 218, 217, 216, 215, 214, 
		213, 212, 211, 210, 210, 209, 208, 207, 
		206, 205, 204, 203, 202, 201, 200, 200, 
		199, 198, 197, 196, 195, 194, 193, 193, 
		192, 191, 190, 189, 188, 187, 187, 186, 
		185, 184, 183, 182, 182, 181, 180, 179, 
		178, 178, 177, 176, 175, 174, 174, 173, 
		172, 171, 170, 170, 169, 168, 167, 166, 
		166, 165, 164, 163, 163, 162, 161, 160, 
		160, 159, 158, 157, 156, 156, 155, 154, 
		153, 153, 152, 151, 151, 150, 149, 148, 
		148, 147, 146, 145, 145, 144, 143, 143, 
		142, 141, 140, 140, 139, 138, 138, 137, 
		136, 135, 135, 134, 133, 133, 132, 131, 
		130, 130, 129, 128, 128, 127, 126, 126, 
		};
	int r1;
	short angle,v;
	double	angularVelocity;	// 理論角速度[rad/s]
	
	if ( pattern != 11 ) {
		angle = getServoAngle();	// ステアリング角取得
		if ( angle < 0 ) {
			angle = -angle;// 負なら正にする
			r1 = rev_radius[angle] + RIGHTCURVE_ENCODER;	// 後輪からエンコーダまでの距離を足す
		} else {
			r1 = rev_radius[angle] + LEFTCURVE_ENCODER;	// 後輪からエンコーダまでの距離を足す
		}
		v = Encoder;	// 速度取得
		if ( angle != 0 ) {
			angularVelocity = (double)( 180 * v) / (r1 * SPEED_CURRENT_DETAIL * PI);// 角速度計算 ω = (v/r)*(180/pi)
		} else {
			angularVelocity = 0;
		}
		
		TurningAngleEnc += angularVelocity;
	} else {
		TurningAngleEnc += 0;
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControl							//
// 処理概要     モーターのPWM決計算						//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void motorControl( void )
{
	int i, j, iRet, Dif, iP, iI, iD, Dev;
	char kp3, ki3, kd3, v;
	const char rev_voltage[] = {       // 目標速度でのモーターの電圧テーブル	
		0, 2, 3, 5, 6, 8, 9, 11, 
		12, 14, 15, 17, 18, 19, 21, 22, 
		24, 25, 27, 28, 30, 31, 33, 34, 
		36, 37, 38, 40, 41, 43, 44, 46, 
		47, 49, 50, 52, 53, 54, 56, 57, 
		59, 60, 62, 63, 65, 66, 68, 69, 
		70, 72, 73, 75, 76, 78, 79, 81, 
		82, 84, 85, 87, 88, 89, 91, 92, 
		94, 95, 97, 98, 100, 101, 103, 104, 
		105, 107, 108, 110, 111, 113, 114, 116
		};
	
	i = targetSpeed;		// 目標値
	j = Encoder * 10;		// 現在値
	
	// デモモードのときゲイン変更
	if ( demo == 1 ) {
		kp3 = 49;
		ki3 = 41;
		kd3 = 0;
	} else if ( demo == 0 ) {
		kp3 = kp3_buff;
		ki3 = ki3_buff;
		kd3 = kd3_buff;
	}
	
	// 駆動モーター用PWM値計算
	Dev = i - j;	// 偏差
	
	if ( Dev > 50 || Dev < -50) {
		// 目標値を超えたらI成分リセット
		if ( Dev >= 0 && AccelefBefore == 1 ) Int3 = 0;
		else if ( Dev < 0 && AccelefBefore == 0 ) Int3 = 0;
		
		// 目標値を変更したらI成分リセット
		if ( i != targetSpeedBefore ) Int3 = 0;
		
		Int3 += (double)Dev * 0.001;		// 積分
		Dif = Dev - EncoderBefore;		// 微分　dゲイン1/1000倍
		
		iP = (int)kp3 * Dev;			// 比例
		iI = (double)ki3 * Int3;		// 積分
		iD = (int)kd3 * Dif;			// 微分
		iRet = iP + iI + iD;
		iRet = iRet >> 4;
		
		// PWMの上限の設定
		if ( iRet >  100 )	iRet =  100;
		if ( iRet <  -100 )	iRet = -100;
	} else {
		v = rev_voltage[targetSpeed / SPEED_CURRENT];
		if ( Dev < 0 ) v = -v;
		iRet = v;
	}
	
	if ( Dev > 0 )	AccelefBefore = 0;
	else		AccelefBefore = 1;
	
	motorPwm = iRet;
	EncoderBefore = Dev;
	targetSpeedBefore = i;
}