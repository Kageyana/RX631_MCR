//====================================//
// インクルード
//====================================//
#include "io.h"
//====================================//
// グローバル変数の宣言
//====================================//
// スイッチ関連
static unsigned char 	dpsw_d[4];	// ディップスイッチの格納先
static unsigned char	tasw_d[5];	// スイッチ値の格納先
/////////////////////////////////////////////////////////////////////
// モジュール名 initIO
// 処理概要     IOポートの初期化
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void initIO(void)
{
	// I/Oポートを設定
	R_PG_IO_PORT_Set_P1();
	R_PG_IO_PORT_Set_P2();
	R_PG_IO_PORT_Set_P3();
	R_PG_IO_PORT_Set_P5();
	R_PG_IO_PORT_Set_PA();
	R_PG_IO_PORT_Set_PB();
	R_PG_IO_PORT_Set_PC();
	R_PG_IO_PORT_Set_PE();
	
	// すべてのIOポートをLOWにする
	R_PG_IO_PORT_Write_P1(0);
	R_PG_IO_PORT_Write_P2(0);
	R_PG_IO_PORT_Write_P3(0);
	R_PG_IO_PORT_Write_P5(0);
	R_PG_IO_PORT_Write_PA(0);
	R_PG_IO_PORT_Write_PB(0);
	R_PG_IO_PORT_Write_PC(0);
	R_PG_IO_PORT_Write_PE(0);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 ledOut
// 処理概要     LEDの点灯
// 引数         rgb 	0x1:青 0x2緑 0x4
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void ledOut ( char rgb )
{
	if ( (rgb & 0x4) == 0x4 ) LEDR_ON
	else LEDR_OFF
	
	if ( (rgb & 0x2) == 0x2 ) LEDG_ON
	else LEDG_OFF
	
	if ( (rgb & 0x1) == 0x1 ) LEDB_ON
	else LEDB_OFF
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getSwitch
// 処理概要     スイッチの読み込み(10msごとに実行)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getSwitch(void)
{
	// タクトスイッチ読み込み
	TACTSWITCH1	// タクトスイッチ上
	TACTSWITCH2	// タクトスイッチ左
	TACTSWITCH3	// タクトスイッチ右
	TACTSWITCH4	// タクトスイッチ下
	TACTSWITCH5	// タクトスイッチ押し込み
	
	// ディップスイッチ読み込み
	DIPSWITCH1
	DIPSWITCH2
	DIPSWITCH3
	DIPSWITCH4
}
/////////////////////////////////////////////////////////////////////
// モジュール名 dipswGet
// 処理概要     ディップスイッチ値を16進数で取得
// 引数         なし
// 戻り値       スイッチ値 0～15
/////////////////////////////////////////////////////////////////////
unsigned char dipswGet(void) 
{
	char	dpsw[4];
	
	if ( dpsw_d[0] == 0 )	dpsw[0] = 0x0;
	else			dpsw[0] = 0x1;
	
	if ( dpsw_d[1] == 0 )	dpsw[1] = 0x0;
	else			dpsw[1] = 0x2;
	
	if ( dpsw_d[2] == 0 )	dpsw[2] = 0x0;
	else			dpsw[2] = 0x4;

	if ( dpsw_d[3] == 0 )	dpsw[3] = 0x0;
	else			dpsw[3] = 0x8;

	return ( dpsw[0] + dpsw[1] + dpsw[2] + dpsw[3] );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 taswGet
// 処理概要     タクトスイッチ値を16進数で取得
// 引数         なし
// 戻り値       スイッチ値 0～7
///////////////////////////////////////////////////////////////////////////
unsigned char taswGet(void) 
{
	char	tasw[5];
	
	if ( tasw_d[0] == 0 )	tasw[0] = 0x1;
	else			tasw[0] = 0x0;
	
	if ( tasw_d[1] == 0 )	tasw[1] = 0x2;	
	else			tasw[1] = 0x0;
	
	if ( tasw_d[2] == 0 )	tasw[2] = 0x4;	
	else			tasw[2] = 0x0;
	
	if ( tasw_d[3] == 0 )	tasw[3] = 0x8;	
	else			tasw[3] = 0x0;
	
	if ( tasw_d[4] == 0 )	tasw[4] = 0xf;
	else			tasw[4] = 0x0;

	return ( tasw[0] + tasw[1] + tasw[2] + tasw[3] + tasw[4] );
}