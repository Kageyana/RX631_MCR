//======================================//
// �C���N���[�h                         //
//======================================//
#include "SetUp.h"
#include "R_PG_RX631_mcr_ver3.0.h"
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "E2dataFlash.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "SCI.h"
#include "I2C_MPU-9255.h"
#include <stdio.h>
//======================================//
// �V���{����`                         //
//======================================//
#define RIGHT	0
#define LEFT	1
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// �p�^�[���֘A
char 	start;

// ���[�h�֘A
char 	pushcart_mode;	// �艟�����[�h��	0:�������s	1:�艟��

// �p�����[�^�֘A
// ����
short	stopping_meter;			// ��~����
// ���x
short	speed_straight;			// �ʏ�g���[�X
short	speed_curve_brake;		// �J�[�u�u���[�L
short	speed_curve_r600;		// R600�J�[�u���x
short	speed_curve_r450;		// R450�J�[�u���x
short	speed_curve_straight;		// S���J�[�u�������x

short	speed_crossline;		// �N���X���C���i�����x
short	speed_ckank_trace;		// �N�����N�i�����x
short	speed_rightclank_curve;		// �E�N�����N���񑬓x
short	speed_rightclank_escape;	// �E�N�����N���A���x
short	speed_leftclank_curve;		// ���N�����N���񑬓x
short	speed_leftclank_escape;		// ���N�����N���A���x

short	speed_halfine;			// �n�[�t���C���i�����x
short	speed_rightchange_trace;	// �E���[���`�F���W�i�����x
short	speed_rightchange_curve;	// �E���[���`�F���W���񑬓x
short	speed_rightchange_escape;	// �E���[���`�F���W���A���x

short	speed_leftchange_trace;		// �����[���`�F���W�i�����x
short	speed_leftchange_curve;		// �����[���`�F���W���񑬓x
short	speed_leftchange_escape;	// �����[���`�F���W���񑬓x

short	speed_slope_brake;		// �����I�_���x
short	speed_slope_trace;		// ��ǂݔ�΂����x

// �p�x
short	angle_rightclank;		// �E�N�����N����p�x
short	angle_leftclank;		// ���N�����N����p�x
short	angle_rightchange;		// �E���[���`�F���W����p�x
short	angle_leftchange;		// �E���[���`�F���W����p�x

// �X�C�b�`�֘A
signed char pushL = 0;
signed char pushR = 0;
char push = 0;
char push1 = 0;
char pattern_sensor = 1;
char pattern_parameter = 1;
char pattern_parameter2 = 1;
char pattern_parameter3 = 1;
char pattern_parameter4 = 1;
char pattern_gain = 1;
char pattern_gain2 = 1;
char pattern_gain3 = 1;
char pattern_speedseting = 1;
char pattern_msd = 1;
char pattern_flash = 1;
char setting_1meter;
char setting_2meter;
char setting_3meter;
char motor_test = 0;
char servo_test = 0;
char servo_test2 = 0;
char fixSpeed = 0;
//======================================//
// �v���g�^�C�v�錾                     //
//======================================//
void data_select ( char *data , char button );
void data_tuning ( void *data, char add , char direction );

