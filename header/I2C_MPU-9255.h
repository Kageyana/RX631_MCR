#ifndef I2C_MPU9255_H_
#define I2C_MPU9255_H_
//====================================//
// �C���N���[�h									//
//====================================//
#include <stdlib.h>
#include "iodefine.h"
#include "PeripheralFunctions.h"
#include "SCI.h"
#include "LineChase.h"
//====================================//
// �V���{����`									//
//====================================//
// MPU-9255 Register Map
#define SELF_TEST_X_GYRO	0x00
#define SELF_TEST_Y_GYRO	0x01
#define SELF_TEST_Z_GYRO	0x02
#define SELF_TEST_X_ACCEL	0x0D
#define SELF_TEST_Y_ACCEL	0x0E
#define SELF_TEST_Z_ACCEL	0x0F
#define XG_OFFSET_H		0x13
#define XG_OFFSET_L		0x14
#define YG_OFFSET_H		0x15
#define YG_OFFSET_L		0x16
#define ZG_OFFSET_H		0x17
#define ZG_OFFSET_L		0x18
#define SMPLRT_DIV			0x19
#define CONFIG				0x1A
#define GYRO_CONFIG		0x1B
#define ACCEL_CONFIG		0x1C
#define ACCEL_CONFIG2		0x1D
#define LP_ACCEL_ODR		0x1E
#define WOM_THR			0x1F
#define FIFO_EN			0x23
#define I2C_MST_CTRL		0x24
#define I2C_SLV0_addr		0x25
#define I2C_SLV0_REG		0x26
#define I2C_SLV0_CTRL		0x27
#define I2C_SLV1_addr		0x28
#define I2C_SLV1_REG		0x29
#define I2C_SLV1_CTRL		0x2A
#define I2C_SLV2_addr		0x2B
#define I2C_SLV2_REG		0x2C
#define I2C_SLV2_CTRL		0x2D
#define I2C_SLV3_addr		0x2E
#define I2C_SLV3_REG		0x2F
#define I2C_SLV3_CTRL		0x30
#define I2C_SLV4_addr		0x31
#define I2C_SLV4_REG		0x32
#define I2C_SLV4_DO		0x33
#define I2C_SLV4_CTRL		0x34
#define I2C_SLV4_DI			0x35
#define I2C_MST_STATUS		0x36
#define INT_PIN_CFG			0x37
#define INT_ENABLE			0x38
#define INT_STATUS			0x3A
#define ACCEL_XOUT_H		0x3B
#define ACCEL_XOUT_L		0x3C
#define ACCEL_YOUT_H		0x3D
#define ACCEL_YOUT_L		0x3E
#define ACCEL_ZOUT_H		0x3F
#define ACCEL_ZOUT_L		0x40
#define TEMP_OUT_H		0x41
#define TEMP_OUT_L			0x42
#define GYRO_XOUT_H		0x43
#define GYRO_XOUT_L		0x44
#define GYRO_YOUT_H		0x45
#define GYRO_YOUT_L		0x46
#define GYRO_ZOUT_H		0x47
#define GYRO_ZOUT_L		0x48
#define EXT_SENS_DATA_00	0x49
#define EXT_SENS_DATA_01	0x4A
#define EXT_SENS_DATA_02	0x4B
#define EXT_SENS_DATA_03	0x4C
#define EXT_SENS_DATA_04	0x4D
#define EXT_SENS_DATA_05	0x4E
#define EXT_SENS_DATA_06	0x4F
#define EXT_SENS_DATA_07	0x50
#define EXT_SENS_DATA_08	0x51
#define EXT_SENS_DATA_09	0x52
#define EXT_SENS_DATA_10	0x53
#define EXT_SENS_DATA_11	0x54
#define EXT_SENS_DATA_12	0x55
#define EXT_SENS_DATA_13	0x56
#define EXT_SENS_DATA_14	0x57
#define EXT_SENS_DATA_15	0x58
#define EXT_SENS_DATA_16	0x59
#define EXT_SENS_DATA_17	0x5A
#define EXT_SENS_DATA_18	0x5B
#define EXT_SENS_DATA_19	0x5C
#define EXT_SENS_DATA_20	0x5D
#define EXT_SENS_DATA_21	0x5E
#define EXT_SENS_DATA_22	0x5F
#define EXT_SENS_DATA_23	0x60
#define I2C_SLV0_DO		0x63
#define I2C_SLV1_DO		0x64
#define I2C_SLV2_DO		0x65
#define I2C_SLV3_DO		0x66
#define I2C_MST_DELAY_CTRL	0x67
#define SIGNAL_PATH_RESET	0x68
#define MOT_DETECT_CTRL	0x69
#define USER_CTRL			0x6A
#define PWR_MGMT_1		0x6B
#define PWR_MGMT_2		0x6C
#define FIFO_COUNTH		0x72
#define FIFO_COUNTL		0x73
#define FIFO_R_W			0x74
#define WHO_AM_I			0x75
#define XA_OFFSET_H		0x77
#define XA_OFFSET_L		0x78
#define YA_OFFSET_H		0x7A
#define YA_OFFSET_L		0x7B
#define ZA_OFFSET_H		0x7D
#define ZA_OFFSET_L		0x7E

