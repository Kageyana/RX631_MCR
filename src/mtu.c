//====================================//
// �C���N���[�h
//====================================//
#include "mtu.h"
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
// �G���R�[�_�֘A
static unsigned short 	cnt_Encoder;	// �G���R�[�_�l�̊i�[��
static unsigned short	encbuff;		// �O��̃G���R�[�_�l
short					Encoder;			// 1ms���Ƃ̃p���X
unsigned int			EncoderTotal;		// �����s����
unsigned int			enc1;				// ���s�p�����J�E���g
unsigned int			enc_slope;			// ��㋗���J�E���g
// ���[�^�[�֘A
signed char		accele_fR;		// �E�O���[�^�[PWM�l
signed char		accele_fL;		// ���O���[�^�[PWM�l
signed char		accele_rR;		// �E�ヂ�[�^�[PWM�l
signed char		accele_rL;		// ���ヂ�[�^�[PWM�l
signed char		sPwm;			// �T�[�{���[�^�[PWM�l
/////////////////////////////////////////////////////////////////////////////////
// ���W���[���� getEncoder
// �����T�v     �G���R�[�_�̃J�E���g���擾���ώZ����(1ms���ƂɎ��s)
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////////////////
void getEncoder (void)
{
	ENCODER_COUNT			// �G���R�[�_�J�E���g�l�擾
	Encoder = cnt_Encoder - encbuff;// ���ݒn����1ms�O�̒l��������1ms�Ԃ̃J�E���g���v�Z
	
	// �ώZ
	EncoderTotal += Encoder;
	enc1 += Encoder;
	enc_slope += Encoder;
	
	encbuff = cnt_Encoder;	// ����͂��̒l��1ms�O�̒l�ƂȂ�
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� motor_f
// �����T�v     ���[�^�[��PWM�̕ύX
// ����         accelefL, accelefR(PWM��1�`100%�Ŏw��)
// �߂�l       ��
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
	
	// ���O��
	if( accelefL >= 0) DIR_FL_FOR
	else DIR_FL_REV
	if ( accelefL == 100 || accelefL == -100 )pwmfl = TGR_MOTOR + 2;
	PWM_FL_OUT
	
	// �E�O��
	if( accelefR >= 0) DIR_FR_FOR
	else DIR_FR_REV
	if ( accelefR == 100 || accelefR == -100 ) pwmfr = TGR_MOTOR + 2;
	PWM_FR_OUT

	// �����
	if( accelerL >= 0 ) DIR_RL_FOR
	else DIR_RL_REV
	if ( accelerL == 100 || accelerL == -100 ) pwmrl = TGR_MOTOR + 2;
	PWM_RL_OUT
	
	// �E���
	if( accelerR >= 0 ) DIR_RR_FOR
	else DIR_RR_REV
	if ( accelerR == 100 || accelerR == -100 ) pwmrr = TGR_MOTOR + 2;
	PWM_RR_OUT
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� servoPwmOut
// �����T�v     �����g���[�X���T�[�{��PWM�̕ύX
// ����         spwm
// �߂�l       �Ȃ�
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