/*""FILE COMMENT""*************************************************************
* System Name	: Low Voltage detection circuit (LVD) API for RX63Nxx
* File Name		: r_pdl_lvd_RX63Nxx.h
* Version		: 1.21
* Contents		: LVD API header
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

#ifndef R_PDL_LVD_RX63Nxx_H
#define R_PDL_LVD_RX63Nxx_H

/* Library prototypes */
bool R_LVD_CreateAll(
	const uint16_t,
	const uint16_t
);
					 
bool R_LVD_ControlAll(
	const uint8_t,
	const uint8_t
);

bool R_LVD_GetStatusAll(
	volatile uint8_t * const
);

/* Macro definitions */
#define R_LVD_Create(a, b) \
( \
R_LVD_CreateAll(a, b) \
)

#define R_LVD_Control(a, b) \
( \
R_LVD_ControlAll( (a), (b) ) \
)

#define R_LVD_GetStatus(a) \
( \
R_LVD_GetStatusAll( (a) ) \
)

#endif
/* End of file */
