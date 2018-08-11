/*""FILE COMMENT""*************************************************************
* System Name	: CGC API
* File Name		: r_pdl_cgc.h
* Version		: 1.21
* Contents		: CGC header
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

#ifndef R_PDL_CGC_H
#define R_PDL_CGC_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_CGC_Set(
	uint8_t,
	uint32_t,
	double,
	double,
	double,
	double,
	double,
	double,
	double,
	double,
	uint16_t
);

bool R_CGC_Control(
	uint8_t,
	uint32_t,
	uint8_t
);

bool R_CGC_GetStatus(
	uint16_t *
);

/* Clock source selection */
#define PDL_CGC_CLK_LOCO				0x01u
#define PDL_CGC_CLK_HOCO				0x02u
#define PDL_CGC_CLK_MAIN				0x04u
#define PDL_CGC_CLK_SUB_CLOCK			0x08u
#define PDL_CGC_CLK_PLL					0x10u
#define PDL_CGC_CLK_IWDTLOCO			0x20u

/* BCLK pin control */
#define PDL_CGC_BCLK_DIV_1				0x00000001ul
#define PDL_CGC_BCLK_DIV_2				0x00000002ul
#define PDL_CGC_BCLK_DISABLE			0x00000004ul
#define PDL_CGC_BCLK_ENABLE				0x00000008ul

/* Low-speed on-chip oscillator control */
#define PDL_CGC_LOCO_ENABLE				0x00000010ul
#define PDL_CGC_LOCO_DISABLE			0x00000020ul

/* High-speed on-chip oscillator control */
#define PDL_CGC_HOCO_ENABLE				0x00000040ul
#define PDL_CGC_HOCO_DISABLE			0x00000080ul

/* High-speed on-chip oscillator power control */
#define PDL_CGC_HOCO_POWER_ON			0x00000100ul
#define PDL_CGC_HOCO_POWER_OFF			0x00000200ul

/* Main clock oscillator control */
#define PDL_CGC_MAIN_ENABLE				0x00000400ul
#define PDL_CGC_MAIN_DISABLE			0x00000800ul

/* Main clock oscillator forced oscillation control */
#define PDL_CGC_MAIN_FORCED_ENABLE		0x00001000ul
#define PDL_CGC_MAIN_FORCED_DISABLE		0x00002000ul

/* Oscillation Stop Detection control */
#define PDL_CGC_OSC_STOP_ENABLE			0x00004000ul
#define PDL_CGC_OSC_STOP_INTERRUPT		0x00008000ul
#define PDL_CGC_OSC_STOP_DISABLE		0x00010000ul
#define PDL_CGC_OSC_STOP_CLEAR_FLAG		0x00020000ul

/* SDCLK pin output control */
#define PDL_CGC_SDCLK_ENABLE			0x00040000ul
#define PDL_CGC_SDCLK_DISABLE			0x00080000ul

/* Sub-clock oscillator drive ability */
#define PDL_CGC_SUB_CLOCK_CL_LOW		0x00100000ul
#define PDL_CGC_SUB_CLOCK_CL_STANDARD	0x00200000ul

/* RTC Count source selection  */
#define PDL_CGC_RTC_SRC_SUB				0x00400000ul
#define PDL_CGC_RTC_SRC_MAIN			0x00800000ul
#define PDL_CGC_RTC_SRC_NONE			0x01000000ul

/* Sub-clock oscillator control */
#define PDL_CGC_SUB_CLOCK_ENABLE		0x01u
#define PDL_CGC_SUB_CLOCK_DISABLE		0x02u

/* PLL control */
#define PDL_CGC_PLL_ENABLE				0x04u
#define PDL_CGC_PLL_DISABLE				0x08u

/* IWDT-dedicated low-speed on-chip oscillator control */
#define PDL_CGC_IWDTLOCO_ENABLE			0x10u
#define PDL_CGC_IWDTLOCO_DISABLE		0x20u

/* RTC initialization control  */
#define PDL_CGC_RTC_TO_BE_USED			0x40u
#define PDL_CGC_RTC_NOT_USE				0x80u

/* Sub-clock oscillator waiting time */
#define PDL_CGC_SUB_2					0x0001ul
#define PDL_CGC_SUB_4					0x0002ul
#define PDL_CGC_SUB_8					0x0004ul
#define PDL_CGC_SUB_16					0x0008ul
#define PDL_CGC_SUB_32					0x0010ul
#define PDL_CGC_SUB_64					0x0020ul
#define PDL_CGC_SUB_512					0x0040ul
#define PDL_CGC_SUB_1024				0x0080ul
#define PDL_CGC_SUB_2048				0x0100ul
#define PDL_CGC_SUB_4096				0x0200ul
#define PDL_CGC_SUB_16384				0x0400ul
#define PDL_CGC_SUB_32768				0x0800ul
#define PDL_CGC_SUB_65536				0x1000ul
#define PDL_CGC_SUB_131072				0x2000ul
#define PDL_CGC_SUB_262144				0x4000ul
#define PDL_CGC_SUB_524288				0x8000ul

#endif
/* End of file */
