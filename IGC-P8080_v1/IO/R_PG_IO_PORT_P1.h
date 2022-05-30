/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_P1.h
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

bool R_PG_IO_PORT_Set_P1(void);
bool R_PG_IO_PORT_Set_P12(void);
bool R_PG_IO_PORT_Set_P13(void);
bool R_PG_IO_PORT_Set_P17(void);
bool R_PG_IO_PORT_Read_P1(uint8_t * data);
bool R_PG_IO_PORT_Read_P12(uint8_t * data);
bool R_PG_IO_PORT_Read_P13(uint8_t * data);
bool R_PG_IO_PORT_Read_P14(uint8_t * data);
bool R_PG_IO_PORT_Read_P15(uint8_t * data);
bool R_PG_IO_PORT_Read_P16(uint8_t * data);
bool R_PG_IO_PORT_Read_P17(uint8_t * data);
bool R_PG_IO_PORT_Write_P1(uint8_t data);
bool R_PG_IO_PORT_Write_P12(uint8_t data);
bool R_PG_IO_PORT_Write_P13(uint8_t data);
bool R_PG_IO_PORT_Write_P17(uint8_t data);



