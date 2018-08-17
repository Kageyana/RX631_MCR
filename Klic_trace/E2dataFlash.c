//======================================//
// �C���N���[�h                         //
//======================================//
#include "E2dataFlash.h"
//======================================//
// �O���[�o���ϐ��̐錾                 //
//======================================//
// �t���b�V���֘A
static volatile short 		BeforeBlockNumber;	// �O��ۑ����̃u���b�N�ԍ�
static volatile short 		BeforeAddrNumber;	// �O��ۑ����̃I�t�Z�b�g�l
static volatile short 		EndBlockNumber;		// ���񏑂����݂̍ŏI�u���b�N�ԍ�
static volatile short 		EndAddrOffset;		// ���񏑂����݂̍ŏI�I�t�Z�b�g�l
short 				flashDataBuff[45];	// �ꎞ�ۑ��o�b�t�@

static volatile short 		NowBlockNumber;		// ���݂̏������܂�Ă���u���b�N�ԍ�
static volatile short 		NowAddrOffset;		// ���݂̏������܂�Ă���I�t�Z�b�g�l
//////////////////////////////////////////////////////////////////////////
// ���W���[���� firmwarecopy						//
// �����T�v     FCU RAM�ւ̃t�@�[���E�F�A�R�s�[				//
// ����         �Ȃ�							//
// �߂�l       �Ȃ�							//
//////////////////////////////////////////////////////////////////////////
void firmwarecopy ( void )
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
// ����         block_number: �u���b�N�ԍ��@command:FCU�R�}���h(16�i��)�o�C�g�T�C�Y 	//
//		addr_number: �擪�A�h���X����̃I�t�Z�b�g�l				//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void fcuCommandByte ( unsigned short block_number, unsigned char command, char addr_number )
{
	volatile unsigned char *e2data;
	volatile unsigned int addr;
	
	addr = ( block_number * 32 ) + 0x00100000 + addr_number;	// �u���b�NN�̃A�h���X���Z�o
	
	e2data = ( unsigned char *)addr;	// �|�C���^�ɃA�h���X���
	*e2data = command;			// �w��A�h���X�ɃR�}���h���s
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� fcuCommandWord							//
// �����T�v     �w��A�h���X��FCU�R�}���h���s						//
// ����         block_number: �u���b�N�ԍ��@command:FCU�R�}���h(16�i��)���[�h�T�C�Y 	//
//		addr_number: �擪�A�h���X����̃I�t�Z�b�g�l				//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void fcuCommandWord ( unsigned short block_number, unsigned short command, char addr_number )
{
	volatile unsigned short *e2data;
	volatile unsigned int addr;
	
	addr = ( block_number * 32 ) + 0x00100000 + addr_number;	// �u���b�NN�̃A�h���X���Z�o
	
	e2data = ( unsigned short *)addr;	// �|�C���^�ɃA�h���X���
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
				printf("FCU������\n");
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
				firmwarecopy();
				printf("FCU����������\n");
				break;
			}
		}
	} else if ( timeout == 0 ){
		//printf("�^�C���A�E�g�Ȃ�\n");
	}
	return timeout;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� checkErrorFlash					//
