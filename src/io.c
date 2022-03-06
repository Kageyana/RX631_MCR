//====================================//
// �C���N���[�h
//====================================//
#include "io.h"
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
// �X�C�b�`�֘A
static unsigned char 	dpsw_d[4];	// �f�B�b�v�X�C�b�`�̊i�[��
static unsigned char	tasw_d[5];	// �X�C�b�`�l�̊i�[��
/////////////////////////////////////////////////////////////////////
// ���W���[���� initIO
// �����T�v     IO�|�[�g�̏�����
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void initIO(void)
{
	// I/O�|�[�g��ݒ�
	R_PG_IO_PORT_Set_P1();
	R_PG_IO_PORT_Set_P2();
	R_PG_IO_PORT_Set_P3();
	R_PG_IO_PORT_Set_P5();
	R_PG_IO_PORT_Set_PA();
	R_PG_IO_PORT_Set_PB();
	R_PG_IO_PORT_Set_PC();
	R_PG_IO_PORT_Set_PE();
	
	// ���ׂĂ�IO�|�[�g��LOW�ɂ���
	R_PG_IO_PORT_Write_P1(0);
	R_PG_IO_PORT_Write_P2(0);
	R_PG_IO_PORT_Write_P3(0);
	R_PG_IO_PORT_Write_P5(0);
	R_PG_IO_PORT_Write_PA(0);
	R_PG_IO_PORT_Write_PB(0);
	R_PG_IO_PORT_Write_PC(0);
	R_PG_IO_PORT_Write_PE(0);
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� ledOut
// �����T�v     LED�̓_��
// ����         rgb 	0x1:�� 0x2�� 0x4
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void ledOut ( char rgb )
{
	if ( (rgb & 0x4) == 0x4 ) LEDR_ON
	else LEDR_OFF
	
	if ( (rgb & 0x2) == 0x2 ) LEDG_ON
	else LEDG_OFF
	
	if ( (rgb & 0x1) == 0x1 ) LEDB_ON
	else LEDB_OFF
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� getSwitch
// �����T�v     �X�C�b�`�̓ǂݍ���(10ms���ƂɎ��s)
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void getSwitch(void)
{
	// �^�N�g�X�C�b�`�ǂݍ���
	TACTSWITCH1	// �^�N�g�X�C�b�`��
	TACTSWITCH2	// �^�N�g�X�C�b�`��
	TACTSWITCH3	// �^�N�g�X�C�b�`�E
	TACTSWITCH4	// �^�N�g�X�C�b�`��
	TACTSWITCH5	// �^�N�g�X�C�b�`��������
	
	// �f�B�b�v�X�C�b�`�ǂݍ���
	DIPSWITCH1
	DIPSWITCH2
	DIPSWITCH3
	DIPSWITCH4
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� dipswGet
// �����T�v     �f�B�b�v�X�C�b�`�l��16�i���Ŏ擾
// ����         �Ȃ�
// �߂�l       �X�C�b�`�l 0�`15
/////////////////////////////////////////////////////////////////////
unsigned char dipswGet(void) 
{
	char	dpsw[4];
	
	if ( dpsw_d[0] == 0 )	dpsw[0] = 0x0;
	else			dpsw[0] = 0x1;
	
	if ( dpsw_d[1] == 0 )	dpsw[1] = 0x0;
	else			dpsw[1] = 0x2;
	
	if ( dpsw_d[2] == 0 )	dpsw[2] = 0x0;
	else			dpsw[2] = 0x4;

	if ( dpsw_d[3] == 0 )	dpsw[3] = 0x0;
	else			dpsw[3] = 0x8;

	return ( dpsw[0] + dpsw[1] + dpsw[2] + dpsw[3] );
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� taswGet
// �����T�v     �^�N�g�X�C�b�`�l��16�i���Ŏ擾
// ����         �Ȃ�
// �߂�l       �X�C�b�`�l 0�`7
///////////////////////////////////////////////////////////////////////////
unsigned char taswGet(void) 
{
	char	tasw[5];
	
	if ( tasw_d[0] == 0 )	tasw[0] = 0x1;
	else			tasw[0] = 0x0;
	
	if ( tasw_d[1] == 0 )	tasw[1] = 0x2;	
	else			tasw[1] = 0x0;
	
	if ( tasw_d[2] == 0 )	tasw[2] = 0x4;	
	else			tasw[2] = 0x0;
	
	if ( tasw_d[3] == 0 )	tasw[3] = 0x8;	
	else			tasw[3] = 0x0;
	
	if ( tasw_d[4] == 0 )	tasw[4] = 0xf;
	else			tasw[4] = 0x0;

	return ( tasw[0] + tasw[1] + tasw[2] + tasw[3] + tasw[4] );
}