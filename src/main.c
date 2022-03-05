//====================================//
// �C���N���[�h
//====================================//
#include "io.h"
#include "mtu.h"
#include "ADconverter.h"

#include "control.h"
#include "setup.h"
#include "sci.h"
#include "E2dataFlash.h"
#include "AQM1602Y.h"
#include "microSD.h"
#include "ICM20648.h"
#include "MemorryTrace.h"
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
// ���s�p�^�[���֘A
char		pattern = 0;	// �p�^�[���ԍ�
char		countdown = 0x0;
short 		angle_center;

// ���[�h�֘A
char		mode_curve;		// �J�[�u����	0:�J�[�u�ȊO	1:�J�[�u���s��
char		mode_error;		// 0:������~ 1:�Z���T�S�� 2:�Z���T�S���� 3:�G���R�[�_��~ 4:�W���C������
char 		mode_autoMotor;	// 0: switch���ŃT�[�{�A�쓮���[�^��PWM�o�͂��w�肷�� 1: Timer�֐����Ŏ����I��PWM�o�͂����s

// �^�C�}�֘A
// 1ms�^�C�}
unsigned int 		cnt1;		// ���s�p�^�C�}�J�E���g
unsigned short	 	cnt_out;	// �R�[�X�A�E�g����p�^�C�}
unsigned short	 	cnt_out2;	// �R�[�X�A�E�g����p�^�C�}2
unsigned short	 	cnt_out3;	// �R�[�X�A�E�g����p�^�C�}3
unsigned short	 	cnt_out4;	// �R�[�X�A�E�g����p�^�C�}4
static char			Timer10;	// 1ms�J�E���g�p
//====================================//
// �v���g�^�C�v�錾
//====================================//
void init_Parameter ( bool lcd );
//====================================//
// ���C���v���O����
//====================================//
void main(void){
	short i, j;
	unsigned int ui;
	
	//=================================//
	// ������
	//=================================//
	L_Sen_ON;		//�Z���T�_��
	inti_lcd();		// LCD������
	
	motorPwmOut(0, 0, 0, 0);	// ���[�^�[��~
	servoPwmOut( 0 );
	
	mode_pushcart = 0;		// �艟�����[�hoff
	mode_slope = 0;			// ����`�F�b�N
	mode_angle = 0;			// �����g���[�X
	mode_autoMotor = 0;		// ����PWM�o�͒�~
	start = 0;				// �Q�[�g�X�^�[�g
	
	//IMU������
	IMU_init();
	// �t���b�V��������
	// �f�[�^�t���b�V������O��p�����[�^��ǂݍ���
	if( !initFlash() ) readFlashSetup( 1, 1, 1 ,1 ,1 ,1 ,1);
	// MicroSD�J�[�h������
	if( !init_msd() ) msdset = 1;
	else msdset = 0;

	// �d���d���̊m�F
	if (Voltage < LOWVOLTAGE ) {
		lcdRowPrintf(UPROW, "LOW BAT ");
		lcdRowPrintf(LOWROW, "  %05.2fV",Voltage);
		led_out(LED_R);
		while(1);
	} else {
		cnt1=0;
		while( cnt1 < 1500){
			lcdRowPrintf(UPROW, "Voltage ");
			lcdRowPrintf(LOWROW, "  %05.2fV",Voltage);
		}
	}

	cnt1=0;
	while(msdset == 1 && cnt1 < 1500){
		lcdRowPrintf(UPROW, " SYSTEM ");
		lcdRowPrintf(LOWROW, "ALLGREEN");
		led_out(LED_G);
	}
	led_out(0);
	
	while(1){
		__setpsw_i();
		if( pattern >= 11 && pattern <= 99 ) {
			if( !mode_pushcart ) {		
				// �艟�����[�hOFF
				if( cnt1 >= 100 ) {		// �����o���Ă���
					if ( EncoderTotal >= ( PALSE_METER * stopping_meter ) ) { // �������߂̏ꍇ
						mode_error = 0;
						pattern = 101;
						mode_autoMotor = 0;
					} else if ( cnt_out >= STOP_SENSOR1 ) {	// �Z���T�S��
						mode_error = 1;
						pattern = 101;
						mode_autoMotor = 0;
					} else if ( cnt_out2 >= STOP_SENSOR2 ) {	// �Z���T�S����
						mode_error = 2;
						pattern = 101;
						mode_autoMotor = 0;
					} else if ( cnt_out3 >= STOP_ENCODER ) {	// �G���R�[�_��~(�Ђ�����Ԃ����H)
						mode_error = 3;
						pattern = 101;
						mode_autoMotor = 0;
					} else if( cnt_out4 >= STOP_GYRO ) {	// �}�C�i�X�̉����x���m(�R�[�X���痎�����H)
						mode_error = 4;	
						pattern = 101;
						mode_autoMotor = 0;
					}
					/*
					// Buletooth�ŊO�������~
					if ( stopWord == 1 ) {
						mode_error = 5;
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
			if ( cnt1 >= 1000 && tasw_get() == SW_PUSH ) {
				mode_error = 6;
				pattern = 101;
			}
		} else if ( pattern >= 100 ) {
			mode_lcd = 1;
			lcdPosition( 0, 0 );
			lcdPrintf("TIME  %d", mode_error);
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
			if ( start && !mode_pushcart ) {
				demo = 0;		// �f�����[�h����
				mode_angle = 0;	// �����g���[�X
				Int = 0;			// �ϕ����Z�b�g
				txt= txt_data;		// ��M�z�񃊃Z�b�g
				cnt_byte = 0;		// ��M�J�E���g���Z�b�g
				
				if ( msdset ) init_log();	// ���O�L�^����
				
				if ( !fixSpeed ) writeFlashBeforeStart(1, 0, 1, 1, 1, 1);	// ���x�p�����[�^���f�[�^�t���b�V���ɕۑ�
				else writeFlashBeforeStart(0, 0, 1, 1, 1, 1);		// ���x�p�����[�^�ȊO��ۑ�
				
				//if (IMUSet) caribrateIMU();
				
				wait_lcd(500);		// 500ms�҂�
				cnt1 = 0;
				pattern = 1;
				break;
			} else if ( start && mode_pushcart ) {
				// �艟�����[�h�̏ꍇ�����ɒʏ�g���[�X
				if ( msdset ) init_log();	// ���O�L�^����
				
				// �����g���[�X�pPID�Q�C���ۑ�
				// �p�x����pPID�Q�C���ۑ�
				writeFlashBeforeStart(0, 0, 1, 1, 0, 0);
				// �ϐ�������
				init_Parameter( 1 );
				break;
			}
			break;
			
		case 1:
			servoPwmOut( ServoPwm );
			if ( start == START_COUNT ) {
				// �J�E���g�_�E���X�^�[�g
				if ( cnt1 >= 3000 ) {	
					// �ϐ�������
					init_Parameter( 0 );
					break;
				} else if ( !(cnt1 % 1000) ) {
					led_out( countdown );
					countdown = countdown << 1;
					break;
				}
			} else if ( start == START_GATE ) {
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
				led_out( LED_R );
			} else {
				led_out( LED_B  );
			}
			break;
		//-------------------------------------------------------------------
		// �y010�z�g���[�X����
		//-------------------------------------------------------------------
		case 11:
			targetSpeed = speed_straight * SPEED_CURRENT;
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
			/*if ( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
			// �J�[�u�`�F�b�N
			if ( i >=  CURVE_R600_START || i <= -CURVE_R600_START ) {
				enc1 = 0;
				mode_curve = 1;
				pattern = 12;
				break;
			}
			break;
			
		case 12:
			// �J�[�u�u���[�L
			targetSpeed = speed_curve_brake * SPEED_CURRENT;
			led_out( LED_R );
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
			/*if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
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
			targetSpeed = speed_curve_r600 * SPEED_CURRENT;
			i = getServoAngle();
			
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
			/*if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
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
			targetSpeed = speed_curve_r450 * SPEED_CURRENT;
			i = getServoAngle();
			
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
			// R600�`�F�b�N
			if ( i < CURVE_R450_START && i > -CURVE_R450_START ) {
				enc1 = 0;
				pattern = 13;
				break;
			}
			break;
		
		case 15:
			// �J�[�u�p���ڑ��s
			targetSpeed = speed_curve_straight * SPEED_CURRENT;
			i = getServoAngle();
			
			if ( enc1 >= enc_mm( 300 ) ) {		// 300mm�i��
				enc1 = 0;
				mode_curve = 0;
				pattern = 11;
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
			/*if ( EncoderTotal >= 5609 ) {
				if ( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
			// �J�[�u�`�F�b�N
			if( i >=  CURVE_R600_START || i <= - CURVE_R600_START ) {
				enc1 = 0;
				pattern = 13;
				break;
			}
			break;
			
		case 16:
			targetSpeed = speed_straight * SPEED_CURRENT;
			
			break;
		//-------------------------------------------------------------------
		//�y020�z�N�����N���o����
		//-------------------------------------------------------------------
		case 21:
			targetSpeed = speed_crossline* SPEED_CURRENT;
			led_out( LED_G );
			
			if( enc1 > enc_mm( 90 ) ) {		// 60mm�i��
				enc1 = 0;
				pattern = 22;
				break;
			}
			break;
			
		case 22:
			targetSpeed = speed_ckank_trace * SPEED_CURRENT;
			
			// �E�N�����N�`�F�b�N
			if( sensor_inp() ==  0x3 ) {
				enc1 = 0;
				led_out( LED_R | LED_G);
				SetAngle = angle_rightclank;
				mode_angle = 1;
				TurningAngleEnc = 0;
				TurningAngleIMU = 0;
				pattern = 31;
				break;
			}
			// ���N�����N�`�F�b�N
			if( sensor_inp() ==  0x6 ) {
				enc1 = 0;
				led_out( LED_G | LED_B );
				SetAngle = angle_leftclank;
				mode_angle = 1;
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
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			i = -TurningAngleIMU;
			j = getAnalogSensor();
			
			if (sensor_inp() == 0x2) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 36;
				break;
			}
			break;
			
		case 32:
			// �O���ǂݔ�΂�
			SetAngle = angle_rightclank;
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			j = getAnalogSensor();
			
			if( -TurningAngleIMU <= 90 && -TurningAngleIMU >= 40) {
				if( j <= -1800 && sensor_inp() == 0x2 ) {
					enc1 = 0;
					i = (short)TurningAngleIMU;
					i = -i;
					j = getServoAngle();
					pattern = 36;
					break;
				}
			}
			break;
			
		case 34:
			// �p�x�ێ�
			// sensor_inp() == 2��ǂ񂾂��ƂɎ��s
			SetAngle = -( 90 - 10 - i ) * DEGPERAD;	// ���C������̊p�x10��
			//SetAngle = angle_rightclank + 160;
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
			j = getAnalogSensor();
			
			if( sensor_inp() == 0x2 && j >= -1800 ) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 36;
				break;
			}
			break;
			
		case 36:
			// ���A
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;

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
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			i = -TurningAngleIMU;
			j = getAnalogSensor();
			
			if ( i >= 20 ) {
				if( j >= 1800 && sensor_inp() == 0x2 ) {
					enc1 = 0;
					i = TurningAngleIMU;
					pattern = 44;
					break;
				}
			}
			break;
			
		case 42:
			// �O���ǂݔ�΂�
			SetAngle = angle_leftclank;
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			j = getAnalogSensor();
			
			if( TurningAngleIMU <= 90 && TurningAngleIMU >= 40) {
				if( j >= 1800 && sensor_inp() == 0x2 ) {
					enc1 = 0;
					i = TurningAngleIMU;
					pattern = 46;
					break;
				}
			}
			break;
			
		case 44:
			// �p�x�ێ�
			// sensor_inp() == 2��ǂ񂾂��ƂɎ��s
			SetAngle = ( 90 - 10 - i ) * DEGPERAD;	// ���C������̊p�x10��
			//SetAngle = angle_leftclank - 160;
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			j = getAnalogSensor();
			
			if( sensor_inp() == 0x2 && j <= 1800) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 46;
				break;
			}
			break;
			
		case 46:
			// ���A
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			
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
			targetSpeed = speed_halfine * SPEED_CURRENT;
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 52;
				break;
			}
			
			if( check_crossline() ) {		// �N���X���C���`�F�b�N
				enc1 = 0;
				pattern = 21;
				break;
			}
			break;
			
		case 52:
			targetSpeed = speed_rightchange_trace * SPEED_CURRENT;
			led_out( LED_R );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				mode_angle = 1;
				pattern = 53;
				break;
			}
			break;
			
		case 53:
			SetAngle = angle_rightchange;
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x1 ) {
				enc1 = 0;
				pattern = 54;
				break;
			}
			break;
			
		case 54:
			SetAngle = 0;
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x4 ) {
				enc1 = 0;
				mode_angle = 0;
				mode_autoMotor = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 55;
				break;
			}
			break;
			
		case 55:
			servoPwmOut( 90 );
			targetSpeed = speed_rightchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x2 ) {
				angle_center = getServoAngle();
				enc1 = 0;
				mode_autoMotor = 1;
				pattern = 56;
				break;
			}
			break;
			
		case 56:
			targetSpeed = speed_rightchange_escape * SPEED_CURRENT;

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
			targetSpeed = speed_halfine * SPEED_CURRENT;
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				mode_angle = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 62;
				break;
			}
			if( check_crossline() ) {		// �N���X���C���`�F�b�N
				enc1 = 0;
				pattern = 21;
				break;
			}
			break;
			
		case 62:
			targetSpeed = speed_leftchange_trace * SPEED_CURRENT;
			led_out( LED_B );
			
			if( sensor_inp() == 0x0 ) {
				enc1 = 0;
				mode_angle = 1;
				pattern = 63;
				break;
			}
			break;
			
		case 63:
			SetAngle = angle_leftchange;
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x4 ) {
				enc1 = 0;
				pattern = 64;
				break;
			}
			break;
			
		case 64:
			SetAngle = 0;
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;

			if( sensor_inp() == 0x1 ) {
				enc1 = 0;
				mode_angle = 0;
				mode_autoMotor = 0;
				Int = 0;			// �ϕ����Z�b�g
				pattern = 65;
				break;
			}
			break;
			
		case 65:
			servoPwmOut( -90 );
			targetSpeed = speed_leftchange_curve * SPEED_CURRENT;
			
			if( sensor_inp() == 0x2 ) {
				angle_center = getServoAngle();
				enc1 = 0;
				mode_autoMotor = 1;
				pattern = 66;
				break;
			}
			break;
			
		case 66:
			targetSpeed = speed_leftchange_escape * SPEED_CURRENT;

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
			// �ڕW���x�ς��Ȃ�
			if( check_slope() == 1 ) {
				if( mode_slope == 0 ) {
					// ���n��
					mode_slope = 1;
					enc1 = 0;
					led_out( LED_R | LED_B );
					pattern = 72;
					break;
				} else if ( mode_slope == 2 && enc_slope >= enc_mm( 600 ) ) {
					// ����I���
					mode_slope = 3;
					enc1 = 0;
					led_out( LED_R | LED_G | LED_B);
					pattern = 74;
					break;
				} else {
					enc1 = 0;
					pattern = 11;
					break;
				}
			} else if ( check_slope() == -1 ) {
				if( mode_slope == 1 && enc_slope >= enc_mm( 1000 ) ) {
					// ���I���A����n��
					mode_slope = 2;
					enc1 = 0;
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
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			
			if( enc1 >= enc_mm( 1200 ) ) {
				enc1 = 0;
				pattern = 73;
				break;
			}
			break;
			
		case 73:
			// ����I�_�u���[�L �S�̓u���[�L
			targetSpeed = 0;

			if( enc1 >= enc_mm( 50 ) ) {
				enc1 = 0;
				pattern = 75;
				break;
			}
			break;
			
		case 74:
			// �����I�_�u���[�L
			targetSpeed = speed_slope_brake * SPEED_CURRENT;
			if( enc1 >= enc_mm( 40 ) ) {
				enc1 = 0;
				pattern = 75;
				break;
			}
			break;
			
		case 75:
			// �W���C���Z���T�����肷��܂œǂݔ�΂�
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			
			if( enc1 >= enc_mm( 150 ) ) {
				enc1 = 0;
				pattern = 76;
				break;
			}
			break;
			
		case 76:
			// �W���C���Z���T�����肷��܂œǂݔ�΂�
			if ( mode_slope == 3 ) {
				targetSpeed = speed_straight * SPEED_CURRENT;
			} else {
				targetSpeed = speed_slope_trace * SPEED_CURRENT;
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
			if( mode_slope  == 0 ) {
				if( enc1 >= enc_mm( 1000 ) ) {
					enc1 = 0;
					enc_slope = 0;
					pattern = 11;
					break;
				}
			} else if ( mode_slope == 3 ) {
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
			
			LEDR_OFF;
			LEDG_OFF;
			LEDB_OFF;
	
			pattern = 102;
			break;
			
		case 102:
			servoPwmOut( ServoPwm );
			targetSpeed = 0;
			motorPwmOut(motorPwm, motorPwm, motorPwm, motorPwm);
			
			if( Encoder <= 0 && Encoder >= -1 ) {
				enc1 = 0;
				pattern = 103;
				break;
			}
			break;
			
		case 103:
			servoPwmOut( ServoPwm );
			motorPwmOut(0, 0, 0, 0);
			
			if( msdset == 1 ) {
				pattern = 104;
				cnt1 = 0;
				break;
			}else{
				pattern = 106;
				break;
			}
			break;
			
		case 104:
			servoPwmOut( ServoPwm );
			// �Ō�̃f�[�^���������܂��܂ő҂�
			if ( cnt1 <= 1000 ) {	// 500ms�҂�
				if( checkMicroSDProcess() == 11 ) {
					msdFlag = 0;			// ���O�L�^�I��
					microSDProcessEnd();        // microSDProcess�I������
					pattern = 105;
					break;
				}
			} else {			// 500ms�ȏ�o�߂�����I��
				servoPwmOut( 0 );
				R_PG_IO_PORT_Write_PE6( 0 );	//�T�[�{���[�^ free���[�h
				pattern = 107;
				break;
			}
			break;
			
		case 105:
			servoPwmOut( ServoPwm );
			// �I���������I���܂ő҂�
			if( checkMicroSDProcess() == 0 ) {
				// MicroSD�ŏI�������݃A�h���X�ۑ�
				flashDataBuff[ 0 ] = msdWorkaddress >> 16;
				flashDataBuff[ 1 ] = msdWorkaddress & 0xffff;	// �I���A�h���X
				writeFlashData( MSD_STARTAREA, MSD_ENDAREA, MSD_DATA, 2 );
				servoPwmOut( 0 );
				R_PG_IO_PORT_Write_PE6( 0 );	//�T�[�{���[�^ free���[�h
				pattern = 106;
				break;
			}
			break;
			
		case 106:
			// mMicroSD�������ݐ���
			// LED�_�ŏ���
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				LEDB_ON;
			}else{
				LEDB_OFF;
			}
			break;
			
		case 107:
			// mMicroSD�������ݎ��s
			// LED�_�ŏ���
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				LEDR_ON;
			}else{
				LEDR_OFF;
			}
			break;
			
		default:
			pattern = 101;
			break;
			
	} // end of "switch ( pattern )"
	} // end of "while ( 1 )"
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� Timer
// �����T�v     1ms���ƂɃ^�C�}���荞��
// ����         �Ȃ�
// �߂�l       �Ȃ�
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
	} else if ( pattern < 11 ) {
		cnt_setup++;
		cnt_setup2++;
		cnt_setup3++;
		cnt_swR++;
		cnt_swL++;
		cnt_flash++;
	}
	cnt1++;
	cnt_gyro++;
			
	// LCD�\��
	if ( mode_lcd ) lcdShowProcess();

	// �G���R�[�_�J�E���g
	getEncoder();

	// PID����l�Z�o
	if ( mode_angle ) servoControl2();	// �p�x
	else servoControl();		// ����
	motorControl();		// ���[�^

	// ���s����PWM�o��
	if ( mode_autoMotor ) {
		if ( mode_angle ) servoPwmOut( ServoPwm2 );	// �p�x
		else servoPwmOut( ServoPwm );	// ����
		if (!mode_pushcart) {
			diff( motorPwm ); // �쓮�փ��[�^PWM�o��
		}
	}
	
	// MicroSD��������
	microSDProcess();
	if ( msdFlag ) sendLog( 8, 8, 3
					// char
					, pattern
					, motorPwm
					, sensor_inp()
					, mode_slope
					, (char)Encoder
					, sPwm
					, (char)(PichAngleIMU*10)
					, (char)(RollAngleIMU*10)
					//short
					, (short)(TurningAngleIMU*10)
					, xg
					, yg
					, zg
					, getServoAngle()
					, SetAngle
					, getAnalogSensor()
					, (short)(Voltage*100)
					// unsigned int
					, EncoderTotal
					, enc1
					, cnt_log
					);
	
	Timer10++;
	
	// �ʐM
	// �����x�y�ъp���x���擾
	
		
	// 10�����ƂɎ��s
	switch ( Timer10 ) {	
	case 1:
		getSwitch();		// �X�C�b�`�ǂݍ���
		get_voltage();		// �d���d���擾
		break;
	case 2:
		read_gyro_data();
		read_accel_data();
		getTurningAngleIMU();
		getPichAngleIMU();
		getRollAngleIMU();
		if (cnt_gyro > 200) {
			RollAngleIMU = 0;
			PichAngleIMU = 0;
			cnt_gyro  = 0;
		}
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
// ���W���[���� init_Parameter
// �����T�v     �ϐ��̏�����
// ����         lcd: 1 lcd�\��  0 lcd��\��
// �߂�l       �Ȃ�
///////////////////////////////////////////////////////////////////////////
void init_Parameter ( bool lcd ) {
	cntmpattern2 = 0;	// �L�^���s�J�E���g���Z�b�g
	EncoderTotal = 10;	// �����s����
	cnt1 = 0;			// �^�C�}���Z�b�g
	enc1 = 0;			// ��ԋ������Z�b�g
	mode_lcd = lcd;		// LCD�\��OFF
	mode_autoMotor = 1; // PWM�o��
	msdFlag = 1;		// �f�[�^�L�^�J�n
	targetSpeed = speed_straight * SPEED_CURRENT; // �ڕW���x�ݒ�
	
	// �p�x�ώZ�l���Z�b�g
	TurningAngleIMU = 0;
	RollAngleIMU = 0;
	PichAngleIMU = 0;
	pattern = 11;		// �ʏ푖�s
}