//==========================//
// �C���N���[�h
//==========================//
#include "ICM20648.h"
//==========================//
// �O���[�o���ϐ��̐錾
//==========================//
volatile int16_t	xa, ya, za; // �����x(16bit�f�[�^)
volatile int16_t	xg, yg, zg;	// �p�����x(16bit�f�[�^)
volatile uint8_t	who_am_i,ret,imuflag;
volatile int16_t	offset[3] = { 0,0,0 };	// �I�t�Z�b�g�l
volatile char 	caribration;

double 		TurningAngleIMU;	// yaw���p�x
double		RollAngleIMU;		// Roll�p�x
double 		PichAngleIMU;		// Pich�p�x
/////////////////////////////////////////////////////////////////////
// ���W���[���� readByte
// �����T�v     �N���X���C�����m
// ����         �Ȃ�
// �߂�l       0:�N���X���C���Ȃ� 1:����
////////////////////////////////////////////////////////////////////
uint8_t readByte( uint8_t reg ) {
	uint8_t ret[1],val[1],dummy[1];
	
	ret[0] = reg | 0x80;
	CS_RESET;
	SPITRANSFER1;
	SPITRANSFER2;
	CS_SET;
	
	return val[0];
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� writeByte
// �����T�v     �N���X���C�����m
// ����         �Ȃ�
// �߂�l       0:�N���X���C���Ȃ� 1:����
////////////////////////////////////////////////////////////////////
void writeByte( uint8_t reg, uint8_t val )  {
	uint8_t ret[1],dummy[1],val2[1];
	
	ret[0] = reg & 0x7F;
	val2[0] = val;
	CS_RESET;
	SPITRANSFER3;
	SPITRANSFER4;
	CS_SET;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� readArry
// �����T�v     �N���X���C�����m
// ����         �Ȃ�
// �߂�l       0:�N���X���C���Ȃ� 1:����
////////////////////////////////////////////////////////////////////
void readArry( uint8_t reg, char *val ) {
	uint8_t ret[1],dummy[1];
	
	ret[0] = reg | 0x80;
	CS_RESET;
	R_PG_SCI_SPIMode_Transfer_C2( ret, dummy, 1);
	R_PG_SCI_SPIMode_Transfer_C2( dummy, val, 6);
	CS_SET;
	
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� checkCrossLine
// �����T�v     �N���X���C�����m
// ����         �Ȃ�
// �߂�l       0:�N���X���C���Ȃ� 1:����
////////////////////////////////////////////////////////////////////
uint8_t initIMU() {

	
	who_am_i = readByte(0x00);	//IMU����m�F�@0xE0�������Ă���΂���
	if ( who_am_i == 0xE0 ) {
		ret = 1;
		writeByte(PWR_MGMT_1,0x01);	//PWR_MGMT_1	�X���[�vӰ�މ���
		writeByte(USER_CTRL,0x10);		//USER_CTRL	���X�@�\�����@SPIonly
		writeByte(REG_BANK_SEL,0x20);	//USER_BANK2
		writeByte(GYRO_CONFIG_1,0x04);	//�����W�}1000dps
		//2:1 GYRO_FS_SEL[1:0] 00:�}250	01:�}500 10:�}1000 11:�}2000
		writeByte(ACCEL_CONFIG,0x04);	//�����W�}8g
		//2:1 ACCEL_FS_SEL[1:0] 00:�}2	01:�}4 10:�}8 11:�}16
		writeByte(REG_BANK_SEL,0x00);	//USER_BANK0
	}
	return ret;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� checkCrossLine
// �����T�v     �N���X���C�����m
// ����         �Ȃ�
// �߂�l       0:�N���X���C���Ȃ� 1:����
////////////////////////////////////////////////////////////////////
void caribrateIMU(void)
{
	char val[14];
	short i, j, axisData[8];
	int axg = 0, ayg = 0, azg = 0;
	
	//STOP_CMT_C0
	for ( i = 0; i < SAMPLE; i++ ) {
		readArry(GYRO_XOUT_H, val);
		for ( j = 0; j < 3; j++ ) {
			axisData[j] = (short)((val[j * 2] << 8 & 0xff00 ) | val[j * 2 + 1]);
			axisData[j] = axisData[j] & 0xfff8;
		}
		axg += axisData[0];
		ayg += axisData[1];
		azg += axisData[2];
	}
	axg /= SAMPLE;
	ayg /= SAMPLE;
	azg /= SAMPLE;
	
	offset[0] = axg / SAMPLE;
	offset[1] = ayg / SAMPLE;
	offset[2] = azg / SAMPLE;
	
	if ( offset[0] < 0 ) offset[0] = -offset[0];
	if ( offset[1] < 0 ) offset[1] = -offset[1];
	if ( offset[2] < 0 ) offset[2] = -offset[2];
	
	TurningAngleIMU = 0;
	RollAngleIMU = 0;
	PichAngleIMU = 0;
	
	//START_CMT_C0
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� checkCrossLine
// �����T�v     �N���X���C�����m
// ����         �Ȃ�
// �߂�l       0:�N���X���C���Ȃ� 1:����
////////////////////////////////////////////////////////////////////
void readGyroData() {
	char val[6];
	readArry(GYRO_XOUT_H, val);
	
	xg = (int16_t)((val[0] << 8 & 0xff00 ) | val[1]);
	yg = (int16_t)((val[2] << 8 & 0xff00 ) | val[3]);
	zg = (int16_t)((val[4] << 8 & 0xff00 ) | val[5]);
	/*
	if ( xg < 0 ) xg += offset[0];
	else		xg -= offset[0];
	if ( yg < 0 ) yg += offset[1];
	else		yg -= offset[1];
	if ( zg < 0 ) zg += offset[2];
	else		zg -= offset[2];*/
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� checkCrossLine
// �����T�v     �N���X���C�����m
// ����         �Ȃ�
// �߂�l       0:�N���X���C���Ȃ� 1:��
////////////////////////////////////////////////////////////////////
void readAccelData() {
	char val[6];
	readArry(ACCEL_XOUT_H, val);
	
	xa = ((int16_t)val[0] << 8) | ((int16_t)val[1]);
	ya = ((int16_t)val[2] << 8) | ((int16_t)val[3]);
	za = ((int16_t)val[4] << 8) | ((int16_t)val[5]);
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getTurningAngleIMU
// �����T�v   	IMU�������p�x�̌v�Z
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void getTurningAngleIMU(void)
{
	double angularVelocity_zg;
	int intzg;
	
	angularVelocity_zg = (double)(zg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	TurningAngleIMU += angularVelocity_zg * DELTATIMU;
	
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getRollAngleIMU
// �����T�v   	IMU���烍�[���p�x�̌v�Z
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void getRollAngleIMU(void)
{
	double angularVelocity_yg;
	int intyg;
	
	angularVelocity_yg = (double)(yg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	RollAngleIMU -= angularVelocity_yg * DELTATIMU;
	
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getPichAngleIMU
// �����T�v     IMU����s�b�`�p�x�̌v�Z
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void getPichAngleIMU( void )
{
	double angularVelocity_xg;
	int intxg;
	
	angularVelocity_xg = (double)(xg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	PichAngleIMU -= angularVelocity_xg * DELTATIMU;
	
}