//====================================//
// インクルード
//====================================//
#include "control.h"
//====================================//
// グローバル変数の宣言
//====================================//
// モード関連
char 	modeLCD = 1;		// LCD表示可否		1:表示		0:消灯		
char 	modeSlope;			// 坂チェック		0:上り坂始め	1:上り坂終わり	2:下り坂始め
char 	modeAngle;			// サーボPWM変更	0:白線トレース	1:角度制御
char	modePushcart;		// 手押しモード可否	0:自動走行	1:手押し
char	msdset;				// MicroSDが初期化されたか	0:初期化失敗	1:初期化成功
char	IMUSet = 0;			// IMUが初期化されたか		0: 初期化失敗	1:初期化成功

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
short	speed_ckank_trace;			// クランク進入速度
short	speed_rightclank_curve;		// 右クランク旋回速度
short	speed_rightclank_escape;	// 右クランク復帰速度
short	speed_leftclank_curve;		// 左クランク旋回速度
short	speed_leftclank_escape;		// 左クランク復帰速度

short	speed_halfine;				// ハーフライン進入速度
short	speed_rightchange_trace;	// 右レーンチェンジ進入速度
short	speed_rightchange_curve;	// 右レーンチェンジ旋回速度
short	speed_rightchange_escape;	// 右レーンチェンジ復帰速度

short	speed_leftchange_trace;		// 左レーンチェンジ進入速度
short	speed_leftchange_curve;		// 左レーンチェンジ旋回速度
short	speed_leftchange_escape;	// 左レーンチェンジ旋回速度

short	speed_slope_brake;			// 下り坂終点速度
short	speed_slope_trace;			// 坂読み飛ばし速度

// サーボ角度
short	angle_rightclank;		// 右クランク旋回角度
short	angle_leftclank;		// 左クランク旋回角度
short	angle_rightchange;		// 右レーンチェンジ旋回角度
short	angle_leftchange;		// 右レーンチェンジ旋回角度

// タイマ関連
short		cntGyro;			// 角度計算用カウンタ

// 角度関連
double 		TurningAngleEnc;	// エンコーダから求めた旋回角度
double		PichAngleAD;		// アナログジャイロから求めたピッチ角度
double		gyVoltageBefore;

// サーボ関連
// 白線トレース
signed char	ServoPwm;		// 白線トレースサーボPWM
short 		SensorBefore;	// 1ms前のセンサ値
char		DevBefore;		// I成分リセット用
double		Int;			// I成分積算値(白線トレース)
// 角度制御
signed char	ServoPwm2;		// 角度サーボPWM
short 		SetAngle;		// 目標角度
short		SetAngleBefore;	// 1ms前の目標角度
short 		AngleBefore2;	// 1ms前の角度
char		AngleBefore3;	// I成分リセット用
double		Int2;			// I成分積算値(角度制御)

// モーター関連
signed char 	motorPwm;			// モーター制御PWM
short			targetSpeed;		// 目標速度
bool			stableSpeed = false;
unsigned int	encStable = 0;
short			cntStable = 0;
char 			AccelefBefore;		// I成分リセット用
short			EncoderBefore;		// 1ms前の速度
int 			targetSpeedBefore;	// 1ms前の目標速度	
double 			Int3;				// I成分積算値(速度制御)

// デモ関連
char 	demo;

// ゲイン関連
char	kp_buff, ki_buff, kd_buff;
char	kp2_buff, ki2_buff, kd2_buff;
char 	kp3_buff, ki3_buff, kd3_buff;

