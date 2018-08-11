/*""FILE COMMENT""*************************************************************
* System Name	: Voltage detection circuit API for RX63Nxx
* File Name		: r_pdl_lvd.h
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

#ifndef R_PDL_LVD_H
#define R_PDL_LVD_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_LVD_Create(
	uint16_t,
	uint16_t
);
					 
bool R_LVD_Control(
	uint8_t,
	uint8_t
);

bool R_LVD_GetStatus(
	uint8_t *
);

/* Create parameter configuration */

/* Monitor, Reset or Interrupt.
If reset; what type of reset negation.
If interrupt; what type and what to detect.*/
#define PDL_LVD_MONITOR_ONLY						0x0001u
#define PDL_LVD_RESET_NEGATION_VCC_MORE_THAN_VDET	0x0002u
#define PDL_LVD_RESET_NEGATION_AFTER_DELAY			0x0004u
#define PDL_LVD_INTERRUPT_NMI_DETECT_RISE			0x0008u
#define PDL_LVD_INTERRUPT_NMI_DETECT_FALL			0x0010u
#define PDL_LVD_INTERRUPT_NMI_DETECT_RISE_AND_FALL	0x0020u

/* Digital Filter */
#define PDL_LVD_FILTER_DISABLE                      0x0040u
#define PDL_LVD_FILTER_LOCO_DIV_1                   0x0080u
#define PDL_LVD_FILTER_LOCO_DIV_2                   0x0100u
#define PDL_LVD_FILTER_LOCO_DIV_4                   0x0200u
#define PDL_LVD_FILTER_LOCO_DIV_8                   0x0400u

/* VDET1 and VDET2 applicable*/
#define PDL_LVD_DISABLE                             0x01u

/* Flag control */
#define PDL_LVD_CLEAR_DETECTION                     0x02u

#endif
/* End of file */
