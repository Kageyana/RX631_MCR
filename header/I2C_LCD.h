#ifndef I2C_LCD_H_
#define I2C_LCD_H_
//======================================//
// �V���{����`                         //
//======================================//
// �X���[�u�A�h���X
#define LCD_SLAVEADDRESS	0x007c
// �t���֘A�ϐ�
#define LCD_MAX_X	17		// �\�������� �� 16 or 20
#define LCD_MAX_Y	2		// �\�������� �c  2 or  4
#define	RSBIT0		0x80		// �R�}���h���M�r�b�g
#define RSBIT1		0xc0		// �f�[�^���M�r�b�g

/******************************************** ���������֐� ******************************************/
#define SET_SCI_C12	R_PG_SCI_Set_C12();	// I2C������
#define I2C_LCD_SEND	R_PG_SCI_I2CMode_Send_C12(0, LCD_SLAVEADDRESS, word, 2);
#define	I2C_LCD_READ	R_PG_SCI_I2CMode_Send_C12(0, LCD_SLAVEADDRESS, Command, 2);

/****************************************************************************************************/
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