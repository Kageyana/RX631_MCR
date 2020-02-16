/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_PB.h
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

bool R_PG_IO_PORT_Set_PB(void);
bool R_PG_IO_PORT_Set_PB0(void);
bool R_PG_IO_PORT_Set_PB2(void);
bool R_PG_IO_PORT_Set_PB4(void);
bool R_PG_IO_PORT_Read_PB(uint8_t * data);
bool R_PG_IO_PORT_Read_PB0(uint8_t * data);
bool R_PG_IO_PORT_Read_PB1(uint8_t * data);
bool R_PG_IO_PORT_Read_PB2(uint8_t * data);
bool R_PG_IO_PORT_Read_PB3(uint8_t * data);
bool R_PG_IO_PORT_Read_PB4(uint8_t * data);
bool R_PG_IO_PORT_Read_PB5(uint8_t * data);
bool R_PG_IO_PORT_Read_PB6(uint8_t * data);
bool R_PG_IO_PORT_Read_PB7(uint8_t * data);
bool R_PG_IO_PORT_Write_PB(uint8_t data);
bool R_PG_IO_PORT_Write_PB0(uint8_t data);
bool R_PG_IO_PORT_Write_PB2(uint8_t data);
bool R_PG_IO_PORT_Write_PB4(uint8_t data);



