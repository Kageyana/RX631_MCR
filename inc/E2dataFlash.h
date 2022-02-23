///////////////////////////////メモ///////////////////////////////////////
// 	書き込みアドレス記録領域1	0～10ブロック
//	パラメータ保存領域		    11～879ブロック
//	
//	書き込みアドレス記録領域2	1022~1023ブロック
//	Angle0保存領域			    1019~1021ブロック
//	
//	書き込みアドレス記録領域3	    997～1007ブロック
//	msdWorkAddress保存領域          1008～1018ブロック
//	
//	書き込みアドレス記録領域4	    967～976ブロック
//	白線トレース用PIDゲイン保存領域	977～996	ブロック
//	
//	書き込みアドレス記録領域5	937～946ブロック
//	角度制御用PIDゲイン保存領域	947～966ブロック
//	
//	書き込みアドレス記録領域6	907～916ブロッ
//	速度制御用PIDゲイン保存領域	917～936ブロック
//	
//	書き込みアドレス記録領域7	877～886ブロック
//	停止距離保存領域			887～906ブロック
//////////////////////////////////////////////////////////////////////////
#ifndef E2DATAFLASH_H_
#define E2DATAFLASH_H_
//======================================//
// インクルード
//======================================//
#include "iodefine.h"
#include "io.h"
#include "mtu.h"
#include "ADconverter.h"
#include "setup.h"
#include "control.h"
#include "microSD.h"
#include <stdio.h>
//======================================//
// シンボル定義
//======================================//
#define NUMDATA		24				// 要素数
#define DATASIZE		( 2 * NUMDATA ) + 2	// ( 2バイト　* 要素数 ) + 2バイト
#define DATA			2				// 1回の書き込みサイズ2byte
#define CLOCK			96				// 動作周波数[MHz]
#define FLASHSTARTADDR	0x00100000		// フラッシュ開始アドレス

// 記録領域
#define PARAMETER_AREA			879		// パラメータ保存領域
#define PARAMETER_STARTAREA	    0		// パラメータ書き込みアドレス記録領域1
#define PARAMETER_ENDAREA		10		// パラメータ書き込みアドレス記録領域2

#define ANGLE0_DATA		    1023			// Angle0保存領域
#define ANGLE0_STARTAREA	1019			// Angle0書き込みアドレス記録領域1
#define ANGLE0_ENDAREA		1021			// Angle0書き込みアドレス記録領域2

#define MSD_DATA			1018			// msdWorkAddress保存領域
#define MSD_STARTAREA		997			// msdWorkAddress書き込みアドレス記録領域1
#define MSD_ENDAREA		    1007			// msdWorkAddress書き込みアドレス記録領域2

#define PID_DATA			996			// 白線トレース用PIDゲイン保存領域
#define PID_STARTAREA		967			// 白線トレース用PIDゲイン書き込みアドレス記録領域1
#define PID_ENDAREA		    976			// 白線トレース用PIDゲイン書き込みアドレス記録領域2

#define PID2_DATA			966			// 角度制御用PIDゲイン保存領域
#define PID2_STARTAREA		937			// 角度制御用PIDゲイン書き込みアドレス記録領域1
#define PID2_ENDAREA		946			// 角度制御用PIDゲイン書き込みアドレス記録領域2

#define PID3_DATA			936			// 速度制御用PIDゲイン保存領域
#define PID3_STARTAREA		907			// 速度制御用PIDゲイン書き込みアドレス記録領域1
#define PID3_ENDAREA		916			// 速度制御用PIDゲイン書き込みアドレス記録領域2

#define STOPMETER_DATA			906		// 速度制御用PIDゲイン保存領域
#define STOPMETER_STARTAREA	    877		// 速度制御用PIDゲイン書き込みアドレス記録領域1
#define STOPMETER_ENDAREA		886		// 速度制御用PIDゲイン書き込みアドレス記録領域2
//======================================//
// グローバル変数の宣言
//======================================//
// タイマ関連
extern unsigned short		cnt_flash;	// フラッシュ用カウント

extern short			flashDataBuff[45];	// 一時保存バッファ

extern volatile unsigned int 		beforeAddr;	// 前回保存時のブロック番号
//======================================//
// プロトタイプ宣言
//======================================//
void wait_flash ( short waitTime );
void FirmWareCopy ( void );
void fcuCommandByte ( unsigned int Addr, unsigned char command );
void fcuCommandWord ( unsigned int Addr, unsigned short command );
bool checkFRDY ( unsigned short waittime );
bool checkErrorFlash ( void );
bool initFlash ( void );
void changeFlashPE ( void );
void changeFlashRead ( void );
signed char checkBlank ( unsigned int Addr );
bool eraseE2DataFlash ( unsigned short Block_number );
void checkWriteAddr ( unsigned int Addr, short endblock, short width_data, short startBlock );
bool writeFlash ( short* write_data, short width_data );
short readFlashAddr ( unsigned int Addr );
void readFlashArray ( unsigned int Addr, volatile short* dataArray, short width_data );
void readFlashArray2 ( unsigned int Addr, volatile short* dataArray, short width_data, short limitArea );
void readBeforeAddr ( short startBlockNumber, short endBlockNumber );
void writeFlashData ( short startBlockNumber, short endBlockNumber, short endData, short width_data );
void readFlashSetup ( bool speed, bool C_angle, bool msd, bool pid_line, bool pid_angle, bool pid_speed, bool meter);
void writeFlashBeforeStart ( bool speed, bool C_angle, bool pid_line, bool pid_angle, bool pid_speed, bool meter );

#endif // E2DATAFLASH_H_