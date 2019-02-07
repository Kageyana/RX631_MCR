//====================================//
// インクルード									//
//====================================//
#include "MemorryTrace.h"
//====================================//
// グローバル変数の宣言							//
//====================================//
// ログ解析関連
char			comp_char[10][100] = {0,0,0,0,0,0,0,0,0,0};
short			comp_short[10][100] = {0,0,0,0,0,0,0,0,0,0};
unsigned int	comp_uint[10][100] = {0,0,0,0,0,0,0,0,0,0};
char			flag[10][100];
unsigned short	cntp[10];
char 			cntmpattern[500];
char			i;

char			mpattern = 11;
///////////////////////////////////////////////////////////////////////////
// モジュール名 msdgetData								//
// 処理概要     ログから角各処理開始時の位置を見つける			//
// 引数         なし									//
// 戻り値       0:正常に終了 1:異常終了						//
///////////////////////////////////////////////////////////////////////////
void msdgetData () 
{
	volatile unsigned short i;
	volatile short ret;
	volatile char pattern_send = 1;
	flag[STRAIGHT][0] = 12;
	mpattern = 11;
	
	msdEndaddress = msdWorkaddress2;	// 読み込み終了アドレス
	msdWorkaddress = msdWorkaddress;	// 読み込み開始アドレス
	
	while ( pattern_send < 4 ) {
		switch ( pattern_send ) {			
			case 1:
				// microSDよりデータ読み込み
				if( msdWorkaddress >= msdEndaddress ) {
					// 書き込み終了アドレスになったら、終わり
					//printf( "End.\n" );
					setBeepPatternS( 0xa8a8 );
					pattern_send = 4;
					break;
				}
				ret = readMicroSD( msdWorkaddress , msdBuff );
				
				if( ret != 0x00 ) {
					// 読み込みエラー
					//printf( "\nmicroSD Read Error!!\n" );
					pattern_send = 4;
					break;
				} else {
					// エラーなし
					msdWorkaddress += 512;		// microSDのアドレスを+512する
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
						
					case 21:
						if ( serchPattern( CROSSLINE, 22 ) ) mpattern = 22;
						break;
						
					case 22:
						if ( serchPattern( RC, 31 ) ) mpattern = 31;
						if ( serchPattern( LC, 41 ) ) mpattern = 41;
						break;
						
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
						
					case 51:
						if ( serchPattern( RLC, 52 ) ) mpattern = 52;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						break;
						
					case 52:
						if ( serchPattern( RLC, 53 ) ) mpattern = 53;
						break;
						
					case 53:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 61:
						if ( serchPattern( LLC, 62 ) ) mpattern = 62;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						break;
						
					case 62:
						if ( serchPattern( LLC, 63 ) ) mpattern = 63;
						break;
						
					case 63:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
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
// モジュール名 serchPattern							//
// 処理概要     ログから各処理開始時の位置を見つける			//
// 引数         なし									//
// 戻り値       0:正常に終了 1:異常終了						//
///////////////////////////////////////////////////////////////////////////
bool serchPattern ( char process, char spattern )
{
	// flag[ STRAIGHT ][0]	0:突入位置 		1: 脱出位置
	// flag[ STRAIGHT ][ 1 ]	繰り返し回数
	// cnt[ STRAIGHT ][0] 	カーブ突入､脱出位置の数
	// comp_uint[ STRAIGHT ][ cnt[ STRAIGHT ][ 0 ] ]	EncoderTotalを格納する
	// printf("%2d, %6d\n", msdBuff[ msdBuffaddress + 0 ], CharTouInt (40) );		// EncoderTotal
	// カーブを脱出する位置を探す
	// spattern を3つ見つける
	if ( msdBuff[ msdBuffaddress + 0 ] == spattern ) flag[ process ][ spattern ]++;
	else		flag[ process ][ spattern ] = 0;
	
	// 3つ以上あればカーブを脱出したと判断する
	if ( flag[ process ][ spattern ] >= 3 ) {
		comp_uint[ process ][ cntp[ process ] ] = CharTouInt (40);		// 距離取得
		//printf("%d, %6d\n", spattern, comp_uint[ process ][ cnt[ process ] ] );
		cntp[ process ]++;
		flag[ process ][ spattern ] = 0;	// 繰り返しカウントをリセット
		cntmpattern[ i ] = spattern;
		i++;
		return true;
	} else {
		return false;
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 serchPattern							//
// 処理概要     ログから各処理開始時の位置を見つける			//
// 引数         なし									//
// 戻り値       0:正常に終了 1:異常終了						//
///////////////////////////////////////////////////////////////////////////
void logmeter( void )
{
	short k, m, l;
	
	if (comp_uint[ STRAIGHT ][ k ]) {
		if ( comp_uint[ STRAIGHT ][ k ] <=EncoderTotal && m >= 0) {
			l = 12;
			m = -m;
			k++;
		} else if ( comp_uint[ STRAIGHT ][ k ] <= EncoderTotal && m < 0 ) {
			l = 11;
			m = -m;
			k++;
		}
	}
}