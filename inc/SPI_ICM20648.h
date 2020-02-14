#ifndef SPI_ICM20648_H_
#define SPI_ICM20648_H_
//==========================//
// インクルード                         			//
//==========================//
#include "R_PG_IGC-P8080_v1.h"
//==========================//
// シンボル定義                         			//
//==========================//
#define CS_RESET 		R_PG_IO_PORT_Write_PC6( 0x0 );
#define CS_SET   		R_PG_IO_PORT_Write_PC6( 0x1 );
#define SET_SCI_C2		R_PG_SCI_Set_C2();		// シリアルI/Oチャネルを設定(SPI)
#define SPITRANSFER1	R_PG_SCI_SPIMode_Transfer_C2( ret, dummy, 1);
#define SPITRANSFER2	R_PG_SCI_SPIMode_Transfer_C2( dummy, val, 1);
#define SPITRANSFER3	R_PG_SCI_SPIMode_Transfer_C2( ret, dummy, 1);
#define SPITRANSFER4	R_PG_SCI_SPIMode_Transfer_C2( val2, dummy, 1);

#define ACCELLSB			2048		// 16[g]
#define GYROLSB			32.8		// 1000[deg/s]
#define TEMP_LSB			333.87	// LSB/°C
#define ROOMTEMPOFFSET	0		// 21°Cのとき0
// データ処理関連
#define CLOCK				96		// 動作周波数[MHz]

#define MAXDATA_RANGE		32764	// 16bitデータの最大値
#define G_ACCELERATION		9.81		// 重力加速度
#define DELTATIMU			0.001	// 取得周期
//==========================//
// グローバル変数の宣言					//
//==========================//
// IMUから取得したデータ
extern volatile int16_t 	xa, ya, za; // 加速度(16bitデータ)
extern volatile int16_t 	xg, yg, zg;	// 角加速度(16bitデータ)
extern volatile uint8_t		who_am_i,ret,imuflag;

extern double 		TurningAngleIMU;	// yaw軸角度
extern double		RollAngleIMU;		// Roll角度
extern double 		PichAngleIMU;		// Pich角度
//==========================//
// プロトタイプ宣言                     			//
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