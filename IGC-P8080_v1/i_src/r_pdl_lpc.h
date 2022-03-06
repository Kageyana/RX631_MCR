/*""FILE COMMENT""*************************************************************
* System Name	: LPC API for RX63Nxx
* File Name		: r_pdl_lpc.h
* Version		: 1.21
* Contents		: LPC API header
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

#ifndef R_PDL_LPC_H
#define R_PDL_LPC_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_LPC_Create(
	uint32_t,
	uint32_t,
	uint32_t,
	uint32_t,
	uint16_t,
	uint16_t,
	uint16_t
);
bool R_LPC_Control(
	uint32_t
);
bool R_LPC_WriteBackup(
	uint8_t *,
	uint8_t
);
bool R_LPC_ReadBackup(
	uint8_t *,
	uint8_t
);
bool R_LPC_GetStatus(
	uint32_t *,
	uint8_t *
);

/* Software and Deep Software Standby mode output port control */
#define PDL_LPC_EXT_BUS_ON					0x00000001ul
#define PDL_LPC_EXT_BUS_HI_Z				0x00000002ul

/* I/O port retention control */
#define PDL_LPC_IO_SAME						0x00000004ul
#define PDL_LPC_IO_DELAY					0x00000008ul

/* Normal operating power consumption control */
#define PDL_LPC_HIGH_SPEED_MODE				0x00000010ul
#define PDL_LPC_LOW_SPEED_MODE_1			0x00000020ul
#define PDL_LPC_LOW_SPEED_MODE_2			0x00000040ul

/* Sleep mode return clock source switching */
#define PDL_LPC_SLEEP_RETURN_SWITCH_DISABLE	0x00000200ul
#define PDL_LPC_SLEEP_RETURN_SWITCH_HOCO	0x00000400ul
#define PDL_LPC_SLEEP_RETURN_SWITCH_MAIN	0x00000800ul

/*Deep software standby */
#define PDL_LPC_DEEPCUT_DISABLE				0x00001000ul
#define PDL_LPC_DEEPCUT_RAM_USB				0x00002000ul
#define PDL_LPC_DEEPCUT_RAM_USB_LVD			0x00004000ul

/* Deep software standby cancel control */

/*IRQ0 to IRQ7 */
#define PDL_LPC_CANCEL_IRQ0_DISABLE		0x00000001ul
#define PDL_LPC_CANCEL_IRQ0_FALLING		0x00000002ul
#define PDL_LPC_CANCEL_IRQ0_RISING		0x00000004ul
#define PDL_LPC_CANCEL_IRQ1_DISABLE		0x00000008ul
#define PDL_LPC_CANCEL_IRQ1_FALLING		0x00000010ul
#define PDL_LPC_CANCEL_IRQ1_RISING		0x00000020ul
#define PDL_LPC_CANCEL_IRQ2_DISABLE		0x00000040ul
#define PDL_LPC_CANCEL_IRQ2_FALLING		0x00000080ul
#define PDL_LPC_CANCEL_IRQ2_RISING		0x00000100ul
#define PDL_LPC_CANCEL_IRQ3_DISABLE		0x00000200ul
#define PDL_LPC_CANCEL_IRQ3_FALLING		0x00000400ul
#define PDL_LPC_CANCEL_IRQ3_RISING		0x00000800ul
#define PDL_LPC_CANCEL_IRQ4_DISABLE		0x00001000ul
#define PDL_LPC_CANCEL_IRQ4_FALLING		0x00002000ul
#define PDL_LPC_CANCEL_IRQ4_RISING		0x00004000ul
#define PDL_LPC_CANCEL_IRQ5_DISABLE		0x00008000ul
#define PDL_LPC_CANCEL_IRQ5_FALLING		0x00010000ul
#define PDL_LPC_CANCEL_IRQ5_RISING		0x00020000ul
#define PDL_LPC_CANCEL_IRQ6_DISABLE		0x00040000ul
#define PDL_LPC_CANCEL_IRQ6_FALLING		0x00080000ul
#define PDL_LPC_CANCEL_IRQ6_RISING		0x00100000ul
#define PDL_LPC_CANCEL_IRQ7_DISABLE		0x00200000ul
#define PDL_LPC_CANCEL_IRQ7_FALLING		0x00400000ul
#define PDL_LPC_CANCEL_IRQ7_RISING		0x00800000ul

