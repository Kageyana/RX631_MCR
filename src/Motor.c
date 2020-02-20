//====================================//
// �C���N���[�h									//
//====================================//
#include "motor.h"
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// ���[�^�[�֘A
signed char		accele_fR;		// �E�O���[�^�[PWM�l
signed char		accele_fL;		// ���O���[�^�[PWM�l
signed char		accele_rR;		// �E�ヂ�[�^�[PWM�l
signed char		accele_rL;		// ���ヂ�[�^�[PWM�l
signed char		sPwm;		// �T�[�{���[�^�[PWM�l

/////////////////////////////////////////////////////////////////////
// ���W���[���� motor_f								//
// �����T�v     ���[�^�[��PWM�̕ύX						//
// ����         accelefL, accelefR(PWM��1�`100%�Ŏw��)		//
// �߂�l       �Ȃ�									//
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
	
	// ���O��
	if( accelefL >= 0) {					
		// ���]
		DIR_FL_FOR
	} else if ( accelefL == 100 || accelefL == -100 ) {	
		// 100%
		if ( accelefL > 0 ) DIR_FL_FOR
		else DIR_FL_REV
		
		pwmfl = TGR_MOTOR + 2;
	} else {						
		// �t�]
		pwmfl = -pwmfl;
		DIR_FL_REV
	}
	PWM_FL_OUT
	
	// �E�O��
	if( accelefR >= 0) {					
		// ���]
		DIR_FR_FOR
	} else if ( accelefR == 100 || accelefR == -100 ) {	
		// 100%
		if ( accelefR > 0 ) DIR_FR_FOR
		else DIR_FR_REV
		
		pwmfr = TGR_MOTOR + 2;
	} else {						
		// �t�]
		pwmfr = -pwmfr;
		DIR_FR_REV
	}
	PWM_FR_OUT
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� motor_r									//
// �����T�v     ���[�^�[��PWM�̕ύX							//
// ����         acelerL, accelerR(PWM��1�`100%�Ŏw��)			//
// �߂�l       �Ȃ�										//
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
	
	// �����
	if( accelerL >= 0 ) {					
		// ���]
		DIR_RL_FOR
	} else if ( accelerL == 100 || accelerL == -100 ) {	
		// 100%
		if (accelerL > 0) DIR_RL_FOR
		else DIR_RL_REV
		
		pwmrl = TGR_MOTOR + 2;
	} else {						
		// �t�]
		pwmrl = -pwmrl;
		DIR_RL_REV
	}
	PWM_RL_OUT
	
	// �E���
	if( accelerR >= 0 ) {					
		// ���]
		DIR_RR_FOR
	} else if ( accelerR == 100 || accelerR == -100 ) {	
		// 100%
		if ( accelerR > 0 ) DIR_RR_FOR
		else DIR_RR_REV
		
		pwmrr = TGR_MOTOR + 2;
	} else {						
		// �t�]
		pwmrr = -pwmrr;
		DIR_RR_REV
	}
	PWM_RR_OUT
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� servoPwmOut								//
// �����T�v     �����g���[�X���T�[�{��PWM�̕ύX					//
// ����         spwm									//
// �߂�l       �Ȃ�										//
///////////////////////////////////////////////////////////////////////////
void servoPwmOut( signed char servopwm )
{
	uint16_t pwm;
	short angle;
	
	sPwm = servopwm;		// ���O�p�ϐ��ɑ��
	//servopwm = -servopwm;		// ��]������ς���
	
	// �T�[�{���~�b�g����
	angle = getServoAngle();
	
	// �p�x�ɂ�郊�~�b�g����
	if ( angle >= SERVO_LIMIT ) servopwm = -15;
	if ( angle <= -SERVO_LIMIT ) servopwm = 15;
	
	// �|�e���V�������[�^�[���O��Ă����琧�䂵�Ȃ�
	if ( angle > SERVO_LIMIT + 100 ) servopwm = 0;
	if ( angle < -SERVO_LIMIT - 100 ) servopwm = 0;

	pwm = (uint16_t)TGR_SERVO * servopwm / 100;
	// �T�[�{���[�^����
	if( servopwm > 0) {				
		// ���]
		DIR_SERVO_FOR
	} else {				
		// �t�]
		pwm = -pwm;
		DIR_SERVO_REV
	}
	PWM_SERVO_OUT
}