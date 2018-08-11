#ifndef PERIPHERALFUNCTIONS_H_
#define PERIPHERALFUNCTIONS_H_
//======================================//
// シンボル定義                         //
//======================================//

#define	SERVO_CENTER		2028		// サーボセンターのAD値
#define	SERVO_LIMIT		430		// サーボリミットAD値±
#define TGR_MOTOR		4798		// ジェネラルレジスタ初期値(駆動モータ)
#define TGR_SERVO		2878		// ジェネラルレジスタ初期値(サーボ)
#define PALSE_METER		24950L		// 1mのパルス
#define PALSE_MILLIMETER	25		// 1mmのパルス
#define SPEED_CURRENT		25		// 1m/sの時　1msのパルス
#define SPEED_CURRENT_DETAIL	25.465		// 1m/sの時　1msのパルス

//======================================//
// グローバル変数の宣言                 //
//======================================//
// タイマ関連
extern unsigned int	cnt0;		// 関数用タイマカウント
extern unsigned short	cnt_flash;	// フラッシュ用カウント
extern unsigned int	cnt_log;	// フラッシュ用カウント
extern short		cnt_swR;	// スイッチ長押し判定用右
extern short		cnt_swL;	// スイッチ長押し判定用左

// センサ関連
extern short		sensorR;	// 右アナログセンサ
extern short		sensorL;	// 左アナログセンサ
extern short		sensorG;	// 左アナログセンサ
extern short		sensorC;	// 中心アナログセンサ
extern short		sensorLL;	// 最左端アナログセンサ
extern short		sensorRR;	// 最右端アナログセンサ

extern short		Angle0;		// サーボセンター値

// エンコーダ関連
extern unsigned int	EncoderTotal;	// 総走行距離
extern signed short	Encoder;	// 1msごとのパルス
extern unsigned int	enc1;		// 走行用距離カウント
extern unsigned int	enc_slope;	// 坂上距離カウント

// モーター関連
extern signed char	accele_fR;	// 右前モーターPWM値
extern signed char	accele_fL;	// 左前モーターPWM値
extern signed char	accele_rR;	// 右後モーターPWM値
extern signed char	accele_rL;	// 左後モーターPWM値
extern signed char	sPwm;		// サーボモーターPWM値

//======================================//
// プロトタイプ宣言                     //
//======================================//
// タイマ関連
void delay( unsigned short delaytime );
// LED関連
void led_out( unsigned char led );

// スイッチ関連
unsigned char tasw_get ( void );
unsigned char dipsw_get( void );

// センサ関連
short getGyro( void );
short getServoAngle(void);
short getAnalogSensor( void );
unsigned char sensor_inp( void );
unsigned char startbar_get( void );

//ブザー関連
void init_BeepS( void );
void setBeepPatternS( unsigned short Beep );
void beepProcessS( void );

// モーター関連
void motor_f( signed char accelefL, signed char accelefR );
void motor_r( signed char accelerL, signed char accelerR );

// サーボ関連
void servoPwmOut( signed char pwm );

#endif // PERIPHERALFUNCTIONS_H_