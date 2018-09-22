/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_PD.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 22.09.2018 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_IO_PORT_Set_PD(void);
bool R_PG_IO_PORT_Set_PD0(void);
bool R_PG_IO_PORT_Read_PD(uint8_t * data);
bool R_PG_IO_PORT_Read_PD0(uint8_t * data);
bool R_PG_IO_PORT_Read_PD1(uint8_t * data);
bool R_PG_IO_PORT_Read_PD2(uint8_t * data);
bool R_PG_IO_PORT_Read_PD3(uint8_t * data);
bool R_PG_IO_PORT_Read_PD4(uint8_t * data);
bool R_PG_IO_PORT_Read_PD5(uint8_t * data);
bool R_PG_IO_PORT_Read_PD6(uint8_t * data);
bool R_PG_IO_PORT_Read_PD7(uint8_t * data);
bool R_PG_IO_PORT_Write_PD(uint8_t data);
bool R_PG_IO_PORT_Write_PD0(uint8_t data);



