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
* File Name    : R_PG_Timer_IWDT.c
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
#include "r_pdl_iwdt.h"
#include "r_pdl_definitions.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_Start_IWDT(void)
*
* Function Name: R_PG_Timer_Start_IWDT
*
* Description  : IWDT�̐ݒ�ƊJ�n
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_IWDT_Set
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_Start_IWDT(void)
{
	return R_IWDT_Set( PDL_IWDT_TIMEOUT_1024 | PDL_IWDT_CLOCK_OCO_16 | PDL_IWDT_TIMEOUT_RESET | PDL_IWDT_WIN_START_100 | PDL_IWDT_WIN_END_0 | PDL_IWDT_STOP_DISABLE );

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_RefreshCounter_IWDT(void)
*
* Function Name: R_PG_Timer_RefreshCounter_IWDT
*
* Description  : IWDT�̃J�E���^�����t���b�V��
*
* Arguments    : �Ȃ�
*
* Return Value : true  : ���t���b�V���ɐ��������ꍇ
*              : false : ���t���b�V���Ɏ��s�����ꍇ
*
* Calling Functions : R_IWDT_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_RefreshCounter_IWDT(void)
{
	return R_IWDT_Control( PDL_IWDT_REFRESH );

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_GetStatus_IWDT(uint16_t* counter_val, bool * undf, bool * ref_err)
*
* Function Name: R_PG_Timer_GetStatus_IWDT
*
* Description  : IWDT�̃X�e�[�^�X�t���O�ƃJ�E���g�l���擾
*
* Arguments    : uint16_t* counter_val : �J�E���^�l�̊i�[��
*              : bool * undf : �A���_�t���[�t���O�̊i�[��
*              : bool * ref_err : ���t���b�V���G���[�t���O�̊i�[��
*
* Return Value : true  : �t���O�̎擾���������s��ꂽ�ꍇ
*              : false : �t���O�̎擾�Ɏ��s�����ꍇ
*
* Calling Functions : R_IWDT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_GetStatus_IWDT(uint16_t* counter_val, bool * undf, bool * ref_err)
{
	uint16_t status;
	bool res;

	res = R_IWDT_Read( &status );

	if(counter_val){ *counter_val = status & 0x3fff; }

	return res;
}



