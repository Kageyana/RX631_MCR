//====================================//
// インクルー
//====================================//
#include "I2C_MPU-9255.h"
//====================================//
// グローバル変数の宣
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

char	whoami;
char cnt_imu = 0;

///////////////////////////////////////////////////////////////////////////
// モジュール名 wait_IM
// 処理概要     遅延処
// 引数         遅延時間(ms
// 戻り値       な
///////////////////////////////////////////////////////////////////////////
void wait_IMU ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 ) / 16;
	for ( i = 0; i < time; i++) __nop();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUWriteByt
// 処理概要     指定したレジスタにデータを書き込
// 引数         slaveaddr:スレーブアドレス reg:レジスタのアドレス data:書き込みデー
// 戻り値       な
///////////////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte( char reg, char data )
{
	char sendData[2] = { reg, data }, num = 2;
    
	I2C_IMU_COMMAND;		// コマンド送信
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadByt
// 処理概要     指定したレジスタのデータを読
// 引数         slaveaddr:スレーブアドレス reg:レジスタのアドレ
// 戻り値       0:正常に受信　1:受信不
///////////////////////////////////////////////////////////////////////////
char IMUReadByte( char reg , char* reciveData )
{
	char sendData[1] = { reg }, num = 1;
  	
	if ( !I2C_IMU_COMMAND ) {	// コマンド送信
		I2C_IMU_RECIVE;		// データ受信
		return 0;
	} else {
		return 1;
	}
}
/////////////////////////////////////////////////////////////////
// モジュール名 IMUReadArr
// 処理概要     指定したレジスタから指定の数だけデータを読
// 引数         slaveaddr:スレーブアドレス addr:レジスタのアドレス 
//				num:読み取るデータ数 dataArry:データの格納
// 戻り値       0:正常に通信	1:通信エラ
//////////////////////////////////////////////////////////////////
bool IMUReadArry( char reg, char num, char* reciveData )
{
	char sendData[1] = { reg };
	if ( !I2C_IMU_ARRY ) return false;
	else	return true;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 init_IM
// 処理概要     IMUの初期化を行
// 引数         な
// 戻り値       0: 初期化成功1:初期化失
///////////////////////////////////////////////////////////////////////////
char init_IMU (void)
{
	char ret  = 0, reciveData[1], i, j = 0;
	for ( i = 0; i < 9; i++ ) {
		if ( !IMUReadByte( WHO_AM_I, reciveData) ) {
			j = 1;
			break;
		}
	}
	if ( j ) {
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
// モジュール名 IMUProces
// 処理概要     1msごとに実行しセンサの値を読み取
// 引数         な
// 戻り値       な
///////////////////////////////////////////////////////////////////////////
void IMUProcess (void)
{
	char 	axisData[14];	// 角加速度、温度の8bit分割データ格納先
	
	if ( !IMUReadArry( GYRO_XOUT_H, 6, axisData) ) {	// 3軸加速度取得
		//8bitデータを16bitデータに変換
		// 角速度
		rawXg = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
		rawYg = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
		rawZg = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
		
		//rawXg &= 0xff80;
		//rawYg &= 0xff80;
		//rawZg &= 0xff80;
		
		if ( rawXg < 0 ) rawXg += offset[0];
		else			rawXg -= offset[0];
		if ( rawYg < 0 ) rawYg += offset[1];
		else			rawYg -= offset[1];
		if ( rawZg < 0 ) rawZg += offset[2];
		else			rawZg -= offset[2];
		/*
		IMUReadArry( ACCEL_XOUT_H, 14, axisData);	// 6軸加速度取得
		
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
	} else {
		setBeepPatternS( 0x8000 );
		init_SCI1( UART, RATE_230400 );
		IMUSet = 0;
	}
	
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 caribrateIM
// 処理概要     1msごとに実行しセンサの値を読み取
// 引数         な
// 戻り値       な
///////////////////////////////////////////////////////////////////////////
void caribrateIMU (void)
{
	char data1[14];
	short i, j, axisData[8];
	int axg = 0, ayg = 0, azg = 0;
	
	IMUSet = 0;
	for ( i = 0; i < SAMPLE; i++ ) {
		IMUReadArry( GYRO_XOUT_H, 6, data1);
		for ( j = 0; j < 4; j++ ) {
			axisData[j] = (short)((data1[j * 2] << 8 & 0xff00 ) | data1[j * 2 + 1]);
			axisData[j] = axisData[j] & 0xfff8;		// 下位3bitを切り捨て
		}
		
		axg += axisData[0];
		ayg += axisData[1];
		azg += axisData[2];
	}
	//　平均値を算出
	axg /= SAMPLE;
	ayg /= SAMPLE;
	azg /= SAMPLE;
	
	offset[0] = axg / SAMPLE;
	offset[1] = ayg / SAMPLE;
	offset[2] = azg / SAMPLE;
	
	if ( offset[0] < 0 ) offset[0] = -offset[0];
	if ( offset[1] < 0 ) offset[1] = -offset[1];
	if ( offset[2] < 0 ) offset[2] = -offset[2];
	
	TurningAngleIMU = 0;
	RollAngleIMU = 0;
	PichAngleIMU = 0;
	
	IMUSet = 1;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getTurningAngleIM
// 処理概要   	IMUから旋回角度の計
// 引数         な
// 戻り値       な
///////////////////////////////////////////////////////////////////////////
void getTurningAngleIMU(void)
{
	double angularVelocity_zg;
	
	angularVelocity_zg = (double)(rawZg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	TurningAngleIMU += (double)( angularVelocity_zg) * DELTATIMU;
	
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getRollAngleIM
// 処理概要   	IMUからロール角度の計
// 引数         な
// 戻り値       な
///////////////////////////////////////////////////////////////////////////
void getRollAngleIMU(void)
{
	double angularVelocity_yg;
	
	angularVelocity_yg = (double)(rawXg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	RollAngleIMU -= (double)( angularVelocity_yg) * DELTATIMU;
	
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getPichAngleIMU
// 処理概要     IMUからピッチ角度の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void getPichAngleIMU( void )
{
	double angularVelocity_xg;
	
	angularVelocity_xg = (double)(rawYg) / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	PichAngleIMU -= (double)( angularVelocity_xg) * DELTATIMU;
	
}