//====================================//
// インクルード									//
//====================================//
#include "I2C_MPU-9255.h"
//====================================//
// グローバル変数の宣言             						//
//====================================//
short	cent_data[7] = {0};	// オフセット値	
volatile short 	rawXa, rawYa, rawZa;	// 加速度(16bitデータ)
volatile short 	rawXg, rawYg, rawZg;	// 角加速度(16bitデータ)
volatile short 	rawTemp;		// 温度(16bitデータ)

char	pattern_caribrateIMU = 0;// キャリブレーション処理
short	sampleIMU[7][SAMPLENUMBER];
short	data_cnt = 0;
short 	median[7], mode[7];
int	averageIMU[7];
char	caribration;		// 0:キャリブレーション停止 1:キャリブレーション中

char	IMUset = 0;		// 0:初期化失敗		1:初期化完了
char	whoami;

///////////////////////////////////////////////////////////////////////////
// モジュール名 wait_IMU								//
// 処理概要     遅延処理								//
// 引数         遅延時間(ms)								//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void wait_IMU ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 )/ 16;
	for ( i = 0; i < time; i++) __nop();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUWriteByte											//
// 処理概要     指定したレジスタにデータを書き込む								//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス data:書き込みデータ	//
// 戻り値       なし													//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IMUWriteByte(char slaveAddr, char reg, char data )
{
	char sendData[2] = { reg, data };
    
	I2C_IMU_COMMAND;	// コマンド送信
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadByte							//
// 処理概要     指定したレジスタのデータを読む				//
// 引数         slaveAddr:スレーブアドレス reg:レジスタのアドレス	//
// 戻り値       0:正常に受信　1:受信不可						//
///////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// モジュール名 IMUReadArry																//
// 処理概要     指定したレジスタから指定の数だけデータを読む											//
// 引数         slaveAddr:スレーブアドレス addr:レジスタのアドレス num:読み取るデータ数 dataArry:データの格納先	//
// 戻り値       なし																		//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMUReadArry(char slaveAddr, char reg, char num, char* reciveData )
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
///////////////////////////////////////////////////////////////////////////
// モジュール名 IMUProcess								//
// 処理概要     1msごとに実行しセンサの値を読み取る			//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void IMUProcess (void)
{
	char 	axisData[14];	// 角加速度、温度の8bit分割データ格納先
	
	IMUReadArry(MPU9255_ADDRESS, GYRO_XOUT_H, 6, axisData);	// 3軸加速度取得
	
	//8bitデータを16bitデータに変換
	// 加速度
	//rawXa = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
	//rawYa = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
	//rawZa = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
	
	// 温度
	//rawTemp = (short)((axisData[6] << 8 & 0xff00 ) | axisData[7]);
	
	// 角速度
	rawXg = (short)((axisData[0] << 8 & 0xff00 ) | axisData[1]);
	rawYg = (short)((axisData[2] << 8 & 0xff00 ) | axisData[3]);
	rawZg = (short)((axisData[4] << 8 & 0xff00 ) | axisData[5]);
	
	rawXa -= cent_data[0];
	rawYa -= cent_data[1];
	rawZa -= cent_data[2];
	rawXg -= cent_data[4];
	rawYg -= cent_data[5];
	rawZg -= cent_data[6];

}
///////////////////////////////////////////////////////////////////////////
// モジュール名 caribrateIMU							//
// 処理概要     1msごとに実行しセンサの値を読み取る			//
// 引数         なし									//
// 戻り値       なし									//
///////////////////////////////////////////////////////////////////////////
void caribrateIMU (char data_Option)
{
	char data[6];
	short i, j, k, datasize;
	short cnt, temp_cnt = 0;
	
	switch( pattern_caribrateIMU ) {
		case 0:
			if ( caribration ) {
				IMUSet = 0;	// キャリブレーション開始
				// 初期化
				data_cnt = 0;
				for ( i = 0; i < 7; i++ ) {
					averageIMU[i] = 0;
					mode[i] = 0;
					median[i] = 0;
				}
				
				pattern_caribrateIMU = 1;
			}
			break;
			
		case 1:
			PORT5.PODR.BIT.B5 = 1;
			IMUReadArry(MPU9255_ADDRESS, ACCEL_XOUT_H, 14, data);	// 6軸データ+温度取得
			
			// 16bitデータに変換
			for ( i = 0; i < 7; i++ ) {
				sampleIMU[i][data_cnt] = (short)((data[i * 2] << 8 & 0xff00 ) | data[i * 2 + 1]);
			}
			data_cnt++;
			if ( data_cnt >= SAMPLENUMBER ) {
				PORT5.PODR.BIT.B5 = 0;
				data_cnt = SAMPLENUMBER;	// データ総数計算
				datasize = 2;		// データサイズ計算 
				
				if( data_Option == AVERAGE )	pattern_caribrateIMU = AVERAGE;
				if( data_Option == MODE )	pattern_caribrateIMU = MODE;
				if( data_Option == MEDIAN )	pattern_caribrateIMU = MEDIAN;
			}
			break;
			
		case AVERAGE + 0:
			for ( i = 0; i < data_cnt; i++ ) {
				for ( j = 0; j < 7; j++ ) averageIMU[j] += sampleIMU[j][i];
			}
			for ( i = 0; i < 7; i++ ) {
				averageIMU[i] /= data_cnt;
				cent_data[i] = averageIMU[i];
			}
			
			pattern_caribrateIMU = 40;
			break;
			
		case MODE + 0:
			for ( i = 0; i < 7; i++ ) {
				// 初期値
				mode[i] = 0;
				cnt = 0;
				for ( j = 0;j < data_cnt; j++ ) {
					temp_cnt = 1;
					for ( k = j+1;k < data_cnt; k++ ) {
						if ( sampleIMU[i][j] == sampleIMU[i][k] ) temp_cnt++;
					}
					
					if ( temp_cnt > cnt ) {
						cnt = temp_cnt;
						mode[i] = sampleIMU[i][j];
					}
				}
				cent_data[i] = mode[i];
			}
			
			pattern_caribrateIMU = 40;
			break;
			
		case MEDIAN + 0:
			for ( i = 0; i < 7; i++ ) {
				qsort( (void*)sampleIMU[i], data_cnt, datasize, short_sort );
				median[i] = sampleIMU[i][data_cnt/2];
				cent_data[i] = median[i];
			}
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