/*""FILE COMMENT""*************************************************************
* System Name	: ADC converter API for RX63Nxx
* File Name		: r_pdl_adc_10.h
* Version		: 1.21
* Contents		: ADC function prototypes
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

#ifndef R_PDL_ADC_10_H
#define R_PDL_ADC_10_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_ADC_10_Set(
	uint16_t
);
bool R_ADC_10_Create(
	uint8_t,
	uint32_t,
	double,
	double,
	void *,
	uint8_t
);
bool R_ADC_10_Destroy(
	uint8_t
);
bool R_ADC_10_Control(
	uint16_t
);
bool R_ADC_10_Read(
	uint8_t,
	uint16_t *
);

#define PDL_ADC_10_PIN_AN0_PE2					0x0001u
#define PDL_ADC_10_PIN_AN1_PE3					0x0002u
#define PDL_ADC_10_PIN_AN2_PE4					0x0004u
#define PDL_ADC_10_PIN_AN3_PE5					0x0008u
#define PDL_ADC_10_PIN_AN4_PE6					0x0010u
#define PDL_ADC_10_PIN_AN5_PE7					0x0020u
#define PDL_ADC_10_PIN_AN6_PD6					0x0040u
#define PDL_ADC_10_PIN_AN7_PD7					0x0080u
#define PDL_ADC_10_PIN_ANEX0_PE0				0x0100u
#define PDL_ADC_10_PIN_ANEX1_PE1				0x0200u
#define PDL_ADC_10_PIN_ADTRG_P13				0x0400u
#define PDL_ADC_10_PIN_ADTRG_P17				0x0800u

/* Scan mode */
#define PDL_ADC_10_MODE_SINGLE					0x00000001u
#define PDL_ADC_10_MODE_CONTINUOUS_SCAN			0x00000002u
#define PDL_ADC_10_MODE_ONE_CYCLE_SCAN			0x00000004u

/* Channel selection */
#define PDL_ADC_10_CHANNELS_OPTION_1			0x00000008u
#define PDL_ADC_10_CHANNELS_OPTION_2			0x00000010u
#define PDL_ADC_10_CHANNELS_OPTION_3			0x00000020u
#define PDL_ADC_10_CHANNELS_OPTION_4			0x00000040u
#define PDL_ADC_10_CHANNELS_OPTION_5			0x00000080u
#define PDL_ADC_10_CHANNELS_OPTION_6			0x00000100u
#define PDL_ADC_10_CHANNELS_OPTION_7			0x00000200u
#define PDL_ADC_10_CHANNELS_OPTION_8			0x00000400u

/* Trigger selection */
#define PDL_ADC_10_TRIGGER_SOFTWARE				0x00000800u 
#define PDL_ADC_10_TRIGGER_MTU0_MTU4_CMIC_A		0x00001000u 
#define PDL_ADC_10_TRIGGER_TMR0_CM				0x00002000u 
#define PDL_ADC_10_TRIGGER_ADTRG				0x00004000u 
#define PDL_ADC_10_TRIGGER_MTU0_CMIC_A			0x00008000u 
#define PDL_ADC_10_TRIGGER_TPU0_TPU4_CMIC_A		0x00010000u 
#define PDL_ADC_10_TRIGGER_MTU4_CM				0x00020000u 
#define PDL_ADC_10_TRIGGER_TPU0_CMIC_A			0x00040000u 

/* Data alignment */
#define PDL_ADC_10_DATA_ALIGNMENT_LEFT			0x00080000u
#define PDL_ADC_10_DATA_ALIGNMENT_RIGHT			0x00100000u

/* DMAC / DTC trigger control */
#define PDL_ADC_10_DMAC_DTC_TRIGGER_DISABLE		0x00200000u
#define PDL_ADC_10_DMAC_TRIGGER_ENABLE			0x00400000u
#define PDL_ADC_10_DTC_TRIGGER_ENABLE			0x00800000u

/* Sampling time calcuation control */
#define PDL_ADC_10_ADSSTR_CALCULATE				0x01000000u
#define PDL_ADC_10_ADSSTR_SPECIFY				0x02000000u

/* Self-Diagnostic */									
#define PDL_ADC_10_SINGLE_AN0_SW				(PDL_ADC_10_MODE_SINGLE | \
												 PDL_ADC_10_CHANNELS_OPTION_1 | \
												 PDL_ADC_10_TRIGGER_SOFTWARE)
#define PDL_ADC_10_ADDIAGR_DISABLE				0x04000000u
#define PDL_ADC_10_ADDIAGR_VREF_0				0x08000000u
#define PDL_ADC_10_ADDIAGR_VREF_0_5				0x10000000u
#define PDL_ADC_10_ADDIAGR_VREF_1				0x20000000u
#define PDL_ADC_10_SELF_DIAGNOSTIC_DISABLE		(PDL_ADC_10_ADDIAGR_DISABLE)
#define PDL_ADC_10_SELF_DIAGNOSTIC_VREF_0		(PDL_ADC_10_SINGLE_AN0_SW | PDL_ADC_10_ADDIAGR_VREF_0)
#define PDL_ADC_10_SELF_DIAGNOSTIC_VREF_0_5		(PDL_ADC_10_SINGLE_AN0_SW | PDL_ADC_10_ADDIAGR_VREF_0_5)
#define PDL_ADC_10_SELF_DIAGNOSTIC_VREF_1		(PDL_ADC_10_SINGLE_AN0_SW | PDL_ADC_10_ADDIAGR_VREF_1)

#define PDL_ADC_10_INPUT_AN						0x40000000u
#define PDL_ADC_10_INPUT_ANEX1					0x80000000u

#define PDL_ADC_10_ANEX_DISABLE					(PDL_ADC_10_INPUT_AN)

/* On / off control */
#define PDL_ADC_10_0_ON		0x0001u
#define PDL_ADC_10_0_OFF	0x0002u
 
/* CPU control */
#define PDL_ADC_10_CPU_ON	0x0100u
#define PDL_ADC_10_CPU_OFF	0x0200u

#endif
/* End of file */
