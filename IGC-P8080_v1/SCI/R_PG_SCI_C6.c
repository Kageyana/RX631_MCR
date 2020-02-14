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
* File Name    : R_PG_SCI_C6.c
* Version      : 1.00
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
* Limitations  : 
******************************************************************************
* History : 15.02.2020 Version Description
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
* Declaration  : bool R_PG_SCI_Set_C6(void)
*
* Function Name: R_PG_SCI_Set_C6
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
bool R_PG_SCI_Set_C6(void)
{
	bool res;

	res = R_SCI_Set(
		6,
		PDL_SCI_PIN_SCI6_SSCL6_P33 | PDL_SCI_PIN_SCI6_SSDA6_P32
	);

	if( !res ){
		return res;
	}

	return R_SCI_Create(
		6,
		PDL_SCI_SYNC | PDL_SCI_TX_CONNECTED | PDL_SCI_RX_CONNECTED | PDL_SCI_IIC_MODE | PDL_SCI_IIC_FILTER_DISABLED | PDL_SCI_IIC_DELAY_SDA_0_1,
		BIT_31 | PDL_SCI_PCLK_DIV_1 | 3 | (256 & 0x00FFFF00ul),
		0,
		0
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_I2CMode_Send_C6(bool addr_10bit, uint16_t slave, uint8_t * data, uint16_t count)
*
* Function Name: R_PG_SCI_I2CMode_Send_C6
*
* Description  : �Ȉ�I2C���[�h�̃f�[�^���M
*
* Arguments    : bool addr_10bit : �X���[�u�A�h���X�t�H�[�}�b�g (1:10�r�b�g  0:7�r�b�g)
*              : uint16_t slave : �X���[�u�A�h���X
*              : uint8_t * data : ���M����f�[�^�̊i�[��̐擪�̃A�h���X
*              : uint16_t count : ���M����f�[�^��
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_IIC_Write
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_I2CMode_Send_C6(bool addr_10bit, uint16_t slave, uint8_t * data, uint16_t count)
{
	uint16_t addr_size = PDL_SCI_IIC_7_BIT_SLAVE_ADDRESS;

	if( addr_10bit )
	{
		addr_size = PDL_SCI_IIC_10_BIT_SLAVE_ADDRESS;
	}
	
	if( data == 0 ){ return false; }

	return R_SCI_IIC_Write(
		6,
		PDL_SCI_IIC_DMAC_DTC_TRIGGER_DISABLE | addr_size,
		slave,
		count,
		data,
		PDL_NO_FUNC
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_I2CMode_SendWithoutStop_C6(bool addr_10bit, uint16_t slave, uint8_t * data, uint16_t count)
*
* Function Name: R_PG_SCI_I2CMode_SendWithoutStop_C6
*
* Description  : �Ȉ�I2C���[�h�̃f�[�^���M(STOP��������)
*
* Arguments    : bool addr_10bit : �X���[�u�A�h���X�t�H�[�}�b�g (1:10�r�b�g  0:7�r�b�g)
*              : uint16_t slave : �X���[�u�A�h���X
*              : uint8_t * data : ���M����f�[�^�̊i�[��̐擪�̃A�h���X
*              : uint16_t count : ���M����f�[�^��
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_IIC_Write
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_I2CMode_SendWithoutStop_C6(bool addr_10bit, uint16_t slave, uint8_t * data, uint16_t count)
{
	uint16_t addr_size = PDL_SCI_IIC_7_BIT_SLAVE_ADDRESS;

	if( addr_10bit )
	{
		addr_size = PDL_SCI_IIC_10_BIT_SLAVE_ADDRESS;
	}
	
	if( data == 0 ){ return false; }

	return R_SCI_IIC_Write(
		6,
		PDL_SCI_IIC_DMAC_DTC_TRIGGER_DISABLE | addr_size | PDL_SCI_IIC_NOSTOP,
		slave,
		count,
		data,
		PDL_NO_FUNC
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_I2CMode_GenerateStopCondition_C6(void)
*
* Function Name: R_PG_SCI_I2CMode_GenerateStopCondition_C6
*
* Description  : STOP�����̐���
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
bool R_PG_SCI_I2CMode_GenerateStopCondition_C6(void)
{
	return R_SCI_Control(
		6,
		PDL_SCI_IIC_STOP
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_GetSentDataCount_C6(uint16_t * count)
*
* Function Name: R_PG_SCI_GetSentDataCount_C6
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
bool R_PG_SCI_GetSentDataCount_C6(uint16_t * count)
{
	uint8_t status;

	if( count == 0 ){ return false; }

	return R_SCI_GetStatus(
		6,
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
* Declaration  : bool R_PG_SCI_I2CMode_Receive_C6(bool addr_10bit, uint16_t slave, uint8_t * data, uint16_t count)
*
* Function Name: R_PG_SCI_I2CMode_Receive_C6
*
* Description  : �Ȉ�I2C���[�h�̃f�[�^��M
*
* Arguments    : bool addr_10bit : �X���[�u�A�h���X�t�H�[�}�b�g (1:10�r�b�g  0:7�r�b�g)
*              : uint16_t slave : �X���[�u�A�h���X
*              : uint8_t * data : ��M�����f�[�^�̊i�[��̐擪�̃A�h���X
*              : uint16_t count : ��M����f�[�^��
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_IIC_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_I2CMode_Receive_C6(bool addr_10bit, uint16_t slave, uint8_t * data, uint16_t count)
{
	uint16_t addr_size = PDL_SCI_IIC_7_BIT_SLAVE_ADDRESS;

	if( addr_10bit )
	{
		addr_size = PDL_SCI_IIC_10_BIT_SLAVE_ADDRESS;
	}
	
	if( data == 0 ){ return false; }

	return R_SCI_IIC_Read(
		6,
		PDL_SCI_IIC_DMAC_DTC_TRIGGER_DISABLE | addr_size,
		slave,
		count,
		data,
		PDL_NO_FUNC
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_I2CMode_RestartReceive_C6(bool addr_10bit, uint16_t slave, uint8_t * data, uint16_t count)
*
* Function Name: R_PG_SCI_I2CMode_RestartReceive_C6
*
* Description  : �Ȉ�I2C���[�h�̃f�[�^��M(RE-START����)
*
* Arguments    : bool addr_10bit : �X���[�u�A�h���X�t�H�[�}�b�g (1:10�r�b�g  0:7�r�b�g)
*              : uint16_t slave : �X���[�u�A�h���X
*              : uint8_t * data : ��M�����f�[�^�̊i�[��̐擪�̃A�h���X
*              : uint16_t count : ��M����f�[�^��
*
* Return Value : true  : �ݒ肪�������s��ꂽ�ꍇ
*              : false : �ݒ�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_IIC_Read
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_I2CMode_RestartReceive_C6(bool addr_10bit, uint16_t slave, uint8_t * data, uint16_t count)
{
	uint16_t addr_size = PDL_SCI_IIC_7_BIT_SLAVE_ADDRESS;

	if( addr_10bit )
	{
		addr_size = PDL_SCI_IIC_10_BIT_SLAVE_ADDRESS;
	}
	
	if( data == 0 ){ return false; }

	return R_SCI_IIC_Read(
		6,
		PDL_SCI_IIC_DMAC_DTC_TRIGGER_DISABLE | addr_size | PDL_SCI_IIC_RESTART,
		slave,
		count,
		data,
		PDL_NO_FUNC
	);

}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_GetReceivedDataCount_C6(uint16_t * count)
*
* Function Name: R_PG_SCI_GetReceivedDataCount_C6
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
bool R_PG_SCI_GetReceivedDataCount_C6(uint16_t * count)
{
	uint8_t status;

	if( count == 0 ){ return false; }

	return R_SCI_GetStatus(
		6,
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
* Declaration  : bool R_PG_SCI_I2CMode_GetEvent_C6(bool * nack)
*
* Function Name: R_PG_SCI_I2CMode_GetEvent_C6
*
* Description  : �Ȉ�I2C���[�h�̌��o�C�x���g�̎擾
*
* Arguments    : bool * nack : NACK���o�t���O�i�[��
*
* Return Value : true  : �擾�ɐ��������ꍇ
*              : false : �擾�Ɏ��s�����ꍇ
*
* Calling Functions : R_SCI_GetStatus
*
* Details      : �ڍׂɂ��Ă̓��t�@�����X�}�j���A�����Q�Ƃ��Ă��������B
******************************************************************************/
bool R_PG_SCI_I2CMode_GetEvent_C6(bool * nack)
{
	uint8_t status;
	bool res;

	res = R_SCI_GetStatus(
		6,
		&status,
		PDL_NO_PTR,
		PDL_NO_PTR,
		PDL_NO_PTR
	);

	if( nack ){ *nack = status & 0x01; }

	return res;
}

/******************************************************************************
* ID           : 
*
* Include      : 
*
* Declaration  : bool R_PG_SCI_StopModule_C6(void)
*
* Function Name: R_PG_SCI_StopModule_C6
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
bool R_PG_SCI_StopModule_C6(void)
{
	return R_SCI_Destroy( 6 );

}



