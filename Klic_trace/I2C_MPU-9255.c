//======================================//
// �C���N���[�h                         //
//======================================//
#include "I2C_MPU-9255.h"
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
volatile int	ave[6] = {0,0,0,0,0,0};	// �I�t�Z�b�g�l	
volatile short 	rawXa, rawYa, rawZa;	// �����x(16bit�f�[�^)
volatile short 	rawXg, rawYg, rawZg;	// �p�����x(16bit�f�[�^)
volatile short 	rawTemp;		// ���x(16bit�f�[�^)

char		IMUset = 0;		// 0:���������s		1:����������

//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUWriteByte								//
// �����T�v     �w�肵�����W�X�^�Ƀf�[�^����������					//
// ����         slaveAddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X data:�������݃f�[�^	//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte(char slaveAddr, char reg, char data )
{
	char sendData[2] = { reg, data };
    
	I2C_IMU_COMMAND
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadByte						//
// �����T�v     �w�肵�����W�X�^�̃f�[�^��ǂ�				//
// ����         slaveAddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X	//
// �߂�l       �w�背�W�X�^�̃f�[�^ 					//
//////////////////////////////////////////////////////////////////////////
char IMUReadByte(char slaveAddr, char reg )
{
	char sendData[1] = { reg }, reciveData[1];
    
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
void IMUReadArry(char slaveAddr, char reg, char num, char* dataArry )
{
	char sendData[1] = { reg };
    
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
		IMUWriteByte(MPU9255_ADDRESS, PWR_MGMT_1, 0x00);	// �X���[�v���[�h����
		IMUWriteByte(MPU9255_ADDRESS, INT_PIN_CFG, 0x02);	// �����v���A�b�v������
		IMUWriteByte(MPU9255_ADDRESS, CONFIG, 0x00);		// 8Hz���[�p�X�t�B���^
		IMUWriteByte(MPU9255_ADDRESS, ACCEL_CONFIG, 0x18);	// �����W�}16g�ɕύX
		IMUWriteByte(MPU9255_ADDRESS, GYRO_CONFIG, 0x10);	// �����W�}1000deg/s�ɕύX
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
	char 	tempData[2];		// ���x��8bit�����f�[�^�i�[��
	
	//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, axisAccelData);	// 3�������x�擾
	IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, axisGyroData);	// 3���p�����x�擾
	IMUReadArry(MPU9255_ADDRESS, TEMP_OUT_H, 2, tempData);		// ���x�擾
	
	//8bit�f�[�^��16bit�f�[�^�ɕϊ�
	// �����x
	/*
	rawXa = (short)((axisAccelData[0] << 8 & 0xff00 ) | axisAccelData[1]);
	rawYa = (short)((axisAccelData[2] << 8 & 0xff00 ) | axisAccelData[3]);
	rawZa = (short)((axisAccelData[4] << 8 & 0xff00 ) | axisAccelData[5]);
	*/
	// �p���x
	rawXg = (short)((axisGyroData[0] << 8 & 0xff00 ) | axisGyroData[1]);
	rawYg = (short)((axisGyroData[2] << 8 & 0xff00 ) | axisGyroData[3]);
	rawZg = (short)((axisGyroData[4] << 8 & 0xff00 ) | axisGyroData[5]);
	
	// ���x
	rawTemp = (short)((tempData[0] << 8 & 0xff00 ) | tempData[1]);
	
	//xa -= ave[0];
	//ya -= ave[1];
	//za -= ave[2];
	//xg -= ave[3];
	//yg -= ave[4];
	//zg -= ave[5];

}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� caribrateIMU						//
// �����T�v     1ms���ƂɎ��s���Z���T�̒l��ǂݎ��			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ� 							//
//////////////////////////////////////////////////////////////////////////
void caribrateIMU (void)
{
	/*
	uint8_t  msb, lsb;
	char a[6], g[6];
	short xa[10], ya[10], za[10], xg[10], yg[10], zg[10], s[6];
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
	
	s[0] = (int)ave[0] * 8 / 4;
	s[1] = (int)ave[1] * 8 / 4;
	s[2] = (int)ave[2] * 8 / 4;
	s[3] = (int)ave[3] * 8 / 4;
	s[4] = (int)ave[4] * 8 / 4;
	s[5] = (int)ave[5] * 8 / 4;
	
	for( i = 0; i < 3; i++ ){
		msb = s[i] >> 8;
		lsb = s[i] & 0xff;
		IMUWriteByte(MPU9255_ADDRESS, XG_OFFSET_H + (i*2), msb );
		IMUWriteByte(MPU9255_ADDRESS, XG_OFFSET_H + (i*2) + 1, lsb );
	}
	for( i = 3; i < 6; i++ ){
		msb = s[i] >> 8;
		lsb = s[i] & 0xff;
		IMUWriteByte(MPU9255_ADDRESS, XA_OFFSET_H + ((i-3)*2), msb );
		IMUWriteByte(MPU9255_ADDRESS, XA_OFFSET_H + ((i-3)*2) + 1, lsb );
	}
	*/
	
	char a[6], g[6];
	short xa[10], ya[10], za[10], xg[10], yg[10], zg[10];
	int i;
	
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
	
	
}