/*""FILE COMMENT""*************************************************************
* System Name	: MCU API for RX63Nxx
* File Name		: r_pdl_mcu_RX63Nxx.h
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

#ifndef R_PDL_MCU_RX63Nxx_H
#define R_PDL_MCU_RX63Nxx_H

/* Library prototypes */
bool R_MCU_ControlAll(
	const uint8_t
);
bool R_MCU_GetStatusAll(
	volatile uint16_t * const,
	volatile uint16_t * const,
	volatile uint32_t * const,
	volatile uint32_t * const
);

/* Macro definitions */
#define R_MCU_Control(a) \
( \
R_MCU_ControlAll( (a) ) \
)

#define R_MCU_GetStatus(a, b, c, d) \
( \
R_MCU_GetStatusAll( (a), (b), (c), (d) ) \
)

#endif
/* End of file */
