//======================================//
// インクルード
//======================================//
#include "E2dataFlash.h"
//======================================//
// グローバル変数の宣言
//======================================//
// タイマ関連
unsigned short	cnt_flash;	// フラッシュ用カウント

// フラッシュ関連
volatile unsigned int 		beforeAddr;	// 前回保存時のブロック番号
short 				flashDataBuff[45];	// 一時保存バッファ

volatile unsigned int 		nowAddr;		// 現在の書き込まれているブロック番号
//////////////////////////////////////////////////////////////////////////
// モジュール名 wait_flash
// 処理概要     遅延処理
// 引数         遅延時間(ms)
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////
void wait_flash ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 )/ 16;
	for ( i = 0; i < time; i++) __nop();
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 FirmWareCopy
// 処理概要     FCU RAMへのファームウェアコピー
// 引数         なし
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////
void FirmWareCopy ( void )
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
// モジュール名 fcuCommandByte
// 処理概要     指定アドレスにFCUコマンド発行
// 引数         Block_number: ブロック番号　command:FCUコマンド(16進数)バイトサイズ
//				Addr_number: 先頭アドレスからのオフセット値
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////////////////////
void fcuCommandByte ( unsigned int Addr, unsigned char command )
{
	volatile unsigned char *e2data;
	
	e2data = ( unsigned char *)Addr;	// ポインタにアドレス代入
	*e2data = command;			// 指定アドレスにコマンド発行
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 fcuCommandWord
// 処理概要     指定アドレスにFCUコマンド発行
// 引数         Block_number: ブロック番号　command:FCUコマンド(16進数)ワードサイズ
//				Addr_number: 先頭アドレスからのオフセット値
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////////////////////
void fcuCommandWord ( unsigned int Addr, unsigned short command )
{
	volatile unsigned short *e2data;
	
	e2data = ( unsigned short *)Addr;	// ポインタにアドレス代入
	*e2data = command;			// 指定アドレスにコマンド発行
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 checkFRDY
// 処理概要     P/E処理中か判定する
// 引数         nバイトのブロックに対する消去時間
// 戻り値      	0: エラーなし 1:タイムアウトエラー
//////////////////////////////////////////////////////////////////////////
bool checkFRDY ( unsigned short waittime )
{
	volatile char timeout = 0;
	
	cnt_flash = 0;
	
	while( cnt_flash <= ( waittime + 10 ) ) {
		//printf("FRDY = %d cnt_flash = %d\n", FLASH.FSTATR0.BIT.FRDY, cnt_flash);
		if ( FLASH.FSTATR0.BIT.FRDY == 0 ) {
			if ( cnt_flash >= waittime ) {
				cnt_flash = 0;
				timeout = true;
				printf("FCU初期化\n");
				break;
			} else {
				continue;
			}
		} else {
			timeout = false;
			break;
		}
	}
	
	// FCU初期化
	if ( timeout ) {
		cnt_flash = 0;
		while( cnt_flash <= 5 ) {
			FLASH.FRESETR.BIT.FRESET = 1;
			if ( cnt_flash >= 5 ) {
				FLASH.FRESETR.BIT.FRESET = 0;
				FirmWareCopy();
				printf("FCU初期化完了\n");
				break;
			}
		}
	} else if ( !timeout ){
		//printf("タイムアウトなし\n");
	}
	return timeout;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 checkErrorFlash
// 処理概要     エラー確認
// 引数         なし
// 戻り値       0: エラーなし 1:エラーあり
//////////////////////////////////////////////////////////////////////////
bool checkErrorFlash ( void )
{
	volatile char error;
	
	if ( FLASH.FSTATR0.BIT.ILGLERR == 1 || FLASH.FSTATR0.BIT.ERSERR == 1 || FLASH.FSTATR0.BIT.PRGERR == 1 ) {
		printf("エラー確認 ILGLERR = %d ERSERR = %d PRGERR = %d\n", FLASH.FSTATR0.BIT.ILGLERR, FLASH.FSTATR0.BIT.ERSERR, FLASH.FSTATR0.BIT.PRGERR);
		error = true;
		
		if ( FLASH.FSTATR0.BIT.ILGLERR == 1 ) {
			printf("不正コマンド検出\n");
			printf("ROMAE = %d DFLAE = %d DFLRPE = %d DFLWPE = %d\n", FLASH.FASTAT.BIT.ROMAE, FLASH.FASTAT.BIT.DFLAE, FLASH.FASTAT.BIT.DFLRPE, FLASH.FASTAT.BIT.DFLWPE);
			
			if ( FLASH.FASTAT.BYTE == 0x10 ) {
				printf("FCUコマンドは受け付けてない\n");
			} else {
				FLASH.FASTAT.BYTE = 0x10;
				printf("FCUコマンドを受け付けてる\n");
			}
			fcuCommandByte( FLASHSTARTADDR, 0x50 );
		} else {
			printf("ステータスクリアコマンド発行(ILGLERR = 0)\n");
		}
		fcuCommandByte( FLASHSTARTADDR, 0x50 );
	} else {
		//printf("エラーなし\n");
		error = false;
	}
	
	return error;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 inti_falsh
// 処理概要     E2データフラッシュの初期化
// 引数         なし
// 戻り値       0:エラーなし　1:エラーあり
// 補足			初期状態 P/Eノーマルモード
//////////////////////////////////////////////////////////////////////////
bool initFlash ( void )
{	
	char ret = false;
	
	// FCUファームウェアをコピー
	FirmWareCopy();
	
	// 周辺クロック通知コマンド発行
	// P/Eモードに移行
	if ( ( FLASH.FENTRYR.WORD & 0x00ff ) != 0x0080 ) FLASH.FENTRYR.WORD = 0xaa80;
	
	// E2データフラッシュ領域のP/Eをを許可
	FLASH.DFLWE0.WORD = 0x1eff;
	
	// FCLK(48MHz)を周辺クロック通知レジスタに設定
	FLASH.PCKAR.BIT.PCKA = 48;	
	
	// 周辺クロック通知コマンド発行
	fcuCommandByte( FLASHSTARTADDR, 0xe9 );
	fcuCommandByte( FLASHSTARTADDR, 0x03 );
	fcuCommandWord( FLASHSTARTADDR, 0x0f0f );
	fcuCommandWord( FLASHSTARTADDR, 0x0f0f );
	fcuCommandWord( FLASHSTARTADDR, 0x0f0f );
	fcuCommandByte(FLASHSTARTADDR, 0xd0 );
	
	// P/E処理中か確認
	if ( checkFRDY(1) == 0 ) {
		// エラー確認
		if ( checkErrorFlash() == 0 ) {
			// E2データフラッシュ領域のP/Eをを許可
			FLASH.DFLWE0.WORD = 0x1e00;
		} else {
			printf("E2データフラッシュ初期化エラー\n");
			ret = true;
		}
	} else {
		printf("E2データフラッシュ初期化エラー\n");
		ret = true;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 changeFlashPE
// 処理概要     E2データフラッシュのP/Eモード移行
// 引数         なし
// 戻り値       なし
// 補足		初期状態 P/Eノーマルモード
//////////////////////////////////////////////////////////////////////////
void changeFlashPE ( void )
{
	// P/Eモードに移行
	if ( ( FLASH.FENTRYR.WORD & 0x00ff ) != 0x0080 ) FLASH.FENTRYR.WORD = 0xaa80;
	
	// P/E 可能状態にする
	FLASH.FWEPROR.BYTE = 0x01;
	
	// P/Eモード移行コマンド発行
	fcuCommandByte( FLASHSTARTADDR, 0xff );
	
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
// モジュール名 changeFlashRead
// 処理概要     E2データフラッシュのリードモード移行
// 引数         なし
// 戻り値       なし
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
// モジュール名 checkBlank
// 処理概要     指定アドレスにブランクチェック
// 引数         Block_number: ブランクチェックするブロック番号
//				Addr_offset: 先頭アドレスからのオフセット値
// 戻り値       0:イレーズ済み　1:書き込み済み -1:エラーあり
//////////////////////////////////////////////////////////////////////////////////
signed char checkBlank ( unsigned int Addr )
{
	volatile signed char ret;
	volatile unsigned int Block_number, lead_Addr, offset;

	// P/Eモード移行
	changeFlashPE();
	
	Block_number = ( Addr - FLASHSTARTADDR ) / 32;	// ブランクチェックするアドレスを算出
	// 各ブロックの先頭アドレス算出
	if ( Block_number <= 63 ) lead_Addr = ( 0 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 64 && Block_number <= 127 ) lead_Addr = ( 64 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 128 && Block_number <= 191 ) lead_Addr = ( 128 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 192 && Block_number <= 255 ) lead_Addr = ( 192 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 256 && Block_number <= 319 ) lead_Addr = ( 256 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 320 && Block_number <= 383 ) lead_Addr = ( 320 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 384 && Block_number <= 447 ) lead_Addr = ( 384 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 448 && Block_number <= 511 ) lead_Addr = ( 448 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 512 && Block_number <= 575 ) lead_Addr = ( 512 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 576 && Block_number <= 639 ) lead_Addr = ( 576 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 640 && Block_number <= 703 ) lead_Addr = ( 640 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 704 && Block_number <= 767 ) lead_Addr = ( 704 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 768 && Block_number <= 831 ) lead_Addr = ( 768 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 832 && Block_number <= 895 ) lead_Addr = ( 832 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 896 && Block_number <= 959 ) lead_Addr = ( 896 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 960 && Block_number <= 1023 ) lead_Addr = ( 960 * 32 ) + FLASHSTARTADDR;
	// オフセット量算出
	offset = Addr - lead_Addr;
	
	// ブランクチェック許可
	FLASH.FMODR.BIT.FRDMD = 1;
	
	// チェックサイズ設定( 2B )
	FLASH.DFLBCCNT.BIT.BCSIZE = 0;
	FLASH.DFLBCCNT.BIT.BCADR = offset;
	
	// ブランクチェックコマンド発行
	fcuCommandByte( Addr, 0x71 );
	fcuCommandByte( Addr, 0xd0 );
	
	// P/E処理中か確認
	if ( checkFRDY( 6 * 1.1 ) == 0 ) {
		// エラー確認
		if ( checkErrorFlash() == 0 ) {
			// ブランクチェック結果取得
			if ( FLASH.DFLBCSTAT.BIT.BCST == 1 ) {
				//printf("アドレス%pは書き込み済み\n", Addr);
				ret = 1;
			} else {
				//printf("アドレス%pはイレーズ済み\n", Addr);
				ret = 0;
			}
		} else {
			ret = -1;
		}
	} else {
		ret = -1;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////
// モジュール名 eraseE2DataFlash
// 処理概要     指定データブロックをイレーズ
// 引数         Block_number: イレーズするブロック番号
// 戻り値      	0:イレーズ完了 1:イレーズエラー
//////////////////////////////////////////////////////////////////////////////////
bool eraseE2DataFlash ( unsigned short Block_number )
{
	volatile char ret = false;
	unsigned int Addr;
	
	Addr = ( Block_number * 32 ) + FLASHSTARTADDR;	// ブロックNのアドレスを算出
	
	// P/Eモード移行
	changeFlashPE();
	
	// 消去プロテクト解除
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	// イレーズ許可
	FLASH.DFLWE0.WORD = 0x1eff;	// 0000～0511ブロックまでP/E許可
	FLASH.DFLWE1.WORD = 0xe1ff;	// 0512～1023ブロックまでP/E許可
	
	// ブロックイレーズコマンド発行
	fcuCommandByte( Addr, 0x20 );
	fcuCommandByte( Addr, 0xd0 );
	
	if ( ( FLASH.DFLWE0.WORD & 0x00ff ) == 0x00ff ) {
		// P/E処理中か確認
		if ( checkFRDY( 20 * 63 * 1.1 ) == 0 ) {
			// エラー確認
			if ( checkErrorFlash() == 0 ) {
				FLASH.FWEPROR.BIT.FLWE = 2;	// 消去プロテクト
				FLASH.DFLWE0.WORD = 0x1e00;	// 0000～0511ブロックまでP/E許可
				FLASH.DFLWE1.WORD = 0xe100;	// 0512～1023ブロックまでP/E許可
				//printf("Block%dErase\n", Block_number);
			}
		}
	} else {
		//printf("Block%dUnErase\n", Block_number);
		ret = true;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 checkWriteAddr
// 処理概要     startBlockからendbloockまでのイレーズ済み領域を探す
// 引数         startBlock: 開始ブロック番号 	endbloock: 終了ブロック番号
//				startoffset: 開始オフセット値	width_data: 確保するデータ量
// 戻り値       0: イレーズ済み領域あり 1:イレーズ済み領域なし
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkWriteAddr ( unsigned int Addr, short endblock, short width_data, short startBlock )
{
	volatile short addrOffset, i, width = 0;
	volatile unsigned int	cheakAddr, leadAddr, limitAddr;

	cheakAddr = Addr;
	limitAddr = ( endblock * 32 ) + FLASHSTARTADDR + 30;
	//printf("width_data = %x\n", width_data);
	// 指定したデータ量の領域が見つかるまでブランクチェックする
	while ( cheakAddr <= limitAddr ) {
		if ( checkBlank( cheakAddr ) > 0 ) {
			width = 0;
		} else {
			width++;
			//printf("width = %x\n", width);
			if ( width == 1 ) {
				// 書き込み開始アドレス
				leadAddr = cheakAddr;
			}
			// 必要なデータ量確保できたらストップ
			if ( width >= width_data ) break;
		}
		cheakAddr += 2;
	}
	
	// 書き込み領域がなければイレーズする
	if ( cheakAddr > limitAddr || width < width_data ) {
		//printf("startBlock = %d\n", startBlock);
		i = startBlock;
		while ( i <= endblock ) {
			//printf("i = %d\n", i);
			eraseE2DataFlash( i );
			i++;
		}
		
		// 開始位置に戻す
		leadAddr = ( startBlock * 32 ) + FLASHSTARTADDR;
	}
	//printf("leadAddr = %x\n", leadAddr);
	nowAddr = leadAddr;
}
//////////////////////////////////////////////////////////////////////////////////
// モジュール名 writeFlash
// 処理概要     指定アドレスに書き込み
// 引数         write_data:書き込むデータのある配列 　width_data:データの個数
// 戻り値       0: 書き込み完了	1: エラーあり
//////////////////////////////////////////////////////////////////////////////////
bool writeFlash ( short* write_data, short width_data )
{
	volatile char ret = false;
	volatile short*	sendData;	// 転送用ポインタ
	volatile unsigned int Addr, i = 1;
	
	sendData = write_data;
	
	// P/Eモード移行
	changeFlashPE();
	
	// 書き込みプロテクト解除
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	// プログラム許可
	FLASH.DFLWE0.WORD = 0x1eff;	// 0000～0511ブロックまでP/E許可
	FLASH.DFLWE1.WORD = 0xe1ff;	// 0512～1023ブロックまでP/E許可
	
	while ( i <= width_data ) {
		if ( nowAddr <= ( 1023 * 32 ) + FLASHSTARTADDR + 30 ) {
			// プログラムコマンド発行
			fcuCommandByte( nowAddr, 0xe8 );
			fcuCommandByte( nowAddr, 0x01 );
			fcuCommandWord( nowAddr, *sendData );	// 書き込み
			fcuCommandByte( nowAddr, 0xd0 );
			
			// P/E処理中か確認
			if ( checkFRDY( 20 * 1.1 ) == 0 ) {
				// エラー確認
				if ( checkErrorFlash() == 0 ) {
					//printf("Addr%x %dWritten\n", nowAddr, *sendData);
					
					nowAddr += 2;	// 次のアドレスへ移動
					*sendData++;	// 次の書き込みデータを読み込む
					i++;			// 書き込み回数
				} else {
					ret = true;
					printf("Write Error\n");
					break;
				}
			} else {
				ret = true;
				printf("Write TimeOut\n");
				break;
			}
		} else {
			ret = true;
			printf("Addr Error\n");
			break;
		}
	}
	nowAddr -= 2;	// 最終書き込みアドレスにする
	
	FLASH.FWEPROR.BIT.FLWE = 2;	// 書き込みプロテクト
	FLASH.DFLWE0.WORD = 0x1e00;	// 0000～0511ブロックまでP/E禁止
	FLASH.DFLWE1.WORD = 0xe100;	// 0512～1023ブロックまでP/E禁止
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashAddr
// 処理概要     指定アドレスの値を読む
// 引数         Addr: E2データフラッシュ領域のアドレス
// 戻り値       Addrの値
//////////////////////////////////////////////////////////////////////////////////////////
short readFlashAddr ( unsigned int Addr )
{
	volatile short ret;
	
	// フラッシュリードモード移行
	changeFlashRead();
	// E2データフラッシュ領域の読み出しを許可
	FLASH.DFLRE0.WORD = 0x2dff;
	FLASH.DFLRE1.WORD = 0xd2ff;
	
	ret = *(volatile unsigned short *)Addr;
	//printf("Addr%x ReadData = %d\n", Addr, ret );
	// E2データフラッシュ領域の読み出し禁止
	FLASH.DFLRE0.WORD = 0x2d00;
	FLASH.DFLRE1.WORD = 0xd200;
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashArray
// 処理概要     指定した領域に保存されているアドレス値から指定された数のデータを読む
// 引数         dataarray: 読んだ値を保存する配列 width_data: データの個数
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashArray ( unsigned int Addr, volatile short* dataArray, short width_data )
{
	volatile short* flashDataBuff2;
	volatile short datasize, i = 0;
	volatile unsigned int DataAddr;	// 前回のパラメータの保存アドレス
	
	flashDataBuff2 = dataArray;
	datasize = 2 * ( width_data - 1 );
	
	// 各データのアドレス算出
	while ( i <= datasize ) {
		DataAddr= Addr - datasize + i;		// データの先頭から読み込みたい
		*flashDataBuff2++ = readFlashAddr ( DataAddr );
		i += 2;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashArray2
// 処理概要     指定した領域に保存されているアドレス値から指定された数のデータを読む
// 引数         dataarray: 読んだ値を保存する配列 width_data: データの個数
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashArray2 ( unsigned int Addr, volatile short* dataArray, short width_data, short limitArea )
{
	volatile short* flashDataBuff2;
	volatile short datasize, i = 0;
	volatile unsigned int DataAddr, limitAddr;	// 前回のパラメータの保存アドレス
	
	flashDataBuff2 = dataArray;
	
	while ( i <= 44 ) {		// 入力された配列をクリア
		*flashDataBuff2++ = 0;
		i++;
	}
	// 制限アドレス算出
	limitAddr = ( (limitArea + 1) * 32 ) + 0x100000;
	
	DataAddr = Addr - limitAddr + 2;		// 書き込まれているデータ数を算出
	
	if ( DataAddr < (2 * width_data) ) {
		datasize = 2 * ( (DataAddr/2) - 1);
	} else {
		datasize = 2 * ( width_data - 1 );
	}
	
	// 各データのアドレス算出
	i = 0;
	flashDataBuff2 = dataArray;
	while ( i <= datasize ) {
		DataAddr= Addr - i;
		*flashDataBuff2++ = readFlashAddr ( DataAddr );
		i += 2;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readbeforeAddr
// 処理概要     指定ブロック間に記録されているブロック番号、オフセット値を読む
// 引数         startBlock: 開始ブロック番号 	endbloock: 終了ブロック番号
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////////////////////
void readbeforeAddr ( short startBlockNumber, short endBlockNumber )
{
	volatile unsigned int cheakAddr, limitAddr;
	
	// startBlockNumber～endBlockNumberに書いた前回書き込み時の最終アドレスを取得する
	cheakAddr = ( startBlockNumber * 32 ) + FLASHSTARTADDR;	// 開始ブロックのアドレスを算出
	limitAddr = ( endBlockNumber * 32 ) + FLASHSTARTADDR + 30;	// 開始ブロックのアドレスを算出
	//printf("cheakAddr = %x\n", cheakAddr);
	if ( !checkBlank( cheakAddr )  ) {		// 最初のアドレスがイレーズされているか確認
		// イレーズ済みなら初期値にセットする
		beforeAddr = ( ( endBlockNumber + 1 ) * 32 ) + FLASHSTARTADDR;
	} else {
		cheakAddr += 2;
		while ( checkBlank( cheakAddr ) > 0 ) {
			cheakAddr += 2;
			if ( cheakAddr >= limitAddr ) break;
		}
		cheakAddr -= 2;		// 書き込みアドレス記録領域に戻す
		// 最終ブロック番号、最終オフセットアドレス取得
		beforeAddr = readFlashAddr( cheakAddr ) + FLASHSTARTADDR;
	}
	//printf("beforeAddr = %x\n", beforeAddr);
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 writeFlashData
// 処理概要     指定ブロック間にflashDataBuffを書き込みアドレスを記録する
// 引数			startBlock: 開始ブロック番号 	endbloock: 終了ブロック番号
//				endData: データ保存最後ブロック	width_data: 確保するデータ量
// 戻り値		なし
//////////////////////////////////////////////////////////////////////////////////////////
void writeFlashData ( short startBlockNumber, short endBlockNumber, short endData, short width_data )
{
	// 前回保存時のアドレス読み込み
	readbeforeAddr( startBlockNumber, endBlockNumber );
	// 書き込み領域確保
	checkWriteAddr ( beforeAddr, endData, width_data, endBlockNumber + 1 );
	// 書き込み開始
	writeFlash ( flashDataBuff, width_data );

	
	// 最終アドレス記録
	flashDataBuff[ 0 ] = (short)( nowAddr - FLASHSTARTADDR );
	//printf("nowAddr = %x\n", nowAddr);
	//printf("flashDataBuff[ 0 ] = %x\n", flashDataBuff[ 0 ]);
	// 書き込み領域確保
	checkWriteAddr ( ( startBlockNumber * 32 ) + FLASHSTARTADDR, endBlockNumber, 1, startBlockNumber );
	// 書き込み開始
	writeFlash ( flashDataBuff, 1 );
	
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 readFlashSetup
// 処理概要     指定アドレスの値を読む
// 引数         Addr: E2データフラッシュ領域のアドレス
// 戻り値       Addrの値
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashSetup ( bool speed, bool C_angle, bool msd, bool pid_line, bool pid_angle, bool pid_speed, bool meter)
{
	short s, s2;
	// フラッシュ読み込み開始
	printf("前回パラメータ読み込み開始\n");
	
	// パラメータ読み込み
	if ( speed ) {
		// 全ブロックイレーズされているか確認する
		if ( checkBlank( ( PARAMETER_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			
			// 前回保存時のアドレス読み込み
			readbeforeAddr( PARAMETER_STARTAREA, PARAMETER_ENDAREA );
			// flashDataBuffにパラメータ読み込み
			readFlashArray( beforeAddr, flashDataBuff, NUMDATA );
			
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
		
		} else if ( checkBlank( ( PARAMETER_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// 全ブロックイレーズまたはエラーが発生したら初期値に設定する
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
			
			printf("Parameter Initialize\n");
		}
	}
		
	// Angle0読み込み
	if ( C_angle ) {
		// 全ブロックイレーズされているか確認する
		if ( checkBlank( ( ANGLE0_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( ANGLE0_STARTAREA, ANGLE0_ENDAREA );	// 前回保存時のアドレス読み込み
			readFlashArray( beforeAddr, flashDataBuff, 1 );		// flashDataBuffにAngle0読み込み
			Angle0 = flashDataBuff[ 0 ];			// データ取得
		} else if ( checkBlank( ( ANGLE0_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// 全ブロックイレーズまたはエラーが発生したら初期値に設定する
			Angle0 = SERVO_CENTER;
			printf("Angle0 Initialize\n");
		}
	}
	
	// msdWorkAddress読み込み
	if ( msd ) {
		// 全ブロックイレーズされているか確認する
		if ( checkBlank( ( MSD_STARTAREA *32 ) + FLASHSTARTADDR  ) ) {
			readbeforeAddr( MSD_STARTAREA, MSD_ENDAREA );	// 前回保存時のアドレス読み込み
			readFlashArray2( beforeAddr, flashDataBuff, 40, MSD_ENDAREA);// flashDataBuffにmsdWorkAddress読み込み読み込み
			// データ取得
			// 直近のログアドレス10個を取得
			s = 0;
			s2 = 0;
			while ( s < 19 ) {
				// 終了アドレス
				msdaddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+1 ] * 0x10000 + 
							(unsigned short)flashDataBuff[ s2 ]);
				// 開始アドレス
				msdaddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+3 ] * 0x10000 + 
							(unsigned short)flashDataBuff[ s2+2 ]);
							
				if ( ( msdaddrBuff[s-2] - 1 ) == msdaddrBuff[s-1] ) {
					msdaddrBuff[s-1] = msdaddrBuff[s-2];		// 開始アドレス
					msdaddrBuff[s-2] += MSD_ENDADDRESS;			// 15秒分のアドレス
					s2 += 2;
				} else {
					s2 += 4;
				}
			}
			// 最新のログアドレス
			printf("msdAddrBuff[0] = %d msdAddrBuff[1] = %d\n", msdaddrBuff[0], msdaddrBuff[1]);
			printf("msdaddrBuff[2] = %d msdaddrBuff[3] = %d\n", msdaddrBuff[2], msdaddrBuff[3]);
			printf("msdaddrBuff[4] = %d msdaddrBuff[5] = %d\n", msdaddrBuff[4], msdaddrBuff[5]);
			printf("msdaddrBuff[6] = %d msdaddrBuff[7] = %d\n", msdaddrBuff[6], msdaddrBuff[7]);
			printf("msdaddrBuff[8] = %d msdaddrBuff[9] = %d\n", msdaddrBuff[8], msdaddrBuff[9]);
			printf("msdaddrBuff[10] = %d msdaddrBuff[11] = %d\n", msdaddrBuff[10], msdaddrBuff[11]);
			printf("msdaddrBuff[12] = %d msdaddrBuff[13] = %d\n", msdaddrBuff[12], msdaddrBuff[13]);
			printf("msdaddrBuff[14] = %d msdaddrBuff[15] = %d\n", msdaddrBuff[14], msdaddrBuff[15]);
			printf("msdaddrBuff[16] = %d msdaddrBuff[17] = %d\n", msdaddrBuff[16], msdaddrBuff[17]);
			printf("msdaddrBuff[18] = %d msdaddrBuff[19] = %d\n", msdaddrBuff[18], msdaddrBuff[19]);
			
			msdWorkaddress = msdaddrBuff[1];	// 前回開始アドレス
			msdWorkaddress2 = msdaddrBuff[0];	// 前回終了アドレス
		} else if ( checkBlank( ( MSD_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// 全ブロックイレーズまたはエラーが発生したら初期値に設定する
			msdWorkaddress = MSD_STARTADDRESS;	// 開始アドレス
			msdWorkaddress2 = MSD_ENDADDRESS;	// 終了アドレス
			printf("msdWorkAddress Initialize\n");
		}
	}
	
	// 白線トレース用PIDゲイン読み込み
	if ( pid_line ) {
		// 全ブロックイレーズされているか確認する
		if ( checkBlank( ( PID_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( PID_STARTAREA, PID_ENDAREA );	// 前回保存時のアドレス読み込み
			readFlashArray( beforeAddr, flashDataBuff, 3 );		// flashDataBuffにPIDゲイン読み込み
			// データ取得
			kp_buff = flashDataBuff[ 0 ];
			ki_buff = flashDataBuff[ 1 ];
			kd_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( ( PID_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// 全ブロックイレーズまたはエラーが発生したら初期値に設定する
			kp_buff = KP;
			ki_buff = KI;
			kd_buff = KD;
			printf("PIDgain Initialize\n");
		}
	}
	
	// 角度制御用PIDゲイン読み込み
	if ( pid_angle ) {
		// 全ブロックイレーズされているか確認する
		if ( checkBlank( ( PID2_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( PID2_STARTAREA, PID2_ENDAREA );	// 前回保存時のアドレス読み込み
			readFlashArray( beforeAddr, flashDataBuff, 3 );		// flashDataBuffにPIDゲイン読み込み
			kp2_buff = flashDataBuff[ 0 ];
			ki2_buff = flashDataBuff[ 1 ];
			kd2_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( ( PID2_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// 全ブロックイレーズまたはエラーが発生したら初期値に設定する
			kp2_buff = KP2;
			ki2_buff = KI2;
			kd2_buff = KD2;
			printf("PID2gain Initialize\n");
		}
	}
	
	// 速度制御用PIDゲイン読み込み
	if ( pid_speed ) {
		// 全ブロックイレーズされているか確認する
		if ( checkBlank( ( PID3_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( PID3_STARTAREA, PID3_ENDAREA );	// 前回保存時のアドレス読み込み
			readFlashArray( beforeAddr, flashDataBuff, 3 );		// flashDataBuffにPIDゲイン読み込み
			// データ取得
			kp3_buff = flashDataBuff[ 0 ];
			ki3_buff = flashDataBuff[ 1 ];
			kd3_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( ( PID3_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// 全ブロックイレーズまたはエラーが発生したら初期値に設定する
			kp3_buff = KP3;
			ki3_buff = KI3;
			kd3_buff = KD3;
			printf("PID3gain Initialize\n");
		}
	}
	
	// 停止距離読み込み
	if ( meter ) {
		// 全ブロックイレーズされているか確認する
		if ( checkBlank( ( STOPMETER_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( STOPMETER_STARTAREA, STOPMETER_ENDAREA );	// 前回保存時のアドレス読み込み
			readFlashArray( beforeAddr, flashDataBuff, 1 );				// flashDataBuffに停止距離読み込み
			stopping_meter = flashDataBuff[ 0 ];				// データ取得
		} else if ( checkBlank( ( STOPMETER_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// 全ブロックイレーズまたはエラーが発生したら初期値に設定する
			stopping_meter = STOPPING_METER;
			printf("StopMeter Initialize\n");
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 writeFlashBeforeStart
// 処理概要     指定アドレスの値を読む
// 引数         Addr: E2データフラッシュ領域のアドレス
// 戻り値       Addrの値
//////////////////////////////////////////////////////////////////////////////////////////
void writeFlashBeforeStart ( bool speed, bool C_angle, bool pid_line, bool pid_angle, bool pid_speed, bool meter )
{
	// フラッシュ書き込み開始
	if ( speed ) {
		// 各速度データをバッファに保存
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
		
		writeFlashData( PARAMETER_STARTAREA, PARAMETER_ENDAREA, PARAMETER_AREA, NUMDATA );
	}
	
	if ( C_angle ) {
		// ポテンションメータ0°値(Angle0)保存
		flashDataBuff[0] = Angle0;
		writeFlashData( ANGLE0_STARTAREA, ANGLE0_ENDAREA, ANGLE0_DATA, 1 );
	}
	
	if ( pid_line ) {
		// 白線トレース用PIDゲイン保存
		flashDataBuff[ 0 ] = kp_buff;
		flashDataBuff[ 1 ] = ki_buff;
		flashDataBuff[ 2 ] = kd_buff;
		writeFlashData( PID_STARTAREA, PID_ENDAREA, PID_DATA, 3 );
	}
	
	if ( pid_angle ) {
		// 角度制御用PIDゲイン保存
		flashDataBuff[ 0 ] = kp2_buff;
		flashDataBuff[ 1 ] = ki2_buff;
		flashDataBuff[ 2 ] = kd2_buff;
		writeFlashData( PID2_STARTAREA, PID2_ENDAREA, PID2_DATA, 3 );
	}
	
	if ( pid_speed ) {
		// 速度制御用PIDゲイン保存
		flashDataBuff[ 0 ] = kp3_buff;
		flashDataBuff[ 1 ] = ki3_buff;
		flashDataBuff[ 2 ] = kd3_buff;
		writeFlashData( PID3_STARTAREA, PID3_ENDAREA, PID3_DATA, 3 );
	}
	
	if ( meter ) {
		// 停止距離保存
		flashDataBuff[ 0 ] = stopping_meter;
		writeFlashData( STOPMETER_STARTAREA, STOPMETER_ENDAREA, STOPMETER_DATA, 1 );
	}
}