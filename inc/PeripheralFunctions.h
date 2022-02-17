#ifndef PERIPHERALFUNCTIONS_H_
#define PERIPHERALFUNCTIONS_H_
//====================================//
// �C���N���[�h
//====================================//
#include "R_PG_IGC-P8080_v1.h"
//====================================//
// �V���{����`
//====================================//
#define SERVO_CENTER		2077		// �T�[�{�Z���^�[��AD�l
#define SERVO_LIMIT		    1500		// �T�[�{���~�b�gAD�l�}
#define TGR_MOTOR			1582		// �W�F�l�������W�X�^�����l(�쓮���[�^)
#define TGR_SERVO			1582		// �W�F�l�������W�X�^�����l(�T�[�{)
#define PALSE_METER		    14644	    // 1m�̃p���X
#define PALSE_MILLIMETER	14.64F	    // 1mm�̃p���X
#define SPEED_CURRENT		15		    // 1m/s�̎��@1ms�̃p���X
#define GATE_VAL			190		    // �Q�[�g�Z���T�������l

#define SW_LEFT			    0x1
#define SW_TOP			    0x2
#define SW_RIGHT			0x4
#define SW_DOWN			    0x8
#define SW_PUSH			    0xf

#define LED_R				0x4
#define LED_G				0x2
#define LED_B				0x1

/*************************************** ���������֐� *************************************/
// �^�C�}���荞��
#define SET_CMT_C0		R_PG_Timer_Set_CMT_U0_C0();			// �R���y�A�}�b�`�^�C�}������(ch0)
#define START_CMT_C0	R_PG_Timer_StartCount_CMT_U0_C0();	// �J�E���g�X�^�[�g(ch0)
#define STOP_CMT_C0	R_PG_Timer_HaltCount_CMT_U0_C0();	// �J�E���g�ꎞ��~(ch0)

// �G���R�[�_
#define ENCODER_COUNT	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );	// �J�E���g�擾

//�Z���TLED
#define L_Sen_ON		R_PG_IO_PORT_Write_PE1( 1 );
#define L_Sen_OFF		R_PG_IO_PORT_Write_PE1( 0 );

// �t���J���[LED

#define LEDR_ON		R_PG_IO_PORT_Write_PB0( 0 );
#define LEDR_OFF		R_PG_IO_PORT_Write_PB0( 1 );

#define LEDG_ON		R_PG_IO_PORT_Write_PA7( 0 );
#define LEDG_OFF		R_PG_IO_PORT_Write_PA7( 1 );

#define LEDB_ON		R_PG_IO_PORT_Write_PA6( 0 );
#define LEDB_OFF		R_PG_IO_PORT_Write_PA6( 1 );

// �^�N�g�X�C�b�`
// �f�W�^���C���v�b�g
#define TACTSWITCH2	R_PG_IO_PORT_Read_P13(&tasw_d[0]);		// ��
#define TACTSWITCH1	R_PG_IO_PORT_Read_P54(&tasw_d[1]);		// ��
#define TACTSWITCH3	R_PG_IO_PORT_Read_P53(&tasw_d[2]);		// �E
#define TACTSWITCH4	R_PG_IO_PORT_Read_P55(&tasw_d[3]);		// ��
#define TACTSWITCH5	R_PG_IO_PORT_Read_P12(&tasw_d[4]);		// ��������
// ���[�^���[�R�[�h�X�C�b�`
#define DIPSWITCH1		R_PG_IO_PORT_Read_P27(&dpsw_d[0]);
#define DIPSWITCH2		R_PG_IO_PORT_Read_P17(&dpsw_d[1]);
#define DIPSWITCH3		R_PG_IO_PORT_Read_P22(&dpsw_d[2]);
#define DIPSWITCH4		R_PG_IO_PORT_Read_P23(&dpsw_d[3]);

// AD�R���o�[�^
#define SET_ADC		R_PG_ADC_12_Set_S12AD0(); 				// 12�r�b�gA/D�R���o�[�^(S12AD0)��ݒ�
#define START_ADC		R_PG_ADC_12_StartConversionSW_S12AD0();	// A/D�ϊ��J�n
#define GET_ADC		R_PG_ADC_12_GetResult_S12AD0( result );	// AD�l���擾

// �f�B���C
#define DELAY			R_PG_IO_PORT_Write_P27(0);		// ���ڑ��̒[�q�����蓖�Ă�

