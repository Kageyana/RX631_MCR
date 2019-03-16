//====================================//
// �C���N���[�h									//
//====================================//
#include "SCI.h"

//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//
char		revErr = 0;		// �ʐM�G���[�ԍ�
// SCI1�֘A
char		SCI1_mode;		// �ʐM����
char		txt_command[128];	// �R�}���h�i�[
char		txt_data[128];		// �f�[�^�i�[
char*	txt;				// ��M�f�[�^�i�[
char		cmmandMode = 0;	// �R�}���h�I��
char		stopWord = 0;		// 0: ��~���[�h����M 1:��~���[�h��M
short 	cnt_byte = 0;		// ��M�����o�C�g��
char 		command = 0;		// 0:�R�}���h��M�҂� 1:�R�}���h���͒� 2:�R�}���h���蒆

char		SCI1_Req_mode;	// 0:�X�^�[�g 1:�X�g�b�v 2:�f�[�^����M��
char		SCI1_RW_mode;	// 0:���M 1:��M
char		SCI1_Slaveaddr;	// �X���[�u�A�h���X
char		SCI1_NumData;		// ���M�f�[�^��
char*	SCI1_DataArry;		// ���M�f�[�^�z��
char		SCI1_DataBuff[255];	// ���M�f�[�^�o�b�t�@

// SCI12�֘A
char		SCI12_Req_mode = 0;	// 0:�X�^�[�g 1:�X�g�b�v
char		SCI12_Slaveaddr;		// ���M�f�[�^��
char		SCI12_NumData;		// �f�[�^��
char		SCI1_NumData2;		// ���M�f�[�^��2
char*	SCI12_DataArry;		// �f�[�^�z��
char*	SCI1_DataArry2;		// ���M�f�[�^�z��2
char		SCI12_DataBuff[255];	// ���M�f�[�^�o�b�t�@

char 		ascii_num[] = {48,49,50,51,52,53,54,55,56,57,97,98,99,100,101,102};


#pragma interrupt Excep_SCI1_RXI1 (vect = VECT_SCI1_RXI1, enable)	// RXI1���荞�݊֐���`
#pragma interrupt Excep_SCI1_TXI1 (vect = VECT_SCI1_TXI1, enable)	// TXI1���荞�݊֐���`
#pragma interrupt Excep_SCI1_TEI1 (vect = VECT_SCI1_TEI1, enable)		// TEI1���荞�݊֐���`

#pragma interrupt Excep_SCI5_RXI5 (vect = VECT_SCI5_RXI5, enable)	// RXI1���荞�݊֐���`
#pragma interrupt Excep_SCI5_TXI5 (vect = VECT_SCI5_TXI5, enable)	// TXI12���荞�݊֐���`
#pragma interrupt Excep_SCI5_TEI5 (vect = VECT_SCI5_TEI5, enable)		// STI12���荞�݊֐���`

#pragma interrupt Excep_SCI12_RXI12 (vect = VECT_SCI12_RXI12, enable)	// RXI1���荞�݊֐���`
#pragma interrupt Excep_SCI12_TXI12 (vect = VECT_SCI12_TXI12, enable)	// TXI12���荞�݊֐���`
#pragma interrupt Excep_SCI12_TEI12 (vect = VECT_SCI12_TEI12, enable)	// STI12���荞�݊֐���`

