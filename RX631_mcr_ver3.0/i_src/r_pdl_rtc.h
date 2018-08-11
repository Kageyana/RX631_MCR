/*""FILE COMMENT""*************************************************************
* System Name	: Real-time clock API for RX63N
* File Name		: r_pdl_rtc.h
* Version		: 1.21
* Contents		: RTC function prototypes
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
*History		: 2012.03.22
*				: Ver 0.10
*				: First draft
*				: 2013.04.03
*				: Ver 0.11
*				: Update for hardware manual Rev 1.60
*				: 2013.04.23
*				: Ver 0.12
*				: TECHNICAL UPDATE TN-RX*-A052A/E and TN-RX*-A060A/E 
*				: 2013.11.25
*				: Ver 0.13
*				: Added new R_RTC_CreateWarm API. REE #975
*				: 2014.03.21
*				: Ver 0.14
*				: remove comment
*""FILE COMMENT END""*********************************************************/

#ifndef R_PDL_RTC_H
#define R_PDL_RTC_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_RTC_Create(
	uint32_t,
	uint8_t,
	uint32_t,
	uint32_t,
	uint8_t,
	uint8_t,
	uint8_t,
	uint16_t,
	uint32_t,
	uint32_t,
	void *,
	uint8_t,
	void *,
	uint8_t
);

bool R_RTC_Control(
	uint32_t,
	uint16_t,
	uint32_t,
	uint32_t,
	uint32_t,
	uint32_t,
	uint16_t,
	uint8_t, 
	uint8_t, 
	uint8_t,
	uint16_t
);

bool R_RTC_Read(
	uint8_t, 
	uint8_t *,
	uint32_t *,
	uint32_t *
);

/* Function is called at warm startup */
bool R_RTC_CreateWarm(
	void *,
	uint8_t,
	void *,
	uint8_t
);

/* Alarm control */
#define PDL_RTC_ALARM_HOUR_DISABLE		0x00000001ul
#define PDL_RTC_ALARM_HOUR_ENABLE		0x00000002ul
#define PDL_RTC_ALARM_MINUTE_DISABLE	0x00000004ul
#define PDL_RTC_ALARM_MINUTE_ENABLE		0x00000008ul
#define PDL_RTC_ALARM_SECOND_DISABLE	0x00000010ul
#define PDL_RTC_ALARM_SECOND_ENABLE		0x00000020ul
#define PDL_RTC_ALARM_YEAR_DISABLE		0x00000040ul
#define PDL_RTC_ALARM_YEAR_ENABLE		0x00000080ul
#define PDL_RTC_ALARM_MONTH_DISABLE		0x00000100ul
#define PDL_RTC_ALARM_MONTH_ENABLE		0x00000200ul
#define PDL_RTC_ALARM_DAY_DISABLE		0x00000400ul
#define PDL_RTC_ALARM_DAY_ENABLE		0x00000800ul
#define PDL_RTC_ALARM_DOW_DISABLE		0x00001000ul
#define PDL_RTC_ALARM_DOW_ENABLE		0x00002000ul

#define PDL_RTC_ALARM_TIME_DISABLE \
(PDL_RTC_ALARM_HOUR_DISABLE | PDL_RTC_ALARM_MINUTE_DISABLE | PDL_RTC_ALARM_SECOND_DISABLE)
#define PDL_RTC_ALARM_TIME_ENABLE \
(PDL_RTC_ALARM_HOUR_ENABLE | PDL_RTC_ALARM_MINUTE_ENABLE | PDL_RTC_ALARM_SECOND_ENABLE)
#define PDL_RTC_ALARM_DATE_DISABLE \
	(PDL_RTC_ALARM_YEAR_DISABLE | PDL_RTC_ALARM_MONTH_DISABLE | \
	PDL_RTC_ALARM_DAY_DISABLE | PDL_RTC_ALARM_DOW_DISABLE)
#define PDL_RTC_ALARM_DATE_ENABLE \
	(PDL_RTC_ALARM_YEAR_ENABLE | PDL_RTC_ALARM_MONTH_ENABLE | \
	PDL_RTC_ALARM_DAY_ENABLE | PDL_RTC_ALARM_DOW_ENABLE)


/* Clock output control */
#define PDL_RTC_OUTPUT_DISABLE			0x00004000ul
#define PDL_RTC_OUTPUT_ENABLE			0x00008000ul

/* Clock control */
#define PDL_RTC_CLOCK_STOP				0x00010000ul
#define PDL_RTC_CLOCK_START				0x00020000ul

/* 30-second adjustment control */
#define PDL_RTC_ADJUST_START			0x00040000ul
 
/* Reset control */
#define PDL_RTC_RESET_START				0x00080000ul

/*12 or 24 Hour Mode selection*/
#define PDL_RTC_24_HOUR_MODE			0x00100000ul
#define PDL_RTC_12_HOUR_MODE			0x00200000ul

/* ------------------------ */
/* R_RTC_Create, PinSetting */
/* ------------------------ */
/*RTCOUT Pin Selection*/
#define PDL_RTC_PIN_RTCOUT_P16	0x01u
#define PDL_RTC_PIN_RTCOUT_P32	0x02u
/*Capture Pins*/
/*RTCIC0*/
#define PDL_RTC_PIN_RTCIC0_P30	0x04u
/*RTCIC1*/	
#define PDL_RTC_PIN_RTCIC1_P31	0x08u
/*RTCIC2*/
#define PDL_RTC_PIN_RTCIC2_P32	0x10u

