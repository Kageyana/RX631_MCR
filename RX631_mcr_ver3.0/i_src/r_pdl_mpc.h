/*""FILE COMMENT""*************************************************************
* System Name	: MPC API
* File Name		: r_pdl_mpc.h
* Version		: 1.21
* Contents		: MPC API header
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

#ifndef R_PDL_MPC_H
#define R_PDL_MPC_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_MPC_Read(
	uint8_t *,
	uint8_t *
);
bool R_MPC_Write(
	uint8_t *,
	uint8_t
);
bool R_MPC_Modify(
	uint8_t *,
	uint8_t,
	uint8_t
);

#define PDL_MPC_REG_P00PFS	(volatile uint8_t *)&MPC.P00PFS
#define PDL_MPC_REG_P01PFS	(volatile uint8_t *)&MPC.P01PFS
#define PDL_MPC_REG_P02PFS	(volatile uint8_t *)&MPC.P02PFS
#define PDL_MPC_REG_P03PFS	(volatile uint8_t *)&MPC.P03PFS
#define PDL_MPC_REG_P05PFS	(volatile uint8_t *)&MPC.P05PFS
#define PDL_MPC_REG_P07PFS	(volatile uint8_t *)&MPC.P07PFS
#define PDL_MPC_REG_P10PFS	(volatile uint8_t *)&MPC.P10PFS
#define PDL_MPC_REG_P11PFS	(volatile uint8_t *)&MPC.P11PFS
#define PDL_MPC_REG_P12PFS	(volatile uint8_t *)&MPC.P12PFS
#define PDL_MPC_REG_P13PFS	(volatile uint8_t *)&MPC.P13PFS
#define PDL_MPC_REG_P14PFS	(volatile uint8_t *)&MPC.P14PFS
#define PDL_MPC_REG_P15PFS	(volatile uint8_t *)&MPC.P15PFS
#define PDL_MPC_REG_P16PFS	(volatile uint8_t *)&MPC.P16PFS
#define PDL_MPC_REG_P17PFS	(volatile uint8_t *)&MPC.P17PFS
#define PDL_MPC_REG_P20PFS	(volatile uint8_t *)&MPC.P20PFS
#define PDL_MPC_REG_P21PFS	(volatile uint8_t *)&MPC.P21PFS
#define PDL_MPC_REG_P22PFS	(volatile uint8_t *)&MPC.P22PFS
#define PDL_MPC_REG_P23PFS	(volatile uint8_t *)&MPC.P23PFS
#define PDL_MPC_REG_P24PFS	(volatile uint8_t *)&MPC.P24PFS
#define PDL_MPC_REG_P25PFS	(volatile uint8_t *)&MPC.P25PFS
#define PDL_MPC_REG_P26PFS	(volatile uint8_t *)&MPC.P26PFS
#define PDL_MPC_REG_P27PFS	(volatile uint8_t *)&MPC.P27PFS
#define PDL_MPC_REG_P30PFS	(volatile uint8_t *)&MPC.P30PFS
#define PDL_MPC_REG_P31PFS	(volatile uint8_t *)&MPC.P31PFS
#define PDL_MPC_REG_P32PFS	(volatile uint8_t *)&MPC.P32PFS
#define PDL_MPC_REG_P33PFS	(volatile uint8_t *)&MPC.P33PFS
#define PDL_MPC_REG_P34PFS	(volatile uint8_t *)&MPC.P34PFS
#define PDL_MPC_REG_P40PFS	(volatile uint8_t *)&MPC.P40PFS
#define PDL_MPC_REG_P41PFS	(volatile uint8_t *)&MPC.P41PFS
#define PDL_MPC_REG_P42PFS	(volatile uint8_t *)&MPC.P42PFS
#define PDL_MPC_REG_P43PFS	(volatile uint8_t *)&MPC.P43PFS
#define PDL_MPC_REG_P44PFS	(volatile uint8_t *)&MPC.P44PFS
#define PDL_MPC_REG_P45PFS	(volatile uint8_t *)&MPC.P45PFS
#define PDL_MPC_REG_P46PFS	(volatile uint8_t *)&MPC.P46PFS
#define PDL_MPC_REG_P47PFS	(volatile uint8_t *)&MPC.P47PFS
#define PDL_MPC_REG_P50PFS	(volatile uint8_t *)&MPC.P50PFS
#define PDL_MPC_REG_P51PFS	(volatile uint8_t *)&MPC.P51PFS
#define PDL_MPC_REG_P52PFS	(volatile uint8_t *)&MPC.P52PFS
#define PDL_MPC_REG_P54PFS	(volatile uint8_t *)&MPC.P54PFS
#define PDL_MPC_REG_P55PFS	(volatile uint8_t *)&MPC.P55PFS
#define PDL_MPC_REG_P56PFS	(volatile uint8_t *)&MPC.P56PFS
#define PDL_MPC_REG_P57PFS	(volatile uint8_t *)&MPC.P57PFS
#define PDL_MPC_REG_P60PFS	(volatile uint8_t *)&MPC.P60PFS
#define PDL_MPC_REG_P61PFS	(volatile uint8_t *)&MPC.P61PFS
#define PDL_MPC_REG_P66PFS	(volatile uint8_t *)&MPC.P66PFS
#define PDL_MPC_REG_P67PFS	(volatile uint8_t *)&MPC.P67PFS
#define PDL_MPC_REG_P70PFS	(volatile uint8_t *)&MPC.P70PFS
#define PDL_MPC_REG_P71PFS	(volatile uint8_t *)&MPC.P71PFS
#define PDL_MPC_REG_P72PFS	(volatile uint8_t *)&MPC.P72PFS
#define PDL_MPC_REG_P73PFS	(volatile uint8_t *)&MPC.P73PFS
#define PDL_MPC_REG_P74PFS	(volatile uint8_t *)&MPC.P74PFS
#define PDL_MPC_REG_P75PFS	(volatile uint8_t *)&MPC.P75PFS
#define PDL_MPC_REG_P76PFS	(volatile uint8_t *)&MPC.P76PFS
#define PDL_MPC_REG_P77PFS	(volatile uint8_t *)&MPC.P77PFS
#define PDL_MPC_REG_P80PFS	(volatile uint8_t *)&MPC.P80PFS
#define PDL_MPC_REG_P81PFS	(volatile uint8_t *)&MPC.P81PFS
#define PDL_MPC_REG_P82PFS	(volatile uint8_t *)&MPC.P82PFS
#define PDL_MPC_REG_P83PFS	(volatile uint8_t *)&MPC.P83PFS
#define PDL_MPC_REG_P86PFS	(volatile uint8_t *)&MPC.P86PFS
#define PDL_MPC_REG_P87PFS	(volatile uint8_t *)&MPC.P87PFS
#define PDL_MPC_REG_P90PFS	(volatile uint8_t *)&MPC.P90PFS
#define PDL_MPC_REG_P91PFS	(volatile uint8_t *)&MPC.P91PFS
#define PDL_MPC_REG_P92PFS	(volatile uint8_t *)&MPC.P92PFS
#define PDL_MPC_REG_P93PFS	(volatile uint8_t *)&MPC.P93PFS
#define PDL_MPC_REG_PA0PFS	(volatile uint8_t *)&MPC.PA0PFS
#define PDL_MPC_REG_PA1PFS	(volatile uint8_t *)&MPC.PA1PFS
#define PDL_MPC_REG_PA2PFS	(volatile uint8_t *)&MPC.PA2PFS
#define PDL_MPC_REG_PA3PFS	(volatile uint8_t *)&MPC.PA3PFS
#define PDL_MPC_REG_PA4PFS	(volatile uint8_t *)&MPC.PA4PFS
#define PDL_MPC_REG_PA5PFS	(volatile uint8_t *)&MPC.PA5PFS
#define PDL_MPC_REG_PA6PFS	(volatile uint8_t *)&MPC.PA6PFS
#define PDL_MPC_REG_PA7PFS	(volatile uint8_t *)&MPC.PA7PFS
#define PDL_MPC_REG_PB0PFS	(volatile uint8_t *)&MPC.PB0PFS
#define PDL_MPC_REG_PB1PFS	(volatile uint8_t *)&MPC.PB1PFS
#define PDL_MPC_REG_PB2PFS	(volatile uint8_t *)&MPC.PB2PFS
#define PDL_MPC_REG_PB3PFS	(volatile uint8_t *)&MPC.PB3PFS
#define PDL_MPC_REG_PB4PFS	(volatile uint8_t *)&MPC.PB4PFS
#define PDL_MPC_REG_PB5PFS	(volatile uint8_t *)&MPC.PB5PFS
#define PDL_MPC_REG_PB6PFS	(volatile uint8_t *)&MPC.PB6PFS
#define PDL_MPC_REG_PB7PFS	(volatile uint8_t *)&MPC.PB7PFS
#define PDL_MPC_REG_PC0PFS	(volatile uint8_t *)&MPC.PC0PFS
#define PDL_MPC_REG_PC1PFS	(volatile uint8_t *)&MPC.PC1PFS
#define PDL_MPC_REG_PC2PFS	(volatile uint8_t *)&MPC.PC2PFS
#define PDL_MPC_REG_PC3PFS	(volatile uint8_t *)&MPC.PC3PFS
#define PDL_MPC_REG_PC4PFS	(volatile uint8_t *)&MPC.PC4PFS
#define PDL_MPC_REG_PC5PFS	(volatile uint8_t *)&MPC.PC5PFS
#define PDL_MPC_REG_PC6PFS	(volatile uint8_t *)&MPC.PC6PFS
#define PDL_MPC_REG_PC7PFS	(volatile uint8_t *)&MPC.PC7PFS
#define PDL_MPC_REG_PD0PFS	(volatile uint8_t *)&MPC.PD0PFS
#define PDL_MPC_REG_PD1PFS	(volatile uint8_t *)&MPC.PD1PFS
#define PDL_MPC_REG_PD2PFS	(volatile uint8_t *)&MPC.PD2PFS
#define PDL_MPC_REG_PD3PFS	(volatile uint8_t *)&MPC.PD3PFS
#define PDL_MPC_REG_PD4PFS	(volatile uint8_t *)&MPC.PD4PFS
#define PDL_MPC_REG_PD5PFS	(volatile uint8_t *)&MPC.PD5PFS
#define PDL_MPC_REG_PD6PFS	(volatile uint8_t *)&MPC.PD6PFS
#define PDL_MPC_REG_PD7PFS	(volatile uint8_t *)&MPC.PD7PFS
#define PDL_MPC_REG_PE0PFS	(volatile uint8_t *)&MPC.PE0PFS
#define PDL_MPC_REG_PE1PFS	(volatile uint8_t *)&MPC.PE1PFS
#define PDL_MPC_REG_PE2PFS	(volatile uint8_t *)&MPC.PE2PFS
#define PDL_MPC_REG_PE3PFS	(volatile uint8_t *)&MPC.PE3PFS
#define PDL_MPC_REG_PE4PFS	(volatile uint8_t *)&MPC.PE4PFS
#define PDL_MPC_REG_PE5PFS	(volatile uint8_t *)&MPC.PE5PFS
#define PDL_MPC_REG_PE6PFS	(volatile uint8_t *)&MPC.PE6PFS
#define PDL_MPC_REG_PE7PFS	(volatile uint8_t *)&MPC.PE7PFS
#define PDL_MPC_REG_PF0PFS	(volatile uint8_t *)&MPC.PF0PFS
#define PDL_MPC_REG_PF1PFS	(volatile uint8_t *)&MPC.PF1PFS
#define PDL_MPC_REG_PF2PFS	(volatile uint8_t *)&MPC.PF2PFS
#define PDL_MPC_REG_PF5PFS	(volatile uint8_t *)&MPC.PF5PFS
#define PDL_MPC_REG_PJ3PFS	(volatile uint8_t *)&MPC.PJ3PFS
#define PDL_MPC_REG_PFCSE	(volatile uint8_t *)&MPC.PFCSE
#define PDL_MPC_REG_PFCSS0	(volatile uint8_t *)&MPC.PFCSS0
#define PDL_MPC_REG_PFCSS1	(volatile uint8_t *)&MPC.PFCSS1
#define PDL_MPC_REG_PFAOE0	(volatile uint8_t *)&MPC.PFAOE0
#define PDL_MPC_REG_PFAOE1	(volatile uint8_t *)&MPC.PFAOE1
#define PDL_MPC_REG_PFBCR0	(volatile uint8_t *)&MPC.PFBCR0
#define PDL_MPC_REG_PFBCR1	(volatile uint8_t *)&MPC.PFBCR1
#define PDL_MPC_REG_PFENET	(volatile uint8_t *)&MPC.PFENET
#define PDL_MPC_REG_PFUSB0	(volatile uint8_t *)&MPC.PFUSB0
#define PDL_MPC_REG_PFUSB1	(volatile uint8_t *)&MPC.PFUSB1

/* Logical operations */
#define PDL_MPC_AND	0x01u
#define PDL_MPC_OR	0x02u
#define PDL_MPC_XOR	0x04u

#endif
/* End of file */
