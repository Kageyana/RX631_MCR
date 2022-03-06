/*""FILE COMMENT""*************************************************************
* System Name	: Serial Communication Interface API for RX63Nxx
* File Name		: r_pdl_sci_RX63Nxx.h
* Version		: 1.21
* Contents		: Serial Interface API header
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

#ifndef R_PDL_SCI_RX63Nxx_H
#define R_PDL_SCI_RX63Nxx_H

#define SCI_NUM_CHANNELS 13


#if defined DEVICE_PACKAGE_48_PIN
	#define SCI_INVALID_CHANNEL(a) \
	( \
		(((a) == 0) || ((a) == 2) || ((a) == 3) || ((a) == 4) || ((a) == 7) || ((a) == 9) || \
		 ((a) == 10) || ((a) == 11) || ((a) >= SCI_NUM_CHANNELS) ) ? \
		true : false \
	)
#endif

#if defined DEVICE_PACKAGE_64_PIN
	#define SCI_INVALID_CHANNEL(a) \
	( \
		(((a) == 0) || ((a) == 2) || ((a) == 3) || ((a) == 4) || ((a) == 7) || ((a) == 10) || \
		 ((a) == 11) || ((a) >= SCI_NUM_CHANNELS) ) ? \
		true : false \
	)
#endif

#if defined DEVICE_PACKAGE_100_PIN
	#define SCI_INVALID_CHANNEL(a) \
	( \
		(((a) == 4) || ((a) == 7) || ((a) == 10) || ((a) == 11) || ((a) >= SCI_NUM_CHANNELS) ) ? \
		true : false \
	)
#endif

#if defined (DEVICE_PACKAGE_144_PIN) || defined (DEVICE_PACKAGE_145_PIN) || \
	defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_177_PIN)
	#define SCI_INVALID_CHANNEL(a) \
	( \
		(a) >= SCI_NUM_CHANNELS ? \
		true : false \
	)
#endif


typedef enum _RPDL_MP_MODE
{
	RPDL_MP_MODE_DISABLED,
	RPDL_MP_MODE_ENABLED,
	RPDL_MP_MODE_EXPECTING_ID
}RPDL_MP_MODE;

typedef struct _RPDL_SCI_MP_SPECIFIC
{
	volatile RPDL_MP_MODE MP_mode;
	volatile uint8_t MP_rx_stationID;
}RPDL_SCI_MP_SPECIFIC;

typedef struct _RPDL_SCI_IIC_SPECIFIC
{
	volatile bool bReStart;
	volatile bool bStop;
}RPDL_SCI_IIC_SPECIFIC;

typedef struct _RPDL_SCI_DATA_GLOBAL
{
	volatile const uint8_t * tx_string_pointer;
	volatile uint8_t * rx_string_pointer;
	volatile uint8_t * rx_string_start;
	volatile uint16_t rx_counter;
	volatile uint16_t tx_counter;
	volatile uint16_t rx_threshold;
	volatile uint16_t tx_threshold;
	volatile uint8_t tx_using_irq;
	volatile uint8_t rx_using_irq;
	volatile uint8_t rx_continuous;
	volatile uint8_t sync_simultaneous;
	volatile uint32_t baud_rate;
	/*union used  as can never be in MP and IIC mode.*/
	union
	{
		RPDL_SCI_MP_SPECIFIC MP;
		RPDL_SCI_IIC_SPECIFIC IIC;		
	};

	/* Callback functions pointer storage*/
	VoidCallBackFunc TX_End_callback_func;
	VoidCallBackFunc RX_End_callback_func;
	VoidCallBackFunc RX_Error_callback_func;
}RPDL_SCI_DATA_GLOBAL;

/* Globals */
extern RPDL_SCI_DATA_GLOBAL  sci_global[];

