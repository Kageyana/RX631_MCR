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
* File Name    : R_PG_Timer_CMT_U0.c
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
#include "r_pdl_cmt.h"
#include "r_pdl_definitions.h"
#include "R_PG_IntFuncsExtern.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_Set_CMT_U0_C0(void)
*
* Function Name: R_PG_Timer_Set_CMT_U0_C0
*
* Description  : CMT��ݒ�
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_CMT_Create
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_Set_CMT_U0_C0(void)
{
	return R_CMT_Create(
		0,
		PDL_CMT_PCLK_DIV_8 | PDL_CMT_DMAC_DTC_TRIGGER_DISABLE | PDL_CMT_STOP,
		5999,
		Timer,
		6
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_StartCount_CMT_U0_C0(void)
*
* Function Name: R_PG_Timer_StartCount_CMT_U0_C0
*
* Description  : CMT�̃J�E���g���J�n
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �J�E���g����̍ĊJ���������s��ꂽ�ꍇ
*              : false : �J�E���g����̍ĊJ�Ɏ��s�����ꍇ
*
* Calling Functions : R_CMT_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_StartCount_CMT_U0_C0(void)
{
	return R_CMT_Control(
		0,
		PDL_CMT_START,
		0
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_HaltCount_CMT_U0_C0(void)
*
* Function Name: R_PG_Timer_HaltCount_CMT_U0_C0
*
* Description  : CMT�̃J�E���g���ꎞ��~
*
* Arguments    : �Ȃ�
*
* Return Value : true  : ��~�ɐ��������ꍇ
*              : false : ��~�Ɏ��s�����ꍇ
*
* Calling Functions : R_CMT_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_HaltCount_CMT_U0_C0(void)
{
	return R_CMT_Control(
		0,
		PDL_CMT_STOP,
		0
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_GetCounterValue_CMT_U0_C0(uint16_t * counter_val)
*
* Function Name: R_PG_Timer_GetCounterValue_CMT_U0_C0
*
* Description  : CMT�̃J�E���^�l���擾
*
* Arguments    : uint16_t * counter_val : �J�E���^�l�̊i�[��
*
* Return Value : true  : �擾�ɐ��������ꍇ
*              : false : �擾�Ɏ��s�����ꍇ
*
* Calling Functions : R_CMT_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_GetCounterValue_CMT_U0_C0(uint16_t * counter_val)
{
	if( counter_val == 0 ){ return false; }

	return R_CMT_Read(
		0,
		PDL_NO_PTR,
		counter_val
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_SetCounterValue_CMT_U0_C0(uint16_t counter_val)
*
* Function Name: R_PG_Timer_SetCounterValue_CMT_U0_C0
*
* Description  : CMT�̃J�E���^�l��ݒ�
*
* Arguments    : uint16_t counter_val : �J�E���^�ɐݒ肷��l
*
* Return Value : true  : �J�E���^�l�̐ݒ�ɐ��������ꍇ
*              : false : �J�E���^�l�̐ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_CMT_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_SetCounterValue_CMT_U0_C0(uint16_t counter_val)
{
	return R_CMT_Control(
		0,
		PDL_CMT_COUNTER,
		counter_val
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_SetConstantRegister_CMT_U0_C0(uint16_t constant_val)
*
* Function Name: R_PG_Timer_SetConstantRegister_CMT_U0_C0
*
* Description  : CMT�̃R���X�^���g���W�X�^�l��ݒ�
*
* Arguments    : uint16_t constant_val : �R���X�^���g���W�X�^�l�̊i�[��
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_CMT_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_SetConstantRegister_CMT_U0_C0(uint16_t constant_val)
{
	return R_CMT_Control(
		0,
		PDL_CMT_CONSTANT,
		constant_val
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_Timer_StopModule_CMT_U0(void)
*
* Function Name: R_PG_Timer_StopModule_CMT_U0
*
* Description  : CMT�̃��j�b�g���~
*
* Arguments    : �Ȃ�
*
* Return Value : true  : ��~�ɐ��������ꍇ
*              : false : ��~�Ɏ��s�����ꍇ
*
* Calling Functions : R_CMT_Destroy
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_Timer_StopModule_CMT_U0(void)
{
	return R_CMT_Destroy( 0 );

}



