//====================================//
// �C���N���[�h
//====================================//
#include "control.h"
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
// ���[�h�֘A
char 	modeLCD = 1;		// LCD�\����		1:�\��		0:����		
char 	modeSlope;			// ��`�F�b�N		0:����n��	1:����I���	2:�����n��
char 	modeAngle;			// �T�[�{PWM�ύX	0:�����g���[�X	1:�p�x����
char	modePushcart;		// �艟�����[�h��	0:�������s	1:�艟��
char	msdset;				// MicroSD�����������ꂽ��	0:���������s	1:����������
char	IMUSet = 0;			// IMU�����������ꂽ��		0: ���������s	1:����������

// �p�����[�^�֘A
// ����
short	stopping_meter;			// ��~����
// ���x
short	speed_straight;			// �ʏ�g���[�X
short	speed_curve_brake;		// �J�[�u�u���[�L
short	speed_curve_r600;		// R600�J�[�u���x
short	speed_curve_r450;		// R450�J�[�u���x
short	speed_curve_straight;		// S���J�[�u�������x

short	speed_crossline;			// �N���X���C���i�����x
short	speed_ckank_trace;			// �N�����N�i�����x
short	speed_rightclank_curve;		// �E�N�����N���񑬓x
short	speed_rightclank_escape;	// �E�N�����N���A���x
short	speed_leftclank_curve;		// ���N�����N���񑬓x
short	speed_leftclank_escape;		// ���N�����N���A���x

short	speed_halfine;				// �n�[�t���C���i�����x
short	speed_rightchange_trace;	// �E���[���`�F���W�i�����x
short	speed_rightchange_curve;	// �E���[���`�F���W���񑬓x
short	speed_rightchange_escape;	// �E���[���`�F���W���A���x

short	speed_leftchange_trace;		// �����[���`�F���W�i�����x
short	speed_leftchange_curve;		// �����[���`�F���W���񑬓x
short	speed_leftchange_escape;	// �����[���`�F���W���񑬓x

short	speed_slope_brake;			// �����I�_���x
short	speed_slope_trace;			// ��ǂݔ�΂����x

// �T�[�{�p�x
short	angle_rightclank;		// �E�N�����N����p�x
short	angle_leftclank;		// ���N�����N����p�x
short	angle_rightchange;		// �E���[���`�F���W����p�x
short	angle_leftchange;		// �E���[���`�F���W����p�x

// �^�C�}�֘A
short		cntGyro;			// �p�x�v�Z�p�J�E���^

// �p�x�֘A
double 		TurningAngleEnc;	// �G���R�[�_���狁�߂�����p�x
double		PichAngleAD;		// �A�i���O�W���C�����狁�߂��s�b�`�p�x
double		gyVoltageBefore;

// �T�[�{�֘A
// �����g���[�X
signed char	ServoPwm;		// �����g���[�X�T�[�{PWM
short 		SensorBefore;	// 1ms�O�̃Z���T�l
char		DevBefore;		// I�������Z�b�g�p
double		Int;			// I�����ώZ�l(�����g���[�X)
// �p�x����
signed char	ServoPwm2;		// �p�x�T�[�{PWM
short 		SetAngle;		// �ڕW�p�x
short		SetAngleBefore;	// 1ms�O�̖ڕW�p�x
short 		AngleBefore2;	// 1ms�O�̊p�x
char		AngleBefore3;	// I�������Z�b�g�p
double		Int2;			// I�����ώZ�l(�p�x����)

// ���[�^�[�֘A
signed char 	motorPwm;			// ���[�^�[����PWM
short			targetSpeed;		// �ڕW���x
bool			stableSpeed = false;
unsigned int	encStable = 0;
short			cntStable = 0;
char 			AccelefBefore;		// I�������Z�b�g�p
short			EncoderBefore;		// 1ms�O�̑��x
int 			targetSpeedBefore;	// 1ms�O�̖ڕW���x	
double 			Int3;				// I�����ώZ�l(���x����)

// �f���֘A
char 	demo;

// �Q�C���֘A
char	kp_buff, ki_buff, kd_buff;
char	kp2_buff, ki2_buff, kd2_buff;
char 	kp3_buff, ki3_buff, kd3_buff;