#define MPU9255_ADDRESS     	0xd0		// �������ݎ��̃X���[�u�A�h���X

#define ACCELLSB			2048		// 16[g]
#define GYROLSB			32.8		// 1000[deg/s]
#define TEMP_LSB			333.87	// LSB/��C
#define ROOMTEMPOFFSET	0		// 21��C�̂Ƃ�0

// �f�[�^�����֘A
#define CLOCK				96		// ������g��[MHz]

#define MAXDATA_RANGE		32764	// 16bit�f�[�^�̍ő�l
#define G_ACCELERATION		9.81		// �d�͉����x
#define DELTATIMU			0.005	// �擾����

// �L�����u���[�V����
#define XGSLOPE			0.0048
#define YGSLOPE			-0.0259
#define ZGSLOPE			-0.0032
#define SAMPLE				5000
#define REC_NUM			8
/*************************************** ���������֐� **********************************/
#define I2C_IMU_COMMAND	send_SCI1_I2cWait( MPU9255_ADDRESS, sendData, num)
#define I2C_IMU_RECIVE		receive_SCI1_I2c( MPU9255_ADDRESS, reciveData, num )
#define I2C_IMU_ARRY		receive_data_SCI1_I2c(MPU9255_ADDRESS, sendData, reciveData, num)
/************************************************************************************/
//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//
// IMU����擾�����f�[�^
extern volatile int 	rawXa, rawYa, rawZa;		// �����x(16bit�f�[�^)
extern volatile int 	rawXg, rawYg, rawZg;	// �p�����x(16bit�f�[�^)
extern volatile short 	rawTemp;				// ���x(16bit�f�[�^)

extern short 	rawXa2, rawYa2, rawZa2;	// �����x(16bit�f�[�^)
extern short 	rawXg2, rawYg2, rawZg2;	// �p�����x(16bit�f�[�^)

// �f�[�^����
extern double 		TurningAngleIMU;	// IMU���狁�߂�����p�x
extern double		RollAngleIMU;		// IMU���狁�߂����[�������p�x
extern double 		PichAngleIMU;		// IMU���狁�߂��s�b�`�����p�x
extern double		TempIMU;			// IMU�̉��x

// ���[�h�֘A
extern char	whoami;
extern char	cnt_imu;

//====================================//
// �v���g�^�C�v�錾								//
//====================================//
void wait_IMU ( short waitTime );
void IMUWriteByte( char reg, char data );
char IMUReadByte( char reg , char* reciveData );
bool IMUReadArry( char reg, char num, char* dataArry );
char init_IMU (void);
void IMUProcess (void);
void caribrateIMU (void);
void getTurningAngleIMU(void);
void getRollAngleIMU(void);
void getPichAngleIMU( void );

#endif // I2C_MPU-9255_H_