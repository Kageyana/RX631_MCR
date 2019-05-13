//====================================//
// �C���N���[�h									//
//====================================//
#include "I2C_MPU-9255.h"
//====================================//
// �O���[�o���ϐ��̐錾             						//
//====================================//
// IMU����擾�����f�[�^
volatile int 	rawXa = 0, rawYa = 0, rawZa = 0;	// �����x(16bit�f�[�^)
volatile int 	rawXg = 0, rawYg = 0, rawZg = 0;	// �p�����x(16bit�f�[�^)

volatile short 	rawTemp;			// ���x(16bit�f�[�^)

// �f�[�^����
double 	TurningAngleIMU;	// IMU���狁�߂����[�p�x
double	RollAngleIMU;		// IMU���狁�߂����[�������p�x
double 	PichAngleIMU;		// IMU���狁�߂��s�b�`�����p�x
double	TempIMU;			// IMU�̉��x
short		offset[3];			// �I�t�Z�b�g�l(16bit)

char	whoami;
char cnt_imu = 0;

///////////////////////////////////////////////////////////////////////////
// ���W���[���� wait_IMU								//
// �����T�v     �x������								//
// ����         �x������(ms)								//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void wait_IMU ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 ) / 16;
	for ( i = 0; i < time; i++) __nop();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUWriteByte											//
