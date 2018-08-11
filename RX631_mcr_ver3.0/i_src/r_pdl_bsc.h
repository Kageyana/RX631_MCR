/*""FILE COMMENT""*************************************************************
* System Name	: BSC API for RX63Nxx
* File Name		: r_pdl_bsc.h
* Version		: 1.21
* Contents		: BSC API header
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

#ifndef R_PDL_BSC_H
#define R_PDL_BSC_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_BSC_Set(
	uint16_t
);
bool R_BSC_Create(
	uint16_t,
	uint32_t,
	uint16_t,
	uint8_t,
	void *,
	uint8_t
);
bool R_BSC_CreateArea(
	uint8_t,
	uint16_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t
);

bool R_BSC_SDRAM_CreateArea(
	uint16_t,
	uint16_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint16_t
);
bool R_BSC_Destroy(
	uint8_t
);

bool R_BSC_Control(
	uint16_t
);
bool R_BSC_GetStatus(
	uint8_t *,
	uint16_t *,
	uint8_t *
);

/* Bus priority selection */
#define PDL_BSC_PRIORITY_RAM_MB2	0x0001u
#define PDL_BSC_PRIORITY_RAM_CPU	0x0002u
#define PDL_BSC_PRIORITY_ROM_MB2	0x0004u
#define PDL_BSC_PRIORITY_ROM_CPU	0x0008u
#define PDL_BSC_PRIORITY_PB1_MB2	0x0010u
#define PDL_BSC_PRIORITY_PB1_MB1	0x0020u
#define PDL_BSC_PRIORITY_PB23_MB2	0x0040u
#define PDL_BSC_PRIORITY_PB23_MB1	0x0080u
/*Peripheral Buses 4 and 5:*/
/*Fixed Priority: Main Bus 2 has priority over Main Bus 1.*/
#define PDL_BSC_PRIORITY_PB45_MB2	0x0100u
/*Priority toggles: After bus is accepted its priority is set lower.*/
#define PDL_BSC_PRIORITY_PB45_MB1	0x0200u
#define PDL_BSC_PRIORITY_PB6_MB2	0x0400u
#define PDL_BSC_PRIORITY_PB6_MB1	0x0800u
#define PDL_BSC_PRIORITY_EB_MB2		0x1000u
#define PDL_BSC_PRIORITY_EB_MB1		0x2000u

/* Chip select pin selection */
#define PDL_BSC_CS0_P60			0x00000001ul			
#define PDL_BSC_CS0_PC7			0x00000002ul
#define PDL_BSC_CS1_P61			0x00000004ul
#define PDL_BSC_CS1_P71			0x00000008ul
#define PDL_BSC_CS1_PC6			0x00000010ul
#define PDL_BSC_CS2_P62			0x00000020ul
#define PDL_BSC_CS2_P72			0x00000040ul
#define PDL_BSC_CS2_PC5			0x00000080ul
#define PDL_BSC_CS3_P63			0x00000100ul
#define PDL_BSC_CS3_P73			0x00000200ul
#define PDL_BSC_CS3_PC4			0x00000400ul
#define PDL_BSC_CS4_P64			0x00000800ul	
#define PDL_BSC_CS4_P74			0x00001000ul
#define PDL_BSC_CS4_P24			0x00002000ul
#define PDL_BSC_CS5_P65			0x00004000ul
#define PDL_BSC_CS5_P75			0x00008000ul
#define PDL_BSC_CS5_P25			0x00010000ul
#define PDL_BSC_CS6_P66			0x00020000ul
#define PDL_BSC_CS6_P76			0x00040000ul
#define PDL_BSC_CS6_P26			0x00080000ul
#define PDL_BSC_CS7_P67			0x00100000ul
#define PDL_BSC_CS7_P77			0x00200000ul
#define PDL_BSC_CS7_P27			0x00400000ul

/*Wait pin*/
#define PDL_BSC_WAIT_P57		0x00800000ul
#define PDL_BSC_WAIT_P55		0x01000000ul
#define PDL_BSC_WAIT_PC5		0x02000000ul
#define PDL_BSC_WAIT_P51		0x04000000ul
#define PDL_BSC_ALE_ENABLE		0x08000000ul

/*A16 to A23 pin selection (Use Port C or Port 9)*/
#define PDL_BSC_A16_A23_PC		0x10000000ul
#define PDL_BSC_A16_A23_P9		0x20000000ul

/* Address output control */        	
#define PDL_BSC_A7_A0_DISABLE	0x00000001ul
#define PDL_BSC_A8_DISABLE		0x00000002ul
#define PDL_BSC_A9_DISABLE		0x00000004ul
#define PDL_BSC_A10_DISABLE		0x00000008ul
#define PDL_BSC_A11_DISABLE		0x00000010ul
#define PDL_BSC_A12_DISABLE		0x00000020ul
#define PDL_BSC_A13_DISABLE		0x00000040ul
#define PDL_BSC_A14_DISABLE		0x00000080ul
#define PDL_BSC_A15_DISABLE		0x00000100ul
#define PDL_BSC_A16_DISABLE		0x00000200ul
#define PDL_BSC_A17_DISABLE		0x00000400ul
#define PDL_BSC_A18_DISABLE		0x00000800ul
#define PDL_BSC_A19_DISABLE		0x00001000ul
#define PDL_BSC_A20_DISABLE		0x00002000ul
#define PDL_BSC_A21_DISABLE		0x00004000ul
#define PDL_BSC_A22_DISABLE		0x00008000ul
#define PDL_BSC_A23_DISABLE		0x00010000ul
#define PDL_BSC_A23_A16_DISABLE	( \
PDL_BSC_A16_DISABLE | \
PDL_BSC_A17_DISABLE | \
PDL_BSC_A18_DISABLE | \
PDL_BSC_A19_DISABLE | \
PDL_BSC_A20_DISABLE | \
PDL_BSC_A21_DISABLE | \
PDL_BSC_A22_DISABLE | \
PDL_BSC_A23_DISABLE)