//#pragma interrupt chaek_SCI_Error (vect = VECT_ICU_GROUP12 )	// ��M�G���[���荞�݊֐���`
///////////////////////////////////////////////////////////////////////////
// ���W���[���� init_SCI1								//
// �����T�v     SCI1�̏�����								//
// ����         mode: �ʐM���� rate:�{�[���[�g��bps�œ���		//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void init_SCI1( char mode, char rate )
{
	unsigned char brr,abcs;
	
	SCI1_mode = mode;
	
	ICU.GEN[GEN_SCI1_ERI1].BIT.EN_SCI1_ERI1 = 1;	// ERI���荞�݊J�n
	ICU.IPR[VECT_ICU_GROUP12].BIT.IPR = 15;		// ERI���荞�ݗD��x15
	
	IPR( SCI1, TXI1 ) = IPR_RXI1;	// RXI���荞�ݗD��x14
	IPR( SCI1, TXI1 ) = IPR_TXI1;	// TXI���荞�ݗD��x13
	IPR( SCI1, TEI1 ) = IPR_TEI1;	// TEIE���荞�ݗD��x12
	
	if ( mode == UART ) {
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
		
		PORT2.ODR1.BIT.B4 = 0;		// P26 CMOS�o��
		PORT3.ODR0.BIT.B0 = 0;		// P30 CMOS�o��
		
		IEN( SCI1, RXI1 ) = 1;	// RXI���荞�݊J�n
		IEN( SCI1, TXI1 ) = 0;	// TXI���荞�݊J�n
		IEN( SCI1, TEI1 ) = 0;	// TEIE���荞�݊J�n
		
		// Set MPC
		PORT3.PMR.BIT.B0 = 0;		// Disable PB1: peripheral
		PORT2.PMR.BIT.B6 = 0;		// Disable PB0: peripheral
		MPC.PWPR.BIT.B0WI = 0;		// Release protect
		MPC.PWPR.BIT.PFSWE = 1;
		MPC.P30PFS.BIT.PSEL = 0x0A;	// Set PB1: TXD1
		MPC.P26PFS.BIT.PSEL = 0x0A;	// Set PB0: RXD1
		MPC.PWPR.BIT.PFSWE = 0;		// Protect
		MPC.PWPR.BIT.B0WI = 1;
		PORT3.PMR.BIT.B0 = 1;		// PB1: peripheral
		PORT2.PMR.BIT.B6 = 1;		// PB0: peripheral
		
		// SCI1 Settings
		SCI1.SCR.BIT.CKE = 0;		// Disable CKE
		SCI1.SMR.BIT.CKS = 0;		// PCLK
		SCI1.SMR.BIT.MP = 0;		// �}���`�v���Z�b�T�ʐM�@�\���֎~
		
		SCI1.SIMR1.BIT.IICM = 0;		// �V���A���C���^�t�F�[�X���[�h
		SCI1.SPMR.BYTE = 0;			// �N���b�N�x��A���]�Ȃ�
		SCI1.SCMR.BIT.SMIF = 0;		// �V���A���R�~���j�P�[�V�����C���^�[�t�F�C�X���[�h
		SCI1.SCMR.BIT.SDIR = 0;		// LSB�t�@�[�X�g
		SCI1.SEMR.BIT.ACS0 = 0;		// �O���N���b�N����
		SCI1.SEMR.BIT.ABCS = abcs;	// 1�r�b�g�]�����Ԓ��̃N���b�N�T�C�N�����@0: 16 1: 8
		
		SCI1.SMR.BIT.STOP = 0;		// 1 stop bit
		SCI1.SMR.BIT.PM = 0;		// none parity
		SCI1.SMR.BIT.CHR = 0;		// 8bit data length
		SCI1.SMR.BIT.CM = 0;		// ����������
		SCI1.BRR = brr;				// 12: 115200bps 1:750000bps 0:1500000bps
		SCI1.SCR.BIT.RIE = 1;		// RXI���荞�ݗv��
		SCI1.SCR.BIT.TE = 1;			// Enable TX
		SCI1.SCR.BIT.RE = 1;			// Enable RX
		
		txt= txt_data;
	} else if ( mode == I2C ) {
		IEN( SCI1, RXI1 ) = 1;	// RXI���荞�݊J�n
		IEN( SCI1, TXI1 ) = 1;	// TXI���荞�݊J�n
		IEN( SCI1, TEI1 ) = 1;	// TEIE���荞�݊J�n
		
		// SCI12
		SYSTEM.PRCR.WORD = 0xA502;	// Release protect
		MSTP(SCI1) = 0;			// Wake up SCI1
		SYSTEM.PRCR.WORD = 0xA500;	// Protect
		
		SCI1.SCR.BYTE = 0;		// Set PFC of external pin used
		
		PORT2.ODR1.BIT.B4 = 1;		// P26 N�`�����l���I�[�v���h���C���o��
		PORT3.ODR0.BIT.B0 = 1;		// P30 N�`�����l���I�[�v���h���C���o��
		
		// Set MPC
		PORT3.PMR.BIT.B0 = 0;			// Disable PB1: peripheral
		PORT2.PMR.BIT.B6 = 0;			// Disable PB0: peripheral
		MPC.PWPR.BIT.B0WI = 0;			// Release protect
		MPC.PWPR.BIT.PFSWE = 1;
		MPC.P30PFS.BIT.PSEL = 0x0A;		// Set PB1: TXD1
		MPC.P26PFS.BIT.PSEL = 0x0A;		// Set PB0: RXD1
		MPC.PWPR.BIT.PFSWE = 0;			// Protect
		MPC.PWPR.BIT.B0WI = 1;
		PORT3.PMR.BIT.B0 = 1;			// PB1: peripheral
		PORT2.PMR.BIT.B6 = 1;			// PB0: peripheral
		
		
		
		// SCI12 Settings
		SCI1.SIMR3.BIT.IICSDAS = 3;	// SDA�[�q���n�C�C���s�[�_���X
		SCI1.SIMR3.BIT.IICSCLS = 3;	// SCL�[�q���n�C�C���s�[�_���X
		
		SCI1.SMR.BYTE = 0x00;		// PCLK�N���b�N
		
		SCI1.SCMR.BIT.SDIR = 1;		// MSB�t�@�[�X�g
		SCI1.SCMR.BIT.SINV = 0;		// ���M�A��M�f�[�^�����̂܂ܑ���M����
		SCI1.SCMR.BIT.SMIF = 0;		// �V���A���R�~���j�P�[�V�����C���^�[�t�F�C�X���[�h
		
		SCI1.BRR = 3;			// �r�b�g���[�g 375kHz
		
		SCI1.SPMR.BYTE = 0;
		SCI1.SEMR.BIT.NFEN = 0;		// �m�C�Y�����@�\����
		SCI1.SNFR.BIT.NFCS = 2;		// 2�����̃N���b�N���m�C�Y�t�B���^�Ɏg�p
		SCI1.SIMR1.BIT.IICM = 1;		// �Ȉ�IIC���[�h
		SCI1.SIMR1.BIT.IICDL = 1;		// 0�`1�T�C�N���x��
		SCI1.SIMR2.BIT.IICINTM = 1;	// ��M���荞�݁A���M���荞�݉\
		SCI1.SIMR2.BIT.IICCSC = 1;	// �N���b�N�������s��
		SCI1.SIMR2.BIT.IICACKT = 1;	// NACK���M�܂���ACK/NACK��M
		
		SCI1.SCR.BYTE = 0x30;		// Enable TX RX
	} else if ( mode == SPI ) {
		
	}

}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� charput								//
// �����T�v     printf�̏o��(printf�Ŏg�p����)					//
// ����         data:�o�͂���ꕶ��							//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void charput( uint8_t data )
{
	if ( !IMUSet ) {
		while(SCI1.SSR.BIT.TEND == 0);
		SCI1.TDR = data;
		SCI1.SSR.BIT.TEND = 0;
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� charget								//
// �����T�v     scanf�̓���(scanf�Ŏg�p����)					//
// ����         �Ȃ�									//
// �߂�l       data:���͂����ꕶ��							//
///////////////////////////////////////////////////////////////////////////
char charget(void)
{
	uint8_t data;
	return data;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI1_RXI1						//
// �����T�v     UART��M���Ɋ��荞�݂Ŏ��s�����				//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void Excep_SCI1_RXI1(void)
{
	char c;
	
	if ( SCI1_mode == UART ) {
		c = SCI1.RDR;
		if ( c == '$' ) {
			command = 1;	// �R�}���h���[�h�Ɉڍs
			txt = txt_command;
		} else {
			command = 0;	// �f�[�^���[�h�Ɉڍs
			txt = txt_data;
		}
		
		if ( command == 1 ) {	// �R�}���h���[�h�̂Ƃ�
			*txt++ = c;		
			if ( c == '\r' ) command = 2;	//�R�}���h�̔�����J�n
		} else {		// �f�[�^���[�h�̂Ƃ�
			*txt++ = c;
			cnt_byte++;
			if ( cnt_byte == 4 ) {	// 4�����ŏI��
				cnt_byte = 0;	// �f�[�^�����Z�b�g
				txt = txt_data;	// �A�h���X���Z�b�g
			}
		}
	} else if ( SCI1_mode == I2C ) {
		*SCI1_DataArry++ = SCI1.RDR;
		SCI1_NumData--;
	} else if ( SCI1_mode == SPI ) {
		
	}
}
/////////////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI1_TEI1								//
// �����T�v     �J�n/ �ĊJ�n/ ��~���������������Ɋ��荞�݂Ŏ��s�����	//
// ����         �Ȃ�										//
// �߂�l       �Ȃ�										//
/////////////////////////////////////////////////////////////////////////////////
void Excep_SCI1_TEI1 ( void )
{
	if ( SCI1_mode == UART ) {
		
	} else if ( SCI1_mode == I2C ) {
		if ( SCI1_Req_mode == 0 ) {
			// �X�^�[�g�R���f�B�V����
			SCI1.SIMR2.BIT.IICACKT = 1;	// NACK���M�܂���ACK/NACK��M
			SCI1.SIMR3.BYTE = 0x00;		// �f�[�^���M����
			SCI1.TDR = SCI1_Slaveaddr;	// �X���[�u�A�h���X��������
			SCI1.SSR.BIT.TEND = 0;
		} else if ( SCI1_Req_mode == 1 ) {
			// �X�g�b�v�R���f�B�V����
			SCI1.SIMR3.BYTE = 0xf0;	// SDA�[�q�ASCL�[�q���n�C�C���s�[�_���X
			SCI1_Req_mode = 0;	// �X�^�[�g�R���f�B�V�����҂�
			
			PORT5.PODR.BIT.B4 = 0;
			
			SCI1.SCR.BIT.TEIE = 0;	// STI���荞�݋֎~
			SCI1.SCR.BIT.TIE = 0;	// TXI���荞�݋֎~
		} else if ( SCI1_Req_mode == 2 ) {
			// ���X�^�[�g�R���f�B�V����
			SCI1_DataArry = SCI1_DataArry2;
			SCI1_NumData = SCI1_NumData2;
			
			SCI1_RW_mode = 0;	// ��M���[�h
			SCI1_Req_mode = 0;	// �X�^�[�g�R���f�B�V�����҂�
			SCI1.SIMR2.BIT.IICACKT = 1;	// NACK���M�܂���ACK/NACK��M
			SCI1.SIMR3.BYTE = 0x00;		// �f�[�^��M����
			SCI1.TDR = SCI1_Slaveaddr | RW_BIT;	// �X���[�u�A�h���X��������
			SCI1.SSR.BIT.TEND = 0;
		}
	} else if ( SCI1_mode == SPI ) {
		
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI1_TXI1						//
// �����T�v     ACK/NACK��M���Ɋ��荞�݂Ŏ��s�����			//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void Excep_SCI1_TXI1( void )
{
	if ( SCI1_mode == UART ) {
		
	} else if ( SCI1_mode == I2C ) {
		// �f�[�^���m�F
		if ( SCI1_NumData <= 1 ) {
			if ( SCI1_NumData == 0 && SCI1_RW_mode >= 1 ) {
				// ���M���[�h
				if ( SCI1_RW_mode == 1 ) {
					SCI1_Req_mode = 1;	// �X�g�b�v�R���f�B�V�����҂�
					SCI1.SIMR3.BYTE = 0x54;	// �X�g�b�v�R���f�B�V�������s
				} else {
					SCI1_Req_mode = 2;	// ���X�^�[�g�R���f�B�V�����҂�
					SCI1.SIMR3.BYTE = 0x52;	// ���X�^�[�g�R���f�B�V�������s
				}
			} else if ( SCI1_RW_mode == 0 ) {
				// ��M���[�h
				if ( SCI1_NumData ) {
					// �c��1
					SCI1.SIMR2.BIT.IICACKT = 1;	// NACK���M�܂���ACK/NACK��M
				} else {
					// �c��0
					SCI1.SCR.BIT.RIE = 0;	// RXI���荞�݋֎~
					SCI1_Req_mode = 1;	// �X�g�b�v�R���f�B�V�����҂�
					SCI1.SIMR3.BYTE = 0x54;	// �X�g�b�v�R���f�B�V�������s
				}
			}
		}
		if ( SCI1.SISR.BIT.IICACKR == 0 && SCI1_Req_mode == 0 ) {
			// �X���[�u�A�h���X���M���ACK��M
			SCI1_Req_mode = 3;	// �f�[�^����M��
			if ( SCI1_RW_mode ) {
				// ���M���[�h
				SCI1.TDR = *SCI1_DataArry++;	// ���M�f�[�^��������
				SCI1.SSR.BIT.TEND = 0;
				SCI1_NumData--;		// ���M�f�[�^����   
			} else {
				// ��M���[�h
				if ( SCI1_NumData >= 2 ) SCI1.SIMR2.BIT.IICACKT = 0;	// ACK���M����
				SCI1.SCR.BIT.RIE = 1;	// RXI���荞�݊J�n
				SCI1.TDR = 0xFF;	// �_�~�[�f�[�^��������
				SCI1.SSR.BIT.TEND = 0;
			}
		} else if ( SCI1.SISR.BIT.IICACKR == 1 ) {
			// NACK��M
			if ( SCI1_RW_mode ) {
				PORT5.PODR.BIT.B4 = 1;
				SCI1_Req_mode = 1;	// �X�g�b�v�R���f�B�V�����҂�
				SCI1.SIMR3.BYTE = 0x54;	// �X�g�b�v�R���f�B�V�������s
			}
		} else if ( SCI1_Req_mode == 3 ) {
			// ACK��M
			if ( SCI1_RW_mode ) {
				// ���M���[�h
				SCI1.TDR = *SCI1_DataArry++;	// ���M�f�[�^��������
				SCI1.SSR.BIT.TEND = 0;
				SCI1_NumData--;		// ���M�f�[�^����
			} else {
				// ��M���[�h
				SCI1.TDR = 0xFF;	// �_�~�[�f�[�^��������
				SCI1.SSR.BIT.TEND = 0;
			}
		}
	} else if ( SCI1_mode == SPI ) {
		
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� commandSCI1							//
// �����T�v     SCI1�Ŏ�M�����R�}���h����������				//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void commandSCI1 (void)
{
	short s,i;
	
	if ( command == 2 ) {	// �R�}���h�I�����Ɏ��s
		if ( strncmp( txt_command, COMMAND1, 3) == 0 ) {	
			cmmandMode = 1;		// �R�}���h�̎�ނ𔻒�
		} else {		
			printf("commandERROR\n");
		}
		
		switch ( cmmandMode ) {
		case 1:
			// �{�[���[�g�ݒ�(br)
			for ( s = 0; s < 15; s++ ) {
				i = ascii_num[s];
				if ( txt_command[3] == i ) {
					printf("br=%d\n", s);
					init_SCI1( UART, s );
					cmmandMode = 0;
					break;
				}
			}
			break;
			
		default:
			break;
		}
		command = 0;	// �R�}���h����I��
	}
	// �ً}��~
	if ( strcmp( txt_data, STOPWORD) == 0 ) {
		stopWord = 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� send_SCI1_I2c												//
// �����T�v     SCI1I2c�̑��M												//
// ����         slaveaddr:�X���[�u�A�h���X data:���M�f�[�^�̐擪�A�h���X num: ���M����f�[�^��	//
// �߂�l       �Ȃ�														//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void send_SCI1_I2c( char slaveaddr, char* data, char num )
{
	volatile char err = 0;
	
	cnt0 = 0;
	while ( SCI1.SIMR3.BYTE != 0xf0 ) {	// �o�X���t���[�ɂȂ�܂ő҂�
		if ( cnt0 ) {
			err = 1;
			SCI1.SIMR3.BYTE = 0xf0;
			break;
		}
	}
	
	if ( !err ) {
		SCI1_RW_mode = 1;	// ���M���[�h
		memcpy( SCI1_DataBuff, data, num );	// ���M�f�[�^���o�b�t�@�Ɉړ�
		
		SCI1_Slaveaddr = slaveaddr;
		SCI1_NumData = num;
		SCI1_DataArry = SCI1_DataBuff;
		
		SCI1_Req_mode = 0;
		SCI1.SCR.BIT.TEIE = 1;		// STI���荞�݋���
		SCI1.SCR.BIT.TIE = 1;		// TXI���荞�݋���
		
		SCI1.SIMR3.BYTE = 0x51;	// �X�^�[�g�R���f�B�V�������s
	}
	
	// �f�[�^�͊��荞�݂ő��M
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� send_SCI1_I2cWait											//
// �����T�v     SCI1I2c�̑��M(wait����)											//
// ����         slaveaddr:�X���[�u�A�h���X data:���M�f�[�^�̐擪�A�h���X num: ���M����f�[�^��	//
// �߂�l       0:ACK��M 1: NACK��M 2:�f�o�C�X�����ڑ�							//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
char send_SCI1_I2cWait( char slaveaddr, char* data, char num )
{
	volatile char err = 0;
	
	cnt0 = 0;
	while ( SCI1.SIMR3.BYTE != 0xf0 ) {	// �o�X���t���[�ɂȂ�܂ő҂�
		if ( cnt0 ) {
			err = 1;
			break;
		}
	}
	
	if ( !err ) {
		SCI1_RW_mode = 1;	// ���M���[�h
		memcpy( SCI1_DataBuff, data, num );	// ���M�f�[�^���o�b�t�@�Ɉړ�
		
		SCI1_Slaveaddr = slaveaddr;
		SCI1_NumData = num;
		SCI1_DataArry = SCI1_DataBuff;
		
		SCI1_Req_mode = 0;		// �X�^�[�g�R���f�B�V�����҂�
		SCI1.SCR.BIT.TEIE = 1;	// STI���荞�݋���
		SCI1.SCR.BIT.TIE = 1;	// TXI���荞�݋���
		SCI1.SIMR3.BYTE = 0x51;		// �X�^�[�g�R���f�B�V�������s
		// �f�[�^�͊��荞�݂ő��M
		cnt0 = 0;
		while ( SCI1.SIMR3.BYTE != 0xf0 ) {	// �o�X���t���[�ɂȂ�܂ő҂�
			if ( cnt0 ) {
				err = 1;
				break;
			}
		}
	}
	
	if ( err == 1 ) return 2;
	else return SCI1.SISR.BIT.IICACKR;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� receive_SCI1_I2c												//
// �����T�v     SCI1 I2c�̎�M												//
// ����         slaveaddr:�X���[�u�A�h���X data:���M�f�[�^�̐擪�A�h���X num: ���M����f�[�^��	//
// �߂�l       �Ȃ�														//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void receive_SCI1_I2c( char slaveaddr, char* data, char num )
{
	volatile char err = 0;
	
	cnt0 = 0;
	while ( SCI1.SIMR3.BYTE != 0xf0 ) {	// �o�X���t���[�ɂȂ�܂ő҂�
		if ( cnt0 ) {
			err = 1;
			break;
		}
	}
	
	if ( !err ) {
		SCI1_RW_mode = 0;	// ��M���[�h
		SCI1_Slaveaddr = slaveaddr | RW_BIT;
		SCI1_NumData = num;
		SCI1_DataArry = data;
		
		SCI1_Req_mode = 0;		// �X�^�[�g�R���f�B�V�����҂�
		SCI1.SCR.BIT.RIE = 0;		// RXI���荞�݋֎~
		SCI1.SCR.BIT.TEIE = 1;		// STI���荞�݋���
		SCI1.SCR.BIT.TIE = 1;		// TXI���荞�݋���
		
		SCI1.SIMR3.BYTE = 0x51;	// �X�^�[�g�R���f�B�V�������s
		
		// �f�[�^�͊��荞�݂ő��M
		PORT5.PODR.BIT.B2 = 1;
		while ( SCI1.SIMR3.BYTE != 0xf0 ) {	// �o�X���t���[�ɂȂ�܂ő҂�
			if ( cnt0 ) {
				err = 1;
				break;
			}
		}
		PORT5.PODR.BIT.B2 = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� receive_data_SCI1_I2c											//
// �����T�v     SCI1 I2c�̑���M												//
// ����         slaveaddr:�X���[�u�A�h���X data:���M�f�[�^�̐擪�A�h���X num: ���M����f�[�^��	//
// �߂�l       �Ȃ�														//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool receive_data_SCI1_I2c( char slaveaddr, char* sendData, char* receiveData, char num )
{
	volatile char err = 0;
	
	cnt0 = 0;
	while ( SCI1.SIMR3.BYTE != 0xf0 ) {	// �o�X���t���[�ɂȂ�܂ő҂�
		if ( cnt0 ) {
			err = 1;
			break;
		}
	}
	
	if ( !err ) {
		SCI1_RW_mode = 2;	// ���W�X�^�ǂݍ��݃��[�h
		
		memcpy( SCI1_DataBuff, sendData, 1 );	// ���M�f�[�^���o�b�t�@�Ɉړ�
		SCI1_Slaveaddr = slaveaddr;
		SCI1_NumData = 1;
		SCI1_DataArry = SCI1_DataBuff;
		
		SCI1_NumData2 = num;
		SCI1_DataArry2 = receiveData;
		
		SCI1_Req_mode = 0;		// �X�^�[�g�R���f�B�V�����҂�
		SCI1.SCR.BIT.RIE = 0;		// RXI���荞�݋֎~
		SCI1.SCR.BIT.TEIE = 1;		// STI���荞�݋���
		SCI1.SCR.BIT.TIE = 1;		// TXI���荞�݋���
		
		SCI1.SIMR3.BYTE = 0x51;	// �X�^�[�g�R���f�B�V�������s
		
		// �f�[�^�͊��荞�݂ő��M
		cnt0 = 0;
		while ( SCI1.SIMR3.BYTE != 0xf0 ) {	// �o�X���t���[�ɂȂ�܂ő҂�
			if ( cnt0 ) {
				err = 1;
				break;
			}
		}
	}
	if ( err == 1 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� init_SCI12								//
// �����T�v     SCI12�̏�����							//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void init_SCI12( void )
{
	IEN( SCI12, TXI12 ) = 1;			// TXI���荞�݊J�n
	IPR( SCI12, TXI12 ) = IPR_TXI12;	// TXI���荞�ݗD��x13
	IEN( SCI12, TEI12 ) = 1;			// TEIE���荞�݊J�n
	IPR( SCI12, TEI12 ) = IPR_TEI12;	// TEIE���荞�ݗD��x12
	
	// SCI12
	SYSTEM.PRCR.WORD = 0xA502;	// Release protect
	MSTP(SCI12) = 0;			// Wake up SCI12
	SYSTEM.PRCR.WORD = 0xA500;	// Protect
	
	SCI12.SCR.BYTE = 0;		// Set PFC of external pin used
	
	PORTE.ODR0.BYTE = 0x14;		// PE1 N�`�����l���I�[�v���h���C���o��
	
	// Set MPC
	PORTE.PMR.BIT.B1 = 0;		// Disable PE1: peripheral
	PORTE.PMR.BIT.B2 = 0;		// Disable PE2: peripheral
	MPC.PWPR.BIT.B0WI = 0;		// Release protect
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PE1PFS.BIT.PSEL = 0x0C;	// Set PE1: SSDA12
	MPC.PE2PFS.BIT.PSEL = 0x0C;	// Set PE2: SSCL12
	MPC.PWPR.BIT.PFSWE = 0;		// Protect
	MPC.PWPR.BIT.B0WI = 1;
	PORTE.PMR.BIT.B1 = 1;		// PE1: peripheral
	PORTE.PMR.BIT.B2 = 1;		// PE2: peripheral
	
	
	
	// SCI12 Settings
	SCI12.SIMR3.BIT.IICSDAS = 3;	// SDA�[�q���n�C�C���s�[�_���X
	SCI12.SIMR3.BIT.IICSCLS = 3;	// SCL�[�q���n�C�C���s�[�_���X
	
	SCI12.SMR.BYTE = 0x00;		// PCLK�N���b�N
	
	SCI12.SCMR.BIT.SDIR = 1;	// MSB�t�@�[�X�g
	SCI12.SCMR.BIT.SINV = 0;	// ���M�A��M�f�[�^�����̂܂ܑ���M����
	SCI12.SCMR.BIT.SMIF = 0;	// �V���A���R�~���j�P�[�V�����C���^�[�t�F�C�X
	
	SCI12.BRR = 3;			// 375kHz
	
	SCI12.SPMR.BYTE = 0;
	SCI12.SEMR.BIT.NFEN = 0;	// �m�C�Y�����@�\����
	SCI12.SNFR.BIT.NFCS = 2;	// 2�����̃N���b�N���m�C�Y�t�B���^�Ɏg�p
	SCI12.SIMR1.BIT.IICM = 1;	// �Ȉ�IIC���[�h
	SCI12.SIMR1.BIT.IICDL = 1;	// 0�`1�T�C�N���x��
	SCI12.SIMR2.BIT.IICINTM = 1;	// ��M���荞�݁A���M���荞�݉\
	SCI12.SIMR2.BIT.IICCSC = 1;	// �N���b�N�������s��
	SCI12.SIMR2.BIT.IICACKT = 1;	// NACK���M�܂���ACK/NACK��M
	
	SCI12.SCR.BYTE = 0x30;		// Enable TX RX
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� send_SCI12_I2c												//
// �����T�v     SCI12I2c�̑��M												//
// ����         slaveaddr:�X���[�u�A�h���X data:���M�f�[�^�̐擪�A�h���X num: ���M����f�[�^��	//
// �߂�l       �Ȃ�														//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void send_SCI12_I2c( char slaveaddr, char* data, char num )
{
	while ( SCI12.SIMR3.BYTE != 0xf0 );	// �o�X���t���[�ɂȂ�܂ő҂�
	
	memcpy( SCI12_DataBuff, data, num );	// ���M�f�[�^���o�b�t�@�Ɉړ�
	
	SCI12_Slaveaddr = slaveaddr;
	SCI12_NumData = num;
	SCI12_DataArry = SCI12_DataBuff;
	
	SCI12_Req_mode = 0;
	SCI12.SCR.BIT.TEIE = 1;		// STI���荞�݋���
	SCI12.SCR.BIT.TIE = 1;		// TXI���荞�݋���
	
	SCI12.SIMR3.BYTE = 0x51;	// �X�^�[�g�R���f�B�V�������s
	
	// �f�[�^�͊��荞�݂ő��M
}
/////////////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI12_TEI12							//
// �����T�v     �J�n/ �ĊJ�n/ ��~���������������Ɋ��荞�݂Ŏ��s�����	//
// ����         �Ȃ�										//
// �߂�l       �Ȃ�										//
/////////////////////////////////////////////////////////////////////////////////
void Excep_SCI12_TEI12 ( void )
{
	if ( SCI12_Req_mode == 0 ) {
		// �X�^�[�g�R���f�B�V����
		SCI12.SIMR3.BYTE = 0x00;	// �f�[�^���M����
		SCI12.TDR = SCI12_Slaveaddr;	// �X���[�u�A�h���X��������
	} else if ( SCI12_Req_mode == 1 ) {
		// �X�g�b�v�R���f�B�V����
		SCI12.SIMR3.BYTE = 0xf0;	// SDA�[�q�ASCL�[�q���n�C�C���s�[�_���X
		SCI12_Req_mode = 0;		// �X�^�[�g�R���f�B�V�����҂�
		
		SCI12.SCR.BIT.TEIE = 0;		// STI���荞�݋֎~
		SCI12.SCR.BIT.TIE = 0;		// TXI���荞�݋֎~
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI12_TXI12						//
// �����T�v     ACK/NACK��M���Ɋ��荞�݂Ŏ��s�����			//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void Excep_SCI12_TXI12( void )
{
	// �f�[�^���m�F
	if ( SCI12_NumData == 0 ) {
		SCI12_Req_mode = 1;		// �X�g�b�v�R���f�B�V�����҂�
		SCI12.SIMR3.BYTE = 0x54;	// �X�g�b�v�R���f�B�V�������s
	} else {
		if ( SCI12.SISR.BIT.IICACKR == 0 && SCI12_Req_mode == 0 ) {
			// ACK��M
			SCI12_Req_mode = 2;		// �f�[�^���M
			SCI12.TDR = *SCI12_DataArry++;	// ���M�f�[�^��������
			SCI12_NumData--;
		} else if ( SCI12.SISR.BIT.IICACKR == 1 && SCI12_Req_mode == 0 ) {
			// NACK��M
			SCI12_Req_mode = 1;		// �X�g�b�v�R���f�B�V�����҂�
			SCI12.SIMR3.BYTE = 0x54;	// �X�g�b�v�R���f�B�V�������s
		} else {
			SCI12.TDR = *SCI12_DataArry++;	// ���M�f�[�^��������
			SCI12_NumData--;
		}
	}
}



///////////////////////////////////////////////////////////////////////////
// ���W���[���� chaek_SCI_Error							//
// �����T�v     UART��M�G���[���Ɋ��荞�݂Ŏ��s�����			//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void chaek_SCI_Error( void )
{
	__clrpsw_i();
	if ( SCI1.SSR.BIT.ORER ) revErr = 1;
	if ( SCI1.SSR.BIT.FER ) revErr = 2;
	if ( SCI1.SSR.BIT.PER ) revErr = 3;
}