// �����T�v     �G���[�m�F						//
// ����         �Ȃ�							//
// �߂�l       0: �G���[�Ȃ� 1:�G���[����				//
//////////////////////////////////////////////////////////////////////////
char checkErrorFlash ( void )
{
	volatile char error;
	
	if ( FLASH.FSTATR0.BIT.ILGLERR == 1 || FLASH.FSTATR0.BIT.ERSERR == 1 || FLASH.FSTATR0.BIT.PRGERR == 1 ) {
		printf("�G���[�m�F ILGLERR = %d ERSERR = %d PRGERR = %d\n", FLASH.FSTATR0.BIT.ILGLERR, FLASH.FSTATR0.BIT.ERSERR, FLASH.FSTATR0.BIT.PRGERR);
		error = 1;
		
		if ( FLASH.FSTATR0.BIT.ILGLERR == 1 ) {
			printf("�s���R�}���h���o\n");
			printf("ROMAE = %d DFLAE = %d DFLRPE = %d DFLWPE = %d\n", FLASH.FASTAT.BIT.ROMAE, FLASH.FASTAT.BIT.DFLAE, 
			FLASH.FASTAT.BIT.DFLRPE, FLASH.FASTAT.BIT.DFLWPE);
			
			if ( FLASH.FASTAT.BYTE == 0x10 ) {
				fcuCommandByte( 0, 0x50, 0 );
				printf("�X�e�[�^�X�N���A�R�}���h���s(FCU�R�}���h�͎󂯕t���ĂȂ�)\n");
			} else {
				FLASH.FASTAT.BYTE = 0x10;
				fcuCommandByte( 0, 0x50, 0 );
				printf("�X�e�[�^�X�N���A�R�}���h���s(FCU�R�}���h���󂯕t���Ă�)\n");
			}
		} else {
			fcuCommandByte( 0, 0x50, 0 );
			printf("�X�e�[�^�X�N���A�R�}���h���s(ILGLERR = 0)\n");
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
	firmwarecopy();
	
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
			printf("E2�f�[�^�t���b�V���������G���[\n");
			ret = 1;
		}
	} else {
		printf("E2�f�[�^�t���b�V���������G���[\n");
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
// ����         block_number: �u�����N�`�F�b�N����u���b�N�ԍ�			//
//		addr_offset: �擪�A�h���X����̃I�t�Z�b�g�l			//
// �߂�l       0:�C���[�Y�ς݁@1:�������ݍς� 2:�G���[����			//
//////////////////////////////////////////////////////////////////////////////////
char checkBlank ( short block_number, char addr_offset )
{
	volatile char ret;
	volatile unsigned int addr, lead_addr, offset;

	// P/E���[�h�ڍs
	changeFlashPE();
	
	addr = ( block_number * 32 ) + 0x00100000 + addr_offset;	// �u�����N�`�F�b�N����A�h���X���Z�o
	// �e�u���b�N�̐擪�A�h���X�Z�o
	if ( block_number >= 0 && block_number <= 63 ) lead_addr = ( 0 * 32 ) + 0x00100000;
	if ( block_number >= 64 && block_number <= 127 ) lead_addr = ( 64 * 32 ) + 0x00100000;
	if ( block_number >= 128 && block_number <= 191 ) lead_addr = ( 128 * 32 ) + 0x00100000;
	if ( block_number >= 192 && block_number <= 255 ) lead_addr = ( 192 * 32 ) + 0x00100000;
	if ( block_number >= 256 && block_number <= 319 ) lead_addr = ( 256 * 32 ) + 0x00100000;
	if ( block_number >= 320 && block_number <= 383 ) lead_addr = ( 320 * 32 ) + 0x00100000;
	if ( block_number >= 384 && block_number <= 447 ) lead_addr = ( 384 * 32 ) + 0x00100000;
	if ( block_number >= 448 && block_number <= 511 ) lead_addr = ( 448 * 32 ) + 0x00100000;
	if ( block_number >= 512 && block_number <= 575 ) lead_addr = ( 512 * 32 ) + 0x00100000;
	if ( block_number >= 576 && block_number <= 639 ) lead_addr = ( 576 * 32 ) + 0x00100000;
	if ( block_number >= 640 && block_number <= 703 ) lead_addr = ( 640 * 32 ) + 0x00100000;
	if ( block_number >= 704 && block_number <= 767 ) lead_addr = ( 704 * 32 ) + 0x00100000;
	if ( block_number >= 768 && block_number <= 831 ) lead_addr = ( 768 * 32 ) + 0x00100000;
	if ( block_number >= 832 && block_number <= 895 ) lead_addr = ( 832 * 32 ) + 0x00100000;
	if ( block_number >= 896 && block_number <= 959 ) lead_addr = ( 896 * 32 ) + 0x00100000;
	if ( block_number >= 960 && block_number <= 1023 ) lead_addr = ( 960 * 32 ) + 0x00100000;
	// �I�t�Z�b�g�ʎZ�o
	offset = addr - lead_addr;
	
	// �u�����N�`�F�b�N����
	FLASH.FMODR.BIT.FRDMD = 1;
	
	// �`�F�b�N�T�C�Y�ݒ�( 2B )
	FLASH.DFLBCCNT.BIT.BCSIZE = 0;
	FLASH.DFLBCCNT.BIT.BCADR = offset;
	
	// �u�����N�`�F�b�N�R�}���h���s
	fcuCommandByte( block_number, 0x71, 0 );
	fcuCommandByte( block_number, 0xd0, addr_offset );
	
	// P/E���������m�F
	if ( checkFRDY( 6 * 1.1 ) == 0 ) {
		// �G���[�m�F
		if ( checkErrorFlash() == 0 ) {
			// �u�����N�`�F�b�N���ʎ擾
			if ( FLASH.DFLBCSTAT.BIT.BCST == 1 ) {
				//printf("�A�h���X%p�͏������ݍς�\n", addr);
				ret = 1;
			} else {
				//printf("�A�h���X%p�̓C���[�Y�ς�\n", addr);
				ret = 0;
			}
		} else {
			ret = 2;
		}
	} else {
		ret = 2;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////
// ���W���[���� eraseE2DataFlash						//
// �����T�v     �w��f�[�^�u���b�N���C���[�Y					//
// ����         block_number: �C���[�Y����u���b�N�ԍ��@			//
// �߂�l      	0:�C���[�Y���� 1:�C���[�Y�G���[					//
//////////////////////////////////////////////////////////////////////////////////
char eraseE2DataFlash ( unsigned short block_number )
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
	fcuCommandByte( block_number, 0xd0, 0 );
	
	if ( ( FLASH.DFLWE0.WORD & 0x00ff ) == 0x00ff ) {
		// P/E���������m�F
		if ( checkFRDY( 20 * 63 * 1.1 ) == 0 ) {
			// �G���[�m�F
			if ( checkErrorFlash() == 0 ) {
				FLASH.FWEPROR.BIT.FLWE = 2;	// �����v���e�N�g
				FLASH.DFLWE0.WORD = 0x1e00;	// 0000�`0511�u���b�N�܂�P/E����
				FLASH.DFLWE1.WORD = 0xe100;	// 0512�`1023�u���b�N�܂�P/E����
				//printf("Block%dErase\n", block_number);
			}
		}
	} else {
		printf("Block%dUnErase\n", block_number);
		ret = 1;
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� checkWriteAddr										//
// �����T�v     startblock����endbloock�܂ł̃C���[�Y�ςݗ̈��T��					//
// ����         startblock: �J�n�u���b�N�ԍ� 	endbloock: �I���u���b�N�ԍ�				//
//		startoffset: �J�n�I�t�Z�b�g�l	width_data: �m�ۂ���f�[�^��				//
// �߂�l       0: �C���[�Y�ςݗ̈悠�� 1:�C���[�Y�ςݗ̈�Ȃ�						//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkWriteAddr ( short startblock, short endblock, char startoffset, short width_data )
{
	volatile char 	addr_offset, lead_addr_offset;
	volatile short	block_number, lead_block_number, i, width = 0;
	
	block_number = startblock;
	addr_offset = startoffset;

	// �w�肵���f�[�^�ʂ̗̈悪������܂Ńu�����N�`�F�b�N����
	while ( block_number <= endblock ) {
		if ( checkBlank( block_number, addr_offset ) == 1 ) {
			width = 0;
		} else if ( checkBlank( block_number, addr_offset ) == 0 ) {
			width++;		
			if ( width == 1 ) {
				// �������݊J�n�A�h���X
				lead_block_number = block_number;
				lead_addr_offset = addr_offset;
			}
			// �K�v�ȃf�[�^�ʊm�ۂł�����X�g�b�v
			if ( width >= width_data ) break;
		} else {
			printf("BlankCheakError\n");
			break;
		}
		
		addr_offset += 2;
		if ( addr_offset == 32 ) {
			// ���̃u���b�N�Ɉړ�����
			addr_offset = 0;
			block_number++;
		}
	}
	// �������ݗ̈悪�Ȃ���΃C���[�Y����
	if ( block_number > endblock || width < width_data ) {
		//printf("�������ݗ̈�Ȃ�\n");
		i = startblock;
		while ( i <= endblock ) {
			eraseE2DataFlash(i);
			i++;
		}
		
		// �J�n�ʒu�ɖ߂�
		lead_block_number = startblock;
		lead_addr_offset = 0;
	}
	
	NowBlockNumber = lead_block_number;
	NowAddrOffset = lead_addr_offset;
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
	volatile unsigned int addr, i = 1;
	
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
					addr = ( NowBlockNumber * 32 ) + 0x00100000 + NowAddrOffset;	// �u���b�NN�̃A�h���X���Z�o
					//printf("Addr%p(%d�u���b�N %d�I�t�Z�b�g) %dWritten\n", addr, NowBlockNumber, NowAddrOffset, *sendData);
					
					NowAddrOffset += 2;
					if ( NowAddrOffset == 32 ) {
						NowAddrOffset = 0;
						NowBlockNumber++;
					}
					*sendData++;
					i++;
				} else {
					ret = 1;
					printf("Write Error\n");
					break;
				}
			} else {
				ret = 1;
				printf("Write TimeOut\n");
				break;
			}
		} else {
			ret = 1;
			printf("BlockNumber Error\n");
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
// ����         block_number:�u���b�N�ԍ� addr_number: �擪�A�h���X����̃I�t�Z�b�g�l	//
// �߂�l       �ǂݍ��񂾒l								//
//////////////////////////////////////////////////////////////////////////////////////////
short readFlashBlock ( short block_number, char addr_offset )
{
	volatile short ret;
	volatile unsigned int addr;
	
	// �t���b�V�����[�h���[�h�ڍs
	changeFlashRead();
	// E2�f�[�^�t���b�V���̈�̓ǂݏo��������
	FLASH.DFLRE0.WORD = 0x2dff;
	FLASH.DFLRE1.WORD = 0xd2ff;
	addr = ( block_number * 32 ) + 0x00100000 + addr_offset;	// �u���b�NN�̃A�h���X���Z�o
	
	ret = *(volatile unsigned short *)addr;
	
	// E2�f�[�^�t���b�V���̈�̓ǂݏo���֎~
	FLASH.DFLRE0.WORD = 0x2d00;
	FLASH.DFLRE1.WORD = 0xd200;
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readFlashAddr								//
// �����T�v     �w��A�h���X�̒l��ǂ�							//
// ����         addr: E2�f�[�^�t���b�V���̈�̃A�h���X					//
// �߂�l       addr�̒l								//
//////////////////////////////////////////////////////////////////////////////////////////
short readFlashAddr ( unsigned int addr )
{
	volatile short ret;
	
	// �t���b�V�����[�h���[�h�ڍs
	changeFlashRead();
	// E2�f�[�^�t���b�V���̈�̓ǂݏo��������
	FLASH.DFLRE0.WORD = 0x2dff;
	FLASH.DFLRE1.WORD = 0xd2ff;
	
	ret = *(volatile unsigned short *)addr;
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
// ����         startblock: �J�n�u���b�N�ԍ� 	endbloock: �I���u���b�N�ԍ�		//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void readBeforeAddr ( short startBlockNumber, short endBlockNumber )
{
	volatile short blocknumber;	// �O��ۑ����̃u���b�N�ԍ��̏����Ă���u���b�N�ԍ�
	volatile short addroffset = 0;
	
	// startBlockNumber�`endBlockNumber�ɏ������O�񏑂����ݎ��̍ŏI�A�h���X���擾����
	blocknumber = startBlockNumber;
	if ( checkBlank( blocknumber, addroffset ) == 0 ) {
		// �C���[�Y�ς݂Ȃ珉���l�ɃZ�b�g����
		BeforeBlockNumber = endBlockNumber + 1;
		BeforeAddrNumber = 0;
	} else {
		while ( checkBlank( blocknumber, addroffset ) == 1 ) {
			addroffset += 2;
			if ( addroffset == 32 ) {
				// ���̃u���b�N�Ɉړ�����
				addroffset = 0;
				blocknumber++;
				if ( blocknumber == endBlockNumber + 1 ) break;
			}
		}
		
		// �u���b�N���܂�������1�O�̃u���b�N���w�肷��
		if ( addroffset == 0 ) {
			blocknumber--;
			addroffset = 32;
		}
		// �ŏI�u���b�N�ԍ��A�ŏI�I�t�Z�b�g�A�h���X�擾
		BeforeBlockNumber = readFlashBlock( blocknumber, ( addroffset - 4 ) );
		BeforeAddrNumber = readFlashBlock( blocknumber, ( addroffset - 2 ) );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� writeFlashData								//
// �����T�v     �w��u���b�N�Ԃ�flashDataBuff���������݃A�h���X���L�^����		//
// ����          startblock: �J�n�u���b�N�ԍ� 	endbloock: �I���u���b�N�ԍ�		//
//		endData: �f�[�^�ۑ��Ō�u���b�N	width_data: �m�ۂ���f�[�^��		//
// �߂�l       �Ȃ�									//
//////////////////////////////////////////////////////////////////////////////////////////
void writeFlashData ( short startBlockNumber, short endBlockNumber, short endData, short width_data )
{
	// �O��ۑ����̃A�h���X�ǂݍ���
	readBeforeAddr( startBlockNumber, endBlockNumber );
	// �������ݗ̈�m��
	checkWriteAddr ( BeforeBlockNumber, endData, BeforeAddrNumber, width_data );
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
	checkWriteAddr ( startBlockNumber, endBlockNumber, 0, 2 );
	// �������݊J�n
	writeFlash ( flashDataBuff, 2 );
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� readFlashSetup								//
// �����T�v     �w��A�h���X�̒l��ǂ�							//
// ����         addr: E2�f�[�^�t���b�V���̈�̃A�h���X					//
// �߂�l       addr�̒l								//
//////////////////////////////////////////////////////////////////////////////////////////
void readFlashSetup ( void )
{
	short s, s2;
	// �t���b�V���ǂݍ��݊J�n
	//printf("�O��p�����[�^�ǂݍ��݊J�n\n");
	
	// �p�����[�^�ǂݍ���
	// �S�u���b�N�C���[�Y����Ă��邩�m�F����
	if ( checkBlank( 0, 0 ) == 1 ) {
		
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
	
	} else if ( checkBlank( 0, 0 ) == 0 ) {
		// �S�u���b�N�C���[�Y����Ă����珉���l�ɐݒ肷��
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
		
		printf("Patameter Initialize\n");
	}
	
	// Angle0�ǂݍ���
	// �S�u���b�N�C���[�Y����Ă��邩�m�F����
	if ( checkBlank( ANGLE0_AREA, 0 ) == 1 ) {
		readBeforeAddr( ANGLE0_AREA, ANGLE0_AREA );	// �O��ۑ����̃A�h���X�ǂݍ���
		readFlashArray( flashDataBuff, 1 );		// flashDataBuff��Angle0�ǂݍ���
		Angle0 = flashDataBuff[ 0 ];			// �f�[�^�擾
	} else if ( checkBlank( ANGLE0_AREA, 0 ) == 0 ) {
		// �S�u���b�N�C���[�Y����Ă����珉���l�ɐݒ肷��
		Angle0 = SERVO_CENTER;
		printf("Angle0 Initialize\n");
	}
	
	// msdWorkAddress�ǂݍ���
	// �S�u���b�N�C���[�Y����Ă��邩�m�F����
	if ( checkBlank( MSD_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( MSD_STARTAREA, MSD_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
		readFlashArray2( flashDataBuff, 40, MSD_ENDAREA);// flashDataBuff��msdWorkAddress�ǂݍ��ݓǂݍ���
		// �f�[�^�擾
		// ���߂̃��O�A�h���X10���擾
		s = 0;
		s2 = 0;
		while ( s < 19 ) {
			msdAddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+1 ] * 0x10000 + 
						(unsigned short)flashDataBuff[ s2 ]);
			msdAddrBuff[s++] = (unsigned int)( (unsigned short)flashDataBuff[ s2+3 ] * 0x10000 + 
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
		
		msdWorkAddress = msdAddrBuff[1];	// �O��J�n�A�h���X
		msdWorkAddress2 = msdAddrBuff[0];	// �O��I���A�h���X
	} else if ( checkBlank( MSD_STARTAREA, 0 ) == 0 ) {
		// �S�u���b�N�C���[�Y����Ă����珉���l�ɐݒ肷��
		msdWorkAddress = MSD_STARTADDRESS;	// �J�n�A�h���X
		msdWorkAddress2 = MSD_ENDADDRESS;	// �I���A�h���X
		printf("msdWorkAddress Initialize\n");
	}
	
	// �����g���[�X�pPID�Q�C���ǂݍ���
	// �S�u���b�N�C���[�Y����Ă��邩�m�F����
	if ( checkBlank( PID_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( PID_STARTAREA, PID_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
		readFlashArray( flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
		// �f�[�^�擾
		kp_buff = flashDataBuff[ 0 ];
		ki_buff = flashDataBuff[ 1 ];
		kd_buff = flashDataBuff[ 2 ];
	} else if ( checkBlank( PID_STARTAREA, 0 ) == 0 ) {
		// �S�u���b�N�C���[�Y����Ă����珉���l�ɐݒ肷��
		kp_buff = KP;
		ki_buff = KI;
		kd_buff = KD;
		printf("PIDgain Initialize\n");
	}
	
	// �p�x����pPID�Q�C���ǂݍ���
	// �S�u���b�N�C���[�Y����Ă��邩�m�F����
	if ( checkBlank( PID2_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( PID2_STARTAREA, PID2_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
		readFlashArray( flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
		// �f�[�^�擾
		kp2_buff = flashDataBuff[ 0 ];
		ki2_buff = flashDataBuff[ 1 ];
		kd2_buff = flashDataBuff[ 2 ];
	} else if ( checkBlank( PID2_STARTAREA, 0 ) == 0 ) {
		// �S�u���b�N�C���[�Y����Ă����珉���l�ɐݒ肷��
		kp2_buff = KP2;
		ki2_buff = KI2;
		kd2_buff = KD2;
		printf("PID2gain Initialize\n");
	}
	
	// ���x����pPID�Q�C���ǂݍ���
	// �S�u���b�N�C���[�Y����Ă��邩�m�F����
	if ( checkBlank( PID3_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( PID3_STARTAREA, PID3_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
		readFlashArray( flashDataBuff, 3 );		// flashDataBuff��PID�Q�C���ǂݍ���
		// �f�[�^�擾
		kp3_buff = flashDataBuff[ 0 ];
		ki3_buff = flashDataBuff[ 1 ];
		kd3_buff = flashDataBuff[ 2 ];
	} else if ( checkBlank( PID3_STARTAREA, 0 ) == 0 ) {
		// �S�u���b�N�C���[�Y����Ă����珉���l�ɐݒ肷��
		kp3_buff = KP3;
		ki3_buff = KI3;
		kd3_buff = KD3;
		printf("PID3gain Initialize\n");
	}
	
	// ��~�����ǂݍ���
	// �S�u���b�N�C���[�Y����Ă��邩�m�F����
	if ( checkBlank( STOPMETER_STARTAREA, 0 ) == 1 ) {
		readBeforeAddr( STOPMETER_STARTAREA, STOPMETER_ENDAREA );	// �O��ۑ����̃A�h���X�ǂݍ���
		readFlashArray( flashDataBuff, 1 );				// flashDataBuff�ɒ�~�����ǂݍ���
		stopping_meter = flashDataBuff[ 0 ];				// �f�[�^�擾
	} else if ( checkBlank( STOPMETER_STARTAREA, 0 ) == 0 ) {
		// �S�u���b�N�C���[�Y����Ă����珉���l�ɐݒ肷��
		stopping_meter = STOPPING_METER;
		printf("StopMeter Initialize\n");
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� writeFlashBeforeStart							//
// �����T�v     �w��A�h���X�̒l��ǂ�							//
// ����         addr: E2�f�[�^�t���b�V���̈�̃A�h���X					//
// �߂�l       addr�̒l								//
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
	
	writeFlashData( 0, 10, PARAMETER_AREA, NUMDATA );
}