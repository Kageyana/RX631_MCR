#ifndef MTU_H_
#define MTU_H_
//====================================//
// �C���N���[�h
//====================================//
#include "R_PG_IGC-P8080_v1.h"
//====================================//
// �V���{����`
//====================================//
#define SERVO_CENTER		2045		// �T�[�{�Z���^�[��AD�l
#define SERVO_LIMIT		    1500		// �T�[�{���~�b�gAD�l�}
#define TGR_MOTOR			1582		// �W�F�l�������W�X�^�����l(�쓮���[�^) TGRA�����l-1
#define TGR_SERVO			1582		// �W�F�l�������W�X�^�����l(�T�[�{)	 TGRA�����l-1
#define PALSE_METER		    14644	    // 1m�̃p���X
#define PALSE_MILLIMETER	14.64F	    // 1mm�̃p���X
#define SPEED_CURRENT		15		    // 1m/s�̎��@1ms�̃p���X
#define GATE_VAL			190		    // �Q�[�g�Z���T�������l
/*************************************** ���������֐� *************************************/
//�Z���TLED
#define L_Sen_ON		R_PG_Timer_SetTGR_D_MTU_U0_C4( TGR_MOTOR+2 );
#define L_Sen_OFF		R_PG_Timer_SetTGR_D_MTU_U0_C4( 0 );
// �G���R�[�_
#define ENCODER_COUNT	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );	// �J�E���g�擾
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
#define PWM_FL_OUT	    R_PG_Timer_SetTGR_B_MTU_U0_C4( abs(pwmfl) );	// PWM�o��
// �E�O��
#define DIR_FR_FOR		R_PG_IO_PORT_Write_PE7( 0 );
#define DIR_FR_REV		R_PG_IO_PORT_Write_PE7( 1 );
#define PWM_FR_OUT	    R_PG_Timer_SetTGR_B_MTU_U0_C0( abs(pwmfr) );
// �����
#define DIR_RL_FOR		R_PG_IO_PORT_Write_PC4( 1 );
#define DIR_RL_REV		R_PG_IO_PORT_Write_PC4( 0 );
#define PWM_RL_OUT	    R_PG_Timer_SetTGR_B_MTU_U0_C3( abs(pwmrl) );
// �E���
#define DIR_RR_FOR		R_PG_IO_PORT_Write_PB2( 0 );
#define DIR_RR_REV		R_PG_IO_PORT_Write_PB2( 1 );
#define PWM_RR_OUT	    R_PG_Timer_SetTGR_B_MTU_U0_C2( abs(pwmrr) );
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
// �G���R�[�_�֘A
void getEncoder (void);

// ���[�^�[�֘A
void motorPwmOut( signed char accelefL, signed char accelefR, signed char accelerL, signed char accelerR );
// �T�[�{�֘A
void servoPwmOut( signed char pwm );

#endif // MTU_H_