// �����T�v     �w�肵�����W�X�^�Ƀf�[�^����������								//
// ����         slaveaddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X data:�������݃f�[�^	//
// �߂�l       �Ȃ�													//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte( char reg, char data )
{
	char sendData[2] = { reg, data }, num = 2;
    
	I2C_IMU_COMMAND;		// �R�}���h���M
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadByte							//
// �����T�v     �w�肵�����W�X�^�̃f�[�^��ǂ�				//
// ����         slaveaddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X	//
// �߂�l       0:����Ɏ�M�@1:��M�s��						//
///////////////////////////////////////////////////////////////////////////
char IMUReadByte( char reg , char* reciveData )
{
	char sendData[1] = { reg }, num = 1;
  	
	if ( !I2C_IMU_COMMAND ) {	// �R�}���h���M
		I2C_IMU_RECIVE;		// �f�[�^��M
		return 0;
	} else {
		return 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadArry																//
// �����T�v     �w�肵�����W�X�^����w��̐������f�[�^��ǂ�											//
// ����         slaveaddr:�X���[�u�A�h���X addr:���W�X�^�̃A�h���X num:�ǂݎ��f�[�^�� dataArry:�f�[�^�̊i�[��	//
// �߂�l       0:����ɒʐM	1:�ʐM�G���[																		//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IMUReadArry( char reg, char num, char* reciveData )
{
	char sendData[1] = { reg };
	if ( !I2C_IMU_ARRY ) return false;
	else	return true;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� init_IMU								//
// �����T�v     IMU�̏��������s��							//
// ����         �Ȃ�									//
// �߂�l       0: ����������1:���������s					//
///////////////////////////////////////////////////////////////////////////
char init_IMU (void)
{
	char ret  = 0, reciveData[1], i, j = 0;
	for ( i = 0; i < 9; i++ ) {
		if ( !IMUReadByte( WHO_AM_I, reciveData) ) {
			j = 1;
			break;
		}
	}
	if ( j ) {
		if ( reciveData[0] == 0x71 || reciveData[0] == 0x73 ) {
			whoami = reciveData[0];
			wait_IMU(35);
			
			IMUWriteByte( PWR_MGMT_1, 0x00);	// �X���[�v���[�h����
			IMUWriteByte( INT_PIN_CFG, 0x02);	// �����v���A�b�v������
			IMUWriteByte( CONFIG, 0x00);		// ���[�p�X�t�B���^���g�p���Ȃ�
			IMUWriteByte( ACCEL_CONFIG, 0x18);	// �����W�}16g�ɕύX
			IMUWriteByte( GYRO_CONFIG, 0x10);	// �����W�}1000deg/s�ɕύX
		} else {
			ret = 1;
		}
	} else {
		ret = 1;
	}
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUProcess								//
// �����T�v     1ms���ƂɎ��s���Z���T�̒l��ǂݎ��			//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void IMUProcess (void)
{
	char 	axisData[14];	// �p�����x�A���x��8bit�����f�[�^�i�[��
	
	if ( !IMUReadArry( GYRO_XOUT_H, 6, axisData) ) {	// 3�������x�擾
		//8bit�f�[�^��16bit�f�[�^�ɕϊ�
		// �p���x
		rawXg = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
		rawYg = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
		rawZg = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
		
		if ( rawXg < 0 ) rawXg += offset[0];
		else			rawXg -= offset[0];
		if ( rawYg < 0 ) rawYg += offset[1];
		else			rawYg -= offset[1];
		if ( rawZg < 0 ) rawZg += offset[2];
		else			rawZg -= offset[2];
		/*
		IMUReadArry( ACCEL_XOUT_H, 14, axisData);	// 6�������x�擾
		
		//8bit�f�[�^��16bit�f�[�^�ɕϊ�
		// �����x
		rawXa = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
		rawYa = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
		rawZa = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
		
		// ���x
		rawTemp = (short)((axisData[6] << 8 & 0xff00 ) | axisData[7]);
		
		// �p���x
		rawXg = (short)((axisData[8] << 8 & 0xff00 ) | axisData[9]);
		rawYg = (short)((axisData[10] << 8 & 0xff00 ) | axisData[11]);
		rawZg = (short)((axisData[12] << 8 & 0xff00 ) | axisData[13]);
		*/
	} else {
		setBeepPatternS( 0x8000 );
		init_SCI1( UART, RATE_230400 );
		IMUSet = 0;
	}
	
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� caribrateIMU							//
// �����T�v     1ms���ƂɎ��s���Z���T�̒l��ǂݎ��			//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void caribrateIMU (void)
{
	char data1[14];
	short i, j, axisData[8];
	int atemp = 0, axg = 0, ayg = 0, azg = 0;
	double	xy, yy, zy;		// �p���x���ϒl
	double	xb, yb, zb;		// �ߎ������̐ؕ�
	
	for ( i = 0; i < SAMPLE; i++ ) {
		IMUReadArry( TEMP_OUT_H, 8, data1);
		for ( j = 0; j < 4; j++ ) axisData[j] = (short)((data1[j * 2] << 8 & 0xff00 ) | data1[j * 2 + 1]);
		atemp += axisData[0];
		axg += axisData[1];
		ayg += axisData[2];
		azg += axisData[3];
	}
	atemp /= SAMPLE;
	axg /= SAMPLE;
	ayg /= SAMPLE;
	azg /= SAMPLE;
	
	TempIMU = (double)(atemp / TEMP_LSB) + 21.0;
	
	xy = (double)axg/GYROLSB;
	xb = (double)xy - ( XGSLOPE * TempIMU );
	
	yy = (double)ayg/GYROLSB;
	yb = (double)yy - ( YGSLOPE * TempIMU );
	
	zy = (double)azg/GYROLSB;
	zb = (double)zy - ( ZGSLOPE * TempIMU );
	
	offset[0] = (((double)XGSLOPE * TempIMU ) + xb ) * GYROLSB;
	offset[1] = (((double)YGSLOPE * TempIMU ) + yb ) * GYROLSB;
	offset[2] = (((double)ZGSLOPE * TempIMU ) + zb ) * GYROLSB;
	
	if ( offset[0] < 0 ) offset[0] = -offset[0];
	if ( offset[1] < 0 ) offset[1] = -offset[1];
	if ( offset[2] < 0 ) offset[2] = -offset[2];
	
	TurningAngleIMU = 0;
	RollAngleIMU = 0;
	PichAngleIMU = 0;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getTurningAngleIMU						//
// �����T�v   	IMU�������p�x�̌v�Z					//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void getTurningAngleIMU(void)
{
	double angularVelocity_zg;
	
	angularVelocity_zg = (double)(rawZg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	TurningAngleIMU += (double)( angularVelocity_zg) * DELTATIMU;
	
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getRollAngleIMU							//
// �����T�v   	IMU���烍�[���p�x�̌v�Z					//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void getRollAngleIMU(void)
{
	double angularVelocity_yg;
	
	angularVelocity_yg = (double)(rawXg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	RollAngleIMU -= (double)( angularVelocity_yg) * DELTATIMU;
	
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getPichAngleIMU							//
// �����T�v     IMU����s�b�`�p�x�̌v�Z						//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void getPichAngleIMU( void )
{
	double angularVelocity_xg;
	
	angularVelocity_xg = (double)(rawYg) / GYROLSB;	// IMU�̃f�[�^���p���x[deg/s]�ɕϊ�
	
	PichAngleIMU -= (double)( angularVelocity_xg) * DELTATIMU;
	
}