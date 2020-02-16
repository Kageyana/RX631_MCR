/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_ADC_12_S12AD0.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 16.02.2020 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_ADC_12_Set_S12AD0(void);
bool R_PG_ADC_12_StartConversionSW_S12AD0(void);
bool R_PG_ADC_12_StopConversion_S12AD0(void);
bool R_PG_ADC_12_GetResult_S12AD0(uint16_t * result);
bool R_PG_ADC_12_StopModule_S12AD0(void);



