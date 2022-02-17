//====================================//
// �C���N���[�h
//====================================//
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include <stdio.h>
#include <string.h>

//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
// �^�C�}�֘A
volatile unsigned short	cnt0;		// �֐��p�^�C�}
static char				ADTimer10;	// AD�ϊ��J�E���g�p

// �X�C�b�`�֘A
static unsigned char 	dpsw_d[4];	// �f�B�b�v�X�C�b�`�̊i�[��
static unsigned char	tasw_d[5];	// �X�C�b�`�l�̊i�[��

// �Z���T�֘A
static unsigned short 	result[14]; 	// 12bitA/D�ϊ����ʂ̊i�[��
static int			senR;		// �E�A�i���O�Z���T�ώZAD�l
static int			senL;		// ���A�i���O�Z���T�ώZAD�l
static int			senG;		// �Q�[�g�Z���T�ώZAD�l
static int			senC;		// ���S�A�i���O�Z���T�ώZAD�l
static int			senLL;		// �ō��[�A�i���O�Z���T�ώZAD�l
static int			senRR;		// �ŉE�[�A�i���O�Z���T�ώZAD�l
static int			VolC;		// �d���`�F�b�J�[AD�l
static int			pot;		// �|�e���V�������[�^�[�ώZAD�l
short 				Angle;		// �|�e���V�������[�^�[����AD�l
short				sensorR;	// �E�A�i���O�Z���T����AD�l
short				sensorL;	// ���A�i���O�Z���T����AD�l
short				sensorG;	// �Q�[�g�Z���T����AD�l
short				sensorG_th = GATE_VAL;	// �Q�[�g�J���������l
short				sensorC;	// ���S�A�i���O�Z���T����AD�l
short				sensorLL;	// �ō��[�A�i���O�Z���T����AD�l
short				sensorRR;	// �ŉE�[�A�i���O�Z���T����AD�l
short				VoltageC;	// �d���`�F�b�J�[AD�l���ϒl
short				Angle0;		// �T�[�{�Z���^�[�l

double		Voltage;

// �G���R�[�_�֘A
static unsigned short 	cnt_Encoder;	// �G���R�[�_�l�̊i�[��
static unsigned short	encbuff;		// �O��̃G���R�[�_�l
short				Encoder;			// 1ms���Ƃ̃p���X
unsigned int		EncoderTotal;		// �����s����
unsigned int		enc1;				// ���s�p�����J�E���g
unsigned int		enc_slope;			// ��㋗���J�E���g

// ���[�^�[�֘A
signed char		accele_fR;		// �E�O���[�^�[PWM�l
signed char		accele_fL;		// ���O���[�^�[PWM�l
signed char		accele_rR;		// �E�ヂ�[�^�[PWM�l
signed char		accele_rL;		// ���ヂ�[�^�[PWM�l
signed char		sPwm;			// �T�[�{���[�^�[PWM�l

