//======================================//
// インクルード                         //
//======================================//
#include "I2C_MPU-9255.h"
//======================================//
// グローバル変数の宣言                 //
//======================================//
volatile short 	xa, ya, za;		// 加速度
volatile short 	xg, yg, zg;		// 角加速度
char	IMUset = 0;		// 0:初期化失敗		1:初期化完了

//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUWriteByte								//
// 処理概要     指定したレジスタにデータを書き込む					//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス data:書き込みデータ	//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte(short slaveAddr, char reg, char data )
{
	uint8_t sendData[2] = { reg, data };
    
	I2C_IMU_COMMAND
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadByte						//
// 処理概要     指定したレジスタのデータを読む				//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス	//
// 戻り値       指定レジスタのデータ 					//
//////////////////////////////////////////////////////////////////////////
char IMUReadByte(short slaveAddr, char reg )
{
	uint8_t sendData[1] = { reg }, reciveData[1];
    
	I2C_IMU_DATA
	I2C_IMU_READ
    
	return reciveData[0];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadArry											//
// 処理概要     指定したレジスタから指定の数だけデータを読む							//
// 引数         slaveAddr:スレーブアドレス addr:レジスタのアドレス num:読み取るデータ数 dataArry:データの格納先	//
// 戻り値       なし 												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry(short slaveAddr, char reg, char num, char* dataArry )
{
	uint8_t sendData[1] = { reg };
    
	I2C_IMU_DATA
	I2C_IMU_ARRY
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 init_IMU						//
// 処理概要     IMUの初期化を行う					//
// 引数         なし							//
// 戻り値       0: 初期化成功1:初期化失敗				//
//////////////////////////////////////////////////////////////////////////
char init_IMU (void)
{
	char ret  = 0, whoami;
	
	whoami = IMUReadByte(MPU9255_ADDRESS, WHO_AM_I);
	if ( whoami == 0x71 || whoami == 0x73 ) {
		IMUWriteByte(MPU9255_ADDRESS, PWR_MGMT_1, 0x00);    // スリープモード解除
		IMUWriteByte(MPU9255_ADDRESS, INT_PIN_CFG, 0x02);   // 内蔵プルアップ無効化
		IMUWriteByte(MPU9255_ADDRESS, ACCEL_CONFIG, 0x18);  // レンジ±16gに変更
		IMUWriteByte(MPU9255_ADDRESS, GYRO_CONFIG, 0x18);   // レンジ±2000deg/sに変更
	} else {
		ret = 1;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 IMUProcess						//
// 処理概要     1msごとに実行しセンサの値を読み取る			//
// 引数         なし							//
// 戻り値       なし 							//
//////////////////////////////////////////////////////////////////////////
void IMUProcess (void)
{
	//char 	axisAccelData[6];	// 加速度の8bit分割データ格納先
	char 	axisGyroData[6];	// 角加速度の8bit分割データ格納先
	
	//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, axisAccelData);	// 3軸加速度取得
	IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, axisGyroData);	// 3軸角加速度取得
	
	//8bitデータを16bitデータに変換
	// 加速度
	/*
	xa = (short)((axisAccelData[0] << 8 ) | axisAccelData[1]);
	ya = (short)((axisAccelData[2] << 8 ) | axisAccelData[3]);
	za = (short)((axisAccelData[4] << 8 ) | axisAccelData[5]);
	*/
	// 角速度
	xg = (short)((axisGyroData[0] << 8 ) | axisGyroData[1]);
	yg = (short)((axisGyroData[2] << 8 ) | axisGyroData[3]);
	zg = (short)((axisGyroData[4] << 8 ) | axisGyroData[5]);

}
//////////////////////////////////////////////////////////////////////////
// モジュール名 caribrateIMU						//
// 処理概要     1msごとに実行しセンサの値を読み取る			//
// 引数         なし							//
// 戻り値       なし 							//
//////////////////////////////////////////////////////////////////////////
void caribrateIMU (void)
{
	uint8_t  msb, lsb;
	char a[6], g[6];
	short xa[10], ya[10], za[10], xg[10], yg[10], zg[10];
	int i, ave[6] = {0,0,0,0,0,0};
	
	for( i = 0; i < 10; i++ ){
		IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, a);	// 3軸加速度取得
		IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, g);	// 3軸角加速度取得
		
		// 加速度
		xa[i] = (short)((a[0] << 8 ) | a[1]);
		ya[i] = (short)((a[2] << 8 ) | a[3]);
		za[i] = (short)((a[4] << 8 ) | a[5]);
		
		// 角速度
		xg[i] = (short)((g[0] << 8 ) | g[1]);
		yg[i] = (short)((g[2] << 8 ) | g[3]);
		zg[i] = (short)((g[4] << 8 ) | g[5]);
		
		ave[0] += xa[i];
		ave[1] += ya[i];
		ave[2] += za[i];
		ave[3] += xg[i];
		ave[4] += yg[i];
		ave[5] += zg[i];
	}
	
	ave[0] /= 10;
	ave[1] /= 10;
	ave[2] /= 10;
	ave[3] /= 10;
	ave[4] /= 10;
	ave[5] /= 10;
	
	ave[0] = ave[0] * pow(2, 3) / 4;
	ave[1] = ave[1] * pow(2, 3) / 4;
	ave[2] = ave[2] * pow(2, 3) / 4;
	ave[3] = ave[3] * pow(2, 3) / 4;
	ave[4] = ave[4] * pow(2, 3) / 4;
	ave[5] = ave[5] * pow(2, 3) / 4;
	
	for( i = 0; i < 3; i++ ){
		msb = ( ave[i] & 0x0000ff00 ) >> 8;
		lsb = (char)ave[i];
		IMUWriteByte(MPU9255_ADDRESS, XG_OFFSET_H + (i*2), lsb );
		IMUWriteByte(MPU9255_ADDRESS, XG_OFFSET_H + (i*2) + 1, msb );
	}
	for( i = 3; i < 6; i++ ){
		msb = ( ave[i] & 0x0000ff00 ) >> 8;
		lsb = (char)ave[i];
		IMUWriteByte(MPU9255_ADDRESS, XA_OFFSET_H + ((i-3)*2), lsb );
		IMUWriteByte(MPU9255_ADDRESS, XA_OFFSET_H + ((i-3)*2) + 1, msb );
	}
}