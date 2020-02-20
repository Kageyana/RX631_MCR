//====================================//
// �C���N���[�h									//
//====================================//
#include "ADC.h"
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// �^�C�}�֘A
static char			ADTimer10;	// AD�ϊ��J�E���g�p
// �Z���T�֘A
static unsigned short 	result[14]; 	// 12bitA/D�ϊ����ʂ̊i�[��
static int			senR;	// �E�A�i���O�Z���T�ώZAD�l
static int			senL;		// ���A�i���O�Z���T�ώZAD�l
static int			senG;	// �Q�[�g�Z���T�ώZAD�l
static int			senC;	// ���S�A�i���O�Z���T�ώZAD�l
static int			senLL;	// �ō��[�A�i���O�Z���T�ώZAD�l
static int			senRR;	// �ŉE�[�A�i���O�Z���T�ώZAD�l
static int			VolC;		// �d���`�F�b�J�[AD�l
static int			pot;		// �|�e���V�������[�^�[�ώZAD�l
short 			Angle;	// �|�e���V�������[�^�[����AD�l
short				sensorR;	// �E�A�i���O�Z���T����AD�l
short				sensorL;	// ���A�i���O�Z���T����AD�l
short				sensorG;	// �Q�[�g�Z���T����AD�l
short				sensorG_th = GATE_VAL;	// �Q�[�g�J���������l
short				sensorC;	// ���S�A�i���O�Z���T����AD�l
short				sensorLL;	// �ō��[�A�i���O�Z���T����AD�l
short				sensorRR;	// �ŉE�[�A�i���O�Z���T����AD�l
short				VoltageC;	// �d���`�F�b�J�[AD�l���ϒl
short				Angle0;	// �T�[�{�Z���^�[�l

double		Voltage;
/////////////////////////////////////////////////////////////////////
// ���W���[���� ADconverter							//
// �����T�v     AD�ϊ����荞��						//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
/////////////////////////////////////////////////////////////////////
void ADconverter ( void )
{
	__setpsw_i();
	GET_ADC
	
	ADTimer10++;
	if ( ADTimer10 == 10 ) {
		ADTimer10 = 0;
		
		Angle = pot / 10;
		sensorR = senR / 10;
		sensorL = senL / 10;
		sensorG = senG / 10;
		sensorC = senC / 10;
		sensorLL = senLL / 10;
		sensorRR = senRR / 10;
		VoltageC = VolC /10;
		
		senR = 0;
		senL = 0;
		senG = 0;
		senC = 0;
		senLL = 0;
		senRR = 0;
		
		VolC = 0;
		pot = 0;
	}
	
	// AD�ϊ��l���o�b�t�@�Ɋi�[
	pot += result[3];
	senG += result[4];
	senLL += result[5];
	senL += result[6];
	senC += result[7];
	senRR += result[8];
	senR += result[9];
	VolC += result[11];
	
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� get_voltage							//
// �����T�v     �d���̎擾							//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
/////////////////////////////////////////////////////////////////////
void get_voltage ( void )
{
	Voltage = VoltageC * 5.05 * 3.94 / 4096;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getServoAngle							//
// �����T�v     �|�e���V�������[�^�[�̃A�i���O�l�Ŏ擾			//
// ����         �Ȃ�									//
// �߂�l       �Z���T�l								//
/////////////////////////////////////////////////////////////////////
short getServoAngle(void) 
{	
	return  ( Angle0 - Angle );
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getAnalogSensor						//
// �����T�v     �A�i���O�Z���T�̃A�i���O�l�Ŏ擾				//
// ����         �Ȃ�									//
// �߂�l       �Z���T�l								//
/////////////////////////////////////////////////////////////////////
short getAnalogSensor(void) 
{
	return sensorR - sensorL;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� sensor_inp							//
// �����T�v     �f�W�^���Z���T�̒l��16�i���Ŏ擾				//
// ����         �Ȃ�									//
// �߂�l       �Z���T�l0�`7							//
/////////////////////////////////////////////////////////////////////
unsigned char sensor_inp(void) 
{
	char l, c, r;
	
	if (sensorRR < 500 ) r = 0x1;
	else r = 0;
	if (sensorC < 500 ) c = 0x2;
	else c = 0;
	if (sensorLL < 500 ) l = 0x4;
	else l = 0;
	
	return l+c+r;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� startbar_get							//
// �����T�v     �X�^�[�g�Q�[�g�̊J�̊m�F					//
// ����         �Ȃ�									//
// �߂�l       0; ���Ă��� 1; �J���Ă���					//
/////////////////////////////////////////////////////////////////////
unsigned char startbar_get(void) 
{
	char ret;
	
	if ( sensorG <= sensorG_th )	ret = 1;
	else			ret = 0;
	
	return ret;
}