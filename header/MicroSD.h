#ifndef MICROSD_H_
#define MICROSD_H_
//======================================//
// �C���N���[�h                         //
//======================================//
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "E2dataFlash.h"
#include "I2C_LCD.h"
#include "SCI.h"
#include "I2C_MPU-9255.h"
#include <string.h>
//========================================//
// �V���{����`										//
//=======================================//
#define WRITINGTIME		2		// ���O�擾����(ms)
#define DATA_BYTE			32		// ���ɕۑ�����f�[�^��(byte)
#define RECODTIME			30000	// �L�^����(ms)
#define MSD_STARTADDRESS	0		// �J�n�A�h���X
#define MSD_ENDADDRESS	960000	// �I���A�h���X�@RECODTIME * DATA_BYTE / WRITINGTIME

/******************************************** ���������֐� *****************************************/
#define SET_SCI_C5		R_PG_SCI_Set_C5();		// �V���A��I/O�`���l����ݒ�(SPI)
#define SET_CMT_C2		R_PG_Timer_Set_CMT_U1_C2();	// �R���y�A�}�b�`�^�C�}������(ch2)
#define START_CMT_C2	R_PG_Timer_StartCount_CMT_U1_C2();	// �J�E���g�X�^�[�g(ch2)

// CS�[�q
#define MSD_CS_TERMINAL_HIGH	R_PG_IO_PORT_Write_PA0( 1 );	// HIGHT
#define MSD_CS_TERMINAL_LOW	R_PG_IO_PORT_Write_PA0( 0 );	// LOW

// SPI
#define SPI_SEND			R_PG_SCI_SPIMode_Transfer_C5( data_tr, data_re, 1);

// SD�X�C�b�`
#define GET_SDSWITCH		R_PG_IO_PORT_Read_PA6(&sd_sw)

/****************************************************************************************************/
//========================================//
// �O���[�o���ϐ��̐錾								//
//=======================================//
// �^�C�}�֘A
extern unsigned short		cnt_log;		// ���O�R��m�F�p�J�E���g

// microSD�֘A
extern signed char		msdBuff[ 512 ];        // �ꎞ�ۑ��o�b�t�@
extern short			msdBuffaddress;       // �ꎞ�L�^�o�b�t�@�����A�h���X
extern short			msdFlag;                	// 1:�f�[�^�L�^ 0:�L�^���Ȃ�
extern short			msdTimer;               	// �擾�Ԋu�v�Z�p
extern unsigned int		msdStartaddress;      // �L�^�J�n�A�h���X
extern unsigned int		msdEndaddress;        // �L�^�I���A�h���X
extern unsigned int		msdWorkaddress;      // ��Ɨp�A�h���X
extern unsigned int		msdWorkaddress2;	// ��Ɨp�A�h���X2
extern volatile char		msdlibError;		// �G���[�ԍ�
extern signed char 		*msdBuffPointa;		// RAM�ۑ��o�b�t�@�p�|�C���^

extern volatile short			msdlibCnt;
extern volatile unsigned char	interrupt_msd_send_data;	// ���M�t���O

extern unsigned int 			msdaddrBuff[25];	// MicroSD�J�[�h�̍ŏI�������݃A�h���X�ۑ��p
//========================================//
// �v���g�^�C�v�錾									//
//=======================================//
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
void sendLog (void);
char msdEndLog ( void );
void msd_send_data (void);
void msdgetData ( void ) ;
void send_Char ( char data );
void send_ShortToChar ( short data );
void send_uIntToChar ( unsigned int data );
short CharToShort( unsigned char offsetaddress );
unsigned int CharTouInt( unsigned char offsetaddress );
void msd_sendToPC ( void );

#endif /* MICROSD_H_ */