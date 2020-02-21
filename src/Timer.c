//====================================//
// �C���N���[�h									//
//====================================//
#include "Timer.h"
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
volatile unsigned short	cnt0;		// �֐��p�^�C�}
unsigned int 			cnt1;		// ���s�p�^�C�}�J�E���g
unsigned short	 		cnt_out;	// �R�[�X�A�E�g����p�^�C�}
unsigned short	 		cnt_out2;	// �R�[�X�A�E�g����p�^�C�}2
unsigned short	 		cnt_out3;	// �R�[�X�A�E�g����p�^�C�}3
unsigned short	 		cnt_out4;	// �R�[�X�A�E�g����p�^�C�}4
static char				Timer10;	// 1ms�J�E���g�p
///////////////////////////////////////////////////////////////////////////
// ���W���[���� Timer									//
// �����T�v     1ms���ƂɃ^�C�}���荞��						//
// ����         �Ȃ�										//
// �߂�l       �Ȃ�										//
///////////////////////////////////////////////////////////////////////////
void Timer (void) {
	__setpsw_i();
	//�@�^�C�}�J�E���g
	if ( pattern < 11 ) {
		cnt_setup++;
		cnt_setup2++;
		cnt_setup3++;
		cnt_swR++;
		cnt_swL++;
	}
	cnt0++;
	cnt1++;
			
	// LCD�\��
	if ( lcd_mode ) lcdShowProcess();

	// �G���R�[�_�J�E���g
	getEncoder();

	// PID����l�Z�o
	if ( angle_mode ) servoControl2();	// �p�x
	else 			servoControl();		// ����
	motorControl();		// ���[�^
	
	
	Timer10++;
	// 10�����ƂɎ��s
	switch ( Timer10 ) {	
	case 1:
		getSwitch();		// �X�C�b�`�ǂݍ���
		get_voltage();		// �d���d���擾
		break;
	case 2:
		break;
	case 3:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		Timer10 = 0;
		break;
	default:
		break;
	}
}