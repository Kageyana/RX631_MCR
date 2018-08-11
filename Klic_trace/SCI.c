//======================================//
// インクルード                         //
//======================================//
#include "SCI.h"
#include "iodefine.h"
#include <machine.h>
#include "vect.h"
#include "R_PG_RX631_mcr_ver3.0.h"
#include "PeripheralFunctions.h"

//TXD PB1
//RXD PB0
//======================================//
// グローバル変数の宣言                 //
//======================================//
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

char ascii_num[] = {48,49,50,51,52,53,54,55,56,57,97,98,99,100,101,102};
char reverr = 0;
char commandDataSelect = 0;

#pragma interrupt Excep_SCI1_RXI1 (vect = VECT_SCI1_RXI1)	// 受信割り込み関数定義
//#pragma interrupt (Excep_SCI1_RXI1(vect=217))	// 受信エラー割り込み関数定義
//////////////////////////////////////////////////////////////////////////
// モジュール名 init_SCI1						//
// 処理概要     SCI1の初期化						//
// 引数         rate:ボーレートをbpsで入力				//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void init_SCI1( char rate ){
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
	 
	ICU.IER[IER_SCI1_RXI1].BIT.IEN_SCI1_RXI1 = 1;	// RXI割り込み開始
	ICU.IPR[VECT_SCI1_RXI1].BIT.IPR = 15;		// RXI割り込み許可
	
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
	
	//ICU.FIR.BIT.FVCT = VECT_SCI1_RXI1;
	//ICU.FIR.BIT.FIEN = 1;
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
char charget(void){
	uint8_t data;
	return data;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 Excep_SCI1_RXI1						//
// 処理概要     UART受信時に割り込みで実行される			//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void Excep_SCI1_RXI1(void) {
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
		//printf("%c",c);
		cnt_byte++;
		if ( cnt_byte == 4 ) {	// 4文字で終了
			cnt_byte = 0;	// データ数リセット
			txt = txt_data;	// アドレスリセット
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 chaek_SCI1_Error					//
// 処理概要     UART受信エラー時に割り込みで実行される			//
// 引数         なし							//
// 戻り値       なし							//
//////////////////////////////////////////////////////////////////////////
void chaek_SCI1_Error( void ) {
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
}