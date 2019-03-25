#ifndef PERIPHERALFUNCTIONS_H_
#define PERIPHERALFUNCTIONS_H_
//====================================//
// �C���N���[�h									//
//====================================//
#include "R_PG_RX631_mcr_ver3.0.h"
//====================================//
// �V���{����`									//
//====================================//
#define SERVO_CENTER		2010		// �T�[�{�Z���^�[��AD�l
#define SERVO_LIMIT		430		// �T�[�{���~�b�gAD�l�}
#define TGR_MOTOR			4798		// �W�F�l�������W�X�^�����l(�쓮���[�^)
#define TGR_SERVO			2878		// �W�F�l�������W�X�^�����l(�T�[�{)
#define PALSE_METER		24750	// 1m�̃p���X
#define PALSE_MILLIMETER	24.75F	// 1mm�̃p���X
#define SPEED_CURRENT		25		// 1m/s�̎��@1ms�̃p���X

/*************************************** ���������֐� *************************************/
// �^�C�}���荞��
#define SET_CMT_C0		R_PG_Timer_Set_CMT_U0_C0();			// �R���y�A�}�b�`�^�C�}������(ch0)
#define START_CMT_C0	R_PG_Timer_StartCount_CMT_U0_C0();	// �J�E���g�X�^�[�g(ch0)

// �G���R�[�_
#define ENCODER_COUNT	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );	// �J�E���g�擾

// �^�N�g�X�C�b�`
// �f�W�^���C���v�b�g
#define TACTSWITCH2	R_PG_IO_PORT_Read_PC5(&tasw_d[0]);
#define TACTSWITCH1	R_PG_IO_PORT_Read_PC4(&tasw_d[1]);
#define TACTSWITCH3	R_PG_IO_PORT_Read_PC6(&tasw_d[2]);
#define TACTSWITCH4	R_PG_IO_PORT_Read_P50(&tasw_d[3]);
// �f�B�b�v�X�C�b�`
#define DIPSWITCH1		R_PG_IO_PORT_Read_PC3(&dpsw_d[0]);
#define DIPSWITCH2		R_PG_IO_PORT_Read_PC2(&dpsw_d[1]);
#define DIPSWITCH3		R_PG_IO_PORT_Read_PC1(&dpsw_d[2]);
#define DIPSWITCH4		R_PG_IO_PORT_Read_PC0(&dpsw_d[3]);

// AD�R���o�[�^
#define SET_ADC		R_PG_ADC_12_Set_S12AD0(); 				// 12�r�b�gA/D�R���o�[�^(S12AD0)��ݒ�
#define START_ADC		R_PG_ADC_12_StartConversionSW_S12AD0();	// A/D�ϊ��J�n
#define GET_ADC		R_PG_ADC_12_GetResult_S12AD0( result );	// AD�l���擾

// �f�B���C
#define DELAY			R_PG_IO_PORT_Write_P27(0);		// ���ڑ��̒[�q�����蓖�Ă�

// LED
#define LED_OUT		R_PG_IO_PORT_Write_P5( led2 );	// LED�_��

// �u�U�[
// �f�W�^���A�E�g
#define BEEP_ON		R_PG_IO_PORT_Write_P23(1);
#define BEEP_OFF		R_PG_IO_PORT_Write_P23(0);

// ���[�^
// MTU������(PWM�@�\�A�G���R�[�_)
#define SET_MTU_C0		R_PG_Timer_Set_MTU_U0_C0();	// ch0
#define SET_MTU_C1		R_PG_Timer_Set_MTU_U0_C1();	// ch1
#define SET_MTU_C2		R_PG_Timer_Set_MTU_U0_C2();	// ch2
#define SET_MTU_C3		R_PG_Timer_Set_MTU_U0_C3();	// ch3
// MTU0,1,2,3�̃J�E���g�J�n
#define START_MTU		R_PG_Timer_SynchronouslyStartCount_MTU_U0( 1, 1, 1, 1, 0);

