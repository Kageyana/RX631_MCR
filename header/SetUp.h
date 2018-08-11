#ifndef SETUP_H_
#define SETUP_H_
//======================================//
// シンボル定義                         //
//======================================//

//======================================//
// グローバル変数の宣言                 //
//======================================//
// パターン関連
extern char		start;
extern unsigned short 	cnt_setup;
extern unsigned short 	cnt_setup2;

// パラメータ関連
extern char fixSpeed;

//======================================//
// プロトタイプ宣言                     //
//======================================//
void setup(void);
char fix_speedsetting ( void );

#endif /* SCI_H_ */