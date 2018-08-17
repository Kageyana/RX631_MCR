//======================================//
// インクルード                         //
//======================================//
#include "E2dataFlash.h"
//======================================//
// グローバル変数の宣言                 //
//======================================//
// フラッシュ関連
static volatile short 		BeforeBlockNumber;	// 前回保存時のブロック番号
static volatile short 		BeforeAddrNumber;	// 前回保存時のオフセット値
static volatile short 		EndBlockNumber;		// 今回書き込みの最終ブロック番号
static volatile short 		EndAddrOffset;		// 今回書き込みの最終オフセット値
short 				flashDataBuff[45];	// 一時保存バッファ

static volatile short 		NowBlockNumber;		// 現在の書き込まれているブロック番号
static volatile short 		NowAddrOffset;		// 現在の書き込まれているオフセット値
//////////////////////////////////////////////////////////////////////////
// モジュール名 firmwarecopy						//
// 処理概要     FCU RAMへのファームウェアコピー				//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void firmwarecopy ( void )
{
	short i;
	static const int fcu_ram_size = 8 * 1024;				// ファームウェアサイズ
	volatile unsigned int *fcu_ram = (unsigned int *)0x007f8000;		// FCU RAMのアドレス
	const volatile unsigned int *fcu_firmware = (unsigned int *)0xfeffe000;	// FCUファームウェアのアドレス
	
	
	// ROMリードモード確認
	if (  FLASH.FENTRYR.WORD & 0x00ff ) FLASH.FENTRYR.WORD = 0xaa00;
	
	FLASH.FCURAME.WORD = 0xc401;	// FCU RAMアクセス許可

	// FCUファームウェアをFCU RAMにコピー
	for ( i = 0; i < fcu_ram_size / sizeof( unsigned int ); i++ ) {
		*fcu_ram++ = *fcu_firmware++;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 fcuCommandByte							//
// 処理概要     指定アドレスにFCUコマンド発行						//
// 引数         block_number: ブロック番号　command:FCUコマンド(16進数)バイトサイズ 	//
//		addr_number: 先頭アドレスからのオフセット値				//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void fcuCommandByte ( unsigned short block_number, unsigned char command, char addr_number )
{
	volatile unsigned char *e2data;
	volatile unsigned int addr;
	
	addr = ( block_number * 32 ) + 0x00100000 + addr_number;	// ブロックNのアドレスを算出
	
	e2data = ( unsigned char *)addr;	// ポインタにアドレス代入
	*e2data = command;			// 指定アドレスにコマンド発行
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 fcuCommandWord							//
// 処理概要     指定アドレスにFCUコマンド発行						//
// 引数         block_number: ブロック番号　command:FCUコマンド(16進数)ワードサイズ 	//
//		addr_number: 先頭アドレスからのオフセット値				//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void fcuCommandWord ( unsigned short block_number, unsigned short command, char addr_number )
{
	volatile unsigned short *e2data;
	volatile unsigned int addr;
	
	addr = ( block_number * 32 ) + 0x00100000 + addr_number;	// ブロックNのアドレスを算出
	
	e2data = ( unsigned short *)addr;	// ポインタにアドレス代入
	*e2data = command;			// 指定アドレスにコマンド発行
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 checkFRDY						//
// 処理概要     P/E処理中か判定する					//
// 引数         nバイトのブロックに対する消去時間			//
// 戻り値      	0: エラーなし 1:タイムアウトエラー			//
//////////////////////////////////////////////////////////////////////////
char checkFRDY ( unsigned short waittime )
{
	volatile char timeout = 0;
	
	cnt_flash = 0;
	
	while( cnt_flash <= ( waittime + 10 ) ) {
		//printf("FRDY = %d cnt_flash = %d\n", FLASH.FSTATR0.BIT.FRDY, cnt_flash);
		if ( FLASH.FSTATR0.BIT.FRDY == 0 ) {
			if ( cnt_flash >= waittime ) {
				cnt_flash = 0;
				timeout = 1;
				printf("FCU初期化\n");
				break;
			} else {
				continue;
			}
		} else {
			timeout = 0;
			break;
		}
	}
	
	// FCU初期化
	if ( timeout == 1 ) {
		cnt_flash = 0;
		while( cnt_flash <= 5 ) {
			FLASH.FRESETR.BIT.FRESET = 1;
			if ( cnt_flash >= 5 ) {
				FLASH.FRESETR.BIT.FRESET = 0;
				firmwarecopy();
				printf("FCU初期化完了\n");
				break;
			}
		}
	} else if ( timeout == 0 ){
		//printf("タイムアウトなし\n");
	}
	return timeout;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 checkErrorFlash					//
// 処理概要     エラー確認						//
// 引数         なし							//
// 戻り値       0: エラーなし 1:エラーあり				//
//////////////////////////////////////////////////////////////////////////
char checkErrorFlash ( void )
{
	volatile char error;
	
	if ( FLASH.FSTATR0.BIT.ILGLERR == 1 || FLASH.FSTATR0.BIT.ERSERR == 1 || FLASH.FSTATR0.BIT.PRGERR == 1 ) {
		printf("エラー確認 ILGLERR = %d ERSERR = %d PRGERR = %d\n", FLASH.FSTATR0.BIT.ILGLERR, FLASH.FSTATR0.BIT.ERSERR, FLASH.FSTATR0.BIT.PRGERR);
		error = 1;
		
		if ( FLASH.FSTATR0.BIT.ILGLERR == 1 ) {
			printf("不正コマンド検出\n");
			printf("ROMAE = %d DFLAE = %d DFLRPE = %d DFLWPE = %d\n", FLASH.FASTAT.BIT.ROMAE, FLASH.FASTAT.BIT.DFLAE, 
			FLASH.FASTAT.BIT.DFLRPE, FLASH.FASTAT.BIT.DFLWPE);
			
			if ( FLASH.FASTAT.BYTE == 0x10 ) {
				fcuCommandByte( 0, 0x50, 0 );
				printf("ステータスクリアコマンド発行(FCUコマンドは受け付けてない)\n");
			} else {
				FLASH.FASTAT.BYTE = 0x10;
				fcuCommandByte( 0, 0x50, 0 );
				printf("ステータスクリアコマンド発行(FCUコマンドを受け付けてる)\n");
			}
		} else {
			fcuCommandByte( 0, 0x50, 0 );
			printf("ステータスクリアコマンド発行(ILGLERR = 0)\n");
		}
	} else {
		//printf("エラーなし\n");
		error = 0;
	}
	
	return error;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 inti_falsh						//
// 処理概要     E2データフラッシュの初期化				//
// 引数         なし							//
// 戻り値       0:エラーなし　1:エラーあり				//
// 補足		初期状態 P/Eノーマルモード				//
//////////////////////////////////////////////////////////////////////////
char initFlash ( void )
{	
	char ret = 0;
	
	// FCUファームウェアをコピー
	firmwarecopy();
	
	// 周辺クロック通知コマンド発行
	// P/Eモードに移行
	if ( ( FLASH.FENTRYR.WORD & 0x00ff ) != 0x0080 ) FLASH.FENTRYR.WORD = 0xaa80;
	
	// E2データフラッシュ領域のP/Eをを許可
	FLASH.DFLWE0.WORD = 0x1eff;
	
	// FCLK(48MHz)を周辺クロック通知レジスタに設定
	FLASH.PCKAR.BIT.PCKA = 48;	
	
	// 周辺クロック通知コマンド発行
	fcuCommandByte( 0, 0xe9, 0 );
	fcuCommandByte( 0, 0x03, 0 );
	fcuCommandWord( 0, 0x0f0f, 0 );
	fcuCommandWord( 0, 0x0f0f, 0 );
	fcuCommandWord( 0, 0x0f0f, 0 );
	fcuCommandByte( 0, 0xd0, 0 );
	
	// P/E処理中か確認
	if ( checkFRDY(1) == 0 ) {
		// エラー確認
		if ( checkErrorFlash() == 0 ) {
			// E2データフラッシュ領域のP/Eをを許可
			FLASH.DFLWE0.WORD = 0x1e00;
		} else {
			printf("E2データフラッシュ初期化エラー\n");
			ret = 1;
		}
	} else {
		printf("E2データフラッシュ初期化エラー\n");
		ret = 1;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 changeFlashPE						//
// 処理概要     E2データフラッシュのP/Eモード移行			//
// 引数         なし							//
// 戻り値       なし							//
// 補足		初期状態 P/Eノーマルモード				//
//////////////////////////////////////////////////////////////////////////
void changeFlashPE ( void )
{
	// P/Eモードに移行
	if ( ( FLASH.FENTRYR.WORD & 0x00ff ) != 0x0080 ) FLASH.FENTRYR.WORD = 0xaa80;
	
	// P/E 可能状態にする
	FLASH.FWEPROR.BYTE = 0x01;
	
	// P/Eモード移行コマンド発行
	fcuCommandByte( 0, 0xff, 0 );
	
	// 処理中確認
	if ( checkFRDY( 300 ) == 0 ) {
		// エラー確認
		if ( checkErrorFlash() == 0 ) {
			if ( checkErrorFlash() == 0 ) {
				//printf("P/E モード移行完了\n");
			} else {
				printf("P/E モード移行エラー\n");
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 changeFlashRead					//
// 処理概要     E2データフラッシュのリードモード移行			//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void changeFlashRead ( void ) 
{
	
	// 処理中確認
	if ( checkFRDY( 300 ) == 0 ) {
		// エラー確認
		if ( checkErrorFlash() == 0 ) {
			// ROM/E2データフラッシュリードモードへ移行
			FLASH.FENTRYR.WORD = 0xaa00;
			if ( ( FLASH.FENTRYR.WORD & 0x00ff ) != 0x0000 ) FLASH.FENTRYR.WORD = 0xaa00;
			// ブランクチェック禁止
			FLASH.FWEPROR.BYTE = 0x02;
			//printf("リードモード移行完了\n");
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
// モジュール名 checkBlank							//
// 処理概要     指定アドレスにブランクチェック					//
// 引数         block_number: ブランクチェックするブロック番号			//
//		addr_offset: 先頭アドレスからのオフセット値			//
// 戻り値       0:イレーズ済み　1:書き込み済み 2:エラーあり			//
//////////////////////////////////////////////////////////////////////////////////
char checkBlank ( short block_number, char addr_offset )
{
	volatile char ret;
	volatile unsigned int addr, lead_addr, offset;

	// P/Eモード移行
	changeFlashPE();
	
	addr = ( block_number * 32 ) + 0x00100000 + addr_offset;	// ブランクチェックするアドレスを算出
	// 各ブロックの先頭アドレス算出
	if ( block_number >= 0 && block_number <= 63 ) lead_addr = ( 0 * 32 ) + 0x00100000;
	if ( block_number >= 64 && block_number <= 127 ) lead_addr = ( 64 * 32 ) + 0x00100000;
	if ( block_number >= 128 && block_number <= 191 ) lead_addr = ( 128 * 32 ) + 0x00100000;
	if ( block_number >= 192 && block_number <= 255 ) lead_addr = ( 192 * 32 ) + 0x00100000;
	if ( block_number >= 256 && block_number <= 319 ) lead_addr = ( 256 * 32 ) + 0x00100000;
	if ( block_number >= 320 && block_number <= 383 ) lead_addr = ( 320 * 32 ) + 0x00100000;
	if ( block_number >= 384 && block_number <= 447 ) lead_addr = ( 384 * 32 ) + 0x00100000;
	if ( block_number >= 448 && block_number <= 511 ) lead_addr = ( 448 * 32 ) + 0x00100000;
	if ( block_number >= 512 && block_number <= 575 ) lead_addr = ( 512 * 32 ) + 0x00100000;
	if ( block_number >= 576 && block_number <= 639 ) lead_addr = ( 576 * 32 ) + 0x00100000;
	if ( block_number >= 640 && block_number <= 703 ) lead_addr = ( 640 * 32 ) + 0x00100000;
	if ( block_number >= 704 && block_number <= 767 ) lead_addr = ( 704 * 32 ) + 0x00100000;
	if ( block_number >= 768 && block_number <= 831 ) lead_addr = ( 768 * 32 ) + 0x00100000;
	if ( block_number >= 832 && block_number <= 895 ) lead_addr = ( 832 * 32 ) + 0x00100000;
	if ( block_number >= 896 && block_number <= 959 ) lead_addr = ( 896 * 32 ) + 0x00100000;
	if ( block_number >= 960 && block_number <= 1023 ) lead_addr = ( 960 * 32 ) + 0x00100000;
	// オフセット量算出
	offset = addr - lead_addr;
	
	// ブランクチェック許可
	FLASH.FMODR.BIT.FRDMD = 1;
	
	// チェックサイズ設定( 2B )
	FLASH.DFLBCCNT.BIT.BCSIZE = 0;
	FLASH.DFLBCCNT.BIT.BCADR = offset;
	
	// ブランクチェックコマンド発行
	fcuCommandByte( block_number, 0x71, 0 );
	fcuCommandByte( block_number, 0xd0, addr_offset );
	
	// P/E処理中か確認
	if ( checkFRDY( 6 * 1.1 ) == 0 ) {
		// エラー確認
		if ( checkErrorFlash() == 0 ) {
			// ブランクチェック結果取得
			if ( FLASH.DFLBCSTAT.BIT.BCST == 1 ) {
				//printf("アドレス%pは書き込み済み\n", addr);
				ret = 1;
			} else {
				//printf("アドレス%pはイレーズ済み\n", addr);
				ret = 0;
			}
		} else {
			ret = 2;
		}
	} else {
		ret = 2;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////
// モジュール名 eraseE2DataFlash						//
// 処理概要     指定データブロックをイレーズ					//
// 引数         block_number: イレーズするブロック番号　			//
// 戻り値      	0:イレーズ完了 1:イレーズエラー					//
//////////////////////////////////////////////////////////////////////////////////
char eraseE2DataFlash ( unsigned short block_number )
{
	volatile char ret = 0;
	
	// P/Eモード移行
	changeFlashPE();
	
	// 消去プロテクト解除
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	// イレーズ許可
	FLASH.DFLWE0.WORD = 0x1eff;	// 0000～0511ブロックまでP/E許可
	FLASH.DFLWE1.WORD = 0xe1ff;	// 0512～1023ブロックまでP/E許可
	
	// ブロックイレーズコマンド発行
	fcuCommandByte( 0, 0x20, 0 );
	fcuCommandByte( block_number, 0xd0, 0 );
	
	if ( ( FLASH.DFLWE0.WORD & 0x00ff ) == 0x00ff ) {
		// P/E処理中か確認
		if ( checkFRDY( 20 * 63 * 1.1 ) == 0 ) {
			// エラー確認
			if ( checkErrorFlash() == 0 ) {
				FLASH.FWEPROR.BIT.FLWE = 2;	// 消去プロテクト
				FLASH.DFLWE0.WORD = 0x1e00;	// 0000～0511ブロックまでP/E許可
				FLASH.DFLWE1.WORD = 0xe100;	// 0512～1023ブロックまでP/E許可
				//printf("Block%dErase\n", block_number);
			}
		}
	} else {
		printf("Block%dUnErase\n", block_number);
		ret = 1;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 checkWriteAddr										//
// 処理概要     startblockからendbloockまでのイレーズ済み領域を探す					//
// 引数         startblock: 開始ブロック番号 	endbloock: 終了ブロック番号				//
//		startoffset: 開始オフセット値	width_data: 確保するデータ量				//
// 戻り値       0: イレーズ済み領域あり 1:イレーズ済み領域なし						//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkWriteAddr ( short startblock, short endblock, char startoffset, short width_data )
{
	volatile char 	addr_offset, lead_addr_offset;
	volatile short	block_number, lead_block_number, i, width = 0;
	
	block_number = startblock;
	addr_offset = startoffset;

	// 指定したデータ量の領域が見つかるまでブランクチェックする
	while ( block_number <= endblock ) {
		if ( checkBlank( block_number, addr_offset ) == 1 ) {
			width = 0;
		} else if ( checkBlank( block_number, addr_offset ) == 0 ) {
			width++;		
			if ( width == 1 ) {
				// 書き込み開始アドレス
				lead_block_number = block_number;
				lead_addr_offset = addr_offset;
			}
			// 必要なデータ量確保できたらストップ
			if ( width >= width_data ) break;
		} else {
			printf("BlankCheakError\n");
			break;
		}
		
		addr_offset += 2;
		if ( addr_offset == 32 ) {
			// 次のブロックに移動する
			addr_offset = 0;
			block_number++;
		}
	}
	// 書き込み領域がなければイレーズする
	if ( block_number > endblock || width < width_data ) {
		//printf("書き込み領域なし\n");
		i = startblock;
		while ( i <= endblock ) {
			eraseE2DataFlash(i);
			i++;
		}
		
		// 開始位置に戻す
		lead_block_number = startblock;
		lead_addr_offset = 0;
	}
	
	NowBlockNumber = lead_block_number;
	NowAddrOffset = lead_addr_offset;
}
//////////////////////////////////////////////////////////////////////////////////
// モジュール名 writeFlash							//
// 処理概要     指定アドレスに書き込み						//
// 引数         write_data:書き込むデータのある配列 　width_data:データの個数	//
// 戻り値       0: 書き込み完了	1: エラーあり					//
//////////////////////////////////////////////////////////////////////////////////
char writeFlash ( short* write_data, short width_data )
{
	volatile char ret = 0;
	volatile short*	sendData;	// 転送用ポインタ
	volatile unsigned int addr, i = 1;
	
	sendData = write_data;
	
	// P/Eモード移行
	changeFlashPE();
	
	// 書き込みプロテクト解除
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	// プログラム許可
	FLASH.DFLWE0.WORD = 0x1eff;	// 0000～0511ブロックまでP/E許可
	FLASH.DFLWE1.WORD = 0xe1ff;	// 0512～1023ブロックまでP/E許可
	
	while ( i <= width_data ) {
		if ( NowBlockNumber <= 1023 ) {
			// プログラムコマンド発行
			fcuCommandByte( NowBlockNumber, 0xe8, 0 );
			fcuCommandByte( NowBlockNumber, 0x01, 0 );
			fcuCommandWord( NowBlockNumber, *sendData, NowAddrOffset );	// 書き込み
			fcuCommandByte( NowBlockNumber, 0xd0, 0 );
			
			// P/E処理中か確認
			if ( checkFRDY( 20 * 1.1 ) == 0 ) {
				// エラー確認
				if ( checkErrorFlash() == 0 ) {
					addr = ( NowBlockNumber * 32 ) + 0x00100000 + NowAddrOffset;	// ブロックNのアドレスを算出
					//printf("Addr%p(%dブロック %dオフセット) %dWritten\n", addr, NowBlockNumber, NowAddrOffset, *sendData);
					
					NowAddrOffset += 2;
					if ( NowAddrOffset == 32 ) {
						NowAddrOffset = 0;
						NowBlockNumber++;
					}
					*sendData++;
					i++;
				} else {
					ret = 1;
					printf("Write Error\n");
					break;
				}
			} else {
				ret = 1;
				printf("Write TimeOut\n");
				break;
			}
		} else {
			ret = 1;
			printf("BlockNumber Error\n");
			break;
		}
	}
	
	FLASH.FWEPROR.BIT.FLWE = 2;	// 書き込みプロテクト
	FLASH.DFLWE0.WORD = 0x1e00;	// 0000～0511ブロックまでP/E禁止
	FLASH.DFLWE1.WORD = 0xe100;	// 0512～1023ブロックまでP/E禁止
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashBlock								//
// 処理概要     指定ブロック番号、指定オフセット値に書かれている値を読む		//
// 引数         block_number:ブロック番号 addr_number: 先頭アドレスからのオフセット値	//
// 戻り値       読み込んだ値								//
//////////////////////////////////////////////////////////////////////////////////////////
short readFlashBlock ( short block_number, char addr_offset )
{
	volatile short ret;
	volatile unsigned int addr;
	
	// フラッシュリードモード移行
	changeFlashRead();
	// E2データフラッシュ領域の読み出しを許可
	FLASH.DFLRE0.WORD = 0x2dff;
	FLASH.DFLRE1.WORD = 0xd2ff;
	addr = ( block_number * 32 ) + 0x00100000 + addr_offset;	// ブロックNのアドレスを算出
	
	ret = *(volatile unsigned short *)addr;
	
	// E2データフラッシュ領域の読み出し禁止
	FLASH.DFLRE0.WORD = 0x2d00;
	FLASH.DFLRE1.WORD = 0xd200;
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashAddr								//
// 処理概要     指定アドレスの値を読む							//
// 引数         addr: E2データフラッシュ領域のアドレス					//
// 戻り値       addrの値								//
//////////////////////////////////////////////////////////////////////////////////////////
short readFlashAddr ( unsigned int addr )
{
	volatile short ret;
	
	// フラッシュリードモード移行
	changeFlashRead();
	// E2データフラッシュ領域の読み出しを許可
	FLASH.DFLRE0.WORD = 0x2dff;
	FLASH.DFLRE1.WORD = 0xd2ff;
	
	ret = *(volatile unsigned short *)addr;
	//printf("ReadData = %d\n", ret );
	// E2データフラッシュ領域の読み出し禁止
	FLASH.DFLRE0.WORD = 0x2d00;
	FLASH.DFLRE1.WORD = 0xd200;
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashArray								//
// 処理概要     指定した領域に保存されているアドレス値から指定された数のデータを読む	//
// 引数         dataarray: 読んだ値を保存する配列 width_data: データの個数		//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashArray ( volatile short* dataArray, short width_data )
{
	volatile short* flashDataBuff2;
	volatile short datasize, i = 0;
	volatile unsigned int EndAddr, DataAddr;	// 前回のパラメータの保存アドレス
	
	flashDataBuff2 = dataArray;
	
	// 最終アドレス算出
	EndAddr = ( BeforeBlockNumber * 32 ) + 0x100000 + BeforeAddrNumber;
	datasize = 2 * ( width_data - 1 );
	
	// 各データのアドレス算出
	while ( i <= datasize ) {
		DataAddr= EndAddr - datasize + i;
		*flashDataBuff2++ = readFlashAddr ( DataAddr );
		i += 2;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashArray2								//
// 処理概要     指定した領域に保存されているアドレス値から指定された数のデータを読む	//
// 引数         dataarray: 読んだ値を保存する配列 width_data: データの個数		//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashArray2 ( volatile short* dataArray, short width_data, short limitArea )
{
	volatile short* flashDataBuff2;
	volatile short datasize, i = 0;
	volatile unsigned int EndAddr, DataAddr, limitAddr;	// 前回のパラメータの保存アドレス
	
	flashDataBuff2 = dataArray;
	
	while ( i <= 44 ) {
		*flashDataBuff2++ = 0;
		i++;
	}
	// 最終アドレス算出
	EndAddr = ( BeforeBlockNumber * 32 ) + 0x100000 + BeforeAddrNumber;
	// 制限アドレス算出
	limitAddr = ( (limitArea + 1) * 32 ) + 0x100000;
	
	DataAddr = EndAddr - limitAddr + 2;		// 書き込まれているデータ数を算出
	
	if ( DataAddr < (2 * width_data) ) {
		datasize = 2 * ( (DataAddr/2) - 1);
	} else {
		datasize = 2 * ( width_data - 1 );
	}
	
	// 各データのアドレス算出
	i = 0;
	flashDataBuff2 = dataArray;
	while ( i <= datasize ) {
		DataAddr= EndAddr - i;
		*flashDataBuff2++ = readFlashAddr ( DataAddr );
		i += 2;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readBeforeAddr								//
// 処理概要     指定ブロック間に記録されているブロック番号、オフセット値を読む		//
// 引数         startblock: 開始ブロック番号 	endbloock: 終了ブロック番号		//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void readBeforeAddr ( short startBlockNumber, short endBlockNumber )
{
	volatile short blocknumber;	// 前回保存時のブロック番号の書いてあるブロック番号
	volatile short addroffset = 0;
	
	// startBlockNumber～endBlockNumberに書いた前回書き込み時の最終アドレスを取得する
	blocknumber = startBlockNumber;
	if ( checkBlank( blocknumber, addroffset ) == 0 ) {
		// イレーズ済みなら初期値にセットする
		BeforeBlockNumber = endBlockNumber + 1;
		BeforeAddrNumber = 0;
	} else {
		while ( checkBlank( blocknumber, addroffset ) == 1 ) {
			addroffset += 2;
			if ( addroffset == 32 ) {
				// 次のブロックに移動する
				addroffset = 0;
				blocknumber++;
				if ( blocknumber == endBlockNumber + 1 ) break;
			}
		}
		
		// ブロックをまたいだ時1つ前のブロックを指定する
		if ( addroffset == 0 ) {
			blocknumber--;
			addroffset = 32;
		}
		// 最終ブロック番号、最終オフセットアドレス取得
		BeforeBlockNumber = readFlashBlock( blocknumber, ( addroffset - 4 ) );
		BeforeAddrNumber = readFlashBlock( blocknumber, ( addroffset - 2 ) );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 writeFlashData								//
// 処理概要     指定ブロック間にflashDataBuffを書き込みアドレスを記録する		//
// 引数          startblock: 開始ブロック番号 	endbloock: 終了ブロック番号		//
//		endData: データ保存最後ブロック	width_data: 確保するデータ量		//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void writeFlashData ( short startBlockNumber, short endBlockNumber, short endData, short width_data )
{
	// 前回保存時のアドレス読み込み
	readBeforeAddr( startBlockNumber, endBlockNumber );
	// 書き込み領域確保
	checkWriteAddr ( BeforeBlockNumber, endData, BeforeAddrNumber, width_data );
	// 書き込み開始
	writeFlash ( flashDataBuff, width_data );

	
	// 最終ブロック番号及びオフセット値記録
	EndAddrOffset = NowAddrOffset - 2;
	EndBlockNumber = NowBlockNumber;
	if ( EndAddrOffset == -2 ) {
		EndAddrOffset = 30;
		EndBlockNumber = NowBlockNumber - 1;
	}
	flashDataBuff[ 0 ] = EndBlockNumber;
	flashDataBuff[ 1 ] = EndAddrOffset;
	
	// 書き込み領域確保
	checkWriteAddr ( startBlockNumber, endBlockNumber, 0, 2 );
	// 書き込み開始
	writeFlash ( flashDataBuff, 2 );
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashSetup								//
// 処理概要     指定アドレスの値を読む							//
// 引数         addr: E2データフラッシュ領域のアドレス					//
// 戻り値       addrの値								//
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashSetup ( void )
{
	short s, s2;
	// フラッシュ読み込み開始
	//printf("前回パラメータ読み込み開始\n");
	
	// パラメータ読み込み
	// 全ブロックイレーズされているか確認する
	if ( checkBlank( 0, 0 ) == 1 ) {
		
		// 前回保存時のアドレス読み込み
		readBeforeAddr( 0, 10 );
		// flashDataBuffにパラメータ読み込み
		readFlashArray( flashDataBuff, NUMDATA );
		
		// 各データ取得
		speed_straight		= flashDataBuff[ 0 ];
		speed_curve_brake	= flashDataBuff[ 1 ];
		speed_curve_r600	= flashDataBuff[ 2 ];
		speed_curve_r450	= flashDataBuff[ 3 ];
		speed_curve_straight	= flashDataBuff[ 4 ];
		
		speed_crossline		= flashDataBuff[ 5 ];
		speed_ckank_trace	= flashDataBuff[ 6 ];
		speed_rightclank_curve	= flashDataBuff[ 7 ];
		speed_rightclank_escape	= flashDataBuff[ 8 ];
		speed_leftclank_curve	= flashDataBuff[ 9 ];
		speed_leftclank_escape	= flashDataBuff[ 10 ];
		
		speed_halfine		= flashDataBuff[ 11 ];
		speed_rightchange_trace = flashDataBuff[ 12 ];
		speed_rightchange_curve	= flashDataBuff[ 13 ];
		speed_rightchange_escape= flashDataBuff[ 14 ];
		speed_leftchange_trace 	= flashDataBuff[ 15 ];
		speed_leftchange_curve	= flashDataBuff[ 16 ];
		speed_leftchange_escape	= flashDataBuff[ 17  ];
		
		speed_slope_brake	= flashDataBuff[ 18 ];
		speed_slope_trace	= flashDataBuff[ 19 ];
				
		angle_rightclank	= flashDataBuff[ 20 ];
		angle_leftclank		= flashDataBuff[ 21 ];
		angle_rightchange	= flashDataBuff[ 22 ];
		angle_leftchange	= flashDataBuff[ 23 ];
	
	} else if ( checkBlank( 0, 0 ) == 0 ) {
		// 全ブロックイレーズされていたら初期値に設定する
		speed_straight		= SPEED_STRAIGHT;
		speed_curve_brake	= SPEED_CURVE_BRAKE;
		speed_curve_r600	= SPEED_CURVE_R600;
		speed_curve_r450	= SPEED_CURVE_R450;
		speed_curve_straight	= SPEED_CURVE_STRAIGHT;
		
		speed_crossline		= SPEED_CROSSLINE;
		speed_ckank_trace	= SPEED_CLANK_TRACE;
		speed_rightclank_curve	= SPEED_RIGHTCLANK_CURVE;
		speed_rightclank_escape	= SPEED_RIGHTCLANK_ESCAPE;
		speed_leftclank_curve	= SPEED_LEFTCLANK_CURVE;
		speed_leftclank_escape	= SPEED_LEFTCLANK_ESCAPE;
		
		speed_halfine		= SPEED_HALFLINE;
		speed_rightchange_trace = SPEED_RIGHTCHANGE_TRACE;
		speed_rightchange_curve	= SPEED_RIGHTCHANGE_CURVE;
		speed_rightchange_escape= SPEED_RIGHTCHANGE_ESCAPE;
		speed_leftchange_trace 	= SPEED_LEFTCHANGE_TRACE;
		speed_leftchange_curve	= SPEED_LEFTCHANGE_CURVE;
		speed_leftchange_escape	= SPEED_LEFTCHANGE_ESCAPE;
		
		speed_slope_brake	= SPEED_SLOPE_BRAKE;
		speed_slope_trace	= SPEED_SLOPE_TRACE;
		
		angle_rightclank	= ANGLE_RIGHTCLANK;
		angle_leftclank		= ANGLE_LEFTCLANK;
		angle_rightchange	= ANGLE_RIGHTCHANGE;
		angle_leftchange	= ANGLE_LEFTCHANGE;
		
		printf("Patameter Initialize\n");
	}
	
	// Angle0読み込み
	// 全ブロックイレーズされているか確認する
	if ( checkBlank( ANGLE0_AREA, 0 ) == 1 ) {
		readBeforeAddr( ANGLE0_AREA, ANGLE0_AREA );	// 前回保存時のアドレス読み込み
		readFlashArray( flashDataBuff, 1 );		// flashDataBuffにAngle0読み込み
		Angle0 = flashDataBuff[ 0 ];			// データ取得
	} else if ( checkBlank( ANGLE0_AREA, 0 ) == 0 ) {
		// 全ブロックイレーズされていたら初期値に設定する
		Angle0 = SERVO_CENTER;
		printf("Angle0 Initialize\n");
	}
	
	// msdWorkAddress読み込み
	// 全ブロックイレーズされているか確認する
	if ( checkBlank( MSD_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( MSD_STARTAREA, MSD_ENDAREA );	// 前回保存時のアドレス読み込み
		readFlashArray2( flashDataBuff, 40, MSD_ENDAREA);// flashDataBuffにmsdWorkAddress読み込み読み込み
		// データ取得
		// 直近のログアドレス10個を取得
		s = 0;
		s2 = 0;
		while ( s < 19 ) {
			msdAddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+1 ] * 0x10000 + 
						(unsigned short)flashDataBuff[ s2 ]);
			msdAddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+3 ] * 0x10000 + 
						(unsigned short)flashDataBuff[ s2+2 ]);
			s2 += 4;
		}
		// 最新のログアドレス
		/*
		printf("msdAddrBuff[0] = %d msdAddrBuff[1] = %d\n", msdAddrBuff[0], msdAddrBuff[1]);
		printf("msdAddrBuff[2] = %d msdAddrBuff[3] = %d\n", msdAddrBuff[2], msdAddrBuff[3]);
		printf("msdAddrBuff[4] = %d msdAddrBuff[5] = %d\n", msdAddrBuff[4], msdAddrBuff[5]);
		printf("msdAddrBuff[6] = %d msdAddrBuff[7] = %d\n", msdAddrBuff[6], msdAddrBuff[7]);
		printf("msdAddrBuff[8] = %d msdAddrBuff[9] = %d\n", msdAddrBuff[8], msdAddrBuff[9]);
		printf("msdAddrBuff[10] = %d msdAddrBuff[11] = %d\n", msdAddrBuff[10], msdAddrBuff[11]);
		printf("msdAddrBuff[12] = %d msdAddrBuff[13] = %d\n", msdAddrBuff[12], msdAddrBuff[13]);
		printf("msdAddrBuff[14] = %d msdAddrBuff[15] = %d\n", msdAddrBuff[14], msdAddrBuff[15]);
		printf("msdAddrBuff[16] = %d msdAddrBuff[17] = %d\n", msdAddrBuff[16], msdAddrBuff[17]);
		printf("msdAddrBuff[18] = %d msdAddrBuff[19] = %d\n", msdAddrBuff[18], msdAddrBuff[19]);
		*/
		
		msdWorkAddress = msdAddrBuff[1];	// 前回開始アドレス
		msdWorkAddress2 = msdAddrBuff[0];	// 前回終了アドレス
	} else if ( checkBlank( MSD_STARTAREA, 0 ) == 0 ) {
		// 全ブロックイレーズされていたら初期値に設定する
		msdWorkAddress = MSD_STARTADDRESS;	// 開始アドレス
		msdWorkAddress2 = MSD_ENDADDRESS;	// 終了アドレス
		printf("msdWorkAddress Initialize\n");
	}
	
	// 白線トレース用PIDゲイン読み込み
	// 全ブロックイレーズされているか確認する
	if ( checkBlank( PID_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( PID_STARTAREA, PID_ENDAREA );	// 前回保存時のアドレス読み込み
		readFlashArray( flashDataBuff, 3 );		// flashDataBuffにPIDゲイン読み込み
		// データ取得
		kp_buff = flashDataBuff[ 0 ];
		ki_buff = flashDataBuff[ 1 ];
		kd_buff = flashDataBuff[ 2 ];
	} else if ( checkBlank( PID_STARTAREA, 0 ) == 0 ) {
		// 全ブロックイレーズされていたら初期値に設定する
		kp_buff = KP;
		ki_buff = KI;
		kd_buff = KD;
		printf("PIDgain Initialize\n");
	}
	
	// 角度制御用PIDゲイン読み込み
	// 全ブロックイレーズされているか確認する
	if ( checkBlank( PID2_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( PID2_STARTAREA, PID2_ENDAREA );	// 前回保存時のアドレス読み込み
		readFlashArray( flashDataBuff, 3 );		// flashDataBuffにPIDゲイン読み込み
		// データ取得
		kp2_buff = flashDataBuff[ 0 ];
		ki2_buff = flashDataBuff[ 1 ];
		kd2_buff = flashDataBuff[ 2 ];
	} else if ( checkBlank( PID2_STARTAREA, 0 ) == 0 ) {
		// 全ブロックイレーズされていたら初期値に設定する
		kp2_buff = KP2;
		ki2_buff = KI2;
		kd2_buff = KD2;
		printf("PID2gain Initialize\n");
	}
	
	// 速度制御用PIDゲイン読み込み
	// 全ブロックイレーズされているか確認する
	if ( checkBlank( PID3_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( PID3_STARTAREA, PID3_ENDAREA );	// 前回保存時のアドレス読み込み
		readFlashArray( flashDataBuff, 3 );		// flashDataBuffにPIDゲイン読み込み
		// データ取得
		kp3_buff = flashDataBuff[ 0 ];
		ki3_buff = flashDataBuff[ 1 ];
		kd3_buff = flashDataBuff[ 2 ];
	} else if ( checkBlank( PID3_STARTAREA, 0 ) == 0 ) {
		// 全ブロックイレーズされていたら初期値に設定する
		kp3_buff = KP3;
		ki3_buff = KI3;
		kd3_buff = KD3;
		printf("PID3gain Initialize\n");
	}
	
	// 停止距離読み込み
	// 全ブロックイレーズされているか確認する
	if ( checkBlank( STOPMETER_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( STOPMETER_STARTAREA, STOPMETER_ENDAREA );	// 前回保存時のアドレス読み込み
		readFlashArray( flashDataBuff, 1 );				// flashDataBuffに停止距離読み込み
		stopping_meter = flashDataBuff[ 0 ];				// データ取得
	} else if ( checkBlank( STOPMETER_STARTAREA, 0 ) == 0 ) {
		// 全ブロックイレーズされていたら初期値に設定する
		stopping_meter = STOPPING_METER;
		printf("StopMeter Initialize\n");
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 writeFlashBeforeStart							//
// 処理概要     指定アドレスの値を読む							//
// 引数         addr: E2データフラッシュ領域のアドレス					//
// 戻り値       addrの値								//
//////////////////////////////////////////////////////////////////////////////////////////
void writeFlashBeforeStart ( void )
{
	// フラッシュ書き込み開始
	// 各データをバッファに保存
	flashDataBuff[ 0 ] = speed_straight;
	flashDataBuff[ 1 ] = speed_curve_brake;
	flashDataBuff[ 2 ] = speed_curve_r600;
	flashDataBuff[ 3 ] = speed_curve_r450;
	flashDataBuff[ 4 ] = speed_curve_straight;
	
	flashDataBuff[ 5 ] = speed_crossline;
	flashDataBuff[ 6 ] = speed_ckank_trace;
	flashDataBuff[ 7 ] = speed_rightclank_curve;
	flashDataBuff[ 8 ] = speed_rightclank_escape;
	flashDataBuff[ 9 ] = speed_leftclank_curve;
	flashDataBuff[ 10 ] = speed_leftclank_escape;
	
	flashDataBuff[ 11 ] = speed_halfine;
	flashDataBuff[ 12 ] = speed_rightchange_trace;
	flashDataBuff[ 13 ] = speed_rightchange_curve;
	flashDataBuff[ 14 ] = speed_rightchange_escape;
	flashDataBuff[ 15 ] = speed_leftchange_trace;
	flashDataBuff[ 16 ] = speed_leftchange_curve;
	flashDataBuff[ 17 ] = speed_leftchange_escape;
	
	flashDataBuff[ 18 ] = speed_slope_brake;
	flashDataBuff[ 19 ] = speed_slope_trace;
	
	flashDataBuff[ 20 ] = angle_rightclank;
	flashDataBuff[ 21 ] = angle_leftclank;
	flashDataBuff[ 22 ] = angle_rightchange;
	flashDataBuff[ 23 ] = angle_leftchange;
	
	writeFlashData( 0, 10, PARAMETER_AREA, NUMDATA );
}