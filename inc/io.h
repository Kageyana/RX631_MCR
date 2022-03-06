#ifndef IO_H_
#define IO_H_
//====================================//
// �C���N���[�h
//====================================//
#include "R_PG_IGC-P8080_v1.h"
//====================================//
// �V���{����`
//====================================//
#define SW_LEFT     0x1
#define SW_TOP      0x2
#define SW_RIGHT    0x4
#define SW_DOWN     0x8
#define SW_PUSH     0xf

#define LED_R       0x4
#define LED_G       0x2
#define LED_B       0x1

/*************************************** ���������֐� *************************************/
// �t���J���[LED
#define LEDR_ON		R_PG_IO_PORT_Write_PB0( 0 );
#define LEDR_OFF	R_PG_IO_PORT_Write_PB0( 1 );

#define LEDG_ON		R_PG_IO_PORT_Write_PA7( 0 );
#define LEDG_OFF	R_PG_IO_PORT_Write_PA7( 1 );

#define LEDB_ON		R_PG_IO_PORT_Write_PA6( 0 );
#define LEDB_OFF	R_PG_IO_PORT_Write_PA6( 1 );

// �^�N�g�X�C�b�`
// �f�W�^���C���v�b�g
#define TACTSWITCH2	R_PG_IO_PORT_Read_P13(&tasw_d[0]);		// ��
#define TACTSWITCH1	R_PG_IO_PORT_Read_P54(&tasw_d[1]);		// ��
#define TACTSWITCH3	R_PG_IO_PORT_Read_P53(&tasw_d[2]);		// �E
#define TACTSWITCH4	R_PG_IO_PORT_Read_P55(&tasw_d[3]);		// ��
#define TACTSWITCH5	R_PG_IO_PORT_Read_P12(&tasw_d[4]);		// ��������
// ���[�^���[�R�[�h�X�C�b�`
#define DIPSWITCH1	R_PG_IO_PORT_Read_P27(&dpsw_d[0]);
#define DIPSWITCH2	R_PG_IO_PORT_Read_P17(&dpsw_d[1]);
#define DIPSWITCH3	R_PG_IO_PORT_Read_P22(&dpsw_d[2]);
#define DIPSWITCH4	R_PG_IO_PORT_Read_P23(&dpsw_d[3]);
/******************************************************************************************/

//====================================//
// �O���[�o���ϐ��̐錾
//====================================//

//====================================//
// �v���g�^�C�v�錾
//====================================//
// IO�|�[�g
void initIO(void);

// LED�֘A
void ledOut ( char rgb );

// �X�C�b�`�֘A
void getSwitch(void);
unsigned char taswGet ( void );
unsigned char dipswGet( void );

#endif // IO_H_