//====================================//
// インクルード									//
//====================================//
#include "SCI.h"
//====================================//
// グローバル変数の宣言							//
//====================================//
char revErr = 0;
// SCI1関連
char	SCI1_mode;		// 通信方式
char	txt_command[128];	// コマンド格納
char	txt_data[128];		// データ格納
char	*txt;			// 受信データ格納
char	cmmandMode = 0;		// コマンド選択
char	stopWord = 0;		// 0: 停止ワード未受信 1:停止ワード受信
short 	cnt_byte = 0;		// 受信したバイト数
char 	command = 0;		// 0:コマンド受信待ち 1:コマンド入力中 2:コマンド判定中

char	SCI1_Req_mode;		// 0:スタート 1:ストップ 2:データ送受信中
char	SCI1_RW_mode;		// 0:送信 1:受信
char	SCI1_SlaveAddr;		// スレーブアドレス
char	SCI1_NumData;		// 送信データ数
char*	SCI1_DataArry;		// 送信データ配列
char	SCI1_DataBuff[255];	// 送信データバッファ

// SCI12関連
char	SCI12_Req_mode = 0;	// 0:スタート 1:ストップ
char	SCI12_SlaveAddr;	// 送信データ数
char	SCI12_NumData;		// データ数
char	SCI1_NumData2;		// 送信データ数2
char*	SCI12_DataArry;		// データ配列
char*	SCI1_DataArry2;		// 送信データ配列2
char	SCI12_DataBuff[255];	// 送信データバッファ

char 	ascii_num[] = {48,49,50,51,52,53,54,55,56,57,97,98,99,100,101,102};


#pragma interrupt Excep_SCI1_RXI1 (vect = VECT_SCI1_RXI1, enable)	// RXI1割り込み関数定義
#pragma interrupt Excep_SCI1_TXI1 (vect = VECT_SCI1_TXI1, enable)	// TXI1割り込み関数定義
#pragma interrupt Excep_SCI1_TEI1 (vect = VECT_SCI1_TEI1, enable)	// TEI1割り込み関数定義

#pragma interrupt Excep_SCI5_RXI5 (vect = VECT_SCI5_RXI5, enable)	// RXI1割り込み関数定義
#pragma interrupt Excep_SCI5_TXI5 (vect = VECT_SCI5_TXI5, enable)	// TXI12割り込み関数定義
#pragma interrupt Excep_SCI5_TEI5 (vect = VECT_SCI5_TEI5, enable)	// STI12割り込み関数定義

#pragma interrupt Excep_SCI12_RXI12 (vect = VECT_SCI12_RXI12, enable)	// RXI1割り込み関数定義
#pragma interrupt Excep_SCI12_TXI12 (vect = VECT_SCI12_TXI12, enable)	// TXI12割り込み関数定義
#pragma interrupt Excep_SCI12_TEI12 (vect = VECT_SCI12_TEI12, enable)	// STI12割り込み関数定義

