//====================================//
// インクルード									//
//====================================//
#include "I2C_MPU-9255.h"
//====================================//
// グローバル変数の宣言             						//
//====================================//
// IMUから取得したデータ
volatile int 	rawXa = 0, rawYa = 0, rawZa = 0;	// 加速度(16bitデータ)
volatile int 	rawXg = 0, rawYg = 0, rawZg = 0;	// 角加速度(16bitデータ)

volatile short 	rawTemp;			// 温度(16bitデータ)

// データ処理
double 	TurningAngleIMU;	// IMUから求めたヨー角度
double	RollAngleIMU;		// IMUから求めたロール方向角度
double 	PichAngleIMU;		// IMUから求めたピッチ方向角度
double	TempIMU;			// IMUの温度
short		offset[3];			// オフセット値(16bit)

char	IMUset = 0;		// 0:初期化失敗		1:初期化完了
char	whoami;
char cnt_imu = 0;

///////////////////////////////////////////////////////////////////////////
// モジュール名 wait_IMU								//
// 処理概要     遅延処理								//
// 引数         遅延時間(ms)								//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void wait_IMU ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 ) / 16;
	for ( i = 0; i < time; i++) __nop();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUWriteByte											//
// 処理概要     指定したレジスタにデータを書き込む								//
// 引数         slaveaddr:スレーブアドレス reg:レジスタのアドレス data:書き込みデータ	//
// 戻り値       なし													//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte( char reg, char data )
{
	char sendData[2] = { reg, data }, num = 2;
    
	I2C_IMU_COMMAND;		// コマンド送信
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadByte							//
// 処理概要     指定したレジスタのデータを読む				//
// 引数         slaveaddr:スレーブアドレス reg:レジスタのアドレス	//
// 戻り値       0:正常に受信　1:受信不可						//
///////////////////////////////////////////////////////////////////////////
char IMUReadByte( char reg , char* reciveData )
{
	char sendData[1] = { reg }, num = 1;
  	
	if ( I2C_IMU_COMMAND == 0 ) {	// コマンド送信
		I2C_IMU_RECIVE;		// データ受信
		return 0;
	} else {
		return 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadArry																//
// 処理概要     指定したレジスタから指定の数だけデータを読む											//
// 引数         slaveaddr:スレーブアドレス addr:レジスタのアドレス num:読み取るデータ数 dataArry:データの格納先	//
// 戻り値       なし																		//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry( char reg, char num, char* reciveData )
{
	char sendData[1] = { reg };
	I2C_IMU_ARRY;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 init_IMU								//
// 処理概要     IMUの初期化を行う							//
// 引数         なし									//
// 戻り値       0: 初期化成功1:初期化失敗					//
///////////////////////////////////////////////////////////////////////////
char init_IMU (void)
{
	char ret  = 0, reciveData[1];
	if ( IMUReadByte( WHO_AM_I, reciveData) == 0 ) {
		if ( reciveData[0] == 0x71 || reciveData[0] == 0x73 ) {
			whoami = reciveData[0];
			wait_IMU(35);
			
			IMUWriteByte( PWR_MGMT_1, 0x00);	// スリープモード解除
			IMUWriteByte( INT_PIN_CFG, 0x02);	// 内蔵プルアップ無効化
			IMUWriteByte( CONFIG, 0x00);		// ローパスフィルタを使用しない
			IMUWriteByte( ACCEL_CONFIG, 0x18);	// レンジ±16gに変更
			IMUWriteByte( GYRO_CONFIG, 0x10);	// レンジ±1000deg/sに変更
		} else {
			ret = 1;
		}
	} else {
		ret = 1;
	}
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 IMUProcess								//
// 処理概要     1msごとに実行しセンサの値を読み取る			//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void IMUProcess (void)
{
	char 	axisData[14];	// 角加速度、温度の8bit分割データ格納先
	
	IMUReadArry( GYRO_XOUT_H, 6, axisData);	// 3軸加速度取得
	
	//8bitデータを16bitデータに変換
	// 温度
	//rawTemp = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
	
	// 角速度
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
	IMUReadArry( ACCEL_XOUT_H, 14, axisData);	// 3軸加速度取得
	
	//8bitデータを16bitデータに変換
	// 加速度
	rawXa = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
	rawYa = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
	rawZa = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
	
	// 温度
	rawTemp = (short)((axisData[6] << 8 & 0xff00 ) | axisData[7]);
	
	// 角速度
	rawXg = (short)((axisData[8] << 8 & 0xff00 ) | axisData[9]);
	rawYg = (short)((axisData[10] << 8 & 0xff00 ) | axisData[11]);
	rawZg = (short)((axisData[12] << 8 & 0xff00 ) | axisData[13]);
	*/
	
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 caribrateIMU							//
// 処理概要     1msごとに実行しセンサの値を読み取る			//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void caribrateIMU (void)
{
	char data1[14];
	short i, j, axisData[8];
	int atemp = 0, axg = 0, ayg = 0, azg = 0;
	double	xy, yy, zy;		// 角速度平均値
	double	xb, yb, zb;		// 近似直線の切片
	
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
// モジュール名 getTurningAngleIMU						//
// 処理概要   	IMUから旋回角度の計算					//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void getTurningAngleIMU(void)
{
	double angularVelocity_zg;
	
	angularVelocity_zg = (double)(rawZg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	TurningAngleIMU += (double)( angularVelocity_zg) * 0.001;
	
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getRollAngleIMU							//
// 処理概要   	IMUからロール角度の計算					//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void getRollAngleIMU(void)
{
	double angularVelocity_yg;
	
	angularVelocity_yg = (double)(rawXg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	RollAngleIMU -= (double)( angularVelocity_yg) * 0.001;
	
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getPichAngleIMU							//
// 処理概要     IMUからピッチ角度の計算						//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void getPichAngleIMU( void )
{
	double angularVelocity_xg;
	
	angularVelocity_xg = (double)(rawYg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	PichAngleIMU -= (double)( angularVelocity_xg) * 0.001;
	
}