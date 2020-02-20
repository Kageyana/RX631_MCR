#ifndef ADC_H_
#define ADC_H_
//====================================//
// �C���N���[�h									//
//====================================//
#include "R_PG_IGC-P8080_v1.h"
//====================================//
// �V���{����`									//
//====================================//
#define SERVO_CENTER		2077		// �T�[�{�Z���^�[��AD�l
#define GATE_VAL			190		// �Q�[�g�Z���T�������l
/*************************************** ���������֐� *************************/
// AD�R���o�[�^
#define SET_ADC		R_PG_ADC_12_Set_S12AD0(); 				// 12�r�b�gA/D�R���o�[�^(S12AD0)��ݒ�
#define START_ADC		R_PG_ADC_12_StartConversionSW_S12AD0();	// A/D�ϊ��J�n
#define GET_ADC		R_PG_ADC_12_GetResult_S12AD0( result );	// AD�l���擾
/**************************************************************************/
//====================================//
// �O���[�o���ϐ��̐錾								//
//====================================//
// �Z���T�֘A
extern short		sensorR;		// �E�A�i���O�Z���T
extern short		sensorL;		// ���A�i���O�Z���T
extern short		sensorG;		// �Q�[�g�Z���T
extern short		sensorG_th;	// �Q�[�g�Z���T
extern short		sensorC;		// ���S�A�i���O�Z���T
extern short		sensorLL;		// �ō��[�A�i���O�Z���T
extern short		sensorRR;		// �ŉE�[�A�i���O�Z���T

extern short		VoltageC;	//�d���`�F�b�J�[
extern double		Voltage;

extern short		Angle0;		// �T�[�{�Z���^�[�l
//====================================//
// �v���g�^�C�v�錾									//
//====================================//
// �Z���T�֘A
short getServoAngle(void);
short getAnalogSensor( void );
unsigned char sensor_inp( void );
unsigned char startbar_get( void );

// �d���l�֘A
void get_volatage( void );
#endif // ADC_H_