///////////////////////////////////////////////////////////////////////////
// ���W���[���� checkCrossLine
// �����T�v     �N���X���C�����m
// ����         �Ȃ�
// �߂�l       0:�N���X���C���Ȃ� 1:����
///////////////////////////////////////////////////////////////////////////
bool checkCrossLine( void )
{
	if ( sensor_inp() == 0x7 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� checkRightLine
// �����T�v     �E�n�[�t���C�����o����
// ����         �Ȃ�
// �߂�l       0:�E�n�[�t���C���Ȃ� 1:����
///////////////////////////////////////////////////////////////////////////
bool checkRightLine( void )
{
	if ( sensor_inp() == 0x3 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� checkLeftLine
// �����T�v     ���n�[�t���C�����o����
// ����         �Ȃ�
// �߂�l       0:���n�[�t���C���Ȃ� 1:����
///////////////////////////////////////////////////////////////////////////
bool checkLeftLine( void )
{
	if ( sensor_inp() == 0x6 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� checkSlope
// �����T�v     �W���C���Z���T�̒l����⓹���o
// ����         �Ȃ�
// �߂�l       0:�⓹�Ȃ� 1:����@-1:�����
///////////////////////////////////////////////////////////////////////////
signed char checkSlope( void )
{
	signed char ret = 0;

	if ( PichAngleIMU >= SLOPE_UPPERLINE_IMU ) ret = 1;
	if ( PichAngleIMU <= SLOPE_LOWERLINE_IMU ) ret = -1;
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� encMM
// �����T�v     mm���G���R�[�_�̃p���X���ɕϊ����ĕԂ�
// ����         mm:�ϊ����钷��[mm]
// �߂�l       �ϊ������p���X��
///////////////////////////////////////////////////////////////////////////
unsigned int encMM( short mm )
{
	return PALSE_MILLIMETER * mm;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� stableSpeedDistance
// �����T�v     �ڕW���x�ɒB���Ă���̎��ԂƋ������v������
// ����         �Ȃ�
// �߂�l       �Ȃ�
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
// ���W���[���� servoControlTrace
// �����T�v     ���C���g���[�X���T�[�{��PWM�̌v�Z
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
void servoControlTrace( void )
{
	int iP, iD, iI, iRet;
	short Dev, Dif;
	
	//�T�[�{���[�^�pPWM�l�v�Z
	Dev = getAnalogSensor();
	// I�����ώZ
	Int += (double)Dev * 0.001;
	if ( Int > 10000 ) Int = 10000;		// I�������~�b�g
	else if ( Int < -10000 ) Int = -10000;
	Dif = ( Dev - SensorBefore ) * 1;	// d�Q�C��1/1000�{

	iP = (int)kp_buff * Dev;		// ���
	iI = (double)ki_buff * Int;		// �ϕ�
	iD = (int)kd_buff * Dif;		// ����
	iRet = iP + iI + iD;
	iRet = iRet >> 10;				// PWM��0�`100�ߖT�Ɏ��߂�

	// PWM�̏���̐ݒ�
	if ( iRet >  100 ) iRet =  100;
	if ( iRet <  -100 ) iRet = -100;
	
	if ( Dev >= 0 )	DevBefore = 0;
	else			DevBefore = 1;
	ServoPwm = iRet;
	SensorBefore = Dev;				// ����͂��̒l��1ms�O�̒l�ƂȂ�
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� servoControlAngle
// �����T�v     �p�x���䎞�T�[�{��PWM�̌v�Z
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
void servoControlAngle( void )
{
	short i, j, Dev, Dif;
	int iP, iD, iI, iRet;
	
	// �ڕW�l�A���ݒl�擾
	i = SetAngle;
	j = getServoAngle();
	
	//�T�[�{���[�^�pPWM�l�v�Z
	Dev = i - j;
		
	// �ڕW�l�𒴂�����I�������Z�b�g
	if ( Dev >= 0 && AngleBefore3 == 1 ) Int2 = 0;
	else if ( Dev < 0 && AngleBefore3 == 0 ) Int2 = 0;
	
	// �ڕW�l��ύX������I�������Z�b�g
	if ( !(i == SetAngleBefore) ) Int2 = 0;
	
	Int2 += (double)Dev * 0.001;
	Dif = ( Dev - AngleBefore2 ) * 1;		// d�Q�C��1/1000�{

	iP = (int)kp2_buff * Dev;		// ���
	iI = (double)ki2_buff * Int2;	// �ϕ�
	iD = (int)kd2_buff * Dif;		// ����
	iRet = iP + iI + iD;
	iRet = iRet >> 4;		// PWM��0�`100�ߖT�Ɏ��߂�

	// PWM�̏���̐ݒ�
	if ( iRet >  100 ) iRet =  100;
	if ( iRet <  -100 ) iRet = -100;

	if ( Dev >= 0 ) 	AngleBefore3 = 0;
	else 				AngleBefore3 = 1;
	SetAngleBefore = i;
	ServoPwm2 = iRet;
	AngleBefore2 = Dev;			// ����͂��̒l��1ms�O�̒l�ƂȂ�
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� diff
// �����T�v   	R1,R2,R3�y��R4�̌v�Z
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
void diff ( signed char pwm )
{
	const char rev_difference_D[] = {       // �p�x������ցA�O�։�]���v�Z
		100,100,100,99,100,99,99,100,99,98,100,98,97,100,97,
		96,100,97,95,99,96,95,99,95,94,99,95,93,99,94,
		92,98,93,91,98,93,90,98,92,89,97,91,88,97,91,
		87,96,90,86,96,90,85,95,89,83,95,88,82,94,88,
		81,93,87,80,92,86,79,92,86,77,91,85,76,90,85,
		75,89,84,73,88,83,72,87,83,71,86,82,69,85,82,
		68,84,81,66,82,80,65,81,80,63,80,79,62,78,79,
		60,77,78,58,75,78,57,73,77,55,72,76,53,70,76,
		51,68,75,49,65,75,47,63,74,44,61,73,
		};

	signed char R1, R2, R3, R4;
	short angle2, r1, r2, r3;
	double angle;
	
	angle = (double)getServoAngle()*AD2DEG;		// AD�l���p�x�ɕϊ�
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
// ���W���[���� motorControl
// �����T�v     ���[�^�[��PWM���v�Z
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
void motorControl( void )
{
	int i, j, iRet, Dif, iP, iI, iD, Dev;
	char kp3, ki3, kd3;
	
	i = targetSpeed;		// �ڕW�l
	j = Encoder * 10;		// ���ݒl targetSpeed�̓G���R�[�_�̃p���X��*10�̂���
							// ���݈ʒu��10�{����

	// �f�����[�h�̂Ƃ��Q�C���ύX
	if ( demo ) {
		kp3 = kp3_buff;
		ki3 = ki3_buff;
		kd3 = kd3_buff;
	} else {
		kp3 = kp3_buff;
		ki3 = ki3_buff;
		kd3 = kd3_buff;
	}
	
	// �쓮���[�^�[�pPWM�l�v�Z
	Dev = i - j;	// �΍�
	// �ڕW�l��ύX������I�������Z�b�g
	if ( i != targetSpeedBefore ) Int3 = 0;
	
	Int3 += (double)Dev * 0.001;	// ���Ԑϕ�
	Dif = Dev - EncoderBefore;		// �����@d�Q�C��1/1000�{
	
	iP = (int)kp3 * Dev;			// ���
	iI = (double)ki3 * Int3;		// �ϕ�
	iD = (int)kd3 * Dif;			// ����
	iRet = iP + iI + iD;
	iRet = iRet >> 4;
	
	// PWM�̏���̐ݒ�
	if ( iRet >  100 ) iRet = 100;
	if ( iRet <  -100 ) iRet = -100;
	
	motorPwm = iRet;
	EncoderBefore = Dev;
	targetSpeedBefore = i;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getLinePositionNow
// �����T�v     �����Z���T�̈ʒu���Z�o����
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
double getLinePositionNow( short angleAD, double angleIMU ) {
	const double turningRadius[] = {       // �p�x������ցA�O�։�]���v�Z
		3637467,8504,4221,2794,2081,1653,1367,1163,1010,890,
		794,716,650,594,546,505,468,436,407,381,
		358,336,317,299,282,267,253,239,227,215,
		204,194,184,174,165,157,148,140,133,125,
		118,110,103,96,
		};
	double thetaM, thetaS, theta1, r1;
	short angle;
	thetaS = (double)abs(angleAD) * AD2DEG;	// [��]�ɕϊ�
	thetaM = fabs(angleIMU) * M_PI / 180;	// [rad]�ɕϊ�
	r1 = turningRadius[(short)thetaS];
	theta1 = 90.0 - thetaS - fabs(angleIMU);	// �@�̊p�x
	theta1 = thetaS * M_PI / 180;			// [rad]�ɕϊ�

	return ( (r1+TREAD/2) * sin(thetaM) ) + ( WHELLBASE * cos(thetaM) ) + ( SENSORBAR * sin(theta1) );
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getLinePositionAfter
// �����T�v     �����̒����Z���T�̈ʒu���Z�o����
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
double getLinePositionAfter ( short angle, double angleIMU ) {
	const double turningRadius[] = {       // �p�x������ցA�O�։�]���v�Z
		3637467,8504,4221,2794,2081,1653,1367,1163,1010,890,
		794,716,650,594,546,505,468,436,407,381,
		358,336,317,299,282,267,253,239,227,215,
		204,194,184,174,165,157,148,140,133,125,
		118,110,103,96,
		};
	double thetaS, thetaM, theta1, theta2, theta3, y1, x1, r, r1;
	
	r1 = turningRadius[angle];
	thetaS = (double)angle * M_PI / 180;	// [rad]�ɕϊ�
	thetaM = fabs(angleIMU) * M_PI / 180;			// [rad]�ɕϊ�

	// �e�p�����[�^�Z�o
	theta1 = M_PI/2 - thetaS - thetaM;
	x1 = turningRadius[(short)angle*4] + ( (TREAD/2) - SENSORBAR*cos(thetaS) );
	y1 = ( (r1+TREAD/2) * sin(thetaM) ) + ( WHELLBASE * cos(thetaM) ) + ( SENSORBAR * sin(theta1) );
	theta2 = atan(fabs(x1/y1));
	r = y1/cos(theta2);
	if (x1 > 0) {
		theta3 = theta1 - theta2;
	} else {
		theta3 = theta1 + theta2;
	}

	return r * cos(theta3);
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getReturnAngle
// �����T�v     �����ɃZ���T�[�o�[�Ɣ��������s�ɂȂ�T�[�{�p�x�̎Z�o
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
short getReturnAngle( double angleIMU, double y1) {
	short i;

	for (i=0;i<=MAXDEG;i++) {
		if (y1 >= getLinePositionAfter( i, angleIMU )) {
			return i;
		}
	}
}