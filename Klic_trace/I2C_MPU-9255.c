//======================================//
// インクルード                         //
//======================================//
#include "I2C_MPU-9255.h"
//======================================//
// グローバル変数の宣言                 //
//======================================//
short	cent_data[6] = {0,0,0,0,0,0};	// オフセット値	
volatile short 	rawXa, rawYa, rawZa;	// 加速度(16bitデータ)
volatile short 	rawXg, rawYg, rawZg;	// 角加速度(16bitデータ)
volatile short 	rawTemp;		// 温度(16bitデータ)

char	pattern_caribrateIMU = 0;// キャリブレーション処理
short	xg_sample[SAMPLENUMBER], yg_sample[SAMPLENUMBER], zg_sample[SAMPLENUMBER];
short	xg_sample[SAMPLENUMBER], yg_sample[SAMPLENUMBER], zg_sample[SAMPLENUMBER];
short	data_cnt = 0;
short 	median, mode;
int	average;
char	caribration;		// 0:キャリブレーション停止 1:キャリブレーション中

char	IMUset = 0;		// 0:初期化失敗		1:初期化完了
char	whoami;

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
    
	I2C_IMU_COMMAND;	// コマンド送信
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadByte						//
// 処理概要     指定したレジスタのデータを読む				//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス	//
// 戻り値       0:正常に受信　1:受信不可				//
//////////////////////////////////////////////////////////////////////////
char IMUReadByte(char slaveAddr, char reg , char* reciveData )
{
	char sendData[1] = { reg }, num = 1;
    
	if ( I2C_IMU_DATA == 0 ) {		// コマンド送信
		I2C_IMU_RECIVE;		// データ受信
		return 0;
	} else {
		return 1;
	}
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
	I2C_IMU_ARRY;
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 init_IMU						//
// 処理概要     IMUの初期化を行う					//
// 引数         なし							//
// 戻り値       0: 初期化成功1:初期化失敗				//
//////////////////////////////////////////////////////////////////////////
char init_IMU (void)
{
	char ret  = 0, reciveData[1];
	
	if ( IMUReadByte(MPU9255_ADDRESS, WHO_AM_I, reciveData) == 0 ) {
		if ( reciveData[0] == 0x71 || reciveData[0] == 0x73 ) {
			whoami = reciveData[0];
			wait_IMU(35);
			
			IMUWriteByte(MPU9255_ADDRESS, PWR_MGMT_1, 0x00);	// スリープモード解除
			IMUWriteByte(MPU9255_ADDRESS, INT_PIN_CFG, 0x02);	// 内蔵プルアップ無効化
			IMUWriteByte(MPU9255_ADDRESS, CONFIG, 0x00);		// ローパスフィルタを使用しない
			IMUWriteByte(MPU9255_ADDRESS, ACCEL_CONFIG, 0x18);	// レンジ±16gに変更
			IMUWriteByte(MPU9255_ADDRESS, GYRO_CONFIG, 0x10);	// レンジ±1000deg/sに変更
		} else {
			ret = 1;
		}
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
	char 	axisData[8];	// 角加速度、温度の8bit分割データ格納先
	
	//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, axisAccelData);	// 3軸加速度取得
	IMUReadArry(MPU9255_ADDRESS, TEMP_OUT_H, 8, axisData);	// 3軸角加速度、温度取得
	
	//8bitデータを16bitデータに変換
	// 加速度
	/*
	rawXa = (short)((axisAccelData[0] << 8 & 0xff00 ) | axisAccelData[1]);
	rawYa = (short)((axisAccelData[2] << 8 & 0xff00 ) | axisAccelData[3]);
	rawZa = (short)((axisAccelData[4] << 8 & 0xff00 ) | axisAccelData[5]);
	*/
	// 温度
	rawTemp = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
	
	// 角速度
	rawXg = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
	rawYg = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
	rawZg = (short)((axisData[6] << 8 & 0xff00 ) | axisData[7]);
	
	//rawXa -= cent_data[0];
	//rawYa -= cent_data[1];
	//rawZa -= cent_data[2];
	//rawXg -= cent_data[3];
	//rawYg -= cent_data[4];
	rawZg -= cent_data[5];

}
//////////////////////////////////////////////////////////////////////////
// モジュール名 caribrateIMU						//
// 処理概要     1msごとに実行しセンサの値を読み取る			//
// 引数         なし							//
// 戻り値       なし 							//
//////////////////////////////////////////////////////////////////////////
void caribrateIMU (char data_Option)
{
	char g[6];
	short i, j, datasize;
	short cnt, temp_cnt = 0;
	
	switch( pattern_caribrateIMU ) {
		case 0:
			if ( caribration ) {
				IMUSet = 0;	// キャリブレーション開始
				// 初期化
				data_cnt = 0;
				average = 0;
				mode = 0;
				median = 0;
				
				pattern_caribrateIMU = 1;
			}
			break;
			
		case 1:
			PORT5.PODR.BIT.B5 = 1;
			//IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 6, a);	// 3軸加速度取得
			IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, g);	// 3軸角加速度取得
			
			// 加速度
			//xa[i] = (short)((a[0] << 8 ) | a[1]);
			//ya[i] = (short)((a[2] << 8 ) | a[3]);
			//za[i] = (short)((a[4] << 8 ) | a[5]);
			
			// 角速度
			xg_sample[data_cnt] = (short)((g[0] << 8 & 0xff00 ) | g[1]);
			yg_sample[data_cnt] = (short)((g[2] << 8 & 0xff00 ) | g[3]);
			zg_sample[data_cnt] = (short)((g[4] << 8 & 0xff00 ) | g[5]);
			
			data_cnt++;
			if ( data_cnt >= SAMPLENUMBER ) {
				PORT5.PODR.BIT.B5 = 0;
				data_cnt = sizeof(xg_sample) / sizeof(xg_sample[0]);	// データ総数計算
				datasize = sizeof(xg_sample[0]);		// データサイズ計算 
				
				if( data_Option == AVERAGE )	pattern_caribrateIMU = AVERAGE * 10;
				if( data_Option == MODE )	pattern_caribrateIMU = MODE * 10;
				if( data_Option == MEDIAN )	pattern_caribrateIMU = MEDIAN  *10;
			}
			break;
			
		case AVERAGE * 10 + 0:
			for ( i = 0; i < data_cnt; i++ ) average += zg_sample[i];
			average /= data_cnt;
			cent_data[5] = average;
			
			pattern_caribrateIMU = 40;
			break;
			
		case MODE * 10 + 0:
			// 初期値
			mode = 0;
			cnt = 0;
			for ( i = 0;i < data_cnt; i++ ) {
				temp_cnt = 1;
				for ( j = i+1;j < data_cnt; j++ ) {
					if ( zg_sample[j] == zg_sample[i] ) temp_cnt++;
				}
				
				if ( temp_cnt > cnt ) {
					cnt = temp_cnt;
					mode = zg_sample[i];
				}
			}
			cent_data[5] = mode;
			
			pattern_caribrateIMU = 40;
			break;
			
		case MEDIAN * 10 + 0:
			qsort( (void*)zg_sample, data_cnt, datasize, short_sort );
			median = zg_sample[1000];
			cent_data[5] = median;
			
			pattern_caribrateIMU = 40;
			break;
		
		case 40:
			IMUSet = 1;
			caribration = 0;
			setBeepPatternS( 0xa300 );
			
			pattern_caribrateIMU = 0;
			break;
		
	}
}