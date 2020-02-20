#ifndef TIMER_H_
#define TIMER_H_
//====================================//
// �C���N���[�h									//
//====================================//
#include "R_PG_IGC-P8080_v1.h"
#include "ADC.h"
#include "IO.h"
#include "MOTOR.h"
#include "Rotaryencoder.h"
#include "Setup.h"
//====================================//
// �V���{����`									//
//====================================//
/*************************************** ���������֐� *************************/
// �^�C�}���荞��
#define SET_CMT_C0		R_PG_Timer_Set_CMT_U0_C0();			// �R���y�A�}�b�`�^�C�}������(ch0)
#define START_CMT_C0	R_PG_Timer_StartCount_CMT_U0_C0();	// �J�E���g�X�^�[�g(ch0)
#define STOP_CMT_C0	R_PG_Timer_HaltCount_CMT_U0_C0();	// �J�E���g�ꎞ��~(ch0)// �^�C�}���荞��
#define SET_CMT_C0		R_PG_Timer_Set_CMT_U0_C0();			// �R���y�A�}�b�`�^�C�}������(ch0)
#define START_CMT_C0	R_PG_Timer_StartCount_CMT_U0_C0();	// �J�E���g�X�^�[�g(ch0)
#define STOP_CMT_C0	R_PG_Timer_HaltCount_CMT_U0_C0();	// �J�E���g�ꎞ��~(ch0)
/**************************************************************************/
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// �^�C�}�֘A
extern volatile unsigned short		cnt0;		// �֐��p�^�C�}
extern unsigned int 				cnt1;		// ���s�p�^�C�}�J�E���g
extern unsigned short	 			cnt_out;	// �R�[�X�A�E�g����p�^�C�}
extern unsigned short	 			cnt_out2;	// �R�[�X�A�E�g����p�^�C�}2
extern unsigned short	 			cnt_out3;	// �R�[�X�A�E�g����p�^�C�}3
extern unsigned short	 			cnt_out4;	// �R�[�X�A�E�g����p�^�C�}4
//====================================//
// �v���g�^�C�v�錾									//
//====================================//

#endif // TIMER_H_