#pragma interrupt chaek_SCI_Error (vect = VECT_ICU_GROUP12 )	// 受信エラー割り込み関数定義
///////////////////////////////////////////////////////////////////////////
// モジュール名 init_SCI1								//
// 処理概要     SCI1の初期化								//
// 引数         mode: 通信方式 rate:ボーレートをbpsで入力		//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void init_SCI1( char mode, char rate )
{
	unsigned char brr,abcs;
	
	SCI1_mode = mode;
	
	ICU.GEN[GEN_SCI1_ERI1].BIT.EN_SCI1_ERI1 = 1;	// ERI割り込み開始
	ICU.IPR[VECT_ICU_GROUP12].BIT.IPR = 15;		// ERI割り込み優先度15
	
	IPR( SCI1, TXI1 ) = IPR_RXI1;	// RXI割り込み優先度14
	IPR( SCI1, TXI1 ) = IPR_TXI1;	// TXI割り込み優先度13
	IPR( SCI1, TEI1 ) = IPR_TEI1;	// TEIE割り込み優先度12
	
	if ( mode == UART ) {
		// ボーレート選択
		if ( rate == RATE_9600 ) {
			abcs = 0;
			brr = 155;
		} else if ( rate == RATE_14400 ) {
			abcs = 0;
			brr = 103;
		} else if ( rate == RATE_19200 ) {
			abcs = 0;
			brr = 77;
		} else if ( rate == RATE_38400 ) {
			abcs = 0;
			brr = 38;
		} else if ( rate == RATE_57600 ) {
			abcs = 0;
			brr = 25;
		} else if ( rate == RATE_115200 ) {
			abcs = 0;
			brr = 12;
		} else if ( rate == RATE_230400 ) {
			abcs = 1;
			brr = 12;
		} else if ( rate == RATE_500000 ) {
			abcs = 0;
			brr = 2;
		} else if ( rate == RATE_750000 ) {
			abcs = 0;
			brr = 1;
		} else if ( rate == RATE_1000000 ) {
			abcs = 1;
			brr = 2;
		} else if ( rate == RATE_1500000 ) {
			abcs = 0;
			brr = 0;
		} else if ( rate == RATE_3000000 ) {
			abcs = 1;
			brr = 0;
		}
		
		// SCI1
		SYSTEM.PRCR.WORD = 0xA502;		// Release protect
		MSTP(SCI1) = 0;				// Wake up SCI1
		SYSTEM.PRCR.WORD = 0xA500;		// Protect
		
		SCI1.SCR.BYTE = 0;			// Set PFC of external pin used
		
		PORT2.ODR1.BIT.B4 = 0;		// P26 CMOS出力
		PORT3.ODR0.BIT.B0 = 0;		// P30 CMOS出力
		
		IEN( SCI1, RXI1 ) = 1;	// RXI割り込み開始
		IEN( SCI1, TXI1 ) = 0;	// TXI割り込み開始
		IEN( SCI1, TEI1 ) = 0;	// TEIE割り込み開始
		
		// Set MPC
		PORT3.PMR.BIT.B0 = 0;		// Disable PB1: peripheral
		PORT2.PMR.BIT.B6 = 0;		// Disable PB0: peripheral
		MPC.PWPR.BIT.B0WI = 0;		// Release protect
		MPC.PWPR.BIT.PFSWE = 1;
		MPC.P30PFS.BIT.PSEL = 0x0A;	// Set PB1: TXD1
		MPC.P26PFS.BIT.PSEL = 0x0A;	// Set PB0: RXD1
		MPC.PWPR.BIT.PFSWE = 0;		// Protect
		MPC.PWPR.BIT.B0WI = 1;
		PORT3.PMR.BIT.B0 = 1;		// PB1: peripheral
		PORT2.PMR.BIT.B6 = 1;		// PB0: peripheral
		
		// SCI1 Settings
		SCI1.SCR.BIT.CKE = 0;		// Disable CKE
		SCI1.SMR.BIT.CKS = 0;		// PCLK
		SCI1.SMR.BIT.MP = 0;		// マルチプロセッサ通信機能を禁止
		
		SCI1.SIMR1.BIT.IICM = 0;	// シリアルインタフェースモード
		SCI1.SPMR.BYTE = 0;		// クロック遅れ、反転なし
		SCI1.SCMR.BIT.SMIF = 0;		// シリアルコミュニケーションインターフェイスモード
		SCI1.SCMR.BIT.SDIR = 0;		// LSBファースト
		SCI1.SEMR.BIT.ACS0 = 0;		// 外部クロック入力
		SCI1.SEMR.BIT.ABCS = abcs;	// 1ビット転送期間中のクロックサイクル数　0: 16 1: 8
		
		SCI1.SMR.BIT.STOP = 0;		// 1 stop bit
		SCI1.SMR.BIT.PM = 0;		// none parity
		SCI1.SMR.BIT.CHR = 0;		// 8bit data length
		SCI1.SMR.BIT.CM = 0;		// 調歩同期式
		SCI1.BRR = brr;			// 12: 115200bps 1:750000bps 0:1500000bps
		SCI1.SCR.BIT.RIE = 1;		// RXI割り込み要求
		SCI1.SCR.BIT.TE = 1;		// Enable TX
		SCI1.SCR.BIT.RE = 1;		// Enable RX
		
		txt= txt_data;
	} else if ( mode == I2C ) {
		IEN( SCI1, RXI1 ) = 1;	// RXI割り込み開始
		IEN( SCI1, TXI1 ) = 1;	// TXI割り込み開始
		IEN( SCI1, TEI1 ) = 1;	// TEIE割り込み開始
		
		// SCI12
		SYSTEM.PRCR.WORD = 0xA502;	// Release protect
		MSTP(SCI1) = 0;			// Wake up SCI1
		SYSTEM.PRCR.WORD = 0xA500;	// Protect
		
		SCI1.SCR.BYTE = 0;		// Set PFC of external pin used
		
		PORT2.ODR1.BIT.B4 = 1;		// P26 Nチャンネルオープンドレイン出力
		PORT3.ODR0.BIT.B0 = 1;		// P30 Nチャンネルオープンドレイン出力
		
		// Set MPC
		PORT3.PMR.BIT.B0 = 0;			// Disable PB1: peripheral
		PORT2.PMR.BIT.B6 = 0;			// Disable PB0: peripheral
		MPC.PWPR.BIT.B0WI = 0;			// Release protect
		MPC.PWPR.BIT.PFSWE = 1;
		MPC.P30PFS.BIT.PSEL = 0x0A;		// Set PB1: TXD1
		MPC.P26PFS.BIT.PSEL = 0x0A;		// Set PB0: RXD1
		MPC.PWPR.BIT.PFSWE = 0;			// Protect
		MPC.PWPR.BIT.B0WI = 1;
		PORT3.PMR.BIT.B0 = 1;			// PB1: peripheral
		PORT2.PMR.BIT.B6 = 1;			// PB0: peripheral
		
		
		
		// SCI12 Settings
		SCI1.SIMR3.BIT.IICSDAS = 3;	// SDA端子をハイインピーダンス
		SCI1.SIMR3.BIT.IICSCLS = 3;	// SCL端子をハイインピーダンス
		
		SCI1.SMR.BYTE = 0x00;		// PCLKクロック
		
		SCI1.SCMR.BIT.SDIR = 1;		// MSBファースト
		SCI1.SCMR.BIT.SINV = 0;		// 送信、受信データをそのまま送受信する
		SCI1.SCMR.BIT.SMIF = 0;		// シリアルコミュニケーションインターフェイスモード
		
		SCI1.BRR = 3;			// ビットレート 375kHz
		
		SCI1.SPMR.BYTE = 0;
		SCI1.SEMR.BIT.NFEN = 0;		// ノイズ除去機能無効
		SCI1.SNFR.BIT.NFCS = 2;		// 2分周のクロックをノイズフィルタに使用
		SCI1.SIMR1.BIT.IICM = 1;	// 簡易IICモード
		SCI1.SIMR1.BIT.IICDL = 1;	// 0～1サイクル遅延
		SCI1.SIMR2.BIT.IICINTM = 1;	// 受信割り込み、送信割り込み可能
		SCI1.SIMR2.BIT.IICCSC = 1;	// クロック同期を行う
		SCI1.SIMR2.BIT.IICACKT = 1;	// NACK送信またはACK/NACK受信
		
		SCI1.SCR.BYTE = 0x30;		// Enable TX RX
	} else if ( mode == SPI ) {
		
	}

}
///////////////////////////////////////////////////////////////////////////
// モジュール名 charput								//
// 処理概要     printfの出力(printfで使用する)					//
// 引数         data:出力する一文字							//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void charput( uint8_t data )
{
	while(SCI1.SSR.BIT.TEND == 0);
	SCI1.TDR = data;
	SCI1.SSR.BIT.TEND = 0;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 charget								//
// 処理概要     scanfの入力(scanfで使用する)					//
// 引数         なし									//
// 戻り値       data:入力した一文字							//
///////////////////////////////////////////////////////////////////////////
char charget(void)
{
	uint8_t data;
	return data;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI1_RXI1						//
// 処理概要     UART受信時に割り込みで実行される				//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void Excep_SCI1_RXI1(void)
{
	char c;
	
	if ( SCI1_mode == UART ) {
		c = SCI1.RDR;
		if ( c == '$' ) {
			command = 1;	// コマンドモードに移行
			txt = txt_command;
		} else {
			command = 0;	// データモードに移行
			txt = txt_data;
		}
		
		if ( command == 1 ) {	// コマンドモードのとき
			*txt++ = c;		
			if ( c == '\r' ) command = 2;	//コマンドの判定を開始
		} else {		// データモードのとき
			*txt++ = c;
			cnt_byte++;
			if ( cnt_byte == 4 ) {	// 4文字で終了
				cnt_byte = 0;	// データ数リセット
				txt = txt_data;	// アドレスリセット
			}
		}
	} else if ( SCI1_mode == I2C ) {
		*SCI1_DataArry++ = SCI1.RDR;
		SCI1_NumData--;
	} else if ( SCI1_mode == SPI ) {
		
	}
}
/////////////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI1_TEI1								//
// 処理概要     開始/ 再開始/ 停止条件生成完了時に割り込みで実行される	//
// 引数         なし										//
// 戻り値       なし										//
/////////////////////////////////////////////////////////////////////////////////
void Excep_SCI1_TEI1 ( void )
{
	if ( SCI1_mode == UART ) {
		
	} else if ( SCI1_mode == I2C ) {
		if ( SCI1_Req_mode == 0 ) {
			// スタートコンディション
			SCI1.SIMR2.BIT.IICACKT = 1;	// NACK送信またはACK/NACK受信
			SCI1.SIMR3.BYTE = 0x00;		// データ送信準備
			SCI1.TDR = SCI1_SlaveAddr;	// スレーブアドレス書き込み
			SCI1.SSR.BIT.TEND = 0;
		} else if ( SCI1_Req_mode == 1 ) {
			// ストップコンディション
			SCI1.SIMR3.BYTE = 0xf0;	// SDA端子、SCL端子をハイインピーダンス
			SCI1_Req_mode = 0;	// スタートコンディション待ち
			
			PORT5.PODR.BIT.B4 = 0;
			
			SCI1.SCR.BIT.TEIE = 0;	// STI割り込み禁止
			SCI1.SCR.BIT.TIE = 0;	// TXI割り込み禁止
		} else if ( SCI1_Req_mode == 2 ) {
			// リスタートコンディション
			SCI1_DataArry = SCI1_DataArry2;
			SCI1_NumData = SCI1_NumData2;
			
			SCI1_RW_mode = 0;	// 受信モード
			SCI1_Req_mode = 0;	// スタートコンディション待ち
			SCI1.SIMR2.BIT.IICACKT = 1;	// NACK送信またはACK/NACK受信
			SCI1.SIMR3.BYTE = 0x00;		// データ受信準備
			SCI1.TDR = SCI1_SlaveAddr | RW_BIT;	// スレーブアドレス書き込み
			SCI1.SSR.BIT.TEND = 0;
		}
	} else if ( SCI1_mode == SPI ) {
		
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI1_TXI1						//
// 処理概要     ACK/NACK受信時に割り込みで実行される			//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void Excep_SCI1_TXI1( void )
{
	if ( SCI1_mode == UART ) {
		
	} else if ( SCI1_mode == I2C ) {
		// データ数確認
		if ( SCI1_NumData <= 1 ) {
			if ( SCI1_NumData == 0 && SCI1_RW_mode >= 1 ) {
				// 送信モード
				if ( SCI1_RW_mode == 1 ) {
					SCI1_Req_mode = 1;	// ストップコンディション待ち
					SCI1.SIMR3.BYTE = 0x54;	// ストップコンディション発行
				} else {
					SCI1_Req_mode = 2;	// リスタートコンディション待ち
					SCI1.SIMR3.BYTE = 0x52;	// リスタートコンディション発行
				}
			} else if ( SCI1_RW_mode == 0 ) {
				// 受信モード
				if ( SCI1_NumData ) {
					// 残り1
					SCI1.SIMR2.BIT.IICACKT = 1;	// NACK送信またはACK/NACK受信
				} else {
					// 残り0
					SCI1.SCR.BIT.RIE = 0;	// RXI割り込み禁止
					SCI1_Req_mode = 1;	// ストップコンディション待ち
					SCI1.SIMR3.BYTE = 0x54;	// ストップコンディション発行
				}
			}
		}
		if ( SCI1.SISR.BIT.IICACKR == 0 && SCI1_Req_mode == 0 ) {
			// スレーブアドレス送信後にACK受信
			SCI1_Req_mode = 3;	// データ送受信中
			if ( SCI1_RW_mode ) {
				// 送信モード
				SCI1.TDR = *SCI1_DataArry++;	// 送信データ書き込み
				SCI1.SSR.BIT.TEND = 0;
				SCI1_NumData--;		// 送信データ減少   
			} else {
				// 受信モード
				if ( SCI1_NumData >= 2 ) SCI1.SIMR2.BIT.IICACKT = 0;	// ACK送信準備
				SCI1.SCR.BIT.RIE = 1;	// RXI割り込み開始
				SCI1.TDR = 0xFF;	// ダミーデータ書き込み
				SCI1.SSR.BIT.TEND = 0;
			}
		} else if ( SCI1.SISR.BIT.IICACKR == 1 ) {
			// NACK受信
			if ( SCI1_RW_mode ) {
				PORT5.PODR.BIT.B4 = 1;
				SCI1_Req_mode = 1;	// ストップコンディション待ち
				SCI1.SIMR3.BYTE = 0x54;	// ストップコンディション発行
			}
		} else if ( SCI1_Req_mode == 3 ) {
			// ACK受信
			if ( SCI1_RW_mode ) {
				// 送信モード
				SCI1.TDR = *SCI1_DataArry++;	// 送信データ書き込み
				SCI1.SSR.BIT.TEND = 0;
				SCI1_NumData--;		// 送信データ減少
			} else {
				// 受信モード
				SCI1.TDR = 0xFF;	// ダミーデータ書き込み
				SCI1.SSR.BIT.TEND = 0;
			}
		}
	} else if ( SCI1_mode == SPI ) {
		
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 commandSCI1							//
// 処理概要     SCI1で受信したコマンドを処理する				//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void commandSCI1 (void)
{
	short s,i;
	
	if ( command == 2 ) {	// コマンド終了時に実行
		if ( strncmp( txt_command, COMMAND1, 3) == 0 ) {	
			cmmandMode = 1;		// コマンドの種類を判定
		} else {		
			printf("commandERROR\n");
		}
		
		switch ( cmmandMode ) {
		case 1:
			// ボーレート設定(br)
			for ( s = 0; s < 15; s++ ) {
				i = ascii_num[s];
				if ( txt_command[3] == i ) {
					printf("br=%d\n", s);
					init_SCI1( UART, s );
					cmmandMode = 0;
					break;
				}
			}
			break;
			
		default:
			break;
		}
		command = 0;	// コマンド判定終了
	}
	// 緊急停止
	if ( strcmp( txt_data, STOPWORD) == 0 ) {
		stopWord = 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 send_SCI1_I2c												//
// 処理概要     SCI1I2cの送信												//
// 引数         slaveAddr:スレーブアドレス data:送信データの先頭アドレス num: 送信するデータ数	//
// 戻り値       なし														//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void send_SCI1_I2c( char slaveAddr, char* data, char num )
{
	while ( SCI1.SIMR3.BYTE != 0xf0 );	// バスがフリーになるまで待つ
	
	SCI1_RW_mode = 1;	// 送信モード
	memcpy( SCI1_DataBuff, data, num );	// 送信データをバッファに移動
	
	SCI1_SlaveAddr = slaveAddr;
	SCI1_NumData = num;
	SCI1_DataArry = SCI1_DataBuff;
	
	SCI1_Req_mode = 0;
	SCI1.SCR.BIT.TEIE = 1;		// STI割り込み許可
	SCI1.SCR.BIT.TIE = 1;		// TXI割り込み許可
	
	SCI1.SIMR3.BYTE = 0x51;	// スタートコンディション発行
	
	// データは割り込みで送信
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 send_SCI1_I2cWait											//
// 処理概要     SCI1I2cの送信												//
// 引数         slaveAddr:スレーブアドレス data:送信データの先頭アドレス num: 送信するデータ数	//
// 戻り値       0:ACK受信 1: NACK受信 2:デバイスが未接続							//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
char send_SCI1_I2cWait( char slaveAddr, char* data, char num )
{
	char err = 0;
	
	while ( SCI1.SIMR3.BYTE != 0xf0 );	// バスがフリーになるまで待つ
	
	SCI1_RW_mode = 1;	// 送信モード
	memcpy( SCI1_DataBuff, data, num );	// 送信データをバッファに移動
	
	SCI1_SlaveAddr = slaveAddr;
	SCI1_NumData = num;
	SCI1_DataArry = SCI1_DataBuff;
	
	SCI1_Req_mode = 0;		// スタートコンディション待ち
	SCI1.SCR.BIT.TEIE = 1;		// STI割り込み許可
	SCI1.SCR.BIT.TIE = 1;		// TXI割り込み許可
	
	SCI1.SIMR3.BYTE = 0x51;		// スタートコンディション発行
	// データは割り込みで送信
	cnt0 = 0;
	while ( SCI1.SIMR3.BYTE != 0xf0 ) {
		if ( cnt0 >= 1 ) {
			err = 1;
			break;
		}
		
	}
	if ( err == 1 ) return 2;
	else return SCI1.SISR.BIT.IICACKR;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 receive_SCI1_I2c												//
// 処理概要     SCI1I2cの送信												//
// 引数         slaveAddr:スレーブアドレス data:送信データの先頭アドレス num: 送信するデータ数	//
// 戻り値       なし														//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void receive_SCI1_I2c( char slaveAddr, char* data, char num )
{
	while ( SCI1.SIMR3.BYTE != 0xf0 );	// バスがフリーになるまで待つ
	
	SCI1_RW_mode = 0;	// 受信モード
	SCI1_SlaveAddr = slaveAddr | RW_BIT;
	SCI1_NumData = num;
	SCI1_DataArry = data;
	
	SCI1_Req_mode = 0;		// スタートコンディション待ち
	SCI1.SCR.BIT.RIE = 0;		// RXI割り込み禁止
	SCI1.SCR.BIT.TEIE = 1;		// STI割り込み許可
	SCI1.SCR.BIT.TIE = 1;		// TXI割り込み許可
	
	SCI1.SIMR3.BYTE = 0x51;	// スタートコンディション発行
	
	// データは割り込みで送信
	while ( SCI1.SIMR3.BYTE != 0xf0 );	// バスがフリーになるまで待つ
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 receive_data_SCI1_I2c											//
// 処理概要     SCI1I2cの送信												//
// 引数         slaveAddr:スレーブアドレス data:送信データの先頭アドレス num: 送信するデータ数	//
// 戻り値       なし														//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void receive_data_SCI1_I2c( char slaveAddr, char* sendData, char* receiveData, char num )
{
	while ( SCI1.SIMR3.BYTE != 0xf0 );	// バスがフリーになるまで待つ
	
	SCI1_RW_mode = 2;	// レジスタ読み込みモード
	
	memcpy( SCI1_DataBuff, sendData, 1 );	// 送信データをバッファに移動
	SCI1_SlaveAddr = slaveAddr;
	SCI1_NumData = 1;
	SCI1_DataArry = SCI1_DataBuff;
	
	SCI1_NumData2 = num;
	SCI1_DataArry2 = receiveData;
	
	SCI1_Req_mode = 0;		// スタートコンディション待ち
	SCI1.SCR.BIT.RIE = 0;		// RXI割り込み禁止
	SCI1.SCR.BIT.TEIE = 1;		// STI割り込み許可
	SCI1.SCR.BIT.TIE = 1;		// TXI割り込み許可
	
	SCI1.SIMR3.BYTE = 0x51;	// スタートコンディション発行
	
	// データは割り込みで送信
	while ( SCI1.SIMR3.BYTE != 0xf0 );	// バスがフリーになるまで待つ
}




///////////////////////////////////////////////////////////////////////////
// モジュール名 init_SCI12								//
// 処理概要     SCI12の初期化							//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void init_SCI12( void )
{
	IEN( SCI12, TXI12 ) = 1;		// TXI割り込み開始
	IPR( SCI12, TXI12 ) = IPR_TXI12;	// TXI割り込み優先度13
	IEN( SCI12, TEI12 ) = 1;		// TEIE割り込み開始
	IPR( SCI12, TEI12 ) = IPR_TEI12;	// TEIE割り込み優先度12
	
	// SCI12
	SYSTEM.PRCR.WORD = 0xA502;	// Release protect
	MSTP(SCI12) = 0;		// Wake up SCI12
	SYSTEM.PRCR.WORD = 0xA500;	// Protect
	
	SCI12.SCR.BYTE = 0;		// Set PFC of external pin used
	
	PORTE.ODR0.BYTE = 0x14;		// PE1 Nチャンネルオープンドレイン出力
	
	// Set MPC
	PORTE.PMR.BIT.B1 = 0;		// Disable PE1: peripheral
	PORTE.PMR.BIT.B2 = 0;		// Disable PE2: peripheral
	MPC.PWPR.BIT.B0WI = 0;		// Release protect
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PE1PFS.BIT.PSEL = 0x0C;	// Set PE1: SSDA12
	MPC.PE2PFS.BIT.PSEL = 0x0C;	// Set PE2: SSCL12
	MPC.PWPR.BIT.PFSWE = 0;		// Protect
	MPC.PWPR.BIT.B0WI = 1;
	PORTE.PMR.BIT.B1 = 1;		// PE1: peripheral
	PORTE.PMR.BIT.B2 = 1;		// PE2: peripheral
	
	
	
	// SCI12 Settings
	SCI12.SIMR3.BIT.IICSDAS = 3;	// SDA端子をハイインピーダンス
	SCI12.SIMR3.BIT.IICSCLS = 3;	// SCL端子をハイインピーダンス
	
	SCI12.SMR.BYTE = 0x00;		// PCLKクロック
	
	SCI12.SCMR.BIT.SDIR = 1;	// MSBファースト
	SCI12.SCMR.BIT.SINV = 0;	// 送信、受信データをそのまま送受信する
	SCI12.SCMR.BIT.SMIF = 0;	// シリアルコミュニケーションインターフェイス
	
	SCI12.BRR = 3;			// 375kHz
	
	SCI12.SPMR.BYTE = 0;
	SCI12.SEMR.BIT.NFEN = 0;	// ノイズ除去機能無効
	SCI12.SNFR.BIT.NFCS = 2;	// 2分周のクロックをノイズフィルタに使用
	SCI12.SIMR1.BIT.IICM = 1;	// 簡易IICモード
	SCI12.SIMR1.BIT.IICDL = 1;	// 0～1サイクル遅延
	SCI12.SIMR2.BIT.IICINTM = 1;	// 受信割り込み、送信割り込み可能
	SCI12.SIMR2.BIT.IICCSC = 1;	// クロック同期を行う
	SCI12.SIMR2.BIT.IICACKT = 1;	// NACK送信またはACK/NACK受信
	
	SCI12.SCR.BYTE = 0x30;		// Enable TX RX
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 send_SCI12_I2c												//
// 処理概要     SCI12I2cの送信												//
// 引数         slaveAddr:スレーブアドレス data:送信データの先頭アドレス num: 送信するデータ数	//
// 戻り値       なし														//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void send_SCI12_I2c( char slaveAddr, char* data, char num )
{
	while ( SCI12.SIMR3.BYTE != 0xf0 );	// バスがフリーになるまで待つ
	
	memcpy( SCI12_DataBuff, data, num );	// 送信データをバッファに移動
	
	SCI12_SlaveAddr = slaveAddr;
	SCI12_NumData = num;
	SCI12_DataArry = SCI12_DataBuff;
	
	SCI12_Req_mode = 0;
	SCI12.SCR.BIT.TEIE = 1;		// STI割り込み許可
	SCI12.SCR.BIT.TIE = 1;		// TXI割り込み許可
	
	SCI12.SIMR3.BYTE = 0x51;	// スタートコンディション発行
	
	// データは割り込みで送信
}
/////////////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI12_TEI12							//
// 処理概要     開始/ 再開始/ 停止条件生成完了時に割り込みで実行される	//
// 引数         なし										//
// 戻り値       なし										//
/////////////////////////////////////////////////////////////////////////////////
void Excep_SCI12_TEI12 ( void )
{
	if ( SCI12_Req_mode == 0 ) {
		// スタートコンディション
		SCI12.SIMR3.BYTE = 0x00;	// データ送信準備
		SCI12.TDR = SCI12_SlaveAddr;	// スレーブアドレス書き込み
	} else if ( SCI12_Req_mode == 1 ) {
		// ストップコンディション
		SCI12.SIMR3.BYTE = 0xf0;	// SDA端子、SCL端子をハイインピーダンス
		SCI12_Req_mode = 0;		// スタートコンディション待ち
		
		SCI12.SCR.BIT.TEIE = 0;		// STI割り込み禁止
		SCI12.SCR.BIT.TIE = 0;		// TXI割り込み禁止
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI12_TXI12						//
// 処理概要     ACK/NACK受信時に割り込みで実行される			//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void Excep_SCI12_TXI12( void )
{
	// データ数確認
	if ( SCI12_NumData == 0 ) {
		SCI12_Req_mode = 1;		// ストップコンディション待ち
		SCI12.SIMR3.BYTE = 0x54;	// ストップコンディション発行
	} else {
		if ( SCI12.SISR.BIT.IICACKR == 0 && SCI12_Req_mode == 0 ) {
			// ACK受信
			SCI12_Req_mode = 2;		// データ送信
			SCI12.TDR = *SCI12_DataArry++;	// 送信データ書き込み
			SCI12_NumData--;
		} else if ( SCI12.SISR.BIT.IICACKR == 1 && SCI12_Req_mode == 0 ) {
			// NACK受信
			SCI12_Req_mode = 1;		// ストップコンディション待ち
			SCI12.SIMR3.BYTE = 0x54;	// ストップコンディション発行
		} else {
			SCI12.TDR = *SCI12_DataArry++;	// 送信データ書き込み
			SCI12_NumData--;
		}
	}
}



///////////////////////////////////////////////////////////////////////////
// モジュール名 chaek_SCI_Error							//
// 処理概要     UART受信エラー時に割り込みで実行される			//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void chaek_SCI_Error( void )
{
	__clrpsw_i();
	if ( SCI1.SSR.BIT.ORER ) revErr = 1;
	if ( SCI1.SSR.BIT.FER ) revErr = 2;
	if ( SCI1.SSR.BIT.PER ) revErr = 3;
}