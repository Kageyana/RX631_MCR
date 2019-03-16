#ifndef SCI_H_
#define SCI_H_
//======================================//
// �C���N���[�h                         //
//======================================//
#include "iodefine.h"
#include <machine.h>
#include <stdio.h>
#include <string.h>
//#include "vect.h"
#include "I2C_MPU-9255.h"
#include "PeripheralFunctions.h"
//======================================//
// �V���{����`                         //
//======================================//
#define	RATE_9600	0x0
#define	RATE_14400	0x1
#define	RATE_19200	0x2
#define	RATE_38400	0x3
#define	RATE_57600	0x4
#define	RATE_115200	0x5
#define	RATE_230400	0x6
#define	RATE_500000	0x7
#define	RATE_750000	0x8
#define	RATE_1000000	0x9
#define	RATE_1500000	0xa
#define	RATE_3000000	0xb

#define	COMMAND1	"$br,"		// �{�[���[�g�ݒ�
#define	STOPWORD	"stop"

// ���荞�ݗD��x
#define	IPR_RXI1		14
#define	IPR_TXI1		13
#define	IPR_TEI1		12

#define	IPR_RXI5		11
#define	IPR_TXI5		10
#define	IPR_TEI5		9

#define	IPR_RXI12		9
#define	IPR_TXI12		8
#define	IPR_TEI12		7

#define	IPR_RXI11		14
#define	IPR_TXI11		13
#define	IPR_TEI11		12

#define	UART			1
#define	I2C			2
#define	SPI			3
#define	RW_BIT		1

#define	SET_SCI_C1	init_SCI1( I2C, 0);
#define	PRINT_ON		1
#define	PRINT_OFF	0
//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//
extern char 		revErr;

// SCI1�֘A
extern char	SCI1_mode;		// �ʐM����
extern char 	txt_command[128];	// �R�}���h�i�[
extern char 	txt_data[128];		// �f�[�^�i�[
extern char	*txt;				// ��M�f�[�^�i�[�p�|�C���^
extern char	cmmandMode;		// �R�}���h�I��
extern char	stopWord;			// 0: ��~���[�h����M 1:��~���[�h��M
extern short 	cnt_byte;			// ��M�����o�C�g��
extern char	command;			// 0:�R�}���h��M�҂� 1:�R�}���h���͒� 2:�R�}���h���蒆

extern char	SCI1_Req_mode;	// 0:�X�^�[�g 1:�X�g�b�v 2;���X�^�[�g 3:�f�[�^����M��
extern char	SCI1_RW_mode;	// 0:��M 1:���M 2:���W�X�^�ǂݍ���
extern char	SCI1_Slaveaddr;	// �X���[�u�A�h���X
extern char	SCI1_NumData;		// ���M�f�[�^��
extern char	SCI1_NumData2;	// ���M�f�[�^��2
extern char*	SCI1_DataArry;		// ���M�f�[�^�z��
extern char*	SCI1_DataArry2;	// ���M�f�[�^�z��2
extern char	SCI1_DataBuff[255];	// ���M�f�[�^�o�b�t�@

// SCI12�֘A
extern char	SCI12_Req_mode;		// 0:�X�^�[�g 1:�X�g�b�v 2;���X�^�[�g 3:�f�[�^����M��
extern char	SCI12_Slaveaddr;		// �X���[�u�A�h���X
extern char	SCI12_NumData;		// ���M�f�[�^��
extern char*	SCI12_DataArry;		// ���M�f�[�^�z��
extern char	SCI12_DataBuff[255];	// ���M�f�[�^�o�b�t�@

extern char	ascii_num[];
//====================================//
// �v���g�^�C�v�錾								//
//====================================//
void init_SCI1( char mode, char rate );
void Excep_SCI1_RXI1( void );
void Excep_SCI1_TXI1( void );
void Excep_SCI1_TEI1( void );
void commandSCI1 (void);
void send_SCI1_I2c( char slaveaddr, char* data, char num );
char send_SCI1_I2cWait( char slaveaddr, char* data, char num );
void receive_SCI1_I2c( char slaveaddr, char* data, char num );
bool receive_data_SCI1_I2c( char slaveaddr, char* sendData, char* receiveData, char num );

void init_SCI12( void );
void send_SCI12_I2c( char slaveaddr, char* data, char num );
void Excep_SCI12_TXI12( void );
void Excep_SCI12_TEI12( void );

void chaek_SCI_Error( void );

#endif /* SCI_H_ */