char ble;
//////////////////////////////////////////////////////////////////////////
// ���W���[���� setup							//
// �����T�v     ���s�O�ݒ�						//
// ����         �Ȃ�							//
// �߂�l       �Ȃ� 							//
//////////////////////////////////////////////////////////////////////////
void setup( void )
{
	char cnt_led;
	short i;
	
	// LED�_�ŏ���
	
	if ( cnt_setup2 >= 600 ) cnt_setup2 = 0;
	if ( cnt_setup2 < 300 ) {
		led_out( 0x02 );
	} else {
		led_out( 0x01 );
	}
	
	// �f�B�b�v�X�C�b�`�ō��ڑI��
	switch ( dipsw_get() ) {
		//------------------------------------------------------------------
		// �y0x0�z�X�^�[�g�҂�
		//------------------------------------------------------------------
		case 0x0:
			lcdPosition( 0, 0 );
			lcdPrintf("START   ");
			lcdPosition( 0, 1 );
			lcdPrintf("   READY");
			// �v�b�V���X�C�b�`�����҂�
			if ( tasw_get() == 0x1 ) start = 2;
			else if ( tasw_get() == 0x2 ) start = 1;
			
			break;
		//------------------------------------------------------------------
		// �y0x1�z�艟�����[�h�I��
		//------------------------------------------------------------------
		case 0x1:
			lcdPosition( 0, 0 );
			lcdPrintf("PUSHCART");
			lcdPosition( 0, 1 );
			if ( pushcart_mode == 1 ) lcdPrintf("      ON");
			else lcdPrintf("     OFF");
			
			data_select ( &pushcart_mode, 1 );
			break;
		//------------------------------------------------------------------
		// �y0x2�z��~��������
		//------------------------------------------------------------------
		case 0x2:
			lcdPosition( 0, 0 );
			lcdPrintf("STOP    ");
			lcdPosition( 0, 1 );
			lcdPrintf("     %2dm", stopping_meter );
			data_tuning ( &stopping_meter, 1, RIGHT );
			break;
		//------------------------------------------------------------------
		// �y0x3�z�p�����[�^����(�ʏ�g���[�X)
		//------------------------------------------------------------------
		case 0x3:
			data_tuning ( &pattern_parameter, 1, LEFT );
			
			if ( pattern_parameter == 6 ) pattern_parameter = 1;
			else if ( pattern_parameter == 0 ) pattern_parameter = 5;
			
			switch( pattern_parameter ) {
				case 1:
					// �ʏ푖�s���x
					lcdPosition( 0, 0 );
					lcdPrintf("STRAIGHT");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_straight / 10 );
					
					data_tuning ( &speed_straight, 2, RIGHT );
					break;
				case 2:
					// �J�[�u�u���[�L
					lcdPosition( 0, 0 );
					lcdPrintf("BRAKE   ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_curve_brake / 10 );
					
					data_tuning ( &speed_curve_brake, 2, RIGHT );
					break;
				case 3:
					// R600�J�[�u���s���x
					lcdPosition( 0, 0 );
					lcdPrintf("R600    ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_curve_r600 / 10  );
					
					data_tuning ( &speed_curve_r600, 2, RIGHT );
					break;
				case 4:
					// R450�J�[�u���s���x
					lcdPosition( 0, 0 );
					lcdPrintf("R450    ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_curve_r450 / 10 );
					
					data_tuning ( &speed_curve_r450, 2, RIGHT );
					break;
				case 5:
					// S���J�[�u�������x
					lcdPosition( 0, 0 );
					lcdPrintf("CURVE_ST");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_curve_straight / 10 );
					
					data_tuning ( &speed_curve_straight, 2, RIGHT );
					break;
			}
			break;
			
		//------------------------------------------------------------------
		// �y0x4�z�p�����[�^����(�N�����N)
		//------------------------------------------------------------------
		case 0x4:
			data_tuning ( &pattern_parameter2, 1, LEFT );
			
			if ( pattern_parameter2 == 7 ) pattern_parameter2 = 1;
			else if ( pattern_parameter2 == 0 ) pattern_parameter2 = 6;
			
			switch( pattern_parameter2 ) {
				case 1:
					// �N���X���C���ʉߎ�
					lcdPosition( 0, 0 );
					lcdPrintf("CROSLINE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_crossline / 10 );
					
					data_tuning ( &speed_crossline, 2, RIGHT );
					break;
				case 2:
					// �N�����N�O�g���[�X
					lcdPosition( 0, 0 );
					lcdPrintf("CL_TRACE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_ckank_trace / 10 );
					
					data_tuning ( &speed_ckank_trace, 2, RIGHT );
					break;
				case 3:
					// �E�N�����N���񑬓x
					lcdPosition( 0, 0 );
					lcdPrintf("RCL_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightclank_curve / 10 );
					
					data_tuning ( &speed_rightclank_curve, 2, RIGHT );
					break;
				case 4:
					// �E�N�����N���A���x
					lcdPosition( 0, 0 );
					lcdPrintf("RCL_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightclank_escape / 10 );
					
					data_tuning ( &speed_rightclank_escape, 2, RIGHT );
					break;
					
				case 5:
					// ���N�����N���񑬓x
					lcdPosition( 0, 0 );
					lcdPrintf("LCL_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftclank_curve / 10 );
					
					data_tuning ( &speed_leftclank_curve, 2, RIGHT );
					break;
					
				case 6:
					// ���N�����N���A���x
					lcdPosition( 0, 0 );
					lcdPrintf("LCL_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftclank_escape / 10 );
					
					data_tuning ( &speed_leftclank_escape, 2, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// �y0x5�z�p�����[�^����(���[���`�F���W)
		//------------------------------------------------------------------
		case 0x5:
			data_tuning ( &pattern_parameter3, 1, LEFT );
			
			if ( pattern_parameter3 == 8 ) pattern_parameter3 = 1;
			else if ( pattern_parameter3 == 0 ) pattern_parameter3 = 7;
			
			switch( pattern_parameter3 ) {
				case 1:
					// �n�[�t���C���ʉߎ�
					lcdPosition( 0, 0 );
					lcdPrintf("HALFLINE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_halfine / 10 );
					
					data_tuning ( &speed_halfine, 2, RIGHT );
					break;
				case 2:
					// �E���[���`�F���W�i�����x
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_STR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_trace / 10 );
					
					data_tuning ( &speed_rightchange_trace, 2, RIGHT );
					break;
				case 3:
					// �E���[���`�F���W���񑬓x
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_curve / 10 );
					
					data_tuning ( &speed_rightchange_curve, 2, RIGHT );
					break;
				case 4:
					// �E���[���`�F���W���A���x
					lcdPosition( 0, 0 );
					lcdPrintf("RLC_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_rightchange_escape / 10 );
					
					data_tuning ( &speed_rightchange_escape, 2, RIGHT );
					break;
				case 5:
					// �����[���`�F���W�i�����x
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_STR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_trace / 10 );
					
					data_tuning ( &speed_leftchange_trace, 2, RIGHT );
					break;
				case 6:
					// �����[���`�F���W���񑬓x
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_CUR ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_curve / 10 );
					
					data_tuning ( &speed_leftchange_curve, 2, RIGHT );
					break;
				case 7:
					// �����[���`�F���W���񑬓x
					lcdPosition( 0, 0 );
					lcdPrintf("LLC_ESC ");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_leftchange_escape / 10 );
					
					data_tuning ( &speed_leftchange_escape, 2, RIGHT );
					break;
			}
			break;
		
		//------------------------------------------------------------------
		// �y0x6�z�p�����[�^����(�⓹�A�p�x)
		//------------------------------------------------------------------
		case 0x6:
			data_tuning ( &pattern_parameter4, 1, LEFT );
			
			if ( pattern_parameter4 == 7 ) pattern_parameter4 = 1;
			else if ( pattern_parameter4 == 0 ) pattern_parameter4 = 6;
			
			switch( pattern_parameter4 ) {
				case 1:
					// �⓹�u���[�L
					lcdPosition( 0, 0 );
					lcdPrintf("SL_BRAKE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_slope_brake / 10 );
					
					data_tuning ( &speed_slope_brake, 2, RIGHT );
					break;
				case 2:
					// ��ǂݔ�΂����x
					lcdPosition( 0, 0 );
					lcdPrintf("SL_TRACE");
					lcdPosition( 0, 1 );
					lcdPrintf("  %3gm/s", (double)speed_slope_trace / 10 );
					
					data_tuning ( &speed_slope_trace, 2, RIGHT );
					break;
				case 3:
					// �E�N�����N����p�x
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_RCR ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_rightclank );
					
					data_tuning ( &angle_rightclank, 1, RIGHT );
					break;
				case 4:
					// ���N�����N����p�x
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_LCL ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_leftclank );
					
					data_tuning ( &angle_leftclank, 1, RIGHT );
					break;
				case 5:
					// �E���[���`�F���W����p�x
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_RLC ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_rightchange );
					
					data_tuning ( &angle_rightchange, 1, RIGHT );
					break;
				case 6:
					// �����[���`�F���W����p�x
					lcdPosition( 0, 0 );
					lcdPrintf("ANG_LLC ");
					lcdPosition( 0, 1 );
					lcdPrintf("    %4d", angle_leftchange );
					
					data_tuning ( &angle_leftchange, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// �y0x7�z�Q�C������(�T�[�{)
		//------------------------------------------------------------------
		case 0x7:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			data_select( &servo_test, 8 );
			angle_mode = 0;
			if ( servo_test == 1 ) servoPwmOut( ServoPwm );
			else servoPwmOut( 0 );
			
			if ( tasw_get() == 0x4 && push1 == 0 ) {
				push1 = 1;
				pattern_gain += 1;
			} else if ( tasw_get() == 0x0 ) {
				push1 = 0;
			}
			if ( pattern_gain == 4 ) pattern_gain = 1;
			else if ( pattern_gain == 0 ) pattern_gain = 3;
			
			switch( pattern_gain ) {
				case 1:
					// kp
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("   %2d %2d", ki_buff, kd_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp_buff, ki_buff, kd_buff);
					}
					
					data_tuning ( &kp_buff, 1, RIGHT );
					break;
				case 2:
					// ki
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("%2d    %2d", kp_buff, kd_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp_buff, ki_buff, kd_buff);
					}
					
					data_tuning ( &ki_buff, 1, RIGHT );
					break;
				case 3:
					// kd
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("%2d %2d   ", kp_buff, ki_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp_buff, ki_buff, kd_buff);
					}
					
					data_tuning ( &kd_buff, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// �y0x8�z�Q�C������(�p�x)
		//------------------------------------------------------------------
		case 0x8:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			data_select( &servo_test2, 8 );
			angle_mode = 1;
			if ( servo_test == 1 ) servoPwmOut( ServoPwm2 );
			else servoPwmOut( 0 );
			if ( servo_test2 == 1 ) SetAngle = 120;
			else SetAngle = -120;
			
			if ( tasw_get() == 0x4 && push1 == 0 ) {
				push1 = 1;
				pattern_gain2 += 1;
			} else if ( tasw_get() == 0x0 ) {
				push1 = 0;
			}
			if ( pattern_gain2 == 4 ) pattern_gain2 = 1;
			else if ( pattern_gain2 == 0 ) pattern_gain2 = 3;
			
			switch( pattern_gain2 ) {
				case 1:
					// kp2
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("   %2d %2d", ki2_buff, kd2_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					}
					
					data_tuning ( &kp2_buff, 1, RIGHT );
					break;
				case 2:
					// ki2
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("%2d    %2d", kp2_buff, kd2_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					}
					
					data_tuning ( &ki2_buff, 1, RIGHT );
					break;
				case 3:
					// kd2
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("%2d %2d   ", kp2_buff, ki2_buff);
					} else {
						lcdPrintf("%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					}
					
					data_tuning ( &kd2_buff, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// �y0x9�z�Q�C������(���x)
		//------------------------------------------------------------------
		case 0x9:
			lcdPosition( 0, 0 );
			lcdPrintf("kp ki kd");
			
			if ( tasw_get() == 0x4 && push1 == 0 ) {
				push1 = 1;
				pattern_gain3 += 1;
			} else if ( tasw_get() == 0x0 ) {
				push1 = 0;
			}
			if ( pattern_gain3 == 4 ) pattern_gain3 = 1;
			else if ( pattern_gain3 == 0 ) pattern_gain3 = 3;
			
			switch( pattern_gain3 ) {
				case 1:
					// kp3
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("   %2d %2d", ki3_buff, kd3_buff );
					} else {
						lcdPrintf("%2d %2d %2d", kp3_buff, ki3_buff, kd3_buff );
					}
					
					data_tuning ( &kp3_buff, 1, RIGHT );
					break;
				case 2:
					// ki3
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("%2d    %2d", kp3_buff, kd3_buff );
					} else {
						lcdPrintf("%2d %2d %2d", kp3_buff, ki3_buff, kd3_buff );
					}
					
					data_tuning ( &ki3_buff, 1, RIGHT );
					break;
					
				case 3:
					// kd3
					//�l��_��
					lcdPosition( 0, 1 );
					if ( cnt_setup >= 500 ) cnt_setup = 0;
					if ( cnt_setup < 250 ) {
						lcdPrintf("%2d %2d   ", kp3_buff, ki3_buff );
					} else {
						lcdPrintf("%2d %2d %2d", kp3_buff, ki3_buff, kd3_buff );
					}
					
					data_tuning ( &kd3_buff, 1, RIGHT );
					break;
			}
			break;
		//------------------------------------------------------------------
		// �y0xa�z1m/s�ł̃p�����[�^
		//------------------------------------------------------------------
		case 0xa:
			data_tuning ( &pattern_speedseting, 1, LEFT );
				
			if ( pattern_speedseting == 4 ) pattern_speedseting = 1;
			else if ( pattern_speedseting == 0 ) pattern_speedseting = 3;
			
			switch( pattern_speedseting ) {
				case 1:
					lcdPosition( 0, 0 );
					lcdPrintf("1METER %d", setting_1meter);
					lcdPosition( 0, 1 );
					lcdPrintf("SETTING ");
					data_select( &setting_1meter, 1 );
					if ( setting_1meter == 1 ) {
						fix_speedsetting();
						setting_2meter = 0;
						setting_3meter = 0;
					} else {
						fixSpeed = 0;
						setting_2meter = 0;
						setting_3meter = 0;
					}
					break;
					
				case 2:
					lcdPosition( 0, 0 );
					lcdPrintf("2METER %d", setting_2meter);
					lcdPosition( 0, 1 );
					lcdPrintf("SETTING ");
					
					data_select( &setting_2meter, 1 );
					if ( setting_2meter == 1 ) {
						fix_speedsetting();
						setting_1meter = 0;
						setting_3meter = 0;
					} else {
						fixSpeed = 0;
						setting_1meter = 0;
						setting_3meter = 0;
					}
					break;
					
				case 3:
					lcdPosition( 0, 0 );
					lcdPrintf("3METER %d", setting_3meter);
					lcdPosition( 0, 1 );
					lcdPrintf("SETTING ");
					
					data_select( &setting_3meter, 1 );
					if ( setting_3meter == 1 ) {
						fix_speedsetting();
						setting_1meter = 0;
						setting_2meter = 0;
					} else {
						fixSpeed = 0;
						setting_1meter = 0;
						setting_2meter = 0;
					}
					break;
			}
			break;
		//------------------------------------------------------------------
		// �y0xb�zMotor_test
		//------------------------------------------------------------------
		case 0xb:
			data_tuning ( &pattern_sensor, 1, LEFT );
			angle_mode = 0;
			
			if ( pattern_sensor == 16 ) pattern_sensor = 1;
			else if ( pattern_sensor == 0 ) pattern_sensor = 15;
			
			switch( pattern_sensor ) {
				case 1:
					// LED
					lcdPosition( 0, 0 );
					lcdPrintf("LED     ");
					lcdPosition( 0, 1 );
					lcdPrintf("        ");
					cnt_led = 0x00;
					if ( tasw_get() == 0x1) {
						while( cnt_led <= 0x1f ) {
							led_out( cnt_led );
							cnt_led++;
							delay(200);
 						}
					}
					break;
					
				case 2:
					// �W���C��
					lcdPosition( 0, 0 );
					lcdPrintf("Gyro    ");
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						//lcdPrintf("   %5d",getGyro());
						lcdPrintf("   %5d",Degrees);
					}
					break;
					
				case 3:
					// �G���R�[�_
					lcdPosition( 0, 0 );
					lcdPrintf("Encoder ");
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						lcdPrintf("  %6d",EncoderTotal);
					}
					break;
							
				case 4:
					// �|�e���V�������[�^�[
					lcdPosition( 0, 0 );
					lcdPrintf("Angle   ");
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						lcdPrintf("    %4d",getServoAngle());
					}
					break;
					
				case 5:
					// �A�i���O�Z���T
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("R  %4d ",sensorR);
						lcdPosition( 0,1 );
						lcdPrintf("L  %4d ",sensorL);
					}
					break;
					
				case 6:
					// �f�W�^���Z���T
					motor_test = 0;
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						startbar_get();
						lcdPosition( 0, 0 );
						lcdPrintf("G  %4d", sensorG);
						lcdPosition( 0, 1 );
						lcdPrintf("D    0x%x", sensor_inp());
					}
					break;
					
				case 7:
					// ���[�^�[�e�X�g
					lcdPosition( 0, 0 );
					lcdPrintf("Motortes");
					lcdPosition( 0, 1 );
					lcdPrintf("     30%%");
					demo = 0;
					if ( motor_test == 1 ) {
						diff( 30 );
					} else {
						motor_f( 0, 0 );
						motor_r( 0, 0 );
					}
					
					data_select( &motor_test, 1 );
					break;
				case 8:
					// �T�[�{�e�X�g
					lcdPosition( 0, 0 );
					lcdPrintf("Servo   ");
					lcdPosition( 0, 1 );
					lcdPrintf("        ");
					if ( motor_test == 1 ) 	{
						servoPwmOut( 20 );
					} else {
						servoPwmOut( 0 );
					}
					data_select( &motor_test, 1 );
					break;
				case 9:
					// �֐��e�X�g
					lcdPosition( 0, 0 );
					lcdPrintf("gASensor");
					
					motor_test = 0;
					if ( cnt_setup >= 100 ) {
						cnt_setup = 0;
						lcdPosition( 0, 1 );
						lcdPrintf("   %5d", getAnalogSensor());
					}
					break;
					
				case 10:
					// Bluetooth;
					lcdPosition( 0, 0 );
					lcdPrintf("Text   %d", reverr);
					lcdPosition( 0, 1 );
					lcdPrintf("%s",txt_data);
					if ( tasw_get() == 0x1 ) integral_rad = 0;
					break;
					
				case 11:
					// ����p�x;
					lcdPosition( 0, 0 );
					lcdPrintf("TurAngle");
					lcdPosition( 0, 1 );
					lcdPrintf("%4d",TurningAngle);
					if ( tasw_get() == 0x1 ) integral_rad = 0;
					break;
					
				case 12:
					// IMU;
					if ( cnt_setup >= 500 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("xa%2.3f",(double)xa*ACCEL_RANGE/MAXDATA_RANGE);
						lcdPosition( 0, 1 );
						lcdPrintf("ya%2.3f",(double)ya*ACCEL_RANGE/MAXDATA_RANGE);
					}
					break;
					
				case 13:
					// IMU;
					if ( cnt_setup >= 500 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("za%2.3f",(double)za*ACCEL_RANGE/MAXDATA_RANGE);
						lcdPosition( 0, 1 );
						lcdPrintf("xg%2.3f",(double)xg*GYRO_RANGE/MAXDATA_RANGE);
					}
					break;
					
				case 14:
					// IMU;
					if ( cnt_setup >= 500 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("yg%2.3f",(double)yg*GYRO_RANGE/MAXDATA_RANGE);
						lcdPosition( 0, 1 );
						lcdPrintf("zg%2.3f",(double)zg*GYRO_RANGE/MAXDATA_RANGE);
					}
					break;
					
				case 15:
					// who am i;
					if ( cnt_setup >= 500 ) {
						cnt_setup = 0;
						lcdPosition( 0, 0 );
						lcdPrintf("who am i");
						lcdPosition( 0, 1 );
						lcdPrintf("%d     %x",IMUSet, IMUReadByte(MPU9255_ADDRESS, WHO_AM_I));
					}
					break;
			}
			break;
		//------------------------------------------------------------------
		// �y0xc�z�ʒu�Œ�f��
		//------------------------------------------------------------------
		case 0xc:
			lcdPosition( 0, 0 );
			lcdPrintf("DEMO%4d",motorPwm);
			target_speed  = 0;
			
			data_select ( &demo, 1 );
			if ( demo == 1 ) {
				motor_f( motorPwm, motorPwm );
				motor_r( motorPwm, motorPwm );
				lcdPosition( 0, 1 );
				lcdPrintf("   Start");
			} else {
				lcdPosition( 0, 1 );
				lcdPrintf("    Stop");
			}
			break;
		//------------------------------------------------------------------
		// �y0xd�zMicroSD
		//------------------------------------------------------------------
		case 0xd:
			lcdPosition( 0, 0 );
			lcdPrintf("MicroSD ");
			
			servo_test = 0;
			angle_mode = 0;
			data_tuning ( &pattern_msd, 1, LEFT );
			if ( pattern_msd == 11 ) pattern_msd = 1;
			else if ( pattern_msd == 0 ) pattern_msd = 10;
			
			switch ( pattern_msd ) {
				case 1:
					msdWorkAddress = msdAddrBuff[1];
					msdWorkAddress2 = msdAddrBuff[0];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("1       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 2:
					msdWorkAddress = msdAddrBuff[3];
					msdWorkAddress2 = msdAddrBuff[2];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("2       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 3:
					msdWorkAddress = msdAddrBuff[5];
					msdWorkAddress2 = msdAddrBuff[4];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("3       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 4:
					msdWorkAddress = msdAddrBuff[7];
					msdWorkAddress2 = msdAddrBuff[6];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("4       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 5:
					msdWorkAddress = msdAddrBuff[9];
					msdWorkAddress2 = msdAddrBuff[8];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("5       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 6:
					msdWorkAddress = msdAddrBuff[11];
					msdWorkAddress2 = msdAddrBuff[10];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("6       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 7:
					msdWorkAddress = msdAddrBuff[13];
					msdWorkAddress2 = msdAddrBuff[12];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("7       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 8:
					msdWorkAddress = msdAddrBuff[15];
					msdWorkAddress2 = msdAddrBuff[14];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("8       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 9:
					msdWorkAddress = msdAddrBuff[17];
					msdWorkAddress2 = msdAddrBuff[16];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("9       ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
				case 10:
					msdWorkAddress = msdAddrBuff[19];
					msdWorkAddress2 = msdAddrBuff[18];
					if ( msdWorkAddress == 0 && msdWorkAddress2 == 0 ) {
						lcdPosition( 0, 1 );
						lcdPrintf("No data");
					} else {
						lcdPosition( 0, 1 );
						lcdPrintf("10      ");
					}
					if ( tasw_get() == 0x1 && push1 == 0 ) {
						push1 = 1;
						msd_sendToPC();
					}else if ( tasw_get() == 0x0 ) {
						push1 = 0;
					}
					break;
			}
			break;
		//------------------------------------------------------------------
		// �y0xe�z�L�����u���[�V����
		//------------------------------------------------------------------
		case 0xe:
			lcdPosition( 0, 0 );
			lcdPrintf("Angle0  ");
			lcdPosition( 0, 1 );
			lcdPrintf("    %4d", Angle0);
			
			flashDataBuff[0] = Angle0;
			data_tuning ( &Angle0, 1, RIGHT );
			data_select( &servo_test, 8 );
			angle_mode = 1;
			if ( servo_test == 1 ) {
				SetAngle = 0;
				servoPwmOut( ServoPwm2 );
			} else {
				servoPwmOut( 0 );
			}
			if ( tasw_get() == 0x4 && push1 == 0 ) {
				push1 = 1;
				writeFlashData( ANGLE0_AREA, ANGLE0_AREA, ANGLE0_DATA, 1 );
			} else if ( tasw_get() == 0x0 ) {
				push1 = 0;
			}
			break;
		//------------------------------------------------------------------
		// �y0xf�z�t���b�V��
		//------------------------------------------------------------------
		case 0xf:
			switch( pattern_flash ) {
				case 1:
					lcdPosition( 0, 0 );
					lcdPrintf("Flash   ");
					lcdPosition( 0, 1 );
					lcdPrintf("AllErase");
					
					if ( tasw_get() == 1 && push1 == 0 ) {
						push1 = 1;
						pattern_flash = 2;
						break;
					} else if ( tasw_get() == 8 && push1 == 0 ) {
						push1 = 1;
						ble = 1;
						break;
					} else {
						push1 = 0;
					}
					break;
				case 2:
					lcdPosition( 0, 0 );
					lcdPrintf("Really? ");
					lcdPosition( 0, 1 );
					lcdPrintf("c:Y 2:N ");
					
					if ( tasw_get() == 2 ) {
						pattern_flash = 1;
						break;
					}
					if ( tasw_get() == 0xc ) {
						pattern_flash = 3;
						break;
					}
					break;
				case 3:
					lcdPosition( 0, 0 );
					lcdPrintf("AllErase");
					lcdPosition( 0, 1 );
					lcdPrintf("     Now");
					i = 0;
					while ( i <= 1023 ) {
						eraseE2DataFlash(i);
						i++;
						//printf("i = %d\n", i);
					}
					cnt_setup = 0;
					pattern_flash = 4;
					break;
				case 4:
					lcdPosition( 0, 1 );
					lcdPrintf("complete");
					if ( cnt_setup >= 1500 ) {
						pattern_flash = 1;
						break;
					}
					break;
			}
			break;
	default:
		lcdPosition( 0, 0 );
		lcdPrintf("       ");
		lcdPosition( 0, 1 );
		lcdPrintf("none    ");
		
		servo_test = 0;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� data_select								//
// �����T�v     �^�N�g�X�C�b�`��0,1�ɕω�������						//
// ����         data: �ω�������ϐ� button: �ǂ̃X�C�b�`�ŕω������邩			//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void data_select ( char *data , char button )
{
	if ( tasw_get() == button ) {
		if ( *data == 1 && push == 0) {
			push = 1;
			*data = 0;
		} else if ( *data == 0 && push == 0) {
			push = 1;
			*data = 1;
		}
	} else {
		push = 0;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� data_tuning								//
// �����T�v     �^�N�g�X�C�b�`��data����������						//
// ����         data: ����������ϐ� add: 0: �ω��� lr: 0:�E�� 1:����			//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void data_tuning ( void *data, char add , char lr )
{
	short *data2 = (short*)data;	// short�^�|�C���^�ɃL���X�g
	
	if ( lr == LEFT ) { //������@
		if ( tasw_get() == 0x4 || tasw_get() == 0x8 ) {
			if ( tasw_get() == 0x4 && pushL == 0 ) {
				pushL = 1;
				*data2 += add;
			} else if ( tasw_get() == 0x8 && pushL == 0 ) {
				pushL = -1;
				*data2 -= add;
			} else if ( pushL != 0 ) {
				// ���������[�h
				if ( cnt_swL >= 400 && cnt_swL < 2000) {
					if ( ( cnt1 % 400 ) == 0 ) {
						cnt1 = 0;
						if ( pushL == 1) *data2 += add;
						else *data2 -= add;
					}
				} else if ( cnt_swL >= 2000 && cnt_swL < 4000 ) {
					if ( ( cnt1 % 200 ) == 0 ) {
						cnt1 = 0;
						if ( pushL == 1) *data2 += add;
						else *data2 -= add;
					}
				} else if ( cnt_swL >= 4000 ) {
					if ( ( cnt1 % 100 ) == 0 ) {
						cnt1 = 0;
						if ( pushL == 1) *data2 += add;
						else *data2 -= add;
					}
				}
			}
		} else {
			pushL = 0;
			cnt_swL = 0;
		}
	} else {
		if ( tasw_get() == 0x1 || tasw_get() == 0x2 ) {
			if ( tasw_get() == 0x2 && pushR == 0 ) {
				pushR = 1;
				*data2 += add;
			} else if ( tasw_get() == 0x1 && pushR == 0 ) {
				pushR = -1;
				*data2 -= add;
			} else if ( pushR != 0 ) {
				// ���������[�h
				if ( cnt_swR >= 400 && cnt_swR < 2000 ) {
					
					if ( ( cnt1 % 400 ) == 0 ) {
						cnt1 = 0;
						if ( pushR == 1) *data2 += add;
						else *data2 -= add;
					}
				} else if ( cnt_swR >= 2000 && cnt_swR < 4000 ) {
					if ( ( cnt1 % 200 ) == 0 ) {
						cnt1 = 0;
						if ( pushR == 1) *data2 += add;
						else *data2 -= add;
					}
				} else if ( cnt_swR >= 4000 ) {
					if ( ( cnt1 % 100 ) == 0 ) {
						cnt1 = 0;
						if ( pushR == 1) *data2 += add;
						else *data2 -= add;
					}
				}
			}
		} else {
			pushR = 0;
			cnt_swR = 0;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� fix_speedsetting							//
// �����T�v     ���x�p�����[�^���Œ�l�ɂ���						//
// ����         �Ȃ�									//
// �߂�l       0: ���x���ɂ��Ȃ��@1: ���x���ɂ���					//
//////////////////////////////////////////////////////////////////////////////////////////
char fix_speedsetting ( void )
{
	char ret = 0;
	
	if ( setting_1meter == 1 ) {
		speed_straight		= 10;
		speed_curve_brake	= 10;
		speed_curve_r600	= 10;
		speed_curve_r450	= 10;
		speed_curve_straight	= 10;
		
		speed_crossline		= 10;
		speed_ckank_trace	= 10;
		speed_rightclank_curve	= 10;
		speed_rightclank_escape	= 10;
		speed_leftclank_curve	= 10;
		speed_leftclank_escape	= 10;
		
		speed_halfine		= 10;
		speed_rightchange_trace = 10;
		speed_rightchange_curve	= 10;
		speed_rightchange_escape= 10;
		speed_leftchange_trace 	= 10;
		speed_leftchange_curve	= 10;
		speed_leftchange_escape	= 10;
		
		speed_slope_brake	= 10;
		speed_slope_trace	= 10;
		
		fixSpeed = 1;
		ret = 1;
	} else if ( setting_2meter == 1 ) {
		speed_straight		= 20;
		speed_curve_brake	= 20;
		speed_curve_r600	= 20;
		speed_curve_r450	= 20;
		speed_curve_straight	= 20;
		
		speed_crossline		= 20;
		speed_ckank_trace	= 20;
		speed_rightclank_curve	= 20;
		speed_rightclank_escape	= 20;
		speed_leftclank_curve	= 20;
		speed_leftclank_escape	= 20;
		
		speed_halfine		= 20;
		speed_rightchange_trace = 20;
		speed_rightchange_curve	= 20;
		speed_rightchange_escape= 20;
		speed_leftchange_trace 	= 20;
		speed_leftchange_curve	= 20;
		speed_leftchange_escape	= 20;
		
		speed_slope_brake	= 20;
		speed_slope_trace	= 20;
		
		fixSpeed = 1;
		ret = 1;
	} else if ( setting_3meter == 1 ) {
		speed_straight		= 30;
		speed_curve_brake	= 30;
		speed_curve_r600	= 30;
		speed_curve_r450	= 30;
		speed_curve_straight	= 30;
		
		speed_crossline		= 30;
		speed_ckank_trace	= 26;
		speed_rightclank_curve	= 22;
		speed_rightclank_escape	= 30;
		speed_leftclank_curve	= 22;
		speed_leftclank_escape	= 30;
		
		speed_halfine		= 30;
		speed_rightchange_trace = 30;
		speed_rightchange_curve	= 30;
		speed_rightchange_escape= 30;
		speed_leftchange_trace 	= 30;
		speed_leftchange_curve	= 30;
		speed_leftchange_escape	= 30;
		
		speed_slope_brake	= 20;
		speed_slope_trace	= 30;
		
		fixSpeed = 1;
		ret = 1;
	}
	
	return ret;
}