/*""FILE COMMENT""*************************************************************
* System Name	: Register Write Protection API
* File Name		: r_pdl_rwp.h
* Version		: 1.21
* Contents		: RWP header
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

#ifndef R_PDL_RWP_H
#define R_PDL_RWP_H

#include "r_pdl_common_defs_RX63Nxx.h"

bool R_RWP_Control(
	uint8_t
);
bool R_RWP_GetStatus(
	uint8_t *,
	uint8_t *
);

/*Control Flags*/
#define PDL_RWP_ENABLE_CGC_WRITE			0x01u
#define PDL_RWP_DISABLE_CGC_WRITE			0x02u
#define PDL_RWP_ENABLE_MODE_RESET_WRITE		0x04u
#define PDL_RWP_DISABLE_MODE_RESET_WRITE	0x08u
#define PDL_RWP_ENABLE_LVD_WRITE			0x10u
#define PDL_RWP_DISABLE_LVD_WRITE			0x20u
#define PDL_RWP_ENABLE_MPC_WRITE			0x40u
#define PDL_RWP_DISABLE_MPC_WRITE			0x80u

#endif
/* End of file */
