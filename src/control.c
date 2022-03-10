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
	int iP, iD, iI, iRet, maxpwm;
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
	// �o�͓d����VOLTAGELIM�ƂȂ�Duty����v�Z
	maxpwm = (int8_t)(VOLTAGELIMTRACE / Voltage *100);

	if ( iRet > maxpwm ) iRet =  maxpwm;
	if ( iRet < -maxpwm ) iRet = -maxpwm;

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
	int iP, iD, iI, iRet, maxpwm;
	
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
	// �o�͓d����VOLTAGELIM�ƂȂ�Duty����v�Z
	maxpwm = (int8_t)(VOLTAGELIMTRACE / Voltage *100);

	if ( iRet > maxpwm ) iRet =  maxpwm;
	if ( iRet < -maxpwm ) iRet = -maxpwm;
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
	int i, j, iRet, Dif, iP, iI, iD, Dev, maxpwm;
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
	// �o�͓d����VOLTAGELIM�ƂȂ�Duty����v�Z
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
// ���W���[���� getLinePositionNow
// �����T�v     �����Z���T�̈ʒu���Z�o����
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
double getLinePositionNow( short angleAD) {
	double angle;
	angle = (double)abs(angleAD) * AD2DEG;	// [��]�ɕϊ�
	angle = 90.0 - angle;
	angle = angle * 180 / M_PI;			// [rad]�ɕϊ�
	return WHELLBASE + (SENSORBAR * sin(angle));
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getLinePositionAfter
// �����T�v     �����̒����Z���T�̈ʒu���Z�o����
// ����         �Ȃ�
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
double getLinePositionAfter (short angle) {
	const double turningRadius[] = {       // �p�x������ցA�O�։�]���v�Z
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
	
	thetaS = (double)angle * 180 / M_PI;		// [rad]�ɕϊ�
	thetaS = M_PI/2 - thetaS;

	// �e�p�����[�^�Z�o
	y1 = WHELLBASE + (SENSORBAR * sin(angle));
	x1 = turningRadius[(short)angle*4] + ( (TREAD/2) - SENSORBAR*cos(thetaS) );
	theta2 = atan(x1/y1);
	r = y1/cos(theta2);
	theta1 = M_PI/2 - thetaS;
	theta3 = theta1 - theta2;

	return r * cos(theta3);
}