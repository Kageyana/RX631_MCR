//==========================//
// インクルード                         			//
//==========================//
#include "SPI_ICM20648.h"
//==========================//
// グローバル変数の宣言					//
//==========================//
volatile int16_t	xa, ya, za; // 加速度(16bitデータ)
volatile int16_t	xg, yg, zg;	// 角加速度(16bitデータ)
volatile uint8_t	who_am_i,ret,imuflag;

double 		TurningAngleIMU;	// yaw軸角度
double		RollAngleIMU;		// Roll角度
double 		PichAngleIMU;		// Pich角度
/////////////////////////////////////////////////////////////////////
// モジュール名 check_crossline						//
// 処理概要     クロスライン検知							//
// 引数         なし									//
// 戻り値       0:クロスラインなし 1:あり						//
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
// モジュール名 check_crossline						//
// 処理概要     クロスライン検知							//
// 引数         なし									//
// 戻り値       0:クロスラインなし 1:あり						//
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
// モジュール名 check_crossline						//
// 処理概要     クロスライン検知							//
// 引数         なし									//
// 戻り値       0:クロスラインなし 1:あり						//
////////////////////////////////////////////////////////////////////
uint8_t IMU_init() {

	
	who_am_i = read_byte(0x00);	//IMU動作確認　0xE0が送られてくればおｋ
	if ( who_am_i == 0xE0 ) {
		ret = 1;
		write_byte(0x06,0x01);	//PWR_MGMT_1	スリープﾓｰﾄﾞ解除
		write_byte(0x03,0x10);	//USER_CTRL	諸々機能無効　SPIonly
		write_byte(0x7F,0x20);	//USER_BANK2
		write_byte(0x01,0x06);	//レンジ±2000dps
		//2:1 GYRO_FS_SEL[1:0] 00:±250	01:±500 10:±1000 11:±2000
		write_byte(0x14,0x06);	//レンジ±16g
		//2:1 ACCEL_FS_SEL[1:0] 00:±2	01:±4 10:±8 11:±16
		write_byte(0x7F,0x00);	//USER_BANK0
	}
	return ret;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 check_crossline						//
// 処理概要     クロスライン検知							//
// 引数         なし									//
// 戻り値       0:クロスラインなし 1:あり						//
////////////////////////////////////////////////////////////////////
void read_gyro_data() {
	xg = ((int16_t)read_byte(0x33) << 8) | ((int16_t)read_byte(0x34));
	yg = ((int16_t)read_byte(0x35) << 8) | ((int16_t)read_byte(0x36));
	zg = ((int16_t)read_byte(0x37) << 8) | ((int16_t)read_byte(0x38));
}
/////////////////////////////////////////////////////////////////////
// モジュール名 check_crossline						//
// 処理概要     クロスライン検知							//
// 引数         なし									//
// 戻り値       0:クロスラインなし 1:あり						//
////////////////////////////////////////////////////////////////////
void read_accel_data() {
	xa = ((int16_t)read_byte(0x2D) << 8) | ((int16_t)read_byte(0x2E));
	ya = ((int16_t)read_byte(0x2F) << 8) | ((int16_t)read_byte(0x30));
	za = ((int16_t)read_byte(0x31) << 8) | ((int16_t)read_byte(0x32));
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getTurningAngleIMU					//
// 処理概要   	IMUから旋回角度の計算					//
// 引数         なし									//
// 戻り値       なし									//
/////////////////////////////////////////////////////////////////////
void getTurningAngleIMU(void)
{
	double angularVelocity_zg;
	
	angularVelocity_zg = (double)(zg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	TurningAngleIMU += (double)( angularVelocity_zg) * DELTATIMU;
	
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getRollAngleIMU						//
// 処理概要   	IMUからロール角度の計算					//
// 引数         なし									//
// 戻り値       なし									//
/////////////////////////////////////////////////////////////////////
void getRollAngleIMU(void)
{
	double angularVelocity_yg;
	
	angularVelocity_yg = (double)(xg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	RollAngleIMU -= (double)( angularVelocity_yg) * DELTATIMU;
	
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getPichAngleIMU						//
// 処理概要     IMUからピッチ角度の計算					//
// 引数         なし									//
// 戻り値       なし									//
/////////////////////////////////////////////////////////////////////
void getPichAngleIMU( void )
{
	double angularVelocity_xg;
	
	angularVelocity_xg = (double)(yg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	PichAngleIMU -= (double)( angularVelocity_xg) * DELTATIMU;
	
}