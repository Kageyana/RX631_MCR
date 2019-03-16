///////////////////////////////////////////////////////////////////////////
//												//
//  FILE	:Klic_trace.c								//
//  DATE	:Thr, Dec 21, 2017							//
//  DESCRIPTION :���s�v���O����							//
//  CPU TYPE	:RX631								//
//												//
///////////////////////////////////////////////////////////////////////////
//====================================//
// �C���N���[�h									//
//====================================//
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "SetUp.h"
#include "SCI.h"
#include "E2dataFlash.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "I2C_MPU-9255.h"
#include "MemorryTrace.h"
#include <stdio.h>

//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//
// ���s�p�^�[���֘A
char		pattern = 0;	// �p�^�[���ԍ�
char		countdown = 0x10;
short 	angle_center;

// ���[�h�֘A
char		curve_moed;	// �J�[�u����	0:�J�[�u�ȊO	1:�J�[�u���s��
char		error_mode;	// 0:������~ 1:�Z���T�S�� 2:�Z���T�S���� 3:�G���R�[�_��~ 4:�W���C������

// �^�C�}�֘A
// 1ms�^�C�}
unsigned int 		cnt1;		// ���s�p�^�C�}�J�E���g
unsigned short	 	cnt_out;	// �R�[�X�A�E�g����p�^�C�}
unsigned short	 	cnt_out2;	// �R�[�X�A�E�g����p�^�C�}2
unsigned short	 	cnt_out3;	// �R�[�X�A�E�g����p�^�C�}3
unsigned short	 	cnt_out4;	// �R�[�X�A�E�g����p�^�C�}4
static char			Timer10;	// 1ms�J�E���g�p
//====================================//
// �v���g�^�C�v�錾								//
//====================================//
void init_Parameter ( bool lcd );
//====================================//
// ���C���v���O����								//
//====================================//
void main(void){
	short i, j;
	unsigned int ui;
	
	//=================================//
	// ������									//
	//=================================//
	inti_lcd();			// LCD������
	
	motor_f( 0, 0 );		// ���[�^�[��~
	motor_r( 0, 0 );
	servoPwmOut( 0 );
	
	pushcart_mode = 0;		// �艟�����[�hoff
	slope_mode = 0;		// ����`�F�b�N
	angle_mode = 0;		// �����g���[�X
	start = 0;				// �Q�[�g�X�^�[�g
	
	init_BeepS();			// �u�U�[������
	SET_SCI_C1
	
	// SCI1������
	if ( !init_IMU() ) {
		setBeepPatternS( 0x8000 );
		IMUSet = 1;
	} else {
		setBeepPatternS( 0xcc00 );
		init_SCI1( UART, RATE_230400 );
		IMUSet = 0;
	}
	wait_lcd(100);
	// �t���b�V��������
	if( !initFlash() ) {
		setBeepPatternS( 0x8000 );
		readFlashSetup( 1, 1, 1 ,1 ,1 ,1 ,1);	// �f�[�^�t���b�V������O��p�����[�^��ǂݍ���
	} else{
		setBeepPatternS( 0xcc00 );
	}
	wait_lcd(100);
	// MicroSD�J�[�h������
	if( !init_msd() ) {
		setBeepPatternS( 0x8000 );
		msdset = 1;
	} else {
		setBeepPatternS( 0xcc00 );
		msdset = 0;
	}
	wait_lcd(100);
	while(1){
		__setpsw_i();
		if( pattern >= 11 && pattern <= 99 ) {
			if( !pushcart_mode ) {		
				// �艟�����[�hOFF
				if( cnt1 >= 100 ) {		// �����o���Ă���
					if ( EncoderTotal >= ( PALSE_METER * stopping_meter ) ) { // �������߂̏ꍇ
						error_mode = 0;
						pattern = 101;
					} else if ( cnt_out >= STOP_SENSOR1 ) {	// �Z���T�S��
						error_mode = 1;
						pattern = 101;
					} else if ( cnt_out2 >= STOP_SENSOR2 ) {	// �Z���T�S����
						error_mode = 2;
						pattern = 101;
					} else if ( cnt_out3 >= STOP_ENCODER ) {	// �G���R�[�_��~(�Ђ�����Ԃ����H)
						error_mode = 3;
						pattern = 101;
					} else if( cnt_out4 >= STOP_GYRO ) {	// �}�C�i�X�̉����x���m(�R�[�X���痎�����H)
						error_mode = 4;	
						pattern = 101;
					}
					/*
					// Buletooth�ŊO�������~
					if ( stopWord == 1 ) {
						error_mode = 5;
						pattern = 101;
					}
					*/
					/*
					// ��莞�ԂŒ�~
					if( cnt1 >= STOP_COUNT ) {
						pattern = 101;
					}
					*/
				}
			} else {			
				// �艟�����[�hON
				lcdPosition( 0, 0 );
				lcdPrintf("now %3d", pattern);
				lcdPosition( 0, 1 );
				lcdPrintf("log %3d", logmeter());
			}
			// �X�C�b�`�Œ�~
			if ( tasw_get() == 0x4 ) {
				error_mode = 6;
				pattern = 101;
			}
		} else if ( pattern >= 100 ) {
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
			if ( start && !pushcart_mode ) {
				demo = 0;		// �f�����[�h����
				angle_mode = 0;	// �����g���[�X
				Int = 0;			// �ϕ����Z�b�g
				txt= txt_data;		// ��M�z�񃊃Z�b�g
				cnt_byte = 0;		// ��M�J�E���g���Z�b�g
				
				if ( msdset ) init_log();	// ���O�L�^����
				
				if ( !fixSpeed ) writeFlashBeforeStart(1, 0, 1, 1, 1, 1);	// ���x�p�����[�^���f�[�^�t���b�V���ɕۑ�
				else writeFlashBeforeStart(0, 0, 1, 1, 1, 1);		// ���x�p�����[�^�ȊO��ۑ�
				
				wait_lcd(500);		// 500ms�҂�
				cnt1 = 0;
				pattern = 1;
				break;
			} else if ( start && pushcart_mode ) {
				// �艟�����[�h�̏ꍇ�����ɒʏ�g���[�X
				if ( msdset ) init_log();	// ���O�L�^����
				
				// �����g���[�X�pPID�Q�C���ۑ�
				// �p�x����pPID�Q�C���ۑ�
				writeFlashBeforeStart(0, 0, 1, 1, 0, 0);
				setBeepPatternS( 0xfff0 );
				// �ϐ�������
				init_Parameter( 1 );
				break;
			}
			break;
			
		case 1:
			servoPwmOut( ServoPwm );
			if ( start == 1 ) {
				// �J�E���g�_�E���X�^�[�g
				if ( cnt1 >= 3000 ) {
					setBeepPatternS( 0xfff0 );
					
					// �ϐ�������
					init_Parameter( 0 );
					break;
				} else if ( !(cnt1 % 1000) ) {
					setBeepPatternS( 0x8000 );
					led_out( countdown );
					countdown = countdown >> 1;
					break;
				}
			} else if ( start == 2 ) {
				// �X�^�[�g�Q�[�g�J���X�^�[�g
				pattern = 2;
				break;
			}
			break;
			
		case 2:
			servoPwmOut( ServoPwm );
			// �X�^�[�g�o�[�J�҂�
			if ( !startbar_get() ) {
				// �ϐ�������
				init_Parameter( 0 );
				break;
			}
			// LED�_�ŏ���
			if ( cnt1 >= 2000 ) cnt1 = 0;
			if ( cnt1 < 1000 ) {
				led_out( 0x04 );
			} else {
				led_out( 0x08 );
			}
			break;
		//-------------------------------------------------------------------
		// �y010�z�g���[�X����
		//-------------------------------------------------------------------
		case 11:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_straight * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			led_out( 0x00 );
			
			// �N���X���C���`�F�b�N
			if ( check_crossline() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// �E�n�[�t���C���`�F�b�N
	   		if ( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
	   		if ( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if ( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// �J�[�u�`�F�b�N
			if ( i >=  CURVE_R600_START || i <= -CURVE_R600_START ) {
				enc1 = 0;
				curve_moed = 1;
				pattern = 12;
				break;
			}
			break;
			
		case 12:
			// �J�[�u�u���[�L
			servoPwmOut( ServoPwm );
			targetSpeed = speed_curve_brake * SPEED_CURRENT;
			led_out( 0x1e );
			diff( motorPwm );
			i = getServoAngle();
			
			if ( enc1 > enc_mm( 60 ) ) {		// 60mm�i��
				enc1 = 0;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 13;
				break;
			}
			
			// �N���X���C���`�F�b�N
			if ( check_crossline() ) {
				enc1 = 0;
				pattern = 21;
				break;
			}
			// �E�n�[�t���C���`�F�b�N
	   		if ( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
	   		if ( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			if ( memory_mode ) {
				enc1 = 0;
				pattern = 16;
				break;
			}
			// �����`�F�b�N
			if ( i <  CURVE_R600_START && i > -CURVE_R600_START ) {
				enc1 = 0;
				pattern = 11;
				break;
			}
			break;
			
		case 13:
			// R600�J�[�u���s
			servoPwmOut( ServoPwm );
			targetSpeed = speed_curve_r600 * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			// �N���X���C���`�F�b�N
			if ( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
	   		if ( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
	   		if ( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}
			// R450�`�F�b�N
			if ( i >= CURVE_R450_START || i <= -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 14;
				break;
			}
			// �J�[�u�p���ڃ`�F�b�N
			if ( i <  CURVE_R600_START && i > -CURVE_R600_START ) {
				enc1 = 0;
				pattern = 15;
				break;
			}
			break;
			
		case 14:
			// R450�J�[�u���s
			servoPwmOut( ServoPwm );
			targetSpeed = speed_curve_r450 * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			// �N���X���C���`�F�b�N
			if ( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
	   		if ( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
	   		if ( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// R600�`�F�b�N
			if ( i < CURVE_R450_START && i > -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 13;
				break;
			}
			break;
		
		case 15:
			// �J�[�u�p���ڑ��s
			servoPwmOut( ServoPwm );
			targetSpeed = speed_curve_straight * SPEED_CURRENT;
			diff( motorPwm );
			i = getServoAngle();
			
			if ( enc1 >= enc_mm( 300 ) ) {		// 300mm�i��
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				curve_moed = 0;
				pattern = 11;
				break;
			}
			
			// �N���X���C���`�F�b�N
			if ( check_crossline() ) {
				enc1 = 0;
				setBeepPatternS( 0x8000 );
				pattern = 21;
				break;
      			}
			// �E�n�[�t���C���`�F�b�N
   			if ( check_rightline() ) {
				enc1 = 0;
				pattern = 51;
				break;
			}
			// ���n�[�t���C���`�F�b�N
   			if ( check_leftline() ) {
				enc1 = 0;
				pattern = 61;
				break;
			}
			// �⓹�`�F�b�N
			if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
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
			
		case 16:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_straight * SPEED_CURRENT;
			diff( motorPwm );
			
			break;
		//-------------------------------------------------------------------
		//�y020�z�N�����N���o����
		//-------------------------------------------------------------------
		case 21:
			servoPwmOut( 0 );
			targetSpeed = speed_crossline* SPEED_CURRENT;
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
			targetSpeed = speed_ckank_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( 0x06 );
			
			// �E�N�����N�`�F�b�N
			if( sensor_inp() ==  0x3 ) {
				enc1 = 0;
				led_out( 0x01 );
				setBeepPatternS( 0xa000 );
				SetAngle = angle_rightclank;
				angle_mode = 1;
				TurningAngleEnc = 0;
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
				TurningAngleEnc = 0;
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
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			i = (Encoder * 10) - targetSpeed;	// �ڕW�l�Ƃ̕΍�
			j = getAnalogSensor();
			diff( motorPwm );
			
			if ( IMUSet ) {
				if( -TurningAngleIMU <= 30 ) {
					if( sensor_inp() == 0x2 ) {
						enc1 = 0;
						angle_mode = 0;
						Int = 0;			// �ϕ����Z�b�g
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
			} else {
				if( -TurningAngleEnc <= 30 ) {
					if( sensor_inp() == 0x2 ) {
						enc1 = 0;
						angle_mode = 0;
						Int = 0;			// �ϕ����Z�b�g
						pattern = 36;
						break;
					}
				} else if ( -TurningAngleEnc >= 20 ) {
					if( j <= -1800 ) {
						enc1 = 0;
						i = (short)-TurningAngleEnc;
						pattern = 34;
						break;
					}
				}
			}
			/*
			if ( sensor_inp() == 0x4 ) {
				enc1 = 0;
				pattern = 32;
				break;
			}
			*/
			break;
			
		case 32:
			// �O���ǂݔ�΂�
			SetAngle = angle_rightclank;
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			
			if ( IMUSet ) {
				if( -TurningAngleIMU <= 90 && -TurningAngleIMU >= 40) {
					if( j <= -1800 ) {
						enc1 = 0;
						i = (short)TurningAngleIMU;
						i = -i;
						pattern = 34;
						break;
					}
				}
			} else {
				if( TurningAngleEnc <= 90 && TurningAngleEnc >= 40) {
					if( j <= -1800 ) {
						enc1 = 0;
						i = (short)TurningAngleEnc;
						pattern = 34;
						break;
					}
				}
			}
			break;
			
		case 34:
			// �p�x�ێ�
			// sensor_inp() == 2��ǂ񂾂��ƂɎ��s
			SetAngle = -( 90 - 10 - i ) * (435/35);	// ���C������̊p�x10��
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			
			if( sensor_inp() == 0x2 && j >= -1800) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 36;
				break;
			}
			break;
			
		case 36:
			// ���A
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
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
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			i = (Encoder * 10) - targetSpeed;	// �ڕW�l�Ƃ̕΍�
			j = getAnalogSensor();
			diff( motorPwm );
			
			if ( IMUSet ) {
				if( TurningAngleIMU <= 30 ) {
					if( sensor_inp() == 0x2 ) {
						enc1 = 0;
						angle_mode = 0;
						Int = 0;			// �ϕ����Z�b�g
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
			} else {
				if( TurningAngleEnc <= 30 ) {
					if( sensor_inp() == 0x2 ) {
						enc1 = 0;
						angle_mode = 0;
						Int = 0;			// �ϕ����Z�b�g
						pattern = 46;
						break;
					}
				} else if ( TurningAngleEnc >= 20 ) {
					if( j >= 1800 ) {
						enc1 = 0;
						i = TurningAngleEnc;
						pattern = 44;
						break;
					}
				}
			}
			/*
			if ( sensor_inp() == 0x1 ) {
				enc1 = 0;
				pattern = 42;
				break;
			}
			*/
			break;
			
		case 42:
			// �O���ǂݔ�΂�
			SetAngle = angle_leftclank;
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			
			if ( IMUSet ) {
				if( TurningAngleIMU <= 90 && TurningAngleIMU >= 40) {
					if( j >= 1800 ) {
						enc1 = 0;
						i = TurningAngleIMU;
						pattern = 44;
						break;
					}
				}
			} else {
				if( TurningAngleEnc <= 90 && TurningAngleEnc >= 40) {
					if( j >= 1800 ) {
						enc1 = 0;
						i = TurningAngleEnc;
						pattern = 44;
						break;
					}
				}
			}
			break;
			
		case 44:
			// �p�x�ێ�
			// sensor_inp() == 2��ǂ񂾂��ƂɎ��s
			SetAngle = ( 90 - 10 - i ) * (435/35);	// ���C������̊p�x10��
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			
			if( sensor_inp() == 0x2 && j <= 1800) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 46;
				break;
			}
			break;
			
		case 46:
			// ���A
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
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
			targetSpeed = speed_halfine * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// �ϕ����Z�b�g
				setBeepPatternS( 0xe000 );
				pattern = 52;
				break;
			}
			/*if( enc1 <=  enc_mm( 20 ) ) {
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
			targetSpeed = speed_rightchange_trace * SPEED_CURRENT;
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
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
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
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );
			
			if( sensor_inp() == 0x2 && getAnalogSensor() < 1500 && getAnalogSensor() > -1500 ) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 57;
				break;
			}
			break;
			
		case 55:
			servoPwmOut( 90 );
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
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
			targetSpeed = speed_rightchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 10 ) ) {
				angle_center = getServoAngle();
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 57;
				break;
			}
			break;
			
		case 57:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			/*
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
			}*/
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
			targetSpeed = speed_halfine * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				setBeepPatternS( 0xe400 );
				angle_mode = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 62;
				break;
			}
			/*if( enc1 <=  enc_mm( 20 ) ) {
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
			targetSpeed = speed_leftchange_trace * SPEED_CURRENT;
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
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
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
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );

			if( sensor_inp() == 0x2 && getAnalogSensor() < 1500 && getAnalogSensor() > -1500 ) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 67;
				break;
			}
			break;
			
		case 65:
			servoPwmOut( -90 );
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
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
			targetSpeed = speed_leftchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 10 ) ) {
				enc1 = 0;
				angle_mode = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 67;
				break;
			}
			break;
			
		case 67:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftchange_escape * SPEED_CURRENT;
			diff( motorPwm );
			/*
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
			*/
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
			// �ڕW���x�ς��Ȃ�
			diff( motorPwm );
			if( check_slope() == 1 ) {
				if( slope_mode == 0 ) {
					// ���n��
					slope_mode = 1;
					enc1 = 0;
					led_out( 0x18 );
					setBeepPatternS( 0xe000 );
					pattern = 72;
					break;
				} else if ( slope_mode == 2 && enc_slope >= enc_mm( 600 ) ) {
					// ����I���
					slope_mode = 3;
					enc1 = 0;
					led_out( 0x05 );
					setBeepPatternS( 0xe000 );
					pattern = 74;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
			} else if ( check_slope() == -1 ) {
				if( slope_mode == 1 && enc_slope >= enc_mm( 1000 ) ) {
					// ���I���A����n��
					slope_mode = 2;
					enc1 = 0;
					led_out( 0x05 );
					setBeepPatternS( 0xe000 );
					pattern = 75;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
				break;
			} else {
				enc1 = 0;
				pattern = 11;
				break;
			}
			break;
			
		case 72:
			// �Ⓒ�_�܂ő��s
			servoPwmOut( ServoPwm );
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 1200 ) ) {
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
			targetSpeed = speed_slope_brake * SPEED_CURRENT;
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
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
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
			if ( slope_mode == 3 ) {
				targetSpeed = speed_straight * SPEED_CURRENT;
			} else {
				targetSpeed = speed_slope_trace * SPEED_CURRENT;
			}
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
					pattern = 11;
					break;
				}
			} else if ( slope_mode == 3 ) {
				if( enc1 >= enc_mm( 500 ) ) {
					enc1 = 0;
					enc_slope = 0;
					pattern = 11;
					break;
				}
			} else {
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
			targetSpeed = 0;
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
				cnt1 = 0;
				break;
			}else{
				setBeepPatternS( 0xaa00 );
				pattern = 106;
				break;
			}
			break;
			
		case 104:
			// �Ō�̃f�[�^���������܂��܂ő҂�
			if ( cnt1 <= 1000 ) {	// 500ms�҂�
				if( checkMicroSDProcess() == 11 ) {
					msdFlag = 0;			// ���O�L�^�I��
					microSDProcessEnd();        // microSDProcess�I������
					pattern = 105;
					break;
				}
			} else {			// 500ms�ȏ�o�߂�����I��
				setBeepPatternS( 0xf0f0 );
				pattern = 107;
				break;
			}
			break;
			
		case 105:
			// �I���������I���܂ő҂�
			if( checkMicroSDProcess() == 0 ) {
				// MicroSD�ŏI�������݃A�h���X�ۑ�
				flashDataBuff[ 0 ] = msdWorkaddress >> 16;
				flashDataBuff[ 1 ] = msdWorkaddress & 0xffff;	// �I���A�h���X
				writeFlashData( MSD_STARTAREA, MSD_ENDAREA, MSD_DATA, 2 );
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
			
		case 107:
			// LED�_�ŏ���
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				led_out( 0x15 );
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
///////////////////////////////////////////////////////////////////////////
// ���W���[���� Timer									//
// �����T�v     1ms���ƂɃ^�C�}���荞��						//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void Timer (void) {
	short s;
	
	__setpsw_i();
	//�@�^�C�}�J�E���g
	if ( pattern >= 11 ) {
		if ( pattern <= 99 ) {
			if ( pattern != 21 ) {				// �N���X���C���ʉߎ��͖���
				if ( sensor_inp() == 0x7 || sensor_inp() == 0x5 ) {	// �Z���T�S��
					cnt_out++;	
				} else {
					cnt_out = 0;
				}
			}
			if ( sensor_inp() == 0x0 && pattern != 53 && pattern != 63 ) cnt_out2++;	// �Z���T�S����
			else cnt_out2 = 0;
			if ( Encoder <= 1 && Encoder >= -1 ) cnt_out3++;		// �G���R�[�_��~(�Ђ�����Ԃ����H)
			else cnt_out3 = 0;
			s = (short)RollAngleIMU;
			if ( s >= 5 || s <= -5 ) cnt_out4++;
			else	cnt_out4 = 0;
		}
		// �E�H�b�`�h�b�O�^�C�}�̣�J�E���g���t���b�V��
		R_PG_Timer_RefreshCounter_IWDT();
	} else if ( pattern < 11 ) {
		cnt_setup++;
		cnt_setup2++;
		cnt_setup3++;
		cnt_swR++;
		cnt_swL++;
		cnt_flash++;
	}
	cnt0++;
	cnt1++;
	cnt_gyro++;
			
	// LCD�\��
	if ( lcd_mode ) lcdShowProcess();

	// �G���R�[�_�J�E���g
	getEncoder();

	// PID����l�Z�o
	if ( angle_mode ) servoControl2();		// �p�x
	else 			servoControl();	// ����
	motorControl();		// ���[�^
	
	// MicroSD��������
	microSDProcess();
	if ( msdFlag ) sendLog();
	
	Timer10++;
	
	// �ʐM
	if ( IMUSet ) {
		// I2C�ʐM�ŉ����x�y�ъp���x���擾
		if ( Timer10 % 5 == 0 ) {
			IMUProcess();
			getTurningAngleIMU();
			getPichAngleIMU();
			getRollAngleIMU();
			if (cnt_gyro > 200) {
				RollAngleIMU = 0;
				PichAngleIMU = 0;
				cnt_gyro  = 0;
			}
		}
	} else {
		// UART��M
		commandSCI1();
		getPichAngleAD();
		getTurningAngleEnc();
		if (cnt_gyro > 200) {
			PichAngleAD = 0;
			cnt_gyro  = 0;
		}
	}

	// 10�����ƂɎ��s
	switch ( Timer10 ) {	
	case 1:
		// �u�U�[
		beepProcessS();
		break;
	case 2:
		// �X�C�b�`�ǂݍ���
		getSwitch();
		break;
	case 3:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		Timer10 = 0;
		break;
	default:
		break;
	}

}

///////////////////////////////////////////////////////////////////////////
// ���W���[���� init_Parameter							//
// �����T�v     �ϐ��̏�����								//
// ����         lcd: 1 lcd�\��  0 lcd��\��						//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void init_Parameter ( bool lcd ) {
	cntmpattern2 = 0;	// �L�^���s�J�E���g���Z�b�g
	EncoderTotal = 10;	// �����s����
	cnt1 = 0;			// �^�C�}���Z�b�g
	enc1 = 0;			// ��ԋ������Z�b�g
	lcd_mode = lcd;		// LCD�\��OFF
	msdFlag = 1;		// �f�[�^�L�^�J�n
	targetSpeed = speed_straight * SPEED_CURRENT; // �ڕW���x�ݒ�
	
	// �p�x�ώZ�l���Z�b�g
	TurningAngleIMU = 0;
	RollAngleIMU = 0;
	PichAngleIMU = 0;
	R_PG_Timer_Start_IWDT();	// �Ɨ��E�H�b�`�h�b�O�^�C�}�̃J�E���g�X�^�[�g
	pattern = 11;		// �ʏ푖�s
}