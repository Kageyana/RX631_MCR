#ifndef MEMMORYTRACE_H_
#define MEMMORYTRACE_H_
//====================================//
// �C���N���[�h
//==================================~=//
#include "microSD.h"
//====================================//
// �V���{����`
//====================================//
// ���O��͊֘A
#define STRAIGHT		1		// ��������
#define CROSSLINE		2		// �N���X���C������
#define RC			    3		// �E�N�����N
#define LC				4		// ���N�����N
#define RLC			    5		// �E���[���`�F���W
#define LLC			    6		// �����[���`�F���W
#define SLUP			7		// ���n��
#define SLDOWN		    8		// ����I���
#define SLMIDDLE		9		// ���I��褉���n��
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
// ���O��͊֘A
extern char		comp_char[10][100];
extern short		comp_short[10][100];
extern unsigned int	comp_uint[10][100];

extern short		cntmpattern2;
extern char		memory_mode;
//====================================//
// �v���g�^�C�v�錾
//====================================//
bool serchPattern ( char process, char spattern );
char logmeter( void );
#endif /* MEMMORYTRACE_H */