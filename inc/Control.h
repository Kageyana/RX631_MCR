#ifndef CONTROL_H_
#define CONTROL_H_
//====================================//
// �C���N���[�h									//
//====================================//
#include "Rotaryencoder.h"
//====================================//
// �V���{����`									//
//====================================//
// �ً}��~
#define	STOPPING_METER			40		// ��~����

// �e�Z�N�V�����ł̖ڕW���x�@x/10[m/s]
#define SPEED_STRAIGHT				54	// �ʏ�g���[�X

#define SPEED_CROSSLINE			28	// �N���X���C���i�����x
#define SPEED_CLANK_TRACE			32	// �N�����N�i�����x
#define SPEED_RIGHTCLANK_CURVE		22	// �E�N�����N���񑬓x
#define SPEED_RIGHTCLANK_ESCAPE		40	// �E�N�����N���A���x
#define SPEED_LEFTCLANK_CURVE		22	// ���N�����N���񑬓x
#define SPEED_LEFTCLANK_ESCAPE		40	// ���N�����N���A���x

#define SPEED_HALFLINE				30	// �n�[�t���C���i�����x
#define SPEED_RIGHTCHANGE_TRACE	46	// �E���[���`�F���W�i�����x
#define SPEED_RIGHTCHANGE_CURVE	46	// �E���[���`�F���W���񑬓x
#define SPEED_RIGHTCHANGE_ESCAPE	46	// �E���[���`�F���W���A���x
#define SPEED_LEFTCHANGE_TRACE		46	// �����[���`�F���W�i�����x
#define SPEED_LEFTCHANGE_CURVE		46	// �����[���`�F���W���񑬓x
#define SPEED_LEFTCHANGE_ESCAPE		46	// �����[���`�F���W���A���x
// �p�x
#define ANGLE_RIGHTCLANK			-400	// �E�N�����N����p�x
#define ANGLE_LEFTCLANK			400	// ���N�����N����p�x
#define ANGLE_RIGHTCHANGE			-160	// �E���[���`�F���W����p�x
#define ANGLE_LEFTCHANGE			160	// �E���[���`�F���W����p�x

// PID�Q�C���֘A
//�����g���[�X
#define KP			11
#define KI			0
#define KD		44

// �p�x����
#define KP2		9
#define KI2		90
#define KD2		90

// ���x����
#define KP3		6
#define KI3		10
#define KD3		0

// �ً}��~�֘A
#define STOP_SENSOR1		60		// �Z���T�S��
#define STOP_SENSOR2		800		// �Z���T�S����
#define STOP_ENCODER		100		// �G���R�[�_��~(�Ђ�����Ԃ����H)
#define STOP_COUNT		10000	// ���Ԓ�~
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// �p�^�[���A���[�h�֘A
extern char 	pattern;			// �p�^�[���ԍ�
extern char	lcd_mode;			// LCD�\���I��
extern char	slope_mode;		// ��`�F�b�N		0:����n��	1:����I���	2:�����n��	3:�����I���
extern char	angle_mode;		// �T�[�{PWM�ύX	0:�����g���[�X	1:�p�x����
extern char	pushcart_mode;		// �艟�����[�h��	0:�������s	1:�艟��
extern char	msdset;			// MicroSD�����������ꂽ��	0:���������s	1:����������
extern char	IMUSet;			// IMU�����������ꂽ��	0: ���������s	1:����������

// �p�����[�^�֘A
// ����stopping_meter
extern short	stopping_meter;			// ��~����
// ���x
extern short	speed_straight;			// �ʏ�g���[�X

extern short	speed_crossline;			// �N���X���C���i�����x
extern short	speed_ckank_trace;		// �N�����N�i�����x
extern short	speed_rightclank_curve;	// �E�N�����N���񑬓x
extern short	speed_rightclank_escape;	// �E�N�����N���A���x
extern short	speed_leftclank_curve;	// ���N�����N���񑬓x
extern short	speed_leftclank_escape;	// ���N�����N���A���x

extern short	speed_halfine;			// �n�[�t���C���i�����x
extern short	speed_rightchange_trace;	// �E���[���`�F���W�i�����x
extern short	speed_rightchange_curve;	// �E���[���`�F���W���񑬓x
extern short	speed_rightchange_escape;// �E���[���`�F���W���A���x

extern short	speed_leftchange_trace;	// �����[���`�F���W�i�����x
extern short	speed_leftchange_curve;	// �����[���`�F���W���񑬓x
extern short	speed_leftchange_escape;	// �����[���`�F���W���񑬓x

// �T�[�{�p�x
extern short	angle_rightclank;		// �E�N�����N����p�x
extern short	angle_leftclank;			// ���N�����N����p�x
extern short	angle_rightchange;		// �E���[���`�F���W����p�x
extern short	angle_leftchange;		// �E���[���`�F���W����p�x

// ���[�^�[�֘A
extern signed char 	motorPwm;	// ���[�^�[����PWM
extern short		targetSpeed;	// �ڕW���x

// �Q�C���֘A
extern char	kp_buff, ki_buff, kd_buff;
extern char	kp2_buff, ki2_buff, kd2_buff;
extern char 	kp3_buff, ki3_buff, kd3_buff;

// �f���֘A
extern char demo;

// �T�[�{�֘A
extern double		Int;			// I�����ώZ�l(�����g���[�X)
extern short 		SetAngle;		// �ڕW�p�x
extern signed char 	ServoPwm;	// �����g���[�X�T�[�{PWM
extern signed char 	ServoPwm2;	// �p�x�T�[�{PWM

//====================================//
// �v���g�^�C�v�錾									//
//====================================//
// �p�����[�^�֘A
void initParameter(void);

// �}�[�J�[�֘A
signed char check_crossline( void );
signed char check_rightline( void );
signed char check_leftline( void );

// ���[�^�[�֘A
void motorControl( void );

// �T�[�{�֘A
void servoControl( void );
void servoControl2( void );

#endif // CONTROL_H_