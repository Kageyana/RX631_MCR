/*""FILE COMMENT""*************************************************************
* System Name	: I/O Port API for RX63N
* File Name		: r_pdl_io_port_RX63Nxx.h
* Version		: 1.21
* Contents		: I/O Port API header
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

#ifndef R_PDL_IO_PORT_RX63N_H
#define R_PDL_IO_PORT_RX63N_H

/* Library prototypes */
bool R_IO_PORT_SetAll(
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint16_t
);
bool R_IO_PORT_ReadControlAll(
	const uint8_t,
	const uint8_t,
	const uint8_t,
	volatile uint16_t * const
);
bool R_IO_PORT_ReadControlRegisterAll(
	const uint8_t,
	const uint8_t,
	volatile uint16_t * const
);
bool R_IO_PORT_ModifyControlAll(
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint16_t
);
bool R_IO_PORT_ModifyControlRegisterAll(
	const uint8_t,
	const uint8_t,
	const uint16_t
);
bool R_IO_PORT_ReadDataAll(
	const uint8_t,
	const uint8_t,
	volatile uint8_t * const
);
bool R_IO_PORT_ReadDataRegisterAll(
	const uint8_t,
	volatile uint8_t * const
);
bool R_IO_PORT_WriteAll(
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t
);
bool R_IO_PORT_WriteRegisterAll(
	const uint8_t,
	const uint8_t
);
bool R_IO_PORT_CompareAll(
	const uint8_t,
	const uint8_t,
	const uint8_t,
	VoidCallBackFunc const
);
bool R_IO_PORT_CompareRegisterAll(
	const uint8_t,
	const uint8_t,
	VoidCallBackFunc const
);
bool R_IO_PORT_ModifyDataAll(
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint16_t,
	const uint8_t
);
bool R_IO_PORT_ModifyDataRegisterAll(
	const uint8_t,
	const uint16_t,
	const uint8_t
);
bool R_IO_PORT_WaitAll(
	const uint8_t,
	const uint8_t,
	const uint8_t
);
bool R_IO_PORT_WaitRegisterAll(
	const uint8_t,
	const uint8_t
);
bool R_IO_PORT_NotAvailableAll(
	void
);
bool R_IO_PORT_SwitchAll(
	const uint16_t
);

bool ReturnFalse(void);

/* The following limits allow the API function macros to decide if a
whole port is being controlled or just a pin.
They do not take into account pin package sizes. */

/* Limits used by the macro definitions */
#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN)
 #define LOWER_PORT_LIMIT	PDL_IO_PORT_0
 #define LOWER_PIN_LIMIT	PDL_IO_PORT_0_0
 #define UPPER_PORT_LIMIT	PDL_IO_PORT_J
 #define UPPER_PIN_LIMIT	(PDL_IO_PORT_J_3 | PDL_IO_PORT_J_5)
#endif

#if defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)
 #define LOWER_PORT_LIMIT	PDL_IO_PORT_0
 #define LOWER_PIN_LIMIT	PDL_IO_PORT_0_0
 #define UPPER_PORT_LIMIT	PDL_IO_PORT_J
 #define UPPER_PIN_LIMIT	(PDL_IO_PORT_J_3 | PDL_IO_PORT_J_5)
#endif

#ifdef DEVICE_PACKAGE_100_PIN
 #define LOWER_PORT_LIMIT	PDL_IO_PORT_0
 #define LOWER_PIN_LIMIT	PDL_IO_PORT_0_5
 #define UPPER_PORT_LIMIT	PDL_IO_PORT_J
 #define UPPER_PIN_LIMIT 	PDL_IO_PORT_J_3
#endif

#ifdef DEVICE_PACKAGE_64_PIN
 #define LOWER_PORT_LIMIT   PDL_IO_PORT_0
 #define LOWER_PIN_LIMIT    PDL_IO_PORT_0_5
 #define UPPER_PORT_LIMIT   PDL_IO_PORT_E
 #define UPPER_PIN_LIMIT    (PDL_IO_PORT_E_0 | PDL_IO_PORT_E_1 | PDL_IO_PORT_E_2 | PDL_IO_PORT_E_3 | PDL_IO_PORT_E_4 | PDL_IO_PORT_E_5)
#endif

#ifdef DEVICE_PACKAGE_48_PIN
 #define LOWER_PORT_LIMIT   PDL_IO_PORT_1
 #define LOWER_PIN_LIMIT    PDL_IO_PORT_1_4
 #define UPPER_PORT_LIMIT   PDL_IO_PORT_E
 #define UPPER_PIN_LIMIT    (PDL_IO_PORT_E_1 | PDL_IO_PORT_E_2 | PDL_IO_PORT_E_3 | PDL_IO_PORT_E_4)
#endif

/* Macro definitions */

/* R_IO_PORT_Set */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Set(a, b) \
( \
( ((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT) ) ? \
R_IO_PORT_SetAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)~(a), (b) ) : \
ReturnFalse() \
)

/* R_IO_PORT_ReadControl */
/* Call the function if the port number is valid. */
#define R_IO_PORT_ReadControl(a, b, c) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_ReadControlAll( (uint8_t)((a) >> 8), (uint8_t)(a), (b), (c) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_ReadControlRegisterAll( (uint8_t)(a), (b), (c) ) : \
ReturnFalse() \
)

/* R_IO_PORT_ModifyControl */
/* Call the function if the port number is valid. */
#define R_IO_PORT_ModifyControl(a, b, c) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_ModifyControlAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)~(a), (b), (c) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_ModifyControlRegisterAll( (uint8_t)(a), (b), (c) ) : \
ReturnFalse() \
)

/* R_IO_PORT_Read */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Read(a, b) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_ReadDataAll( (uint8_t)((a) >> 8), (uint8_t)(a), (b) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_ReadDataRegisterAll( (uint8_t)(a), (b) ) : \
ReturnFalse() \
)

/* R_IO_PORT_Write */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Write(a, b) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_WriteAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)~(a), (b) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_WriteRegisterAll( (uint8_t)(a), (b) ): \
ReturnFalse() \
)

/* R_IO_PORT_Compare */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Compare(a, b, c) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_CompareAll( (uint8_t)((a) >> 8), (uint8_t)(a), (b), (c) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_CompareRegisterAll( (uint8_t)(a), (b), (c) ): \
ReturnFalse() \
)

/* R_IO_PORT_Modify */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Modify(a, b, c) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_ModifyDataAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)~(a), (b), (c) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_ModifyDataRegisterAll( (uint8_t)(a), (b), (c) ): \
ReturnFalse() \
)

/* R_IO_PORT_Wait */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Wait(a, b) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) && (b == 1u) ) ? \
R_IO_PORT_WaitAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)(a) ) : \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) && (b == 0u) ) ? \
R_IO_PORT_WaitAll( (uint8_t)((a) >> 8), (uint8_t)(a), 0u ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_WaitRegisterAll( (uint8_t)(a), (b) ) : \
ReturnFalse() \
)

/* R_IO_PORT_NotAvailable */
#define R_IO_PORT_NotAvailable() \
( \
R_IO_PORT_NotAvailableAll() \
)

/* R_IO_PORT_Switch */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Switch(a) \
( \
R_IO_PORT_SwitchAll((a)) \
)

#endif
/* End of file */
