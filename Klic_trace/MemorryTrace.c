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
char			flag[100][20] = {0,0,0,0,0,0,0,0,0,0};
short			cnt[10];

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
	cnt[0] = 0;
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
				// flag[ STRAIGHT ][0]	0:�˓��ʒu 		1: �E�o�ʒu
				// flag[ STRAIGHT ][ 1 ]	�J��Ԃ���
				// cnt[ STRAIGHT ][0] 	�J�[�u�˓���E�o�ʒu�̐�
				// comp_uint[ STRAIGHT ][ cnt[ STRAIGHT ][ 0 ] ]	EncoderTotal���i�[����
				// printf("%2d, %6d\n", msdBuff[ msdBuffaddress + 0 ], CharTouInt (40) );		// EncoderTotal
				switch( mpattern ) {
					case 11:
						if( serchPattern( STRAIGHT, 12 ) ) mpattern = 12;
						break;
						
					case 12:
						if( serchPattern( STRAIGHT, 11 ) ) mpattern = 11;
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
	// �J�[�u��E�o����ʒu��T��
	// spattern ��3������
	if ( msdBuff[ msdBuffaddress + 0 ] == spattern ) flag[ process ][ spattern ]++;
	else		flag[ process ][ spattern ] = 0;
	
	// 3�ȏ゠��΃J�[�u��E�o�����Ɣ��f����
	if ( flag[ process ][ spattern ] >= 3 ) {
		comp_uint[ process ][ cnt[ process ] ] = CharTouInt (40);		// �����擾
		//printf("%d, %6d\n", spattern, comp_uint[ process ][ cnt[ process ] ] );
		cnt[ process ]++;
		flag[ process ][ spattern ] = 0;	// �J��Ԃ��J�E���g�����Z�b�g
		return true;
	} else {
		return false;
	}
}