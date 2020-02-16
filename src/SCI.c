//====================================//
// インクルード									//
//====================================//
#include "SCI.h"

//====================================//
// グローバル変数の宣言							//
//====================================//
char		revErr = 0;		// 通信エラー番号
// SCI1関連
char		SCI1_mode;		// 通信方式
char		txt_command[128];	// コマンド格納
char		txt_data[128];		// データ格納
char*	txt;				// 受信データ格納
char		cmmandMode = 0;	// コマンド選択
char		stopWord = 0;		// 0: 停止ワード未受信 1:停止ワード受信
short 	cnt_byte = 0;		// 受信したバイト数
char 		command = 0;		// 0:コマンド受信待ち 1:コマンド入力中 2:コマンド判定中

char		SCI1_Req_mode;	// 0:スタート 1:ストップ 2:データ送受信中
char		SCI1_RW_mode;	// 0:送信 1:受信
char		SCI1_Slaveaddr;	// スレーブアドレス
char		SCI1_NumData;		// 送信データ数
char*	SCI1_DataArry;		// 送信データ配列
char		SCI1_DataBuff[255];	// 送信データバッファ

// SCI12関連
char		SCI12_Req_mode = 0;	// 0:スタート 1:ストップ
char		SCI12_Slaveaddr;		// 送信データ数
char		SCI12_NumData;		// データ数
char		SCI1_NumData2;		// 送信データ数2
char*	SCI12_DataArry;		// データ配列
char*	SCI1_DataArry2;		// 送信データ配列2
char		SCI12_DataBuff[255];	// 送信データバッファ
///////////////////////////////////////////////////////////////////////////
// モジュール名 init_SCI1								//
// 処理概要     SCI1の初期化								//
// 引数         mode: 通信方式 rate:ボーレートをbpsで入力		//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
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
	
	SCI1.SCR.BYTE = 0;			//Set PFC of external pin used
	
	//__set_fintv((unsigned int*)FastInterupt);
	 
	ICU.IER[IER_SCI1_RXI1].BIT.IEN_SCI1_RXI1 = 1;	// RXI??????J?n
	ICU.IPR[VECT_SCI1_RXI1].BIT.IPR = 15;		// RXI?????????
	
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
	SCI1.SEMR.BIT.ABCS = abcs;		// 1?r?b?g?]?????????N???b?N?T?C?N?????@0: 16 1: 8
	
	SCI1.SMR.BIT.STOP = 0;			// 1 stop bit
	SCI1.SMR.BIT.PM = 0;			// none parity
	SCI1.SMR.BIT.CHR = 0;			// 8bit data length
	SCI1.SMR.BIT.CM = 0;			// ??????????
	SCI1.BRR = brr;				// 12: 115200bps 1:750000bps 0:1500000bps
	SCI1.SCR.BIT.RIE = 1;			// RXI??????v??
	SCI1.SCR.BIT.TE = 1;			// Enable TX
	SCI1.SCR.BIT.RE = 1;			// Enable RX
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 init_SCI6								//
// 処理概要     SCI1の初期化								//
// 引数         mode: 通信方式 rate:ボーレートをbpsで入力			//
// 戻り値       なし										//
///////////////////////////////////////////////////////////////////////////
void init_SCI6( char rate )
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
	MSTP(SCI6) = 0;				// Wake up SCI6
	SYSTEM.PRCR.WORD = 0xA500;		// Protect
	
	SCI6.SCR.BYTE = 0;			//Set PFC of external pin used
	
	//__set_fintv((unsigned int*)FastInterupt);
	 
	//ICU.IER[IER_SCI6_RXI1].BIT.IEN_SCI6_RXI1 = 1;	// RXI??????J?n
	//ICU.IPR[VECT_SCI6_RXI1].BIT.IPR = 15;		// RXI?????????
	
	// Set MPC
	PORT3.PMR.BIT.B3 = 1;			// Disable P33: peripheral
	PORT3.PMR.BIT.B2 = 1;			// Disable P32: peripheral
	MPC.PWPR.BIT.B0WI = 0;			// Release protect
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P33PFS.BIT.PSEL = 0x0A;		// Set P33: RXD6
	MPC.P32PFS.BIT.PSEL = 0x0A;		// Set P32: TXD6
	MPC.PWPR.BIT.PFSWE = 0;			// Protect
	MPC.PWPR.BIT.B0WI = 1;
	PORT3.PMR.BIT.B3 = 1;			// P33: peripheral
	PORT2.PMR.BIT.B2 = 1;			// P32: peripheral
	
	// SCI6 Settings
	SCI6.SCR.BIT.CKE = 0;			// Disable CKE
	SCI6.SMR.BIT.CKS = 0;			// PCLK
	SCI6.SMR.BIT.MP = 0;			// Disable multiple processor
	
	SCI6.SIMR1.BIT.IICM = 0;
	SCI6.SPMR.BIT.CKPH = 0;
	SCI6.SPMR.BIT.CKPOL = 0;
	SCI6.SCMR.BIT.SMIF = 0;
	SCI6.SEMR.BIT.ACS0 = 0;
	SCI6.SEMR.BIT.ABCS = abcs;		// 1?r?b?g?]?????????N???b?N?T?C?N?????@0: 16 1: 8
	
	SCI6.SMR.BIT.STOP = 0;			// 1 stop bit
	SCI6.SMR.BIT.PM = 0;			// none parity
	SCI6.SMR.BIT.CHR = 0;			// 8bit data length
	SCI6.SMR.BIT.CM = 0;			// ??????????
	SCI6.BRR = brr;				// 12: 115200bps 1:750000bps 0:1500000bps
	SCI6.SCR.BIT.RIE = 1;			// RXI??????v??
	SCI6.SCR.BIT.TE = 1;			// Enable TX
	SCI6.SCR.BIT.RE = 1;			// Enable RX
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 charput								//
// 処理概要     printfの出力(printfで使用する)					//
// 引数         data:出力する一文字							//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void charput( uint8_t data )
{
	while(SCI6.SSR.BIT.TEND == 0);
	LEDR_ON
	SCI6.TDR = data;
	SCI6.SSR.BIT.TEND = 0;
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