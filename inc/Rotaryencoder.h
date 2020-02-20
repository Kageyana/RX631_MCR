#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_
//====================================//
// �C���N���[�h									//
//====================================//
#include "R_PG_IGC-P8080_v1.h"
//====================================//
// �V���{����`									//
//====================================//
#define SPEED_CURRENT		25		// 1m/s�̎��@1ms�̃p���X	1000*1��]������̃p�X����/�v�[���O�a/pi
/*************************************** ���������֐� *************************/
// AD�R���o�[�^
#define ENCODER_COUNT	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );	// �J�E���g�擾
/**************************************************************************/
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// �G���R�[�_�֘A
extern unsigned int	EncoderTotal;	// �����s����
extern signed short	Encoder;		// 1ms���Ƃ̃p���X
extern unsigned int	enc1;		// ���s�p�����J�E���g
extern unsigned int	enc_slope;		// ��㋗���J�E���g
//====================================//
// �v���g�^�C�v�錾									//
//====================================//
// �G���R�[�_�֘A
void getEncoder (void);
// �G���R�[�_�֘A
unsigned int enc_mm( short mm );
#endif // ROTARYENCODER_H_