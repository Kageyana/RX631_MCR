#ifndef I2C_LCD_H_
#define I2C_LCD_H_
//====================================//
// �C���N���[�h 
//====================================//
#include "R_PG_IGC-P8080_v1.h"
#include "sci.h"
#include <stdio.h>
#include <stdarg.h>
//====================================//
// �V���{����`
//====================================//
// �X���[�u�A�h���X
#define LCD_SLAVEADDRESS	0x007c
// �t���֘A�ϐ�
#define LCD_MAX_X	17		// �\�������� �� 16 or 20
#define LCD_MAX_Y	2		// �\�������� �c  2 or  4
#define RSBIT0		0x00	// �R�}���h���M�r�b�g
#define RSBIT1		0x40	// �f�[�^���M�r�b�g
#define UPROW       0       // 1�s��
#define LOWROW      1       // 2�s��

#define CLOCK		96		// ������g��[MHz]

/******************************** ���������֐� *********************************/
#define 	SET_SCI_LCD	R_PG_SCI_Set_C9();	// I2C������
#define 	I2C_LCD_SEND	R_PG_SCI_I2CMode_Send_C9(0, LCD_SLAVEADDRESS, word, 2);
#define	    I2C_LCD_READ	R_PG_SCI_I2CMode_Send_C9(0, LCD_SLAVEADDRESS, Command, 2);
/***************************************************************************/
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//

//====================================//
// �v���g�^�C�v�錾
//====================================//
// LCD�֘A
void waitLcd ( short waitTime );
void lcdShowProcess ( void );
void lcdPosition ( char x ,char y );
void intiLcd ( void );
void lcdCMD ( unsigned char cmd );
void lcdPut ( unsigned char data );
int lcdPrintf ( char *format, ... );
void lcdcursol (void);
int lcdRowPrintf (char step, char *format, ...);

#endif // I2C_LCD_H_