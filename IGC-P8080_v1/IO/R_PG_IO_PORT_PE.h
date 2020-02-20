/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010-2013 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_IO_PORT_PE.h
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

bool R_PG_IO_PORT_Set_PE(void);
bool R_PG_IO_PORT_Set_PE1(void);
bool R_PG_IO_PORT_Set_PE3(void);
bool R_PG_IO_PORT_Set_PE4(void);
bool R_PG_IO_PORT_Set_PE5(void);
bool R_PG_IO_PORT_Set_PE6(void);
bool R_PG_IO_PORT_Set_PE7(void);
bool R_PG_IO_PORT_Read_PE(uint8_t * data);
bool R_PG_IO_PORT_Read_PE0(uint8_t * data);
bool R_PG_IO_PORT_Read_PE1(uint8_t * data);
bool R_PG_IO_PORT_Read_PE2(uint8_t * data);
bool R_PG_IO_PORT_Read_PE3(uint8_t * data);
bool R_PG_IO_PORT_Read_PE4(uint8_t * data);
bool R_PG_IO_PORT_Read_PE5(uint8_t * data);
bool R_PG_IO_PORT_Read_PE6(uint8_t * data);
bool R_PG_IO_PORT_Read_PE7(uint8_t * data);
bool R_PG_IO_PORT_Write_PE(uint8_t data);
bool R_PG_IO_PORT_Write_PE1(uint8_t data);
bool R_PG_IO_PORT_Write_PE3(uint8_t data);
bool R_PG_IO_PORT_Write_PE4(uint8_t data);
bool R_PG_IO_PORT_Write_PE5(uint8_t data);
bool R_PG_IO_PORT_Write_PE6(uint8_t data);
bool R_PG_IO_PORT_Write_PE7(uint8_t data);



