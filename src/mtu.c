//====================================//
// インクルード
//====================================//
#include "mtu.h"
//====================================//
// グローバル変数の宣言
//====================================//
// エンコーダ関連
static unsigned short 	cnt_Encoder;	// エンコーダ値の格納先
static unsigned short	encbuff;		// 前回のエンコーダ値
short					Encoder;			// 1msごとのパルス
unsigned int			EncoderTotal;		// 総走行距離
unsigned int			enc1;				// 走行用距離カウント
unsigned int			enc_slope;			// 坂上距離カウント
// モーター関連
signed char		accele_fR;		// 右前モーターPWM値
signed char		accele_fL;		// 左前モーターPWM値
signed char		accele_rR;		// 右後モーターPWM値
signed char		accele_rL;		// 左後モーターPWM値
signed char		sPwm;			// サーボモーターPWM値
/////////////////////////////////////////////////////////////////////////////////
// モジュール名 getEncoder
// 処理概要     エンコーダのカウントを取得し積算する(1msごとに実行)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////////////////
void getEncoder (void)
{
	ENCODER_COUNT			// エンコーダカウント値取得
	Encoder = cnt_Encoder - encbuff;// 現在地から1ms前の値を引いて1ms間のカウントを計算
	
	// 積算
	EncoderTotal += Encoder;
	enc1 += Encoder;
	enc_slope += Encoder;
	
	encbuff = cnt_Encoder;	// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motor_f
// 処理概要     モーターのPWMの変更
// 引数         accelefL, accelefR(PWMを1～100%で指定)
// 戻り値       な
///////////////////////////////////////////////////////////////////////////
void motorPwmOut( signed char accelefL, signed char accelefR, signed char accelerL, signed char accelerR )
{
	uint16_t pwmfl, pwmfr, pwmrl, pwmrr;
	
	accele_fR = accelefR;
	accele_fL = accelefL;
	accele_rR = accelerR;
	accele_rL = accelerL;
	
	pwmfl = TGR_MOTOR * accelefL / 100;
	pwmfr = TGR_MOTOR * accelefR / 100;
	pwmrl = TGR_MOTOR * accelerL / 100;
	pwmrr = TGR_MOTOR * accelerR / 100;
	
	// 左前輪
	if( accelefL >= 0) DIR_FL_FOR
	else DIR_FL_REV
	if ( accelefL == 100 || accelefL == -100 )pwmfl = TGR_MOTOR + 2;
	PWM_FL_OUT
	
	// 右前輪
	if( accelefR >= 0) DIR_FR_FOR
	else DIR_FR_REV
	if ( accelefR == 100 || accelefR == -100 ) pwmfr = TGR_MOTOR + 2;
	PWM_FR_OUT

	// 左後輪
	if( accelerL >= 0 ) DIR_RL_FOR
	else DIR_RL_REV
	if ( accelerL == 100 || accelerL == -100 ) pwmrl = TGR_MOTOR + 2;
	PWM_RL_OUT
	
	// 右後輪
	if( accelerR >= 0 ) DIR_RR_FOR
	else DIR_RR_REV
	if ( accelerR == 100 || accelerR == -100 ) pwmrr = TGR_MOTOR + 2;
	PWM_RR_OUT
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 servoPwmOut
// 処理概要     白線トレース時サーボのPWMの変更
// 引数         spwm
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void servoPwmOut( signed char servopwm )
{
	uint16_t pwm;
	short angle;
	
	sPwm = servopwm;		// ログ用変数に代入
	//servopwm = -servopwm;		// 回転方向を変える
	
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
	if( servopwm > 0) {				
		// 正転
		DIR_SERVO_FOR
	} else {				
		// 逆転
		pwm = -pwm;
		DIR_SERVO_REV
	}
	PWM_SERVO_OUT
}