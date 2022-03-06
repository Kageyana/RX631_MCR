/*""FILE COMMENT""*************************************************************
* System Name	: POE API for RX63N
* File Name		: r_pdl_poe_RX63Nxx.h
* Version		: 1.21
* Contents		: POE API header
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

#ifndef R_PDL_POE_RX63Nxx_H
#define R_PDL_POE_RX63Nxx_H

#define POE_INTERRUPTS	2

/* Callback function storage */
extern VoidCallBackFunc rpdl_POE_callback_func[];

/* Library prototypes */
bool R_POE_SetAll(
	const uint32_t,
	const uint16_t,
	const uint16_t
);
bool R_POE_CreateAll(
	const uint8_t,
	VoidCallBackFunc const,
	VoidCallBackFunc const,
	const uint8_t
);
bool R_POE_ControlAll(
	const uint8_t,
	const uint16_t,
	const uint8_t
);
bool R_POE_GetStatusAll(
	volatile uint16_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_POE_Set(a, b, c) \
( \
R_POE_SetAll( (a), (b), (c) ) \
)

#define R_POE_Create(a, b, c, d) \
( \
( ((d) <= IPL_MAX) ) ? \
R_POE_CreateAll( (a), (b), (c), (d)) : \
ReturnFalse() \
)

#define R_POE_Control(a, b, c) \
( \
R_POE_ControlAll( (a), (b), (c) ) \
)

#define R_POE_GetStatus(a) \
( \
R_POE_GetStatusAll( (a) ) \
)

#endif
/* End of file */
