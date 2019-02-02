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
char			flag[100][20] = {0,0,0,0,0,0,0,0,0,0};
short			cnt[10];

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
	cnt[0] = 0;
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
				// flag[ STRAIGHT ][0]	0:突入位置 		1: 脱出位置
				// flag[ STRAIGHT ][ 1 ]	繰り返し回数
				// cnt[ STRAIGHT ][0] 	カーブ突入､脱出位置の数
				// comp_uint[ STRAIGHT ][ cnt[ STRAIGHT ][ 0 ] ]	EncoderTotalを格納する
				// printf("%2d, %6d\n", msdBuff[ msdBuffaddress + 0 ], CharTouInt (40) );		// EncoderTotal
				switch( mpattern ) {
					case 11:
						if( serchPattern( STRAIGHT, 12 ) ) mpattern = 12;
						break;
						
					case 12:
						if( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
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
	// カーブを脱出する位置を探す
	// spattern を3つ見つける
	if ( msdBuff[ msdBuffaddress + 0 ] == spattern ) flag[ process ][ spattern ]++;
	else		flag[ process ][ spattern ] = 0;
	
	// 3つ以上あればカーブを脱出したと判断する
	if ( flag[ process ][ spattern ] >= 3 ) {
		comp_uint[ process ][ cnt[ process ] ] = CharTouInt (40);		// 距離取得
		//printf("%d, %6d\n", spattern, comp_uint[ process ][ cnt[ process ] ] );
		cnt[ process ]++;
		flag[ process ][ spattern ] = 0;	// 繰り返しカウントをリセット
		return true;
	} else {
		return false;
	}
}