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
#include "SPI_ICM20648.h"
#include "MemorryTrace.h"
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// ���s�p�^�[���֘A
char		pattern = 0;	// �p�^�[���ԍ�
char		countdown = 0x0;
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
// �v���g�^�C�v�錾									//
//====================================//
void init_Parameter ( bool lcd );
//====================================//
// ���C���v���O����									//
//====================================//
void main(void){
	short i, j;
	unsigned int ui;
	
	//=================================//
	// ������									//
	//=================================//
	L_Sen_ON;		//�Z���T�_��
	inti_lcd();			// LCD������
	
	motor_f( 0, 0 );		// ���[�^�[��~
	motor_r( 0, 0 );
	servoPwmOut( 0 );
	
	pushcart_mode = 0;		// �艟�����[�hoff
	angle_mode = 0;		// �����g���[�X
	start = 0;				// �Q�[�g�X�^�[�g
	
	led_out(0);
	
	while(1){
	__setpsw_i();	// ���d���荞�݋���
	switch( pattern ) {
		//-------------------------------------------------------------------
		// �y000�z�X�^�[�g����
		//-------------------------------------------------------------------
		case 0:
			// �X�^�[�g�O�ݒ�
			setup();
			if ( start && !pushcart_mode ) {
				cnt1 = 0;
				pattern = 1;
				break;
			} else if ( start && pushcart_mode ) {
				// �艟�����[�h�̏ꍇ�����ɒʏ�g���[�X
				// �ϐ�������
				init_Parameter( 1 );
				pattern = 11;		// �ʏ푖�s
				break;
			}
			break;
			
		case 1:
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
			break;
		//-------------------------------------------------------------------
		//�y020�z�N�����N���o����
		//-------------------------------------------------------------------
		case 21:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_crossline* SPEED_CURRENT;
			diff( motorPwm );
			led_out( LED_G );
			
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
			
			// �E�N�����N�`�F�b�N
			if( sensor_inp() ==  0x3 ) {
				enc1 = 0;
				led_out( LED_R | LED_G);
				SetAngle = angle_rightclank;
				angle_mode = 1;
				pattern = 31;
				break;
			}
			// ���N�����N�`�F�b�N
			if( sensor_inp() ==  0x6 ) {
				enc1 = 0;
				led_out( LED_G | LED_B );
				SetAngle = angle_leftclank;
				angle_mode = 1;
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
			i = -TurningAngleIMU;
			j = getAnalogSensor();
			diff( motorPwm );
			
			if ( i >= 20 ) {
				if( j <= -1800 && sensor_inp() == 0x2 ) {
					enc1 = 0;
					i = (short)-TurningAngleIMU;
					pattern = 34;
					break;
				}
			}
			break;
			
		case 32:
			// �O���ǂݔ�΂�
			SetAngle = angle_rightclank;
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			
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
			SetAngle = -( 90 - 10 - i ) * (435/35);	// ���C������̊p�x10��
			//SetAngle = angle_rightclank + 160;
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			
			if( sensor_inp() == 0x2 && j >= -1800 ) {
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
			i = -TurningAngleIMU;
			j = getAnalogSensor();
			diff( motorPwm );
			
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
			servoPwmOut( ServoPwm2 );
			j = getAnalogSensor();
			diff( motorPwm );
			
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
			SetAngle = ( 90 - 10 - i ) * (435/35);	// ���C������̊p�x10��
			//SetAngle = angle_leftclank - 160;
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
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightchange_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( LED_R );
			
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

			// �⓹�`�F�b�N
			/*if( EncoderTotal >= 5609 ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
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
				angle_mode = 0;
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
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftchange_trace * SPEED_CURRENT;
			diff( motorPwm );
			led_out( LED_B );
			
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

			// �⓹�`�F�b�N
			/*if( EncoderTotal >= enc_mm( 1000 ) ) {
				if( check_slope() == 1 || check_slope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
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
					led_out( LED_R | LED_B );
					pattern = 72;
					break;
				} else if ( slope_mode == 2 && enc_slope >= enc_mm( 600 ) ) {
					// ����I���
					slope_mode = 3;
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
				if( slope_mode == 1 && enc_slope >= enc_mm( 1000 ) ) {
					// ���I���A����n��
					slope_mode = 2;
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
			servoPwmOut( ServoPwm );
			targetSpeed = speed_slope_trace * SPEED_CURRENT;
			diff( motorPwm );
			
			if( enc1 >= enc_mm( 1200 ) ) {
				enc1 = 0;
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
			
			LEDR_OFF;
			LEDG_OFF;
			LEDB_OFF;
	
			pattern = 102;
			break;
			
		case 102:
			servoPwmOut( ServoPwm );
			targetSpeed = 0;
			motor_f( motorPwm, motorPwm );
			motor_r( motorPwm, motorPwm );
			
			if( Encoder <= 0 && Encoder >= -1 ) {
				enc1 = 0;
				pattern = 103;
				break;
			}
			break;
			
		case 103:
			servoPwmOut( ServoPwm );
			motor_f( 0, 0 );
			motor_r( 0, 0 );
			
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
				pattern = 106;
				break;
			}
			break;
			
		case 106:
			servoPwmOut( 0 );
			// LED�_�ŏ���
			if( cnt1 >= 200 ) cnt1 = 0;
			if( cnt1 < 100 ) {
				LEDB_ON;
			}else{
				LEDB_OFF;
			}
			break;
			
		case 107:
			servoPwmOut( 0 );
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
// ���W���[���� Timer									//
// �����T�v     1ms���ƂɃ^�C�}���荞��						//
// ����         �Ȃ�										//
// �߂�l       �Ȃ�										//
///////////////////////////////////////////////////////////////////////////
void Timer (void) {
	short s;
	
	__setpsw_i();
	//�@�^�C�}�J�E���g
	if ( pattern >= 11 ) {
		
	} else if ( pattern < 11 ) {
		cnt_setup++;
		cnt_setup2++;
		cnt_setup3++;
		cnt_swR++;
		cnt_swL++;
	}
	cnt0++;
	cnt1++;
			
	// LCD�\��
	if ( lcd_mode ) lcdShowProcess();

	// �G���R�[�_�J�E���g
	getEncoder();

	// PID����l�Z�o
	if ( angle_mode ) servoControl2();	// �p�x
	else 			servoControl();		// ����
	motorControl();		// ���[�^
	
	
	Timer10++;
	// 10�����ƂɎ��s
	switch ( Timer10 ) {	
	case 1:
		getSwitch();		// �X�C�b�`�ǂݍ���
		get_voltage();		// �d���d���擾
		break;
	case 2:
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
	EncoderTotal = 10;	// �����s����
	cnt1 = 0;			// �^�C�}���Z�b�g
	enc1 = 0;			// ��ԋ������Z�b�g
	lcd_mode = lcd;		// LCD�\��OFF
	angle_mode = 0;	// �����g���[�X
	targetSpeed = speed_straight * SPEED_CURRENT; // �ڕW���x�ݒ�
}