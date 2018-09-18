//======================================//
// インクルード                         //
//======================================//
#include "SCI.h"
//======================================//
// グローバル変数の宣言                 //
//======================================//
char revErr = 0;
// SCI1関連
char		txt_command[128];
char		txt_data[128];
char		*txt;
const char	*txt_stop = "stop";
const char	*txt_command1 = BUNDRATE;
char		*txt_rate;
char		cmmandMode = 0;
char		cmderr = 0;
char		stopWord = 0;
short 		cnt_byte = 0;
char		commandEnd;
char 		commandEnd = 0;
char 		commandDataSelect = 0;

// SCI12関連
char		SCI12_Req_mode = 0;	// 0:スタート 1:ストップ
char		SCI12_SlaveAddr;	// 送信データ数
char		SCI12_NumData;		// データ数
char*		SCI12_DataArry;		// データ配列

char 		ascii_num[] = {48,49,50,51,52,53,54,55,56,57,97,98,99,100,101,102};


#pragma interrupt Excep_SCI1_RXI1 (vect = VECT_SCI1_RXI1, enable )	// 受信割り込み関数定義

#pragma interrupt Excep_SCI12_TXI12 (vect = VECT_SCI12_TXI12, enable )	// TXI12割り込み関数定義
#pragma interrupt Excep_SCI12_TEI12 (vect = VECT_SCI12_TEI12, enable )	// STI12割り込み関数定義

