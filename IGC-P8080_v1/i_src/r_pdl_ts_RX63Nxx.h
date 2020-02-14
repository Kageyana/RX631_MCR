/*""FILE COMMENT""*************************************************************
* System Name	: Temperature Sensor API for RX63Nxx
* File Name		: r_pdl_ts_RX63Nxx.h
* Version		: 1.21
* Contents		: TS header
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

#ifndef R_PDL_TS_RX63Nxx_H
#define R_PDL_TS_RX63Nxx_H

/* Library prototypes */
bool R_TS_CreateAll(
	void
);
bool R_TS_ControlAll(
	const uint8_t 
);
bool R_TS_DestroyAll(
	void
);

/* Macro definitions */
#define R_TS_Create() \
( \
R_TS_CreateAll() \
)

#define R_TS_Control(a) \
( \
R_TS_ControlAll(a) \
)

#define R_TS_Destroy() \
( \
R_TS_DestroyAll() \
)

#endif
/* End of file */
