//======================================//
// �C���N���[�h                         //
//======================================//
#include "I2C_MPU-9255.h"
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
volatile short 	xa, ya, za;		// �����x
volatile short 	xg, yg, zg;		// �p�����x
char	IMUset = 0;		// 0:���������s		1:����������

//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUWriteByte								//
// �����T�v     �w�肵�����W�X�^�Ƀf�[�^����������					//
// ����         slaveAddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X data:�������݃f�[�^	//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte(short slaveAddr, char reg, char data )
{
	uint8_t sendData[2] = { reg, data };
    
	I2C_IMU_COMMAND
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadByte						//
// �����T�v     �w�肵�����W�X�^�̃f�[�^��ǂ�				//
// ����         slaveAddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X	//
// �߂�l       �w�背�W�X�^�̃f�[�^ 					//
//////////////////////////////////////////////////////////////////////////
char IMUReadByte(short slaveAddr, char reg )
{
	uint8_t sendData[1] = { reg }, reciveData[1];
    
	I2C_IMU_DATA
	I2C_IMU_READ
    
	return reciveData[0];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadArry											//
// �����T�v     �w�肵�����W�X�^����w��̐������f�[�^��ǂ�							//
// ����         slaveAddr:�X���[�u�A�h���X addr:���W�X�^�̃A�h���X num:�ǂݎ��f�[�^�� dataArry:�f�[�^�̊i�[��	//
// �߂�l       �Ȃ� 												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry(short slaveAddr, char reg, char num, char* dataArry )
{
	uint8_t sendData[1] = { reg };
    
	I2C_IMU_DATA
	I2C_IMU_ARRY
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� init_IMU						//
// �����T�v     IMU�̏��������s��					//
// ����         �Ȃ�							//
// �߂�l       0: ����������1:���������s				//
//////////////////////////////////////////////////////////////////////////
char init_IMU (void)
{
	char ret  = 0, whoami;
	
	whoami = IMUReadByte(MPU9255_ADDRESS, WHO_AM_I);
	if ( whoami == 0x71 || whoami == 0x73 ) {
		IMUWriteByte(MPU9255_ADDRESS, PWR_MGMT_1, 0x00);    // �X���[�v���[�h����
		IMUWriteByte(MPU9255_ADDRESS, INT_PIN_CFG, 0x02);   // �����v���A�b�v������
		IMUWriteByte(MPU9255_ADDRESS, ACCEL_CONFIG, 0x18);  // �����W�}16g�ɕύX
		IMUWriteByte(MPU9255_ADDRESS, GYRO_CONFIG, 0x18);   // �����W�}2000deg/s�ɕύX
	} else {
		ret = 1;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUProcess						//
// �����T�v     1ms���ƂɎ��s���Z���T�̒l��ǂݎ��			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ� 							//
//////////////////////////////////////////////////////////////////////////
void IMUProcess (void)
{
	//char 	axisAccelData[6];	// �����x��8bit�����f�[�^�i�[��
	char 	axisGyroData[6];	// �p�����x��8bit�����f�[�^�i�[��
	
	//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, axisAccelData);	// 3�������x�擾
	IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, axisGyroData);	// 3���p�����x�擾
	
	//8bit�f�[�^��16bit�f�[�^�ɕϊ�
	// �����x
	/*
	xa = (short)((axisAccelData[0] << 8 ) | axisAccelData[1]);
	ya = (short)((axisAccelData[2] << 8 ) | axisAccelData[3]);
	za = (short)((axisAccelData[4] << 8 ) | axisAccelData[5]);
	*/
	// �p���x
	xg = (short)((axisGyroData[0] << 8 ) | axisGyroData[1]);
	yg = (short)((axisGyroData[2] << 8 ) | axisGyroData[3]);
	zg = (short)((axisGyroData[4] << 8 ) | axisGyroData[5]);

}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� caribrateIMU						//
// �����T�v     1ms���ƂɎ��s���Z���T�̒l��ǂݎ��			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ� 							//
//////////////////////////////////////////////////////////////////////////
void caribrateIMU (void)
{
	uint8_t  msb, lsb;
	char a[6], g[6];
	short xa[10], ya[10], za[10], xg[10], yg[10], zg[10];
	int i, ave[6] = {0,0,0,0,0,0};
	
	for( i = 0; i < 10; i++ ){
		IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, a);	// 3�������x�擾
		IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, g);	// 3���p�����x�擾
		
		// �����x
		xa[i] = (short)((a[0] << 8 ) | a[1]);
		ya[i] = (short)((a[2] << 8 ) | a[3]);
		za[i] = (short)((a[4] << 8 ) | a[5]);
		
		// �p���x
		xg[i] = (short)((g[0] << 8 ) | g[1]);
		yg[i] = (short)((g[2] << 8 ) | g[3]);
		zg[i] = (short)((g[4] << 8 ) | g[5]);
		
		ave[0] += xa[i];
		ave[1] += ya[i];
		ave[2] += za[i];
		ave[3] += xg[i];
		ave[4] += yg[i];
		ave[5] += zg[i];
	}
	
	ave[0] /= 10;
	ave[1] /= 10;
	ave[2] /= 10;
	ave[3] /= 10;
	ave[4] /= 10;
	ave[5] /= 10;
	
	ave[0] = ave[0] * pow(2, 3) / 4;
	ave[1] = ave[1] * pow(2, 3) / 4;
	ave[2] = ave[2] * pow(2, 3) / 4;
	ave[3] = ave[3] * pow(2, 3) / 4;
	ave[4] = ave[4] * pow(2, 3) / 4;
	ave[5] = ave[5] * pow(2, 3) / 4;
	
	for( i = 0; i < 3; i++ ){
		msb = ( ave[i] & 0x0000ff00 ) >> 8;
		lsb = (char)ave[i];
		IMUWriteByte(MPU9255_ADDRESS, XG_OFFSET_H + (i*2), lsb );
		IMUWriteByte(MPU9255_ADDRESS, XG_OFFSET_H + (i*2) + 1, msb );
	}
	for( i = 3; i < 6; i++ ){
		msb = ( ave[i] & 0x0000ff00 ) >> 8;
		lsb = (char)ave[i];
		IMUWriteByte(MPU9255_ADDRESS, XA_OFFSET_H + ((i-3)*2), lsb );
		IMUWriteByte(MPU9255_ADDRESS, XA_OFFSET_H + ((i-3)*2) + 1, msb );
	}
}