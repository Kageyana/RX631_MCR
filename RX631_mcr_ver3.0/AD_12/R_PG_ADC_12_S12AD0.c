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
* File Name    : R_PG_ADC_12_S12AD0.c
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
#include "r_pdl_adc_12.h"
#include "r_pdl_definitions.h"
#include "R_PG_IntFuncsExtern.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_ADC_12_Set_S12AD0(void)
*
* Function Name: R_PG_ADC_12_Set_S12AD0
*
* Description  : A/D�ϊ���̐ݒ�
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_ADC_12_Create
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_ADC_12_Set_S12AD0(void)
{
	return R_ADC_12_Create(
		0,
		PDL_ADC_12_CHANNEL_3 | PDL_ADC_12_CHANNEL_4 | PDL_ADC_12_CHANNEL_5 | PDL_ADC_12_CHANNEL_6 | PDL_ADC_12_CHANNEL_7 | PDL_ADC_12_CHANNEL_8 | PDL_ADC_12_CHANNEL_12 | PDL_ADC_12_CHANNEL_13,
		PDL_ADC_12_SCAN_CONTINUOUS | PDL_ADC_12_DIV_8 | PDL_ADC_12_DATA_ALIGNMENT_RIGHT | PDL_ADC_12_CLEAR_RESULT | PDL_ADC_12_INPUT_AN | PDL_ADC_12_DMAC_DTC_TRIGGER_DISABLE | PDL_ADC_12_SAMPLING_TIME_SPECIFY,
		PDL_ADC_12_TRIGGER_SOFTWARE,
		PDL_NO_DATA,
		75,
		20,
		ADconverter,
		11
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_ADC_12_StartConversionSW_S12AD0(void)
*
* Function Name: R_PG_ADC_12_StartConversionSW_S12AD0
*
* Description  : A/D�ϊ���̊J�n(�\�t�g�E�F�A�g���K)
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_ADC_12_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_ADC_12_StartConversionSW_S12AD0(void)
{
	return R_ADC_12_Control( PDL_ADC_12_0_ON );

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_ADC_12_StopConversion_S12AD0(void)
*
* Function Name: R_PG_ADC_12_StopConversion_S12AD0
*
* Description  : A/D�ϊ��̒��f
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ϊ���~���������s��ꂽ�ꍇ
*              : false : �ϊ���~�Ɏ��s�����ꍇ
*
* Calling Functions : R_ADC_12_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_ADC_12_StopConversion_S12AD0(void)
{
	return R_ADC_12_Control( PDL_ADC_12_0_OFF );

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_ADC_12_GetResult_S12AD0(uint16_t * result)
*
* Function Name: R_PG_ADC_12_GetResult_S12AD0
*
* Description  : A/D�ϊ����ʂ̎擾
*
* Arguments    : uint16_t * result : A/D�ϊ����ʂ̊i�[��
*
* Return Value : true  : ���ʂ̎擾���������s��ꂽ�ꍇ
*              : false : ���ʂ̎擾�Ɏ��s�����ꍇ
*
* Calling Functions : R_ADC_12_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_ADC_12_GetResult_S12AD0(uint16_t * result)
{
	if( result == 0 )
	{
		return false;
	}

	return R_ADC_12_Read(
		0,
		result
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_ADC_12_StopModule_S12AD0(void)
*
* Function Name: R_PG_ADC_12_StopModule_S12AD0
*
* Description  : A/D�ϊ���̒�~
*
* Arguments    : �Ȃ�
*
* Return Value : true  : ��~�ɐ��������ꍇ
*              : false : ��~�Ɏ��s�����ꍇ
*
* Calling Functions : R_ADC_12_Destroy
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_ADC_12_StopModule_S12AD0(void)
{
	return R_ADC_12_Destroy( 0 );

}



