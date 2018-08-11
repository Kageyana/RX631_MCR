/*""FILE COMMENT""*************************************************************
* System Name	: CGC API for RX63Nxx
* File Name		: r_pdl_cgc_RX63Nxx.h
* Version		: 1.21
* Contents		: CGC header
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

#ifndef R_PDL_CGC_RX63Nxx_H
#define R_PDL_CGC_RX63Nxx_H

/* Library prototypes */
bool R_CGC_SetAll(
	const uint8_t,
	const uint32_t,
	const double,
	const double,
	const double,
	const double,
	const double,
	const double,
	const double,
	const double,
	const uint16_t
);

bool R_CGC_ControlAll(
	const uint8_t,
	const uint32_t,
	const uint8_t
);

bool R_CGC_GetStatusAll(
	volatile uint16_t * const
);

/* Macro definitions */

#define R_CGC_Set(a, b, c, d, e, f, g, h, i, j, k) \
R_CGC_SetAll( (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k) )

#define R_CGC_Control(a, b, c) \
R_CGC_ControlAll( (a), (b), (c) )

#define R_CGC_GetStatus(a) \
R_CGC_GetStatusAll( (a) )

#endif
/* End of file */