// ���O��
#define DIR_FL_FOR		R_PG_IO_PORT_Write_PB2( 0 );		// ���[�^��]����(���])
#define DIR_FL_REV		R_PG_IO_PORT_Write_PB2( 1 );		// ���[�^��]����(���])
#define PWM_FL_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C0( pwmfl );	// PWM�o��
// �E�O��
#define DIR_FR_FOR		R_PG_IO_PORT_Write_PB4( 0 );
#define DIR_FR_REV		R_PG_IO_PORT_Write_PB4( 1 );
#define PWM_FR_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C0( pwmfr );
// �����
#define DIR_RL_FOR		R_PG_IO_PORT_Write_P20( 0 );
#define DIR_RL_REV		R_PG_IO_PORT_Write_P20( 1 );
#define PWM_RL_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C3( pwmrl );
// �E���
#define DIR_RR_FOR		R_PG_IO_PORT_Write_P15( 0 );
#define DIR_RR_REV		R_PG_IO_PORT_Write_P15( 1 );
#define PWM_RR_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C3( pwmrr );
// �T�[�{
#define DIR_SERVO_FOR	R_PG_IO_PORT_Write_PB6( 0 );
#define DIR_SERVO_REV	R_PG_IO_PORT_Write_PB6( 1 );
#define PWM_SERVO_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C2( pwm );

/******************************************************************************************/

//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//
// �^�C�}�֘A
extern volatile unsigned short		cnt0;		// �֐��p�^�C�}

// �Z���T�֘A
extern short		sensorR;		// �E�A�i���O�Z���T
extern short		sensorL;		// ���A�i���O�Z���T
extern short		sensorG;		// ���A�i���O�Z���T
extern short		sensorC;		// ���S�A�i���O�Z���T
extern short		sensorLL;		// �ō��[�A�i���O�Z���T
extern short		sensorRR;		// �ŉE�[�A�i���O�Z���T

extern short		Angle0;		// �T�[�{�Z���^�[�l

// �G���R�[�_�֘A
extern unsigned int	EncoderTotal;	// �����s����
extern signed short	Encoder;		// 1ms���Ƃ̃p���X
extern unsigned int	enc1;		// ���s�p�����J�E���g
extern unsigned int	enc_slope;		// ��㋗���J�E���g

// ���[�^�[�֘A
extern signed char	accele_fR;		// �E�O���[�^�[PWM�l
extern signed char	accele_fL;		// ���O���[�^�[PWM�l
extern signed char	accele_rR;		// �E�ヂ�[�^�[PWM�l
extern signed char	accele_rL;		// ���ヂ�[�^�[PWM�l
extern signed char	sPwm;		// �T�[�{���[�^�[PWM�l

//====================================//
// �v���g�^�C�v�錾								//
//====================================//
// IO�|�[�g
void init_IO(void);

// �^�C�}�֘A
void delay( unsigned short delaytime );

// �G���R�[�_�֘A
void getEncoder (void);

// LED�֘A
void led_out( unsigned char led );

// �X�C�b�`�֘A
void getSwitch(void);
unsigned char tasw_get ( void );
unsigned char dipsw_get( void );

// �Z���T�֘A
short getGyro( void );
short getServoAngle(void);
short getAnalogSensor( void );
unsigned char sensor_inp( void );
unsigned char startbar_get( void );

//�u�U�[�֘A
void init_BeepS( void );
void setBeepPatternS( unsigned short Beep );
void beepProcessS( void );

// ���[�^�[�֘A
void motor_f( signed char accelefL, signed char accelefR );
void motor_r( signed char accelerL, signed char accelerR );

// �T�[�{�֘A
void servoPwmOut( signed char pwm );

// ��r�֘A
int short_sort( const void* a, const void* b );

#endif // PERIPHERALFUNCTIONS_H_