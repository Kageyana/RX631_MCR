#ifndef SPI_ICM20648_H_
#define SPI_ICM20648_H_
//==========================//
// インクルード
//==========================//
#include "R_PG_IGC-P8080_v1.h"
#include <math.h>
//==========================//
// シンボル定義
//==========================//
// USER BANK 0
#define WHO_AM_I			0x00
#define USER_CTRL			0x03
#define LP_CONFIG			0x05
#define PWR_MGMT_1			0x06
#define PWR_MGMT_2			0x07
#define INT_PIN_CFG			0x0F
#define INT_ENABLE			0x10
#define INT_ENABLE_1		0x11
#define INT_ENABLE_2		0x12
#define INT_ENABLE_3		0x13
#define I2C_MST_STATUS		0x17
#define INT_STATUS			0x19
#define INT_STATUS_1		0x1A
#define INT_STATUS_2		0x1B
#define INT_STATUS_3		0x1C
#define DELAY_TIMEH			0x28
#define DELAY_TIMEL			0x29
#define ACCEL_XOUT_H		0x2D    
#define ACCEL_XOUT_L		0x2E 
#define ACCEL_YOUT12_H		0x2F
#define ACCEL_YOUT_L		0x30
#define ACCEL_ZOUT_H		0x31
#define ACCEL_ZOUT_L		0x32
#define GYRO_XOUT_H			0x33
#define GYRO_XOUT_L			0x34
#define GYRO_YOUT_H			0x35
#define GYRO_YOUT_L			0x36
#define GYRO_ZOUT_H			0x37
#define GYRO_ZOUT_L			0x38
#define TEMP_OUT_H			0x39
#define TEMP_OUT_L				0x3A
#define EXT_SLV_SENS_DATA_00	0x3B
#define EXT_SLV_SENS_DATA_01	0x3C
#define EXT_SLV_SENS_DATA_02	0x3D
#define EXT_SLV_SENS_DATA_03	0x3E
#define EXT_SLV_SENS_DATA_04	0x3F
#define EXT_SLV_SENS_DATA_05	0x40
#define EXT_SLV_SENS_DATA_06	0x41
#define EXT_SLV_SENS_DATA_07	0x42
#define EXT_SLV_SENS_DATA_08	0x43
#define EXT_SLV_SENS_DATA_09	0x44
#define EXT_SLV_SENS_DATA_10	0x45
#define EXT_SLV_SENS_DATA_11	0x46
#define EXT_SLV_SENS_DATA_12	0x47
#define EXT_SLV_SENS_DATA_13	0x48
#define EXT_SLV_SENS_DATA_14	0x49
#define EXT_SLV_SENS_DATA_15	0x4A
#define EXT_SLV_SENS_DATA_16	0x4B
#define EXT_SLV_SENS_DATA_17	0x4C
#define EXT_SLV_SENS_DATA_18	0x4D
#define EXT_SLV_SENS_DATA_19	0x4E
#define EXT_SLV_SENS_DATA_20	0x4F
#define EXT_SLV_SENS_DATA_21	0x50
#define EXT_SLV_SENS_DATA_22	0x51
#define EXT_SLV_SENS_DATA_23	0x52
#define FIFO_EN_1				0x66
#define FIFO_EN_2				0x67
#define FIFO_RST				0x68
#define FIFO_MODE				0x69
#define FIFO_COUNTH			    0x70
#define FIFO_COUNTL			    0x71
#define FIFO_R_W				0x72
#define DATA_RDY_STATUS		    0x74
#define FIFO_CFG				0x76
#define REG_BANK_SEL			0x7F
// USER BANK 2
#define GYRO_SMPLRT_DIV		    0x00
#define GYRO_CONFIG_1			0x01
#define GYRO_CONFIG_2			0x02
#define XG_OFFS_USRH			0x03
#define XG_OFFS_USRL			0x04
#define YG_OFFS_USRH			0x05
#define YG_OFFS_USRL			0x06
#define ZG_OFFS_USRH			0x07
#define ZG_OFFS_USRL			0x08
#define ODR_ALIGN_EN			0x09
#define ACCEL_SMPLRT_DIV_1		0x10
#define ACCEL_SMPLRT_DIV_2		0x11
#define ACCEL_INTEL_CTRL		0x12
#define ACCEL_WOM_THR			0x13
#define ACCEL_CONFIG			0x14
#define ACCEL_CONFIG_2			0x15
#define FSYNC_CONFIG			0x52
#define TEMP_CONFIG			    0x53
#define MOD_CTRL_USR			0x54

/*************************************** 自動生成関数 *************************************/
#define CS_RESET 		R_PG_IO_PORT_Write_PC6( 0x0 );
#define CS_SET   		R_PG_IO_PORT_Write_PC6( 0x1 );
#define SET_SCI_C2		R_PG_SCI_Set_C2();		// シリアルI/Oチャネルを設定(SPI)
#define SPITRANSFER1	R_PG_SCI_SPIMode_Transfer_C2( ret, dummy, 1);
#define SPITRANSFER2	R_PG_SCI_SPIMode_Transfer_C2( dummy, val, 1);
#define SPITRANSFER3	R_PG_SCI_SPIMode_Transfer_C2( ret, dummy, 1);
#define SPITRANSFER4	R_PG_SCI_SPIMode_Transfer_C2( val2, dummy, 1);
/******************************************************************************************/
#define ACCELLSB			4096		// 16[g]
#define GYROLSB			    32.8		// 2000[deg/s]
#define TEMP_LSB			333.87	// LSB/°C
#define ROOMTEMPOFFSET	    0		// 21°Cのとき0
// データ処理関連
#define CLOCK				96		// 動作周波数[MHz]

#define MAXDATA_RANGE		32764	// 16bitデータの最大値
#define G_ACCELERATION		9.81		// 重力加速度
#define DELTATIMU			0.01F	// 取得周期
#define SAMPLE				5000		// キャリブレーション回数
//==========================//
// グローバル変数の宣言
//==========================//
// IMUから取得したデータ
extern volatile int16_t 	xa, ya, za; // 加速度(16bitデータ)
extern volatile int16_t 	xg, yg, zg;	// 角加速度(16bitデータ)
extern volatile uint8_t		who_am_i,ret,imuflag;
extern volatile int16_t		offset[3];	// オフセット値
extern volatile char 		caribration;

extern double 		TurningAngleIMU;	// yaw軸角度
extern double		RollAngleIMU;		// Roll角度
extern double 		PichAngleIMU;		// Pich角度
//==========================//
// プロトタイプ宣言
//==========================//
uint8_t readByte( uint8_t );
void writeByte( uint8_t, uint8_t);
void readArry( uint8_t reg, char *val);
uint8_t initIMU(void);
void caribrateIMU(void);
void readGyroData(void);
void readAccelData(void);
void getTurningAngleIMU(void);
void getRollAngleIMU(void);
void getPichAngleIMU( void );

#endif // SPI_ICM20648_H_