#pragma interrupt chaek_SCI_Error (vect = VECT_ICU_GROUP12, enable )	// 受信エラー割り込み関数定義
//////////////////////////////////////////////////////////////////////////
// モジュール名 init_SCI1						//
// 処理概要     SCI1の初期化						//
// 引数         rate:ボーレートをbpsで入力				//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void init_SCI1( char rate )
{
	unsigned char brr,abcs;
	
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
	
	ICU.IER[IER_SCI1_RXI1].BIT.IEN_SCI1_RXI1 = 1;	// RXI割り込み開始
	ICU.IPR[VECT_SCI1_RXI1].BIT.IPR = 14;		// RXI割り込み優先度14
	ICU.GEN[GEN_SCI1_ERI1].BIT.EN_SCI1_ERI1 = 1;	// ERI割り込み開始
	ICU.IPR[VECT_ICU_GROUP12].BIT.IPR = 15;		// ERI割り込み優先度15
	
	// Set MPC
	PORT3.PMR.BIT.B0 = 1;			// Disable PB1: peripheral
	PORT2.PMR.BIT.B6 = 1;			// Disable PB0: peripheral
	MPC.PWPR.BIT.B0WI = 0;			// Release protect
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P30PFS.BIT.PSEL = 0x0A;		// Set PB1: TXD1
	MPC.P26PFS.BIT.PSEL = 0x0A;		// Set PB0: RXD1
	MPC.PWPR.BIT.PFSWE = 0;			// Protect
	MPC.PWPR.BIT.B0WI = 1;
	PORT3.PMR.BIT.B0 = 1;			// PB1: peripheral
	PORT2.PMR.BIT.B6 = 1;			// PB0: peripheral
	
	// SCI1 Settings
	SCI1.SCR.BIT.CKE = 0;			// Disable CKE
	SCI1.SMR.BIT.CKS = 0;			// PCLK
	SCI1.SMR.BIT.MP = 0;			// Disable multiple processor
	
	SCI1.SIMR1.BIT.IICM = 0;
	SCI1.SPMR.BIT.CKPH = 0;
	SCI1.SPMR.BIT.CKPOL = 0;
	SCI1.SCMR.BIT.SMIF = 0;
	SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = abcs;		// 1ビット転送期間中のクロックサイクル数　0: 16 1: 8
	
	SCI1.SMR.BIT.STOP = 0;			// 1 stop bit
	SCI1.SMR.BIT.PM = 0;			// none parity
	SCI1.SMR.BIT.CHR = 0;			// 8bit data length
	SCI1.SMR.BIT.CM = 0;			// 調歩同期式
	SCI1.BRR = brr;				// 12: 115200bps 1:750000bps 0:1500000bps
	SCI1.SCR.BIT.RIE = 1;			// RXI割り込み要求
	SCI1.SCR.BIT.TE = 1;			// Enable TX
	SCI1.SCR.BIT.RE = 1;			// Enable RX
	
	txt= txt_data;

}
//////////////////////////////////////////////////////////////////////////
// モジュール名 charput							//
// 処理概要     printfの出力(printfで使用する)				//
// 引数         data:出力する一文字					//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void charput( uint8_t data )
{
	while(SCI1.SSR.BIT.TEND == 0);
	SCI1.TDR = data;
	SCI1.SSR.BIT.TEND = 0;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 charget							//
// 処理概要     scanfの入力(scanfで使用する)				//
// 引数         なし							//
// 戻り値       data:入力した一文字					//
//////////////////////////////////////////////////////////////////////////
char charget(void)
{
	uint8_t data;
	return data;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI1_RXI1						//
// 処理概要     UART受信時に割り込みで実行される			//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void Excep_SCI1_RXI1(void)
{
	char c;
	
	__clrpsw_i();
	
	c = SCI1.RDR;
	if ( c == '$' ) {
		commandDataSelect = 1;	// コマンドモードに移行
		txt = txt_command;
	}
	
	if ( commandDataSelect == 1 ) {	// コマンドモードのとき
		*txt++ = c;
		if ( c == ',' ) {	// コマンドの種類
			if ( strcmp( txt_command, txt_command1) == 0 ) {	// コマンド判定
				txt = txt_command;
				cmmandMode = 1;
			} else {		// 正しいコマンドではない
				cmderr = 1;
			}
		} else if ( c == '\r' ) {	// コマンド入力終了
			commandDataSelect = 0;	// データモードに移行
			txt = txt_data;		// アドレスリセット
			commandEnd = 1;		//コマンドの判定を開始
		}
	} else {		// データモードのとき
		*txt++ = c;
		cnt_byte++;
		if ( cnt_byte == 4 ) {	// 4文字で終了
			cnt_byte = 0;	// データ数リセット
			txt = txt_data;	// アドレスリセット
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 commandSCI1						//
// 処理概要     SCI1で受信したコマンドを処理する			//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void commandSCI1 (void)
{
	short s,i;
	
	if ( commandEnd == 1 ) {	// コマンド終了時に実行
		if ( cmderr == 1 ) {
			printf("commandERROR\n");
			commandEnd = 0;
		} else {
			switch ( cmmandMode ) {
			case 1:
				// ボーレート設定(br)
				for ( s = 0; s < 15; s++ ) {
					i = ascii_num[s];
					if ( txt_command[0] == i ) {
						printf("br=%d\n", s);
						init_SCI1( s );
						break;
					}
				}
				break;
				
			default:
				break;
			}
			commandEnd = 0;
		}
	}
	// 緊急停止
	if ( strcmp( txt_data, txt_stop) == 0 ) {
		stopWord = 1;
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 init_SCI12						//
// 処理概要     SCI12の初期化						//
// 引数         rate:ボーレートをkHzで入力				//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void init_SCI12( void )
{
	
	// SCI12
	SYSTEM.PRCR.WORD = 0xA502;		// Release protect
	MSTP(SCI12) = 0;			// Wake up SCI12
	SYSTEM.PRCR.WORD = 0xA500;		// Protect
	
	SCI12.SCR.BYTE = 0;			// Set PFC of external pin used
	
	IEN( SCI12, TXI12 ) = 1;	// TXI割り込み開始
	IPR( SCI12, TXI12 ) = 13;	// TXI割り込み優先度13
	IEN( SCI12, TEI12 ) = 1;	// TEIE割り込み開始
	IPR( SCI12, TEI12 ) = 12;	// TEIE割り込み優先度12
	
	// Set MPC
	PORTE.PMR.BIT.B1 = 0;			// Disable PE1: peripheral
	PORTE.PMR.BIT.B2 = 0;			// Disable PE2: peripheral
	MPC.PWPR.BIT.B0WI = 0;			// Release protect
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PE1PFS.BIT.PSEL = 0x0C;		// Set PE1: SSDA12
	MPC.PE2PFS.BIT.PSEL = 0x0C;		// Set PE2: SSCL12
	MPC.PWPR.BIT.PFSWE = 0;			// Protect
	MPC.PWPR.BIT.B0WI = 1;
	PORTE.PMR.BIT.B1 = 1;			// PE1: peripheral
	PORTE.PMR.BIT.B2 = 1;			// PE2: peripheral
	
	PORTE.ODR0.BYTE = 0x1c;			// PE1,PE2 Nチャンネルオープンドレイン出力
	
	// SCI12 Settings
	SCI12.SIMR3.BIT.IICSDAS = 3;		// SDA端子をハイインピーダンス
	SCI12.SIMR3.BIT.IICSCLS = 3;		// SCL端子をハイインピーダンス
	
	SCI12.SMR.BYTE = 0x00;			// PCLKクロック
	
	SCI12.SCMR.BIT.SDIR = 1;		// MSBファースト
	SCI12.SCMR.BIT.SINV = 0;		// 送信、受信データをそのまま送受信する
	SCI12.SCMR.BIT.SMIF = 0;		// シリアルコミュニケーションインターフェイス
	
	SCI12.BRR = 9;				// 375kHz
	
	SCI12.SPMR.BYTE = 0;
	SCI12.SEMR.BIT.NFEN = 0;		// ノイズ除去機能無効
	SCI12.SNFR.BIT.NFCS = 2;		// 2分周のクロックをノイズフィルタに使用
	SCI12.SIMR1.BIT.IICM = 1;		// 簡易IICモード
	SCI12.SIMR1.BIT.IICDL = 0;		// 出力遅延なし
	SCI12.SIMR2.BIT.IICINTM = 1;		// 受信割り込み、送信割り込み可能
	SCI12.SIMR2.BIT.IICCSC = 1;		// クロック同期を行う
	SCI12.SIMR2.BIT.IICACKT = 1;		// NACK送信またはACK/NACK受信
	
	SCI12.SPMR.BYTE = 0x00;
	
	SCI12.SCR.BYTE = 0x30;			// Enable TX RX
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 send_SCI12_I2c						//
// 処理概要     SCI12I2cの送信						//
// 引数         なし							//
// 戻り値       data:入力した一文字					//
//////////////////////////////////////////////////////////////////////////
void send_SCI12_I2c( char slaveAddr, char* data, char num )
{
	SCI12_SlaveAddr = slaveAddr;
	SCI12_NumData = num;
	SCI12_DataArry = data;
	
	SCI12_Req_mode = 0;
	
	SCI12.SCR.BIT.TEIE = 1;			// STI割り込み許可
	SCI12.SCR.BIT.TIE = 1;			// TXI割り込み許可
	
	SCI12.SIMR3.BIT.IICSTIF = 0;
	SCI12.SIMR3.BYTE = 0x51;		// スタートコンディション発行
	
	// データは割り込みで送信
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI12_TEI12					//
// 処理概要     開始/ 再開始/ 停止条件生成完了時に割り込みで実行される	//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void Excep_SCI12_TEI12 ( void )
{
	if ( SCI12_Req_mode == 0 ) {
		led_out( 0x01 );
		SCI12.SIMR3.BYTE = 0x00;	// データ送信準備
		SCI12.TDR = SCI12_SlaveAddr;	// スレーブアドレス書き込み
	} else {
		led_out( 0x02 );
		SCI12.SIMR3.BIT.IICSDAS = 3;	// SDA端子をハイインピーダンス
		SCI12.SIMR3.BIT.IICSCLS = 3;	// SCL端子をハイインピーダンス
		SCI12_Req_mode = 0;		// スタートコンディション待ち
		
		SCI12.SCR.BIT.TEIE = 0;		// STI割り込み禁止
		SCI12.SCR.BIT.TIE = 0;		// TXI割り込み禁止
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI12_TXI12					//
// 処理概要     SCI12受信時に割り込みで実行される			//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void Excep_SCI12_TXI12( void )
{
	// データ数確認
	if ( SCI12_NumData == 0 ) {
		led_out( 0x04 );
		SCI12.SIMR3.BIT.IICSTIF = 0;
		SCI12.SIMR3.BYTE = 0x54;	// ストップコンディション発行
	} else {
		if ( SCI12.SISR.BIT.IICACKR == 0 && SCI12_Req_mode == 0 ) {
			// ACK受信
			led_out( 0x10 );
			SCI12_Req_mode = 1;		// ストップコンディション待ち
			SCI12.TDR = *SCI12_DataArry++;	// 送信データ書き込み
			SCI12_NumData--;
		} else if ( SCI12.SISR.BIT.IICACKR == 1 && SCI12_Req_mode == 0 ) {
			// NACK受信
			led_out( 0x08 );
			SCI12.SIMR3.BIT.IICSTIF = 0;
			SCI12_Req_mode = 1;		// ストップコンディション待ち
			SCI12.SIMR3.BYTE = 0x54;	// ストップコンディション発行
		} else {
			SCI12.TDR = *SCI12_DataArry++;	// 送信データ書き込み
			SCI12_NumData--;
		}
	}
	
	
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 chaek_SCI_Error						//
// 処理概要     UART受信エラー時に割り込みで実行される			//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void chaek_SCI_Error( void )
{
	/*
	if ( SCI1.SSR.BIT.ORER ) {		
		// オーバーランエラー処理
		*txt++ = SCI1.RDR;
	}
	
	if ( SCI1.SSR.BIT.FER ) {		
		
		if ( PORT3.PIDR.BIT.B0 ) {	// ブレーク信号検出
			SCI1.SCR.BIT.RE = 1;
		} else {
			// フレーミングエラー処理
			if ( SCI1.SSR.BIT.PER == 1 ) {
				// パリティエラー処理
			}
		}
		// エラーフラグクリア
		SCI1.SSR.BIT.ORER = 0;
		SCI1.SSR.BIT.FER = 0;
		SCI1.SSR.BIT.PER = 0;
	}
	*/
	__clrpsw_i();
	if ( SCI1.SSR.BIT.ORER ) revErr = 1;
	if ( SCI1.SSR.BIT.FER ) revErr = 2;
	if ( SCI1.SSR.BIT.PER ) revErr = 3;
	
}