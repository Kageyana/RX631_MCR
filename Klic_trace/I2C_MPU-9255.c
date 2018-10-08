//====================================//
// �C���N���[�h									//
//====================================//
#include "I2C_MPU-9255.h"
//====================================//
// �O���[�o���ϐ��̐錾             						//
//====================================//
short	cent_data[7] = {0};	// �I�t�Z�b�g�l	
volatile short 	rawXa, rawYa, rawZa;	// �����x(16bit�f�[�^)
volatile short 	rawXg, rawYg, rawZg;	// �p�����x(16bit�f�[�^)
volatile short 	rawTemp;		// ���x(16bit�f�[�^)

char	pattern_caribrateIMU = 0;// �L�����u���[�V��������
short	sampleIMU[7][SAMPLENUMBER];
short	data_cnt = 0;
short 	median[7], mode[7];
int	averageIMU[7];
char	caribration;		// 0:�L�����u���[�V������~ 1:�L�����u���[�V������

char	IMUset = 0;		// 0:���������s		1:����������
char	whoami;

///////////////////////////////////////////////////////////////////////////
// ���W���[���� wait_IMU								//
// �����T�v     �x������								//
// ����         �x������(ms)								//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void wait_IMU ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 )/ 16;
	for ( i = 0; i < time; i++) __nop();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUWriteByte											//
