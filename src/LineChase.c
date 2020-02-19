//====================================//
// �C���N���[�h									//
//====================================//
#include "LineChase.h"
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// ���[�h�֘A
char 	lcd_mode = 1;		// LCD�\����		1:�\��		0:����		
char 	angle_mode;		// �T�[�{PWM�ύX		0:�����g���[�X	1:�p�x����
char	pushcart_mode;		// �艟�����[�h��	0:�������s	1:�艟��

// �p�����[�^�֘A
// ����
short	stopping_meter;			// ��~����
// ���x
short	speed_straight;			// �ʏ�g���[�X
short	speed_curve_brake;		// �J�[�u�u���[�L
short	speed_curve_r600;		// R600�J�[�u���x
short	speed_curve_r450;		// R450�J�[�u���x
short	speed_curve_straight;	// S���J�[�u�������x

short	speed_crossline;			// �N���X���C���i�����x
short	speed_ckank_trace;		// �N�����N�i�����x
short	speed_rightclank_curve;	// �E�N�����N���񑬓x
short	speed_rightclank_escape;	// �E�N�����N���A���x
short	speed_leftclank_curve;	// ���N�����N���񑬓x
short	speed_leftclank_escape;	// ���N�����N���A���x

short	speed_halfine;			// �n�[�t���C���i�����x
short	speed_rightchange_trace;	// �E���[���`�F���W�i�����x
short	speed_rightchange_curve;	// �E���[���`�F���W���񑬓x
short	speed_rightchange_escape;// �E���[���`�F���W���A���x

short	speed_leftchange_trace;	// �����[���`�F���W�i�����x
short	speed_leftchange_curve;	// �����[���`�F���W���񑬓x
short	speed_leftchange_escape;	// �����[���`�F���W���񑬓x

short	speed_slope_brake;		// �����I�_���x
short	speed_slope_trace;		// ��ǂݔ�΂����x

// �T�[�{�p�x
short	angle_rightclank;		// �E�N�����N����p�x
short	angle_leftclank;			// ���N�����N����p�x
short	angle_rightchange;		// �E���[���`�F���W����p�x
short	angle_leftchange;		// �E���[���`�F���W����p�x

// �T�[�{�֘A
// �����g���[�X
signed char	ServoPwm;	// �����g���[�X�T�[�{PWM
short 		SensorBefore;	// 1ms�O�̃Z���T�l
char			DevBefore;		// I�������Z�b�g�p
double		Int;			// I�����ώZ�l(�����g���[�X)
// �p�x����
signed char	ServoPwm2;		// �p�x�T�[�{PWM
short 		SetAngle;		// �ڕW�p�x
short			SetAngleBefore;		// 1ms�O�̖ڕW�p�x
short 		AngleBefore2;	// 1ms�O�̊p�x
char			AngleBefore3;		// I�������Z�b�g�p
double		Int2;			// I�����ώZ�l(�p�x����)

// ���[�^�[�֘A
signed char 	motorPwm;	// ���[�^�[����PWM
char 			AccelefBefore;		// I�������Z�b�g�p
short			EncoderBefore;		// 1ms�O�̑��x
int 			targetSpeedBefore;	// 1ms�O�̖ڕW���x	
double 		Int3;			// I�����ώZ�l(���x����)
short			targetSpeed;		// �ڕW���x

// �Q�C���֘A
char	kp_buff, ki_buff, kd_buff;
char	kp2_buff, ki2_buff, kd2_buff;
char kp3_buff, ki3_buff, kd3_buff;

