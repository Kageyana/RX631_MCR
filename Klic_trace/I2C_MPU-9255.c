//======================================//
// �C���N���[�h                         //
//======================================//
#include "I2C_MPU-9255.h"
#include "R_PG_RX631_mcr_ver3.0.h"
#include <stdio.h>

//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
short 	xa, ya, za;		// �����x
short 	xg, yg, zg;		// �p�����x
char	IMUset = 0;		// 0:���������s		1:����������

//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUWriteByte								//
// �����T�v     �w�肵�����W�X�^�Ƀf�[�^����������					//
// ����         slaveAddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X data:�������݃f�[�^	//
// �߂�l       0:�@�������ݎ��s 1: �������ݐ���					//
//////////////////////////////////////////////////////////////////////////////////////////
char IMUWriteByte(short slaveAddr, char reg, char data ) {
	uint8_t ret, sendData[2] = { reg, data };
    
	ret = R_PG_SCI_I2CMode_Send_C1(0, slaveAddr, sendData, 2);
    
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadByte						//
// �����T�v     �w�肵�����W�X�^�̃f�[�^��ǂ�				//
// ����         slaveAddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X	//
// �߂�l       �w�背�W�X�^�̃f�[�^ 					//
//////////////////////////////////////////////////////////////////////////
char IMUReadByte(short slaveAddr, char reg ) {
	uint8_t data[1] = { reg }, data2[1];
    
	R_PG_SCI_I2CMode_Send_C1(0, slaveAddr, data, 1);
	R_PG_SCI_I2CMode_Receive_C1(0, slaveAddr, data2, 1);
    
	return data2[0];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadArry											//
// �����T�v     �w�肵�����W�X�^����w��̐������f�[�^��ǂ�							//
// ����         slaveAddr:�X���[�u�A�h���X addr:���W�X�^�̃A�h���X num:�ǂݎ��f�[�^�� dataArry:�f�[�^�̊i�[��	//
// �߂�l       �Ȃ� 												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry(short slaveAddr, char addr, char num, char* dataArry ) {
	uint8_t data[1] = { addr };
    
	data[0] = addr;
	R_PG_SCI_I2CMode_Send_C1(0, slaveAddr, data, 1);
	R_PG_SCI_I2CMode_Receive_C1(0, slaveAddr, (uint8_t*)dataArry, num);
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� init_IMU						//
// �����T�v     IMU�̏��������s��					//
// ����         �Ȃ�							//
// �߂�l       0: ���������s�@1:����������				//
//////////////////////////////////////////////////////////////////////////
char init_IMU (void) {
	char ret, whoami;
	
	whoami = IMUReadByte(MPU9255_ADDRESS, WHO_AM_I);
	if ( whoami == 0x71 || whoami == 0x73 ) {
		IMUWriteByte(MPU9255_ADDRESS, PWR_MGMT_1, 0x00);    // �X���[�v���[�h����
		IMUWriteByte(MPU9255_ADDRESS, INT_PIN_CFG, 0x02);   // �����v���A�b�v������
		IMUWriteByte(MPU9255_ADDRESS, ACCEL_CONFIG, 0x18);  // �����W�}16g�ɕύX
		IMUWriteByte(MPU9255_ADDRESS, GYRO_CONFIG, 0x18);   // �����W�}2000deg/s�ɕύX
		ret = 1;
	} else {
		ret = 0;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUProcess						//
// �����T�v     1ms���ƂɎ��s���Z���T�̒l��ǂݎ��			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ� 							//
//////////////////////////////////////////////////////////////////////////
void IMUProcess (void) {
	char 	axisAccelData[6];	// �����x��8bit�����f�[�^�i�[��
	char 	axisGyroData[6];	// �p�����x��8bit�����f�[�^�i�[��
	
	//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, axisAccelData);	// 3�������x�擾
	IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, axisGyroData);	// 3���p�����x�擾
	
	//8bit�f�[�^��16bit�f�[�^�ɕϊ�
	// �����x
	//xa = (short)((axisAccelData[0] << 8 ) | axisAccelData[1]);
	//ya = (short)((axisAccelData[2] << 8 ) | axisAccelData[3]);
	//za = (short)((axisAccelData[4] << 8 ) | axisAccelData[5]);
	
	// �p���x
	xg = (short)((axisGyroData[0] << 8 ) | axisGyroData[1]);
	yg = (short)((axisGyroData[2] << 8 ) | axisGyroData[3]);
	zg = (short)((axisGyroData[4] << 8 ) | axisGyroData[5]);
}