/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_P0.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 17.05.2022 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_IO_PORT_Set_P0(void);
bool R_PG_IO_PORT_Set_P05(void);
bool R_PG_IO_PORT_Read_P0(uint8_t * data);
bool R_PG_IO_PORT_Read_P05(uint8_t * data);
bool R_PG_IO_PORT_Read_P07(uint8_t * data);
bool R_PG_IO_PORT_Write_P0(uint8_t data);
bool R_PG_IO_PORT_Write_P05(uint8_t data);



