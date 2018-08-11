/*""FILE COMMENT""*************************************************************
* System Name	: PPG API for RX63N
* File Name		: r_pdl_PPG.h
* Version		: 1.21
* Contents		: PPG API header
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

#ifndef R_PDL_PPG_H
#define R_PDL_PPG_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_PPG_Create(
	uint32_t,
	uint16_t,
	uint8_t
);
bool R_PPG_Destroy(
	uint32_t
);
bool R_PPG_Control(
	uint32_t,
	uint8_t
);

/* Output pin selection */
/* 								group         popfs_selection(bit19~16) pin_selection(bit7~0) */
#define PDL_PPG_PO0_PIN_P20     (PDL_PPG_GROUP_0 | 0x000000 | 0x01u)
#define PDL_PPG_PO1_PIN_P21     (PDL_PPG_GROUP_0 | 0x000000 | 0x02u)
#define PDL_PPG_PO2_PIN_P22     (PDL_PPG_GROUP_0 | 0x000000 | 0x04u)
#define PDL_PPG_PO3_PIN_P23     (PDL_PPG_GROUP_0 | 0x000000 | 0x08u)
#define PDL_PPG_PO4_PIN_P24     (PDL_PPG_GROUP_1 | 0x000000 | 0x10u)
#define PDL_PPG_PO5_PIN_P25     (PDL_PPG_GROUP_1 | 0x000000 | 0x20u)
#define PDL_PPG_PO6_PIN_P26     (PDL_PPG_GROUP_1 | 0x000000 | 0x40u)
#define PDL_PPG_PO7_PIN_P27     (PDL_PPG_GROUP_1 | 0x000000 | 0x80u)
#define PDL_PPG_PO8_PIN_P30     (PDL_PPG_GROUP_2 | 0x000000 | 0x01u)
#define PDL_PPG_PO9_PIN_P31     (PDL_PPG_GROUP_2 | 0x000000 | 0x02u)
#define PDL_PPG_PO10_PIN_P32    (PDL_PPG_GROUP_2 | 0x000000 | 0x04u)
#define PDL_PPG_PO11_PIN_P33    (PDL_PPG_GROUP_2 | 0x000000 | 0x08u)
#define PDL_PPG_PO12_PIN_P34    (PDL_PPG_GROUP_3 | 0x000000 | 0x10u)
#define PDL_PPG_PO13_PIN_P13    (PDL_PPG_GROUP_3 | 0x000000 | 0x20u)
#define PDL_PPG_PO13_PIN_P15    (PDL_PPG_GROUP_3 | 0x040000 | 0x20u)
#define PDL_PPG_PO14_PIN_P16    (PDL_PPG_GROUP_3 | 0x000000 | 0x40u)
#define PDL_PPG_PO15_PIN_P14    (PDL_PPG_GROUP_3 | 0x000000 | 0x80u)
#define PDL_PPG_PO15_PIN_P17    (PDL_PPG_GROUP_3 | 0x400000 | 0x80u)
#define PDL_PPG_PO16_PIN_P73    (PDL_PPG_GROUP_4 | 0x000000 | 0x01u)
#define PDL_PPG_PO16_PIN_PA0    (PDL_PPG_GROUP_4 | 0x010000 | 0x01u)
#define PDL_PPG_PO17_PIN_PA1    (PDL_PPG_GROUP_4 | 0x000000 | 0x02u)
#define PDL_PPG_PO17_PIN_PC0    (PDL_PPG_GROUP_4 | 0x040000 | 0x02u)
#define PDL_PPG_PO18_PIN_PA2    (PDL_PPG_GROUP_4 | 0x000000 | 0x04u)
#define PDL_PPG_PO18_PIN_PC1    (PDL_PPG_GROUP_4 | 0x100000 | 0x04u)
#define PDL_PPG_PO18_PIN_PE1    (PDL_PPG_GROUP_4 | 0x200000 | 0x04u)
#define PDL_PPG_PO19_PIN_P74    (PDL_PPG_GROUP_4 | 0x000000 | 0x08u)
#define PDL_PPG_PO19_PIN_PA3    (PDL_PPG_GROUP_4 | 0x400000 | 0x08u)
#define PDL_PPG_PO20_PIN_P75    (PDL_PPG_GROUP_5 | 0x000000 | 0x10u)
#define PDL_PPG_PO20_PIN_PA4    (PDL_PPG_GROUP_5 | 0x010000 | 0x10u)
#define PDL_PPG_PO21_PIN_PA5    (PDL_PPG_GROUP_5 | 0x000000 | 0x20u)
#define PDL_PPG_PO21_PIN_PC2    (PDL_PPG_GROUP_5 | 0x040000 | 0x20u)
#define PDL_PPG_PO22_PIN_P76    (PDL_PPG_GROUP_5 | 0x000000 | 0x40u)
#define PDL_PPG_PO22_PIN_PA6    (PDL_PPG_GROUP_5 | 0x100000 | 0x40u)
#define PDL_PPG_PO23_PIN_P77    (PDL_PPG_GROUP_5 | 0x000000 | 0x80u)
#define PDL_PPG_PO23_PIN_PA7    (PDL_PPG_GROUP_5 | 0x400000 | 0x80u)
#define PDL_PPG_PO23_PIN_PE2    (PDL_PPG_GROUP_5 | 0x800000 | 0x80u)
#define PDL_PPG_PO24_PIN_PB0    (PDL_PPG_GROUP_6 | 0x000000 | 0x01u)
#define PDL_PPG_PO24_PIN_PC3    (PDL_PPG_GROUP_6 | 0x010000 | 0x01u)
#define PDL_PPG_PO25_PIN_PB1    (PDL_PPG_GROUP_6 | 0x000000 | 0x02u)
#define PDL_PPG_PO25_PIN_PC4    (PDL_PPG_GROUP_6 | 0x040000 | 0x02u)
#define PDL_PPG_PO26_PIN_P80    (PDL_PPG_GROUP_6 | 0x000000 | 0x04u)
#define PDL_PPG_PO26_PIN_PB2    (PDL_PPG_GROUP_6 | 0x100000 | 0x04u)
#define PDL_PPG_PO26_PIN_PE3    (PDL_PPG_GROUP_6 | 0x200000 | 0x04u)
#define PDL_PPG_PO27_PIN_P81    (PDL_PPG_GROUP_6 | 0x000000 | 0x08u)
#define PDL_PPG_PO27_PIN_PB3    (PDL_PPG_GROUP_6 | 0x400000 | 0x08u)
#define PDL_PPG_PO28_PIN_P82    (PDL_PPG_GROUP_7 | 0x000000 | 0x10u)
#define PDL_PPG_PO28_PIN_PB4    (PDL_PPG_GROUP_7 | 0x010000 | 0x10u)
#define PDL_PPG_PO28_PIN_PE4    (PDL_PPG_GROUP_7 | 0x020000 | 0x10u)
#define PDL_PPG_PO29_PIN_PB5    (PDL_PPG_GROUP_7 | 0x000000 | 0x20u)
#define PDL_PPG_PO29_PIN_PC5    (PDL_PPG_GROUP_7 | 0x040000 | 0x20u)
#define PDL_PPG_PO30_PIN_PB6    (PDL_PPG_GROUP_7 | 0x000000 | 0x40u)
#define PDL_PPG_PO30_PIN_PC6    (PDL_PPG_GROUP_7 | 0x100000 | 0x40u)
#define PDL_PPG_PO31_PIN_PB7    (PDL_PPG_GROUP_7 | 0x000000 | 0x80u)
#define PDL_PPG_PO31_PIN_PC7    (PDL_PPG_GROUP_7 | 0x400000 | 0x80u)