///////////////////////////////////////////////////////////////////////////
// ���W���[���� check_crossline							//
// �����T�v     �N���X���C�����m								//
// ����         �Ȃ�										//
// �߂�l       0:�N���X���C���Ȃ� 1:����							//
///////////////////////////////////////////////////////////////////////////
signed char check_crossline( void )
{
	if ( sensor_inp() == 0x7 ) return 1;
	else return 0;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� check_rightline							//
// �����T�v     �E�n�[�t���C�����o����						//
// ����         �Ȃ�										//
// �߂�l       0:�E�n�[�t���C���Ȃ� 1:����						//
///////////////////////////////////////////////////////////////////////////
signed char check_rightline( void )
{
	if ( sensor_inp() == 0x3 ) return 1;
	else return 0;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� check_leftline								//
// �����T�v     ���n�[�t���C�����o����						//
// ����         �Ȃ�										//
// �߂�l       0:���n�[�t���C���Ȃ� 1:����						//
///////////////////////////////////////////////////////////////////////////
signed char check_leftline( void )
{
	if ( sensor_inp() == 0x6 ) return 1;
	else return 0;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� enc_mm									//
// �����T�v     mm���G���R�[�_�̃p���X���ɕϊ����ĕԂ�				//
// ����         mm:�ϊ����钷��[mm]						//
// �߂�l       �ϊ������p���X��								//
///////////////////////////////////////////////////////////////////////////
unsigned int enc_mm( short mm )
{
	return SPEED_CURRENT * mm;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� servoControl							//
// �����T�v     ���C���g���[�X���T�[�{��PWM�̌v�Z				//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void servoControl( void )
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
	iRet = iRet >> 10;

	// PWM�̏���̐ݒ�(���肵����70���x��)
	if ( iRet >  70 ) iRet =  90;		// �}�C�R���J�[�����肵����
	if ( iRet <  -70 ) iRet = -90;	// �����90���炢�ɂ��Ă�������
	
	if ( Dev >= 0 )	DevBefore = 0;
	else			DevBefore = 1;
	ServoPwm = iRet;
	SensorBefore = Dev;				// ����͂��̒l��1ms�O�̒l�ƂȂ�
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� servoControl2							//
// �����T�v     �p�x���䎞�T�[�{��PWM�̌v�Z					//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void servoControl2( void )
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
	iI = (double)ki2_buff * Int2;		// �ϕ�
	iD = (int)kd2_buff * Dif;		// ����
	iRet = iP + iI + iD;
	iRet = iRet >> 4;		// PWM��0�`100�̊ԂɎ��߂�

	// PWM�̏���̐ݒ�(���肵����70���x��)
	if ( iRet >  90 ) iRet =  90;		// �}�C�R���J�[�����肵����
	if ( iRet <  -90 ) iRet = -90;	// �����90���炢�ɂ��Ă�������
	
	if ( Dev >= 0 ) 	AngleBefore3 = 0;
	else 			AngleBefore3 = 1;
	SetAngleBefore = i;
	ServoPwm2 = iRet;
	AngleBefore2 = Dev;			// ����͂��̒l��1ms�O�̒l�ƂȂ�
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� motorControl							//
// �����T�v     ���[�^�[��PWM���v�Z						//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void motorControl( void )
{
	int i, j, iRet, Dif, iP, iI, iD, Dev;
	char kp3, ki3, kd3;
	
	i = targetSpeed;		// �ڕW�l x[m/s]*10
	j = Encoder * 10;	// ���ݒl �ڕW�l�ɍ��킹��10�{����
	
	// �쓮���[�^�[�pPWM�l�v�Z
	Dev = i - j;	// �΍�
	// �ڕW�l��ύX������I�������Z�b�g
	if ( i != targetSpeedBefore ) Int3 = 0;
	
	Int3 += (double)Dev * 0.001;	// �ϕ�
	Dif = Dev - EncoderBefore;		// �����@d�Q�C��1/1000�{
	
	iP = (int)kp3 * Dev;			// ���
	iI = (double)ki3 * Int3;		// �ϕ�
	iD = (int)kd3 * Dif;			// ����
	iRet = iP + iI + iD;
	iRet = iRet >> 4;
	
	// PWM�̏���̐ݒ�
	if ( iRet >  100 )	iRet =  100;
	if ( iRet <  -100 )	iRet = -100;
	
	motorPwm = iRet;
	EncoderBefore = Dev;
	targetSpeedBefore = i;
}