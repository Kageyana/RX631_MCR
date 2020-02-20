//====================================//
// �C���N���[�h									//
//====================================//
#include "Rotaryencoder.h"
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// �G���R�[�_�֘A
static unsigned short 	cnt_Encoder;	// �G���R�[�_�l�̊i�[��
static unsigned short	encbuff;		// �O��̃G���R�[�_�l
short				Encoder;		// 1ms���Ƃ̃p���X
unsigned int		EncoderTotal;	// �����s����
unsigned int		enc1;		// ���s�p�����J�E���g
unsigned int		enc_slope;		// ��㋗���J�E���g
/////////////////////////////////////////////////////////////////////////////////
// ���W���[���� getEncoder									//
// �����T�v     �G���R�[�_�̃J�E���g���擾���ώZ����(1ms���ƂɎ��s)			//
// ����         �Ȃ�											//
// �߂�l       �Ȃ�											//
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