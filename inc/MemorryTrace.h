#ifndef MEMMORYTRACE_H_
#define MEMMORYTRACE_H_
//====================================//
// インクルード
//==================================~=//
#include "microSD.h"
//====================================//
// シンボル定義
//====================================//
// ログ解析関連
#define STRAIGHT		1		// 直線加速
#define CROSSLINE		2		// クロスライン減速
#define RC			    3		// 右クランク
#define LC				4		// 左クランク
#define RLC			    5		// 右レーンチェンジ
#define LLC			    6		// 左レーンチェンジ
#define SLUP			7		// 上り始め
#define SLDOWN		    8		// 下り終わり
#define SLMIDDLE		9		// 上り終わり､下り始め
//====================================//
// グローバル変数の宣言
//====================================//
// ログ解析関連
extern char		comp_char[10][100];
extern short		comp_short[10][100];
extern unsigned int	comp_uint[10][100];

extern short		cntmpattern2;
extern char		memory_mode;
//====================================//
// プロトタイプ宣言
//====================================//
bool serchPattern ( char process, char spattern );
char logmeter( void );
#endif /* MEMMORYTRACE_H */