/*""FILE COMMENT""*************************************************************
* System Name	: 12-bit ADC converter API for RX63Nxx
* File Name		: r_pdl_adc_12.h
* Version		: 1.21
* Contents		: ADC_12 function prototypes
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

#ifndef R_PDL_ADC_12_H
#define R_PDL_ADC_12_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_ADC_12_Create(
	uint8_t,
	uint32_t,
	uint32_t,
	uint16_t,
	uint32_t,
	double,
	double,
	void *,
	uint8_t
);
bool R_ADC_12_Destroy(
	uint8_t
);
bool R_ADC_12_Control(
	uint8_t
);
bool R_ADC_12_Read(
	uint8_t,
	uint16_t *
);

/*** R_ADC_12_Create: Channel parameter ***/
/* Input channel selection */
#define PDL_ADC_12_CHANNEL_0				0x00000001ul
#define PDL_ADC_12_CHANNEL_1				0x00000002ul
#define PDL_ADC_12_CHANNEL_2				0x00000004ul
#define PDL_ADC_12_CHANNEL_3				0x00000008ul
#define PDL_ADC_12_CHANNEL_4				0x00000010ul
#define PDL_ADC_12_CHANNEL_5				0x00000020ul
#define PDL_ADC_12_CHANNEL_6				0x00000040ul
#define PDL_ADC_12_CHANNEL_7				0x00000080ul
#define PDL_ADC_12_CHANNEL_8				0x00000100ul
#define PDL_ADC_12_CHANNEL_9				0x00000200ul
#define PDL_ADC_12_CHANNEL_10				0x00000400ul
#define PDL_ADC_12_CHANNEL_11				0x00000800ul
#define PDL_ADC_12_CHANNEL_12				0x00001000ul
#define PDL_ADC_12_CHANNEL_13				0x00002000ul
#define PDL_ADC_12_CHANNEL_14				0x00004000ul
#define PDL_ADC_12_CHANNEL_15				0x00008000ul
#define PDL_ADC_12_CHANNEL_16				0x00010000ul
#define PDL_ADC_12_CHANNEL_17				0x00020000ul
#define PDL_ADC_12_CHANNEL_18				0x00040000ul
#define PDL_ADC_12_CHANNEL_19				0x00080000ul
#define PDL_ADC_12_CHANNEL_20				0x00100000ul

/*** R_ADC_12_Create: Configuration parameter ***/

/* Scan mode */
#define PDL_ADC_12_SCAN_SINGLE				0x00000001ul
#define PDL_ADC_12_SCAN_CONTINUOUS			0x00000002ul

/* Clock division */
#define PDL_ADC_12_DIV_1					0x00000004ul
#define PDL_ADC_12_DIV_2					0x00000008ul
#define PDL_ADC_12_DIV_4					0x00000010ul
#define PDL_ADC_12_DIV_8					0x00000020ul

/* Data alignment */
#define PDL_ADC_12_DATA_ALIGNMENT_LEFT		0x00000040ul
#define PDL_ADC_12_DATA_ALIGNMENT_RIGHT		0x00000080ul

/* Result register clearing */
#define PDL_ADC_12_RETAIN_RESULT			0x00000100ul
#define PDL_ADC_12_CLEAR_RESULT				0x00000200ul

/* ADEXICR: Input source */
#define PDL_ADC_12_INPUT_AN					0x00000400ul
#define PDL_ADC_12_INPUT_TS					0x00000800ul
#define PDL_ADC_12_INPUT_REF				0x00001000ul

/* DMAC / DTC trigger control */
#define PDL_ADC_12_DMAC_DTC_TRIGGER_DISABLE	0x00002000ul
#define PDL_ADC_12_DMAC_TRIGGER_ENABLE		0x00004000ul
#define PDL_ADC_12_DTC_TRIGGER_ENABLE		0x00008000ul
										
/* ADSSTR23: Sampling time calcuation control (Temperature Sensor)*/
#define PDL_ADC_12_SAMPLING_TIME_TEMP_CALCULATE 0x00010000ul
#define PDL_ADC_12_SAMPLING_TIME_TEMP_SPECIFY	0x00020000ul

/* ADSSTR01: Sampling time calcuation control (Analaog Inputs)*/
#define PDL_ADC_12_SAMPLING_TIME_CALCULATE 		0x00040000ul
#define PDL_ADC_12_SAMPLING_TIME_SPECIFY		0x00080000ul

/*** R_ADC_12_Create: trigger_selection parameter ***/

/* Trigger selection */
#define PDL_ADC_12_TRIGGER_SOFTWARE			0x0001u
#define PDL_ADC_12_TRIGGER_ADTRG0			0x0002u
#define PDL_ADC_12_TRIGGER_MTU0_ICCM_A		0x0004u
#define PDL_ADC_12_TRIGGER_MTU0_ICCM_B		0x0008u
#define PDL_ADC_12_TRIGGER_MTU0_MTU4_ICCM	0x0010u
#define PDL_ADC_12_TRIGGER_TPU0_TPU4_ICCM_A	0x0020u
#define PDL_ADC_12_TRIGGER_MTU0_CM_E		0x0040u
#define PDL_ADC_12_TRIGGER_MTU0_CM_F		0x0080u
#define PDL_ADC_12_TRIGGER_MTU4_CM			0x0100u
#define PDL_ADC_12_TRIGGER_TPU0_ICCM_A		0x0200u
#define PDL_ADC_12_TRIGGER_TMR0				0x0400u
#define PDL_ADC_12_TRIGGER_TMR2				0x0800u

/* ADTRG0 Pin selection */
#define PDL_ADC_12_PIN_ADTRG0_P07			0x1000u
#define PDL_ADC_12_PIN_ADTRG0_P16			0x2000u
#define PDL_ADC_12_PIN_ADTRG0_P25			0x4000u

/*** R_ADC_12_Create: value_addition parameter ***/

/* Value addition mode selection */
#define PDL_ADC_12_VALUE_ADD_CHANNEL_0	0x00000001u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_1	0x00000002u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_2	0x00000004u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_3	0x00000008u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_4	0x00000010u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_5	0x00000020u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_6	0x00000040u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_7	0x00000080u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_8	0x00000100u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_9	0x00000200u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_10	0x00000400u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_11	0x00000800u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_12	0x00001000u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_13	0x00002000u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_14	0x00004000u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_15	0x00008000u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_16	0x00010000u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_17	0x00020000u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_18	0x00040000u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_19	0x00080000u
#define PDL_ADC_12_VALUE_ADD_CHANNEL_20	0x00100000u

#define PDL_ADC_12_VALUE_ADD_TS			0x00200000u
#define PDL_ADC_12_VALUE_ADD_REF		0x00400000u

/* Value addition count selection */
#define PDL_ADC_12_VALUE_ADD_TIME_1		0x00800000u 
#define PDL_ADC_12_VALUE_ADD_TIME_2		0x01000000u 
#define PDL_ADC_12_VALUE_ADD_TIME_3		0x02000000u 
#define PDL_ADC_12_VALUE_ADD_TIME_4		0x04000000u 


/*** R_ADC_12_Control: ***/

/* On / off control */
#define PDL_ADC_12_0_ON		0x01u
#define PDL_ADC_12_0_OFF	0x02u

/* CPU control */
#define PDL_ADC_12_CPU_OFF	0x04u

#endif
/* End of file */
