#ifndef I2C_LCD_H_
#define I2C_LCD_H_
//======================================//
// �V���{����`                         //
//======================================//
// �t���֘A�ϐ�
#define LCD_MAX_X	17		// �\�������� �� 16 or 20
#define LCD_MAX_Y	2		// �\�������� �c  2 or  4
#define	RSBIT0		0x80		// �R�}���h���M�r�b�g
#define RSBIT1		0xc0		// �f�[�^���M�r�b�g

//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//

//======================================//
// �v���g�^�C�v�錾                     //
//======================================//

// LCD�֘A
void lcdShowProcess( void );
void lcdPosition( char x ,char y );
void inti_lcd( void );
void lcd_CMD( unsigned char cmd );
void lcd_put( unsigned char data );
int lcdPrintf( char *format, ... );
void lcdcursol (void);

#endif // I2C_LCD_H_