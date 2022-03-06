//====================================//
// インクルード
//====================================//
#include "MemorryTrace.h"
//====================================//
// グローバル変数の宣言
//====================================//
// ログ解析関連
char			comp_char[10][100];
short			comp_short[10][100];
unsigned int	comp_uint[10][100];
char			flag[10][100];
unsigned short	cntp[10];
char 			cntmpattern[500];
short			cntmpattern2;

char			mpattern = 11;
char			memory_mode = 0;
///////////////////////////////////////////////////////////////////////////
// モジュール名 msdgetData
// 処理概要     ログから角各処理開始時の位置を見つける
// 引数         なし
// 戻り値       0:正常に終了 1:異常終了
///////////////////////////////////////////////////////////////////////////
void msdgetData () 
{
	volatile unsigned short i;
	char j,k;
	volatile short ret;
	volatile char pattern_send = 1;
	mpattern = 11;
	cntmpattern2 = 0;
	
	msdEndaddress = msdWorkaddress2;	// 読み込み終了アドレス
	msdWorkaddress = msdWorkaddress;	// 読み込み開始アドレス
	
	for ( j = 0;j < 10; j++ ) {
		for ( k = 0;k < 100; k++ ) {
			comp_char[ j ][ k ] = 0;
			comp_short[ j ][ k ] = 0;
			comp_uint[ j ][ k ] = 0;
			flag[ j ][ k ] = 0;
		}
	}
	
	while ( pattern_send < 4 ) {
		switch ( pattern_send ) {			
			case 1:
				// microSDよりデータ読み込み
				if( msdWorkaddress >= msdEndaddress ) {
					// 書き込み終了アドレスになったら、終わり
					cntmpattern2 = 0;
					memory_mode = 1;
					pattern_send = 4;
					break;
				}
				ret = readMicroSD( msdWorkaddress , msdBuff );
				
				if( ret != 0x00 ) {
					// 読み込みエラー
					printf( "\nmicroSD Read Error!!\n" );
					pattern_send = 4;
					break;
				} else {
					// エラーなし
					msdWorkaddress += 512;	// microSDのアドレスを+512する
					msdBuffaddress = 0;		// 配列からの読み込み位置を0に
					pattern_send = 2;
					break;
				}
				break;
				
			case 2:
				switch( mpattern ) {
					case 11:
						if ( serchPattern( STRAIGHT, 12 ) ) mpattern = 12;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 12:
						if ( serchPattern( STRAIGHT, 13 ) ) mpattern = 13;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						/*
					case 13:
						if ( serchPattern( STRAIGHT, 14 ) ) mpattern = 14;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 14:
						if ( serchPattern( STRAIGHT, 15 ) ) mpattern = 15;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 15:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						*/
					case 21:
						if ( serchPattern( CROSSLINE, 22 ) ) mpattern = 22;
						break;
						/*
					case 22:
						if ( serchPattern( RC, 31 ) ) mpattern = 31;
						if ( serchPattern( LC, 41 ) ) mpattern = 41;
						break;
						/*
					case 31:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 41:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						*/
					case 51:
						if ( serchPattern( RLC, 52 ) ) mpattern = 52;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						break;
						/*
					case 52:
						if ( serchPattern( RLC, 53 ) ) mpattern = 53;
						break;
						
					case 53:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						*/
					case 61:
						if ( serchPattern( LLC, 62 ) ) mpattern = 62;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						break;
						
					case 62:
						if ( serchPattern( LLC, 63 ) ) mpattern = 63;
						break;
						/*
					case 63:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						*/
					case 71:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					
				}
				
				msdBuffaddress += DATA_BYTE;
				
				if( msdBuffaddress >= 512 ) {
					pattern_send = 1;
					break;
				}
				break;
		}
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 serchPattern
// 処理概要     ログから各処理開始時の位置を見つける
// 引数         なし
// 戻り値       0:正常に終了 1:異常終了
///////////////////////////////////////////////////////////////////////////
bool serchPattern ( char process, char spattern )
{
	// flag[ STRAIGHT ][0]	0:突入位置 		1: 脱出位置
	// flag[ STRAIGHT ][ 1 ]	繰り返し回数
	// cnt[ STRAIGHT ][0] 	カーブ突入､脱出位置の数
	// comp_uint[ STRAIGHT ][ cnt[ STRAIGHT ][ 0 ] ]	EncoderTotalを格納する
	// カーブを脱出する位置を探す
	// spattern を3つ見つける
	if ( msdBuff[ msdBuffaddress + 0 ] == spattern ) flag[ process ][ spattern ]++;
	else		flag[ process ][ spattern ] = 0;
	
	// 3つ以上あればカーブを脱出したと判断する
	if ( flag[ process ][ spattern ] >= 3 ) {
		cntmpattern[ cntmpattern2 ] = spattern;	// patternを記録 
		comp_uint[ process ][ cntmpattern2++ ] = CharTouInt (22);		// 距離取得
		flag[ process ][ spattern ] = 0;	// 繰り返しカウントをリセット
		printf("cntmpattern[ %d ] = %d\n", cntmpattern2 - 1, cntmpattern[ cntmpattern2 - 1 ]);	
		printf("comp_uint[%d][%d] = %d\n", process, cntmpattern2 - 1, comp_uint[ process ][ cntmpattern2 - 1 ] );
		printf("logpattern = %d\n",cntmpattern[ cntmpattern2-1 ] / 10);
		return true;
	} else {
		return false;
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 logmeter
// 処理概要     ログから前回の位置でのpatternを表示する
// 引数         なし
// 戻り値       0:正常に終了 1:異常終了
///////////////////////////////////////////////////////////////////////////
char logmeter( void )
{
	char logpattern;
	
	logpattern = cntmpattern[ cntmpattern2 ] / 10;		// 記録したpatternを呼び出し
	if ( comp_uint[ logpattern ][ cntmpattern2 ] ) {	
		if ( comp_uint[ logpattern ][ cntmpattern2 ] <= EncoderTotal ) {
			cntmpattern2++;
		}
	}
	return cntmpattern[ cntmpattern2 ];
}