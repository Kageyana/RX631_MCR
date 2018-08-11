/*""FILE COMMENT""*************************************************************
* System Name	: MCU API for RX63Nxx
* File Name		: r_pdl_mcu.h
* Version		: 1.21
* Contents		: MCU API header
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

#ifndef R_PDL_MCU_H
#define R_PDL_MCU_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_MCU_Control(
	uint8_t
);
bool R_MCU_GetStatus(
	uint16_t *,
	uint16_t *,
	uint32_t *,
	uint32_t *
);

/* On-chip ROM control */
#define PDL_MCU_ROM_ENABLE	0x01u
#define PDL_MCU_ROM_DISABLE	0x02u

/* Software reset control */
#define PDL_MCU_RESET_START	0x10u

/* Start type flag control */
#define PDL_MCU_WARM_START	0x20u

#endif
/* End of file */
