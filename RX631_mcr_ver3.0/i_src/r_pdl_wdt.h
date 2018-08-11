/*""FILE COMMENT""*************************************************************
* System Name	: WDT API for RX63Nxx
* File Name		: r_pdl_wdt.h
* Version		: 1.21
* Contents		: WDT API header
* Customer		: 
* Model			: 
* Order			: 
* CPU			: RX
* Compiler		: RXC
* OS			: Nothing
* Programmer	: 
* Note			: 
*******************************************************************************
* Copyright, 2014. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FILE COMMENT END""*********************************************************/

#ifndef R_PDL_WDT_H
#define R_PDL_WDT_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_WDT_Set(
	uint32_t
);
bool R_WDT_Control(
	uint8_t
);

bool R_WDT_Read(
	uint16_t *
);

/*** R_WDT_Set: Parameter1 ****/
/* Time-Out Period selection (Cycles)*/
#define PDL_WDT_TIMEOUT_1024	0x00000001u
#define PDL_WDT_TIMEOUT_4096	0x00000002u
#define PDL_WDT_TIMEOUT_8192	0x00000004u
#define PDL_WDT_TIMEOUT_16384	0x00000008u

/* Clock Division Ratio selection*/
#define PDL_WDT_PCLK_DIV_4		0x00000010u
#define PDL_WDT_PCLK_DIV_64		0x00000020u
#define PDL_WDT_PCLK_DIV_128	0x00000040u
#define PDL_WDT_PCLK_DIV_512	0x00000080u
#define PDL_WDT_PCLK_DIV_2048	0x00000100u
#define PDL_WDT_PCLK_DIV_8192	0x00000200u

/*Window Start Position selection */
#define PDL_WDT_WIN_START_25	0x00010000u
#define PDL_WDT_WIN_START_50	0x00020000u
#define PDL_WDT_WIN_START_75	0x00040000u
#define PDL_WDT_WIN_START_100	0x00080000u 

/*Window End Position selection */
#define PDL_WDT_WIN_END_0		0x00100000u
#define PDL_WDT_WIN_END_25		0x00200000u
#define PDL_WDT_WIN_END_50		0x00400000u
#define PDL_WDT_WIN_END_75		0x00800000u

/* Timeout control */
#define PDL_WDT_TIMEOUT_NMI		0x01000000u
#define PDL_WDT_TIMEOUT_RESET	0x02000000u

/*** R_WDT_Control: Parameter1 ****/
#define PDL_WDT_RESET_COUNTER	0x01u

#endif
/* End of file */