/* Library prototypes */
bool R_SCI_SetAll(
	const uint8_t,
	const uint16_t
);
bool R_SCI_CreateAll(
	const uint8_t,
	const uint32_t,
	const uint32_t,
	const uint8_t,
	const uint8_t
);
bool R_SCI_CreateAsyncAll(
	const uint8_t,
	const uint32_t,
	const uint32_t,
	const uint8_t,
	const uint8_t
);
bool R_SCI_CreateSyncAll(
	const uint8_t,
	const uint32_t,
	const uint32_t,
	const uint8_t,
	const uint8_t
);
bool R_SCI_CreateSmartAll(
	const uint8_t,
	const uint32_t,
	const uint32_t,
	const uint8_t,
	const uint8_t
);
bool R_SCI_DestroyAll(
	const uint8_t
);
bool R_SCI_SendAll(
	const uint8_t,
	const uint16_t,
	volatile const uint8_t * const,
	const uint16_t,
	VoidCallBackFunc const
);
bool R_SCI_SendStationID(
	const uint8_t,
	const uint16_t 
);
bool R_SCI_SendData(
	const uint8_t,
	const uint16_t,
	volatile const uint8_t * const,
	const uint16_t,
	VoidCallBackFunc const
);
bool R_SCI_ReceiveAll(
	const uint8_t,
	const uint16_t,
	volatile uint8_t * const,
	const uint16_t,
	VoidCallBackFunc const,
	VoidCallBackFunc const
);
bool R_SCI_ReceiveStationID(
	const uint8_t,
	const uint16_t,
	VoidCallBackFunc const,
	VoidCallBackFunc const
);
bool R_SCI_ReceiveData(
	const uint8_t,
	const uint16_t,
	volatile uint8_t * const,
	const uint16_t,
	VoidCallBackFunc const,
	VoidCallBackFunc const
);
bool R_SCI_ControlAll(
	const uint8_t,
	const uint16_t
);
bool R_SCI_GetStatusAll(
	const uint8_t,
	volatile uint8_t * const,
	volatile uint8_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const
);
bool ReturnFalse(void);

/*SPI uses this rather than Recieve/Send*/
bool R_SCI_SPI_TransferAll(
	const uint8_t,
	const uint16_t,
	const uint16_t,
	volatile const uint8_t * const,
	VoidCallBackFunc const,
	volatile uint8_t * const,
	VoidCallBackFunc const,
	VoidCallBackFunc const
);
/*IIC uses these rather than Recieve/Send*/
bool R_SCI_IIC_WriteAll(
	const uint8_t,
	const uint16_t,
	const uint16_t,
	const uint16_t,
	volatile const uint8_t *,
	VoidCallBackFunc const
);
bool R_SCI_IIC_ReadAll(
	const uint8_t,
	const uint16_t,
	const uint16_t,
	const uint16_t,
	volatile uint8_t * const,
	VoidCallBackFunc const
);
	
bool R_SCI_IIC_ReadLastByteAll(const uint8_t,
							volatile uint8_t * const);

/* Macro definitions */
#define R_SCI_Set(a, b) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_SetAll( (a), (b) ) \
)

#define R_SCI_Create(a, b, c, d, e) \
( \
( SCI_INVALID_CHANNEL(a) || ((c) == 0) || ((d) > IPL_MAX) || ((e) > IPL_MAX) ) ? \
ReturnFalse(): \
R_SCI_CreateAll( (a), (b), (c), (d), (e) )\
)

#define R_SCI_Destroy(a) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_DestroyAll( (a) ) \
)

#define R_SCI_Send(a, b, c, d, e) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_SendAll( (a), (b), (c), (d), (e) ) \
)

#define R_SCI_Receive(a, b, c, d, e, f) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_ReceiveAll( (a), (b), (c), (d), (e), (f) ) \
)

/*SPI uses this rather than R_SCI_Receive / R_SCI_Send*/
/*If a valid channel is supplied R_SCI_SPI_TransferAll will be called.*/
#define R_SCI_SPI_Transfer(a, b, c, d, e, f, g, h) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_SPI_TransferAll( (a), (b), (c), (d), (e), (f), (g), (h) ) \
)

/*IIC uses these rather than R_SCI_Receive / R_SCI_Send*/
/*If a valid channel is supplied R_SCI_IIC_WriteAll will be called.*/
#define R_SCI_IIC_Write(a, b, c, d, e, f) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_IIC_WriteAll( (a), (b), (c), (d), (e), (f) ) \
)

/*If a valid channel is supplied R_SCI_IIC_ReadAll will be called.*/
#define R_SCI_IIC_Read(a, b, c, d, e, f) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_IIC_ReadAll( (a), (b), (c), (d), (e), (f) ) \
)

/*If a valid channel is supplied R_SCI_IIC_ReadLastByteAll will be called.*/
#define R_SCI_IIC_ReadLastByte(a, b) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_IIC_ReadLastByteAll( (a), (b) ) \
)

#define R_SCI_Control(a, b) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_ControlAll( (a), (b) ) \
)

#define R_SCI_GetStatus(a, b, c, d, e) \
( \
SCI_INVALID_CHANNEL(a) ? \
ReturnFalse(): \
R_SCI_GetStatusAll( (a), (b), (c), (d), (e) ) \
)

#endif
/* End of file */