// �����T�v     �w�肵�����W�X�^�Ƀf�[�^����������								//
// ����         slaveAddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X data:�������݃f�[�^	//
// �߂�l       �Ȃ�													//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte(char slaveAddr, char reg, char data )
{
	char sendData[2] = { reg, data };
    
	I2C_IMU_COMMAND;	// �R�}���h���M
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadByte							//
// �����T�v     �w�肵�����W�X�^�̃f�[�^��ǂ�				//
// ����         slaveAddr:�X���[�u�A�h���X reg:���W�X�^�̃A�h���X	//
// �߂�l       0:����Ɏ�M�@1:��M�s��						//
///////////////////////////////////////////////////////////////////////////
char IMUReadByte(char slaveAddr, char reg , char* reciveData )
{
	char sendData[1] = { reg }, num = 1;
    
	if ( I2C_IMU_DATA == 0 ) {		// �R�}���h���M
		I2C_IMU_RECIVE;		// �f�[�^��M
		return 0;
	} else {
		return 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� IMUReadArry																//
// �����T�v     �w�肵�����W�X�^����w��̐������f�[�^��ǂ�											//
// ����         slaveAddr:�X���[�u�A�h���X addr:���W�X�^�̃A�h���X num:�ǂݎ��f�[�^�� dataArry:�f�[�^�̊i�[��	//
// �߂�l       �Ȃ�																		//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry(char slaveAddr, char reg, char num, char* reciveData )
{
	char sendData[1] = { reg };
	I2C_IMU_ARRY;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� init_IMU								//
// �����T�v     IMU�̏��������s��							//
// ����         �Ȃ�									//
// �߂�l       0: ����������1:���������s					//
///////////////////////////////////////////////////////////////////////////
char init_IMU (void)
{
	char ret  = 0, reciveData[1];
	
	if ( IMUReadByte(MPU9255_ADDRESS, WHO_AM_I, reciveData) == 0 ) {
		if ( reciveData[0] == 0x71 || reciveData[0] == 0x73 ) {
			whoami = reciveData[0];
			wait_IMU(35);
			
			IMUWriteByte(MPU9255_ADDRESS, PWR_MGMT_1, 0x00);	// �X���[�v���[�h����
			IMUWriteByte(MPU9255_ADDRESS, INT_PIN_CFG, 0x02);	// �����v���A�b�v������
			IMUWriteByte(MPU9255_ADDRESS, CONFIG, 0x00);		// ���[�p�X�t�B���^���g�p���Ȃ�
			IMUWriteByte(MPU9255_ADDRESS, ACCEL_CONFIG, 0x18);	// �����W�}16g�ɕύX
			IMUWriteByte(MPU9255_ADDRESS, GYRO_CONFIG, 0x10);	// �����W�}1000deg/s�ɕύX
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
	
	IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, axisData);	// 3�������x�擾
	
	//8bit�f�[�^��16bit�f�[�^�ɕϊ�
	// �����x
	//rawXa = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
	//rawYa = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
	//rawZa = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
	
	// ���x
	//rawTemp = (short)((axisData[6] << 8 & 0xff00 ) | axisData[7]);
	
	// �p���x
	rawXg = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
	rawYg = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
	rawZg = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
	
	rawXa -= cent_data[0];
	rawYa -= cent_data[1];
	rawZa -= cent_data[2];
	rawXg -= cent_data[4];
	rawYg -= cent_data[5];
	rawZg -= cent_data[6];

}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� caribrateIMU							//
// �����T�v     1ms���ƂɎ��s���Z���T�̒l��ǂݎ��			//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void caribrateIMU (char data_Option)
{
	char data[6];
	short i, j, k, datasize;
	short cnt, temp_cnt = 0;
	
	switch( pattern_caribrateIMU ) {
		case 0:
			if ( caribration ) {
				IMUSet = 0;	// �L�����u���[�V�����J�n
				// ������
				data_cnt = 0;
				for ( i = 0; i < 7; i++ ) {
					averageIMU[i] = 0;
					mode[i] = 0;
					median[i] = 0;
				}
				
				pattern_caribrateIMU = 1;
			}
			break;
			
		case 1:
			PORT5.PODR.BIT.B5 = 1;
			IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 14, data);	// 6���f�[�^+���x�擾
			
			// 16bit�f�[�^�ɕϊ�
			for ( i = 0; i < 7; i++ ) {
				sampleIMU[i][data_cnt] = (short)((data[i * 2] << 8 & 0xff00 ) | data[i * 2 + 1]);
			}
			data_cnt++;
			if ( data_cnt >= SAMPLENUMBER ) {
				PORT5.PODR.BIT.B5 = 0;
				data_cnt = SAMPLENUMBER;	// �f�[�^�����v�Z
				datasize = 2;		// �f�[�^�T�C�Y�v�Z 
				
				if( data_Option == AVERAGE )	pattern_caribrateIMU = AVERAGE;
				if( data_Option == MODE )	pattern_caribrateIMU = MODE;
				if( data_Option == MEDIAN )	pattern_caribrateIMU = MEDIAN;
			}
			break;
			
		case AVERAGE + 0:
			for ( i = 0; i < data_cnt; i++ ) {
				for ( j = 0; j < 7; j++ ) averageIMU[j] += sampleIMU[j][i];
			}
			for ( i = 0; i < 7; i++ ) {
				averageIMU[i] /= data_cnt;
				cent_data[i] = averageIMU[i];
			}
			
			pattern_caribrateIMU = 40;
			break;
			
		case MODE + 0:
			for ( i = 0; i < 7; i++ ) {
				// �����l
				mode[i] = 0;
				cnt = 0;
				for ( j = 0;j < data_cnt; j++ ) {
					temp_cnt = 1;
					for ( k = j+1;k < data_cnt; k++ ) {
						if ( sampleIMU[i][j] == sampleIMU[i][k] ) temp_cnt++;
					}
					
					if ( temp_cnt > cnt ) {
						cnt = temp_cnt;
						mode[i] = sampleIMU[i][j];
					}
				}
				cent_data[i] = mode[i];
			}
			
			pattern_caribrateIMU = 40;
			break;
			
		case MEDIAN + 0:
			for ( i = 0; i < 7; i++ ) {
				qsort( (void*)sampleIMU[i], data_cnt, datasize, short_sort );
				median[i] = sampleIMU[i][data_cnt/2];
				cent_data[i] = median[i];
			}
			pattern_caribrateIMU = 40;
			break;
		
		case 40:
			IMUSet = 1;
			caribration = 0;
			setBeepPatternS( 0xa300 );
			
			pattern_caribrateIMU = 0;
			break;
		
	}
}