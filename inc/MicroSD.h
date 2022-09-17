﻿#ifndef MICROSD_H_
#define MICROSD_H_
//======================================//
// インクルード
//======================================//
//#include "PeripheralFunctions.h"
#include "E2dataFlash.h"
#include <string.h>
#include <stdarg.h>
//========================================//
// シンボル定義
//=======================================//
#define WRITINGTIME			2		// ログ取得周期(ms)
#define DATA_BYTE			38		// 一回に保存するデータ数(byte)
#define RECODTIME			30000	// 記録時間(ms)
#define MSD_STARTADDRESS	0		// 開始アドレス
#define MSD_ENDADDRESS		960000	// 終了アドレス　RECODTIME * DATA_BYTE / WRITINGTIME

/******************************************** 自動生成関数 *****************************************/
#define SET_SCI_MSD		R_PG_SCI_Set_C5();		// シリアルI/Oチャネルを設定(SPI)
#define SET_CMT_MSD		R_PG_Timer_Set_CMT_U1_C2();	// コンペアマッチタイマ初期化(ch2)
#define START_CMT_MSD	R_PG_Timer_StartCount_CMT_U1_C2();	// カウントスタート(ch2)

// CS端子
#define MSD_CS_TERMINAL_HIGH	R_PG_IO_PORT_Write_PA3( 1 );	// HIGHT
#define MSD_CS_TERMINAL_LOW		R_PG_IO_PORT_Write_PA3( 0 );	// LOW

// SPI
#define SPI_SEND			R_PG_SCI_SPIMode_Transfer_C5( data_tr, data_re, 1);

// SDスイッチ
#define GET_SDSWITCH		R_PG_IO_PORT_Read_PA5(&sd_sw)

/****************************************************************************************************/
//========================================//
// グローバル変数の宣言
//=======================================//
// タイマ関連
extern unsigned int		cnt_log;		// ログ漏れ確認用カウント

// microSD関連
extern signed char		msdBuff[ 512 ];     // 一時保存バッファ
extern short			msdBuffaddress;     // 一時記録バッファ書込アドレス
extern short			msdFlag;            // 1:データ記録 0:記録しない
extern short			msdTimer;           // 取得間隔計算用
extern unsigned int		msdStartaddress;    // 記録開始アドレス
extern unsigned int		msdEndaddress;      // 記録終了アドレス
extern unsigned int		msdWorkaddress;     // 作業用アドレス
extern unsigned int		msdWorkaddress2;	// 作業用アドレス2
extern volatile char	msdlibError;		// エラー番号
extern signed char 		*msdBuffPointa;		// RAM保存バッファ用ポインタ

extern volatile short			msdlibCnt;
extern volatile unsigned char	interrupt_msd_send_data;	// 送信フラグ

extern unsigned int 			msdaddrBuff[25];	// MicroSDカードの最終書き込みアドレス保存用
//========================================//
// プロトタイプ宣言
//=======================================//
void msd_write( unsigned char data );
unsigned char msd_read( void );
unsigned char msd_CMD ( unsigned char cmd, unsigned char arg1, unsigned char arg2,
			unsigned char arg3, unsigned char arg4, unsigned char crc );
char initMicroSD ( void );
void init_log ( void );
bool getMicroSD_CSD( volatile unsigned char *p );
char readMicroSD ( unsigned int address, signed char *read );
char writeMicroSD ( unsigned int address, signed char *write );
char eraseMicroSD( unsigned int st_address, unsigned int ed_address );
char setMicroSDdata( signed char *p );
char microSDProcessStart( unsigned int address );
char microSDProcessEnd( void );
void microSDProcess( void );
char checkMicroSDProcess( void );
void sendLog (char c, char s, char i, ...);
char msdEndLog ( void );
void msd_send_data (void);
void msdgetData ( void ) ;
void send_Char ( char data );
void send_ShortToChar ( short data );
void send_uIntToChar ( unsigned int data );
short CharToShort( unsigned char offsetaddress );
unsigned int CharTouInt( unsigned char offsetaddress );
void msd_sendToPC ( void );

#endif /* MICROSD_H_ */