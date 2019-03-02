/************************************************************************
*
* Device     : RX/RX600/RX63N,RX631
*
* File Name  : hwsetup.c
*
* Abstract   : Hardware Setup file.
*
* History    : 0.10  (2011-02-21)  [Hardware Manual Revision : 0.01]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2011 Renesas Electronics Corporation.
* and Renesas Solutions Corp.
*
************************************************************************/

#include "iodefine.h"
#include "PeripheralFunctions.h"
#include "MicroSD.h"
#include "I2C_LCD.h"
#include "SCI.h"

extern void HardwareSetup(void);

// ID�R�[�h�ݒ�(45ffffffffffffffffffffffffffffff)
#pragma address id_code=0xffffffa0 // ID codes (Default)
const unsigned long id_code[4] = {
        0x45ffffff,
        0xffffffff,
        0xffffffff,
        0xffffffff,
};

void HardwareSetup(void)
{
	R_PG_IO_PORT_SetPortNotAvailable();	// ���݂��Ȃ��|�[�g��ݒ�
	R_PG_Clock_WaitSet(0.01); 		// �N���b�N��ݒ肵0.01�b��ɃN���b�N�\�[�X�؂�ւ�
	
	SET_MTU_C0		// �}���`�t�@���N�V�����^�C�}��ݒ�
	SET_MTU_C1
	SET_MTU_C2
	SET_MTU_C3
	
	//SET_SCI_C1
	SET_SCI_C5 		// �V���A��I/O�`���l����ݒ�(SPI)
	SET_SCI_C12 		// �V���A��I/O�`���l����ݒ�(I2C)
	
	SET_CMT_C0		// �R���y�A�}�b�`�^�C�}��ݒ�(ch0)
	SET_CMT_C2		// �R���y�A�}�b�`�^�C�}��ݒ�(ch2)
	
	init_IO();			// IO�|�[�g�̏�����
	
	SET_ADC			// 12�r�b�gA/D�R���o�[�^(S12AD0)��ݒ�
	
	START_MTU		// MTU0,1,2,3�̃J�E���g�J�n
	
	START_ADC		// A/D�ϊ��J�n
	START_CMT_C0 	// �J�E���g�X�^�[�g(ch0)
	START_CMT_C2 	// �J�E���g�X�^�[�g(ch2)
}
