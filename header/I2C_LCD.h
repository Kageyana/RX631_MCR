#ifndef I2C_LCD_H_
#define I2C_LCD_H_
//====================================//
// �C���N���[�h                  						       //
//====================================//
#include "R_PG_RX631_mcr_ver3.0.h"
#include "SCI.h"
#include <stdio.h>
#include <stdarg.h>
//====================================//
// �V���{����`									//
//====================================//
// �X���[�u�A�h���X
#define LCD_SLAVEaddrESS	0x7c
// �t���֘A�ϐ�
#define LCD_MAX_X	17		// �\�������� �� 16 or 20
#define LCD_MAX_Y	2		// �\�������� �c  2 or  4
#define	RSBIT0		0x80		// �R�}���h���M�r�b�g
#define RSBIT1		0xc0		// �f�[�^���M�r�b�g

#define CLOCK		96		// ������g��[MHz]

/******************************** ���������֐� *********************************/
#define	SET_SCI_C12	init_SCI12();	// I2C������
#define	I2C_LCD_SEND	send_SCI12_I2c(LCD_SLAVEaddrESS, word, 2);
#define	I2C_LCD_READ	send_SCI12_I2c(LCD_SLAVEaddrESS, Command, 2);
/***************************************************************************/
//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//

//====================================//
// �v���g�^�C�v�錾								//
//====================================//
// LCD�֘A
void wait_lcd ( short waitTime );
void lcdShowProcess( void );
void lcdPosition( char x ,char y );
void inti_lcd( void );
void lcd_CMD( unsigned char cmd );
void lcd_put( unsigned char data );
int lcdPrintf( char *format, ... );
void lcdcursol (void);

#endif // I2C_LCD_H_