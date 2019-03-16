//====================================//
// �C���N���[�h									//
//====================================//
#include "MicroSD.h"
//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//
// �^�C�}�֘A
unsigned short			cnt_log;				// ���O�R��m�F�p�J�E���g

static unsigned char		msdlibBuff[512 + 128];	// �ꎞ�ۊǃo�b�t�@
static volatile short		msdlibMode;			// ���
volatile short			msdlibCnt;				// �������ݐ�
static volatile unsigned char*	msdlibWrite;		// �������݃f�[�^�̃A�h���X
volatile char			msdlibError;			// �G���[�ԍ�
static volatile unsigned int	msdSize;				// microSD�̃T�C�Y(kB�P��)
static volatile short		modeSector;			// 1:�Z�N�^�w�胂�[�h 0:�A�h���X
volatile unsigned char		interrupt_msd_send_data = 0;	// ���M�t���O

// microSD�֘A
signed char		msdBuff[ 512 ];		// �ꎞ�ۑ��o�b�t�@
short				msdBuffaddress;	// �ꎞ�L�^�o�b�t�@�����A�h���X
short				msdFlag = 0;		// 1:�f�[�^�L�^ 0:�L�^���Ȃ�
short				msdTimer;			// �擾�Ԋu�v�Z�p
unsigned int		msdStartaddress;	// �L�^�J�n�A�h���X
unsigned int		msdEndaddress;	// �L�^�I���A�h���X
unsigned int		msdWorkaddress;	// ��Ɨp�A�h���X
unsigned int		msdWorkaddress2;	// ��Ɨp�A�h���X2
signed char 		*msdBuffPointa;		// RAM�ۑ��o�b�t�@�p�|�C���^
unsigned int 		msdaddrBuff[25];	// MicroSD�J�[�h�̍ŏI�������݃A�h���X�ۑ��p
                                        
