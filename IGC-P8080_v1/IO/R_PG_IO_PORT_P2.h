/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_P2.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 19.02.2020 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_IO_PORT_Set_P2(void);
bool R_PG_IO_PORT_Set_P22(void);
bool R_PG_IO_PORT_Set_P23(void);
bool R_PG_IO_PORT_Set_P27(void);
bool R_PG_IO_PORT_Read_P2(uint8_t * data);
bool R_PG_IO_PORT_Read_P20(uint8_t * data);
bool R_PG_IO_PORT_Read_P21(uint8_t * data);
bool R_PG_IO_PORT_Read_P22(uint8_t * data);
bool R_PG_IO_PORT_Read_P23(uint8_t * data);
bool R_PG_IO_PORT_Read_P24(uint8_t * data);
bool R_PG_IO_PORT_Read_P25(uint8_t * data);
bool R_PG_IO_PORT_Read_P26(uint8_t * data);
bool R_PG_IO_PORT_Read_P27(uint8_t * data);
bool R_PG_IO_PORT_Write_P2(uint8_t data);
bool R_PG_IO_PORT_Write_P22(uint8_t data);
bool R_PG_IO_PORT_Write_P23(uint8_t data);
bool R_PG_IO_PORT_Write_P27(uint8_t data);



