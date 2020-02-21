#ifndef MOTOR_H_
#define MOTOR_H_
//====================================//
// �C���N���[�h									//
//====================================//
#include "R_PG_IGC-P8080_v1.h"
#include "Control.h"
//====================================//
// �V���{����`									//
//====================================//
#define TGR_MOTOR			1582		// �W�F�l�������W�X�^�����l(�쓮���[�^)
#define TGR_SERVO			1582		// �W�F�l�������W�X�^�����l(�T�[�{)
#define SERVO_LIMIT		420		// �T�[�{���~�b�gAD�l�}
/*************************************** ���������֐� *************************/
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
#define PWM_FL_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C4( pwmfl );	// PWM�o��
// �E�O��
#define DIR_FR_FOR		R_PG_IO_PORT_Write_PE7( 0 );
#define DIR_FR_REV		R_PG_IO_PORT_Write_PE7( 1 );
#define PWM_FR_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C0( pwmfr );
// �����
#define DIR_RL_FOR		R_PG_IO_PORT_Write_PC4( 1 );
#define DIR_RL_REV		R_PG_IO_PORT_Write_PC4( 0 );
#define PWM_RL_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C3( pwmrl );
// �E���
#define DIR_RR_FOR		R_PG_IO_PORT_Write_PB2( 0 );
#define DIR_RR_REV		R_PG_IO_PORT_Write_PB2( 1 );
#define PWM_RR_OUT	R_PG_Timer_SetTGR_B_MTU_U0_C2( pwmrr );
// �T�[�{1
#define DIR_SERVO_FOR	R_PG_IO_PORT_Write_PE5( 1 );
#define DIR_SERVO_REV	R_PG_IO_PORT_Write_PE5( 0 );
#define PWM_SERVO_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C0( pwm );
// �T�[�{2
#define DIR_LANCER_FOR	R_PG_IO_PORT_Write_PC2( 0 );
#define DIR_LANCER_REV	R_PG_IO_PORT_Write_PC2( 1 );
#define PWM_LANCER_OUT	R_PG_Timer_SetTGR_D_MTU_U0_C3( pwml );
/**************************************************************************/
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// ���[�^�[�֘A
extern signed char	accele_fR;		// �E�O���[�^�[PWM�l
extern signed char	accele_fL;		// ���O���[�^�[PWM�l
extern signed char	accele_rR;		// �E�ヂ�[�^�[PWM�l
extern signed char	accele_rL;		// ���ヂ�[�^�[PWM�l
//====================================//
// �v���g�^�C�v�錾									//
//====================================//
// ���[�^�[�֘A
void motor_f( signed char accelefL, signed char accelefR );
void motor_r( signed char accelerL, signed char accelerR );

// �T�[�{�֘A
void servoPwmOut( signed char pwm );
#endif // MOTOR_H_