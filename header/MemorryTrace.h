#ifndef MEMMORYTRACE_H_
#define MEMMORYTRACE_H_
//====================================//
// �C���N���[�h									//
//==================================~=//
#include "MicroSD.h"
//====================================//
// �V���{����`									//
//====================================//
// ���O��͊֘A
#define STRAIGHT			1		// ��������
//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//
// ���O��͊֘A
extern char			comp_char[10][100];
extern short			comp_short[10][100];
extern unsigned int		comp_uint[10][100];
//====================================//
// �v���g�^�C�v�錾								//
//====================================//
bool serchPattern ( char process, char spattern );
#endif /* MEMMORYTRACE_H */