///////////////////////////////����///////////////////////////////////////
// 	�������݃A�h���X�L�^�̈�1	0�`10�u���b�N			//
//	�p�����[�^�ۑ��̈�		11�`879�u���b�N			//
//									//
//	�������݃A�h���X�L�^�̈�2	1022�u���b�N			//
//	Angle0�ۑ��̈�			1023�u���b�N			//
//									//
//	�������݃A�h���X�L�^�̈�3	1000�`1010�u���b�N		//
//	msdWorkAddress�ۑ��̈�		1011�`1021�u���b�N		//
//									//
//	�������݃A�h���X�L�^�̈�4	970�`979�u���b�N		//
//	�����g���[�X�pPID�Q�C���ۑ��̈�	980�`999�u���b�N		//
//									//
//	�������݃A�h���X�L�^�̈�5	940�`949�u���b�N		//
//	�p�x����pPID�Q�C���ۑ��̈�	950�`969�u���b�N		//
//									//
//	�������݃A�h���X�L�^�̈�6	910�`919�u���b�N		//
//	���x����pPID�Q�C���ۑ��̈�	920�`939�u���b�N		//
//									//
//	�������݃A�h���X�L�^�̈�7	880�`889�u���b�N		//
//	��~�����ۑ��̈�		890�`909�u���b�N		//
//////////////////////////////////////////////////////////////////////////
#ifndef E2DATAFLASH_H_
#define E2DATAFLASH_H_
//======================================//
// �C���N���[�h                         //
//======================================//
#include "iodefine.h"
#include "E2dataFlash.h"
#include "PeripheralFunctions.h"
#include "SetUp.h"
#include "LineChase.h"
#include "MicroSD.h"
#include <stdio.h>
//======================================//
// �V���{����`                         //
//======================================//
#define NUMDATA		24			// �v�f��
#define DATASIZE	( 2 * NUMDATA ) + 2	// ( 2�o�C�g�@* �v�f�� ) + 2�o�C�g

#define PARAMETER_AREA	879			// �p�����[�^�ۑ��̈�

#define ANGLE0_DATA	1023			// Angle0�ۑ��̈�
#define ANGLE0_AREA	1022			// Angle0�������݃A�h���X�L�^�̈�

#define MSD_DATA	1021			// msdWorkAddress�ۑ��̈�
#define MSD_STARTAREA	1000			// msdWorkAddress�������݃A�h���X�L�^�̈�1
#define MSD_ENDAREA	1010			// msdWorkAddress�������݃A�h���X�L�^�̈�2

#define PID_DATA	999			// �����g���[�X�pPID�Q�C���ۑ��̈�
#define PID_STARTAREA	970			// �����g���[�X�pPID�Q�C���������݃A�h���X�L�^�̈�1
#define PID_ENDAREA	979			// �����g���[�X�pPID�Q�C���������݃A�h���X�L�^�̈�2

#define PID2_DATA	969			// �p�x����pPID�Q�C���ۑ��̈�
#define PID2_STARTAREA	940			// �p�x����pPID�Q�C���������݃A�h���X�L�^�̈�1
#define PID2_ENDAREA	949			// �p�x����pPID�Q�C���������݃A�h���X�L�^�̈�2

#define PID3_DATA	939			// ���x����pPID�Q�C���ۑ��̈�
#define PID3_STARTAREA	910			// ���x����pPID�Q�C���������݃A�h���X�L�^�̈�1
#define PID3_ENDAREA	919			// ���x����pPID�Q�C���������݃A�h���X�L�^�̈�2

#define STOPMETER_DATA		909		// ���x����pPID�Q�C���ۑ��̈�
#define STOPMETER_STARTAREA	880		// ���x����pPID�Q�C���������݃A�h���X�L�^�̈�1
#define STOPMETER_ENDAREA	889		// ���x����pPID�Q�C���������݃A�h���X�L�^�̈�2
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
extern short flashDataBuff[45];			// �ꎞ�ۑ��o�b�t�@
//======================================//
// �v���g�^�C�v�錾                     //
//======================================//
void firmwarecopy ( void );
void fcuCommandByte ( unsigned short block_number, unsigned char command, char addr_number );
void fcuCommandWord ( unsigned short block_number, unsigned short command, char addr_number );
char checkFRDY ( unsigned short waittime );
char checkErrorFlash ( void );
char initFlash ( void );
void changeFlashPE ( void );
void changeFlashRead ( void );
char checkBlank ( short block_number, char addr_offset );
char eraseE2DataFlash ( unsigned short block_number );
void checkWriteAddr ( short startblock, short endblock, char startoffset, short width_data );
char writeFlash ( short* write_data, short width_data );
short readFlashBlock ( short block_number, char addr_number );
short readFlashAddr ( unsigned int addr );
void readFlashArray ( volatile short* array, short width_data );
void readFlashArray2 ( volatile short* dataArray, short width_data, short limitArea );
void readBeforeAddr ( short startBlockNumber, short endBlockNumber );
void writeFlashData ( short startBlockNumber, short endBlockNumber, short endData, short width_data );
void readFlashSetup ( void );
void writeFlashBeforeStart ( void );

#endif // E2DATAFLASH_H_