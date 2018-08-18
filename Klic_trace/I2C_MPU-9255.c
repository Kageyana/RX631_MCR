//======================================//
// インクルード                         //
//======================================//
#include "I2C_MPU-9255.h"
//======================================//
// グローバル変数の宣言                 //
//======================================//
short 	xa, ya, za;		// 加速度
short 	xg, yg, zg;		// 角加速度
char	IMUset = 0;		// 0:初期化失敗		1:初期化完了

//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUWriteByte								//
// 処理概要     指定したレジスタにデータを書き込む					//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス data:書き込みデータ	//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte(short slaveAddr, char reg, char data ) {
	uint8_t sendData[1] = { reg };
    
	I2C_IMU_SEND
	
	sendData[0] = data;
    	I2C_IMU_SEND
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadByte						//
// 処理概要     指定したレジスタのデータを読む				//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス	//
// 戻り値       指定レジスタのデータ 					//
//////////////////////////////////////////////////////////////////////////
char IMUReadByte(short slaveAddr, char reg ) {
	uint8_t sendData[1] = { reg }, reciveData[1];
    
	I2C_IMU_SEND
	I2C_IMU_READ
    
	return reciveData[0];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadArry											//
// 処理概要     指定したレジスタから指定の数だけデータを読む							//
// 引数         slaveAddr:スレーブアドレス addr:レジスタのアドレス num:読み取るデータ数 dataArry:データの格納先	//
// 戻り値       なし 												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry(short slaveAddr, char addr, char num, char* dataArry ) {
	uint8_t sendData[1] = { addr };
    
	sendData[0] = addr;
	I2C_IMU_SEND
	I2C_IMU_ARRY
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 init_IMU						//
// 処理概要     IMUの初期化を行う					//
// 引数         なし							//
// 戻り値       0: 初期化成功1:初期化失敗				//
//////////////////////////////////////////////////////////////////////////
char init_IMU (void) {
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
void IMUProcess (void) {
	//char 	axisAccelData[6];	// 加速度の8bit分割データ格納先
	char 	axisGyroData[6];	// 角加速度の8bit分割データ格納先
	
	//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, axisAccelData);	// 3軸加速度取得
	IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, axisGyroData);	// 3軸角加速度取得
	
	//8bitデータを16bitデータに変換
	// 加速度
	//xa = (short)((axisAccelData[0] << 8 ) | axisAccelData[1]);
	//ya = (short)((axisAccelData[2] << 8 ) | axisAccelData[3]);
	//za = (short)((axisAccelData[4] << 8 ) | axisAccelData[5]);
	
	// 角速度
	xg = (short)((axisGyroData[0] << 8 ) | axisGyroData[1]);
	yg = (short)((axisGyroData[2] << 8 ) | axisGyroData[3]);
	zg = (short)((axisGyroData[4] << 8 ) | axisGyroData[5]);
}