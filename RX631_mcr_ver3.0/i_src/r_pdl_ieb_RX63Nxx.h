/*""FILE COMMENT""*************************************************************
* System Name	: IEBus API for RX63Nxx
* File Name		: r_pdl_ieb_RX63Nxx.h
* Version		: 1.21
* Contents		: IEB header
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

#ifndef R_PDL_IEB_RX63Nxx_H
#define R_PDL_IEB_RX63Nxx_H

#define IEB_CHANNELS 1

/* Callback function storage */
extern VoidCallBackFunc rpdl_IEB_callback_func;

/* Globals */
extern volatile uint8_t rpdl_IEB_ierxd_pin_selection;
extern volatile uint8_t rpdl_IEB_ietxd_pin_selection;
extern volatile uint8_t * rpdl_IEB_rx_data_start_ptr;
extern volatile uint8_t * rpdl_IEB_rx_data_length_ptr;

/* Library prototypes */

bool R_IEB_SetAll(
	const uint8_t
);

bool R_IEB_CreateAll(
	const uint32_t,
	const uint16_t,
	const uint8_t
);

bool R_IEB_DestroyAll(
	void
);

bool R_IEB_MasterSendAll(
	const uint16_t,
	const uint16_t,
	volatile const uint8_t * const,
	const uint8_t,
	VoidCallBackFunc const
);

bool R_IEB_MasterReceiveAll(
	const uint16_t,
	const uint16_t,
	volatile uint8_t * const,
	volatile uint8_t * const,
	VoidCallBackFunc const
);

bool R_IEB_SlaveMonitorAll(
	volatile uint8_t * const,
	volatile uint8_t * const,
	VoidCallBackFunc const
);

bool R_IEB_SlaveWriteAll(
	volatile const uint8_t * const,
	const uint8_t
);

bool R_IEB_ControlAll(
	const uint32_t,
	const uint16_t
);

bool R_IEB_GetStatusAll(
	volatile uint16_t * const,
	volatile uint8_t * const,
	volatile uint32_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const
);

/* Macro definitions */

#define R_IEB_Set(a) \
R_IEB_SetAll( (a) )

#define R_IEB_Create(a, b, c, d) \
( \
( ( ((a) < IEB_CHANNELS) && ((d) <= IPL_MAX) ) ) ? \
R_IEB_CreateAll( (b), (c), (d) ): \
ReturnFalse() \
)

#define R_IEB_Destroy(a) \
( \
( (a) < IEB_CHANNELS ) ? \
R_IEB_DestroyAll(): \
ReturnFalse() \
)

#define R_IEB_MasterSend(a, b, c, d, e, f) \
( \
( (a) < IEB_CHANNELS ) ? \
R_IEB_MasterSendAll( (b), (c), (d), (e), (f) ): \
ReturnFalse() \
)

#define R_IEB_MasterReceive(a, b, c, d, e, f) \
( \
( (a) < IEB_CHANNELS ) ? \
R_IEB_MasterReceiveAll( (b), (c), (d), (e), (f) ): \
ReturnFalse() \
)

#define R_IEB_SlaveMonitor(a, b, c, d) \
( \
( (a) < IEB_CHANNELS ) ? \
R_IEB_SlaveMonitorAll( (b), (c), (d) ): \
ReturnFalse() \
)

#define R_IEB_SlaveWrite(a, b, c) \
( \
( (a) < IEB_CHANNELS ) ? \
R_IEB_SlaveWriteAll( (b), (c) ): \
ReturnFalse() \
)

#define R_IEB_Control(a, b, c) \
( \
( (a) < IEB_CHANNELS ) ? \
R_IEB_ControlAll( (b), (c) ): \
ReturnFalse() \
)

#define R_IEB_GetStatus(a, b, c, d, e, f) \
( \
( (a) < IEB_CHANNELS ) ? \
R_IEB_GetStatusAll( (b), (c), (d), (e), (f) ): \
ReturnFalse() \
)

#endif
/* End of file */
