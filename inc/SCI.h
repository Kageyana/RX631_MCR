#ifndef SCI_H_
#define SCI_H_
//====================================//
// インクルード                 							//
//====================================//
#include "iodefine.h"
#include "machine.h"
#include <stdio.h>
//====================================//
// シンボル定義									//
//====================================//
#define	RATE_9600	0x0
#define	RATE_14400	0x1
#define	RATE_19200	0x2
#define	RATE_38400	0x3
#define	RATE_57600	0x4
#define	RATE_115200	0x5
#define	RATE_230400	0x6
#define	RATE_500000	0x7
#define	RATE_750000	0x8
#define	RATE_1000000	0x9
#define	RATE_1500000	0xa
#define	RATE_3000000	0xb
//====================================//
// グローバル変数の宣言								//
//====================================//

//====================================//
// プロトタイプ宣言									//
//====================================//
void init_SCI1( char rate );
void init_SCI6( char rate );
#endif /* SCI_H_ */