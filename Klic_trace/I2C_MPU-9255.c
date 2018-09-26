//======================================//
// �C���N���[�h                         //
//======================================//
#include "I2C_MPU-9255.h"
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
volatile int	cent_data[6] = {0,0,0,0,0,0};	// �I�t�Z�b�g�l	
volatile short 	rawXa, rawYa, rawZa;	// �����x(16bit�f�[�^)
volatile short 	rawXg, rawYg, rawZg;	// �p�����x(16bit�f�[�^)
volatile short 	rawTemp;		// ���x(16bit�f�[�^)

char		IMUset = 0;		// 0:���������s		1:����������

//////////////////////////////////////////////////////////////////////////
// ���W���[���� wait_IMU						//
// �����T�v     �x������						//
// ����         �x������(ms)						//
// �߂�l       �Ȃ�                                                    //
//////////////////////////////////////////////////////////////////////////
void wait_IMU ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 )/ 16;
	for ( i = 0; i < time; i++) __nop();
}
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
	char sendData[1] = { reg }, reciveData[1], num = 1;
    
	I2C_IMU_ARRY
    
	return reciveData[0];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadArry											//
// �����T�v     �w�肵�����W�X�^����w��̐������f�[�^��ǂ�							//
// ����         slaveAddr:�X���[�u�A�h���X addr:���W�X�^�̃A�h���X num:�ǂݎ��f�[�^�� dataArry:�f�[�^�̊i�[��	//
// �߂�l       �Ȃ� 												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry(char slaveAddr, char reg, char num, char* reciveData )
{
	char sendData[1] = { reg };
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
	
	//xa -= cent_data[0];
	//ya -= cent_data[1];
	//za -= cent_data[2];
	//xg -= cent_data[3];
	//yg -= cent_data[4];
	//zg -= cent_data[5];

}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� caribrateIMU						//
// �����T�v     1ms���ƂɎ��s���Z���T�̒l��ǂݎ��			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ� 							//
//////////////////////////////////////////////////////////////////////////
void caribrateIMU (void)
{
	char a[6], g[6];
	volatile short xa[2000], ya[2000], za[2000], xg[2000], yg[2000], zg[2000];
	volatile short cnt, temp_cnt = 0;
	
	IMUSet = 0;
	PORT5.PODR.BIT.B5 = 1;
	for( int i = 0; i < 100; i++ ){
		//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, a);	// 3�������x�擾
		IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, g);	// 3���p�����x�擾
		
		// �����x
		//xa[i] = (short)((a[0] << 8 ) | a[1]);
		//ya[i] = (short)((a[2] << 8 ) | a[3]);
		//za[i] = (short)((a[4] << 8 ) | a[5]);
		
		// �p���x
		xg[i] = (short)((g[0] << 8 & 0xff00 ) | g[1]);
		yg[i] = (short)((g[2] << 8 & 0xff00 ) | g[3]);
		zg[i] = (short)((g[4] << 8 & 0xff00 ) | g[5]);

		wait_IMU(10);
	}
	PORT5.PODR.BIT.B5 = 0;
	/*

	// ���[�h���菈��
	// �����l
	mode = 0;
	cnt = 0;
	for ( int j = 0;j < 400; j++ ) {
		temp_cnt = 1;
		for ( int k = j+1;j < 400; k++ ) {
			if ( zg[k] == zg[j] ) temp_cnt++;
		}
		
		if ( temp_cnt > cnt ) {
			cnt = temp_cnt;
			mode = zg[j];
		}
	}
	cent_data[5] = mode;
	
	*/
	IMUSet = 1;
}