///////////////////////////////////////////////////////////////////////////
// モジュール名 checkCrossLine
// 処理概要     クロスライン検知
// 引数         なし
// 戻り値       0:クロスラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkCrossLine( void )
{
	if ( sensor_inp() == 0x7 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkRightLine
// 処理概要     右ハーフライン検出処理
// 引数         なし
// 戻り値       0:右ハーフラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkRightLine( void )
{
	if ( sensor_inp() == 0x3 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkLeftLine
// 処理概要     左ハーフライン検出処理
// 引数         なし
// 戻り値       0:左ハーフラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkLeftLine( void )
{
	if ( sensor_inp() == 0x6 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkSlope
// 処理概要     ジャイロセンサの値から坂道検出
// 引数         なし
// 戻り値       0:坂道なし 1:上り坂　-1:下り坂
///////////////////////////////////////////////////////////////////////////
signed char checkSlope( void )
{
	signed char ret = 0;

	if ( PichAngleIMU >= SLOPE_UPPERLINE_IMU ) ret = 1;
	if ( PichAngleIMU <= SLOPE_LOWERLINE_IMU ) ret = -1;
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 encMM
// 処理概要     mmをエンコーダのパルス数に変換して返す
// 引数         mm:変換する長さ[mm]
// 戻り値       変換したパルス数
///////////////////////////////////////////////////////////////////////////
unsigned int encMM( short mm )
{
	return PALSE_MILLIMETER * mm;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 stableSpeedDistance
// 処理概要     目標速度に達してからの時間と距離を計測する
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
unsigned int stableSpeedDistance( void )
{
	if ( Encoder*10 >= targetSpeed ) stableSpeed = true;
	if (stableSpeed) {
		encStable += Encoder;
		cntStable++;
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 servoControlTrace
// 処理概要     ライントレース時サーボのPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void servoControlTrace( void )
{
	int iP, iD, iI, iRet, maxpwm;
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
	iRet = iRet >> 10;				// PWMを0～100近傍に収める

	// PWMの上限の設定
	// 出力電圧がVOLTAGELIMとなるDuty比を計算
	maxpwm = (int8_t)(VOLTAGELIMTRACE / Voltage *100);

	if ( iRet > maxpwm ) iRet =  maxpwm;
	if ( iRet < -maxpwm ) iRet = -maxpwm;

	if ( iRet >  100 ) iRet =  100;
	if ( iRet <  -100 ) iRet = -100;
	
	if ( Dev >= 0 )	DevBefore = 0;
	else			DevBefore = 1;
	ServoPwm = iRet;
	SensorBefore = Dev;				// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 servoControlAngle
// 処理概要     角度制御時サーボのPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void servoControlAngle( void )
{
	short i, j, Dev, Dif;
	int iP, iD, iI, iRet, maxpwm;
	
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
	iI = (double)ki2_buff * Int2;	// 積分
	iD = (int)kd2_buff * Dif;		// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 4;		// PWMを0～100近傍に収める

	// PWMの上限の設定
	// 出力電圧がVOLTAGELIMとなるDuty比を計算
	maxpwm = (int8_t)(VOLTAGELIMTRACE / Voltage *100);

	if ( iRet > maxpwm ) iRet =  maxpwm;
	if ( iRet < -maxpwm ) iRet = -maxpwm;
	if ( iRet >  100 ) iRet =  100;
	if ( iRet <  -100 ) iRet = -100;

	if ( Dev >= 0 ) 	AngleBefore3 = 0;
	else 				AngleBefore3 = 1;
	SetAngleBefore = i;
	ServoPwm2 = iRet;
	AngleBefore2 = Dev;			// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 diff
// 処理概要   	R1,R2,R3及びR4の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void diff ( signed char pwm )
{
	const char rev_difference_D[] = {       // 角度から内輪、外輪回転差計算
		100,100,100,99,100,99,99,100,99,98,100,98,97,100,97,
		96,100,97,95,99,96,95,99,95,94,99,95,93,99,94,
		92,98,93,91,98,93,90,98,92,89,97,91,88,97,91,
		87,96,90,86,96,89,84,95,89,83,94,88,82,94,88,
		81,93,87,80,92,86,78,92,86,77,91,85,76,90,84,
		75,89,84,73,88,83,72,87,83,70,86,82,69,85,81,
		67,83,81,66,82,80,64,81,80,63,79,79,61,78,79,
		59,76,78,58,75,77,56,73,77,54,71,76,52,69,76,
		50,67,75,48,65,74,46,62,74,44,60,73,41,57,73,
		38,53,72,35,49,71,31,44,71,25,36,70,19,28,69,
		19,28,68,19,29,68,19,29,68,19,29,68,19,29,67,
		19,30,67,19,30,67,19,30,67,19,30,66,19,31,66,
		19,31,66,19,31,66,19,31,65,19,32,65,18,32,65,
		18,32,65,18,32,65,18,32,65,18,32,64,18,33,64,
		18,33,64,18,33,64,18,33,64,18,33,64,18,33,64,
		18,34,63,18,34,63,18,34,63,18,34,63,18,34,63,
		18,34,63,18,34,63,18,34,63,18,34,63,
	};

	signed char R1, R2, R3, R4;
	short angle2, r1, r2, r3;
	double angle;
	
	angle = (double)getServoAngle()*AD2DEG;		// AD値を角度に変換
	angle2 = abs((short)angle* 3);
	
	if ( pwm >= 0 ) {
		r1 = rev_difference_D[ angle2 ];
		r2 = rev_difference_D[ angle2 + 1 ];
		r3 = rev_difference_D[ angle2 + 2 ];
		
		R1 = r1 * pwm / 100;
		R2 = r2 * pwm / 100;
		R3 = r3 * pwm / 100;
		R4 = pwm;
		
		if ( angle >= 0 ) {
			motorPwmOut(R1, R3, R2, R4);
		} else {
			motorPwmOut(R3, R1, R4, R2);
		}
	} else {
		r1 = rev_difference_D[ angle2 ];
		r2 = rev_difference_D[ angle2 + 1 ];
		r3 = rev_difference_D[ angle2 + 2 ];
		
		R1 = r1 * pwm / 100;
		R2 = r2 * pwm / 100;
		R3 = r3 * pwm / 100;
		R4 = pwm;
		
		if ( angle >= 0 ) {
			motorPwmOut(R3, R4, R1, R2);
		} else {
			motorPwmOut(R4, R3, R2, R1);
		}
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControl
// 処理概要     モーターのPWM決計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControl( void )
{
	int i, j, iRet, Dif, iP, iI, iD, Dev, maxpwm;
	char kp3, ki3, kd3;
	
	i = targetSpeed;		// 目標値
	j = Encoder * 10;		// 現在値 targetSpeedはエンコーダのパルス数*10のため
							// 現在位置も10倍する

	// デモモードのときゲイン変更
	if ( demo ) {
		kp3 = kp3_buff;
		ki3 = ki3_buff;
		kd3 = kd3_buff;
	} else {
		kp3 = kp3_buff;
		ki3 = ki3_buff;
		kd3 = kd3_buff;
	}
	
	// 駆動モーター用PWM値計算
	Dev = i - j;	// 偏差
	// 目標値を変更したらI成分リセット
	if ( i != targetSpeedBefore ) Int3 = 0;
	
	Int3 += (double)Dev * 0.001;	// 時間積分
	Dif = Dev - EncoderBefore;		// 微分　dゲイン1/1000倍
	
	iP = (int)kp3 * Dev;			// 比例
	iI = (double)ki3 * Int3;		// 積分
	iD = (int)kd3 * Dif;			// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 4;
	
	// PWMの上限の設定
	// 出力電圧がVOLTAGELIMとなるDuty比を計算
	maxpwm = (int8_t)(VOLTAGELIM / Voltage *100);

	// if ( iRet > maxpwm ) iRet =  maxpwm;
	// if ( iRet < -maxpwm ) iRet = -maxpwm;
	if ( iRet >  100 ) iRet = 100;
	if ( iRet <  -100 ) iRet = -100;
	
	motorPwm = iRet;
	EncoderBefore = Dev;
	targetSpeedBefore = i;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getLinePositionNow
// 処理概要     中央センサの位置を算出する
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
double getLinePositionNow( short angleAD) {
	double angle;
	angle = (double)abs(angleAD) * AD2DEG;	// [°]に変換
	angle = 90.0 - angle;
	angle = angle * 180 / M_PI;			// [rad]に変換
	return WHELLBASE + (SENSORBAR * sin(angle));
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getLinePositionAfter
// 処理概要     旋回後の中央センサの位置を算出する
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
double getLinePositionAfter (short angle) {
	const double turningRadius[] = {       // 角度から内輪、外輪回転差計算
		3637467,3637410,3637467,3637410,8436,8493,8437,8495,4187,4244,4189,4246,2772,2829,2776,2833,2064,2121,2069,2127,
		1640,1696,1646,1703,1356,1413,1364,1421,1153,1210,1163,1220,1001,1058,1012,1069,883,939,894,951,
		787,844,800,858,709,766,724,781,644,700,660,717,589,645,606,663,542,597,560,617,
		500,555,520,577,464,519,485,542,432,486,454,511,403,457,427,484,377,431,402,459,
		354,407,380,437,333,386,360,417,313,366,342,399,296,347,326,383,279,331,311,368,
		264,315,297,354,250,300,284,341,236,286,272,329,224,273,261,318,212,261,251,308,
		201,249,242,299,191,238,233,290,181,227,224,281,172,217,216,274,163,207,209,266,
		154,198,202,259,146,189,195,253,138,180,189,246,130,171,183,240,123,162,178,235,
		115,154,172,229,108,145,167,224,101,136,162,219,93,128,157,214,86,118,152,209,
		78,109,147,205,70,98,143,200,61,86,138,195,48,69,131,189,38,55,125,183,
		41,61,124,181,45,66,122,179,48,70,120,177,50,74,119,176,52,78,117,174,
		54,81,116,173,56,84,115,171,58,87,113,170,59,89,112,169,61,92,111,168,
		62,94,110,167,63,96,109,165,64,98,108,164,65,99,107,163,66,101,106,163,
		67,103,105,162,68,104,104,161,68,106,103,160,69,107,103,159,69,108,102,159,
		70,109,101,158,71,110,101,157,71,111,100,157,71,112,100,156,72,113,99,156,
		72,114,99,155,73,114,98,155,73,115,98,154,73,115,97,154,73,116,97,153,
		74,116,97,153,74,117,96,153,74,117,96,153,74,118,96,152,
		};
	double thetaS, theta1, theta2, theta3, y1, x1, r;
	
	thetaS = (double)angle * 180 / M_PI;		// [rad]に変換
	thetaS = M_PI/2 - thetaS;

	// 各パラメータ算出
	y1 = WHELLBASE + (SENSORBAR * sin(angle));
	x1 = turningRadius[(short)angle*4] + ( (TREAD/2) - SENSORBAR*cos(thetaS) );
	theta2 = atan(x1/y1);
	r = y1/cos(theta2);
	theta1 = M_PI/2 - thetaS;
	theta3 = theta1 - theta2;

	return r * cos(theta3);
}