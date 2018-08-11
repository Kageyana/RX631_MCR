//======================================//
// �C���N���[�h                         //
//======================================//
#include "R_PG_RX631_mcr_ver3.0.h"
#include "PeripheralFunctions.h"
#include "LineChase.h"
#include "I2C_LCD.h"
#include "MicroSD.h"
#include "iodefine.h"
#include "SCI.h"
#include "I2C_MPU-9255.h"
#include <stdio.h>
#include <string.h>

//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// �^�C�}�֘A
// 100us�^�C�}
unsigned int		cnt0;		// �֐��p�^�C�}�J�E���g
unsigned int 		cnt1;		// ���s�p�^�C�}�J�E���g
unsigned short	 	cnt_out;	// �R�[�X�A�E�g����p�^�C�}�J�E���g
unsigned short	 	cnt_out2;	// �R�[�X�A�E�g����p�^�C�}�J�E���g2
unsigned short	 	cnt_out3;	// �R�[�X�A�E�g����p�^�C�}�J�E���g3
unsigned short	 	cnt_out4;	// �R�[�X�A�E�g����p�^�C�}�J�E���g4
unsigned short 		cnt_setup;	// �Z�b�g�A�b�v�Ŏg�p
unsigned short 		cnt_setup2;	// �Z�b�g�A�b�v�Ŏg�p
static char		ADTimer10;	// AD�ϊ��J�E���g�p
// 1ms�^�C�}
unsigned short		cnt_flash;	// �t���b�V���p�^�C�}�J�E���g
unsigned short 		cnt_gyro;	// �p�x�v�Z�p�^�C�}�J�E���g
static char		Timer10;	// 1ms�J�E���g�p
short			cnt_swR;	// �X�C�b�`����������p�E
short			cnt_swL;	// �X�C�b�`����������p��
// 2ms�^�C�}
unsigned int		cnt_log = 0;	// ���O�R��p�J�E���g

// �X�C�b�`�֘A
static unsigned char 	dpsw_d[4];	// �f�B�b�v�X�C�b�`�̊i�[��
static unsigned char	tasw_d[4];	// �X�C�b�`�l�̊i�[��

// �Z���T�֘A
static unsigned short 	result[21]; 	// 12bitA/D�ϊ����ʂ̊i�[��
static short		senR[10];	// �E�A�i���O�Z���TAD�l
static short		senL[10];	// ���A�i���O�Z���TAD�l
static short		senG[10];	// �Q�[�g�Z���TAD�l
static short		senC[10];	// ���S�A�i���O�Z���TAD�l
static short		senLL[10];	// �ō��[�A�i���O�Z���TAD�l
static short		senRR[10];	// �ŉE�[�A�i���O�Z���TAD�l
static short		gy[10];		// �W���C���Z���TAD�l
static short		pot[10];	// �|�e���V�������[�^�[AD�l
short			sensorR;	// �E�A�i���O�Z���TAD�l���ϒl
short			sensorL;	// ���A�i���O�Z���TAD�l���ϒl
short			sensorG;	// �Q�[�g�Z���TAD�l���ϒl
short			sensorC;	// ���S�A�i���O�Z���TAD�l���ϒl
short			sensorLL;	// �ō��[�A�i���O�Z���TAD�l���ϒl
short			sensorRR;	// �ŉE�[�A�i���O�Z���TAD�l���ϒl
short			Angle0;		// �T�[�{�Z���^�[�l

// �u�U�[�֘A
unsigned short 		BeepPattern;
unsigned short 		BeepTimer;
static char		BeepMode;

// �G���R�[�_�֘A
static unsigned short 	cnt_Encoder;	// �G���R�[�_�l�̊i�[��
static unsigned short	encbuff;	// �O��̃G���R�[�_�l
short			Encoder;	// 1ms���Ƃ̃p���X
unsigned int		EncoderTotal;	// �����s����
unsigned int		enc1;		// ���s�p�����J�E���g
unsigned int		enc_slope;	// ��㋗���J�E���g

