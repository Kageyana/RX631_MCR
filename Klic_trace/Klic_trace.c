//////////////////////////////////////////////////////////////////////////
//									//
//  FILE	:Klic_trace.c						//
//  DATE	:Thr, Dec 21, 2017					//
//  DESCRIPTION :���s�v���O����						//
//  CPU TYPE	:RX631							//
//									//
//////////////////////////////////////////////////////////////////////////
//======================================//
// �C���N���[�h                         //
//======================================//
#include "R_PG_RX631_mcr_ver3.0.h" 
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "SetUp.h"
#include "SCI.h"
#include "E2dataFlash.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "I2C_MPU-9255.h"
#include <stdio.h>

//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// ���s�p�^�[���֘A
char 	pattern = 0;	// �p�^�[���ԍ�
char 	countdown = 4;
short 	angle_center;

// ���[�h�֘A
char 	lcd_mode = 1;	// LCD�\����		1:�\��		0:����		
char 	slope_mode;	// ��`�F�b�N		0:����n��	1:����I���	2:�����n��	3:�����I���
char 	angle_mode;	// �T�[�{PWM�ύX	0:�����g���[�X	1:�p�x����
char	curve_moed;	// �J�[�u����		0:�J�[�u�ȊO	1:�J�[�u���s��
char	msdset;		// MicroSD�����������ꂽ��
char	IMUSet;		// IMU�����������ꂽ��	0: ���������s	1:����������
char	error_mode;	// 0:������~ 1:�Z���T�S�� 2:�Z���T�S���� 3:�G���R�[�_��~ 4:�W���C������

