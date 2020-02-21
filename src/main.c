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
#include "ADC.h"
#include "IO.h"
#include "MOTOR.h"
#include "Rotaryencoder.h"
#include "Timer.h"
#include "Control.h"
#include "SetUp.h"
#include "SCI.h"
#include "I2C_LCD.h"
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// ���s�p�^�[���֘A
char		pattern = 0;	// �p�^�[���ԍ�
char		countdown = 0x0;
short 	angle_center;

//====================================//
// �v���g�^�C�v�錾									//
//====================================//
void initStatus ( bool lcd );
//====================================//
// ���C���v���O����									//
//====================================//
void main(void){
	//=================================//
	// ������									//
	//=================================//
	L_Sen_ON;		//�Z���T�_��
	inti_lcd();			// LCD������
	lcd_mode = 1;
	
	motor_f( 0, 0 );		// ���[�^�[��~
	motor_r( 0, 0 );
	servoPwmOut( 0 );
	
	pushcart_mode = 0;	// �艟�����[�hoff
	angle_mode = 0;	// �����g���[�X
	start = 0;			// �Q�[�g�X�^�[�g
	
	initParameter();		// �e�p�����[�^������
	
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
				initStatus( 1 );
				pattern = 11;		// �ʏ푖�s
				break;
			}
			break;
			
		case 1:
			servoPwmOut( ServoPwm );
			// �X�^�[�g�o�[�J�҂�
			if ( !startbar_get() ) {
				// �ϐ�������
				initStatus( 0 );
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
			servoPwmOut( ServoPwm );		// �T�[�{����
			targetSpeed = speed_straight * SPEED_CURRENT;	// �ڕW���x
			motor_f(motorPwm, motorPwm);	// �쓮���[�^����
			motor_r(motorPwm, motorPwm);			
			led_out( 0x00 );					// LED����
			
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			led_out( LED_G );
			
			if( enc1 > enc_mm( 90 ) ) {		// 90mm�i��
				enc1 = 0;
				pattern = 22;
				break;
			}
			break;
			
		case 22:
			servoPwmOut( ServoPwm );
			targetSpeed = speed_ckank_trace * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
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
			SetAngle = angle_rightclank;		// �ڕW�p�x
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_rightclank_curve * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if ( sensor_inp() == 0x2 ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 32;
				break;
			}
			break;
			
		case 32:
			// ���A
			servoPwmOut( ServoPwm );
			targetSpeed = speed_rightclank_escape * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);

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
			SetAngle = angle_leftclank;		// �ڕW�p�x
			servoPwmOut( ServoPwm2 );
			targetSpeed = speed_leftclank_curve * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if ( sensor_inp() == 0x2 ) {
				enc1 = 0;
				angle_mode = 0;
				pattern = 42;
				break;
			}
			break;
			
		case 42:
			// ���A
			servoPwmOut( ServoPwm );
			targetSpeed = speed_leftclank_escape * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);

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
			servoPwmOut( ServoPwm );
			targetSpeed = speed_halfine * SPEED_CURRENT;
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				angle_mode = 0;
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);

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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
			if( enc1 > enc_mm( 60 ) ) {
				enc1 = 0;
				angle_mode = 0;
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);
			
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
			motor_f(motorPwm, motorPwm);
			motor_r(motorPwm, motorPwm);

			if( enc1 >= enc_mm( 600 ) ) {
				enc1 = 0;
				led_out( 0x0 );
				pattern = 11;
				break;
			}
			break;
		//-------------------------------------------------------------------
		//�y100�z��~����
		//-------------------------------------------------------------------
		case 101:
			enc1 = 0;	
			
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
			
			pattern = 106;
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
			
		default:
			pattern = 101;
			break;
			
	} // end of "switch ( pattern )"
	} // end of "while ( 1 )"
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� init_Parameter							//
// �����T�v     �ϐ��̏�����								//
// ����         lcd: 1 lcd�\��  0 lcd��\��						//
// �߂�l       �Ȃ�										//
///////////////////////////////////////////////////////////////////////////
void initStatus ( bool lcd ) {
	EncoderTotal = 10;	// �����s����
	cnt1 = 0;			// �^�C�}���Z�b�g
	enc1 = 0;			// ��ԋ������Z�b�g
	lcd_mode = lcd;		// LCD�\��OFF
	angle_mode = 0;	// �����g���[�X
	targetSpeed = speed_straight * SPEED_CURRENT; // �ڕW���x�ݒ�
}