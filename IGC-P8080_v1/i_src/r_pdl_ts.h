/*""FILE COMMENT""*************************************************************
* System Name	: Temperature Sensor API for RX63Nxx
* File Name		: r_pdl_ts.h
* Version		: 1.21
* Contents		: TS function prototypes
* Customer		:
* Model			:
* Order			:
* CPU			: RX
* Compiler		: RXC
* OS			:
* Programmer	:
* Note			:
*******************************************************************************
* Copyright, 2014. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FILE COMMENT END""*********************************************************/

#ifndef R_PDL_TS_H
#define R_PDL_TS_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_TS_Create(
	void
);
bool R_TS_Control(
	uint8_t
);
bool R_TS_Destroy(
	void
);

/* Temperature Sensor output control */
#define	PDL_TS_OUTPUT_ENABLE	0x01u
#define PDL_TS_OUTPUT_DISABLE	0x02u

#endif
/* End of file */
