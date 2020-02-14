//==========================//
// �C���N���[�h                         			//
//==========================//
#include "SPI_ICM20648.h"
//==========================//
// �O���[�o���ϐ��̐錾					//
//==========================//
volatile int16_t	xa, ya, za; // �����x(16bit�f�[�^)
volatile int16_t	xg, yg, zg;	// �p�����x(16bit�f�[�^)
volatile uint8_t	who_am_i,ret,imuflag;

double 		TurningAngleIMU;	// yaw���p�x
double		RollAngleIMU;		// Roll�p�x
double 		PichAngleIMU;		// Pich�p�x
/////////////////////////////////////////////////////////////////////
// ���W���[���� check_crossline						//
// �����T�v     �N���X���C�����m							//
// ����         �Ȃ�									//
// �߂�l       0:�N���X���C���Ȃ� 1:����						//
////////////////////////////////////////////////////////////////////
uint8_t read_byte( uint8_t reg ) {
	uint8_t ret[1],val[1],dummy[1];
	
	ret[0] = reg | 0x80;
	CS_RESET;
	SPITRANSFER1;
	SPITRANSFER2;
	CS_SET;
	
	return val[0];
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� check_crossline						//
// �����T�v     �N���X���C�����m							//
// ����         �Ȃ�									//
// �߂�l       0:�N���X���C���Ȃ� 1:����						//
////////////////////////////////////////////////////////////////////
void write_byte( uint8_t reg, uint8_t val )  {
	uint8_t ret[1],dummy[1],val2[1];
	
	ret[0] = reg & 0x7F;
	val2[0] = val;
	CS_RESET;
	SPITRANSFER3;
	SPITRANSFER4;
	CS_SET;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� check_crossline						//
// �����T�v     �N���X���C�����m							//
// ����         �Ȃ�									//
// �߂�l       0:�N���X���C���Ȃ� 1:����						//
////////////////////////////////////////////////////////////////////
uint8_t IMU_init() {

	
	who_am_i = read_byte(0x00);	//IMU����m�F�@0xE0�������Ă���΂���
	if ( who_am_i == 0xE0 ) {
		ret = 1;
		write_byte(0x06,0x01);	//PWR_MGMT_1	�X���[�vӰ�މ���
		write_byte(0x03,0x10);	//USER_CTRL	���X�@�\�����@SPIonly
		write_byte(0x7F,0x20);	//USER_BANK2
		write_byte(0x01,0x06);	//�����W�}2000dps
		//2:1 GYRO_FS_SEL[1:0] 00:�}250	01:�}500 10:�}1000 11:�}2000
		write_byte(0x14,0x06);	//�����W�}16g
		//2:1 ACCEL_FS_SEL[1:0] 00:�}2	01:�}4 10:�}8 11:�}16
		write_byte(0x7F,0x00);	//USER_BANK0
	}
	return ret;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� check_crossline						//
// �����T�v     �N���X���C�����m							//
// ����         �Ȃ�									//
// �߂�l       0:�N���X���C���Ȃ� 1:����						//
////////////////////////////////////////////////////////////////////
void read_gyro_data() {
	xg = ((int16_t)read_byte(0x33) << 8) | ((int16_t)read_byte(0x34));
	yg = ((int16_t)read_byte(0x35) << 8) | ((int16_t)read_byte(0x36));
	zg = ((int16_t)read_byte(0x37) << 8) | ((int16_t)read_byte(0x38));
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� check_crossline						//
// �����T�v     �N���X���C�����m							//
// ����         �Ȃ�									//
// �߂�l       0:�N���X���C���Ȃ� 1:����						//
////////////////////////////////////////////////////////////////////
void read_accel_data() {
	xa = ((int16_t)read_byte(0x2D) << 8) | ((int16_t)read_byte(0x2E));
	ya = ((int16_t)read_byte(0x2F) << 8) | ((int16_t)read_byte(0x30));
	za = ((int16_t)read_byte(0x31) << 8) | ((int16_t)read_byte(0x32));
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getTurningAngleIMU					//
// �����T�v   	IMU�������p�x�̌v�Z					//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
/////////////////////////////////////////////////////////////////////
void getTurningAngleIMU(void)
{
	double angularVelocity_zg;
	
	angularVelocity_zg = (double)(zg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	TurningAngleIMU += (double)( angularVelocity_zg) * DELTATIMU;
	
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getRollAngleIMU						//
// �����T�v   	IMU���烍�[���p�x�̌v�Z					//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
/////////////////////////////////////////////////////////////////////
void getRollAngleIMU(void)
{
	double angularVelocity_yg;
	
	angularVelocity_yg = (double)(xg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	RollAngleIMU -= (double)( angularVelocity_yg) * DELTATIMU;
	
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getPichAngleIMU						//
// �����T�v     IMU����s�b�`�p�x�̌v�Z					//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
/////////////////////////////////////////////////////////////////////
void getPichAngleIMU( void )
{
	double angularVelocity_xg;
	
	angularVelocity_xg = (double)(yg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	PichAngleIMU -= (double)( angularVelocity_xg) * DELTATIMU;
	
}