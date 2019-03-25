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
* File Name    : R_PG_SCI_C5.c
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
#include "r_pdl_sci.h"
#include "r_pdl_definitions.h"
#include "R_PG_IntFuncsExtern.h"


/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_Set_C5(void)
*
* Function Name: R_PG_SCI_Set_C5
*
* Description  : �V���A��I/O�`���l���̐ݒ�
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_Create
*                   : R_SCI_Set
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_Set_C5(void)
{
	bool res;

	res = R_SCI_Set(
		5,
		PDL_SCI_PIN_SCI5_SCK5_PA1 | PDL_SCI_PIN_SCI5_SMISO5_PA2 | PDL_SCI_PIN_SCI5_SMOSI5_PA4
	);

	if( !res ){
		return res;
	}

	return R_SCI_Create(
		5,
		PDL_SCI_SYNC | PDL_SCI_TX_CONNECTED | PDL_SCI_RX_CONNECTED | PDL_SCI_SPI_MODE | PDL_SCI_MSB_FIRST | PDL_SCI_INVERSION_OFF | PDL_SCI_CLK_INT_OUT | PDL_SCI_SPI_SS_DISABLE,
		BIT_31 | PDL_SCI_PCLK_DIV_1 | 0 | (256 & 0x00FFFF00ul),
		0,
		0
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_SPIMode_Transfer_C5(uint8_t * tx_start, uint8_t * rx_start, uint16_t count)
*
* Function Name: R_PG_SCI_SPIMode_Transfer_C5
*
* Description  : �Ȉ�SPI���[�h�̃f�[�^�]��
*
* Arguments    : uint8_t * tx_start : ���M����f�[�^�̐擪�̃A�h���X
*              : uint8_t * rx_start : ��M�����f�[�^�̊i�[��̐擪�̃A�h���X
*              : uint16_t count : �]������f�[�^��
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_SPI_Transfer
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_SPIMode_Transfer_C5(uint8_t * tx_start, uint8_t * rx_start, uint16_t count)
{
	if( (tx_start == 0) || \
		(rx_start == 0) )
	{
		return false;
	}

	return R_SCI_SPI_Transfer(
		5,
		PDL_SCI_SPI_TX_DMAC_DTC_TRIGGER_DISABLE | PDL_SCI_SPI_RX_DMAC_DTC_TRIGGER_DISABLE,
		count,
		tx_start,
		PDL_NO_FUNC,
		rx_start,
		PDL_NO_FUNC,
		PDL_NO_FUNC
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_GetSentDataCount_C5(uint16_t * count)
*
* Function Name: R_PG_SCI_GetSentDataCount_C5
*
* Description  : �V���A���f�[�^�̑��M���擾
*
* Arguments    : uint16_t * count : ���݂̑��M�����ő��M���ꂽ�f�[�^���̊i�[��
*
* Return Value : true  : �擾�ɐ��������ꍇ
*              : false : �擾�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_GetStatus
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_GetSentDataCount_C5(uint16_t * count)
{
	uint8_t status;

	if( count == 0 ){ return false; }

	return R_SCI_GetStatus(
		5,
		&status,
		PDL_NO_PTR,
		count,
		PDL_NO_PTR
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_StopCommunication_C5(void)
*
* Function Name: R_PG_SCI_StopCommunication_C5
*
* Description  : �V���A���f�[�^�̑���M��~
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_StopCommunication_C5(void)
{
	return R_SCI_Control(
		5,
		PDL_SCI_STOP_TX_AND_RX
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_GetReceivedDataCount_C5(uint16_t * count)
*
* Function Name: R_PG_SCI_GetReceivedDataCount_C5
*
* Description  : �V���A���f�[�^�̎�M���擾
*
* Arguments    : uint16_t * count : ���݂̎�M�����Ŏ�M�����f�[�^���̊i�[��
*
* Return Value : true  : �擾�ɐ��������ꍇ
*              : false : �擾�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_GetStatus
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_GetReceivedDataCount_C5(uint16_t * count)
{
	uint8_t status;

	if( count == 0 ){ return false; }

	return R_SCI_GetStatus(
		5,
		&status,
		PDL_NO_PTR,
		PDL_NO_PTR,
		count
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_SPIMode_GetErrorFlag_C5(bool * overrun)
*
* Function Name: R_PG_SCI_SPIMode_GetErrorFlag_C5
*
* Description  : �Ȉ�SPI���[�h�̃V���A����M�G���[�t���O�̎擾
*
* Arguments    : bool * overrun : �I�[�o�����G���[�t���O�̊i�[��
*
* Return Value : true  : �擾�ɐ��������ꍇ
*              : false : �擾�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_GetStatus
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_SPIMode_GetErrorFlag_C5(bool * overrun)
{
	uint8_t status;
	bool res;

	res = R_SCI_GetStatus(
		5,
		&status,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR
	);

	if( overrun ){
		*overrun = (status >> 5) & 0x01;
	}

	return res;
}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_ClearReceptionErrorFlag_C5(void)
*
* Function Name: R_PG_SCI_ClearReceptionErrorFlag_C5
*
* Description  : �V���A����M�G���[�t���O�̃N���A
*
* Arguments    : �Ȃ�
*
* Return Value : true  : �N���A�ɐ��������ꍇ
*              : false : �N���A�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_Control
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_ClearReceptionErrorFlag_C5(void)
{
	return R_SCI_Control(
		5,
		PDL_SCI_CLEAR_RECEIVE_ERROR_FLAGS
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_GetTransmitStatus_C5(bool * complete)
*
* Function Name: R_PG_SCI_GetTransmitStatus_C5
*
* Description  : �V���A���f�[�^���M��Ԃ̎擾
*
* Arguments    : bool * complete : ���M�I���t���O�i�[��
*
* Return Value : true  : �擾�ɐ��������ꍇ
*              : false : �擾�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_GetStatus
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_GetTransmitStatus_C5(bool * complete)
{
	uint8_t status;
	bool res;

	res = R_SCI_GetStatus(
		5,
		&status,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR
	);

	if( complete ){ *complete = (status >> 2) & 0x01; }

	return res;
}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_StopModule_C5(void)
*
* Function Name: R_PG_SCI_StopModule_C5
*
* Description  : �V���A��I/O�`���l���̒�~
*
* Arguments    : �Ȃ�
*
* Return Value : true  : ��~�ɐ��������ꍇ
*              : false : ��~�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_Destroy
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_StopModule_C5(void)
{
	return R_SCI_Destroy( 5 );

}



