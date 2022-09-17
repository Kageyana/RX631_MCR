﻿#ifndef I2C_LCD_H_
#define I2C_LCD_H_
//====================================//
// インクルード 
//====================================//
#include "R_PG_IGC-P8080_v1.h"
#include "sci.h"
#include <stdio.h>
#include <stdarg.h>
//====================================//
// シンボル定義
//====================================//
// スレーブアドレス
#define LCD_SLAVEADDRESS	0x007c
// 液晶関連変数
#define LCD_MAX_X	17		// 表示文字数 横 16 or 20
#define LCD_MAX_Y	2		// 表示文字数 縦  2 or  4
#define RSBIT0		0x00	// コマンド送信ビット
#define RSBIT1		0x40	// データ送信ビット
#define UPROW       0       // 1行目
#define LOWROW      1       // 2行目

#define CLOCK		96		// 動作周波数[MHz]

/******************************** 自動生成関数 *********************************/
#define 	SET_SCI_LCD	R_PG_SCI_Set_C9();	// I2C初期化
#define 	I2C_LCD_SEND	R_PG_SCI_I2CMode_Send_C9(0, LCD_SLAVEADDRESS, word, 2);
#define	    I2C_LCD_READ	R_PG_SCI_I2CMode_Send_C9(0, LCD_SLAVEADDRESS, Command, 2);
/***************************************************************************/
//====================================//
// グローバル変数の宣言
//====================================//

//====================================//
// プロトタイプ宣言
//====================================//
// LCD関連
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