/*IRQ8 to IRQ15 */
#define PDL_LPC_CANCEL_IRQ8_DISABLE		0x00000001ul
#define PDL_LPC_CANCEL_IRQ8_FALLING		0x00000002ul
#define PDL_LPC_CANCEL_IRQ8_RISING		0x00000004ul
#define PDL_LPC_CANCEL_IRQ9_DISABLE		0x00000008ul
#define PDL_LPC_CANCEL_IRQ9_FALLING		0x00000010ul
#define PDL_LPC_CANCEL_IRQ9_RISING		0x00000020ul
#define PDL_LPC_CANCEL_IRQ10_DISABLE	0x00000040ul
#define PDL_LPC_CANCEL_IRQ10_FALLING	0x00000080ul
#define PDL_LPC_CANCEL_IRQ10_RISING		0x00000100ul
#define PDL_LPC_CANCEL_IRQ11_DISABLE	0x00000200ul
#define PDL_LPC_CANCEL_IRQ11_FALLING	0x00000400ul
#define PDL_LPC_CANCEL_IRQ11_RISING		0x00000800ul
#define PDL_LPC_CANCEL_IRQ12_DISABLE	0x00001000ul
#define PDL_LPC_CANCEL_IRQ12_FALLING	0x00002000ul
#define PDL_LPC_CANCEL_IRQ12_RISING		0x00004000ul
#define PDL_LPC_CANCEL_IRQ13_DISABLE	0x00008000ul
#define PDL_LPC_CANCEL_IRQ13_FALLING	0x00010000ul
#define PDL_LPC_CANCEL_IRQ13_RISING		0x00020000ul
#define PDL_LPC_CANCEL_IRQ14_DISABLE	0x00040000ul
#define PDL_LPC_CANCEL_IRQ14_FALLING	0x00080000ul
#define PDL_LPC_CANCEL_IRQ14_RISING		0x00100000ul
#define PDL_LPC_CANCEL_IRQ15_DISABLE	0x00200000ul
#define PDL_LPC_CANCEL_IRQ15_FALLING	0x00400000ul
#define PDL_LPC_CANCEL_IRQ15_RISING		0x00800000ul

#define PDL_LPC_CANCEL_LVD1_DISABLE		0x00000001ul
#define PDL_LPC_CANCEL_LVD1_FALLING		0x00000002ul
#define PDL_LPC_CANCEL_LVD1_RISING		0x00000004ul
#define PDL_LPC_CANCEL_LVD2_DISABLE		0x00000008ul
#define PDL_LPC_CANCEL_LVD2_FALLING		0x00000010ul
#define PDL_LPC_CANCEL_LVD2_RISING		0x00000020ul
#define PDL_LPC_CANCEL_RTCI_DISABLE		0x00000040ul
#define PDL_LPC_CANCEL_RTCI_ENABLE		0x00000080ul
#define PDL_LPC_CANCEL_RTCA_DISABLE		0x00000100ul
#define PDL_LPC_CANCEL_RTCA_ENABLE		0x00000200ul
#define PDL_LPC_CANCEL_NMI_DISABLE		0x00000400ul
#define PDL_LPC_CANCEL_NMI_FALLING		0x00000800ul
#define PDL_LPC_CANCEL_NMI_RISING		0x00001000ul
#define PDL_LPC_CANCEL_IICD_DISABLE		0x00002000ul
#define PDL_LPC_CANCEL_IICD_FALLING		0x00004000ul
#define PDL_LPC_CANCEL_IICD_RISING		0x00008000ul
#define PDL_LPC_CANCEL_IICC_DISABLE		0x00010000ul
#define PDL_LPC_CANCEL_IICC_FALLING		0x00020000ul
#define PDL_LPC_CANCEL_IICC_RISING		0x00040000ul
/*USB interrupt does not have an edge selection */
#define PDL_LPC_CANCEL_USB_DISABLE		0x00080000ul
#define PDL_LPC_CANCEL_USB_ENABLE		0x00100000ul
#define PDL_LPC_CANCEL_CAN_DISABLE		0x00200000ul
#define PDL_LPC_CANCEL_CAN_FALLING		0x00400000ul
#define PDL_LPC_CANCEL_CAN_RISING		0x00800000ul

