/*""FILE COMMENT""*************************************************************
* System Name	: Frequency measurement API for RX63Nxx
* File Name		: r_pdl_mck_RX63Nxx.h
* Version		: 1.21
* Contents		: MCK header
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

#ifndef R_PDL_MCK_RX63Nxx_H
#define R_PDL_MCK_RX63Nxx_H

/* Library prototypes */

bool R_MCK_ControlAll(
	const uint16_t
);

/* Macro definitions */

#define R_MCK_Control(a) \
R_MCK_ControlAll( (a) )

#endif
/* End of file */
