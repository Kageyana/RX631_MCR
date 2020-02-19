//====================================//
// インクルード									//
//====================================//
#include "LineChase.h"
//====================================//
// グローバル変数の宣言								//
//====================================//
// モード関連
char 	lcd_mode = 1;		// LCD表示可否		1:表示		0:消灯		
char 	angle_mode;		// サーボPWM変更		0:白線トレース	1:角度制御
char	pushcart_mode;		// 手押しモード可否	0:自動走行	1:手押し

// パラメータ関連
// 距離
short	stopping_meter;			// 停止距離
// 速度
short	speed_straight;			// 通常トレース
short	speed_curve_brake;		// カーブブレーキ
short	speed_curve_r600;		// R600カーブ速度
short	speed_curve_r450;		// R450カーブ速度
short	speed_curve_straight;	// S字カーブ直線速度

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

// ゲイン関連
char	kp_buff, ki_buff, kd_buff;
char	kp2_buff, ki2_buff, kd2_buff;
char kp3_buff, ki3_buff, kd3_buff;

///////////////////////////////////////////////////////////////////////////
// モジュール名 check_crossline							//
// 処理概要     クロスライン検知								//
// 引数         なし										//
// 戻り値       0:クロスラインなし 1:あり							//
///////////////////////////////////////////////////////////////////////////
signed char check_crossline( void )
{
	if ( sensor_inp() == 0x7 ) return 1;
	else return 0;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 check_rightline							//
// 処理概要     右ハーフライン検出処理						//
// 引数         なし										//
// 戻り値       0:右ハーフラインなし 1:あり						//
///////////////////////////////////////////////////////////////////////////
signed char check_rightline( void )
{
	if ( sensor_inp() == 0x3 ) return 1;
	else return 0;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 check_leftline								//
// 処理概要     左ハーフライン検出処理						//
// 引数         なし										//
// 戻り値       0:左ハーフラインなし 1:あり						//
///////////////////////////////////////////////////////////////////////////
signed char check_leftline( void )
{
	if ( sensor_inp() == 0x6 ) return 1;
	else return 0;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 enc_mm									//
// 処理概要     mmをエンコーダのパルス数に変換して返す				//
// 引数         mm:変換する長さ[mm]						//
// 戻り値       変換したパルス数								//
///////////////////////////////////////////////////////////////////////////
unsigned int enc_mm( short mm )
{
	return SPEED_CURRENT * mm;
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
	// I成分積算
	Int += (double)Dev * 0.001;
	if ( Int > 10000 ) Int = 10000;		// I成分リミット
	else if ( Int < -10000 ) Int = -10000;
	Dif = ( Dev - SensorBefore ) * 1;	// dゲイン1/1000倍

	iP = (int)kp_buff * Dev;		// 比例
	iI = (double)ki_buff * Int;		// 積分
	iD = (int)kd_buff * Dif;		// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 10;

	// PWMの上限の設定(安定したら70程度に)
	if ( iRet >  70 ) iRet =  90;		// マイコンカーが安定したら
	if ( iRet <  -70 ) iRet = -90;	// 上限を90くらいにしてください
	
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
	iRet = iRet >> 4;		// PWMを0～100の間に収める

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
// モジュール名 motorControl							//
// 処理概要     モーターのPWM決計算						//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void motorControl( void )
{
	int i, j, iRet, Dif, iP, iI, iD, Dev;
	char kp3, ki3, kd3;
	
	i = targetSpeed;		// 目標値 x[m/s]*10
	j = Encoder * 10;	// 現在値 目標値に合わせて10倍する
	
	// 駆動モーター用PWM値計算
	Dev = i - j;	// 偏差
	// 目標値を変更したらI成分リセット
	if ( i != targetSpeedBefore ) Int3 = 0;
	
	Int3 += (double)Dev * 0.001;	// 積分
	Dif = Dev - EncoderBefore;		// 微分　dゲイン1/1000倍
	
	iP = (int)kp3 * Dev;			// 比例
	iI = (double)ki3 * Int3;		// 積分
	iD = (int)kd3 * Dif;			// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 4;
	
	// PWMの上限の設定
	if ( iRet >  100 )	iRet =  100;
	if ( iRet <  -100 )	iRet = -100;
	
	motorPwm = iRet;
	EncoderBefore = Dev;
	targetSpeedBefore = i;
}