//======================================//
// �C���N���[�h
//======================================//
#include "AQM0802A.h"
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
// LCD�֘A
static volatile char	buffLcdData[ LCD_MAX_X / LCD_MAX_Y ];		// �\���o�b�t�@
static char		buffLcdData2[ LCD_MAX_X / LCD_MAX_Y + 10 ]; 	// �\���o�b�t�@�ꎞ��ƃG���A
static volatile int	lcdBuffPosition;				// �o�b�t�@�ɏ������ވʒu
static volatile int	lcdMode2 = 1;					// �\������No�Ǘ�
static volatile int	lcdNowLocate;					// ���݂̕\�����Ă���ʒu
static volatile int	lcdRefreshFlag;					// ���t���b�V���t���O

//////////////////////////////////////////////////////////////////////////
// ���W���[���� lcdPut
// �����T�v     �f�[�^���M
// ����         data
// �߂�l       �Ȃ�
//////////////////////////////////////////////////////////////////////////
void lcdPut( unsigned char data )
{
	uint8_t word[] = { RSBIT1, data };
	I2C_LCD_SEND
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� lcdCMD
// �����T�v     �R�}���h���M
// ����         cmd
// �߂�l       �Ȃ�
//////////////////////////////////////////////////////////////////////////
void lcdCMD( unsigned char cmd ) 
{
	uint8_t Command[] = { RSBIT0, cmd };
 	I2C_LCD_READ
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� waitLcd
// �����T�v     �x������
// ����         �x������(ms)
// �߂�l       �Ȃ�
//////////////////////////////////////////////////////////////////////////
void waitLcd ( short waitTime )
{
	volatile int time, i = 0;
	
	time = (int)waitTime * ( CLOCK * 1000 )/ 16;
	for ( i = 0; i < time; i++) __nop();
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� intiLcd
// �����T�v     LCD�̏�����
// ����         �Ȃ�
// �߂�l       �Ȃ�
//////////////////////////////////////////////////////////////////////////
 void intiLcd(void)
 {
	waitLcd(4);
	lcdCMD(0x38);	// function set			: �f�[�^����8�{�E�\���͂Q�s�E�t�H���g��5x8�h�b�g
	waitLcd(1);
	lcdCMD(0x39);	// function set           	: �g���R�}���h�̐ݒ��L���ɂ���
	waitLcd(1);
	lcdCMD(0x14);	// Internal OSC frequency 	: �o�C�A�X�̑I���Ɠ���OSC���g���̒���
	waitLcd(1);
	lcdCMD(0x70);	// Contrast set          	: �R���g���X�g�����f�[�^(����4�r�b�g)
	waitLcd(1);
	lcdCMD(0x56);	// Power/ICON/Contrast control	: ������H�L���A�R���g���X�g�����f�[�^(���2�r�b�g)
	waitLcd(1);
	lcdCMD(0x6c);	// Follower control     	: �t�H���A��H��ON�A�������̒������s��
	waitLcd(200);
	lcdCMD(0x38);	// function set         	: �g���R�}���h��ݒ�𖳌��ɂ���
	waitLcd(1);
	lcdCMD(0x0c);	// display ON/OFF control      	: ��ʕ\����ON�E�J�[�\���\����OFF�E�J�[�\���_�ł�OFF
	waitLcd(1);
	lcdCMD(0x01);	// Clear Display 		: ��ʑS�̂�20H�̽�߰��ŕ\���A���ق�col=0,row=0�Ɉړ�
	waitLcd(2);
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� lcdLocate
// �����T�v     �t���J�[�\���ړ�
// ����         x , y
// �߂�l       �Ȃ�
//////////////////////////////////////////////////////////////////////////
static void lcdLocate( int x, int y )
{
    volatile unsigned char work = 0x80;

    // x�̌v�Z
    work += x;

    // y�̌v�Z
    if( y == 1 ) {
        work += 0x40;
    } else if( y == 2 ) {
        work += 0x14;
    } else if( y == 3 ) {
        work += 0x54;
    }

    // �J�[�\���ړ�
    lcdCMD(work);
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� lcdShowProcess
// �����T�v     �t���\������
// ����         �Ȃ�
// �߂�l       �Ȃ�
// ����         ���̊֐��͊��荞�݂�1ms���ƂɎ��s���Ă�������
//////////////////////////////////////////////////////////////////////////
void lcdShowProcess( void )
{
    switch( lcdMode2 ) {
    case 1: // �f�[�^�X�V���ꂽ���`�F�b�N
    	if( lcdRefreshFlag ) {
    		lcdRefreshFlag = 0;
    		lcdMode2 = 2;
	}
        break;

    case 2: // �ʒu������
	lcdNowLocate = 0;
    	lcdLocate( 0, 0 );
	lcdMode2 = 3;
        break;

    case 3: // ���s�ʒu�̊m�F 
	if( lcdNowLocate % LCD_MAX_X == 0 ) {
    		lcdLocate( 0, lcdNowLocate / LCD_MAX_X );
	}
	lcdMode2 = 4;
        break;

    case 4: // �f�[�^�\������
	lcdPut(buffLcdData[ lcdNowLocate++ ]);
	if( lcdNowLocate >= LCD_MAX_X * LCD_MAX_Y ) {
		lcdMode2 = 1;
	} else {
		lcdMode2 = 3;
	}
	break;

    default:
	lcdMode2 = 1;
   	break;
    }
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� lcdPrintf
// �����T�v     �t���֕\�� �\���ʒu�͉ߋ��ɕ\�������ʒu�̎�����
// ����         printf�Ɠ���
// �߂�l       ���펞�F�o�͂��������� �ُ펞�F���̐�
//////////////////////////////////////////////////////////////////////////
int lcdPrintf(char *format, ...)
{
    volatile va_list argptr;
    volatile char    *p;
    volatile short     ret = 0;

    va_start(argptr, format);
    ret = vsprintf( buffLcdData2, format, argptr );
    va_end(argptr);

    if( ret > 0 ) {
        // vsprintf������Ȃ�t���o�b�t�@�֓]��
        p = buffLcdData2;
        while( *p ) {
            buffLcdData[lcdBuffPosition++] = *p++;
            if( lcdBuffPosition >= LCD_MAX_X * LCD_MAX_Y ) {
                lcdBuffPosition = 0;
            }
        }
        lcdRefreshFlag = 1;
    }
    return ret;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� lcdPosition
// �����T�v     �t���̕\���ʒu�w��
// ����         ���ʒu , �c�ʒu
// �߂�l       �Ȃ�
//////////////////////////////////////////////////////////////////////////
void lcdPosition(char x ,char y)
{
    if( x >= LCD_MAX_X ) return;
    if( y >= LCD_MAX_Y ) return;

    lcdBuffPosition = x + y * LCD_MAX_X;
}
//////////////////////////////////////////////////////////////////////////
// ���W���[���� lcdPrintf
// �����T�v     �t���֕\�� �\���ʒu�͉ߋ��ɕ\�������ʒu�̎�����
// ����         printf�Ɠ���
// �߂�l       ���펞�F�o�͂��������� �ُ펞�F���̐�
//////////////////////////////////////////////////////////////////////////
int lcdRowPrintf(char step, char *format, ...)
{
    volatile va_list argptr;
    volatile char    *p;
    volatile short     ret = 0;

	if(step == UPROW){
		lcdPosition( 0, 0 );
	} else if (step == LOWROW){
		lcdPosition( 0, 1 );
	}

    va_start(argptr, format);
    ret = vsprintf( buffLcdData2, format, argptr );
    va_end(argptr);

    if( ret > 0 ) {
        // vsprintf������Ȃ�t���o�b�t�@�֓]��
        p = buffLcdData2;
        while( *p ) {
            buffLcdData[lcdBuffPosition++] = *p++;
            if( lcdBuffPosition >= LCD_MAX_X * LCD_MAX_Y ) {
                lcdBuffPosition = 0;
            }
        }
        lcdRefreshFlag = 1;
    }
    return ret;
}