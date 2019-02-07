//====================================//
// �C���N���[�h									//
//====================================//
#include "MemorryTrace.h"
//====================================//
// �O���[�o���ϐ��̐錾							//
//====================================//
// ���O��͊֘A
char			comp_char[10][100] = {0,0,0,0,0,0,0,0,0,0};
short			comp_short[10][100] = {0,0,0,0,0,0,0,0,0,0};
unsigned int	comp_uint[10][100] = {0,0,0,0,0,0,0,0,0,0};
char			flag[10][100];
unsigned short	cntp[10];
char 			cntmpattern[500];
char			i;

char			mpattern = 11;
///////////////////////////////////////////////////////////////////////////
// ���W���[���� msdgetData								//
// �����T�v     ���O����p�e�����J�n���̈ʒu��������			//
// ����         �Ȃ�									//
// �߂�l       0:����ɏI�� 1:�ُ�I��						//
///////////////////////////////////////////////////////////////////////////
void msdgetData () 
{
	volatile unsigned short i;
	volatile short ret;
	volatile char pattern_send = 1;
	flag[STRAIGHT][0] = 12;
	mpattern = 11;
	
	msdEndaddress = msdWorkaddress2;	// �ǂݍ��ݏI���A�h���X
	msdWorkaddress = msdWorkaddress;	// �ǂݍ��݊J�n�A�h���X
	
	while ( pattern_send < 4 ) {
		switch ( pattern_send ) {			
			case 1:
				// microSD���f�[�^�ǂݍ���
				if( msdWorkaddress >= msdEndaddress ) {
					// �������ݏI���A�h���X�ɂȂ�����A�I���
					//printf( "End.\n" );
					setBeepPatternS( 0xa8a8 );
					pattern_send = 4;
					break;
				}
				ret = readMicroSD( msdWorkaddress , msdBuff );
				
				if( ret != 0x00 ) {
					// �ǂݍ��݃G���[
					//printf( "\nmicroSD Read Error!!\n" );
					pattern_send = 4;
					break;
				} else {
					// �G���[�Ȃ�
					msdWorkaddress += 512;		// microSD�̃A�h���X��+512����
					msdBuffaddress = 0;		// �z�񂩂�̓ǂݍ��݈ʒu��0��
					pattern_send = 2;
					break;
				}
				break;
				
			case 2:
				switch( mpattern ) {
					case 11:
						if ( serchPattern( STRAIGHT, 12 ) ) mpattern = 12;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 12:
						if ( serchPattern( STRAIGHT, 13 ) ) mpattern = 13;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 13:
						if ( serchPattern( STRAIGHT, 14 ) ) mpattern = 14;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 14:
						if ( serchPattern( STRAIGHT, 15 ) ) mpattern = 15;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 15:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 21:
						if ( serchPattern( CROSSLINE, 22 ) ) mpattern = 22;
						break;
						
					case 22:
						if ( serchPattern( RC, 31 ) ) mpattern = 31;
						if ( serchPattern( LC, 41 ) ) mpattern = 41;
						break;
						
					case 31:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 41:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 51:
						if ( serchPattern( RLC, 52 ) ) mpattern = 52;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						break;
						
					case 52:
						if ( serchPattern( RLC, 53 ) ) mpattern = 53;
						break;
						
					case 53:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 61:
						if ( serchPattern( LLC, 62 ) ) mpattern = 62;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						break;
						
					case 62:
						if ( serchPattern( LLC, 63 ) ) mpattern = 63;
						break;
						
					case 63:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					case 71:
						if ( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
						if ( serchPattern( CROSSLINE, 21 ) ) mpattern = 21;
						if ( serchPattern( RLC, 51 ) ) mpattern = 51;
						if ( serchPattern( LLC, 61 ) ) mpattern = 61;
						break;
						
					
				}
				
				msdBuffaddress += DATA_BYTE;
				
				if( msdBuffaddress >= 512 ) {
					pattern_send = 1;
					break;
				}
				break;
		}
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� serchPattern							//
// �����T�v     ���O����e�����J�n���̈ʒu��������			//
// ����         �Ȃ�									//
// �߂�l       0:����ɏI�� 1:�ُ�I��						//
///////////////////////////////////////////////////////////////////////////
bool serchPattern ( char process, char spattern )
{
	// flag[ STRAIGHT ][0]	0:�˓��ʒu 		1: �E�o�ʒu
	// flag[ STRAIGHT ][ 1 ]	�J��Ԃ���
	// cnt[ STRAIGHT ][0] 	�J�[�u�˓���E�o�ʒu�̐�
	// comp_uint[ STRAIGHT ][ cnt[ STRAIGHT ][ 0 ] ]	EncoderTotal���i�[����
	// printf("%2d, %6d\n", msdBuff[ msdBuffaddress + 0 ], CharTouInt (40) );		// EncoderTotal
	// �J�[�u��E�o����ʒu��T��
	// spattern ��3������
	if ( msdBuff[ msdBuffaddress + 0 ] == spattern ) flag[ process ][ spattern ]++;
	else		flag[ process ][ spattern ] = 0;
	
	// 3�ȏ゠��΃J�[�u��E�o�����Ɣ��f����
	if ( flag[ process ][ spattern ] >= 3 ) {
		comp_uint[ process ][ cntp[ process ] ] = CharTouInt (40);		// �����擾
		//printf("%d, %6d\n", spattern, comp_uint[ process ][ cnt[ process ] ] );
		cntp[ process ]++;
		flag[ process ][ spattern ] = 0;	// �J��Ԃ��J�E���g�����Z�b�g
		cntmpattern[ i ] = spattern;
		i++;
		return true;
	} else {
		return false;
	}
}
///////////////////////////////////////////////////////////////////////////
// ���W���[���� serchPattern							//
// �����T�v     ���O����e�����J�n���̈ʒu��������			//
// ����         �Ȃ�									//
// �߂�l       0:����ɏI�� 1:�ُ�I��						//
///////////////////////////////////////////////////////////////////////////
void logmeter( void )
{
	short k, m, l;
	
	if (comp_uint[ STRAIGHT ][ k ]) {
		if ( comp_uint[ STRAIGHT ][ k ] <=EncoderTotal && m >= 0) {
			l = 12;
			m = -m;
			k++;
		} else if ( comp_uint[ STRAIGHT ][ k ] <= EncoderTotal && m < 0 ) {
			l = 11;
			m = -m;
			k++;
		}
	}
}