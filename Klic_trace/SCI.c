//======================================//
// �C���N���[�h                         //
//======================================//
#include "SCI.h"
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
char revErr = 0;
// SCI1�֘A
char		txt_command[128];
char		txt_data[128];
char		*txt;
const char	*txt_stop = "stop";
const char	*txt_command1 = BUNDRATE;
char		*txt_rate;
char		cmmandMode = 0;
char		cmderr = 0;
char		stopWord = 0;
short 		cnt_byte = 0;
char		commandEnd;
char 		commandEnd = 0;
char 		commandDataSelect = 0;

// SCI12�֘A
char		SCI12_Req_mode = 0;	// 0:�X�^�[�g 1:�X�g�b�v
char		SCI12_SlaveAddr;	// ���M�f�[�^��
char		SCI12_NumData;		// �f�[�^��
char*		SCI12_DataArry;		// �f�[�^�z��

char 		ascii_num[] = {48,49,50,51,52,53,54,55,56,57,97,98,99,100,101,102};


#pragma interrupt Excep_SCI1_RXI1 (vect = VECT_SCI1_RXI1, enable )	// ��M���荞�݊֐���`

#pragma interrupt Excep_SCI12_TXI12 (vect = VECT_SCI12_TXI12, enable )	// TXI12���荞�݊֐���`
#pragma interrupt Excep_SCI12_TEI12 (vect = VECT_SCI12_TEI12, enable )	// STI12���荞�݊֐���`

