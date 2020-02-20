#ifndef SETUP_H_
#define SETUP_H_
//======================================//
// �C���N���[�h                         //
//======================================//
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "E2dataFlash.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "SCI.h"
#include "SPI_ICM20648.h"
#include "MemorryTrace.h"
#include <stdio.h>
//======================================//
// �V���{����`                         //
//======================================//
#define UD	0
#define LR		1

#define START_COUNT	1
#define START_GATE		2

//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// �p�^�[���֘A
extern char		start;

// �^�C�}�֘A
extern unsigned short 	cnt_setup;
extern unsigned short 	cnt_setup2;
extern unsigned short 	cnt_setup3;	
extern short		cnt_swR;	// �X�C�b�`����������p�E
extern short		cnt_swL;	// �X�C�b�`����������p��

// �p�����[�^�֘A
extern char fixSpeed;

//======================================//
// �v���g�^�C�v�錾                     //
//======================================//
void setup(void);
char fix_speedsetting ( void );

#endif /* SCI_H_ */