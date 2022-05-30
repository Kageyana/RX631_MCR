#ifndef SETUP_H_
#define SETUP_H_
//======================================//
// �C���N���[�h
//======================================//
#include "io.h"
#include "mtu.h"
#include "ADconverter.h"
#include "control.h"
#include "E2dataFlash.h"
#include "AQM0802A.h"
#include "MicroSD.h"
#include "sci.h"
#include "ICM20648.h"
#include "MemorryTrace.h"
#include <stdio.h>
//======================================//
// �V���{����`
//======================================//
#define UD	0
#define LR	1

#define START_COUNT	    1
#define START_GATE		2

//======================================//
// �O���[�o���ϐ��̐錾
//======================================//
// �p�^�[���֘A
extern char		start;

// �^�C�}�֘A
extern unsigned short 	cntSetup1;
extern unsigned short 	cntSetup2;
extern unsigned short 	cntSetup3;	
extern short		cntSwitchUD;	// �X�C�b�`����������p�E
extern short		cntSwitchLR;	// �X�C�b�`����������p��

// �p�����[�^�֘A
extern char fixSpeed;

//======================================//
// �v���g�^�C�v�錾
//======================================//
void setup(void);
char fixSpeedSetting ( void );

#endif /* SCI_H_ */