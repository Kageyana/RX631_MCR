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
volatile short 		BeforeBlockNumber;	// �O��ۑ����̃u���b�N�ԍ�
volatile short 		BeforeAddrNumber;	// �O��ۑ����̃I�t�Z�b�g�l
volatile short 		EndBlockNumber;		// ���񏑂����݂̍ŏI�u���b�N�ԍ�
volatile short 		EndAddrOffset;		// ���񏑂����݂̍ŏI�I�t�Z�b�g�l
short 			flashDataBuff[45];	// �ꎞ�ۑ��o�b�t�@

static volatile short 		NowBlockNumber;		// ���݂̏������܂�Ă���u���b�N�ԍ�
static volatile short 		NowAddrOffset;		// ���݂̏������܂�Ă���I�t�Z�b�g�l
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
void fcuCommandByte ( unsigned short Block_number, unsigned char command, char Addr_number )
{
	volatile unsigned char *e2data;
	volatile unsigned int Addr;
	
	Addr = ( Block_number * 32 ) + 0x00100000 + Addr_number;	// �u���b�NN�̃A�h���X���Z�o
	
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
void fcuCommandWord ( unsigned short Block_number, unsigned short command, char Addr_number )
{
	volatile unsigned short *e2data;
	volatile unsigned int Addr;
	
	Addr = ( Block_number * 32 ) + 0x00100000 + Addr_number;	// �u���b�NN�̃A�h���X���Z�o
	
	e2data = ( unsigned short *)Addr;	// �|�C���^�ɃA�h���X���
	*e2data = command;			// �w��A�h���X�ɃR�}���h���s
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� checkFRDY						//
// �����T�v     P/E�����������肷��					//
// ����         n�o�C�g�̃u���b�N�ɑ΂����������			//
// �߂�l      	0: �G���[�Ȃ� 1:�^�C���A�E�g�G���[			//
//////////////////////////////////////////////////////////////////////////
char checkFRDY ( unsigned short waittime )
{
	volatile char timeout = 0;
	
	cnt_flash = 0;
	
	while( cnt_flash <= ( waittime + 10 ) ) {
		//printf("FRDY = %d cnt_flash = %d\n", FLASH.FSTATR0.BIT.FRDY, cnt_flash);
		if ( FLASH.FSTATR0.BIT.FRDY == 0 ) {
			if ( cnt_flash >= waittime ) {
				cnt_flash = 0;
				timeout = 1;
				//printf("FCU������\n");
				break;
			} else {
				continue;
			}
		} else {
			timeout = 0;
			break;
		}
	}
	
	// FCU������
	if ( timeout == 1 ) {
		cnt_flash = 0;
		while( cnt_flash <= 5 ) {
			FLASH.FRESETR.BIT.FRESET = 1;
			if ( cnt_flash >= 5 ) {
				FLASH.FRESETR.BIT.FRESET = 0;
				FirmWareCopy();
				//printf("FCU����������\n");
				break;
			}
		}
	} else if ( timeout == 0 ){
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
char checkErrorFlash ( void )
{
	volatile char error;
	
	if ( FLASH.FSTATR0.BIT.ILGLERR == 1 || FLASH.FSTATR0.BIT.ERSERR == 1 || FLASH.FSTATR0.BIT.PRGERR == 1 ) {
		//printf("�G���[�m�F ILGLERR = %d ERSERR = %d PRGERR = %d\n", FLASH.FSTATR0.BIT.ILGLERR, FLASH.FSTATR0.BIT.ERSERR, FLASH.FSTATR0.BIT.PRGERR);
		error = 1;
		
		if ( FLASH.FSTATR0.BIT.ILGLERR == 1 ) {
			//printf("�s���R�}���h���o\n");
			//printf("ROMAE = %d DFLAE = %d DFLRPE = %d DFLWPE = %d\n", FLASH.FASTAT.BIT.ROMAE, FLASH.FASTAT.BIT.DFLAE, FLASH.FASTAT.BIT.DFLRPE, FLASH.FASTAT.BIT.DFLWPE);
			
			if ( FLASH.FASTAT.BYTE == 0x10 ) {
				fcuCommandByte( 0, 0x50, 0 );
				//printf("�X�e�[�^�X�N���A�R�}���h���s(FCU�R�}���h�͎󂯕t���ĂȂ�)\n");
			} else {
				FLASH.FASTAT.BYTE = 0x10;
				fcuCommandByte( 0, 0x50, 0 );
				//printf("�X�e�[�^�X�N���A�R�}���h���s(FCU�R�}���h���󂯕t���Ă�)\n");
			}
		} else {
			fcuCommandByte( 0, 0x50, 0 );
			//printf("�X�e�[�^�X�N���A�R�}���h���s(ILGLERR = 0)\n");
		}
	} else {
		//printf("�G���[�Ȃ�\n");
		error = 0;
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
char initFlash ( void )
{	
	char ret = 0;
	
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
	fcuCommandByte( 0, 0xe9, 0 );
	fcuCommandByte( 0, 0x03, 0 );
	fcuCommandWord( 0, 0x0f0f, 0 );
	fcuCommandWord( 0, 0x0f0f, 0 );
	fcuCommandWord( 0, 0x0f0f, 0 );
	fcuCommandByte( 0, 0xd0, 0 );
	
	// P/E���������m�F
	if ( checkFRDY(1) == 0 ) {
		// �G���[�m�F
		if ( checkErrorFlash() == 0 ) {
			// E2�f�[�^�t���b�V���̈��P/E��������
			FLASH.DFLWE0.WORD = 0x1e00;
		} else {
			//printf("E2�f�[�^�t���b�V���������G���[\n");
			ret = 1;
		}
	} else {
		//printf("E2�f�[�^�t���b�V���������G���[\n");
		ret = 1;
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
	fcuCommandByte( 0, 0xff, 0 );
	
	// �������m�F
	if ( checkFRDY( 300 ) == 0 ) {
		// �G���[�m�F
		if ( checkErrorFlash() == 0 ) {
			if ( checkErrorFlash() == 0 ) {
				//printf("P/E ���[�h�ڍs����\n");
			} else {
				//printf("P/E ���[�h�ڍs�G���[\n");
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
signed char checkBlank ( short Block_number, char Addr_offset )
{
	volatile signed char ret;
	volatile unsigned int Addr, lead_Addr, offset;

	// P/E���[�h�ڍs
	changeFlashPE();
	
	Addr = ( Block_number * 32 ) + 0x00100000 + Addr_offset;	// �u�����N�`�F�b�N����A�h���X���Z�o
	// �e�u���b�N�̐擪�A�h���X�Z�o
	if ( Block_number >= 0 && Block_number <= 63 ) lead_Addr = ( 0 * 32 ) + 0x00100000;
	if ( Block_number >= 64 && Block_number <= 127 ) lead_Addr = ( 64 * 32 ) + 0x00100000;
	if ( Block_number >= 128 && Block_number <= 191 ) lead_Addr = ( 128 * 32 ) + 0x00100000;
	if ( Block_number >= 192 && Block_number <= 255 ) lead_Addr = ( 192 * 32 ) + 0x00100000;
	if ( Block_number >= 256 && Block_number <= 319 ) lead_Addr = ( 256 * 32 ) + 0x00100000;
	if ( Block_number >= 320 && Block_number <= 383 ) lead_Addr = ( 320 * 32 ) + 0x00100000;
	if ( Block_number >= 384 && Block_number <= 447 ) lead_Addr = ( 384 * 32 ) + 0x00100000;
	if ( Block_number >= 448 && Block_number <= 511 ) lead_Addr = ( 448 * 32 ) + 0x00100000;
	if ( Block_number >= 512 && Block_number <= 575 ) lead_Addr = ( 512 * 32 ) + 0x00100000;
	if ( Block_number >= 576 && Block_number <= 639 ) lead_Addr = ( 576 * 32 ) + 0x00100000;
	if ( Block_number >= 640 && Block_number <= 703 ) lead_Addr = ( 640 * 32 ) + 0x00100000;
	if ( Block_number >= 704 && Block_number <= 767 ) lead_Addr = ( 704 * 32 ) + 0x00100000;
	if ( Block_number >= 768 && Block_number <= 831 ) lead_Addr = ( 768 * 32 ) + 0x00100000;
	if ( Block_number >= 832 && Block_number <= 895 ) lead_Addr = ( 832 * 32 ) + 0x00100000;
	if ( Block_number >= 896 && Block_number <= 959 ) lead_Addr = ( 896 * 32 ) + 0x00100000;
	if ( Block_number >= 960 && Block_number <= 1023 ) lead_Addr = ( 960 * 32 ) + 0x00100000;
	// �I�t�Z�b�g�ʎZ�o
	offset = Addr - lead_Addr;
	
	// �u�����N�`�F�b�N����
	FLASH.FMODR.BIT.FRDMD = 1;
	
	// �`�F�b�N�T�C�Y�ݒ�( 2B )
	FLASH.DFLBCCNT.BIT.BCSIZE = 0;
	FLASH.DFLBCCNT.BIT.BCADR = offset;
	
	// �u�����N�`�F�b�N�R�}���h���s
	fcuCommandByte( Block_number, 0x71, 0 );
	fcuCommandByte( Block_number, 0xd0, Addr_offset );
	
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
char eraseE2DataFlash ( unsigned short Block_number )
{
	volatile char ret = 0;
	
	// P/E���[�h�ڍs
	changeFlashPE();
	
	// �����v���e�N�g����
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	// �C���[�Y����
	FLASH.DFLWE0.WORD = 0x1eff;	// 0000�`0511�u���b�N�܂�P/E����
	FLASH.DFLWE1.WORD = 0xe1ff;	// 0512�`1023�u���b�N�܂�P/E����
	
	// �u���b�N�C���[�Y�R�}���h���s
	fcuCommandByte( 0, 0x20, 0 );
	fcuCommandByte( Block_number, 0xd0, 0 );
	
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
		ret = 1;
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
void checkWriteAddr ( short startNumber, char startOffset, short endblock, short width_data, short startBlock )
{
	volatile char 	addrOffset, leadAddrOffset;
	volatile short	blockNumber, leadBlockNumber, i, width = 0;
	
	blockNumber = startNumber;
	addrOffset = startOffset;

	// �w�肵���f�[�^�ʂ̗̈悪������܂Ńu�����N�`�F�b�N����
	while ( blockNumber <= endblock ) {
		if ( checkBlank( blockNumber, addrOffset ) > 0 ) {
			width = 0;
		} else if ( checkBlank( blockNumber, addrOffset ) == 0 ) {
			width++;
			if ( width == 1 ) {
				// �������݊J�n�A�h���X
				leadBlockNumber = blockNumber;
				leadAddrOffset = addrOffset;
			}
			// �K�v�ȃf�[�^�ʊm�ۂł�����X�g�b�v
			if ( width >= width_data ) break;
		} else {
			//printf("BlankCheakError\n");
			break;
		}
		
		addrOffset += 2;
		if ( addrOffset == 32 ) {
			// ���̃u���b�N�Ɉړ�����
			addrOffset = 0;
			blockNumber++;
		}
	}
	//printf("blockNumber = %d\n", blockNumber);
	//printf("addrOffset = %d\n", addrOffset);
	// �������ݗ̈悪�Ȃ���΃C���[�Y����
	if ( blockNumber > endblock || width < width_data ) {
		//printf("startBlock = %d\n", startBlock);
		i = startBlock;
		while ( i <= endblock ) {
			//printf("i = %d\n", i);
			eraseE2DataFlash(i);
			i++;
		}
		
		// �J�n�ʒu�ɖ߂�
		leadBlockNumber = startBlock + 1;
		leadAddrOffset = 0;
	}
	
	NowBlockNumber = leadBlockNumber;
	NowAddrOffset = leadAddrOffset;
}
//////////////////////////////////////////////////////////////////////////////////
// ���W���[���� writeFlash							//
// �����T�v     �w��A�h���X�ɏ�������						//
// ����         write_data:�������ރf�[�^�̂���z�� �@width_data:�f�[�^�̌�	//
// �߂�l       0: �������݊���	1: �G���[����					//
//////////////////////////////////////////////////////////////////////////////////
char writeFlash ( short* write_data, short width_data )
{
	volatile char ret = 0;
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
		if ( NowBlockNumber <= 1023 ) {
			// �v���O�����R�}���h���s
			fcuCommandByte( NowBlockNumber, 0xe8, 0 );
			fcuCommandByte( NowBlockNumber, 0x01, 0 );
			fcuCommandWord( NowBlockNumber, *sendData, NowAddrOffset );	// ��������
			fcuCommandByte( NowBlockNumber, 0xd0, 0 );
			
			// P/E���������m�F
			if ( checkFRDY( 20 * 1.1 ) == 0 ) {
				// �G���[�m�F
				if ( checkErrorFlash() == 0 ) {
					Addr = ( NowBlockNumber * 32 ) + 0x00100000 + NowAddrOffset;	// �u���b�NN�̃A�h���X���Z�o
					//printf("Addr%p(%d�u���b�N %d�I�t�Z�b�g) %dWritten\n", Addr, NowBlockNumber, NowAddrOffset, *sendData);
					
					NowAddrOffset += 2;
					if ( NowAddrOffset == 32 ) {
						NowAddrOffset = 0;
						NowBlockNumber++;
					}
					*sendData++;
					i++;
				} else {
					ret = 1;
					//printf("Write Error\n");
					break;
				}
			} else {
				ret = 1;
				//printf("Write TimeOut\n");
				break;
			}
		} else {
			ret = 1;
			//printf("BlockNumber Error\n");
			break;
		}
	}
	
	FLASH.FWEPROR.BIT.FLWE = 2;	// �������݃v���e�N�g
	FLASH.DFLWE0.WORD = 0x1e00;	// 0000�`0511�u���b�N�܂�P/E�֎~
	FLASH.DFLWE1.WORD = 0xe100;	// 0512�`1023�u���b�N�܂�P/E�֎~
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readFlashBlock								//
// �����T�v     �w��u���b�N�ԍ��A�w��I�t�Z�b�g�l�ɏ�����Ă���l��ǂ�		//
// ����         Block_number:�u���b�N�ԍ� Addr_number: �擪�A�h���X����̃I�t�Z�b�g�l	//
// �߂�l       �ǂݍ��񂾒l								//
//////////////////////////////////////////////////////////////////////////////////////////
short readFlashBlock ( short Block_number, char Addr_offset )
{
	volatile short ret;
	volatile unsigned int Addr;
	
	// �t���b�V�����[�h���[�h�ڍs
	changeFlashRead();
	// E2�f�[�^�t���b�V���̈�̓ǂݏo��������
	FLASH.DFLRE0.WORD = 0x2dff;
	FLASH.DFLRE1.WORD = 0xd2ff;
	Addr = ( Block_number * 32 ) + 0x00100000 + Addr_offset;	// �u���b�NN�̃A�h���X���Z�o
	
	ret = *(volatile unsigned short *)Addr;
	
	// E2�f�[�^�t���b�V���̈�̓ǂݏo���֎~
	FLASH.DFLRE0.WORD = 0x2d00;
	FLASH.DFLRE1.WORD = 0xd200;
	
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
	//printf("ReadData = %d\n", ret );
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
void readFlashArray ( volatile short* dataArray, short width_data )
{
	volatile short* flashDataBuff2;
	volatile short datasize, i = 0;
	volatile unsigned int EndAddr, DataAddr;	// �O��̃p�����[�^�̕ۑ��A�h���X
	
	flashDataBuff2 = dataArray;
	
	// �ŏI�A�h���X�Z�o
	EndAddr = ( BeforeBlockNumber * 32 ) + 0x100000 + BeforeAddrNumber;
	datasize = 2 * ( width_data - 1 );
	
	// �e�f�[�^�̃A�h���X�Z�o
	while ( i <= datasize ) {
		DataAddr= EndAddr - datasize + i;
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
void readFlashArray2 ( volatile short* dataArray, short width_data, short limitArea )
{
	volatile short* flashDataBuff2;
	volatile short datasize, i = 0;
	volatile unsigned int EndAddr, DataAddr, limitAddr;	// �O��̃p�����[�^�̕ۑ��A�h���X
	
	flashDataBuff2 = dataArray;
	
	while ( i <= 44 ) {
		*flashDataBuff2++ = 0;
		i++;
	}
	// �ŏI�A�h���X�Z�o
	EndAddr = ( BeforeBlockNumber * 32 ) + 0x100000 + BeforeAddrNumber;
	// �����A�h���X�Z�o
	limitAddr = ( (limitArea + 1) * 32 ) + 0x100000;
	
	DataAddr = EndAddr - limitAddr + 2;		// �������܂�Ă���f�[�^�����Z�o
	
	if ( DataAddr < (2 * width_data) ) {
		datasize = 2 * ( (DataAddr/2) - 1);
	} else {
		datasize = 2 * ( width_data - 1 );
	}
	
	// �e�f�[�^�̃A�h���X�Z�o
	i = 0;
	flashDataBuff2 = dataArray;
	while ( i <= datasize ) {
		DataAddr= EndAddr - i;
		*flashDataBuff2++ = readFlashAddr ( DataAddr );
		i += 2;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readBeforeAddr								//
// �����T�v     �w��u���b�N�ԂɋL�^����Ă���u���b�N�ԍ��A�I�t�Z�b�g�l��ǂ�		//
// ����         startBlock: �J�n�u���b�N�ԍ� 	endbloock: �I���u���b�N�ԍ�		//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void readBeforeAddr ( short startBlockNumber, short endBlockNumber )
{
	volatile short Blocknumber;	// �O��ۑ����̃u���b�N�ԍ��̏����Ă���u���b�N�ԍ�
	volatile short Addroffset = 0;
	
	// startBlockNumber�`endBlockNumber�ɏ������O�񏑂����ݎ��̍ŏI�A�h���X���擾����
	Blocknumber = startBlockNumber;
	if ( checkBlank( Blocknumber, Addroffset ) == 0 ) {
		// �C���[�Y�ς݂Ȃ珉���l�ɃZ�b�g����
		BeforeBlockNumber = endBlockNumber + 1;
		BeforeAddrNumber = 0;
	} else {
		while ( checkBlank( Blocknumber, Addroffset ) == 1 ) {
			Addroffset += 2;
			if ( Addroffset == 32 ) {
				// ���̃u���b�N�Ɉړ�����
				Addroffset = 0;
				Blocknumber++;
				if ( Blocknumber == endBlockNumber + 1 ) break;
			}
		}
		
		// �u���b�N���܂�������1�O�̃u���b�N���w�肷��
		if ( Addroffset == 0 ) {
			Blocknumber--;
			Addroffset = 32;
		}
		// �ŏI�u���b�N�ԍ��A�ŏI�I�t�Z�b�g�A�h���X�擾
		BeforeBlockNumber = readFlashBlock( Blocknumber, ( Addroffset - 4 ) );
		BeforeAddrNumber = readFlashBlock( Blocknumber, ( Addroffset - 2 ) );
	}
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
	readBeforeAddr( startBlockNumber, endBlockNumber );
	// �������ݗ̈�m��
	checkWriteAddr ( BeforeBlockNumber, BeforeAddrNumber, endData, width_data, endBlockNumber + 1 );
	// �������݊J�n
	writeFlash ( flashDataBuff, width_data );

	
	// �ŏI�u���b�N�ԍ��y�уI�t�Z�b�g�l�L�^
	EndAddrOffset = NowAddrOffset - 2;
	EndBlockNumber = NowBlockNumber;
	if ( EndAddrOffset == -2 ) {
		EndAddrOffset = 30;
		EndBlockNumber = NowBlockNumber - 1;
	}
	flashDataBuff[ 0 ] = EndBlockNumber;
	flashDataBuff[ 1 ] = EndAddrOffset;
	
	// �������ݗ̈�m��
	checkWriteAddr ( startBlockNumber, 0, endBlockNumber, 2, startBlockNumber );
	led_out(0x1f);
	// �������݊J�n
	writeFlash ( flashDataBuff, 2 );
	
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readFlashSetup								//
// �����T�v     �w��A�h���X�̒l��ǂ�							//
// ����         Addr: E2�f�[�^�t���b�V���̈�̃A�h���X					//
// �߂�l       Addr�̒l								//
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashSetup ( bool speed, bool C_angle, bool msd, bool pid_line, bool pid_angle, bool pid_speed, bool meter )
{
	short s, s2;
	// �t���b�V���ǂݍ��݊J�n
	//printf("�O��p�����[�^�ǂݍ��݊J�n\n");
	
	// �p�����[�^�ǂݍ���
	if ( speed ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( PARAMETER_STARTAREA, 0 ) ) {
			
			// �O��ۑ����̃A�h���X�ǂݍ���
			readBeforeAddr( 0, 10 );
			// flashDataBuff�Ƀp�����[�^�ǂݍ���
			readFlashArray( flashDataBuff, NUMDATA );
			
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
		
		} else if ( checkBlank( PARAMETER_STARTAREA, 0 ) <= 0 ) {
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
			
			//printf("Patameter Initialize\n");
		}
	}
		
	// Angle0�ǂݍ���
	if ( C_angle ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( ANGLE0_STARTAREA, 0 ) ) {
			readBeforeAddr( ANGLE0_STARTAREA, ANGLE0_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( flashDataBuff, 1 );		// flashDataBuff��Angle0�ǂݍ���
			Angle0 = flashDataBuff[ 0 ];			// �f�[�^�擾
		} else if ( checkBlank( ANGLE0_STARTAREA, 0 ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			Angle0 = SERVO_CENTER;
			//printf("Angle0 Initialize\n");
		}
	}
	
	// msdWorkAddress�ǂݍ���
	if ( msd ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( MSD_STARTAREA, 0 ) ) {
			readBeforeAddr( MSD_STARTAREA, MSD_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray2( flashDataBuff, 40, MSD_ENDAREA);// flashDataBuff��msdWorkAddress�ǂݍ��ݓǂݍ���
			// �f�[�^�擾
			// ���߂̃��O�A�h���X10���擾
			s = 0;
			s2 = 0;
			while ( s < 19 ) {
				msdaddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+1 ] * 0x10000 + 
							(unsigned short)flashDataBuff[ s2 ]);
				msdaddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+3 ] * 0x10000 + 
							(unsigned short)flashDataBuff[ s2+2 ]);
				s2 += 4;
			}
			// �ŐV�̃��O�A�h���X
			/*
			printf("msdAddrBuff[0] = %d msdAddrBuff[1] = %d\n", msdAddrBuff[0], msdAddrBuff[1]);
			printf("msdAddrBuff[2] = %d msdAddrBuff[3] = %d\n", msdAddrBuff[2], msdAddrBuff[3]);
			printf("msdAddrBuff[4] = %d msdAddrBuff[5] = %d\n", msdAddrBuff[4], msdAddrBuff[5]);
			printf("msdAddrBuff[6] = %d msdAddrBuff[7] = %d\n", msdAddrBuff[6], msdAddrBuff[7]);
			printf("msdAddrBuff[8] = %d msdAddrBuff[9] = %d\n", msdAddrBuff[8], msdAddrBuff[9]);
			printf("msdAddrBuff[10] = %d msdAddrBuff[11] = %d\n", msdAddrBuff[10], msdAddrBuff[11]);
			printf("msdAddrBuff[12] = %d msdAddrBuff[13] = %d\n", msdAddrBuff[12], msdAddrBuff[13]);
			printf("msdAddrBuff[14] = %d msdAddrBuff[15] = %d\n", msdAddrBuff[14], msdAddrBuff[15]);
			printf("msdAddrBuff[16] = %d msdAddrBuff[17] = %d\n", msdAddrBuff[16], msdAddrBuff[17]);
			printf("msdAddrBuff[18] = %d msdAddrBuff[19] = %d\n", msdAddrBuff[18], msdAddrBuff[19]);
			*/
			
			msdWorkaddress = msdaddrBuff[1];	// �O��J�n�A�h���X
			msdWorkaddress2 = msdaddrBuff[0];	// �O��I���A�h���X
		} else if ( checkBlank( MSD_STARTAREA, 0 ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			msdWorkaddress = MSD_STARTADDRESS;	// �J�n�A�h���X
			msdWorkaddress2 = MSD_ENDADDRESS;	// �I���A�h���X
			//printf("msdWorkAddress Initialize\n");
		}
	}
	
	// �����g���[�X�pPID�Q�C���ǂݍ���
	if ( pid_line ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( PID_STARTAREA, 0 ) ) {
			readBeforeAddr( PID_STARTAREA, PID_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
			// �f�[�^�擾
			kp_buff = flashDataBuff[ 0 ];
			ki_buff = flashDataBuff[ 1 ];
			kd_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( PID_STARTAREA, 0 ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			kp_buff = KP;
			ki_buff = KI;
			kd_buff = KD;
			//printf("PIDgain Initialize\n");
		}
	}
	
	// �p�x����pPID�Q�C���ǂݍ���
	if ( pid_angle ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( PID2_STARTAREA, 0 ) ) {
			readBeforeAddr( PID2_STARTAREA, PID2_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
			kp2_buff = flashDataBuff[ 0 ];
			ki2_buff = flashDataBuff[ 1 ];
			kd2_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( PID2_STARTAREA, 0 ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			kp2_buff = KP2;
			ki2_buff = KI2;
			kd2_buff = KD2;
			//printf("PID2gain Initialize\n");
		}
	}
	
	// ���x����pPID�Q�C���ǂݍ���
	if ( pid_speed ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( PID3_STARTAREA, 0 ) ) {
			readBeforeAddr( PID3_STARTAREA, PID3_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
			// �f�[�^�擾
			printf("kp3 = %d, ki3 = %d kd3 = %d\n", flashDataBuff[ 0 ], flashDataBuff[ 1 ], flashDataBuff[ 2 ]);
			kp3_buff = flashDataBuff[ 0 ];
			ki3_buff = flashDataBuff[ 1 ];
			kd3_buff = flashDataBuff[ 2 ];
		} else if ( checkBlank( PID3_STARTAREA, 0 ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			kp3_buff = KP3;
			ki3_buff = KI3;
			kd3_buff = KD3;
			//printf("PID3gain Initialize\n");
		}
	}
	
	// ��~�����ǂݍ���
	if ( meter ) {
		// �S�u���b�N�C���[�Y����Ă��邩�m�F����
		if ( checkBlank( STOPMETER_STARTAREA, 0 ) ) {
			readBeforeAddr( STOPMETER_STARTAREA, STOPMETER_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
			readFlashArray( flashDataBuff, 1 );				// flashDataBuff�ɒ�~�����ǂݍ���
			stopping_meter = flashDataBuff[ 0 ];				// �f�[�^�擾
		} else if ( checkBlank( STOPMETER_STARTAREA, 0 ) <= 0 ) {
			// �S�u���b�N�C���[�Y�܂��̓G���[�����������珉���l�ɐݒ肷��
			stopping_meter = STOPPING_METER;
			//printf("StopMeter Initialize\n");
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� writeFlashBeforeStart							//
// �����T�v     �w��A�h���X�̒l��ǂ�							//
// ����         Addr: E2�f�[�^�t���b�V���̈�̃A�h���X					//
// �߂�l       Addr�̒l								//
//////////////////////////////////////////////////////////////////////////////////////////
void writeFlashBeforeStart ( void )
{
	// �t���b�V���������݊J�n
	// �e�f�[�^���o�b�t�@�ɕۑ�
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