// ���[�^�[�֘A
signed char		accele_fR;	// �E�O���[�^�[PWM�l
signed char		accele_fL;	// ���O���[�^�[PWM�l
signed char		accele_rR;	// �E�ヂ�[�^�[PWM�l
signed char		accele_rL;	// ���ヂ�[�^�[PWM�l
signed char		sPwm;		// �T�[�{���[�^�[PWM�l

//////////////////////////////////////////////////////////////////////////
// ���W���[���� Timer							//
// �����T�v     1ms���ƂɃ^�C�}���荞��					//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void Timer (void) {
	short s,i;
	
	__setpsw_i();
	//�@�^�C�}�J�E���g
	if ( pattern >= 11 && pattern <= 99 ) {	
		if ( pattern != 21 ) {				// �N���X���C���ʉߎ��͖���
			if ( sensor_inp() == 0x7 || sensor_inp() == 0x5 ) {	// �Z���T�S��
				cnt_out++;	
			} else {
				cnt_out = 0;
			}
		}
		if ( sensor_inp() == 0x0 && pattern != 53 && pattern != 63 ) cnt_out2++;	// �Z���T�S����
		else cnt_out2 = 0;
		if ( Encoder == 0 ) cnt_out3++;		// �G���R�[�_��~(�Ђ�����Ԃ����H)
		else cnt_out3 = 0;
		if ( slope_mode != 0 || slope_mode != 1 ) {
			if ( check_slope() == -1 ) cnt_out4++;
			else	cnt_out4 = 0;
		}
	} else if ( pattern < 11 ) {
		cnt0++;
		cnt_setup++;
		cnt_setup2++;
		cnt_flash++;
	}
	cnt1++;
	cnt_swR++;
	cnt_swL++;
			
	// LCD�\��
	if ( lcd_mode ) {
		lcdShowProcess();
	}

	// �G���R�[�_�J�E���g
	R_PG_Timer_GetCounterValue_MTU_U0_C1( &cnt_Encoder );
	Encoder = cnt_Encoder - encbuff;
	EncoderTotal += Encoder;
	enc1 += Encoder;
	enc_slope += Encoder;
	encbuff = cnt_Encoder;

	// PID����l�Z�o
	if ( angle_mode == 0 ) {
		servoControl();
	} else {
		servoControl2();
	}
	motorControl();
	
	// �p�x�v�Z
	get_degrees();
	get_TurningAngle();

	// MicroSD��������
	microSDProcess();
	if( msdFlag == 1 ) {
		msdTimer++;
		if( msdTimer == WRITINGTIME ) {
			msdTimer = 0;
			msdBuffPointa = msdBuff + msdBuffAddress;
			send_Char	(	pattern		);
			send_Char	(	motorPwm 	);
			send_Char	(	accele_fL 	);
			send_Char	(	accele_fR 	);
			send_Char	(	accele_rL 	);
			send_Char	(	accele_rR 	);
			send_Char	(	sPwm	 	);
			send_Char	(	ServoPwm 	);
			send_Char	(	ServoPwm2 	);
			send_Char	(	sensor_inp() 	);
			send_Char	( 	slope_mode	);
			send_Char	( 	msdlibError	);
			send_ShortToChar(	getServoAngle()	);
			send_ShortToChar(	SetAngle	);
			send_ShortToChar(	getAnalogSensor());
			send_ShortToChar(	sensorL		);
			send_ShortToChar(	sensorR		);
			send_ShortToChar(	getGyro()	);
			send_ShortToChar(	Degrees		);
			send_ShortToChar(	TurningAngle	);
			send_ShortToChar(	Encoder		);
			send_ShortToChar(	target_speed/10	);
			send_uIntToChar (	EncoderTotal	);
			send_uIntToChar (	enc1		);
			send_uIntToChar (	cnt_log		);
			cnt_log += WRITINGTIME;
			msdBuffAddress += DATA_BYTE;       // RAM�̋L�^�A�h���X������
			if( msdBuffAddress >= 512 ) {
				msdBuffAddress = 0;
				setMicroSDdata( msdBuff ); 
				msdWorkAddress += 512;
				if( msdWorkAddress >= msdEndAddress ) {
					msdFlag = 0;
				}
			}
		}
	}

	// UART��M
	if ( strcmp( txt_data, txt_stop) == 0 ) {
		stopWord = 1;
	}
	
	if ( IMUSet == 0 ) {
		if ( commandEnd == 1 ) {	// �R�}���h�I�����Ɏ��s
			if ( cmderr == 1 ) {
				printf("commandERROR\n");
				commandEnd = 0;
			} else {
				switch ( cmmandMode ) {
				case 1:
					// �{�[���[�g�ݒ�(br)
					for ( s = 0; s < 15; s++ ) {
						i = ascii_num[s];
						if ( txt_command[0] == i ) {
							printf("br=%d\n", s);
							init_SCI1( s );
							break;
						}
					}
					break;
					
				default:
					break;
				}
				commandEnd = 0;
			}
		}
	} else {
		// �����x�y�ъp���x���擾
		IMUProcess();
	}
	
	Timer10++;
	// 10�����ƂɎ��s
	switch ( Timer10 ) {	
	case 1:
		// �u�U�[
		beepProcessS();
		break;
	case 2:
		// �^�N�g�X�C�b�`�ǂݍ���
		R_PG_IO_PORT_Read_PC4(&tasw_d[1]);	// �^�N�g�X�C�b�`�E��
		R_PG_IO_PORT_Read_PC5(&tasw_d[0]);	// �^�N�g�X�C�b�`�E��
		R_PG_IO_PORT_Read_PC6(&tasw_d[2]);	// �^�N�g�X�C�b�`����
		R_PG_IO_PORT_Read_P50(&tasw_d[3]);	// �^�N�g�X�C�b�`����
		break;
	case 3:
		// �f�B�b�v�X�C�b�`�ǂݍ���
		R_PG_IO_PORT_Read_PC3(&dpsw_d[0]);
		R_PG_IO_PORT_Read_PC2(&dpsw_d[1]);
		R_PG_IO_PORT_Read_PC1(&dpsw_d[2]);
		R_PG_IO_PORT_Read_PC0(&dpsw_d[3]);
		break;
	case 5:
		if (SCI1.SSR.BIT.ORER) {
			reverr = 1;
		} else if (SCI1.SSR.BIT.FER) {
			reverr = 2;
		} else if (SCI1.SSR.BIT.PER) {
			reverr = 3;
		} else {
			//reverr = 0;
		}
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
//////////////////////////////////////////////////////////////////////////
// ���W���[���� ADconverter						//
// �����T�v     AD�ϊ����荞��						//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void ADconverter ( void )
{
	__setpsw_i();
	R_PG_ADC_12_GetResult_S12AD0( result );
	
	ADTimer10++;
	if ( ADTimer10 == 10 ) {
		ADTimer10 = 0;
	}
	
	// AD�ϊ��l���o�b�t�@�Ɋi�[
	senLL[ADTimer10] = result[3];
	senL[ADTimer10] = result[4];
	senG[ADTimer10] = result[5];
	senC[ADTimer10] = result[6];
	senR[ADTimer10] = result[7];
	senRR[ADTimer10] = result[8];
	gy[ADTimer10] = result[12];
	pot[ADTimer10] = result[13];
	
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� delay	                                                //
// �����T�v     �x������ 1 = 1ms					//
// ����         delaytime						//
// �߂�l       �Ȃ�                                                    //
//////////////////////////////////////////////////////////////////////////
void delay(unsigned short delaytime)
{
	cnt0 = 0;
	while( cnt0 <= delaytime ) {
		R_PG_IO_PORT_Write_P27(0);
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� dipsw_get						//
// �����T�v     �f�B�b�v�X�C�b�`�l��16�i���Ŏ擾			//
// ����         �Ȃ�							//
// �߂�l       �X�C�b�`�l 0�`15					//
//////////////////////////////////////////////////////////////////////////
unsigned char dipsw_get( void ) 
{
	char	dpsw[4];
	
	if ( dpsw_d[0] == 0 )	dpsw[0] = 0x1;
	else			dpsw[0] = 0x0;
	
	if ( dpsw_d[1] == 0 )	dpsw[1] = 0x2;
	else			dpsw[1] = 0x0;
	
	if ( dpsw_d[2] == 0 )	dpsw[2] = 0x4;
	else			dpsw[2] = 0x0;
	
	if ( dpsw_d[3] == 0 )	dpsw[3] = 0x8;
	else			dpsw[3] = 0x0;

	return ( dpsw[0] + dpsw[1] + dpsw[2] + dpsw[3] );
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� led_out							//
// �����T�v     LED�̓_��						//
// ����         led(�Ή�����LED�ԍ�)					//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void led_out ( unsigned char led )
{
	unsigned char led2;

	led2 = led << 1;
	R_PG_IO_PORT_Write_P5( led2 );
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� tasw_get						//
// �����T�v     �^�N�g�X�C�b�`�l��16�i���Ŏ擾				//
// ����         �Ȃ�							//
// �߂�l       �X�C�b�`�l 0�`7						//
//////////////////////////////////////////////////////////////////////////
unsigned char tasw_get(void) 
{
	char	tasw[4];
	
	if ( tasw_d[0] == 0 )	tasw[0] = 0x1;
	else			tasw[0] = 0x0;
	
	if ( tasw_d[1] == 0 )	tasw[1] = 0x2;	
	else			tasw[1] = 0x0;
	
	if ( tasw_d[2] == 0 )	tasw[2] = 0x4;	
	else			tasw[2] = 0x0;
	
	if ( tasw_d[3] == 0 )	tasw[3] = 0x8;	
	else			tasw[3] = 0x0;

	return ( tasw[0] + tasw[1] + tasw[2] + tasw[3] );
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� getGyro						//
// �����T�v     �W���C���Z���T�̃A�i���O�l�Ŏ擾			//
// ����         �Ȃ�							//
// �߂�l       �Z���T�l						//
//////////////////////////////////////////////////////////////////////////
short getGyro(void) 
{
	short gyro;
	// ���ϒl�̌v�Z
	gyro =  ( gy[0] + gy[1] + gy[2] + gy[3] + gy[4] + gy[5] + gy[6] + gy[7] + gy[8] + gy[9] ) / 10;
	
	return ( gyro - 1796 );
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� getServoAngle						//
// �����T�v     �|�e���V�������[�^�[�̃A�i���O�l�Ŏ擾			//
// ����         �Ȃ�							//
// �߂�l       �Z���T�l						//
//////////////////////////////////////////////////////////////////////////
short getServoAngle(void) 
{	
	short Angle;
	
	// ���ϒl�̌v�Z
	Angle =  ( pot[0] + pot[1] + pot[2] + pot[3] + pot[4] + pot[5] + pot[6] + pot[7] + pot[8] + pot[9] ) / 10;
	
	return  ( Angle - Angle0 );
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� getAnalogSensor						//
// �����T�v     �A�i���O�Z���T�̃A�i���O�l�Ŏ擾			//
// ����         �Ȃ�							//
// �߂�l       �Z���T�l						//
//////////////////////////////////////////////////////////////////////////
short getAnalogSensor(void) 
{
	// ���ϒl�̌v�Z
	sensorR = ( senR[0] + senR[1] + senR[2] + senR[3] + senR[4] + senR[5] + senR[6] + senR[7] + senR[8] + senR[9] ) / 10;
	//sensorR >>= 1;
	sensorL = ( senL[0] + senL[1] + senL[2] + senL[3] + senL[4] + senL[5] + senL[6] + senL[7] + senL[8] + senL[9] ) / 10;
	//sensorL += 100;

	return sensorR - sensorL;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� sensor_inp						//
// �����T�v     �f�W�^���Z���T�̒l��16�i���Ŏ擾			//
// ����         �Ȃ�							//
// �߂�l       �Z���T�l0�`7						//
//////////////////////////////////////////////////////////////////////////
unsigned char sensor_inp(void) 
{
	char l, c, r;
	
	sensorC = ( senC[0] + senC[1] + senC[2] + senC[3] + senC[4] + senC[5] + senC[6] + senC[7] + senC[8] + senC[9] ) / 10;
	sensorLL = ( senLL[0] + senLL[1] + senLL[2] + senLL[3] + senLL[4] + senLL[5] + senLL[6] + senLL[7] + senLL[8] + senLL[9] ) / 10;
	sensorRR = ( senRR[0] + senRR[1] + senRR[2] + senRR[3] + senRR[4] + senRR[5] + senRR[6] + senRR[7] + senRR[8] + senRR[9] ) / 10;
	
	if (sensorRR < 3000 ) r = 0x1;
	else r = 0;
	if (sensorC < 2000 ) c = 0x2;
	else c = 0;
	if (sensorLL < 3000 ) l = 0x4;
	else l = 0;
	
	return l+c+r;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� startbar_get						//
// �����T�v     �X�^�[�g�Q�[�g�̊J�̊m�F				//
// ����         �Ȃ�							//
// �߂�l       0; ���Ă��� 1; �J���Ă���				//
//////////////////////////////////////////////////////////////////////////
unsigned char startbar_get(void) 
{
	char ret;
		
	sensorG = ( senG[0] + senG[1] + senG[2] + senG[3] + senG[4] + senG[5] + senG[6] + senG[7] + senG[8] + senG[9] ) / 10;
	if ( sensorG <= 2000 )	ret = 1;
	else			ret = 0;
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� init_BeepS						//
// �����T�v     �u�U�[�֘A������					//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void init_BeepS( void )
{
    BeepPattern = 0x0000;
    BeepTimer   = 0;
    BeepMode    = 0;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� setBeepPatternS						//
// �����T�v     �u�U�[�o�̓p�^�[���Z�b�g				//
// ����         Beep:���p�^�[��					//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void setBeepPatternS( unsigned short Beep )
{
    BeepPattern = Beep;
    BeepTimer   = 0;
    BeepMode    = 1;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� beepProcessS						//
// �����T�v     �u�U�[����						//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
// ����		���̊֐���1ms���Ɏ��s���Ă�������			//
//////////////////////////////////////////////////////////////////////////
void beepProcessS( void )
{
	if ( BeepMode ) {
		if ( BeepTimer % 5 == 0 ) {
			if ( BeepPattern & 0x8000 ) {
				R_PG_IO_PORT_Write_P23(1);
			} else {
				R_PG_IO_PORT_Write_P23(0);
			}
			BeepPattern <<= 1;
		}
		BeepTimer++;
		if ( BeepTimer == 17 * 5 ) {
			BeepMode = 0;
			R_PG_IO_PORT_Write_P23(0);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� motor_f							//
// �����T�v     ���[�^�[��PWM�̕ύX					//
// ����         accelefL, accelefR(PWM��1�`100%�Ŏw��)			//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void motor_f( signed char accelefL, signed char accelefR ){
	uint16_t pwmfl, pwmfr;
	
	if ( pushcart_mode ) {
		accelefR = 0;
		accelefL = 0;
	}
	
	accele_fR = accelefR;
	accele_fL = accelefL;
	
	pwmfl = TGR_MOTOR * accelefL / 100;
	pwmfr = TGR_MOTOR * accelefR / 100;
	
	if( accelefL >= 0) {					// ���]
		R_PG_IO_PORT_Write_PB2( 0 );
		R_PG_Timer_SetTGR_D_MTU_U0_C0( pwmfl );
	} else if ( accelefL == 100 || accelefL == -100 ) {	// 100%
		if ( accelefL > 0 ) {
			R_PG_IO_PORT_Write_PB2( 0 );
		} else {
			R_PG_IO_PORT_Write_PB2( 1 );
		}
		R_PG_Timer_SetTGR_D_MTU_U0_C0( TGR_MOTOR + 2 );
	} else {						// �t�]
		R_PG_IO_PORT_Write_PB2( 1 );
		R_PG_Timer_SetTGR_D_MTU_U0_C0( -pwmfl );
	}
	
	if( accelefR >= 0) {					// ���]
		R_PG_IO_PORT_Write_PB4( 0 );
		R_PG_Timer_SetTGR_B_MTU_U0_C0( pwmfr );
	} else if ( accelefR == 100 || accelefR == -100 ) {	// 100%
		if ( accelefR > 0 ) {
			R_PG_IO_PORT_Write_PB4( 0 );
		} else {
			R_PG_IO_PORT_Write_PB4( 1 );
		}
		R_PG_Timer_SetTGR_B_MTU_U0_C0( TGR_MOTOR + 2 );
	} else {						// �t�]
		R_PG_IO_PORT_Write_PB4( 1 );
		R_PG_Timer_SetTGR_B_MTU_U0_C0( -pwmfr );
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� motor_r							//
// �����T�v     ���[�^�[��PWM�̕ύX					//
// ����         acelerL, accelerR(PWM��1�`100%�Ŏw��)			//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void motor_r( signed char accelerL, signed char accelerR ){
	uint16_t pwmrl, pwmrr;
	
	if ( pushcart_mode ) {
		accelerL = 0;
		accelerR = 0;
	}
	
	accele_rR = accelerR;
	accele_rL = accelerL;
	
	pwmrl = TGR_MOTOR * accelerL / 100;
	pwmrr = TGR_MOTOR * accelerR / 100;
	
	if( accelerL >= 0 ) {					// ���]
		R_PG_IO_PORT_Write_P20( 0 );
		R_PG_Timer_SetTGR_B_MTU_U0_C3( pwmrl );
	} else if ( accelerL == 100 || accelerL == -100 ) {	// 100%
		if (accelerL > 0) {
			R_PG_IO_PORT_Write_P20( 0 );
		} else {
			R_PG_IO_PORT_Write_P20( 1 );
		}
		R_PG_Timer_SetTGR_B_MTU_U0_C3( TGR_MOTOR + 2 );
	} else {						// �t�]
		R_PG_IO_PORT_Write_P20( 1 );
		R_PG_Timer_SetTGR_B_MTU_U0_C3( -pwmrl );
	}
	
	if( accelerR >= 0 ) {					// ���]
		R_PG_IO_PORT_Write_P15( 0 );
		R_PG_Timer_SetTGR_D_MTU_U0_C3( pwmrr );
	} else if ( accelerR == 100 || accelerR == -100 ) {	// 100%
		if ( accelerR > 0 ) {
			R_PG_IO_PORT_Write_P15( 0 );
		} else {
			R_PG_IO_PORT_Write_P15( 1 );
		}
		R_PG_Timer_SetTGR_D_MTU_U0_C3( TGR_MOTOR + 2 );
	} else {						// �t�]
		R_PG_IO_PORT_Write_P15( 1 );
		R_PG_Timer_SetTGR_D_MTU_U0_C3( -pwmrr );
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� servoPwmOut						//
// �����T�v     �����g���[�X���T�[�{��PWM�̕ύX				//
// ����         spwm							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void servoPwmOut( signed char servopwm )
{
	uint16_t pwm;
	short angle;
	
	sPwm = servopwm;
	
	// �T�[�{���~�b�g����
	angle = getServoAngle();
	
	// �p�x�ɂ�郊�~�b�g����
	if ( angle >= SERVO_LIMIT ) servopwm = -15;
	if ( angle <= -SERVO_LIMIT ) servopwm = 15;
	
	// �|�e���V�������[�^�[���O��Ă����琧�䂵�Ȃ�
	if ( angle > SERVO_LIMIT + 100 ) servopwm = 0;
	if ( angle < -SERVO_LIMIT - 100 ) servopwm = 0;

	pwm = (uint16_t)TGR_SERVO * servopwm / 100;
	// �T�[�{���[�^����
	if( servopwm > 0) {				// ���]
		R_PG_IO_PORT_Write_PB6( 0 );
		R_PG_Timer_SetTGR_B_MTU_U0_C2( pwm );
	} else {				// �t�]
		R_PG_IO_PORT_Write_PB6( 1 );
		R_PG_Timer_SetTGR_B_MTU_U0_C2( -pwm );
	}
}