/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_P4.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 21.02.2020 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_IO_PORT_Set_P4(void);
bool R_PG_IO_PORT_Set_P41(void);
bool R_PG_IO_PORT_Read_P4(uint8_t * data);
bool R_PG_IO_PORT_Read_P40(uint8_t * data);
bool R_PG_IO_PORT_Read_P41(uint8_t * data);
bool R_PG_IO_PORT_Read_P42(uint8_t * data);
bool R_PG_IO_PORT_Read_P43(uint8_t * data);
bool R_PG_IO_PORT_Read_P44(uint8_t * data);
bool R_PG_IO_PORT_Read_P45(uint8_t * data);
bool R_PG_IO_PORT_Read_P46(uint8_t * data);
bool R_PG_IO_PORT_Read_P47(uint8_t * data);
bool R_PG_IO_PORT_Write_P4(uint8_t data);
bool R_PG_IO_PORT_Write_P41(uint8_t data);