/* SDRAM output control */            	
#define PDL_BSC_SDRAM_PINS_DISABLE		0x00020000ul
#define PDL_BSC_SDRAM_PINS_ENABLE		0x00040000ul
#define PDL_BSC_SDRAM_DQM1_DISABLE		0x00080000ul
#define PDL_BSC_SDRAM_DQM1_ENABLE		0x00100000ul

/* Recovery cycle insertion control */        	
#define PDL_BSC_RCV_SRRS_ENABLE		0x0001u
#define PDL_BSC_RCV_SRRD_ENABLE		0x0002u
#define PDL_BSC_RCV_SRWS_ENABLE		0x0004u
#define PDL_BSC_RCV_SRWD_ENABLE		0x0008u
#define PDL_BSC_RCV_SWRS_ENABLE		0x0010u
#define PDL_BSC_RCV_SWRD_ENABLE		0x0020u
#define PDL_BSC_RCV_SWWS_ENABLE		0x0040u
#define PDL_BSC_RCV_SWWD_ENABLE		0x0080u
#define PDL_BSC_RCV_MRRS_ENABLE		0x0100u
#define PDL_BSC_RCV_MRRD_ENABLE		0x0200u
#define PDL_BSC_RCV_MRWS_ENABLE		0x0400u
#define PDL_BSC_RCV_MRWD_ENABLE		0x0800u
#define PDL_BSC_RCV_MWRS_ENABLE		0x1000u
#define PDL_BSC_RCV_MWRD_ENABLE		0x2000u
#define PDL_BSC_RCV_MWWS_ENABLE		0x4000u
#define PDL_BSC_RCV_MWWD_ENABLE		0x8000u

/* Error monitoring */
#define PDL_BSC_ERROR_ILLEGAL_ADDRESS_ENABLE	0x01u
#define PDL_BSC_ERROR_ILLEGAL_ADDRESS_DISABLE	0x02u
#define PDL_BSC_ERROR_TIME_OUT_ENABLE			0x04u
#define PDL_BSC_ERROR_TIME_OUT_DISABLE			0x08u

/* Data Bus width */
#define PDL_BSC_WIDTH_8					0x0001u
#define PDL_BSC_WIDTH_16				0x0002u
#define PDL_BSC_WIDTH_32				0x0004u

/* Endian mode */
#define PDL_BSC_ENDIAN_SAME				0x0008u
#define PDL_BSC_ENDIAN_OPPOSITE			0x0010u

/* Multiplexed mode */
#define PDL_BSC_SEPARATE				0x0020u
#define PDL_BSC_MULTIPLEXED				0x0040u

/* Write access mode */
#define PDL_BSC_WRITE_BYTE				0x0080u
#define PDL_BSC_WRITE_SINGLE			0x0100u

/* External wait control */
#define PDL_BSC_WAIT_DISABLE			0x0200u
#define PDL_BSC_WAIT_ENABLE				0x0400u

/* Page access control */
#define PDL_BSC_PAGE_READ_DISABLE		0x0800u
#define PDL_BSC_PAGE_READ_NORMAL		0x1000u
#define PDL_BSC_PAGE_READ_CONTINUOUS	0x2000u
#define PDL_BSC_PAGE_WRITE_DISABLE		0x4000u
#define PDL_BSC_PAGE_WRITE_ENABLE		0x8000u

/* R_BSC_Control parameter options */
#define PDL_BSC_ENABLE						0x01u
#define PDL_BSC_DISABLE						0x02u
#define PDL_BSC_ERROR_CLEAR	 				0x04u
#define PDL_BSC_DISABLE_BUSERR_IRQ			0x08u
/* R_BSC_Control SDRAM parameter options */
#define PDL_BSC_SDRAM_INITIALIZATION		0x0010u
#define PDL_BSC_SDRAM_AUTO_REFRESH_ENABLE	0x0020u
#define PDL_BSC_SDRAM_AUTO_REFRESH_DISABLE	0x0040u
#define PDL_BSC_SDRAM_SELF_REFRESH_ENABLE	0x0080u
#define PDL_BSC_SDRAM_SELF_REFRESH_DISABLE	0x0100u
#define PDL_BSC_SDRAM_ENABLE				0x0200u
#define PDL_BSC_SDRAM_DISABLE				0x0400u

/* R_BSC_SDRAM_CreateArea parameter options */
/* Configuration selection */
/* Bus width */
#define PDL_BSC_SDRAM_WIDTH_16				0x0001u
#define PDL_BSC_SDRAM_WIDTH_8				0x0002u
#define PDL_BSC_SDRAM_WIDTH_32				0x0004u

/* Endian mode */
#define PDL_BSC_SDRAM_ENDIAN_SAME			0x0008u
#define PDL_BSC_SDRAM_ENDIAN_OPPOSITE		0x0010u

/* Continuous access mode */
#define PDL_BSC_SDRAM_CONT_ACCESS_ENABLE	0x0020u
#define PDL_BSC_SDRAM_CONT_ACCESS_DISABLE	0x0040u

/* Address multiplex selection */
#define PDL_BSC_SDRAM_8_BIT_SHIFT			0x0080u
#define PDL_BSC_SDRAM_9_BIT_SHIFT			0x0100u
#define PDL_BSC_SDRAM_10_BIT_SHIFT			0x0200u
#define PDL_BSC_SDRAM_11_BIT_SHIFT			0x0400u

#endif
/* End of file */