/* -----------------------------------------------------------*/
/* RTC_Control, Error Adjustment parameter  */
/* Bits 0 to 7 are reserved for the error adjustment value*/
/* ---------------------------------------------------------- */
#define PDL_RTC_ERROR_AUTO_ADJUST_DISABLE		0x0100u
#define PDL_RTC_ERROR_AUTO_ADJUST_ENABLE		0x0200u
#define PDL_RTC_ERROR_AUTO_ADJUST_PERIOD_10S	0x0400u
#define PDL_RTC_ERROR_AUTO_ADJUST_PERIOD_60S	0x0800u
#define PDL_RTC_ERROR_ADJUST_PLUS				0x1000u
#define PDL_RTC_ERROR_ADJUST_MINUS				0x2000u
/*This is only valid in RTC_Control */
#define PDL_RTC_ERROR_UPDATE_ERROR_ADJUST_VALUE 0x4000u

/* Current time counter selection */
#define PDL_RTC_UPDATE_CURRENT_HOUR		0x0001u
#define PDL_RTC_UPDATE_CURRENT_MINUTE	0x0002u
#define PDL_RTC_UPDATE_CURRENT_SECOND	0x0004u
#define PDL_RTC_UPDATE_CURRENT_TIME	\
(PDL_RTC_UPDATE_CURRENT_HOUR | PDL_RTC_UPDATE_CURRENT_MINUTE | PDL_RTC_UPDATE_CURRENT_SECOND)

/* Current date counter selection */
#define PDL_RTC_UPDATE_CURRENT_YEAR		0x0008u
#define PDL_RTC_UPDATE_CURRENT_MONTH	0x0010u
#define PDL_RTC_UPDATE_CURRENT_DAY		0x0020u
#define PDL_RTC_UPDATE_CURRENT_DOW		0x0040u
#define PDL_RTC_UPDATE_CURRENT_DATE \
(PDL_RTC_UPDATE_CURRENT_YEAR | PDL_RTC_UPDATE_CURRENT_MONTH | PDL_RTC_UPDATE_CURRENT_DAY | PDL_RTC_UPDATE_CURRENT_DOW)

/* Alarm time counter selection */
#define PDL_RTC_UPDATE_ALARM_HOUR		0x0080u
#define PDL_RTC_UPDATE_ALARM_MINUTE		0x0100u
#define PDL_RTC_UPDATE_ALARM_SECOND		0x0200u
#define PDL_RTC_UPDATE_ALARM_TIME \
(PDL_RTC_UPDATE_ALARM_HOUR | PDL_RTC_UPDATE_ALARM_MINUTE | PDL_RTC_UPDATE_ALARM_SECOND)

/* Alarm date counter selection */
#define PDL_RTC_UPDATE_ALARM_YEAR		0x0400u
#define PDL_RTC_UPDATE_ALARM_MONTH		0x0800u
#define PDL_RTC_UPDATE_ALARM_DAY		0x1000u
#define PDL_RTC_UPDATE_ALARM_DOW		0x2000u
#define PDL_RTC_UPDATE_ALARM_DATE \
(PDL_RTC_UPDATE_ALARM_YEAR | PDL_RTC_UPDATE_ALARM_MONTH	| PDL_RTC_UPDATE_ALARM_DAY	| PDL_RTC_UPDATE_ALARM_DOW)

/* ----------------------------------------------- */
/* R_RTC_Create and R_RTC_Control, Capture setting */
/* ----------------------------------------------- */
/*Edge detection on RTCIC(n) pin*/
#define PDL_RTC_CAPTURE_EDGE_NONE			0x01u
#define PDL_RTC_CAPTURE_EDGE_RISING			0x02u
#define PDL_RTC_CAPTURE_EDGE_FALLING		0x04u
#define PDL_RTC_CAPTURE_EDGE_BOTH			0x08u
/*Noise Filter*/
#define PDL_RTC_CAPTURE_FILTER_OFF			0x10u
#define PDL_RTC_CAPTURE_FILTER_ON_DIV_1		0x20u
#define PDL_RTC_CAPTURE_FILTER_ON_DIV_32	0x40u

/* ------------------------------------------------ */
/* R_RTC_Create and R_RTC_Control, Periodic setting */
/* ------------------------------------------------ */
/* Periodic interrupt selection */
#define PDL_RTC_PERIODIC_DISABLE		0x0001ul
#define PDL_RTC_PERIODIC_256_HZ			0x0002ul
#define PDL_RTC_PERIODIC_128_HZ			0x0004ul
#define PDL_RTC_PERIODIC_64_HZ			0x0008ul
#define PDL_RTC_PERIODIC_32_HZ			0x0010ul
#define PDL_RTC_PERIODIC_16_HZ			0x0020ul
#define PDL_RTC_PERIODIC_8_HZ			0x0040ul
#define PDL_RTC_PERIODIC_4_HZ			0x0080ul
#define PDL_RTC_PERIODIC_2_HZ			0x0100ul
#define PDL_RTC_PERIODIC_1_HZ			0x0200ul
#define PDL_RTC_PERIODIC_2S				0x0400ul


/* -----------------*/
/* R_RTC_Read 		*/
/* -----------------*/
#define PDL_RTC_READ_CURRENT				0x01u
#define PDL_RTC_READ_ALARM					0x02u
#define PDL_RTC_READ_CAPTURE_0				0x04u
#define PDL_RTC_READ_CAPTURE_1				0x08u
#define PDL_RTC_READ_CAPTURE_2				0x10u

#endif

/* End of file */