/////////////////////////////////////////////////////////////////////
// ���W���[���� ADconverter
// �����T�v     AD�ϊ����荞��
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void ADconverter ( void )
{
	__setpsw_i();
	GET_ADC
	
	ADTimer10++;
	if ( ADTimer10 == 10 ) {
		ADTimer10 = 0;
		
		Angle = pot / 10;
		sensorR = senR / 10;
		sensorL = senL / 10;
		sensorG = senG / 10;
		sensorC = senC / 10;
		sensorLL = senLL / 10;
		sensorRR = senRR / 10;
		VoltageC = VolC /10;
		
		senR = 0;
		senL = 0;
		senG = 0;
		senC = 0;
		senLL = 0;
		senRR = 0;
		
		VolC = 0;
		pot = 0;
	}
	
	// AD�ϊ��l���o�b�t�@�Ɋi�[
	pot += result[3];
	senG += result[4];
	senLL += result[5];
	senL += result[6];
	senC += result[7];
	senRR += result[8];
	senR += result[9];
	VolC += result[11];
	
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� init_IO
// �����T�v     IO�|�[�g�̏�����
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void init_IO(void)
{
	// I/O�|�[�g��ݒ�
	R_PG_IO_PORT_Set_P1();
	R_PG_IO_PORT_Set_P2();
	R_PG_IO_PORT_Set_P3();
	R_PG_IO_PORT_Set_P5();
	R_PG_IO_PORT_Set_PA();
	R_PG_IO_PORT_Set_PB();
	R_PG_IO_PORT_Set_PC();
	R_PG_IO_PORT_Set_PE();
	
	// ���ׂĂ�IO�|�[�g��LOW�ɂ���
	R_PG_IO_PORT_Write_P1(0);
	R_PG_IO_PORT_Write_P2(0);
	R_PG_IO_PORT_Write_P3(0);
	R_PG_IO_PORT_Write_P5(0);
	R_PG_IO_PORT_Write_PA(0);
	R_PG_IO_PORT_Write_PB(0);
	R_PG_IO_PORT_Write_PC(0);
	R_PG_IO_PORT_Write_PE(0);
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� led_out
// �����T�v     LED�̓_��
// ����         rgb 	0x1:�� 0x2�� 0x4
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void led_out ( char rgb )
{
	if ( (rgb & 0x4) == 0x4 ) LEDR_ON
	else LEDR_OFF
	
	if ( (rgb & 0x2) == 0x2 ) LEDG_ON
	else LEDG_OFF
	
	if ( (rgb & 0x1) == 1 ) LEDB_ON
	else LEDB_OFF
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� get_voltage
// �����T�v     �d���̎擾
// ����         �Ȃ�
// �߂�l       ��
/////////////////////////////////////////////////////////////////////
void get_voltage ( void )
{
	Voltage = VoltageC * 5.05 * 3.94 / 4096;
}
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
/////////////////////////////////////////////////////////////////////
// ���W���[���� getSwitch
// �����T�v     �X�C�b�`�̓ǂݍ���(10ms���ƂɎ��s)
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void getSwitch(void)
{
	// �^�N�g�X�C�b�`�ǂݍ���
	TACTSWITCH1	// �^�N�g�X�C�b�`��
	TACTSWITCH2	// �^�N�g�X�C�b�`��
	TACTSWITCH3	// �^�N�g�X�C�b�`�E
	TACTSWITCH4	// �^�N�g�X�C�b�`��
	TACTSWITCH5	// �^�N�g�X�C�b�`��������
	
	// �f�B�b�v�X�C�b�`�ǂݍ���
	DIPSWITCH1
	DIPSWITCH2
	DIPSWITCH3
	DIPSWITCH4
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� dipsw_get
// �����T�v     �f�B�b�v�X�C�b�`�l��16�i���Ŏ擾
// ����         �Ȃ�
// �߂�l       �X�C�b�`�l 0�`15
/////////////////////////////////////////////////////////////////////
unsigned char dipsw_get(void) 
{
	char	dpsw[4];
	
	if ( dpsw_d[0] == 0 )	dpsw[0] = 0x0;
	else			dpsw[0] = 0x1;
	
	if ( dpsw_d[1] == 0 )	dpsw[1] = 0x0;
	else			dpsw[1] = 0x2;
	
	if ( dpsw_d[2] == 0 )	dpsw[2] = 0x0;
	else			dpsw[2] = 0x4;

	if ( dpsw_d[3] == 0 )	dpsw[3] = 0x0;
	else			dpsw[3] = 0x8;

	return ( dpsw[0] + dpsw[1] + dpsw[2] + dpsw[3] );
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� tasw_get
// �����T�v     �^�N�g�X�C�b�`�l��16�i���Ŏ擾
// ����         �Ȃ�
// �߂�l       �X�C�b�`�l 0�`7
///////////////////////////////////////////////////////////////////////////
unsigned char tasw_get(void) 
{
	char	tasw[5];
	
	if ( tasw_d[0] == 0 )	tasw[0] = 0x1;
	else			tasw[0] = 0x0;
	
	if ( tasw_d[1] == 0 )	tasw[1] = 0x2;	
	else			tasw[1] = 0x0;
	
	if ( tasw_d[2] == 0 )	tasw[2] = 0x4;	
	else			tasw[2] = 0x0;
	
	if ( tasw_d[3] == 0 )	tasw[3] = 0x8;	
	else			tasw[3] = 0x0;
	
	if ( tasw_d[4] == 0 )	tasw[4] = 0xf;
	else			tasw[4] = 0x0;

	return ( tasw[0] + tasw[1] + tasw[2] + tasw[3] + tasw[4] );
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getServoAngle
// �����T�v     �|�e���V�������[�^�[�̃A�i���O�l�Ŏ擾
// ����         �Ȃ�
// �߂�l       �Z���T�l
///////////////////////////////////////////////////////////////////////////
short getServoAngle(void) 
{	
	return  ( Angle0 - Angle );
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getAnalogSensor
// �����T�v     �A�i���O�Z���T�̃A�i���O�l�Ŏ擾
// ����         �Ȃ�
// �߂�l       �Z���T�l
///////////////////////////////////////////////////////////////////////////
short getAnalogSensor(void) 
{
	return sensorR - sensorL;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� sensor_inp
// �����T�v     �f�W�^���Z���T�̒l��16�i���Ŏ擾
// ����         �Ȃ�
// �߂�l       �Z���T�l0�`
///////////////////////////////////////////////////////////////////////////
unsigned char sensor_inp(void) 
{
	char l, c, r;
	
	if (sensorRR < 500 ) r = 0x1;
	else r = 0;
	if (sensorC < 500 ) c = 0x2;
	else c = 0;
	if (sensorLL < 500 ) l = 0x4;
	else l = 0;
	
	return l+c+r;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� startbar_get
// �����T�v     �X�^�[�g�Q�[�g�̊J�̊m�F
// ����         �Ȃ�
// �߂�l       0; ���Ă��� 1; �J���Ă�
///////////////////////////////////////////////////////////////////////////
unsigned char startbar_get(void) 
{
	char ret;
	
	if ( sensorG <= sensorG_th )	ret = 1;
	else			ret = 0;
	
	return ret;
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
	
	if ( pushcart_mode ) {
		accelefR = 0;
		accelefL = 0;
		accelerL = 0;
		accelerR = 0;
	}
	
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
///////////////////////////////////////////////////////////////////////////
// ���W���[���� short_sort
// �����T�v     short�^�ϐ��̔�r������
// ����         ��r����z��
// �߂�l       -1:a<b 0:a=b 1:a>b
///////////////////////////////////////////////////////////////////////////
int short_sort( const void* a, const void* b )
{
	// ������void*�^�ƋK�肳��Ă���̂�int�^��cast����
	if( *( short * )a < *( short * )b ) {
		return -1;
	} else
	if( *( short * )a == *( short * )b ) {
		return 0;
		}
	return 1;
}