///////////////////////////////////////////////////////////////////////////
// ���W���[���� msd_write								//
// �����T�v     microSD 1�o�C�g��������						//
// ����         char �f�[�^								//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void msd_write( unsigned char data )
{
	uint8_t data_tr[] = { data }, data_re[ 1 ];
	
	SPI_SEND
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� msd_read								//
// �����T�v     microSD 1�o�C�g�ǂݍ���						//
// ����         �Ȃ�									//
// �߂�l       char �f�[�^								//
///////////////////////////////////////////////////////////////////////////
unsigned char msd_read( void )
{
	uint8_t data_tr[] = { 0xff }, data_re[ 1 ] = { 0x00 }, ret;
	volatile short data;
	
	SPI_SEND
	
	data = data_re[0];
	ret = data & 0x00ff;
	
	return  ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� msd_CMD								//
// �����T�v     �R�}���h���M								//
// ����         �R�}���h�A����1,����2,����3,����4,CRC			//
// �߂�l       microSD����̖߂�l						//
///////////////////////////////////////////////////////////////////////////
unsigned char msd_CMD ( unsigned char cmd, unsigned char arg1, unsigned char arg2,
			unsigned char arg3, unsigned char arg4, unsigned char crc )
{
	volatile unsigned char ret;
	volatile unsigned short cnt_retry;
	
	msd_write ( 0xff );
	
	msd_write ( cmd );
	msd_write ( arg1 );
	msd_write ( arg2 );
	msd_write ( arg3 );
	msd_write ( arg4 );
	msd_write ( crc );
	
	// ���X�|���X���Ԃ��Ă���܂ł܂�
	cnt_retry = 1500;
	while ( --cnt_retry ) {
		MSD_CS_TERMINAL_HIGH		// CS�[�q��HIGH�ɂ���
		MSD_CS_TERMINAL_LOW		// CS�[�q��LOW�ɂ���
		
		ret = msd_read();
		if ( ret != 0xff ) break;	// 0xff�ȊO�ŏI��
	}
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� init_msd								//
// �����T�v     MicroSD�̏�����							//
// ����         �Ȃ�									//
// �߂�l       0:����������	1�ȏ�:���������s					//
///////////////////////////////////////////////////////////////////////////
char init_msd ( void )
{
	uint8_t sd_sw;
	volatile unsigned char pattern_intimsd = 1, ret = 0, response_register[ 16 ];
	volatile short receive, i;
	
	while ( ret == 0 && pattern_intimsd <= 8 ) {
		GET_SDSWITCH;
		if ( sd_sw == 0 ) {
			switch ( pattern_intimsd ) {
				case 1:
					MSD_CS_TERMINAL_HIGH			// CS�[�q��HIGH�ɂ���
					
					for ( i = 0; i < 10; i++) {
						receive = msd_read();		// 80�N���b�N(�_�~�[�f�[�^10�o�C�g)�𑗂�
					}
					
					printf("Dumydata send\n");
					pattern_intimsd = 2;
					break;
					
				case 2:
					MSD_CS_TERMINAL_LOW			// CS�[�q��LOW�ɂ���
					
					i = 0;
					do {
						// CMD0���M
						receive = msd_CMD( 0x40, 0x00, 0x00, 0x00, 0x00, 0x95 );
						
						if ( i >= 10 ) {
							ret = 3;
							printf("CMD0 error\n");
							break;
						}
						i++;
					} while ( ( receive & 0xfe ) != 0x0 );	// 0x01����M�����SD�J�[�h�̓A�C�h�����
					
					pattern_intimsd = 3;
					break;
					
				case 3:
					printf("CMD0 send\n");
					i = 0;
					do {
						// CMD8���M
						receive = msd_CMD( 0x48, 0x00, 0x00, 0x01, 0xaa, 0x87 );
						
						i++;
						if ( i >= 20 ) {
							ret = 4;
							printf("CMD8 error\n");
							break;
						}
					} while ( receive == 0xff );
						
					// R7���X�|���X�c����擾
					for ( i = 0; i < 5; i++ ) {
						response_register[ i ] = msd_read();
					}
					
					if ( response_register[ 2 ] != 0x01 || response_register[ 3 ] != 0xaa ) {
						ret = 4;
						printf("CMD8 error\n");
						break;
					}
					
					pattern_intimsd = 4;
					break;
						
				case 4:
					printf("CMD8 send\n");
					i = 0;
					do {
						// CMD58���M
						receive = msd_CMD( 0x7a, 0x40, 0x00, 0x00, 0x00, 0xaf );
						
						i++;
						if ( i > 50 ) {
							ret = 5;
							printf("CMD58 error\n");
							break;
						}
					} while ( receive != 0x01 );
						
					// R3���X�|���X�c����擾
					for ( i = 0; i < 6; i++ ) {
						response_register[ i ] = msd_read();
					}
					
					pattern_intimsd = 5;
					break;
						
				case 5:
					printf("CMD58 send\n");
					i = 0;
					do {
						// CMD55���M
						receive = msd_CMD( 0x77, 0x00, 0x00, 0x00, 0x00, 0x65 );
						// ACMD41���M
						receive = msd_CMD( 0x69, 0x40, 0x00, 0x00, 0x00, 0x77 );
						
						i++;
						if ( i > 1000 ) {
							ret = 6;
							printf("ACMD41 error\n");
							break;
						}
					} while ( receive != 0x00 );
					
					pattern_intimsd = 6;
					break;
						
				case 6:
					printf("ACMD41 send\n");
					i = 0;
					do {
						// CMD58���M
						receive = msd_CMD( 0x7a, 0x40, 0x00, 0x00, 0x00, 0xaf );
						
						if ( i >= 10 ) {
							ret = 7;
							printf("CMD58 error\n");
							break;
						}
						i++;
					} while ( receive != 0x00 );
						
					// R3���X�|���X�c����擾
					for ( i = 0; i < 6; i++ ) {
						response_register[ i ] = msd_read();
					}
						
					if ( response_register[ 0 ] & 0x40 ) {
						printf("SDHCcard\n");
					} else {
						printf("SDcard\n");
					}
					pattern_intimsd = 7;
					break;
						
				case 7:
					printf("CMD58 send\n");
					// CSD�擾
					receive = getMicroSD_CSD( response_register );
					if ( receive == 1 ) {
						ret = 8;
						printf("CSD error\n");
						break;
					}
					// �e�ʌv�Z
					// SDHC�̏ꍇ
					msdSize   = ( ( unsigned short )response_register[7]&0x3f) << 16;
					msdSize  |=  ( unsigned short )response_register[8] << 8;
					msdSize  |=  ( unsigned short )response_register[9];
					msdSize  += 1;
					msdSize <<= 9;
					printf("%dkB\n", msdSize);
					pattern_intimsd = 8;
					break;
						
				case 8:
					MSD_CS_TERMINAL_HIGH		// CS�[�q��HIGH�ɂ���
					pattern_intimsd = 9;
					break;
			}
		} else {
			ret = 1;
			break;
		}
	}
	
	if ( ret == 0 ) {
		printf("SDcard Initialize\n");
	} else if ( ret == 1 ) {
		printf("No slot card\n");
	}
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� getMicroSD_CSD							//
// �����T�v     microSD Card Specific Data�擾				//
// ����         signed char *�ǂݍ��ݔz��(16�o�C�g�ȏ�)			//
// �߂�l       0:���� 1�ȏ�:�G���[							//
///////////////////////////////////////////////////////////////////////////
char getMicroSD_CSD( volatile unsigned char *p )
{
	volatile char ret = 0;
	volatile short receive, i;
	
	ret = 0;
	MSD_CS_TERMINAL_LOW			// CS�[�q��LOW�ɂ���
	
	// CMD9���M
	receive = msd_CMD( 0x49, 0x00, 0x00, 0x00, 0x00, 0xaf );
	
	i = 0;
	do {
		receive = msd_read();
		
		if ( i > 1000 ) {
			ret = 1;
			break;
		}
		i++;
	} while ( receive & 0x80 );
	
	*p++ = receive;
	
	// CSD�ǂݍ���
	for ( i = 1; i < 16; i++ ) {
		*p++ = msd_read();
	}
	
	// �_�~�[���[�h
	msd_read();
	msd_read();
	
	// �_�~�[�N���b�N���M
	msd_write( 0xff );
	MSD_CS_TERMINAL_HIGH			// CS�[�q��HIGH�ɂ���
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� readMicroSD							//
// �����T�v     microSD����f�[�^�ǂݍ���(512�o�C�g)			//
// ����         unsigned int	�A�h���X						//
//		signed char	*�ǂݍ��ޔz��̃A�h���X			//
// �߂�l       0:���� 1�ȏ�:�G���[							//
///////////////////////////////////////////////////////////////////////////
char readMicroSD ( unsigned int address, signed char *read )
{
	volatile short            i, receive;
	volatile unsigned char  a1, a2, a3, a4, pattern_msd_read = 1, ret = 0;
	
	if( msdlibMode != 0 ) {             // �ʂȍ�ƒ��Ȃ�G���[
		return 1;
	}
	msdlibMode = 1;
	
	// SDHC�Ȃ�A�h���X���Z�N�^�ԍ��ɕϊ�
	address >>= 9;
	
	a1 = ( unsigned char )( ( address&0xff000000 ) >> 24 );
	a2 = ( unsigned char )( ( address&0x00ff0000 ) >> 16 );
	a3 = ( unsigned char )( ( address&0x0000ff00 ) >>  8 );
	a4 = ( unsigned char )(  address&0x000000ff       );
	
	while ( ret < 1 && pattern_msd_read <= 3 ) {
		switch ( pattern_msd_read ) {
			case 1:
				MSD_CS_TERMINAL_HIGH		// CS�[�q��HIGH�ɂ���
				msd_write( 0xff );		// �_�~�[�N���b�N���M
				MSD_CS_TERMINAL_LOW		// CS�[�q��LOW�ɂ���
				msd_write( 0xff );		// �_�~�[�N���b�N���M
				pattern_msd_read = 2;
				break;
			
			case 2:
				// CMD17���M
				receive = msd_CMD( 0x51, a1, a2, a3, a4, 0xff );
				
				// ���X�|���X�m�F
				if ( receive != 0x00 ) {
					ret = 1;
					break;
				}
				// 0xfe���m�F
				i = 0;
				do {
					receive = msd_read();
					i++;
					if ( i >= 1000 ) {
						ret = 2;
						break;
					}
				} while( receive != 0xfe );
				
				pattern_msd_read = 3;
				break;
				
			case 3:
				// �f�[�^�ǂݍ���
				for ( i = 0; i < 512; i++ ) {
					*read++ = msd_read();
				}
				
				// CRC 2�o�C�g�ǂݍ���
				msd_read();
				msd_read();
				
				pattern_msd_read = 4;
				break;
		}
	}
	
	msd_write( 0xff );			// �_�~�[�N���b�N���M
	MSD_CS_TERMINAL_HIGH			// CS�[�q��HIGH�ɂ���
	msdlibMode = ( ret != 0 ) ? 99 : 0;	// �������G���[�Ȃ�99���Z�b�g
	
	if ( ret == 0 ) {
		//printf("�ǂݍ��݊���\n");
	} else if ( ret == 1 ) {
		printf("CMD17 error\n");
	} else if ( ret == 2 ) {
		printf("response error\n");
	}
				
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� writeMicroSD							//
// �����T�v     microSD��������(512�o�C�g)					//
// ����         unsigned int	�A�h���X						//
//		signed char	*�������ޔz��̃A�h���X			//
// �߂�l       0:���� 1�ȏ�:�G���[							//
///////////////////////////////////////////////////////////////////////////
char writeMicroSD ( unsigned int address, signed char *write )
{
	volatile short            i, receive;
	volatile unsigned char  a1, a2, a3, a4, pattern_msd_write = 1, ret = 0;
	
	if( msdlibMode != 0 ) {             // �ʂȍ�ƒ��Ȃ�G���[
		return 1;
	}
	msdlibMode = 1;
	
	while ( ret < 1 && pattern_msd_write <= 4 ) {
		switch ( pattern_msd_write ) {
			case 1:
				// SDHC�Ȃ�A�h���X���Z�N�^�ԍ��ɕϊ�
				address >>= 9;
				
				MSD_CS_TERMINAL_LOW		// CS�[�q��LOW�ɂ���
				
				pattern_msd_write = 2;
				break;
			
			case 2:
				// CMD24���M
				a1 = ( unsigned char )( ( address&0xff000000 ) >> 24 );
				a2 = ( unsigned char )( ( address&0x00ff0000 ) >> 16 );
				a3 = ( unsigned char )( ( address&0x0000ff00 ) >>  8 );
				a4 = ( unsigned char )(  address&0x000000ff       );
				
				receive = msd_CMD( 0x58, a1, a2, a3, a4, 0x00 );
				
				// ���X�|���X�m�F
				if ( ( receive & 0x1f ) != 0x00 ) {
					ret = 1;
					break;
				}
				// 1�o�C�g���Ԃ��󂯂�
				receive = msd_read();
				
				pattern_msd_write = 3;
				break;
				
			case 3:
				// �J�n���}
				msd_write( 0xfe );
				// �f�[�^���M
				/*for ( i=0; i<512; i++ ) {
					msd_write( *write++ );
				}*/
				i = 0;
				while ( i < 512 ) {
					msd_write( *write++ );
					i++;
				}
				// CRC���M
				msd_write( 0xff );
				msd_write( 0xff );
				
				pattern_msd_write = 4;
				break;
				
			case 4:
				// ���X�|���X�ǂݍ���
				receive = msd_read();
				receive &= 0x1f;
				
				if ( receive == 0x05 ) {
					ret = 0;        // �������ݐ���I��
				} else if ( receive == 0x0d ) {
					ret = 2;       // �������݃G���[
				} else {
					ret = 3;       // ����ȊO�̃G���[
				}
				// busy��Ԃ��I���܂ő҂�
				/*for ( i=0; i<10000; i++ ) {
					if( msd_read() != 0x00 ) break;
				}*/
				i = 0;
				while ( i < 10000 ) {
					if( msd_read() != 0x00 ) break;
					i++;
				}
				pattern_msd_write = 5;
				break;
		}
	}
	
	// �_�~�[�N���b�N���M
	msd_write( 0xff );
	
	MSD_CS_TERMINAL_HIGH			// CS�[�q��HIGH�ɂ���
	msdlibMode = (ret != 0) ? 99 : 0;	// �������G���[�Ȃ�99���Z�b�g
	
	if ( ret == 0 ) {
		//printf("�������ݐ���I��\n");
	} else if ( ret == 1 ) {
		printf("CMD24 error\n");
	} else if ( ret == 2 ) {
		printf("Write error\n");
	} else if ( ret == 3 ) {
		printf("Other error\n");
	}
				
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� eraseMicroSD							//
// �����T�v     microSD�̃f�[�^�C���[�X(0x00��������)			//
// ����         unsigned int	�J�n�A�h���X , �I���A�h���X			//
// �߂�l       0:���� 1�ȏ�:�G���[							//
///////////////////////////////////////////////////////////////////////////
char eraseMicroSD( unsigned int st_address, unsigned int ed_address )
{
	volatile short		i, j, receive;
	volatile unsigned char	a1, a2, a3, a4, pattern_msd_erase = 1, ret = 0;
	
	if( msdlibMode != 0 ) {             // �ʂȍ�ƒ��Ȃ�G���[
		return 1;
	}
	msdlibMode = 1;
	
	while ( ret < 1 && pattern_msd_erase <= 4 ) {
		switch ( pattern_msd_erase ) {
			case 1:
				// SDHC�Ȃ�A�h���X���Z�N�^�ԍ��ɕϊ�
				st_address >>= 9;
				ed_address >>= 9;
				
				MSD_CS_TERMINAL_LOW	// CS�[�q��LOW�ɂ���
				
				pattern_msd_erase = 2;
				break;
			
			case 2:
				// CMD32���M(�C���[�X�J�n�A�h���X�̃Z�b�g)
				a1 = ( unsigned char )( ( st_address&0xff000000 ) >> 24 );
				a2 = ( unsigned char )( ( st_address&0x00ff0000 ) >> 16 );
				a3 = ( unsigned char )( ( st_address&0x0000ff00 ) >>  8 );
				a4 = ( unsigned char )(  st_address&0x000000ff       );
				
				receive = msd_CMD( 0x60, a1, a2, a3, a4, 0xff );
				
				// ���X�|���X�m�F
				if ( receive != 0x00 ) {
					ret = 1;
					break;
				}
				
				pattern_msd_erase = 3;
				break;
				
			case 3:
				// �E�F�C�g
				for( j=0; j<10000; j++ );
				
				// CMD33���M(�C���[�X�I���A�h���X�̃Z�b�g)
				a1 = (unsigned char)( (ed_address&0xff000000) >> 24);
				a2 = (unsigned char)( (ed_address&0x00ff0000) >> 16);
				a3 = (unsigned char)( (ed_address&0x0000ff00) >>  8);
				a4 = (unsigned char)(  ed_address&0x000000ff       );
				
				receive = msd_CMD( 0x61, a1, a2, a3, a4, 0xff );
				
				// ���X�|���X�m�F
				if( receive != 0x00 ) {
					ret = 2;
					break;
				}
				
				pattern_msd_erase = 4;
				break;
				
			case 4:
				// �E�F�C�g
				for( j=0; j<10000; j++ );
				
				// CMD38���M(�C���[�X)
				receive = msd_CMD( 0x66, 0, 0, 0, 0, 0xff );
				
				// ���X�|���X�m�F
				if( receive != 0x00 ) {
					ret = 3;
					break;
				}
				
				// busy��Ԃ��I���܂ő҂�
				for( i=0; i<10000; i++ ) {
					if( msd_read() != 0x00 ) break;
				}
				
				pattern_msd_erase = 5;
				break;
		}
	}
	
	// �E�F�C�g
	for( j = 0; j < 10000; j++ );
	
	msd_write( 0xff );		// �_�~�[�N���b�N���M
	MSD_CS_TERMINAL_HIGH		// CS�[�q��HIGH�ɂ���
	
	// �C���[�X��A�P��_�~�[���C�g
	if ( ret == 0 ) {
		for ( i=0; i<512; i++ ) msdlibBuff[i] = 0;   // �o�b�t�@�N���A
		msdlibMode = 0;
		receive = writeMicroSD( st_address, ( signed char* )msdlibBuff );
		if ( receive != 0x00 ) {
			// �������݂ł���
			// ���܂ɃG���[��Ԃ�microSD�����邪�A����Ȃ悤�Ȃ̂�
			// �G���[�͖������Ă���
			//ret = 44;
		}
	}
	msdlibMode = (ret != 0) ? 99 : 0;   // �������G���[�Ȃ�99���Z�b�g
	
	if ( ret == 0 ) {
		//printf("�C���[�Y����I��\n");
	} else if ( ret == 1 ) {
		printf("CMD32 error\n");
	} else if ( ret == 2 ) {
		printf("CMD33 error\n");
	} else if ( ret == 3 ) {
		printf("CMD38 errorn");
	}
	
	return ret;
}
/////////////////////////////////////////////////////////////////////////////////
// ���W���[���� setMicroSDdata								//
// �����T�v     microSD�Ƀf�[�^�Z�b�g							//
// ����         microSD�ɏ������ރf�[�^�̂���z��					//
// �߂�l       12:����ɏI�� ����ȊO:�������ݒ��ō���̃Z�b�g�͖���	//
/////////////////////////////////////////////////////////////////////////////////
char setMicroSDdata( signed char *p )
{	
	volatile short i = 0;
	
	if( msdlibMode != 11) {				// ���ݏ������ݏ�������?
		return msdlibMode;
	} else {
		// 512�o�C�g msdlibBuff�֓]��
		memcpy( msdlibBuff, p, 512 );
		
		msdlibBuff[512] = 0xff;
		msdlibBuff[513] = 0xff;
		
		msdlibCnt = 514;			// �o�b�t�@�������ݐ�
		msdlibWrite = msdlibBuff;
		
		msd_write( 0xfc );			// 1�������M(�c��͊����݂�)
		interrupt_msd_send_data = 1;
		//R_PG_Timer_StartCount_CMT_U1_C2(); 	// �J�E���g�X�^�[�g
		
		
		msdlibMode = 12;			// microSDProcess �����J�n
		return msdlibMode;
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� microSDProcessStart						//
// �����T�v     microSDProcess�J�n����						//
// ����         microSD�̏������݃A�h���X					//
// �߂�l       0:����ɏI�� ����ȊO:���ɏ������ݒ�				//
///////////////////////////////////////////////////////////////////////////
char microSDProcessStart( unsigned int address )
{
	volatile short		receive;
	volatile char		ret = 0;
	volatile unsigned char  a1, a2, a3, a4;
	
	if( msdlibMode != 0 ) {
		// ���ɏ�����
		ret = 1;
		return ret;
	} else {
		// SDHC�Ȃ�A�h���X���Z�N�^�ԍ��ɕϊ�
		address >>= 9;
		
		// microSD�ɏ������݃A�h���X�Z�b�g
		a1 = (unsigned char)( (address&0xff000000) >> 24);
		a2 = (unsigned char)( (address&0x00ff0000) >> 16);
		a3 = (unsigned char)( (address&0x0000ff00) >>  8);
		a4 = (unsigned char)(  address&0x000000ff       );
		
		MSD_CS_TERMINAL_LOW	// CS�[�q��LOW�ɂ���
		
		// CMD25���M
		receive = msd_CMD( 0x59, a1, a2, a3, a4, 0xff );
		
		// ���X�|���X�m�F
		if( ( receive & 0x80 ) != 0x00 ) {
			ret = 2;                        // �Z�b�g�ł��� 
			MSD_CS_TERMINAL_HIGH		// CS�[�q��HIGH�ɂ���
		} else {
			printf("CMD25 send\n");
			msdlibMode = 11;                // �Z�b�g����
		}
	}
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� microSDProcessEnd						//
// �����T�v     microSDProcess�I������						//
// ����         microSD�̏������݃A�h���X					//
// �߂�l       0:����ɏI�� ����ȊO:���ɏ������ݒ�				//
///////////////////////////////////////////////////////////////////////////
char microSDProcessEnd( void )
{
	volatile char ret = 1;
	
	if( msdlibMode == 11 ) {
		msdlibMode = 21;                // �I�������Z�b�g
	} else if( msdlibMode == 0 ) {
		ret = 0;                        // �I��
	}
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� microSDProcess							//
// �����T�v     microSD�@�Ԍ��������ݏ���					//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void microSDProcess( void )
{
	volatile short receive;
	
	// microSD msdprintf�֐�����(1ms���ƂɎ��s)
	//microSDprintfProcess();
	
	switch( msdlibMode ) {
		case 0:
			// �J�n�O�̑ҋ@
			break;
			
		case 1:
			// �ʂȍ�ƒ�
			break;
		
		case 11:
			// �J�n��̑ҋ@�@��������
			break;
		
		case 12:
			// ���M���荞�݂�microSD�Ƀf�[�^�������ݒ�
			if( msdlibCnt <= 0 ) {
				msdlibMode = 14;
			}
			break;
		
		case 13:
			// �Ō�̃f�[�^����܂ő҂�
			/*if( ri_u1c1 == 1 ) {
				i = u1rb;                   // �_�~�[���[�h
				msdlibMode = 14;
			}*/
			break;
		
		case 14:
			// �I������
			// ���X�|���X�ǂݍ���
			receive = msd_read();
			receive &= 0x1f;
		
			if( receive == 0x05 ) {
				msdlibError = 2;            // �������ݐ���I��
				msdlibMode = 15;
			} else if( receive == 0xc ) {
				msdlibError = 3;            // �������݃G���[
				msdlibMode = 31;
			} else {
				msdlibError = 4;            // ����ȊO�̃G���[
				msdlibMode = 31;
			}
			break;
		
		case 15:
			// busy��Ԃ��I���܂ő҂�
			if( msd_read() != 0x00 ) {
				msdlibMode = 11;
			}
			break;
	
		case 21:
			// �I������
			msd_write( 0xfd );
			msdlibMode = 22;
			break;
	
		case 22:
			// busy��Ԃ��I���܂ő҂�
			if( msd_read() != 0x00 ) {
				msdlibMode = 31;
			}
			break;
		
		case 31:
			// CS="1"
			msd_write( 0xff );		// �_�~�[�N���b�N���M
			MSD_CS_TERMINAL_LOW		// CS�[�q��LOW�ɂ���
			msdlibMode = 32;		// �����I��
			msdlibCnt = 10;
			break;

		case 32:
			msdlibCnt--;
			if( msdlibCnt == 0 ) {
				msdlibMode = 0;             // �����I��
			}
			break;
		
		case 99:
			// �������G���[
			break;
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� checkMicroSDProcess						//
// �����T�v     microSD�@�Ԍ��������ݏ����̏I���`�F�b�N			//
// ����         �Ȃ�									//
// �߂�l       0:�������� 11:�J�n��̑ҋ@ ����ȊO:������		//
///////////////////////////////////////////////////////////////////////////
char checkMicroSDProcess( void )
{
	return msdlibMode;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� msd_send_data							//
// �����T�v     �������݃f�[�^��MicroSD�ɑ��M				//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
// ����		10us���ƂɎ��s������						//
///////////////////////////////////////////////////////////////////////////
void msd_send_data (void)
{	
	__setpsw_i();
	if ( interrupt_msd_send_data == 1 ) {
		msd_write( *msdlibWrite++ );
		msdlibCnt--;
		if ( msdlibCnt <= 0) {
			interrupt_msd_send_data = 0;
			//R_PG_Timer_HaltCount_CMT_U1_C2();
		}
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� init_log								//
// �����T�v     �������ݏ���								//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void init_log ( void )
{
	volatile char ret, pattern_inti_log = 0;
	
	// microSD �������݊J�n�A�h���X
	// 512�̔{���ɐݒ肷��
	msdStartaddress = msdaddrBuff[0] + 1;
	// �I���A�h���X�������l�̏ꍇ�J�n�A�h���X��0�ɂ���
	if ( msdStartaddress == MSD_ENDADDRESS + 1 || msdStartaddress >= 4000000000 ) msdStartaddress = 0;

	// microSD �������ݏI���A�h���X
	// �������݂���������[ms] : x = 10[ms] : 64�o�C�g(�ۑ��o�C�g��)
	// 5000ms�Ȃ�Ax = 5000 * 64 / 10 = 32000
	// ���ʂ�512�̔{���ɂȂ�悤�ɌJ��グ����B����āA32256�ɂ���B
	// msdEndaddressBuff = RecodeTime * Data_Size / WriteTime
	// msdEndaddressBuff = ( 512 - ( msdEndaddressBuff % 512 ) ) + msdEndaddressBuff;
	
	msdEndaddress = MSD_ENDADDRESS;
	msdEndaddress += msdStartaddress;   // �X�^�[�g������
	printf("msdStartaddress = %d\n", msdStartaddress);
	printf("msdEndaddress = %d\n", msdEndaddress);
	
	while ( pattern_inti_log < 2 ) {
		switch ( pattern_inti_log ) {
			case 0:
				// microSD�N���A
				ret = eraseMicroSD( msdStartaddress, msdEndaddress - 1 );
				if( ret != 0x00 ) {
					setBeepPatternS( 0xaa00 );
					printf( "\nmicroSD Erase Error!!\n" );  // �G���[
					break;
				}
				pattern_inti_log = 1;
				break;
				
			case 1:
				// microSDProcess�J�n����
				ret = microSDProcessStart( msdStartaddress );
			        if( ret != 0x00 ) {
					setBeepPatternS( 0xaa00 );
					printf( "\nmicroSD microSDProcess Error!!\n" );  // �G���[
					break;
				}
				msdBuffaddress = 0;
				msdWorkaddress = msdStartaddress;
				
				flashDataBuff[ 0 ] = msdStartaddress >> 16;
				flashDataBuff[ 1 ] = msdStartaddress & 0xffff;	// �J�n�A�h���X
				writeFlashData( MSD_STARTAREA, MSD_ENDAREA, MSD_DATA, 2 );
				
				pattern_inti_log = 2;
				break;
		}
	} 
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� msdEndLog								//
// �����T�v     ���O�̏I������							//
// ����         �Ȃ�									//
// �߂�l       0:����ɏI�� 1:�ُ�I��						//
///////////////////////////////////////////////////////////////////////////
char msdEndLog ( void )
{
	char pattern_msdend = 0;
	
	while ( pattern_msdend < 2 ) {
	switch( pattern_msdend ) {
		case 0:
				// �Ō�̃f�[�^���������܂��܂ő҂�
				if ( checkMicroSDProcess() == 11 ) {
					msdFlag = 0;			// ���O�L�^�I��
					microSDProcessEnd();        // microSDProcess�I������
					pattern_msdend = 1;
					break;
				} else if ( checkMicroSDProcess() == 0 ) {
					setBeepPatternS( 0xf0f0 );
					pattern_msdend = 3;
					break;
				}
				break;
				
			case 1:
				// �I���������I���܂ő҂�
				if ( checkMicroSDProcess() == 0 ) {
					// MicroSD�ŏI�������݃A�h���X�ۑ�
					flashDataBuff[ 0 ] = msdWorkaddress >> 16;
					flashDataBuff[ 1 ] = msdWorkaddress & 0xffff;	// �I���A�h���X
					writeFlashData( MSD_STARTAREA, MSD_ENDAREA, MSD_DATA, 2 );
					pattern_msdend = 2;
					setBeepPatternS( 0xa8a8 );
					break;
				}
				break;
	}
	}
	
	return pattern - 2;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� sendLog								//
// �����T�v     PC�փf�[�^�]��							//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void sendLog (void) {
	msdTimer++;
	if( msdTimer == WRITINGTIME ) {
		msdTimer = 0;
		msdBuffPointa = msdBuff + msdBuffaddress;
		// ��������L�^
		send_Char			(	pattern		);
		send_Char			(	motorPwm 	);
		send_Char			(	sensor_inp() 	);
		send_Char			( 	slope_mode	);
		send_Char			(	(char)Encoder		);
		send_Char			(	sPwm		);
		send_Char			(	(char)PichAngleIMU*10	);
		send_Char			(	(char)RollAngleIMU*10	);
		
		send_ShortToChar	(	(short)TurningAngleIMU*10	);
		send_ShortToChar	(	rawXg		);
		send_ShortToChar	(	rawYg		);
		send_ShortToChar	(	rawZg		);
		send_ShortToChar	(	getServoAngle()	);
		send_ShortToChar	(	SetAngle		);
		send_ShortToChar	(	getAnalogSensor()	);
		
		send_uIntToChar 	(	EncoderTotal	);
		send_uIntToChar 	(	enc1			);
		send_ShortToChar 	(	cnt_log		);
		// �����܂�
		cnt_log += WRITINGTIME;
		msdBuffaddress += DATA_BYTE;       // RAM�̋L�^�A�h���X������
		if( msdBuffaddress >= 512 ) {
			msdBuffaddress = 0;
			setMicroSDdata( msdBuff ); 
			msdWorkaddress += 512;
			if( msdWorkaddress >= msdEndaddress ) {
				msdFlag = 0;
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� msd_sendToPC							//
// �����T�v     PC�փf�[�^�]��							//
// ����         �Ȃ�									//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void msd_sendToPC ( void )
{
	volatile unsigned short i;
	volatile short ret;
	volatile char pattern_send = 1;
	
	while ( pattern_send < 4 ) {
		if( tasw_get() == 0x2 ) {
			msdWorkaddress = msdEndaddress;
			pattern_send = 2;
		}
		switch ( pattern_send ) {		
			case 1:
				i = 0;
				// �^�C�g��
				printf(	"Time[ms],"		);
				printf(	"pattern,"			);
				printf(	"MotorPwm,"		);
				printf(	"sensor_inp(),"		);
				printf(	"slope_mode,"		);
				printf(	"Encoder,"			);
				printf(	"sPwm,"		);
				printf(	"PichAngleIMU,"		);
				printf(	"RollAngleIMU,"		);
				
				printf(	"TurningAngleIMU,"	);
				printf(	"xg[degrees/sec],"	);
				printf(	"yg[degrees/sec],"	);
				printf(	"zg[degrees/sec],"	);
				printf(	"getServoAngle,"	);
				printf(	"SetAngle,"		);
				printf(	"getAnalogSensor,"	);
				
				printf(	"EncoderTotal,"		);
				printf(	"enc1,"			);
				printf(	"cnt_log[ms]"		);
				printf("\n");
				
				msdEndaddress = msdWorkaddress2;	// �ǂݍ��ݏI���A�h���X
				msdWorkaddress = msdWorkaddress;	// �ǂݍ��݊J�n�A�h���X
				
				pattern_send = 2;
				break;
				
			case 2:
				// microSD���f�[�^�ǂݍ���
				if( msdWorkaddress >= msdEndaddress ) {
					// �������ݏI���A�h���X�ɂȂ�����A�I���
					printf( "End.\n" );
					setBeepPatternS( 0xa8a8 );
					pattern_send = 4;
					break;
				}
				ret = readMicroSD( msdWorkaddress , msdBuff );
				
				if( ret != 0x00 ) {
					// �ǂݍ��݃G���[
					printf( "\nmicroSD Read Error!!\n" );
					pattern_send = 4;
					break;
				} else {
					// �G���[�Ȃ�
					msdWorkaddress += 512;		// microSD�̃A�h���X��+512����
					msdBuffaddress = 0;		// �z�񂩂�̓ǂݍ��݈ʒu��0��
					pattern_send = 3;
					break;
				}
				break;
				
			case 3:
				// �f�[�^�]��
				printf("%5d,", i);
				printf("%5d,", msdBuff[ msdBuffaddress + 0 ]);	// pattern
				printf("%5d,", msdBuff[ msdBuffaddress + 1 ]);	// motorPwm
				printf("%5d,", msdBuff[ msdBuffaddress + 2 ]);	// sensor_inp()
				printf("%5d,", msdBuff[ msdBuffaddress + 3 ]);	// slope_mode
				printf("%5d,", msdBuff[ msdBuffaddress + 4 ]);	// Encoder
				printf("%5d,", msdBuff[ msdBuffaddress + 5 ]);	// targetSpeed
				printf("%5d,", msdBuff[ msdBuffaddress + 6 ]/10);	// PichAngleIMU
				printf("%5d,", msdBuff[ msdBuffaddress + 7 ]/10);	// RollAngleIMU
				
				printf("%4.2f,", (double)CharToShort(8)/10);	// TurningAngleIMU
				printf("%4.4f,", (double)CharToShort(10) / GYROLSB);// xg
				printf("%4.4f,", (double)CharToShort(12) / GYROLSB);// yg
				printf("%4.4f,", (double)CharToShort(14) / GYROLSB);// zg
				printf("%5d,", CharToShort(16));				// getServoAngle()
				printf("%5d,", CharToShort(18));				// SetAngle
				printf("%5d,", CharToShort(20));				// getAnalogSensor()
				
				printf("%6d,", CharTouInt (22));		// EncoderTotal
				printf("%6d,", CharTouInt (26));		// enc1
				printf("%6d", CharToShort (30));		// cnt_log
				printf("\n");
				i += WRITINGTIME;
				msdBuffaddress += DATA_BYTE;
				
				if( msdBuffaddress >= 512 ) {
					pattern_send = 2;
					break;
				}
				break;
		}
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� send_Char								//
// �����T�v     char�^�ϐ���msdBuff�ɑ���					//
// ����         �ϊ�����char�^�ϐ�							//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void send_Char ( char data )
{
	*msdBuffPointa++ = data;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� ShortToChar							//
// �����T�v     short�^�ϐ���char�^�ɕϊ�����msdBuff�ɑ���		//
// ����         �ϊ�����short�^�ϐ�							//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void send_ShortToChar ( short data )
{
	*msdBuffPointa++ = data >> 8;
	*msdBuffPointa++ = data & 0xff;
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� uintToChar								//
// �����T�v     unsigned int�^�ϐ���char�^�ɕϊ�����msdBuff�ɑ���	//
// ����         �ϊ�����unsigned int�^�ϐ�					//
// �߂�l       �Ȃ�									//
///////////////////////////////////////////////////////////////////////////
void send_uIntToChar ( unsigned int data )
{
	*msdBuffPointa++ = data >> 24;
	*msdBuffPointa++ = ( data & 0x00ff0000 ) >> 16;
	*msdBuffPointa++ = ( data & 0x0000ff00 ) >> 8;
	*msdBuffPointa++ = data & 0x000000ff;
}
///////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� CharToShort									//
// �����T�v     unsigned char�^�ϐ���short�^�ɕϊ�����					//
// ����         data:�ϊ�����signed char�^�ϐ�	offsetaddress: MicroSD���̏���	//
// �߂�l       �ϊ�����short�^									//
///////////////////////////////////////////////////////////////////////////////////////
short CharToShort( unsigned char offsetaddress )
{
	volatile short s;
	
	s = (short)( (unsigned char)msdBuff[msdBuffaddress + offsetaddress] * 0x100 + 
			(unsigned char)msdBuff[msdBuffaddress + offsetaddress + 1] );
					
	return s;				
}
///////////////////////////////////////////////////////////////////////////////////////
// ���W���[���� CharTouInt										//
// �����T�v     unsigned char�^�ϐ���unsigned int�^�ɕϊ�����				//
// ����         data:�ϊ�����signed char�^�ϐ�	offsetaddress: MicroSD���̏���	//
// �߂�l       �ϊ�����unsigned int�^								//
///////////////////////////////////////////////////////////////////////////////////////
unsigned int CharTouInt( unsigned char offsetaddress )
{
	volatile unsigned int i;
	
	i = (unsigned int)(unsigned char)msdBuff[msdBuffaddress + offsetaddress] * 0x1000000;
	i += (unsigned int)(unsigned char)msdBuff[msdBuffaddress + offsetaddress + 1] * 0x10000;
	i += (unsigned int)(unsigned char)msdBuff[msdBuffaddress + offsetaddress + 2] * 0x100;
	i += (unsigned int)(unsigned char)msdBuff[msdBuffaddress + offsetaddress + 3];
					
	return i;				
}