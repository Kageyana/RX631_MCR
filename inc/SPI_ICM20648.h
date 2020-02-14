#ifndef SPI_ICM20648_H_
#define SPI_ICM20648_H_
//==========================//
// �C���N���[�h                         			//
//==========================//
#include "R_PG_IGC-P8080_v1.h"
//==========================//
// �V���{����`                         			//
//==========================//
#define CS_RESET 		R_PG_IO_PORT_Write_PC6( 0x0 );
#define CS_SET   		R_PG_IO_PORT_Write_PC6( 0x1 );
#define SET_SCI_C2		R_PG_SCI_Set_C2();		// �V���A��I/O�`���l����ݒ�(SPI)
#define SPITRANSFER1	R_PG_SCI_SPIMode_Transfer_C2( ret, dummy, 1);
#define SPITRANSFER2	R_PG_SCI_SPIMode_Transfer_C2( dummy, val, 1);
#define SPITRANSFER3	R_PG_SCI_SPIMode_Transfer_C2( ret, dummy, 1);
#define SPITRANSFER4	R_PG_SCI_SPIMode_Transfer_C2( val2, dummy, 1);

#define ACCELLSB			2048		// 16[g]
#define GYROLSB			32.8		// 1000[deg/s]
#define TEMP_LSB			333.87	// LSB/��C
#define ROOMTEMPOFFSET	0		// 21��C�̂Ƃ�0
// �f�[�^�����֘A
#define CLOCK				96		// ������g��[MHz]

#define MAXDATA_RANGE		32764	// 16bit�f�[�^�̍ő�l
#define G_ACCELERATION		9.81		// �d�͉����x
#define DELTATIMU			0.001	// �擾����
//==========================//
// �O���[�o���ϐ��̐錾					//
//==========================//
// IMU����擾�����f�[�^
extern volatile int16_t 	xa, ya, za; // �����x(16bit�f�[�^)
extern volatile int16_t 	xg, yg, zg;	// �p�����x(16bit�f�[�^)
extern volatile uint8_t		who_am_i,ret,imuflag;

extern double 		TurningAngleIMU;	// yaw���p�x
extern double		RollAngleIMU;		// Roll�p�x
extern double 		PichAngleIMU;		// Pich�p�x
//==========================//
// �v���g�^�C�v�錾                     			//
//==========================//
uint8_t read_byte( uint8_t );
void write_byte( uint8_t, uint8_t);
uint8_t IMU_init(void);
void read_gyro_data(void);
void read_accel_data(void);
void getTurningAngleIMU(void);
void getRollAngleIMU(void);
void getPichAngleIMU( void );

#endif // SPI_ICM20648_H_