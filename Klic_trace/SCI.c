//======================================//
// �C���N���[�h                         //
//======================================//
#include "SCI.h"
#include "iodefine.h"
#include <machine.h>
#include "vect.h"
#include "R_PG_RX631_mcr_ver3.0.h"
#include "PeripheralFunctions.h"

//TXD PB1
//RXD PB0
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
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

char ascii_num[] = {48,49,50,51,52,53,54,55,56,57,97,98,99,100,101,102};
char reverr = 0;
char commandDataSelect = 0;

#pragma interrupt Excep_SCI1_RXI1 (vect = VECT_SCI1_RXI1)	// ��M���荞�݊֐���`
//#pragma interrupt (Excep_SCI1_RXI1(vect=217))	// ��M�G���[���荞�݊֐���`
//////////////////////////////////////////////////////////////////////////
// ���W���[���� init_SCI1						//
// �����T�v     SCI1�̏�����						//
// ����         rate:�{�[���[�g��bps�œ���				//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void init_SCI1( char rate ){
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
	
	SCI1.SCR.BYTE = 0;			//Set PFC of external pin used
	
	//__set_fintv((unsigned int*)FastInterupt);
	 
	ICU.IER[IER_SCI1_RXI1].BIT.IEN_SCI1_RXI1 = 1;	// RXI���荞�݊J�n
	ICU.IPR[VECT_SCI1_RXI1].BIT.IPR = 15;		// RXI���荞�݋���
	
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
	
	//ICU.FIR.BIT.FVCT = VECT_SCI1_RXI1;
	//ICU.FIR.BIT.FIEN = 1;
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
char charget(void){
	uint8_t data;
	return data;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� Excep_SCI1_RXI1						//
// �����T�v     UART��M���Ɋ��荞�݂Ŏ��s�����			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void Excep_SCI1_RXI1(void) {
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
		//printf("%c",c);
		cnt_byte++;
		if ( cnt_byte == 4 ) {	// 4�����ŏI��
			cnt_byte = 0;	// �f�[�^�����Z�b�g
			txt = txt_data;	// �A�h���X���Z�b�g
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� chaek_SCI1_Error					//
// �����T�v     UART��M�G���[���Ɋ��荞�݂Ŏ��s�����			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void chaek_SCI1_Error( void ) {
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
}