#pragma interrupt chaek_SCI_Error (vect = VECT_ICU_GROUP12, enable )	// ��M�G���[���荞�݊֐���`
//////////////////////////////////////////////////////////////////////////
// ���W���[���� init_SCI1						//
// �����T�v     SCI1�̏�����						//
// ����         rate:�{�[���[�g��bps�œ���				//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void init_SCI1( char rate )
{
	unsigned char brr,abcs;
	
	// �{�[���[�g�I��
	if ( rate == RATE_9600 ) {
		abcs = 0;
		brr = 155;
	} else if ( rate == RATE_14400 ) {
		abcs = 0;
		brr = 103;
	} else if ( rate == RATE_19200 ) {
		abcs = 0;
		brr = 77;
	} else if ( rate == RATE_38400 ) {
		abcs = 0;
		brr = 38;
	} else if ( rate == RATE_57600 ) {
		abcs = 0;
		brr = 25;
	} else if ( rate == RATE_115200 ) {
		abcs = 0;
		brr = 12;
	} else if ( rate == RATE_230400 ) {
		abcs = 1;
		brr = 12;
	} else if ( rate == RATE_500000 ) {
		abcs = 0;
		brr = 2;
	} else if ( rate == RATE_750000 ) {
		abcs = 0;
		brr = 1;
	} else if ( rate == RATE_1000000 ) {
		abcs = 1;
		brr = 2;
	} else if ( rate == RATE_1500000 ) {
		abcs = 0;
		brr = 0;
	} else if ( rate == RATE_3000000 ) {
		abcs = 1;
		brr = 0;
	}
	
	// SCI1
	SYSTEM.PRCR.WORD = 0xA502;		// Release protect
	MSTP(SCI1) = 0;				// Wake up SCI1
	SYSTEM.PRCR.WORD = 0xA500;		// Protect
	
	SCI1.SCR.BYTE = 0;			// Set PFC of external pin used
	
	ICU.IER[IER_SCI1_RXI1].BIT.IEN_SCI1_RXI1 = 1;	// RXI���荞�݊J�n
	ICU.IPR[VECT_SCI1_RXI1].BIT.IPR = 14;		// RXI���荞�ݗD��x14
	ICU.GEN[GEN_SCI1_ERI1].BIT.EN_SCI1_ERI1 = 1;	// ERI���荞�݊J�n
	ICU.IPR[VECT_ICU_GROUP12].BIT.IPR = 15;		// ERI���荞�ݗD��x15
	
	// Set MPC
	PORT3.PMR.BIT.B0 = 1;			// Disable PB1: peripheral
	PORT2.PMR.BIT.B6 = 1;			// Disable PB0: peripheral
	MPC.PWPR.BIT.B0WI = 0;			// Release protect
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P30PFS.BIT.PSEL = 0x0A;		// Set PB1: TXD1
	MPC.P26PFS.BIT.PSEL = 0x0A;		// Set PB0: RXD1
	MPC.PWPR.BIT.PFSWE = 0;			// Protect
	MPC.PWPR.BIT.B0WI = 1;
	PORT3.PMR.BIT.B0 = 1;			// PB1: peripheral
	PORT2.PMR.BIT.B6 = 1;			// PB0: peripheral
	
	// SCI1 Settings
	SCI1.SCR.BIT.CKE = 0;			// Disable CKE
	SCI1.SMR.BIT.CKS = 0;			// PCLK
	SCI1.SMR.BIT.MP = 0;			// Disable multiple processor
	
	SCI1.SIMR1.BIT.IICM = 0;
	SCI1.SPMR.BIT.CKPH = 0;
	SCI1.SPMR.BIT.CKPOL = 0;
	SCI1.SCMR.BIT.SMIF = 0;
	SCI1.SEMR.BIT.ACS0 = 0;
	SCI1.SEMR.BIT.ABCS = abcs;		// 1�r�b�g�]�����Ԓ��̃N���b�N�T�C�N�����@0: 16 1: 8
	
	SCI1.SMR.BIT.STOP = 0;			// 1 stop bit
	SCI1.SMR.BIT.PM = 0;			// none parity
	SCI1.SMR.BIT.CHR = 0;			// 8bit data length
	SCI1.SMR.BIT.CM = 0;			// ����������
	SCI1.BRR = brr;				// 12: 115200bps 1:750000bps 0:1500000bps
	SCI1.SCR.BIT.RIE = 1;			// RXI���荞�ݗv��
	SCI1.SCR.BIT.TE = 1;			// Enable TX
	SCI1.SCR.BIT.RE = 1;			// Enable RX
	
	txt= txt_data;

}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� charput							//
// �����T�v     printf�̏o��(printf�Ŏg�p����)				//
// ����         data:�o�͂���ꕶ��					//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void charput( uint8_t data )
{
	while(SCI1.SSR.BIT.TEND == 0);
	SCI1.TDR = data;
	SCI1.SSR.BIT.TEND = 0;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� charget							//
// �����T�v     scanf�̓���(scanf�Ŏg�p����)				//
// ����         �Ȃ�							//
// �߂�l       data:���͂����ꕶ��					//
//////////////////////////////////////////////////////////////////////////
char charget(void)
{
	uint8_t data;
	return data;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI1_RXI1						//
// �����T�v     UART��M���Ɋ��荞�݂Ŏ��s�����			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void Excep_SCI1_RXI1(void)
{
	char c;
	
	__clrpsw_i();
	
	c = SCI1.RDR;
	if ( c == '$' ) {
		commandDataSelect = 1;	// �R�}���h���[�h�Ɉڍs
		txt = txt_command;
	}
	
	if ( commandDataSelect == 1 ) {	// �R�}���h���[�h�̂Ƃ�
		*txt++ = c;
		if ( c == ',' ) {	// �R�}���h�̎��
			if ( strcmp( txt_command, txt_command1) == 0 ) {	// �R�}���h����
				txt = txt_command;
				cmmandMode = 1;
			} else {		// �������R�}���h�ł͂Ȃ�
				cmderr = 1;
			}
		} else if ( c == '\r' ) {	// �R�}���h���͏I��
			commandDataSelect = 0;	// �f�[�^���[�h�Ɉڍs
			txt = txt_data;		// �A�h���X���Z�b�g
			commandEnd = 1;		//�R�}���h�̔�����J�n
		}
	} else {		// �f�[�^���[�h�̂Ƃ�
		*txt++ = c;
		cnt_byte++;
		if ( cnt_byte == 4 ) {	// 4�����ŏI��
			cnt_byte = 0;	// �f�[�^�����Z�b�g
			txt = txt_data;	// �A�h���X���Z�b�g
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� commandSCI1						//
// �����T�v     SCI1�Ŏ�M�����R�}���h����������			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void commandSCI1 (void)
{
	short s,i;
	
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
	// �ً}��~
	if ( strcmp( txt_data, txt_stop) == 0 ) {
		stopWord = 1;
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� init_SCI12						//
// �����T�v     SCI12�̏�����						//
// ����         rate:�{�[���[�g��kHz�œ���				//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void init_SCI12( void )
{
	
	// SCI12
	SYSTEM.PRCR.WORD = 0xA502;		// Release protect
	MSTP(SCI12) = 0;			// Wake up SCI12
	SYSTEM.PRCR.WORD = 0xA500;		// Protect
	
	SCI12.SCR.BYTE = 0;			// Set PFC of external pin used
	
	IEN( SCI12, TXI12 ) = 1;	// TXI���荞�݊J�n
	IPR( SCI12, TXI12 ) = 13;	// TXI���荞�ݗD��x13
	IEN( SCI12, TEI12 ) = 1;	// TEIE���荞�݊J�n
	IPR( SCI12, TEI12 ) = 12;	// TEIE���荞�ݗD��x12
	
	// Set MPC
	PORTE.PMR.BIT.B1 = 0;			// Disable PE1: peripheral
	PORTE.PMR.BIT.B2 = 0;			// Disable PE2: peripheral
	MPC.PWPR.BIT.B0WI = 0;			// Release protect
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PE1PFS.BIT.PSEL = 0x0C;		// Set PE1: SSDA12
	MPC.PE2PFS.BIT.PSEL = 0x0C;		// Set PE2: SSCL12
	MPC.PWPR.BIT.PFSWE = 0;			// Protect
	MPC.PWPR.BIT.B0WI = 1;
	PORTE.PMR.BIT.B1 = 1;			// PE1: peripheral
	PORTE.PMR.BIT.B2 = 1;			// PE2: peripheral
	
	PORTE.ODR0.BYTE = 0x1c;			// PE1,PE2 N�`�����l���I�[�v���h���C���o��
	
	// SCI12 Settings
	SCI12.SIMR3.BIT.IICSDAS = 3;		// SDA�[�q���n�C�C���s�[�_���X
	SCI12.SIMR3.BIT.IICSCLS = 3;		// SCL�[�q���n�C�C���s�[�_���X
	
	SCI12.SMR.BYTE = 0x00;			// PCLK�N���b�N
	
	SCI12.SCMR.BIT.SDIR = 1;		// MSB�t�@�[�X�g
	SCI12.SCMR.BIT.SINV = 0;		// ���M�A��M�f�[�^�����̂܂ܑ���M����
	SCI12.SCMR.BIT.SMIF = 0;		// �V���A���R�~���j�P�[�V�����C���^�[�t�F�C�X
	
	SCI12.BRR = 9;				// 375kHz
	
	SCI12.SPMR.BYTE = 0;
	SCI12.SEMR.BIT.NFEN = 0;		// �m�C�Y�����@�\����
	SCI12.SNFR.BIT.NFCS = 2;		// 2�����̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI12.SIMR1.BIT.IICM = 1;		// �Ȉ�IIC���[�h
	SCI12.SIMR1.BIT.IICDL = 0;		// �o�͒x���Ȃ�
	SCI12.SIMR2.BIT.IICINTM = 1;		// ��M���荞�݁A���M���荞�݉\
	SCI12.SIMR2.BIT.IICCSC = 1;		// �N���b�N�������s��
	SCI12.SIMR2.BIT.IICACKT = 1;		// NACK���M�܂���ACK/NACK��M
	
	SCI12.SPMR.BYTE = 0x00;
	
	SCI12.SCR.BYTE = 0x30;			// Enable TX RX
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� send_SCI12_I2c						//
// �����T�v     SCI12I2c�̑��M						//
// ����         �Ȃ�							//
// �߂�l       data:���͂����ꕶ��					//
//////////////////////////////////////////////////////////////////////////
void send_SCI12_I2c( char slaveAddr, char* data, char num )
{
	SCI12_SlaveAddr = slaveAddr;
	SCI12_NumData = num;
	SCI12_DataArry = data;
	
	SCI12_Req_mode = 0;
	
	SCI12.SCR.BIT.TEIE = 1;			// STI���荞�݋���
	SCI12.SCR.BIT.TIE = 1;			// TXI���荞�݋���
	
	SCI12.SIMR3.BIT.IICSTIF = 0;
	SCI12.SIMR3.BYTE = 0x51;		// �X�^�[�g�R���f�B�V�������s
	
	// �f�[�^�͊��荞�݂ő��M
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI12_TEI12					//
// �����T�v     �J�n/ �ĊJ�n/ ��~���������������Ɋ��荞�݂Ŏ��s�����	//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void Excep_SCI12_TEI12 ( void )
{
	if ( SCI12_Req_mode == 0 ) {
		led_out( 0x01 );
		SCI12.SIMR3.BYTE = 0x00;	// �f�[�^���M����
		SCI12.TDR = SCI12_SlaveAddr;	// �X���[�u�A�h���X��������
	} else {
		led_out( 0x02 );
		SCI12.SIMR3.BIT.IICSDAS = 3;	// SDA�[�q���n�C�C���s�[�_���X
		SCI12.SIMR3.BIT.IICSCLS = 3;	// SCL�[�q���n�C�C���s�[�_���X
		SCI12_Req_mode = 0;		// �X�^�[�g�R���f�B�V�����҂�
		
		SCI12.SCR.BIT.TEIE = 0;		// STI���荞�݋֎~
		SCI12.SCR.BIT.TIE = 0;		// TXI���荞�݋֎~
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI12_TXI12					//
// �����T�v     SCI12��M���Ɋ��荞�݂Ŏ��s�����			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void Excep_SCI12_TXI12( void )
{
	// �f�[�^���m�F
	if ( SCI12_NumData == 0 ) {
		led_out( 0x04 );
		SCI12.SIMR3.BIT.IICSTIF = 0;
		SCI12.SIMR3.BYTE = 0x54;	// �X�g�b�v�R���f�B�V�������s
	} else {
		if ( SCI12.SISR.BIT.IICACKR == 0 && SCI12_Req_mode == 0 ) {
			// ACK��M
			led_out( 0x10 );
			SCI12_Req_mode = 1;		// �X�g�b�v�R���f�B�V�����҂�
			SCI12.TDR = *SCI12_DataArry++;	// ���M�f�[�^��������
			SCI12_NumData--;
		} else if ( SCI12.SISR.BIT.IICACKR == 1 && SCI12_Req_mode == 0 ) {
			// NACK��M
			led_out( 0x08 );
			SCI12.SIMR3.BIT.IICSTIF = 0;
			SCI12_Req_mode = 1;		// �X�g�b�v�R���f�B�V�����҂�
			SCI12.SIMR3.BYTE = 0x54;	// �X�g�b�v�R���f�B�V�������s
		} else {
			SCI12.TDR = *SCI12_DataArry++;	// ���M�f�[�^��������
			SCI12_NumData--;
		}
	}
	
	
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� chaek_SCI_Error						//
// �����T�v     UART��M�G���[���Ɋ��荞�݂Ŏ��s�����			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void chaek_SCI_Error( void )
{
	/*
	if ( SCI1.SSR.BIT.ORER ) {		
		// �I�[�o�[�����G���[����
		*txt++ = SCI1.RDR;
	}
	
	if ( SCI1.SSR.BIT.FER ) {		
		
		if ( PORT3.PIDR.BIT.B0 ) {	// �u���[�N�M�����o
			SCI1.SCR.BIT.RE = 1;
		} else {
			// �t���[�~���O�G���[����
			if ( SCI1.SSR.BIT.PER == 1 ) {
				// �p���e�B�G���[����
			}
		}
		// �G���[�t���O�N���A
		SCI1.SSR.BIT.ORER = 0;
		SCI1.SSR.BIT.FER = 0;
		SCI1.SSR.BIT.PER = 0;
	}
	*/
	__clrpsw_i();
	if ( SCI1.SSR.BIT.ORER ) revErr = 1;
	if ( SCI1.SSR.BIT.FER ) revErr = 2;
	if ( SCI1.SSR.BIT.PER ) revErr = 3;
	
}