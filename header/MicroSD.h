#ifndef MICROSD_H_
#define MICROSD_H_
//======================================//
// シンボル定義                         //
//======================================//
// CS端子
#define MSD_CS_TERMINAL_HIGH	R_PG_IO_PORT_Write_PA0( 1 );
#define MSD_CS_TERMINAL_LOW	R_PG_IO_PORT_Write_PA0( 0 );

#define WRITINGTIME		2		// ログ取得周期(ms)
#define DATA_BYTE		64		// 一回に保存するデータ数(byte)
#define RECODTIME		30000		// 記録時間(ms)
#define MSD_STARTADDRESS	0		// 開始アドレス
#define MSD_ENDADDRESS		1920000		// 終了アドレス　RECODTIME * DATA_BYTE / WRITINGTIME
//======================================//
// グローバル変数の宣言                 //
//======================================//
// microSD関連
extern signed char		msdBuff[ 512 ];         // 一時保存バッファ
extern short			msdBuffAddress;         // 一時記録バッファ書込アドレス
extern short			msdFlag;                // 1:データ記録 0:記録しない
extern short			msdTimer;               // 取得間隔計算用
extern unsigned int		msdStartAddress;        // 記録開始アドレス
extern unsigned int		msdEndAddress;          // 記録終了アドレス
extern unsigned int		msdWorkAddress;         // 作業用アドレス
extern unsigned int		msdWorkAddress2;	// 作業用アドレス2
extern volatile char		msdlibError;		// エラー番号
extern signed char 		*msdBuffPointa;		// RAM保存バッファ用ポインタ

extern volatile short		msdlibCnt;
extern volatile unsigned char	interrupt_msd_send_data;	// 送信フラグ

extern unsigned int msdAddrBuff[25];			// MicroSDカードの最終書き込みアドレス保存用
//======================================//
// プロトタイプ宣言                     //
//======================================//
void msd_write( unsigned char data );
unsigned char msd_read( void );
unsigned char msd_CMD ( unsigned char cmd, unsigned char arg1, unsigned char arg2,
			unsigned char arg3, unsigned char arg4, unsigned char crc );
char init_msd ( void );
void init_log ( void );
char getMicroSD_CSD( volatile unsigned char *p );
char readMicroSD ( unsigned int address, signed char *read );
char writeMicroSD ( unsigned int address, signed char *write );
char eraseMicroSD( unsigned int st_address, unsigned int ed_address );
char setMicroSDdata( signed char *p );
char microSDProcessStart( unsigned int address );
char microSDProcessEnd( void );
void microSDProcess( void );
char checkMicroSDProcess( void );
void msd_send_data (void);
void send_Char ( char data );
void send_CharToShort ( short data );
void send_uIntToChar ( unsigned int data );
short CharToShort( unsigned char offsetAddress );
unsigned int CharTouInt( unsigned char offsetAddress );
void msd_sendToPC ( void );

#endif /* MICROSD_H_ */