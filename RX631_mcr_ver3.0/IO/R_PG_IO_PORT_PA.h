/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_PA.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 15.02.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

bool R_PG_IO_PORT_Set_PA(void);
bool R_PG_IO_PORT_Set_PA0(void);
bool R_PG_IO_PORT_Set_PA6(void);
bool R_PG_IO_PORT_Read_PA(uint8_t * data);
bool R_PG_IO_PORT_Read_PA0(uint8_t * data);
bool R_PG_IO_PORT_Read_PA1(uint8_t * data);
bool R_PG_IO_PORT_Read_PA2(uint8_t * data);
bool R_PG_IO_PORT_Read_PA3(uint8_t * data);
bool R_PG_IO_PORT_Read_PA4(uint8_t * data);
bool R_PG_IO_PORT_Read_PA5(uint8_t * data);
bool R_PG_IO_PORT_Read_PA6(uint8_t * data);
bool R_PG_IO_PORT_Read_PA7(uint8_t * data);
bool R_PG_IO_PORT_Write_PA(uint8_t data);
bool R_PG_IO_PORT_Write_PA0(uint8_t data);
bool R_PG_IO_PORT_Write_PA6(uint8_t data);



