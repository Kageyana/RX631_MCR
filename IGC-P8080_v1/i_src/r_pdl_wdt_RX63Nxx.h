/*""FILE COMMENT""*************************************************************
* System Name	: WDT API for RX63Nxx
* File Name		: r_pdl_wdt_RX63Nxx.h
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

#ifndef R_PDL_WDT_RX63Nxx_H
#define R_PDL_WDT_RX63Nxx_H

/* Callback function storage */
extern VoidCallBackFunc rpdl_WDT_callback_func;

/* Library prototypes */
bool R_WDT_SetAll(
	const uint32_t
);
bool R_WDT_ControlAll(
	const uint8_t
);
bool R_WDT_ReadAll(
	volatile uint16_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_WDT_Set(a) \
( \
R_WDT_SetAll( (a) ) \
)

#define R_WDT_Control(a) \
( \
R_WDT_ControlAll( (a) ) \
)

#define R_WDT_Read(a) \
( \
R_WDT_ReadAll( (a) ) \
)

#endif
/* End of file */
