//======================================//
// インクルード                         //
//======================================//
#include "I2C_MPU-9255.h"
//======================================//
// グローバル変数の宣言                 //
//======================================//
volatile int	cent_data[6] = {0,0,0,0,0,0};	// オフセット値	
volatile short 	rawXa, rawYa, rawZa;	// 加速度(16bitデータ)
volatile short 	rawXg, rawYg, rawZg;	// 角加速度(16bitデータ)
volatile short 	rawTemp;		// 温度(16bitデータ)

char		IMUset = 0;		// 0:初期化失敗		1:初期化完了

//////////////////////////////////////////////////////////////////////////
// モジュール名 wait_IMU						//
// 処理概要     遅延処理						//
// 引数         遅延時間(ms)						//
// 戻り値       なし                                                    //
//////////////////////////////////////////////////////////////////////////
void wait_IMU ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 )/ 16;
	for ( i = 0; i < time; i++) __nop();
}
//////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUWriteByte								//
// 処理概要     指定したレジスタにデータを書き込む					//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス data:書き込みデータ	//
// 戻り値       なし									//
//////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte(char slaveAddr, char reg, char data )
{
	char sendData[2] = { reg, data };
    
	I2C_IMU_COMMAND
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadByte						//
// 処理概要     指定したレジスタのデータを読む				//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス	//
// 戻り値       指定レジスタのデータ 					//
//////////////////////////////////////////////////////////////////////////
char IMUReadByte(char slaveAddr, char reg )
{
	char sendData[1] = { reg }, reciveData[1], num = 1;
    
	I2C_IMU_ARRY
    
	return reciveData[0];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadArry											//
// 処理概要     指定したレジスタから指定の数だけデータを読む							//
// 引数         slaveAddr:スレーブアドレス addr:レジスタのアドレス num:読み取るデータ数 dataArry:データの格納先	//
// 戻り値       なし 												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry(char slaveAddr, char reg, char num, char* reciveData )
{
	char sendData[1] = { reg };
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
		IMUWriteByte(MPU9255_ADDRESS, PWR_MGMT_1, 0x00);	// スリープモード解除
		IMUWriteByte(MPU9255_ADDRESS, INT_PIN_CFG, 0x02);	// 内蔵プルアップ無効化
		IMUWriteByte(MPU9255_ADDRESS, CONFIG, 0x00);		// 8Hzローパスフィルタ
		IMUWriteByte(MPU9255_ADDRESS, ACCEL_CONFIG, 0x18);	// レンジ±16gに変更
		IMUWriteByte(MPU9255_ADDRESS, GYRO_CONFIG, 0x10);	// レンジ±1000deg/sに変更
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
	char 	tempData[2];		// 温度の8bit分割データ格納先
	
	//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, axisAccelData);	// 3軸加速度取得
	IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, axisGyroData);	// 3軸角加速度取得
	IMUReadArry(MPU9255_ADDRESS, TEMP_OUT_H, 2, tempData);		// 温度取得
	
	//8bitデータを16bitデータに変換
	// 加速度
	/*
	rawXa = (short)((axisAccelData[0] << 8 & 0xff00 ) | axisAccelData[1]);
	rawYa = (short)((axisAccelData[2] << 8 & 0xff00 ) | axisAccelData[3]);
	rawZa = (short)((axisAccelData[4] << 8 & 0xff00 ) | axisAccelData[5]);
	*/
	// 角速度
	rawXg = (short)((axisGyroData[0] << 8 & 0xff00 ) | axisGyroData[1]);
	rawYg = (short)((axisGyroData[2] << 8 & 0xff00 ) | axisGyroData[3]);
	rawZg = (short)((axisGyroData[4] << 8 & 0xff00 ) | axisGyroData[5]);
	
	// 温度
	rawTemp = (short)((tempData[0] << 8 & 0xff00 ) | tempData[1]);
	
	//xa -= cent_data[0];
	//ya -= cent_data[1];
	//za -= cent_data[2];
	//xg -= cent_data[3];
	//yg -= cent_data[4];
	//zg -= cent_data[5];

}
//////////////////////////////////////////////////////////////////////////
// モジュール名 caribrateIMU						//
// 処理概要     1msごとに実行しセンサの値を読み取る			//
// 引数         なし							//
// 戻り値       なし 							//
//////////////////////////////////////////////////////////////////////////
void caribrateIMU (void)
{
	char a[6], g[6];
	volatile short xa[2000], ya[2000], za[2000], xg[2000], yg[2000], zg[2000];
	volatile short cnt, temp_cnt = 0;
	
	IMUSet = 0;
	PORT5.PODR.BIT.B5 = 1;
	for( int i = 0; i < 100; i++ ){
		//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, a);	// 3軸加速度取得
		IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, g);	// 3軸角加速度取得
		
		// 加速度
		//xa[i] = (short)((a[0] << 8 ) | a[1]);
		//ya[i] = (short)((a[2] << 8 ) | a[3]);
		//za[i] = (short)((a[4] << 8 ) | a[5]);
		
		// 角速度
		xg[i] = (short)((g[0] << 8 & 0xff00 ) | g[1]);
		yg[i] = (short)((g[2] << 8 & 0xff00 ) | g[3]);
		zg[i] = (short)((g[4] << 8 & 0xff00 ) | g[5]);

		wait_IMU(10);
	}
	PORT5.PODR.BIT.B5 = 0;
	/*

	// モード特定処理
	// 初期値
	mode = 0;
	cnt = 0;
	for ( int j = 0;j < 400; j++ ) {
		temp_cnt = 1;
		for ( int k = j+1;j < 400; k++ ) {
			if ( zg[k] == zg[j] ) temp_cnt++;
		}
		
		if ( temp_cnt > cnt ) {
			cnt = temp_cnt;
			mode = zg[j];
		}
	}
	cent_data[5] = mode;
	
	*/
	IMUSet = 1;
}