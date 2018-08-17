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
#include "I2C_MPU-9255.h"
#include <stdio.h>
//======================================//
// �V���{����`                         //
//======================================//

//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// �p�^�[���֘A
extern char		start;
extern unsigned short 	cnt_setup;
extern unsigned short 	cnt_setup2;

// �p�����[�^�֘A
extern char fixSpeed;

//======================================//
// �v���g�^�C�v�錾                     //
//======================================//
void setup(void);
char fix_speedsetting ( void );

#endif /* SCI_H_ */