// ���[�^
// MTU������(PWM�@�\�A�G���R�[�_
#define SET_MTU_C0	R_PG_Timer_Set_MTU_U0_C0();		// ch0
#define SET_MTU_C1	R_PG_Timer_Set_MTU_U0_C1();		// ch1
#define SET_MTU_C2	R_PG_Timer_Set_MTU_U0_C2();		// ch2
#define SET_MTU_C3	R_PG_Timer_Set_MTU_U0_C3();		// ch3
#define SET_MTU_C4	R_PG_Timer_Set_MTU_U0_C4();		// ch4
// MTU0,1,2,3,4�̃J�E���g�J�n
#define START_MTU	R_PG_Timer_SynchronouslyStartCount_MTU_U0( 1, 1, 1, 1, 1);

// ���O��
#define DIR_FL_FOR		R_PG_IO_PORT_Write_PE3( 1 );		// ���[�^��]����(���])
#define DIR_FL_REV		R_PG_IO_PORT_Write_PE3( 0 );		// ���[�^��]����(�t�]�j
#define PWM_FL_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C4( abs(pwmfl) );	// PWM�o��
// �E�O��
#define DIR_FR_FOR		R_PG_IO_PORT_Write_PE7( 0 );
#define DIR_FR_REV		R_PG_IO_PORT_Write_PE7( 1 );
#define PWM_FR_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C0( abs(pwmfr) );
// �����
#define DIR_RL_FOR		R_PG_IO_PORT_Write_PC4( 1 );
#define DIR_RL_REV		R_PG_IO_PORT_Write_PC4( 0 );
#define PWM_RL_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C3( abs(pwmrl) );
// �E���
#define DIR_RR_FOR		R_PG_IO_PORT_Write_PB2( 0 );
#define DIR_RR_REV		R_PG_IO_PORT_Write_PB2( 1 );
#define PWM_RR_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C2( abs(pwmrr) );
// �T�[�{1
#define DIR_SERVO_FOR	R_PG_IO_PORT_Write_PE5( 1 );
#define DIR_SERVO_REV	R_PG_IO_PORT_Write_PE5( 0 );
#define PWM_SERVO_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C0( abs(pwm) );
// �T�[�{2
#define DIR_LANCER_FOR	R_PG_IO_PORT_Write_PC2( 0 );
#define DIR_LANCER_REV	R_PG_IO_PORT_Write_PC2( 1 );
#define PWM_LANCER_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C3( abs(pwml) );

/******************************************************************************************/

//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
// �^�C�}�֘A
extern volatile unsigned short		cnt0;		// �֐��p�^�C�}

// �Z���T�֘A
extern short		sensorR;		// �E�A�i���O�Z���T
extern short		sensorL;		// ���A�i���O�Z���T
extern short		sensorG;		// �Q�[�g�Z���T
extern short		sensorG_th;	// �Q�[�g�Z���T
extern short		sensorC;		// ���S�A�i���O�Z���T
extern short		sensorLL;		// �ō��[�A�i���O�Z���T
extern short		sensorRR;		// �ŉE�[�A�i���O�Z���T

extern short		L_sencnt;

extern short		VoltageC;	//�d���`�F�b�J�[
extern double		Voltage;

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
// �v���g�^�C�v�錾
//====================================//
// IO�|�[�g
void init_IO(void);

// �^�C�}�֘A
void delay( unsigned short delaytime );

// LED�֘A
void led_out ( char rgb );

// �G���R�[�_�֘A
void getEncoder (void);

// �X�C�b�`�֘A
void getSwitch(void);
unsigned char tasw_get ( void );
unsigned char dipsw_get( void );

// �Z���T�֘A
short getServoAngle(void);
short getAnalogSensor( void );
unsigned char sensor_inp( void );
unsigned char startbar_get( void );

//�u�U�[�֘A
void init_BeepS( void );
void setBeepPatternS( unsigned short Beep );
void beepProcessS( void );

// ���[�^�[�֘A
void motorPwmOut( signed char accelefL, signed char accelefR, signed char accelerL, signed char accelerR );
// �T�[�{�֘A
void servoPwmOut( signed char pwm );

// �d���l�֘A
void get_volatage( void );

// ��r�֘A
int short_sort( const void* a, const void* b );

#endif // PERIPHERALFUNCTIONS_H_