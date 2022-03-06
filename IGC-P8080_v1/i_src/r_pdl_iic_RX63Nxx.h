/*""FILE COMMENT""*************************************************************
* System Name	: IIC API for RX63Nxx
* File Name		: r_pdl_iic_RX63Nxx.h
* Version		: 1.21
* Contents		: IIC API header
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

#ifndef R_PDL_IIC_RX63Nxx_H
#define R_PDL_IIC_RX63Nxx_H

#define IIC_CHANNELS 4

/* Globals */
extern volatile uint8_t rpdl_IIC_stop[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_current_state[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_next_state[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_slave_address_lower[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_slave_address_upper[IIC_CHANNELS];
extern const volatile uint8_t * rpdl_IIC_tx_data_pointer[IIC_CHANNELS];
extern const volatile uint8_t * rpdl_IIC_tx_data_start[IIC_CHANNELS];
extern volatile uint8_t * rpdl_IIC_rx_data_pointer[IIC_CHANNELS];
extern volatile uint8_t * rpdl_IIC_rx_data_start[IIC_CHANNELS];
extern volatile uint16_t rpdl_IIC_rx_counter[IIC_CHANNELS];
extern volatile uint16_t rpdl_IIC_tx_counter[IIC_CHANNELS];
extern volatile uint16_t rpdl_IIC_rx_threshold[IIC_CHANNELS];
extern volatile uint16_t rpdl_IIC_tx_threshold[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_detected_address[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_rx_transfer_method[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_tx_transfer_method[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_rx_dmac_channel[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_tx_dmac_channel[IIC_CHANNELS];

/* IIC state machine states */
typedef enum {
	IIC_SEND_START,
	IIC_MASTER_SEND_SLAVE_ADDRESS_7,
	IIC_MASTER_SEND_SLAVE_ADDRESS_10a,
	IIC_MASTER_SEND_SLAVE_ADDRESS_10b,
	IIC_MASTER_SEND_SLAVE_ADDRESS_10c,
	IIC_MASTER_SEND_SLAVE_ADDRESS_10d,
	IIC_MASTER_SEND_DATA,
	IIC_MASTER_WAIT_FOR_TX_COMPLETE,
	IIC_MASTER_WAIT_FOR_STOP,
	IIC_EXIT_LOOP,
	IIC_MASTER_START_READ,
	IIC_MASTER_READ_DATA,
	IIC_SEND_STOP,
	IIC_SLAVE_MONITOR,
	IIC_SLAVE_SEND_DATA,
	IIC_SLAVE_READ_SLAVE_ADDRESS,
	IIC_SLAVE_READ_DATA
} iic_states;

/* Transfer options */
#define IIC_TRANSFER_CPU  0
#define IIC_TRANSFER_DMAC 2
#define IIC_TRANSFER_DTC  1

/* Callback function prototype */
extern VoidCallBackFunc	rpdl_IIC_callback_func[IIC_CHANNELS];

/*Utility function used from IIC interrupt handlers.*/
extern void rpdl_IIC_Reset_TimeOut_Counter(const uint8_t channel);

/* Library prototypes */
bool R_IIC_CreateAll(
	const uint8_t,
	const uint32_t,
	const uint32_t,
	const uint16_t,
	const uint16_t,
	const uint16_t,
	const uint32_t,
	const uint32_t
);
bool R_IIC_DestroyAll(
	const uint8_t
);
bool R_IIC_MasterSendAll(
	const uint8_t,
	const uint16_t,
	const uint16_t,
	const volatile uint8_t * const,
	const uint16_t,
	VoidCallBackFunc const,
	const uint8_t
);
bool R_IIC_MasterReceiveAll(
	const uint8_t,
	const uint16_t,
	const uint16_t,
	volatile uint8_t * const,
	const uint16_t,
	VoidCallBackFunc const,
	const uint8_t
);
bool R_IIC_MasterReceiveLastAll(
	const uint8_t,
	volatile uint8_t * const
);
bool R_IIC_SlaveMonitorAll(
	const uint8_t,
	const uint16_t,
	volatile uint8_t * const,
	const uint16_t,
	VoidCallBackFunc const,
	const uint8_t
);
bool R_IIC_SlaveSendAll(
	const uint8_t,
	const volatile uint8_t * const,
	const uint16_t
);
bool R_IIC_ControlAll(
	const uint8_t,
	const uint8_t
);
bool R_IIC_GetStatusAll(
	const uint8_t,
	volatile uint32_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

/*Only channel 2 avaialable in 48 and 64 pin packages*/
#if defined (DEVICE_PACKAGE_48_PIN) || defined (DEVICE_PACKAGE_64_PIN)
	#define IIC_VALID_CHANNEL(a) \
	( \
		((a) == 2)? true : false \
	)
#endif

/*Only channels 0 and 2 avaialable in 100 pin packages*/
#if defined (DEVICE_PACKAGE_100_PIN)
	#define IIC_VALID_CHANNEL(a) \
	( \
		( ((a) == 0) || ((a) == 2) ) ? true : false \
	)
#endif

#if defined (DEVICE_PACKAGE_144_PIN) || defined (DEVICE_PACKAGE_145_PIN) || \
	defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_177_PIN)
	#define IIC_VALID_CHANNEL(a) \
	( \
		((a) < IIC_CHANNELS) ? true : false \
	)
#endif

#define R_IIC_Create(a, b, c, d, e, f, g, h) \
( \
IIC_VALID_CHANNEL(a) ? \
R_IIC_CreateAll( (a), (b), (c), (d), (e), (f), (g), (h) ) : \
ReturnFalse() \
)

#define R_IIC_Destroy(a) \
( \
IIC_VALID_CHANNEL(a) ? \
R_IIC_DestroyAll( (a) ) : \
ReturnFalse() \
)

#define R_IIC_MasterSend(a, b, c, d, e, f, g) \
( \
( IIC_VALID_CHANNEL(a) && ((g) <= IPL_MAX) ) ? \
R_IIC_MasterSendAll( (a), (b), (c), (d), (e), (f), (g) ) : \
ReturnFalse() \
)

#define R_IIC_MasterReceive(a, b, c, d, e, f, g) \
( \
( IIC_VALID_CHANNEL(a) && ((g) <= IPL_MAX) ) ? \
R_IIC_MasterReceiveAll( (a), (b), (c), (d), (e), (f), (g) ) : \
ReturnFalse() \
)

#define R_IIC_MasterReceiveLast(a, b) \
( \
IIC_VALID_CHANNEL(a) ? \
R_IIC_MasterReceiveLastAll( (a), (b) ) : \
ReturnFalse() \
)

#define R_IIC_SlaveMonitor(a, b, c, d, e, f) \
( \
( IIC_VALID_CHANNEL(a) && ((f) <= IPL_MAX) ) ? \
R_IIC_SlaveMonitorAll( (a), (b), (c), (d), (e), (f) ) : \
ReturnFalse() \
)

#define R_IIC_SlaveSend(a, b, c) \
( \
IIC_VALID_CHANNEL(a) ? \
R_IIC_SlaveSendAll( (a), (b), (c) ) : \
ReturnFalse() \
)

#define R_IIC_Control(a, b) \
( \
IIC_VALID_CHANNEL(a) ? \
R_IIC_ControlAll( (a), (b) ) : \
ReturnFalse() \
)

#define R_IIC_GetStatus(a, b, c, d) \
( \
IIC_VALID_CHANNEL(a) ? \
R_IIC_GetStatusAll( (a), (b), (c), (d) ) : \
ReturnFalse() \
)

#endif
/* End of file */
