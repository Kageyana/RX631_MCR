#ifndef LINECHASE_H_
#define LINECHASE_H_
//======================================//
// �V���{����`                         //
//======================================//
// �ً}��~
#define	STOPPING_METER		23		// ��~����

// �e�Z�N�V�����ł̖ڕW���x�@x/10[m/s]
#define SPEED_STRAIGHT			44	// �ʏ�g���[�X
#define SPEED_CURVE_BRAKE		38	// �J�[�u�u���[�L
#define SPEED_CURVE_R600		38	// R600�J�[�u���x
#define SPEED_CURVE_R450		34	// R450�J�[�u���x
#define SPEED_CURVE_STRAIGHT		38	// S���J�[�u�������x

#define SPEED_CROSSLINE			12	// �N���X���C���i�����x
#define SPEED_CLANK_TRACE		30	// �N�����N�i�����x
#define SPEED_RIGHTCLANK_CURVE		22	// �E�N�����N���񑬓x
#define SPEED_RIGHTCLANK_ESCAPE		36	// �E�N�����N���A���x
#define SPEED_LEFTCLANK_CURVE		22	// ���N�����N���񑬓x
#define SPEED_LEFTCLANK_ESCAPE		36	// ���N�����N���A���x

#define SPEED_HALFLINE			30	// �n�[�t���C���i�����x
#define SPEED_RIGHTCHANGE_TRACE		42	// �E���[���`�F���W�i�����x
#define SPEED_RIGHTCHANGE_CURVE		42	// �E���[���`�F���W���񑬓x
#define SPEED_RIGHTCHANGE_ESCAPE	42	// �E���[���`�F���W���A���x
#define SPEED_LEFTCHANGE_TRACE		42	// �����[���`�F���W�i�����x
#define SPEED_LEFTCHANGE_CURVE		42	// �����[���`�F���W���񑬓x
#define SPEED_LEFTCHANGE_ESCAPE		42	// �����[���`�F���W���A���x

#define SPEED_SLOPE_BRAKE		20	// �����I�_���x
#define SPEED_SLOPE_TRACE		36	// ��ǂݔ�΂����x
// �p�x
#define ANGLE_RIGHTCLANK		-420	// �E�N�����N����p�x
#define ANGLE_LEFTCLANK			420	// ���N�����N����p�x
#define ANGLE_RIGHTCHANGE		-160	// �E���[���`�F���W����p�x
#define ANGLE_LEFTCHANGE		160	// �E���[���`�F���W����p�x

// �J�[�u�֘A
#define CURVE_R600_START	40		// R600�J�nAD�l
#define CURVE_R450_START	140		// R450�J�nAD�l

// �W���C���֘A
#define AD_3V3VOLTAGE		0.806		// 3V����1AD�l������̓d��[mV]
#define AD_5VOLTAGE		1.22		// 5V����1AD�l������̓d��[mV]
#define GYROVOLTAGE		0.67		// �d�����p�����x[mV/deg/s]
#define SLOPEUPPERLINE		17		// ���⌟�o�p�x
#define SLOPELOWERLINE		-14		// ����⌟�o�p�x

#define PI			3.141592	// �~����
#define RIGHTCURVE_ENCODER	78.5		// �E�֒��S����G���R�[�_�[�̒��S�܂ł̋���
#define LEFTCURVE_ENCODER	74.5		// ���֒��S����G���R�[�_�[�̒��S�܂ł̋���

// PID�Q�C���֘A
//�����g���[�X
#define KP			20//30
#define KI			2//41
#define KD			58//27

// �p�x����
#define KP2			9
#define KI2			90
#define KD2			90

// ���x����
#define KP3			6
#define KI3			20
#define KD3			0

// �ً}��~�֘A
#define STOP_SENSOR1		60		// �Z���T�S��
#define STOP_SENSOR2		800		// �Z���T�S����
#define STOP_ENCODER		10		// �G���R�[�_��~(�Ђ�����Ԃ����H)
#define STOP_GYRO		30		// �}�C�i�X�̉����x���m(�R�[�X���痎�����H)
#define STOP_COUNT		10000		// ���Ԓ�~
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// �p�^�[���A���[�h�֘A
extern char 	pattern;			// �p�^�[���ԍ�
extern char	lcd_mode;			// LCD�\���I��
extern char	slope_mode;			// ��`�F�b�N		0:����n��	1:����I���	2:�����n��	3:�����I���
extern char	angle_mode;
extern char	pushcart_mode;			// �艟�����[�h
extern char	msdset;				// MicroSD�����������ꂽ��
extern char	IMUSet;				// IMU�����������ꂽ��	0: ���������s	1:����������


