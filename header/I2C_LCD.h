#ifndef I2C_LCD_H_
#define I2C_LCD_H_
//======================================//
// シンボル定義                         //
//======================================//
// 液晶関連変数
#define LCD_MAX_X	17		// 表示文字数 横 16 or 20
#define LCD_MAX_Y	2		// 表示文字数 縦  2 or  4
#define	RSBIT0		0x80		// コマンド送信ビット
#define RSBIT1		0xc0		// データ送信ビット

//======================================//
// グローバル変数の宣言                 //
//======================================//

//======================================//
// プロトタイプ宣言                     //
//======================================//

// LCD関連
void lcdShowProcess( void );
void lcdPosition( char x ,char y );
void inti_lcd( void );
void lcd_CMD( unsigned char cmd );
void lcd_put( unsigned char data );
int lcdPrintf( char *format, ... );
void lcdcursol (void);

#endif // I2C_LCD_H_