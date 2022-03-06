///////////////////////////////����///////////////////////////////////////
// 	�������݃A�h���X�L�^�̈�1	0�`10�u���b�N
//	�p�����[�^�ۑ��̈�		    11�`879�u���b�N
//	
//	�������݃A�h���X�L�^�̈�2	1022~1023�u���b�N
//	Angle0�ۑ��̈�			    1019~1021�u���b�N
//	
//	�������݃A�h���X�L�^�̈�3	    997�`1007�u���b�N
//	msdWorkAddress�ۑ��̈�          1008�`1018�u���b�N
//	
//	�������݃A�h���X�L�^�̈�4	    967�`976�u���b�N
//	�����g���[�X�pPID�Q�C���ۑ��̈�	977�`996	�u���b�N
//	
//	�������݃A�h���X�L�^�̈�5	937�`946�u���b�N
//	�p�x����pPID�Q�C���ۑ��̈�	947�`966�u���b�N
//	
//	�������݃A�h���X�L�^�̈�6	907�`916�u���b
//	���x����pPID�Q�C���ۑ��̈�	917�`936�u���b�N
//	
//	�������݃A�h���X�L�^�̈�7	877�`886�u���b�N
//	��~�����ۑ��̈�			887�`906�u���b�N
//////////////////////////////////////////////////////////////////////////
#ifndef E2DATAFLASH_H_
#define E2DATAFLASH_H_
//======================================//
// �C���N���[�h
//======================================//
#include "iodefine.h"
#include "io.h"
#include "mtu.h"
#include "ADconverter.h"
#include "setup.h"
#include "control.h"
#include "microSD.h"
#include <stdio.h>
//======================================//
// �V���{����`
//======================================//
#define NUMDATA		24				// �v�f��
#define DATASIZE		( 2 * NUMDATA ) + 2	// ( 2�o�C�g�@* �v�f�� ) + 2�o�C�g
#define DATA			2				// 1��̏������݃T�C�Y2byte
#define CLOCK			96				// ������g��[MHz]
#define FLASHSTARTADDR	0x00100000		// �t���b�V���J�n�A�h���X

// �L�^�̈�
#define PARAMETER_AREA			879		// �p�����[�^�ۑ��̈�
#define PARAMETER_STARTAREA	    0		// �p�����[�^�������݃A�h���X�L�^�̈�1
#define PARAMETER_ENDAREA		10		// �p�����[�^�������݃A�h���X�L�^�̈�2

#define ANGLE0_DATA		    1023			// Angle0�ۑ��̈�
#define ANGLE0_STARTAREA	1019			// Angle0�������݃A�h���X�L�^�̈�1
#define ANGLE0_ENDAREA		1021			// Angle0�������݃A�h���X�L�^�̈�2

#define MSD_DATA			1018			// msdWorkAddress�ۑ��̈�
#define MSD_STARTAREA		997			// msdWorkAddress�������݃A�h���X�L�^�̈�1
#define MSD_ENDAREA		    1007			// msdWorkAddress�������݃A�h���X�L�^�̈�2

#define PID_DATA			996			// �����g���[�X�pPID�Q�C���ۑ��̈�
#define PID_STARTAREA		967			// �����g���[�X�pPID�Q�C���������݃A�h���X�L�^�̈�1
#define PID_ENDAREA		    976			// �����g���[�X�pPID�Q�C���������݃A�h���X�L�^�̈�2

#define PID2_DATA			966			// �p�x����pPID�Q�C���ۑ��̈�
#define PID2_STARTAREA		937			// �p�x����pPID�Q�C���������݃A�h���X�L�^�̈�1
#define PID2_ENDAREA		946			// �p�x����pPID�Q�C���������݃A�h���X�L�^�̈�2

#define PID3_DATA			936			// ���x����pPID�Q�C���ۑ��̈�
#define PID3_STARTAREA		907			// ���x����pPID�Q�C���������݃A�h���X�L�^�̈�1
#define PID3_ENDAREA		916			// ���x����pPID�Q�C���������݃A�h���X�L�^�̈�2

#define STOPMETER_DATA			906		// ���x����pPID�Q�C���ۑ��̈�
#define STOPMETER_STARTAREA	    877		// ���x����pPID�Q�C���������݃A�h���X�L�^�̈�1
#define STOPMETER_ENDAREA		886		// ���x����pPID�Q�C���������݃A�h���X�L�^�̈�2
//======================================//
// �O���[�o���ϐ��̐錾
//======================================//
// �^�C�}�֘A
extern unsigned short		cnt_flash;	// �t���b�V���p�J�E���g

extern short			flashDataBuff[45];	// �ꎞ�ۑ��o�b�t�@

extern volatile unsigned int 		beforeAddr;	// �O��ۑ����̃u���b�N�ԍ�
//======================================//
// �v���g�^�C�v�錾
//======================================//
void wait_flash ( short waitTime );
void FirmWareCopy ( void );
void fcuCommandByte ( unsigned int Addr, unsigned char command );
void fcuCommandWord ( unsigned int Addr, unsigned short command );
bool checkFRDY ( unsigned short waittime );
bool checkErrorFlash ( void );
bool initFlash ( void );
void changeFlashPE ( void );
void changeFlashRead ( void );
signed char checkBlank ( unsigned int Addr );
bool eraseE2DataFlash ( unsigned short Block_number );
void checkWriteAddr ( unsigned int Addr, short endblock, short width_data, short startBlock );
bool writeFlash ( short* write_data, short width_data );
short readFlashAddr ( unsigned int Addr );
void readFlashArray ( unsigned int Addr, volatile short* dataArray, short width_data );
void readFlashArray2 ( unsigned int Addr, volatile short* dataArray, short width_data, short limitArea );
void readBeforeAddr ( short startBlockNumber, short endBlockNumber );
void writeFlashData ( short startBlockNumber, short endBlockNumber, short endData, short width_data );
void readFlashSetup ( bool speed, bool C_angle, bool msd, bool pid_line, bool pid_angle, bool pid_speed, bool meter);
void writeFlashBeforeStart ( bool speed, bool C_angle, bool pid_line, bool pid_angle, bool pid_speed, bool meter );

#endif // E2DATAFLASH_H_