//======================================//
// ���C���v���O����	                //
//======================================//
void main(void){
	short i, j, rpwm, lpwm;
	unsigned int ui;
	
	//======================================//
	// ������		                //
	//======================================//
	inti_lcd();			// LCD������
	lcdPosition( 0, 0 );
	lcdPrintf("INITIALI");
	lcdPosition( 0, 1 );
	lcdPrintf("    ZING");
	
	motor_f( 0, 0 );		// ���[�^�[��~
	motor_r( 0, 0 );
	servoPwmOut( 0 );
	
	pushcart_mode = 0;		// �艟�����[�hoff
	slope_mode = 0;			// ����`�F�b�N
	angle_mode = 0;			// �����g���[�X
	start = 0;			// �Q�[�g�X�^�[�g
	
	init_BeepS();			// �u�U�[������
	
	// �t���b�V��������
	if( initFlash() == 0 ) {
		setBeepPatternS( 0x8000 );
		readFlashSetup();	// �f�[�^�t���b�V������O��p�����[�^��ǂݍ���
	} else{
		setBeepPatternS( 0xaa00 );
	}
	
	// MicroSD�J�[�h������
	if( init_msd() == 0 ) {
		setBeepPatternS( 0x4000 );
		msdset = 1;
	} else {
		setBeepPatternS( 0xaa00 );
		msdset = 0;
	}
	
	// IMU�̏�����
	IMUSet = 0;
	#ifdef USED_IMU
	init_IMU();
	IMUSet = 1;
	#endif
	//if ( init_IMU() == 1 ) IMUSet == 1;
	//else	IMUSet == 0;
	
	// �{�[���[�g230400bps
	txt_rate = "$br";
	txt_rate += 0x5;
	while(1){
		if( pattern >= 11 && pattern <= 99 ) {
			if( !pushcart_mode ) {		
				// �艟�����[�hOFF
				if( cnt1 >= 500 ) {		// �����o���Ă���
					if( EncoderTotal >= ( PALSE_METER * stopping_meter ) ) { // �������߂̏ꍇ
						error_mode = 0;
						pattern = 101;
					} else if( cnt_out >= STOP_SENSOR1 ) {	// �Z���T�S��
						error_mode = 1;
						pattern = 101;
					} else if( cnt_out2 >= STOP_SENSOR2 ) {	// �Z���T�S����
						error_mode = 2;
						pattern = 101;
					} else if( cnt_out3 >= STOP_ENCODER ) {	// �G���R�[�_��~(�Ђ�����Ԃ����H)
						error_mode = 3;
						pattern = 101;
					} else if( cnt_out4 >= STOP_GYRO ) {	// �}�C�i�X�̉����x���m(�R�[�X���痎�����H)
						error_mode = 4;	
						//pattern = 101;
					} else if ( stopWord == 1 ) {
						error_mode = 5;
						pattern = 101;
					}
					
					if ( stopWord == 1 ) {
						error_mode = 5;
						pattern = 101;
					}
					/*
					if( cnt1 >= STOP_COUNT ) {
						pattern = 101;
					}
					*/
				}
			} else {			
				// �艟�����[�hON
				lcdPosition( 0, 0 );
				lcdPrintf("case %3d", pattern);
				lcdPosition( 0, 1 );
				lcdPrintf("%4d", (short)TurningAngleIMU);
			}
		} else if( pattern >= 100 ) {
			lcd_mode = 1;
			lcdPosition( 0, 0 );
			lcdPrintf("TIME  %d", error_mode);
			lcdPosition( 0, 1 );
			lcdPrintf(" %5dms", ui);
		}
		
	switch( pattern ) {
		//-------------------------------------------------------------------
		// �y000�z�X�^�[�g����
		//-------------------------------------------------------------------
		case 0:
			// �X�^�[�g�O�ݒ�
			setup();
			if( start >= 1 && !pushcart_mode ) {
				demo = 0;		// �f�����[�h����
				angle_mode = 0;		// �����g���[�X
				txt= txt_data;		// ��M�z�񃊃Z�b�g
				cnt_byte = 0;		// ��M�J�E���g���Z�b�g
				if( msdset == 1 ) init_log();	// ���O�L�^����
				if( fixSpeed == 0 ) writeFlashBeforeStart();	// ���x�p�����[�^���f�[�^�t���b�V���ɕۑ�
				
				// �����g���[�X�pPID�Q�C���ۑ�
				flashDataBuff[ 0 ] = kp_buff;
				flashDataBuff[ 1 ] = ki_buff;
				flashDataBuff[ 2 ] = kd_buff;
				writeFlashData( PID_STARTAREA, PID_ENDAREA, PID_DATA, 3 );
				// �p�x����pPID�Q�C���ۑ�
				flashDataBuff[ 0 ] = kp2_buff;
				flashDataBuff[ 1 ] = ki2_buff;
				flashDataBuff[ 2 ] = kd2_buff;
				writeFlashData( PID2_STARTAREA, PID2_ENDAREA, PID2_DATA, 3 );
				// ���x����pPID�Q�C���ۑ�
				flashDataBuff[ 0 ] = kp3_buff;
				flashDataBuff[ 1 ] = ki3_buff;
				flashDataBuff[ 2 ] = kd3_buff;
				writeFlashData( PID3_STARTAREA, PID3_ENDAREA, PID3_DATA, 3 );
				// ��~�����ۑ�
				flashDataBuff[ 0 ] = stopping_meter;
				writeFlashData( STOPMETER_STARTAREA, STOPMETER_ENDAREA, STOPMETER_DATA, 1 );
				
				target_speed = speed_straight * SPEED_CURRENT; // �ڕW���x�ݒ�
				cnt1 = 0;		// �^�C�}���Z�b�g
				pattern = 1;
				break;
			}else if( start >= 1 && pushcart_mode ) {
				// �艟�����[�h�̏ꍇ�����ɒʏ�g���[�X
				// �����g���[�X�pPID�Q�C���ۑ�
				flashDataBuff[ 0 ] = kp_buff;
				flashDataBuff[ 1 ] = ki_buff;
				flashDataBuff[ 2 ] = kd_buff;
				writeFlashData( PID_STARTAREA, PID_ENDAREA, PID_DATA, 3 );
				// �p�x����pPID�Q�C���ۑ�
				flashDataBuff[ 0 ] = kp2_buff;
				flashDataBuff[ 1 ] = ki2_buff;
				flashDataBuff[ 2 ] = kd2_buff;
				writeFlashData( PID2_STARTAREA, PID2_ENDAREA, PID2_DATA, 3 );
				
				lcdPosition( 0, 1 );
				lcdPrintf("        ");
				setBeepPatternS( 0xfff0 );
				
				EncoderTotal = 10;		// �����s����
				cnt1 = 0;			// �^�C�}���Z�b�g
				lcd_mode = 1;			// LCD�\��ON
				pattern = 11;
				break;
			}
			break;
			
		case 1:
			if( start == 1 ) {
				// �J�E���g�_�E���X�^�[�g
				if( cnt1 >= 1500 && countdown == 4 ) {
					countdown = 3;
					lcdPosition( 0, 1 );
					lcdPrintf("       3");
					setBeepPatternS( 0x8000 );
					led_out( 0x10 );
				}
				if( cnt1 >= 2500 && countdown == 3 ) {
					countdown = 2;
					lcdPosition( 0, 1 );
					lcdPrintf("       2");
					setBeepPatternS( 0x8000 );
					led_out( 0x04 );
				}
				if( cnt1 >= 3500 && countdown == 2 ) {
					countdown = 1;
					lcdPosition( 0, 1 );
					lcdPrintf("       1");
					setBeepPatternS( 0x8000 );
					led_out( 0x01 );
				}
				if( cnt1 >= 4500 && countdown == 1 ) {
					countdown = 0;
					lcdPosition( 0, 1 );
					lcdPrintf("        ");
					setBeepPatternS( 0xfff0 );
						
					EncoderTotal = 10;	// �����s����
					cnt1 = 0;		// �^�C�}���Z�b�g
					lcd_mode = 0;		// LCD�\��OFF
					msdFlag = 1;		// �f�[�^�L�^�J�n
					pattern = 11;
					break;
				}
			}else if( start == 2 ) {
				// �X�^�[�g�Q�[�g�J���X�^�[�g
				pattern = 2;
				break;
			}
			break;
			
		case 2:
			// �X�^�[�g�o�[�J�҂�
			if( !startbar_get() ) {
				EncoderTotal = 10;		// �����s����
				cnt1 = 0;			// �^�C�}���Z�b�g
				lcd_mode = 1;			// LCD�\��OFF
				msdFlag = 1;                    // �f�[�^�L�^�J�n
				pattern = 11;
				break;
			}
			// LED�_�ŏ���
			if( cnt1 >= 2000 ) cnt1 = 0;
			if( cnt1 < 1000 ) {
				led_out( 0x04 );
			}else{
				led_out( 0x08 );
			}
			break;
		//-------------------------------------------------------------------
		// �y010�z�g���[�X����
		//-------------------------------------------------------------------
		case 11:
			servoPwmOut( ServoPwm );
			target_speed = speed_straight * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			led_out( 0x00 );
			
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// �E�n�[�t���C���`�F�b�N
	   		if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
	   		if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// �J�[�u�`�F�b�N
			if( i >=  CURVE_R600_START || i <= - CURVE_R600_START ) {
				enc1 = 0;
				curve_moed = 1;
				pattern = 12;
				break;
			}
			break;
			
		case 12:
			// �J�[�u�u���[�L
			servoPwmOut( ServoPwm );
			target_speed = speed_curve_brake * SPEED_CURRENT;
			led_out( 0x1e );
			diff( motorPwm );
			
			if( enc1 > enc_mm( 60 ) ) {		// 60mm�i��
				enc1 = 0;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 13;
				break;
			}
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// �E�n�[�t���C���`�F�b�N
	   		if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
	   		if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// �J�[�u�p���ڃ`�F�b�N
			if( i <  CURVE_R600_START && i > -CURVE_R600_START ) {
				enc1 = 0;
				pattern = 11;
				break;
			}
			break;
			
		case 13:
			// R600�J�[�u���s
			servoPwmOut( ServoPwm );
			target_speed = speed_curve_r600 * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
	   		if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
	   		if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// R450�`�F�b�N
			if( i >= CURVE_R450_START || i <= -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 14;
				break;
			}
			// �J�[�u�p���ڃ`�F�b�N
			if( i <  CURVE_R600_START && i > -CURVE_R600_START ) {
				if ( enc1 >= enc_mm(50) ) {
					enc1 = 0;
					pattern = 15;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
			}
			break;
			
		case 14:
			// R450�J�[�u���s
			servoPwmOut( ServoPwm );
			target_speed = speed_curve_r450 * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
	   		if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
	   		if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// R600�`�F�b�N
			if( i < CURVE_R450_START && i > -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 13;
				break;
			}
			break;
		
		case 15:
			// �J�[�u�p���ڑ��s
			servoPwmOut( ServoPwm );
			target_speed = speed_curve_straight * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			if( enc1 >= enc_mm( 300 ) ) {		// 300mm�i��
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				curve_moed = 0;
				pattern = 11;
				break;
			}
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// �J�[�u�`�F�b�N
			if( i >=  CURVE_R600_START || i <= - CURVE_R600_START ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 13;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//�y020�z�N�����N���o����
		//-------------------------------------------------------------------
		case 21:
			servoPwmOut( 0 );
			target_speed = speed_crossline* SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x03 );
			
			if( enc1 > enc_mm( 90 ) ) {		// 60mm�i��
				enc1 = 0;
				pattern = 22;
				break;
			}
			break;
			
		case 22:
			servoPwmOut( ServoPwm );
			target_speed = speed_ckank_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x06 );
			
			// �E�N�����N�`�F�b�N
			if( sensor_inp() ==  0x3 ) {
				enc1 = 0;
				led_out( 0x01 );
				setBeepPatternS( 0xa000 );
				SetAngle = angle_rightclank;
				angle_mode = 1;
				TurningAngleIMU = 0;
				pattern = 31;
				break;
			}
			// ���N�����N�`�F�b�N
			if( sensor_inp() ==  0x6 ) {
				enc1 = 0;
				led_out( 0x02 );
				setBeepPatternS( 0xa800 );
				SetAngle = angle_leftclank;
				angle_mode = 1;
				TurningAngleIMU = 0;
				pattern = 41;
				break;
			}
			
	        break;
		//-------------------------------------------------------------------
		//�y030�z�E�N�����N����
		//-------------------------------------------------------------------
		case 31:
			SetAngle = angle_rightclank;
			servoPwmOut( ServoPwm2 );
			target_speed = speed_rightclank_curve * SPEED_CURRENT;
			i = (Encoder * 10) - target_speed;	// �ڕW�l�Ƃ̕΍�
			j = getAnalogSensor();
			
			if( i >= 200 && enc1 <= enc_mm( 40 ) ) {
				// ���x���߂Ȃ�}�u���[�L
				motor_f( -100, 0 );
				motor_r( -100, 0 );
			} else {
				// ���������A�E�������݂̂Ɏg�p����
				if( motorPwm > 0 ) rpwm = 0;		// �������@�E0
				else rpwm = motorPwm;			// �������@�E����
				if( motorPwm > 0 ) lpwm = motorPwm;	// �������@������
				else lpwm = 0;				// �������@��0
				motor_f( lpwm, rpwm);
				motor_r( lpwm, rpwm);	
			}
			if( -TurningAngleIMU <= 30 ) {
				if( sensor_inp() == 0x2 ) {
					enc1 = 0;
					angle_mode = 0;
					pattern = 36;
					break;
				}
			} else if ( -TurningAngleIMU >= 20 ) {
				if( j <= -1800 ) {
					enc1 = 0;
					i = (short)-TurningAngleIMU;
					pattern = 34;
					break;
				}
			}
			if ( sensor_inp() == 0x4 ) {
				enc1 = 0;
				pattern = 32;
				break;
			}
			break;
			
		case 32:
			// �O���ǂݔ�΂�
			SetAngle = angle_rightclank;
			target_speed = speed_rightclank_curve * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			
			// ���������A�E�������݂̂Ɏg�p����
			if( motorPwm > 0 ) rpwm = 0;		// �������@�E0
			else rpwm = motorPwm;			// �������@�E����
			if( motorPwm > 0 ) lpwm = motorPwm;	// �������@������
			else lpwm = 0;				// �������@��0
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			
			//if( -TurningAngleIMU <= 90 && -TurningAngleIMU >= 40) {
				if( j <= -1800 ) {
					enc1 = 0;
					i = (short)-TurningAngleIMU;
					pattern = 34;
					break;
				}
			//}
			break;
			
		case 34:
			// �p�x�ێ�
			// sensor_inp() == 2��ǂ񂾂��ƂɎ��s
			SetAngle = -( 90 - 10 - i ) * (435/35);	// ���C������̊p�x20��
			target_speed = speed_rightclank_escape * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			
			// ���������A�E�������݂̂Ɏg�p����
			if( motorPwm > 0 ) rpwm = 0;		// �������@�E0
			else rpwm = motorPwm;			// �������@�E����
			if( motorPwm > 0 ) lpwm = motorPwm;	// �������@������
			else lpwm = 0;				// �������@��0
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			led_out(0x18);
			
			if( sensor_inp() == 0x2 && j >= -1800) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 36;
				break;
			}
			break;
			
		case 36:
			// ���A
			servoPwmOut( ServoPwm );
			target_speed = speed_rightclank_escape * SPEED_CURRENT;
			diff( motorPwm );
			led_out(0x06);
			
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			if( enc1 >= enc_mm( 600 ) ) {		// ���肷��܂ő҂�(600mm)
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//�y040�z���N�����N����
		//-------------------------------------------------------------------
		case 41:
			SetAngle = angle_leftclank;
			servoPwmOut( ServoPwm2 );
			target_speed = speed_leftclank_curve * SPEED_CURRENT;
			i = (Encoder * 10) - target_speed;	// �ڕW�l�Ƃ̕΍�
			j = getAnalogSensor();
			
			if( i >= 200 && enc1 <= enc_mm( 40 ) ) {
				// ���x���߂Ȃ�}�u���[�L
				motor_f( -100, 0 );
				motor_r( -100, 0 );
			} else {
				// ���������A�E�������݂̂Ɏg�p����
				if( motorPwm < 0 ) rpwm = 0;
				else rpwm = motorPwm;
				if( motorPwm > 0 ) lpwm = 0;
				else lpwm = motorPwm;
				motor_f( lpwm, rpwm);
				motor_r( lpwm, rpwm);	
			}
			if( TurningAngleIMU <= 30 ) {
				if( sensor_inp() == 0x2 ) {
					enc1 = 0;
					angle_mode = 0;
					pattern = 46;
					break;
				}
			} else if ( TurningAngleIMU >= 20 ) {
				if( j >= 1800 ) {
					enc1 = 0;
					i = TurningAngleIMU;
					pattern = 44;
					break;
				}
			}
			if ( sensor_inp() == 0x1 ) {
				enc1 = 0;
				pattern = 42;
				break;
			}
			break;
			
		case 42:
			// �O���ǂݔ�΂�
			SetAngle = angle_leftclank;
			target_speed = speed_leftclank_curve * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			
			// ���������A�E�������݂̂Ɏg�p����
			if( motorPwm < 0 ) rpwm = 0;
			else rpwm = motorPwm;
			if( motorPwm > 0 ) lpwm = 0;
			else lpwm = motorPwm;
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			
			//if( TurningAngleIMU <= 90 && TurningAngleIMU >= 40) {
				if( j >= 1800 ) {
					enc1 = 0;
					i = TurningAngleIMU;
					pattern = 44;
					break;
				}
			//}
			break;

		case 43:
			// �O���ǂݔ�΂�2
			SetAngle = angle_leftclank;
			target_speed = speed_leftclank_escape * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			
			// ���������A�E�������݂̂Ɏg�p����
			if( motorPwm < 0 ) rpwm = 0;
			else rpwm = motorPwm;
			if( motorPwm > 0 ) lpwm = 0;
			else lpwm = motorPwm;
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			
			if( sensor_inp() == 0x2 ) {
				enc1 = 0;
				if( getServoAngle() >= angle_leftclank ) {
					i = angle_leftclank;
				}else{
					i = getServoAngle();
				}
				pattern = 44;
				break;
			}
			break;
			
		case 44:
			// �p�x�ێ�
			// sensor_inp() == 2��ǂ񂾂��ƂɎ��s
			SetAngle = ( 90 - 10 - i ) * (435/35);	// ���C������̊p�x20��
			target_speed = speed_leftclank_escape * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			
			// ���������A�E�������݂̂Ɏg�p����
			if( motorPwm < 0 ) rpwm = 0;
			else rpwm = motorPwm;
			if( motorPwm > 0 ) lpwm = 0;
			else lpwm = motorPwm;
			motor_f( lpwm, rpwm);
			motor_r( lpwm, rpwm);
			led_out(0x18);
			
			if( sensor_inp() == 0x2 && j <= 1800) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 46;
				break;
			}
			break;
			
		case 46:
			// ���A
			servoPwmOut( ServoPwm );
			target_speed = speed_leftclank_escape * SPEED_CURRENT;
			diff( motorPwm );
			led_out(0x06);
			
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			if( enc1 >= enc_mm( 600 ) ) {		// ���肷��܂ő҂�(600mm)
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//�y050�z�E���[���`�F���W����
		//-------------------------------------------------------------------
		case 51:
			servoPwmOut( 0 );
			target_speed = speed_halfine * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				angle_mode = 0;
				setBeepPatternS( 0xe000 );
				pattern = 52;
				break;
			}
			/*if( enc1 <= 20 * PALSE_MILLIMETER ) {
				if( sensor_inp() == 0x2 ) {
					enc1 = 0;
					pattern = 11;
					break;
				}
			}*/
			if( check_crossline() ) {		// �N���X���C���`�F�b�N
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
			}
			break;
			
		case 52:
			servoPwmOut( ServoPwm );
			target_speed = speed_rightchange_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x04 );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				angle_mode = 1;
				pattern = 53;
				break;
			}
			break;
			
		case 53:
			SetAngle = angle_rightchange;
			servoPwmOut( ServoPwm2 );
			target_speed = speed_rightchange_curve * SPEED_CURRENT;
			diff( motorPwm );
			
			if( sensor_inp() == 0x1 ) {
				enc1 = 0;
				pattern = 54;
				break;
			}
			break;
			
		case 54:
			SetAngle = 0;
			servoPwmOut( ServoPwm2 );
			target_speed = speed_rightchange_curve * SPEED_CURRENT;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );
			
			if( sensor_inp() == 0x2 && getAnalogSensor() < 1500 && getAnalogSensor() > -1500 ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 57;
				break;
			}
			break;
			
		case 55:
			servoPwmOut( 90 );
			target_speed = speed_rightchange_curve * SPEED_CURRENT;
			diff( motorPwm );
			
			if( sensor_inp() == 0x2 ) {
				angle_center = getServoAngle();
				enc1 = 0;
				pattern = 56;
				break;
			}
			break;
			
		case 56:
			SetAngle = angle_center;
			servoPwmOut( ServoPwm2 );
			target_speed = speed_rightchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 10 ) ) {
				angle_center = getServoAngle();
				enc1 = 0;
				angle_mode = 0;
				pattern = 57;
				break;
			}
			break;
			
		case 57:
			servoPwmOut( ServoPwm );
			target_speed = speed_rightchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			if( enc1 >= enc_mm( 600 ) ) {
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//�y060�z�����[���`�F���W����
		//-------------------------------------------------------------------
		case 61:
			servoPwmOut( 0 );
			target_speed = speed_halfine * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				setBeepPatternS( 0xe400 );
				angle_mode = 0;
				pattern = 62;
				break;
			}
			/*if( enc1 <= 20 * PALSE_MILLIMETER ) {
				if( sensor_inp() == 0x2 ) {
					enc1 = 0;
					pattern = 11;
					break;
				}
			}*/
			if( check_crossline() ) {		// �N���X���C���`�F�b�N
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
			}
			break;
			
		case 62:
			servoPwmOut( ServoPwm );
			target_speed = speed_leftchange_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x08 );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				angle_mode = 1;
				pattern = 63;
				break;
			}
			break;
			
		case 63:
			SetAngle = angle_leftchange;
			servoPwmOut( ServoPwm2 );
			target_speed = speed_leftchange_curve * SPEED_CURRENT;
			diff( motorPwm );
			
			if( sensor_inp() == 0x4 ) {
				enc1 = 0;
				pattern = 64;
				break;
			}
			break;
			
		case 64:
			SetAngle = 0;
			servoPwmOut( ServoPwm2 );
			target_speed = speed_leftchange_curve * SPEED_CURRENT;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );

			if( sensor_inp() == 0x2 && getAnalogSensor() < 1500 && getAnalogSensor() > -1500 ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 67;
				break;
			}
			break;
			
		case 65:
			servoPwmOut( -90 );
			target_speed = speed_leftchange_curve * SPEED_CURRENT;
			diff( motorPwm );
			
			if( sensor_inp() == 0x2 ) {
				angle_center = getServoAngle();
				enc1 = 0;
				pattern = 66;
				break;
			}
			break;
			
		case 66:
			SetAngle = angle_center;
			servoPwmOut( ServoPwm2 );
			target_speed = speed_leftchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 10 ) ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 67;
				break;
			}
			break;
			
		case 67:
			servoPwmOut( ServoPwm );
			target_speed = speed_leftchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			if( enc1 >= enc_mm( 600 ) ) {
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//�y070�z�⓹����
		//-------------------------------------------------------------------
		case 71:
			// �댟�m���f
			servoPwmOut( ServoPwm );
			//target_speed = ( speed_slope_trace / 10 ) * SPEED_CURRENT;
			diff( motorPwm );
			//if( cnt1 >= 10 ) {
				if( check_slope() == 1 ) {
					if( slope_mode == 0 ) {
						// ���n��
						enc1 = 0;
						led_out( 0x18 );
						setBeepPatternS( 0xe000 );
						pattern = 72;
						break;
					}else if( slope_mode == 2 && enc_slope >= enc_mm( 600 ) ) {
						// ����I���
						enc1 = 0;
						led_out( 0x05 );
						setBeepPatternS( 0xe000 );
						slope_mode = 0;
						pattern = 74;
						break;
					}else{
						enc1 = 0;
						pattern = 11;
						break;
					}
				}else if( check_slope() == -1 ) {
					if( slope_mode == 1 && enc_slope >= enc_mm( 1000 ) ) {
						// ���I���A����n��
						enc1 = 0;
						led_out( 0x05 );
						setBeepPatternS( 0xe000 );
						slope_mode = 2;
						pattern = 75;
						break;
					}else{
						enc1 = 0;
						pattern = 11;
						break;
					}
					break;
				}else{
					enc1 = 0;
					pattern = 11;
					break;
				}
			//}
			break;
			
		case 72:
			// �Ⓒ�_�܂ő��s
			servoPwmOut( ServoPwm );
			target_speed = speed_slope_trace * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 660 ) ) {
				enc1 = 0;
				setBeepPatternS( 0xe000 );
				led_out( 0x03 );
				pattern = 73;
				break;
			}
			break;
			
		case 73:
			// ����I�_�u���[�L
			servoPwmOut( ServoPwm );
			motor_f( -100, -100 );
			motor_r( -100, -100 );
			
			if( enc1 >= enc_mm( 50 ) ) {
				enc1 = 0;
				led_out( 0x10 );
				pattern = 75;
				break;
			}
			break;
			
		case 74:
			// �����I�_�u���[�L
			servoPwmOut( ServoPwm );
			target_speed = speed_slope_brake * SPEED_CURRENT;
			diff( motorPwm );
			if( enc1 >= enc_mm( 40 ) ) {
				enc1 = 0;
				led_out( 0x0f );
				pattern = 75;
				break;
			}
			break;
			
		case 75:
			// �W���C���Z���T�����肷��܂œǂݔ�΂�
			servoPwmOut( ServoPwm );
			target_speed = speed_slope_trace * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 150 ) ) {
				enc1 = 0;
				pattern = 76;
				break;
			}
			break;
			
		case 76:
			// �W���C���Z���T�����肷��܂œǂݔ�΂�
			servoPwmOut( ServoPwm );
			target_speed = speed_slope_trace * SPEED_CURRENT;
			diff( motorPwm );
			
			// �N���X���C���`�F�b�N
			if( check_crossline() ) {
				enc1 = 0;
				led_out( 0x03 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
   			if( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
   			if( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			if( slope_mode  == 0 ) {
				if( enc1 >= enc_mm( 1000 ) ) {
					enc1 = 0;
					enc_slope = 0;
					slope_mode = 1;
					pattern = 11;
					break;
				}
			}else{
				if( enc1 >= enc_mm( 400 ) ) {
					enc1 = 0;
					enc_slope = 0;
					pattern = 11;
					break;
				}
			}
			break;
		//-------------------------------------------------------------------
		//�y100�z��~����
		//-------------------------------------------------------------------
		case 101:
			enc1 = 0;	
			ui = cnt1;	// ���s���Ԏ擾
			pattern = 102;
			break;
			
		case 102:
			servoPwmOut( ServoPwm );
			target_speed = 0;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );
			
			if( Encoder <= 1 && Encoder >= -1 ) {
				enc1 = 0;
				pattern = 103;
				break;
			}
			break;
			
		case 103:
			servoPwmOut( 0 );
			motor_f( 0, 0 );
			motor_r( 0, 0 );
			
			if( msdset == 1 ) {
				pattern = 104;
				break;
			}else{
				setBeepPatternS( 0xaa00 );
				pattern = 106;
				break;
			}
			break;
			
		case 104:
			// �Ō�̃f�[�^���������܂��܂ő҂�
			printf("case 104\n");
			if( checkMicroSDProcess() == 11 ) {
				msdFlag = 0;	// ���O�L�^�I��
				printf("microSDProcessEndNOW\n");
				microSDProcessEnd();        // microSDProcess�I������
				pattern = 105;
				break;
			}else if( checkMicroSDProcess() == 0 ) {
				setBeepPatternS( 0xf0f0 );
				pattern = 106;
				break;
			}
			break;
			
		case 105:
			// �I���������I���܂ő҂�
			if( checkMicroSDProcess() == 0 ) {
				// MicroSD�ŏI�������݃A�h���X�ۑ�
				flashDataBuff[ 0 ] = msdStartAddress >> 16;
				flashDataBuff[ 1 ] = msdStartAddress & 0xffff;	// �J�n�A�h���X
				flashDataBuff[ 2 ] = msdWorkAddress >> 16;
				flashDataBuff[ 3 ] = msdWorkAddress & 0xffff;	// �I���A�h���X
				writeFlashData( MSD_STARTAREA, MSD_ENDAREA, MSD_DATA, 4 );
				printf("msdStartAddress = %d\n", msdStartAddress);
				printf("msdEndAddress = %d\n", msdWorkAddress);
				pattern = 106;
				setBeepPatternS( 0xa8a8 );
				break;
			}
			break;
			
		case 106:
			// LED�_�ŏ���
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				led_out( 0x1f );
			}else{
				led_out( 0x00 );
			}
			break;
			
		default:
			pattern = 101;
			break;
			
	} // end of "switch ( pattern )"
	} // end of "while ( 1 )"
}