/*""FILE COMMENT""*************************************************************
* System Name	: Frequency measurement API
* File Name		: r_pdl_mck.h
* Version		: 1.21
* Contents		: MCK header
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

#ifndef R_PDL_MCK_H
#define R_PDL_MCK_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_MCK_Control(
	uint16_t
);

/* Reference clock selection for system 1 */
#define PDL_MCK_1_DISABLE				0x0001u
#define PDL_MCK_1_REFERENCE_MTCLKD		0x0002u
#define PDL_MCK_1_REFERENCE_LOCO		0x0004u
#define PDL_MCK_1_REFERENCE_MAIN		0x0008u
#define PDL_MCK_1_REFERENCE_SUB_CLOCK	0x0010u

/* Reference clock selection for system 2 */
#define PDL_MCK_2_DISABLE				0x0020u
#define PDL_MCK_2_REFERENCE_TCLKD		0x0040u
#define PDL_MCK_2_REFERENCE_LOCO		0x0080u
#define PDL_MCK_2_REFERENCE_MAIN		0x0100u
#define PDL_MCK_2_REFERENCE_SUB_CLOCK	0x0200u

#endif
/* End of file */