/* Main clock oscillator waiting time */
#define PDL_LPC_MAIN_2					0x0001ul
#define PDL_LPC_MAIN_4					0x0002ul
#define PDL_LPC_MAIN_8					0x0004ul
#define PDL_LPC_MAIN_16					0x0008ul
#define PDL_LPC_MAIN_32					0x0010ul
#define PDL_LPC_MAIN_64					0x0020ul
#define PDL_LPC_MAIN_512				0x0040ul
#define PDL_LPC_MAIN_1024				0x0080ul
#define PDL_LPC_MAIN_2048				0x0100ul
#define PDL_LPC_MAIN_4096				0x0200ul
#define PDL_LPC_MAIN_16384				0x0400ul
#define PDL_LPC_MAIN_32768				0x0800ul
#define PDL_LPC_MAIN_65536				0x1000ul
#define PDL_LPC_MAIN_131072				0x2000ul
#define PDL_LPC_MAIN_262144				0x4000ul
#define PDL_LPC_MAIN_524288				0x8000ul

/* Sub-clock oscillator waiting time */
#define PDL_LPC_SUB_2					0x0001ul
#define PDL_LPC_SUB_4					0x0002ul
#define PDL_LPC_SUB_8					0x0004ul
#define PDL_LPC_SUB_16					0x0008ul
#define PDL_LPC_SUB_32					0x0010ul
#define PDL_LPC_SUB_64					0x0020ul
#define PDL_LPC_SUB_512					0x0040ul
#define PDL_LPC_SUB_1024				0x0080ul
#define PDL_LPC_SUB_2048				0x0100ul
#define PDL_LPC_SUB_4096				0x0200ul
#define PDL_LPC_SUB_16384				0x0400ul
#define PDL_LPC_SUB_32768				0x0800ul
#define PDL_LPC_SUB_65536				0x1000ul
#define PDL_LPC_SUB_131072				0x2000ul
#define PDL_LPC_SUB_262144				0x4000ul
#define PDL_LPC_SUB_524288				0x8000ul

/* PLL waiting time */
#define PDL_LPC_PLL_16					0x0001ul
#define PDL_LPC_PLL_32					0x0002ul
#define PDL_LPC_PLL_64					0x0004ul
#define PDL_LPC_PLL_512					0x0008ul
#define PDL_LPC_PLL_1024				0x0010ul
#define PDL_LPC_PLL_2048				0x0020ul
#define PDL_LPC_PLL_4096				0x0040ul
#define PDL_LPC_PLL_16384				0x0080ul
#define PDL_LPC_PLL_32768				0x0100ul
#define PDL_LPC_PLL_65536				0x0200ul
#define PDL_LPC_PLL_131072				0x0400ul
#define PDL_LPC_PLL_262144				0x0800ul
#define PDL_LPC_PLL_524288				0x1000ul
#define PDL_LPC_PLL_1048576				0x2000ul
#define PDL_LPC_PLL_2097152				0x4000ul
#define PDL_LPC_PLL_4194304				0x8000ul

/***R_LPC_CONTROL***/

/* Mode */
#define PDL_LPC_MODE_SLEEP					0x00000001u
#define PDL_LPC_MODE_ALL_MODULE_CLOCK_STOP	0x00000002u
#define PDL_LPC_MODE_SOFTWARE_STANDBY		0x00000004u
#define PDL_LPC_MODE_DEEP_SOFTWARE_STANDBY	0x00000008u

/* All-module clock stop cancellation modification */
#define PDL_LPC_TMR_OFF						0x00000010u
#define PDL_LPC_TMR_UNIT_0					0x00000020u
#define PDL_LPC_TMR_UNIT_1					0x00000040u
#define PDL_LPC_TMR_BOTH					0x00000080u

/* I/O port retention cancellation */
#define PDL_LPC_IO_RELEASE					0x00000100u

/* Normal operating power consumption control */
#define PDL_LPC_CHANGE_HIGH_SPEED			0x00000200ul
#define PDL_LPC_CHANGE_LOW_SPEED_1			0x00000400ul
#define PDL_LPC_CHANGE_LOW_SPEED_2			0x00000800ul

/* Sleep mode return clock source switching */
#define PDL_LPC_SLEEP_RETURN_CHANGE_DISABLE	0x00001000ul
#define PDL_LPC_SLEEP_RETURN_CHANGE_HOCO	0x00002000ul
#define PDL_LPC_SLEEP_RETURN_CHANGE_MAIN	0x00004000ul


#endif
/* End of file */
