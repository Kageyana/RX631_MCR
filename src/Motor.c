//====================================//
// インクルード									//
//====================================//
#include "motor.h"
//====================================//
// グローバル変数の宣言								//
//====================================//
// モーター関連
signed char		accele_fR;		// 右前モーターPWM値
signed char		accele_fL;		// 左前モーターPWM値
signed char		accele_rR;		// 右後モーターPWM値
signed char		accele_rL;		// 左後モーターPWM値
signed char		sPwm;		// サーボモーターPWM値

/////////////////////////////////////////////////////////////////////
// モジュール名 motor_f								//
// 処理概要     モーターのPWMの変更						//
// 引数         accelefL, accelefR(PWMを1～100%で指定)		//
// 戻り値       なし									//
/////////////////////////////////////////////////////////////////////
void motor_f( signed char accelefL, signed char accelefR )
{
	uint16_t pwmfl, pwmfr;
	
	if ( pushcart_mode ) {
		accelefR = 0;
		accelefL = 0;
	}
	
	accele_fR = accelefR;
	accele_fL = accelefL;
	
	pwmfl = TGR_MOTOR * accelefL / 100;
	pwmfr = TGR_MOTOR * accelefR / 100;
	
	// 左前輪
	if( accelefL >= 0) {					
		// 正転
		DIR_FL_FOR
	} else if ( accelefL == 100 || accelefL == -100 ) {	
		// 100%
		if ( accelefL > 0 ) DIR_FL_FOR
		else DIR_FL_REV
		
		pwmfl = TGR_MOTOR + 2;
	} else {						
		// 逆転
		pwmfl = -pwmfl;
		DIR_FL_REV
	}
	PWM_FL_OUT
	
	// 右前輪
	if( accelefR >= 0) {					
		// 正転
		DIR_FR_FOR
	} else if ( accelefR == 100 || accelefR == -100 ) {	
		// 100%
		if ( accelefR > 0 ) DIR_FR_FOR
		else DIR_FR_REV
		
		pwmfr = TGR_MOTOR + 2;
	} else {						
		// 逆転
		pwmfr = -pwmfr;
		DIR_FR_REV
	}
	PWM_FR_OUT
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motor_r									//
// 処理概要     モーターのPWMの変更							//
// 引数         acelerL, accelerR(PWMを1～100%で指定)			//
// 戻り値       なし										//
///////////////////////////////////////////////////////////////////////////
void motor_r( signed char accelerL, signed char accelerR )
{
	uint16_t pwmrl, pwmrr;
	
	if ( pushcart_mode ) {
		accelerL = 0;
		accelerR = 0;
	}
	
	accele_rR = accelerR;
	accele_rL = accelerL;
	
	pwmrl = TGR_MOTOR * accelerL / 100;
	pwmrr = TGR_MOTOR * accelerR / 100;
	
	// 左後輪
	if( accelerL >= 0 ) {					
		// 正転
		DIR_RL_FOR
	} else if ( accelerL == 100 || accelerL == -100 ) {	
		// 100%
		if (accelerL > 0) DIR_RL_FOR
		else DIR_RL_REV
		
		pwmrl = TGR_MOTOR + 2;
	} else {						
		// 逆転
		pwmrl = -pwmrl;
		DIR_RL_REV
	}
	PWM_RL_OUT
	
	// 右後輪
	if( accelerR >= 0 ) {					
		// 正転
		DIR_RR_FOR
	} else if ( accelerR == 100 || accelerR == -100 ) {	
		// 100%
		if ( accelerR > 0 ) DIR_RR_FOR
		else DIR_RR_REV
		
		pwmrr = TGR_MOTOR + 2;
	} else {						
		// 逆転
		pwmrr = -pwmrr;
		DIR_RR_REV
	}
	PWM_RR_OUT
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 servoPwmOut								//
// 処理概要     白線トレース時サーボのPWMの変更					//
// 引数         spwm									//
// 戻り値       なし										//
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