// �p�����[�^�֘A
// ����
extern short	stopping_meter;			// ��~����
// ���x
extern short	speed_straight;			// �ʏ�g���[�X
extern short	speed_curve_brake;		// �J�[�u�u���[�L
extern short	speed_curve_r600;		// R600�J�[�u���x
extern short	speed_curve_r450;		// R450�J�[�u���x
extern short	speed_curve_straight;		// S���J�[�u�������x

extern short	speed_crossline;		// �N���X���C���i�����x
extern short	speed_ckank_trace;		// �N�����N�i�����x
extern short	speed_rightclank_curve;		// �E�N�����N���񑬓x
extern short	speed_rightclank_escape;	// �E�N�����N���A���x
extern short	speed_leftclank_curve;		// ���N�����N���񑬓x
extern short	speed_leftclank_escape;		// ���N�����N���A���x

extern short	speed_halfine;			// �n�[�t���C���i�����x
extern short	speed_rightchange_trace;	// �E���[���`�F���W�i�����x
extern short	speed_rightchange_curve;	// �E���[���`�F���W���񑬓x
extern short	speed_rightchange_escape;	// �E���[���`�F���W���A���x

extern short	speed_leftchange_trace;		// �����[���`�F���W�i�����x
extern short	speed_leftchange_curve;		// �����[���`�F���W���񑬓x
extern short	speed_leftchange_escape;	// �����[���`�F���W���񑬓x

extern short	speed_slope_brake;		// �����I�_���x
extern short	speed_slope_trace;		// ��ǂݔ�΂����x

// �p�x
extern short	angle_rightclank;		// �E�N�����N����p�x
extern short	angle_leftclank;		// ���N�����N����p�x
extern short	angle_rightchange;		// �E���[���`�F���W����p�x
extern short	angle_leftchange;		// �E���[���`�F���W����p�x

// �^�C�}�֘A
extern unsigned int	cnt1;		// ���s�p�^�C�}�J�E���g
extern unsigned int	cnt_log;	// ���s�p�^�C�}�J�E���g
extern unsigned short	cnt_out;	// �R�[�X�A�E�g����p�^�C�}�J�E���g
extern unsigned short	cnt_out2;	// �R�[�X�A�E�g����p�^�C�}�J�E���g2
extern unsigned short	cnt_out3;	// �R�[�X�A�E�g����p�^�C�}�J�E���g3
extern unsigned short	cnt_out4;	// �R�[�X�A�E�g����p�^�C�}�J�E���g4
extern unsigned short 	cnt_gyro;	// �p�x�v�Z�p�^�C�}�J�E���g

// �p�x�֘A
extern short 		Degrees;	// �W���C���Z���T����v�Z�����p�x(degrees)
extern double		integral_rad;	// �p���x�ώZ�l
extern double		angularVelocity;// ���_�p���x[rad/s]
extern short 		TurningAngle;	// ����p�x

// ���[�^�[�֘A
extern signed char 	motorPwm;	// ���[�^�[����PWM
extern short		target_speed;	// �ڕW���x

// �Q�C���֘A
extern signed char	kp_buff, ki_buff, kd_buff;
extern signed char	kp2_buff, ki2_buff, kd2_buff;
extern signed char 	kp3_buff, ki3_buff, kd3_buff;

// �f���֘A
extern char demo;

// �T�[�{�֘A
extern short 		SetAngle;	// �ڕW�p�x
extern signed char 	ServoPwm;	// �����g���[�X�T�[�{PWM
extern signed char 	ServoPwm2;	// �p�x�T�[�{PWM

//======================================//
// �v���g�^�C�v�錾                     //
//======================================//
// �}�[�J�[�֘A
signed char check_crossline( void );
signed char check_rightline( void );
signed char check_leftline( void );
signed char check_slope( void );

// �p�x�֘A
void get_degrees( void );
void get_TurningAngle(void);

// �G���R�[�_�֘A
unsigned int enc_mm( short mm );

// ���[�^�[�֘A
void motorControl( void );

// ���֍��֘A
void diff ( signed char pwm );

// �T�[�{�֘A
void servoControl( void );
void servoControl2( void );

#endif // LINECHASE_H_