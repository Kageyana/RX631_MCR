#ifndef I2C_LCD_H_
#define I2C_LCD_H_
//====================================//
// インクルード                  						       //
//====================================//
#include "R_PG_RX631_mcr_ver3.0.h"
#include "SCI.h"
#include <stdio.h>
#include <stdarg.h>
//====================================//
// シンボル定義									//
//====================================//
// スレーブアドレス
#define LCD_SLAVEaddrESS	0x7c
// 液晶関連変数
#define LCD_MAX_X	17		// 表示文字数 横 16 or 20
#define LCD_MAX_Y	2		// 表示文字数 縦  2 or  4
#define	RSBIT0		0x80		// コマンド送信ビット
#define RSBIT1		0xc0		// データ送信ビット

#define CLOCK		96		// 動作周波数[MHz]

/******************************** 自動生成関数 *********************************/
#define	SET_SCI_C12	init_SCI12();	// I2C初期化
#define	I2C_LCD_SEND	send_SCI12_I2c(LCD_SLAVEaddrESS, word, 2);
#define	I2C_LCD_READ	send_SCI12_I2c(LCD_SLAVEaddrESS, Command, 2);
/***************************************************************************/
//====================================//
// グローバル変数の宣言							//
//====================================//

//====================================//
// プロトタイプ宣言								//
//====================================//
// LCD関連
void wait_lcd ( short waitTime );
void lcdShowProcess( void );
void lcdPosition( char x ,char y );
void inti_lcd( void );
void lcd_CMD( unsigned char cmd );
void lcd_put( unsigned char data );
int lcdPrintf( char *format, ... );
void lcdcursol (void);

#endif // I2C_LCD_H_