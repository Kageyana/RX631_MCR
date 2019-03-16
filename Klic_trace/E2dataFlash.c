//======================================//
// �C���N���[�h                         //
//======================================//
#include "E2dataFlash.h"
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// �^�C�}�֘A
unsigned short	cnt_flash;	// �t���b�V���p�J�E���g

// �t���b�V���֘A
volatile unsigned int 		beforeAddr;	// �O��ۑ����̃u���b�N�ԍ�
short 				flashDataBuff[45];	// �ꎞ�ۑ��o�b�t�@

volatile unsigned int 		nowAddr;		// ���݂̏������܂�Ă���u���b�N�ԍ�
//////////////////////////////////////////////////////////////////////////
// ���W���[���� wait_flash						//
// �����T�v     �x������						//
// ����         �x������(ms)						//
// �߂�l       �Ȃ�                                                    //
//////////////////////////////////////////////////////////////////////////
void wait_flash ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 )/ 16;
	for ( i = 0; i < time; i++) __nop();
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� FirmWareCopy						//
// �����T�v     FCU RAM�ւ̃t�@�[���E�F�A�R�s�[				//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void FirmWareCopy ( void )
{
	short i;
	static const int fcu_ram_size = 8 * 1024;				// �t�@�[���E�F�A�T�C�Y
	volatile unsigned int *fcu_ram = (unsigned int *)0x007f8000;		// FCU RAM�̃A�h���X
	const volatile unsigned int *fcu_firmware = (unsigned int *)0xfeffe000;	// FCU�t�@�[���E�F�A�̃A�h���X
	
	
	// ROM���[�h���[�h�m�F
	if (  FLASH.FENTRYR.WORD & 0x00ff ) FLASH.FENTRYR.WORD = 0xaa00;
	
	FLASH.FCURAME.WORD = 0xc401;	// FCU RAM�A�N�Z�X����

	// FCU�t�@�[���E�F�A��FCU RAM�ɃR�s�[
	for ( i = 0; i < fcu_ram_size / sizeof( unsigned int ); i++ ) {
		*fcu_ram++ = *fcu_firmware++;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� fcuCommandByte							//
// �����T�v     �w��A�h���X��FCU�R�}���h���s						//
// ����         Block_number: �u���b�N�ԍ��@command:FCU�R�}���h(16�i��)�o�C�g�T�C�Y 	//
//		Addr_number: �擪�A�h���X����̃I�t�Z�b�g�l				//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void fcuCommandByte ( unsigned int Addr, unsigned char command )
{
	volatile unsigned char *e2data;
	
	e2data = ( unsigned char *)Addr;	// �|�C���^�ɃA�h���X���
	*e2data = command;			// �w��A�h���X�ɃR�}���h���s
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� fcuCommandWord							//
// �����T�v     �w��A�h���X��FCU�R�}���h���s						//
// ����         Block_number: �u���b�N�ԍ��@command:FCU�R�}���h(16�i��)���[�h�T�C�Y 	//
//		Addr_number: �擪�A�h���X����̃I�t�Z�b�g�l				//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void fcuCommandWord ( unsigned int Addr, unsigned short command )
{
	volatile unsigned short *e2data;
	
	e2data = ( unsigned short *)Addr;	// �|�C���^�ɃA�h���X���
	*e2data = command;			// �w��A�h���X�ɃR�}���h���s
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� checkFRDY						//
// �����T�v     P/E�����������肷��					//
// ����         n�o�C�g�̃u���b�N�ɑ΂����������			//
// �߂�l      	0: �G���[�Ȃ� 1:�^�C���A�E�g�G���[			//
//////////////////////////////////////////////////////////////////////////
bool checkFRDY ( unsigned short waittime )
{
	volatile char timeout = 0;
	
	cnt_flash = 0;
	
	while( cnt_flash <= ( waittime + 10 ) ) {
		//printf("FRDY = %d cnt_flash = %d\n", FLASH.FSTATR0.BIT.FRDY, cnt_flash);
		if ( FLASH.FSTATR0.BIT.FRDY == 0 ) {
			if ( cnt_flash >= waittime ) {
				cnt_flash = 0;
				timeout = true;
				printf("FCU������\n");
				break;
			} else {
				continue;
			}
		} else {
			timeout = false;
			break;
		}
	}
	
	// FCU������
	if ( timeout ) {
		cnt_flash = 0;
		while( cnt_flash <= 5 ) {
			FLASH.FRESETR.BIT.FRESET = 1;
			if ( cnt_flash >= 5 ) {
				FLASH.FRESETR.BIT.FRESET = 0;
				FirmWareCopy();
				printf("FCU����������\n");
				break;
			}
		}
	} else if ( !timeout ){
		//printf("�^�C���A�E�g�Ȃ�\n");
	}
	return timeout;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� checkErrorFlash						//
// �����T�v     �G���[�m�F						//
// ����         �Ȃ�							//
// �߂�l       0: �G���[�Ȃ� 1:�G���[����				//
//////////////////////////////////////////////////////////////////////////
bool checkErrorFlash ( void )
{
	volatile char error;
	
	if ( FLASH.FSTATR0.BIT.ILGLERR == 1 || FLASH.FSTATR0.BIT.ERSERR == 1 || FLASH.FSTATR0.BIT.PRGERR == 1 ) {
		printf("�G���[�m�F ILGLERR = %d ERSERR = %d PRGERR = %d\n", FLASH.FSTATR0.BIT.ILGLERR, FLASH.FSTATR0.BIT.ERSERR, FLASH.FSTATR0.BIT.PRGERR);
		error = true;
		
		if ( FLASH.FSTATR0.BIT.ILGLERR == 1 ) {
			printf("�s���R�}���h���o\n");
			printf("ROMAE = %d DFLAE = %d DFLRPE = %d DFLWPE = %d\n", FLASH.FASTAT.BIT.ROMAE, FLASH.FASTAT.BIT.DFLAE, FLASH.FASTAT.BIT.DFLRPE, FLASH.FASTAT.BIT.DFLWPE);
			
			if ( FLASH.FASTAT.BYTE == 0x10 ) {
				printf("FCU�R�}���h�͎󂯕t���ĂȂ�\n");
			} else {
				FLASH.FASTAT.BYTE = 0x10;
				printf("FCU�R�}���h���󂯕t���Ă�\n");
			}
			fcuCommandByte( FLASHSTARTADDR, 0x50 );
		} else {
			printf("�X�e�[�^�X�N���A�R�}���h���s(ILGLERR = 0)\n");
		}
		fcuCommandByte( FLASHSTARTADDR, 0x50 );
	} else {
		//printf("�G���[�Ȃ�\n");
		error = false;
	}
	
	return error;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� inti_falsh						//
// �����T�v     E2�f�[�^�t���b�V���̏�����				//
// ����         �Ȃ�							//
// �߂�l       0:�G���[�Ȃ��@1:�G���[����				//
// �⑫		������� P/E�m�[�}�����[�h				//
//////////////////////////////////////////////////////////////////////////
bool initFlash ( void )
{	
	char ret = false;
	
	// FCU�t�@�[���E�F�A���R�s�[
	FirmWareCopy();
	
	// ���ӃN���b�N�ʒm�R�}���h���s
	// P/E���[�h�Ɉڍs
	if ( ( FLASH.FENTRYR.WORD & 0x00ff ) != 0x0080 ) FLASH.FENTRYR.WORD = 0xaa80;
	
	// E2�f�[�^�t���b�V���̈��P/E��������
	FLASH.DFLWE0.WORD = 0x1eff;
	
	// FCLK(48MHz)�����ӃN���b�N�ʒm���W�X�^�ɐݒ�
	FLASH.PCKAR.BIT.PCKA = 48;	
	
	// ���ӃN���b�N�ʒm�R�}���h���s
	fcuCommandByte( FLASHSTARTADDR, 0xe9 );
	fcuCommandByte( FLASHSTARTADDR, 0x03 );
	fcuCommandWord( FLASHSTARTADDR, 0x0f0f );
	fcuCommandWord( FLASHSTARTADDR, 0x0f0f );
	fcuCommandWord( FLASHSTARTADDR, 0x0f0f );
	fcuCommandByte(FLASHSTARTADDR, 0xd0 );
	
	// P/E���������m�F
	if ( checkFRDY(1) == 0 ) {
		// �G���[�m�F
		if ( checkErrorFlash() == 0 ) {
			// E2�f�[�^�t���b�V���̈��P/E��������
			FLASH.DFLWE0.WORD = 0x1e00;
		} else {
			printf("E2�f�[�^�t���b�V���������G���[\n");
			ret = true;
		}
	} else {
		printf("E2�f�[�^�t���b�V���������G���[\n");
		ret = true;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� changeFlashPE						//
// �����T�v     E2�f�[�^�t���b�V����P/E���[�h�ڍs			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
// �⑫		������� P/E�m�[�}�����[�h				//
//////////////////////////////////////////////////////////////////////////
void changeFlashPE ( void )
{
	// P/E���[�h�Ɉڍs
	if ( ( FLASH.FENTRYR.WORD & 0x00ff ) != 0x0080 ) FLASH.FENTRYR.WORD = 0xaa80;
	
	// P/E �\��Ԃɂ���
	FLASH.FWEPROR.BYTE = 0x01;
	
	// P/E���[�h�ڍs�R�}���h���s
	fcuCommandByte( FLASHSTARTADDR, 0xff );
	
	// �������m�F
	if ( checkFRDY( 300 ) == 0 ) {
		// �G���[�m�F
		if ( checkErrorFlash() == 0 ) {
			if ( checkErrorFlash() == 0 ) {
				//printf("P/E ���[�h�ڍs����\n");
			} else {
				printf("P/E ���[�h�ڍs�G���[\n");
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� changeFlashRead					//
// �����T�v     E2�f�[�^�t���b�V���̃��[�h���[�h�ڍs			//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void changeFlashRead ( void ) 
{
	
	// �������m�F
	if ( checkFRDY( 300 ) == 0 ) {
		// �G���[�m�F
		if ( checkErrorFlash() == 0 ) {
			// ROM/E2�f�[�^�t���b�V�����[�h���[�h�ֈڍs
			FLASH.FENTRYR.WORD = 0xaa00;
			if ( ( FLASH.FENTRYR.WORD & 0x00ff ) != 0x0000 ) FLASH.FENTRYR.WORD = 0xaa00;
			// �u�����N�`�F�b�N�֎~
			FLASH.FWEPROR.BYTE = 0x02;
			//printf("���[�h���[�h�ڍs����\n");
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
// ���W���[���� checkBlank							//
// �����T�v     �w��A�h���X�Ƀu�����N�`�F�b�N					//
// ����         Block_number: �u�����N�`�F�b�N����u���b�N�ԍ�			//
//		Addr_offset: �擪�A�h���X����̃I�t�Z�b�g�l			//
// �߂�l       0:�C���[�Y�ς݁@1:�������ݍς� -1:�G���[����			//
//////////////////////////////////////////////////////////////////////////////////
signed char checkBlank ( unsigned int Addr )
{
	volatile signed char ret;
	volatile unsigned int Block_number, lead_Addr, offset;

	// P/E���[�h�ڍs
	changeFlashPE();
	
	Block_number = ( Addr - FLASHSTARTADDR ) / 32;	// �u�����N�`�F�b�N����A�h���X���Z�o
	// �e�u���b�N�̐擪�A�h���X�Z�o
	if ( Block_number <= 63 ) lead_Addr = ( 0 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 64 && Block_number <= 127 ) lead_Addr = ( 64 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 128 && Block_number <= 191 ) lead_Addr = ( 128 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 192 && Block_number <= 255 ) lead_Addr = ( 192 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 256 && Block_number <= 319 ) lead_Addr = ( 256 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 320 && Block_number <= 383 ) lead_Addr = ( 320 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 384 && Block_number <= 447 ) lead_Addr = ( 384 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 448 && Block_number <= 511 ) lead_Addr = ( 448 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 512 && Block_number <= 575 ) lead_Addr = ( 512 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 576 && Block_number <= 639 ) lead_Addr = ( 576 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 640 && Block_number <= 703 ) lead_Addr = ( 640 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 704 && Block_number <= 767 ) lead_Addr = ( 704 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 768 && Block_number <= 831 ) lead_Addr = ( 768 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 832 && Block_number <= 895 ) lead_Addr = ( 832 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 896 && Block_number <= 959 ) lead_Addr = ( 896 * 32 ) + FLASHSTARTADDR;
	if ( Block_number >= 960 && Block_number <= 1023 ) lead_Addr = ( 960 * 32 ) + FLASHSTARTADDR;
	// �I�t�Z�b�g�ʎZ�o
	offset = Addr - lead_Addr;
	
	// �u�����N�`�F�b�N����
	FLASH.FMODR.BIT.FRDMD = 1;
	
	// �`�F�b�N�T�C�Y�ݒ�( 2B )
	FLASH.DFLBCCNT.BIT.BCSIZE = 0;
	FLASH.DFLBCCNT.BIT.BCADR = offset;
	
	// �u�����N�`�F�b�N�R�}���h���s
	fcuCommandByte( Addr, 0x71 );
	fcuCommandByte( Addr, 0xd0 );
	
	// P/E���������m�F
	if ( checkFRDY( 6 * 1.1 ) == 0 ) {
		// �G���[�m�F
		if ( checkErrorFlash() == 0 ) {
			// �u�����N�`�F�b�N���ʎ擾
			if ( FLASH.DFLBCSTAT.BIT.BCST == 1 ) {
				//printf("�A�h���X%p�͏������ݍς�\n", Addr);
				ret = 1;
			} else {
				//printf("�A�h���X%p�̓C���[�Y�ς�\n", Addr);
				ret = 0;
			}
		} else {
			ret = -1;
		}
	} else {
		ret = -1;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////
// ���W���[���� eraseE2DataFlash						//
// �����T�v     �w��f�[�^�u���b�N���C���[�Y					//
// ����         Block_number: �C���[�Y����u���b�N�ԍ��@			//
// �߂�l      	0:�C���[�Y���� 1:�C���[�Y�G���[					//
//////////////////////////////////////////////////////////////////////////////////
bool eraseE2DataFlash ( unsigned short Block_number )
{
	volatile char ret = false;
	unsigned int Addr;
	
	Addr = ( Block_number * 32 ) + FLASHSTARTADDR;	// �u���b�NN�̃A�h���X���Z�o
	
	// P/E���[�h�ڍs
	changeFlashPE();
	
	// �����v���e�N�g����
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	// �C���[�Y����
	FLASH.DFLWE0.WORD = 0x1eff;	// 0000�`0511�u���b�N�܂�P/E����
	FLASH.DFLWE1.WORD = 0xe1ff;	// 0512�`1023�u���b�N�܂�P/E����
	
	// �u���b�N�C���[�Y�R�}���h���s
	fcuCommandByte( Addr, 0x20 );
	fcuCommandByte( Addr, 0xd0 );
	
	if ( ( FLASH.DFLWE0.WORD & 0x00ff ) == 0x00ff ) {
		// P/E���������m�F
		if ( checkFRDY( 20 * 63 * 1.1 ) == 0 ) {
			// �G���[�m�F
			if ( checkErrorFlash() == 0 ) {
				FLASH.FWEPROR.BIT.FLWE = 2;	// �����v���e�N�g
				FLASH.DFLWE0.WORD = 0x1e00;	// 0000�`0511�u���b�N�܂�P/E����
				FLASH.DFLWE1.WORD = 0xe100;	// 0512�`1023�u���b�N�܂�P/E����
				//printf("Block%dErase\n", Block_number);
			}
		}
	} else {
		//printf("Block%dUnErase\n", Block_number);
		ret = true;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� checkWriteAddr										//
// �����T�v     startBlock����endbloock�܂ł̃C���[�Y�ςݗ̈��T��					//
// ����         startBlock: �J�n�u���b�N�ԍ� 	endbloock: �I���u���b�N�ԍ�				//
//		startoffset: �J�n�I�t�Z�b�g�l	width_data: �m�ۂ���f�[�^��				//
// �߂�l       0: �C���[�Y�ςݗ̈悠�� 1:�C���[�Y�ςݗ̈�Ȃ�						//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkWriteAddr ( unsigned int Addr, short endblock, short width_data, short startBlock )
{
	volatile short addrOffset, i, width = 0;
	volatile unsigned int	cheakAddr, leadAddr, limitAddr;

	cheakAddr = Addr;
	limitAddr = ( endblock * 32 ) + FLASHSTARTADDR + 30;
	//printf("width_data = %x\n", width_data);
	// �w�肵���f�[�^�ʂ̗̈悪������܂Ńu�����N�`�F�b�N����
	while ( cheakAddr <= limitAddr ) {
		if ( checkBlank( cheakAddr ) > 0 ) {
			width = 0;
		} else {
			width++;
			//printf("width = %x\n", width);
			if ( width == 1 ) {
				// �������݊J�n�A�h���X
				leadAddr = cheakAddr;
			}
			// �K�v�ȃf�[�^�ʊm�ۂł�����X�g�b�v
			if ( width >= width_data ) break;
		}
		cheakAddr += 2;
	}
	
	// �������ݗ̈悪�Ȃ���΃C���[�Y����
	if ( cheakAddr > limitAddr || width < width_data ) {
		//printf("startBlock = %d\n", startBlock);
		i = startBlock;
		while ( i <= endblock ) {
			//printf("i = %d\n", i);
			eraseE2DataFlash( i );
			i++;
		}
		
		// �J�n�ʒu�ɖ߂�
		leadAddr = ( startBlock * 32 ) + FLASHSTARTADDR;
	}
	//printf("leadAddr = %x\n", leadAddr);
	nowAddr = leadAddr;
}
//////////////////////////////////////////////////////////////////////////////////
// ���W���[���� writeFlash							//
// �����T�v     �w��A�h���X�ɏ�������						//
// ����         write_data:�������ރf�[�^�̂���z�� �@width_data:�f�[�^�̌�	//
// �߂�l       0: �������݊���	1: �G���[����					//
//////////////////////////////////////////////////////////////////////////////////
bool writeFlash ( short* write_data, short width_data )
{
	volatile char ret = false;
	volatile short*	sendData;	// �]���p�|�C���^
	volatile unsigned int Addr, i = 1;
	
	sendData = write_data;
	
	// P/E���[�h�ڍs
	changeFlashPE();
	
	// �������݃v���e�N�g����
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	// �v���O��������
	FLASH.DFLWE0.WORD = 0x1eff;	// 0000�`0511�u���b�N�܂�P/E����
	FLASH.DFLWE1.WORD = 0xe1ff;	// 0512�`1023�u���b�N�܂�P/E����
	
	while ( i <= width_data ) {
		if ( nowAddr <= ( 1023 * 32 ) + FLASHSTARTADDR + 30 ) {
			// �v���O�����R�}���h���s
			fcuCommandByte( nowAddr, 0xe8 );
			fcuCommandByte( nowAddr, 0x01 );
			fcuCommandWord( nowAddr, *sendData );	// ��������
			fcuCommandByte( nowAddr, 0xd0 );
			
			// P/E���������m�F
			if ( checkFRDY( 20 * 1.1 ) == 0 ) {
				// �G���[�m�F
				if ( checkErrorFlash() == 0 ) {
					//printf("Addr%x %dWritten\n", nowAddr, *sendData);
					
					nowAddr += 2;	// ���̃A�h���X�ֈړ�
					*sendData++;	// ���̏������݃f�[�^��ǂݍ���
					i++;			// �������݉�
				} else {
					ret = true;
					printf("Write Error\n");
					break;
				}
			} else {
				ret = true;
				printf("Write TimeOut\n");
				break;
			}
		} else {
			ret = true;
			printf("Addr Error\n");
			break;
		}
	}
	nowAddr -= 2;	// �ŏI�������݃A�h���X�ɂ���
	
	FLASH.FWEPROR.BIT.FLWE = 2;	// �������݃v���e�N�g
	FLASH.DFLWE0.WORD = 0x1e00;	// 0000�`0511�u���b�N�܂�P/E�֎~
	FLASH.DFLWE1.WORD = 0xe100;	// 0512�`1023�u���b�N�܂�P/E�֎~
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readFlashAddr								//
// �����T�v     �w��A�h���X�̒l��ǂ�							//
// ����         Addr: E2�f�[�^�t���b�V���̈�̃A�h���X					//
// �߂�l       Addr�̒l								//
//////////////////////////////////////////////////////////////////////////////////////////
short readFlashAddr ( unsigned int Addr )
{
	volatile short ret;
	
	// �t���b�V�����[�h���[�h�ڍs
	changeFlashRead();
	// E2�f�[�^�t���b�V���̈�̓ǂݏo��������
	FLASH.DFLRE0.WORD = 0x2dff;
	FLASH.DFLRE1.WORD = 0xd2ff;
	
	ret = *(volatile unsigned short *)Addr;
	//printf("Addr%x ReadData = %d\n", Addr, ret );
	// E2�f�[�^�t���b�V���̈�̓ǂݏo���֎~
	FLASH.DFLRE0.WORD = 0x2d00;
	FLASH.DFLRE1.WORD = 0xd200;
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readFlashArray								//
// �����T�v     �w�肵���̈�ɕۑ�����Ă���A�h���X�l����w�肳�ꂽ���̃f�[�^��ǂ�	//
// ����         dataarray: �ǂ񂾒l��ۑ�����z�� width_data: �f�[�^�̌�		//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashArray ( unsigned int Addr, volatile short* dataArray, short width_data )
{
	volatile short* flashDataBuff2;
	volatile short datasize, i = 0;
	volatile unsigned int DataAddr;	// �O��̃p�����[�^�̕ۑ��A�h���X
	
	flashDataBuff2 = dataArray;
	datasize = 2 * ( width_data - 1 );
	
	// �e�f�[�^�̃A�h���X�Z�o
	while ( i <= datasize ) {
		DataAddr= Addr - datasize + i;		// �f�[�^�̐擪����ǂݍ��݂���
		*flashDataBuff2++ = readFlashAddr ( DataAddr );
		i += 2;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readFlashArray2								//
// �����T�v     �w�肵���̈�ɕۑ�����Ă���A�h���X�l����w�肳�ꂽ���̃f�[�^��ǂ�	//
// ����         dataarray: �ǂ񂾒l��ۑ�����z�� width_data: �f�[�^�̌�		//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashArray2 ( unsigned int Addr, volatile short* dataArray, short width_data, short limitArea )
{
	volatile short* flashDataBuff2;
	volatile short datasize, i = 0;
	volatile unsigned int DataAddr, limitAddr;	// �O��̃p�����[�^�̕ۑ��A�h���X
	
	flashDataBuff2 = dataArray;
	
	while ( i <= 44 ) {		// ���͂��ꂽ�z����N���A
		*flashDataBuff2++ = 0;
		i++;
	}
	// �����A�h���X�Z�o
	limitAddr = ( (limitArea + 1) * 32 ) + 0x100000;
	
	DataAddr = Addr - limitAddr + 2;		// �������܂�Ă���f�[�^�����Z�o
	
	if ( DataAddr < (2 * width_data) ) {
		datasize = 2 * ( (DataAddr/2) - 1);
	} else {
		datasize = 2 * ( width_data - 1 );
	}
	
	// �e�f�[�^�̃A�h���X�Z�o
	i = 0;
	flashDataBuff2 = dataArray;
	while ( i <= datasize ) {
		DataAddr= Addr - i;
		*flashDataBuff2++ = readFlashAddr ( DataAddr );
		i += 2;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readbeforeAddr								//
// �����T�v     �w��u���b�N�ԂɋL�^����Ă���u���b�N�ԍ��A�I�t�Z�b�g�l��ǂ�		//
// ����         startBlock: �J�n�u���b�N�ԍ� 	endbloock: �I���u���b�N�ԍ�		//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void readbeforeAddr ( short startBlockNumber, short endBlockNumber )
{
	volatile unsigned int cheakAddr, limitAddr;
	
	// startBlockNumber�`endBlockNumber�ɏ������O�񏑂����ݎ��̍ŏI�A�h���X���擾����
	cheakAddr = ( startBlockNumber * 32 ) + FLASHSTARTADDR;	// �J�n�u���b�N�̃A�h���X���Z�o
	limitAddr = ( endBlockNumber * 32 ) + FLASHSTARTADDR + 30;	// �J�n�u���b�N�̃A�h���X���Z�o
	//printf("cheakAddr = %x\n", cheakAddr);
	if ( !checkBlank( cheakAddr )  ) {		// �ŏ��̃A�h���X���C���[�Y����Ă��邩�m�F
		// �C���[�Y�ς݂Ȃ珉���l�ɃZ�b�g����
		beforeAddr = ( ( endBlockNumber + 1 ) * 32 ) + FLASHSTARTADDR;
	} else {
		cheakAddr += 2;
		while ( checkBlank( cheakAddr ) > 0 ) {
			cheakAddr += 2;
			if ( cheakAddr >= limitAddr ) break;
		}
		cheakAddr -= 2;		// �������݃A�h���X�L�^�̈�ɖ߂�
		// �ŏI�u���b�N�ԍ��A�ŏI�I�t�Z�b�g�A�h���X�擾
		beforeAddr = readFlashAddr( cheakAddr ) + FLASHSTARTADDR;
	}
	//printf("beforeAddr = %x\n", beforeAddr);
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� writeFlashData								//
// �����T�v     �w��u���b�N�Ԃ�flashDataBuff���������݃A�h���X���L�^����		//
// ����          startBlock: �J�n�u���b�N�ԍ� 	endbloock: �I���u���b�N�ԍ�		//
//		endData: �f�[�^�ۑ��Ō�u���b�N	width_data: �m�ۂ���f�[�^��		//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void writeFlashData ( short startBlockNumber, short endBlockNumber, short endData, short width_data )
{
	// �O��ۑ����̃A�h���X�ǂݍ���
	readbeforeAddr( startBlockNumber, endBlockNumber );
	// �������ݗ̈�m��
	checkWriteAddr ( beforeAddr, endData, width_data, endBlockNumber + 1 );
	// �������݊J�n
	writeFlash ( flashDataBuff, width_data );

	
	// �ŏI�A�h���X�L�^
	flashDataBuff[ 0 ] = (short)( nowAddr - FLASHSTARTADDR );
	//printf("nowAddr = %x\n", nowAddr);
	//printf("flashDataBuff[ 0 ] = %x\n", flashDataBuff[ 0 ]);
	// �������ݗ̈�m��
	checkWriteAddr ( ( startBlockNumber * 32 ) + FLASHSTARTADDR, endBlockNumber, 1, startBlockNumber );
	// �������݊J�n
	writeFlash ( flashDataBuff, 1 );
	
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readFlashSetup								//
// �����T�v     �w��A�h���X�̒l��ǂ�							//
// ����         Addr: E2�f�[�^�t���b�V���̈�̃A�h���X					//
// �߂�l       Addr�̒l								//
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashSetup ( bool speed, bool C_angle, bool msd, bool pid_line, bool pid_angle, bool pid_speed, bool meter)
{
	short s, s2;
	// �t���b�V���ǂݍ��݊J�n
	printf("�O��p�����[�^�ǂݍ��݊J�n\n");
	
	// �p�����[�^�ǂݍ���
	if ( speed ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( ( PARAMETER_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			
			// �O��ۑ����̃A�h���X�ǂݍ���
			readbeforeAddr( PARAMETER_STARTAREA, PARAMETER_ENDAREA );
			// flashDataBuff�Ƀp�����[�^�ǂݍ���
			readFlashArray( beforeAddr, flashDataBuff, NUMDATA );
			
			// �e�f�[�^�擾
			speed_straight		= flashDataBuff[ 0 ];
			speed_curve_brake	= flashDataBuff[ 1 ];
			speed_curve_r600	= flashDataBuff[ 2 ];
			speed_curve_r450	= flashDataBuff[ 3 ];
			speed_curve_straight	= flashDataBuff[ 4 ];
			
			speed_crossline		= flashDataBuff[ 5 ];
			speed_ckank_trace	= flashDataBuff[ 6 ];
			speed_rightclank_curve	= flashDataBuff[ 7 ];
			speed_rightclank_escape	= flashDataBuff[ 8 ];
			speed_leftclank_curve	= flashDataBuff[ 9 ];
			speed_leftclank_escape	= flashDataBuff[ 10 ];
			
			speed_halfine		= flashDataBuff[ 11 ];
			speed_rightchange_trace = flashDataBuff[ 12 ];
			speed_rightchange_curve	= flashDataBuff[ 13 ];
			speed_rightchange_escape= flashDataBuff[ 14 ];
			speed_leftchange_trace 	= flashDataBuff[ 15 ];
			speed_leftchange_curve	= flashDataBuff[ 16 ];
			speed_leftchange_escape	= flashDataBuff[ 17  ];
			
			speed_slope_brake	= flashDataBuff[ 18 ];
			speed_slope_trace	= flashDataBuff[ 19 ];
					
			angle_rightclank	= flashDataBuff[ 20 ];
			angle_leftclank		= flashDataBuff[ 21 ];
			angle_rightchange	= flashDataBuff[ 22 ];
			angle_leftchange	= flashDataBuff[ 23 ];
		
		} else if ( checkBlank( ( PARAMETER_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			speed_straight		= SPEED_STRAIGHT;
			speed_curve_brake	= SPEED_CURVE_BRAKE;
			speed_curve_r600	= SPEED_CURVE_R600;
			speed_curve_r450	= SPEED_CURVE_R450;
			speed_curve_straight	= SPEED_CURVE_STRAIGHT;
			
			speed_crossline		= SPEED_CROSSLINE;
			speed_ckank_trace	= SPEED_CLANK_TRACE;
			speed_rightclank_curve	= SPEED_RIGHTCLANK_CURVE;
			speed_rightclank_escape	= SPEED_RIGHTCLANK_ESCAPE;
			speed_leftclank_curve	= SPEED_LEFTCLANK_CURVE;
			speed_leftclank_escape	= SPEED_LEFTCLANK_ESCAPE;
			
			speed_halfine		= SPEED_HALFLINE;
			speed_rightchange_trace = SPEED_RIGHTCHANGE_TRACE;
			speed_rightchange_curve	= SPEED_RIGHTCHANGE_CURVE;
			speed_rightchange_escape= SPEED_RIGHTCHANGE_ESCAPE;
			speed_leftchange_trace 	= SPEED_LEFTCHANGE_TRACE;
			speed_leftchange_curve	= SPEED_LEFTCHANGE_CURVE;
			speed_leftchange_escape	= SPEED_LEFTCHANGE_ESCAPE;
			
			speed_slope_brake	= SPEED_SLOPE_BRAKE;
			speed_slope_trace	= SPEED_SLOPE_TRACE;
			
			angle_rightclank	= ANGLE_RIGHTCLANK;
			angle_leftclank		= ANGLE_LEFTCLANK;
			angle_rightchange	= ANGLE_RIGHTCHANGE;
			angle_leftchange	= ANGLE_LEFTCHANGE;
			
			printf("Parameter Initialize\n");
		}
	}
		
	// Angle0�ǂݍ���
	if ( C_angle ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( ( ANGLE0_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( ANGLE0_STARTAREA, ANGLE0_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( beforeAddr, flashDataBuff, 1 );		// flashDataBuff��Angle0�ǂݍ���
			Angle0 = flashDataBuff[ 0 ];			// �f�[�^�擾
		} else if ( checkBlank( ( ANGLE0_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			Angle0 = SERVO_CENTER;
			printf("Angle0 Initialize\n");
		}
	}
	
	// msdWorkAddress�ǂݍ���
	if ( msd ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( ( MSD_STARTAREA *32 ) + FLASHSTARTADDR  ) ) {
			readbeforeAddr( MSD_STARTAREA, MSD_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray2( beforeAddr, flashDataBuff, 40, MSD_ENDAREA);// flashDataBuff��msdWorkAddress�ǂݍ��ݓǂݍ���
			// �f�[�^�擾
			// ���߂̃��O�A�h���X10���擾
			s = 0;
			s2 = 0;
			while ( s < 19 ) {
				// �I���A�h���X
				msdaddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+1 ] * 0x10000 + 
							(unsigned short)flashDataBuff[ s2 ]);
				// �J�n�A�h���X
				msdaddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+3 ] * 0x10000 + 
							(unsigned short)flashDataBuff[ s2+2 ]);
							
				if ( ( msdaddrBuff[s-2] - 1 ) == msdaddrBuff[s-1] ) {
					msdaddrBuff[s-1] = msdaddrBuff[s-2];		// �J�n�A�h���X
					msdaddrBuff[s-2] += MSD_ENDADDRESS;			// 15�b���̃A�h���X
					s2 += 2;
				} else {
					s2 += 4;
				}
			}
			// �ŐV�̃��O�A�h���X
			printf("msdAddrBuff[0] = %d msdAddrBuff[1] = %d\n", msdaddrBuff[0], msdaddrBuff[1]);
			printf("msdaddrBuff[2] = %d msdaddrBuff[3] = %d\n", msdaddrBuff[2], msdaddrBuff[3]);
			printf("msdaddrBuff[4] = %d msdaddrBuff[5] = %d\n", msdaddrBuff[4], msdaddrBuff[5]);
			printf("msdaddrBuff[6] = %d msdaddrBuff[7] = %d\n", msdaddrBuff[6], msdaddrBuff[7]);
			printf("msdaddrBuff[8] = %d msdaddrBuff[9] = %d\n", msdaddrBuff[8], msdaddrBuff[9]);
			printf("msdaddrBuff[10] = %d msdaddrBuff[11] = %d\n", msdaddrBuff[10], msdaddrBuff[11]);
			printf("msdaddrBuff[12] = %d msdaddrBuff[13] = %d\n", msdaddrBuff[12], msdaddrBuff[13]);
			printf("msdaddrBuff[14] = %d msdaddrBuff[15] = %d\n", msdaddrBuff[14], msdaddrBuff[15]);
			printf("msdaddrBuff[16] = %d msdaddrBuff[17] = %d\n", msdaddrBuff[16], msdaddrBuff[17]);
			printf("msdaddrBuff[18] = %d msdaddrBuff[19] = %d\n", msdaddrBuff[18], msdaddrBuff[19]);
			
			msdWorkaddress = msdaddrBuff[1];	// �O��J�n�A�h���X
			msdWorkaddress2 = msdaddrBuff[0];	// �O��I���A�h���X
		} else if ( checkBlank( ( MSD_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			msdWorkaddress = MSD_STARTADDRESS;	// �J�n�A�h���X
			msdWorkaddress2 = MSD_ENDADDRESS;	// �I���A�h���X
			printf("msdWorkAddress Initialize\n");
		}
	}
	
	// �����g���[�X�pPID�Q�C���ǂݍ���
	if ( pid_line ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( ( PID_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( PID_STARTAREA, PID_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( beforeAddr, flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
			// �f�[�^�擾
			kp_buff = flashDataBuff[ 0 ];
			ki_buff = flashDataBuff[ 1 ];
			kd_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( ( PID_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			kp_buff = KP;
			ki_buff = KI;
			kd_buff = KD;
			printf("PIDgain Initialize\n");
		}
	}
	
	// �p�x����pPID�Q�C���ǂݍ���
	if ( pid_angle ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( ( PID2_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( PID2_STARTAREA, PID2_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( beforeAddr, flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
			kp2_buff = flashDataBuff[ 0 ];
			ki2_buff = flashDataBuff[ 1 ];
			kd2_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( ( PID2_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			kp2_buff = KP2;
			ki2_buff = KI2;
			kd2_buff = KD2;
			printf("PID2gain Initialize\n");
		}
	}
	
	// ���x����pPID�Q�C���ǂݍ���
	if ( pid_speed ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( ( PID3_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( PID3_STARTAREA, PID3_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( beforeAddr, flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
			// �f�[�^�擾
			kp3_buff = flashDataBuff[ 0 ];
			ki3_buff = flashDataBuff[ 1 ];
			kd3_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( ( PID3_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			kp3_buff = KP3;
			ki3_buff = KI3;
			kd3_buff = KD3;
			printf("PID3gain Initialize\n");
		}
	}
	
	// ��~�����ǂݍ���
	if ( meter ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( ( STOPMETER_STARTAREA *32 ) + FLASHSTARTADDR ) ) {
			readbeforeAddr( STOPMETER_STARTAREA, STOPMETER_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( beforeAddr, flashDataBuff, 1 );				// flashDataBuff�ɒ�~�����ǂݍ���
			stopping_meter = flashDataBuff[ 0 ];				// �f�[�^�擾
		} else if ( checkBlank( ( STOPMETER_STARTAREA *32 ) + FLASHSTARTADDR ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			stopping_meter = STOPPING_METER;
			printf("StopMeter Initialize\n");
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� writeFlashBeforeStart							//
// �����T�v     �w��A�h���X�̒l��ǂ�							//
// ����         Addr: E2�f�[�^�t���b�V���̈�̃A�h���X					//
// �߂�l       Addr�̒l								//
//////////////////////////////////////////////////////////////////////////////////////////
void writeFlashBeforeStart ( bool speed, bool C_angle, bool pid_line, bool pid_angle, bool pid_speed, bool meter )
{
	// �t���b�V���������݊J�n
	if ( speed ) {
		// �e���x�f�[�^���o�b�t�@�ɕۑ�
		flashDataBuff[ 0 ] = speed_straight;
		flashDataBuff[ 1 ] = speed_curve_brake;
		flashDataBuff[ 2 ] = speed_curve_r600;
		flashDataBuff[ 3 ] = speed_curve_r450;
		flashDataBuff[ 4 ] = speed_curve_straight;
		
		flashDataBuff[ 5 ] = speed_crossline;
		flashDataBuff[ 6 ] = speed_ckank_trace;
		flashDataBuff[ 7 ] = speed_rightclank_curve;
		flashDataBuff[ 8 ] = speed_rightclank_escape;
		flashDataBuff[ 9 ] = speed_leftclank_curve;
		flashDataBuff[ 10 ] = speed_leftclank_escape;
		
		flashDataBuff[ 11 ] = speed_halfine;
		flashDataBuff[ 12 ] = speed_rightchange_trace;
		flashDataBuff[ 13 ] = speed_rightchange_curve;
		flashDataBuff[ 14 ] = speed_rightchange_escape;
		flashDataBuff[ 15 ] = speed_leftchange_trace;
		flashDataBuff[ 16 ] = speed_leftchange_curve;
		flashDataBuff[ 17 ] = speed_leftchange_escape;
		
		flashDataBuff[ 18 ] = speed_slope_brake;
		flashDataBuff[ 19 ] = speed_slope_trace;
		
		flashDataBuff[ 20 ] = angle_rightclank;
		flashDataBuff[ 21 ] = angle_leftclank;
		flashDataBuff[ 22 ] = angle_rightchange;
		flashDataBuff[ 23 ] = angle_leftchange;
		
		writeFlashData( PARAMETER_STARTAREA, PARAMETER_ENDAREA, PARAMETER_AREA, NUMDATA );
	}
	
	if ( C_angle ) {
		// �|�e���V�������[�^0���l(Angle0)�ۑ�
		flashDataBuff[0] = Angle0;
		writeFlashData( ANGLE0_STARTAREA, ANGLE0_ENDAREA, ANGLE0_DATA, 1 );
	}
	
	if ( pid_line ) {
		// �����g���[�X�pPID�Q�C���ۑ�
		flashDataBuff[ 0 ] = kp_buff;
		flashDataBuff[ 1 ] = ki_buff;
		flashDataBuff[ 2 ] = kd_buff;
		writeFlashData( PID_STARTAREA, PID_ENDAREA, PID_DATA, 3 );
	}
	
	if ( pid_angle ) {
		// �p�x����pPID�Q�C���ۑ�
		flashDataBuff[ 0 ] = kp2_buff;
		flashDataBuff[ 1 ] = ki2_buff;
		flashDataBuff[ 2 ] = kd2_buff;
		writeFlashData( PID2_STARTAREA, PID2_ENDAREA, PID2_DATA, 3 );
	}
	
	if ( pid_speed ) {
		// ���x����pPID�Q�C���ۑ�
		flashDataBuff[ 0 ] = kp3_buff;
		flashDataBuff[ 1 ] = ki3_buff;
		flashDataBuff[ 2 ] = kd3_buff;
		writeFlashData( PID3_STARTAREA, PID3_ENDAREA, PID3_DATA, 3 );
	}
	
	if ( meter ) {
		// ��~�����ۑ�
		flashDataBuff[ 0 ] = stopping_meter;
		writeFlashData( STOPMETER_STARTAREA, STOPMETER_ENDAREA, STOPMETER_DATA, 1 );
	}
}