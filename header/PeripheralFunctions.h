#ifndef PERIPHERALFUNCTIONS_H_
#define PERIPHERALFUNCTIONS_H_
//======================================//
// �V���{����`                         //
//======================================//

#define	SERVO_CENTER		2028		// �T�[�{�Z���^�[��AD�l
#define	SERVO_LIMIT		430		// �T�[�{���~�b�gAD�l�}
#define TGR_MOTOR		4798		// �W�F�l�������W�X�^�����l(�쓮���[�^)
#define TGR_SERVO		2878		// �W�F�l�������W�X�^�����l(�T�[�{)
#define PALSE_METER		24950L		// 1m�̃p���X
#define PALSE_MILLIMETER	25		// 1mm�̃p���X
#define SPEED_CURRENT		25		// 1m/s�̎��@1ms�̃p���X
#define SPEED_CURRENT_DETAIL	25.465		// 1m/s�̎��@1ms�̃p���X

//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// �^�C�}�֘A
extern unsigned int	cnt0;		// �֐��p�^�C�}�J�E���g
extern unsigned short	cnt_flash;	// �t���b�V���p�J�E���g
extern unsigned int	cnt_log;	// �t���b�V���p�J�E���g
extern short		cnt_swR;	// �X�C�b�`����������p�E
extern short		cnt_swL;	// �X�C�b�`����������p��

// �Z���T�֘A
extern short		sensorR;	// �E�A�i���O�Z���T
extern short		sensorL;	// ���A�i���O�Z���T
extern short		sensorG;	// ���A�i���O�Z���T
extern short		sensorC;	// ���S�A�i���O�Z���T
extern short		sensorLL;	// �ō��[�A�i���O�Z���T
extern short		sensorRR;	// �ŉE�[�A�i���O�Z���T

extern short		Angle0;		// �T�[�{�Z���^�[�l

// �G���R�[�_�֘A
extern unsigned int	EncoderTotal;	// �����s����
extern signed short	Encoder;	// 1ms���Ƃ̃p���X
extern unsigned int	enc1;		// ���s�p�����J�E���g
extern unsigned int	enc_slope;	// ��㋗���J�E���g

// ���[�^�[�֘A
extern signed char	accele_fR;	// �E�O���[�^�[PWM�l
extern signed char	accele_fL;	// ���O���[�^�[PWM�l
extern signed char	accele_rR;	// �E�ヂ�[�^�[PWM�l
extern signed char	accele_rL;	// ���ヂ�[�^�[PWM�l
extern signed char	sPwm;		// �T�[�{���[�^�[PWM�l

//======================================//
// �v���g�^�C�v�錾                     //
//======================================//
// �^�C�}�֘A
void delay( unsigned short delaytime );
// LED�֘A
void led_out( unsigned char led );

// �X�C�b�`�֘A
unsigned char tasw_get ( void );
unsigned char dipsw_get( void );

// �Z���T�֘A
short getGyro( void );
short getServoAngle(void);
short getAnalogSensor( void );
unsigned char sensor_inp( void );
unsigned char startbar_get( void );

//�u�U�[�֘A
void init_BeepS( void );
void setBeepPatternS( unsigned short Beep );
void beepProcessS( void );

// ���[�^�[�֘A
void motor_f( signed char accelefL, signed char accelefR );
void motor_r( signed char accelerL, signed char accelerR );

// �T�[�{�֘A
void servoPwmOut( signed char pwm );

#endif // PERIPHERALFUNCTIONS_H_