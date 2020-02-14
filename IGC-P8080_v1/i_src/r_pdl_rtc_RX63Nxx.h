/*""FILE COMMENT""*************************************************************
* System Name	: Real-time clock API for RX63Nxx
* File Name		: r_pdl_rtc_RX63Nxx.h
* Version		: 1.21
* Contents		: RTC header
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

#ifndef R_PDL_RTC_RX63Nxx_H
#define R_PDL_RTC_RX63Nxx_H

extern VoidCallBackFunc rpdl_RTC_Periodic_callback_func;
extern VoidCallBackFunc rpdl_RTC_Alarm_callback_func;

/* Library prototypes */
/* function is called at cold startup */
bool R_RTC_CreateAll(
	const uint32_t,
	const uint8_t,
	const uint32_t,
	const uint32_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint16_t,
	const uint32_t,
	const uint32_t,
	VoidCallBackFunc const,
	const uint8_t,
	VoidCallBackFunc const,
	const uint8_t
);

bool R_RTC_ControlAll(
	const uint32_t,
	const uint16_t,
	const uint32_t,
	const uint32_t,
	const uint32_t,
	const uint32_t,
	const uint16_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint16_t
);
bool R_RTC_ReadAll(
	const uint8_t,
	volatile uint8_t * const,
	volatile uint32_t * const,
	volatile uint32_t * const
);
bool R_RTC_DestroyAll(void);

/* function is called at warm startup */
bool R_RTC_CreateWarmAll(
	VoidCallBackFunc const,
	const uint8_t,
	VoidCallBackFunc const,
	const uint8_t
);

bool ReturnFalse(void);

/* Macro definitions */

#define R_RTC_Create(a, b, c, d, e, f, g, h, i, j, k, l, m , n) \
( \
( ((l) <= IPL_MAX) && ((n) <= IPL_MAX) ) ? \
R_RTC_CreateAll( (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n) ): \
ReturnFalse() \
)

#define R_RTC_Control(a, b, c, d, e, f, g, h, i, j, k) \
( \
R_RTC_ControlAll( (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k) ) \
)

#define R_RTC_Read(a, b, c, d) \
( \
R_RTC_ReadAll( (a), (b), (c), (d) ) \
)

#define R_RTC_Destroy() \
( \
R_RTC_DestroyAll() \
)

#define R_RTC_CreateWarm(a, b, c , d) \
( \
( ((b) <= IPL_MAX) && ((d) <= IPL_MAX) ) ? \
R_RTC_CreateWarmAll( (a), (b), (c), (d) ): \
ReturnFalse() \
)

#endif
/* End of file */
