/*""FILE COMMENT""*************************************************************
* System Name	: Serial Peripheral Interface API for RX63Nxx
* File Name		: r_pdl_spi_RX63Nxx.h
* Version		: 1.21
* Contents		: SPI API header
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

#ifndef R_PDL_SPI_RX63Nxx_H
#define R_PDL_SPI_RX63Nxx_H

#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) ||\
	defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)
#define SPI_CHANNELS 3
#else
#define SPI_CHANNELS 2
#endif

#define SPI_COMMANDS 8

/* Globals */
extern volatile uint8_t rpdl_SPI_method[SPI_CHANNELS];
extern volatile uint16_t rpdl_SPI_sequence_count[SPI_CHANNELS];
extern volatile uint16_t rpdl_SPI_tx_sequence_counter[SPI_CHANNELS];
extern volatile uint16_t rpdl_SPI_rx_sequence_counter[SPI_CHANNELS];
extern volatile uint8_t rpdl_SPI_tx_frame_counter[SPI_CHANNELS];
extern volatile uint8_t rpdl_SPI_frame_total[SPI_CHANNELS];
extern volatile uint8_t rpdl_SPI_rx_frame_counter[SPI_CHANNELS];
extern volatile const uint32_t * rpdl_SPI_tx_ptr[SPI_CHANNELS];
extern volatile uint32_t * rpdl_SPI_rx_ptr[SPI_CHANNELS];

/* Callback function prototypes */
extern VoidCallBackFunc rpdl_SPI_event_callback_func[SPI_CHANNELS];
extern VoidCallBackFunc rpdl_SPI_error_callback_func[SPI_CHANNELS];

/* Request trigger options */
typedef enum {
	SPI_USING_POLLING,
	SPI_USING_IRQ,
	SPI_USING_DMAC_DTC
} rpdl_spi_transfer_options;

bool R_SPI_CreateAll(
	const uint8_t,
	const uint32_t,
	const uint32_t,
	const uint32_t,
	const uint32_t
);
bool R_SPI_DestroyAll(
	const uint8_t
);
bool R_SPI_CommandAll(
	const uint8_t,
	const uint8_t,
	const uint32_t,
	const uint8_t
);
bool R_SPI_TransferAll(
	const uint8_t,
	const uint8_t,
	volatile const uint32_t * const,
	volatile uint32_t * const,
	const uint16_t,
	VoidCallBackFunc const,
	const uint8_t,
	VoidCallBackFunc const,
	const uint8_t
);
bool R_SPI_ControlAll(
	const uint8_t,
	const uint8_t,
	const uint32_t
);
bool R_SPI_GetStatusAll(
	const uint8_t,
	volatile uint16_t * const,
	volatile uint16_t * const
);
bool R_SPI_SetAll(
	const uint8_t,
	const uint32_t,
	const uint32_t
);

bool ReturnFalse(void);

/* Macro definitions */
#define R_SPI_Create(a, b, c, d, e) \
( \
( (a) < SPI_CHANNELS) ? \
R_SPI_CreateAll( (a), (b), (c), (d), (e) ): \
ReturnFalse() \
)

#define R_SPI_Destroy(a) \
( \
( (a) < SPI_CHANNELS) ? \
R_SPI_DestroyAll( (a) ): \
ReturnFalse() \
)

#define R_SPI_Command(a, b, c, d) \
( \
( ( ((a) < SPI_CHANNELS) && ((b) < SPI_COMMANDS) ) ) ? \
R_SPI_CommandAll( (a), (b), (c), (d) ): \
ReturnFalse() \
)

#define R_SPI_Transfer(a, b, c, d, e, f, g, h, i) \
( \
( ( ((a) < SPI_CHANNELS) && ((g) <= IPL_MAX) && ((i) <= IPL_MAX) ) ) ? \
R_SPI_TransferAll( (a), (b), (c), (d), (e), (f), (g), (h), (i) ): \
ReturnFalse() \
)

#define R_SPI_Control(a, b, c) \
( \
( (a) < SPI_CHANNELS) ? \
R_SPI_ControlAll( (a), (b), (c) ): \
ReturnFalse() \
)

#define R_SPI_GetStatus(a, b, c) \
( \
( (a) < SPI_CHANNELS) ? \
R_SPI_GetStatusAll( (a), (b), (c) ): \
ReturnFalse() \
)

#define R_SPI_Set(a, b, c) \
( \
( (a) < SPI_CHANNELS) ? \
R_SPI_SetAll( (a), (b), (c) ): \
ReturnFalse() \
)

#endif
/* End of file */