/* Output trigger selection */
#define PDL_PPG_TRIGGER_MTU0	0x0001u
#define PDL_PPG_TRIGGER_MTU1	0x0002u
#define PDL_PPG_TRIGGER_MTU2	0x0004u
#define PDL_PPG_TRIGGER_MTU3	0x0008u
#define PDL_PPG_TRIGGER_TPU0	0x0010u
#define PDL_PPG_TRIGGER_TPU1	0x0020u
#define PDL_PPG_TRIGGER_TPU2	0x0040u
#define PDL_PPG_TRIGGER_TPU3	0x0080u

/* Non-overlap control */
#define PDL_PPG_NORMAL			0x0100u
#define PDL_PPG_NON_OVERLAP		0x0200u

/* Invert control */
#define PDL_PPG_DIRECT			0x0400u
#define PDL_PPG_INVERT			0x0800u

/* Group selection */
#define PDL_PPG_GROUP_0			0x00000100u
#define PDL_PPG_GROUP_1			0x00000200u
#define PDL_PPG_GROUP_2			0x00000400u
#define PDL_PPG_GROUP_3			0x00000800u
#define PDL_PPG_GROUP_4			0x01001000u
#define PDL_PPG_GROUP_5			0x01002000u
#define PDL_PPG_GROUP_6			0x01004000u
#define PDL_PPG_GROUP_7			0x01008000u
#endif
/* End of file */
