#ifndef SCI_H_
#define SCI_H_
//======================================//
// インクルード
//======================================//
#include "iodefine.h"
#include <machine.h>
#include <stdio.h>
#include <string.h>
#include "R_PG_IGC-P8080_v1.h"
#include "io.h"
//======================================//
// シンボル定義
//======================================//
#define	RATE_9600	    0x0
#define	RATE_14400	    0x1
#define	RATE_19200	    0x2
#define	RATE_38400	    0x3
#define	RATE_57600	    0x4
#define	RATE_115200	    0x5
#define	RATE_230400	    0x6
#define	RATE_500000	    0x7
#define	RATE_750000	    0x8
#define	RATE_1000000	0x9
#define	RATE_1500000	0xa
#define	RATE_3000000	0xb

#define	COMMAND1	"$br,"		// ボーレート設定
#define	STOPWORD	"stop"

// 割り込み優先度
#define	IPR_RXI1		14
#define	IPR_TXI1		13
#define	IPR_TEI1		12

#define	IPR_RXI5		11
#define	IPR_TXI5		10
#define	IPR_TEI5		9

#define	IPR_RXI12		9
#define	IPR_TXI12		8
#define	IPR_TEI12		7

#define	IPR_RXI11		14
#define	IPR_TXI11		13
#define	IPR_TEI11		12

#define	UART			1
#define	I2C			2
#define	SPI			3
#define	RW_BIT		1

#define	PRINT_ON	1
#define	PRINT_OFF	0
//====================================//
// グローバル変数の宣言
//====================================//
extern char revErr;

// SCI1関連
extern char	    modeSCI1;		    // 通信方式
extern char     txtCommand[128];	// コマンド格納
extern char     txtData[128];		// データ格納
extern char*    txt;				// 受信データ格納用ポインタ
extern char	    cmmandMode;		    // コマンド選択
extern char	    stopWord;			// 0: 停止ワード未受信 1:停止ワード受信
extern short    cntByte;			// 受信したバイト数
extern char	    command;			// 0:コマンド受信待ち 1:コマンド入力中 2:コマンド判定中

extern char	    SCI1_Req_mode;	    // 0:スタート 1:ストップ 2;リスタート 3:データ送受信中
extern char	    SCI1_RW_mode;	    // 0:受信 1:送信 2:レジスタ読み込み
extern char	    SCI1_Slaveaddr;	    // スレーブアドレス
extern char	    SCI1_NumData;		// 送信データ数
extern char	    SCI1_NumData2;	    // 送信データ数2
extern char*    SCI1_DataArry;		// 送信データ配列
extern char*	SCI1_DataArry2; 	// 送信データ配列2
extern char	    SCI1_DataBuff[255];	// 送信データバッファ

// SCI12関連
extern char	    SCI12_Req_mode;		    // 0:スタート 1:ストップ 2;リスタート 3:データ送受信中
extern char	    SCI12_Slaveaddr;		// スレーブアドレス
extern char	    SCI12_NumData;		    // 送信データ数
extern char*	SCI12_DataArry;		    // 送信データ配列
extern char	    SCI12_DataBuff[255];	// 送信データバッファ

extern char	    ascii_num[];
//====================================//
// プロトタイプ宣言
//====================================//
void initSCI1( char rate );
void initSCI6( char rate );

#endif /* SCI_H_ */