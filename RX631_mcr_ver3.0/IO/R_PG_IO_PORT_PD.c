/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_PD.c
* Version      : 1.00
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
* Limitations  : 
******************************************************************************
* History : 06.03.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_pdl_io_port.h"
#include "r_pdl_definitions.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PD(void)
*
* Function Name: R_PG_IO_PORT_Set_PD
*
* Description  : I/O�|�[�g�̐ݒ�
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Set_PD(void)
{
	bool res;

	res = R_IO_PORT_Set(
		PDL_IO_PORT_D_0,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_OFF
	);

	if( !res ){
		return res;
	}

	return res;
}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Set_PD0(void)
*
* Function Name: R_PG_IO_PORT_Set_PD0
*
* Description  : I/O�|�[�g(1�[�q)�̐ݒ�
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Set
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Set_PD0(void)
{
	return R_IO_PORT_Set(
		PDL_IO_PORT_D_0,
		PDL_IO_PORT_INPUT | PDL_IO_PORT_PULL_UP_OFF
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD
*
* Description  : I/O�|�[�g���W�X�^����̓ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD0(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD0
*
* Description  : I/O�|�[�g���W�X�^����̃r�b�g�ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD0(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D_0,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD1(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD1
*
* Description  : I/O�|�[�g���W�X�^����̃r�b�g�ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD1(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D_1,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD2(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD2
*
* Description  : I/O�|�[�g���W�X�^����̃r�b�g�ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD2(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D_2,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD3(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD3
*
* Description  : I/O�|�[�g���W�X�^����̃r�b�g�ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD3(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D_3,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD4(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD4
*
* Description  : I/O�|�[�g���W�X�^����̃r�b�g�ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD4(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D_4,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD5(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD5
*
* Description  : I/O�|�[�g���W�X�^����̃r�b�g�ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD5(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D_5,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD6(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD6
*
* Description  : I/O�|�[�g���W�X�^����̃r�b�g�ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD6(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D_6,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Read_PD7(uint8_t * data)
*
* Function Name: R_PG_IO_PORT_Read_PD7
*
* Description  : I/O�|�[�g���W�X�^����̃r�b�g�ǂݏo��
*
* Arguments    : uint8_t * data : �ǂݏo�����[�q��Ԃ̊i�[��
*
* Return Value : true  : �ǂݏo�����������s��ꂽ�ꍇ
*              : false : �ǂݏo���Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Read_PD7(uint8_t * data)
{
	if( data == 0 ){ return false; }

	return R_IO_PORT_Read(
		PDL_IO_PORT_D_7,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PD(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PD
*
* Description  : I/O�|�[�g�f�[�^���W�X�^�ւ̏�������
*
* Arguments    : uint8_t data : �������ޒl
*
* Return Value : true  : �������݂ɐ��������ꍇ
*              : false : �������݂Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Write_PD(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_D,
		data
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_IO_PORT_Write_PD0(uint8_t data)
*
* Function Name: R_PG_IO_PORT_Write_PD0
*
* Description  : I/O�|�[�g�f�[�^���W�X�^�ւ̃r�b�g��������
*
* Arguments    : uint8_t data : �������ޒl
*
* Return Value : true  : �������݂ɐ��������ꍇ
*              : false : �������݂Ɏ��s�����ꍇ
*
* Calling Functions : R_IO_PORT_Write
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_IO_PORT_Write_PD0(uint8_t data)
{
	return R_IO_PORT_Write(
		PDL_IO_PORT_D_0,
		data
	);

}



