#ifndef MICROSD_H_
#define MICROSD_H_
//======================================//
// �V���{����`                         //
//======================================//
// CS�[�q
#define MSD_CS_TERMINAL_HIGH	R_PG_IO_PORT_Write_PA0( 1 );
#define MSD_CS_TERMINAL_LOW	R_PG_IO_PORT_Write_PA0( 0 );

#define WRITINGTIME		2		// ���O�擾����(ms)
#define DATA_BYTE		64		// ���ɕۑ�����f�[�^��(byte)
#define RECODTIME		30000		// �L�^����(ms)
#define MSD_STARTADDRESS	0		// �J�n�A�h���X
#define MSD_ENDADDRESS		1920000		// �I���A�h���X�@RECODTIME * DATA_BYTE / WRITINGTIME
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// microSD�֘A
extern signed char		msdBuff[ 512 ];         // �ꎞ�ۑ��o�b�t�@
extern short			msdBuffAddress;         // �ꎞ�L�^�o�b�t�@�����A�h���X
extern short			msdFlag;                // 1:�f�[�^�L�^ 0:�L�^���Ȃ�
extern short			msdTimer;               // �擾�Ԋu�v�Z�p
extern unsigned int		msdStartAddress;        // �L�^�J�n�A�h���X
extern unsigned int		msdEndAddress;          // �L�^�I���A�h���X
extern unsigned int		msdWorkAddress;         // ��Ɨp�A�h���X
extern unsigned int		msdWorkAddress2;	// ��Ɨp�A�h���X2
extern volatile char		msdlibError;		// �G���[�ԍ�
extern signed char 		*msdBuffPointa;		// RAM�ۑ��o�b�t�@�p�|�C���^

extern volatile short		msdlibCnt;
extern volatile unsigned char	interrupt_msd_send_data;	// ���M�t���O

extern unsigned int msdAddrBuff[25];			// MicroSD�J�[�h�̍ŏI�������݃A�h���X�ۑ��p
//======================================//
// �v���g�^�C�v�錾                     //
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