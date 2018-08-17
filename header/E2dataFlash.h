///////////////////////////////メモ///////////////////////////////////////
// 	書き込みアドレス記録領域1	0～10ブロック			//
//	パラメータ保存領域		11～879ブロック			//
//									//
//	書き込みアドレス記録領域2	1022ブロック			//
//	Angle0保存領域			1023ブロック			//
//									//
//	書き込みアドレス記録領域3	1000～1010ブロック		//
//	msdWorkAddress保存領域		1011～1021ブロック		//
//									//
//	書き込みアドレス記録領域4	970～979ブロック		//
//	白線トレース用PIDゲイン保存領域	980～999ブロック		//
//									//
//	書き込みアドレス記録領域5	940～949ブロック		//
//	角度制御用PIDゲイン保存領域	950～969ブロック		//
//									//
//	書き込みアドレス記録領域6	910～919ブロック		//
//	速度制御用PIDゲイン保存領域	920～939ブロック		//
//									//
//	書き込みアドレス記録領域7	880～889ブロック		//
//	停止距離保存領域		890～909ブロック		//
//////////////////////////////////////////////////////////////////////////
#ifndef E2DATAFLASH_H_
#define E2DATAFLASH_H_
//======================================//
// インクルード                         //
//======================================//
#include "iodefine.h"
#include "E2dataFlash.h"
#include "PeripheralFunctions.h"
#include "SetUp.h"
#include "LineChase.h"
#include "MicroSD.h"
#include <stdio.h>
//======================================//
// シンボル定義                         //
//======================================//
#define NUMDATA		24			// 要素数
#define DATASIZE	( 2 * NUMDATA ) + 2	// ( 2バイト　* 要素数 ) + 2バイト

#define PARAMETER_AREA	879			// パラメータ保存領域

#define ANGLE0_DATA	1023			// Angle0保存領域
#define ANGLE0_AREA	1022			// Angle0書き込みアドレス記録領域

#define MSD_DATA	1021			// msdWorkAddress保存領域
#define MSD_STARTAREA	1000			// msdWorkAddress書き込みアドレス記録領域1
#define MSD_ENDAREA	1010			// msdWorkAddress書き込みアドレス記録領域2

#define PID_DATA	999			// 白線トレース用PIDゲイン保存領域
#define PID_STARTAREA	970			// 白線トレース用PIDゲイン書き込みアドレス記録領域1
#define PID_ENDAREA	979			// 白線トレース用PIDゲイン書き込みアドレス記録領域2

#define PID2_DATA	969			// 角度制御用PIDゲイン保存領域
#define PID2_STARTAREA	940			// 角度制御用PIDゲイン書き込みアドレス記録領域1
#define PID2_ENDAREA	949			// 角度制御用PIDゲイン書き込みアドレス記録領域2

#define PID3_DATA	939			// 速度制御用PIDゲイン保存領域
#define PID3_STARTAREA	910			// 速度制御用PIDゲイン書き込みアドレス記録領域1
#define PID3_ENDAREA	919			// 速度制御用PIDゲイン書き込みアドレス記録領域2

#define STOPMETER_DATA		909		// 速度制御用PIDゲイン保存領域
#define STOPMETER_STARTAREA	880		// 速度制御用PIDゲイン書き込みアドレス記録領域1
#define STOPMETER_ENDAREA	889		// 速度制御用PIDゲイン書き込みアドレス記録領域2
//======================================//
// グローバル変数の宣言                 //
//======================================//
extern short flashDataBuff[45];			// 一時保存バッファ
//======================================//
// プロトタイプ宣言                     //
//======================================//
void firmwarecopy ( void );
void fcuCommandByte ( unsigned short block_number, unsigned char command, char addr_number );
void fcuCommandWord ( unsigned short block_number, unsigned short command, char addr_number );
char checkFRDY ( unsigned short waittime );
char checkErrorFlash ( void );
char initFlash ( void );
void changeFlashPE ( void );
void changeFlashRead ( void );
char checkBlank ( short block_number, char addr_offset );
char eraseE2DataFlash ( unsigned short block_number );
void checkWriteAddr ( short startblock, short endblock, char startoffset, short width_data );
char writeFlash ( short* write_data, short width_data );
short readFlashBlock ( short block_number, char addr_number );
short readFlashAddr ( unsigned int addr );
void readFlashArray ( volatile short* array, short width_data );
void readFlashArray2 ( volatile short* dataArray, short width_data, short limitArea );
void readBeforeAddr ( short startBlockNumber, short endBlockNumber );
void writeFlashData ( short startBlockNumber, short endBlockNumber, short endData, short width_data );
void readFlashSetup ( void );
void writeFlashBeforeStart ( void );

#endif // E2DATAFLASH_H_