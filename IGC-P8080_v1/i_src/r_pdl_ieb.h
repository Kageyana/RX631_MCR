/*""FILE COMMENT""*************************************************************
* System Name	: IEBus API
* File Name		: r_pdl_ieb.h
* Version		: 1.21
* Contents		: IEB header
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

#ifndef R_PDL_IEB_H
#define R_PDL_IEB_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_IEB_Set(
	uint8_t
);

bool R_IEB_Create(
	uint8_t,
	uint32_t,
	uint16_t,
	uint8_t
);

bool R_IEB_Destroy(
	uint8_t
);

bool R_IEB_MasterSend(
	uint8_t,
	uint16_t,
	uint16_t,
	uint8_t *,
	uint8_t,
	void *
);

bool R_IEB_MasterReceive(
	uint8_t,
	uint16_t,
	uint16_t,
	uint8_t *,
	uint8_t *,
	void *
);

bool R_IEB_SlaveMonitor(
	uint8_t,
	uint8_t *,
	uint8_t *,
	void *
);

bool R_IEB_SlaveWrite(
	uint8_t,
	uint8_t *,
	uint8_t
);

bool R_IEB_Control(
	uint8_t,
	uint32_t,
	uint16_t
);

bool R_IEB_GetStatus(
	uint8_t,
	uint16_t *,
	uint8_t *,
	uint32_t *,
	uint16_t *,
	uint16_t *
);

/* IERXD pin selection */
#define PDL_IEB_PIN_IERXD_P16	0x01u
#define PDL_IEB_PIN_IERXD_PC2	0x02u

/* IETXD pin selection */
#define PDL_IEB_PIN_IETXD_P17	0x04u
#define PDL_IEB_PIN_IETXD_PC3	0x08u

/* Clock frequency type */
#define PDL_IEB_CLOCK_INTEGER			0x00000001ul
#define PDL_IEB_CLOCK_VGA				0x00000002ul
#define PDL_IEB_CLOCK_IECLK_DIV_2		0x00000004ul
#define PDL_IEB_CLOCK_IECLK_DIV_3		0x00000008ul
#define PDL_IEB_CLOCK_IECLK_DIV_4		0x00000010ul
#define PDL_IEB_CLOCK_IECLK_DIV_5		0x00000020ul
#define PDL_IEB_CLOCK_IECLK_DIV_6		0x00000040ul
#define PDL_IEB_CLOCK_IECLK_DIV_7		0x00000080ul

/* Communication mode */
#define PDL_IEB_MODE_0					0x00000100ul
#define PDL_IEB_MODE_1					0x00000200ul

/* Reception control */
#define PDL_IEB_RX_ENABLE				0x00000400ul
#define PDL_IEB_RX_DISABLE				0x00000800ul

/* Input / Output polarity control */
#define PDL_IEB_POLARITY_LOW			0x00001000ul
#define PDL_IEB_POLARITY_HIGH			0x00002000ul

/* Digital filter control */
#define PDL_IEB_FILTER_IECLK_DIV_1		0x00004000ul
#define PDL_IEB_FILTER_IECLK_DIV_2		0x00008000ul
#define PDL_IEB_FILTER_IECLK_DIV_3		0x00010000ul
#define PDL_IEB_FILTER_IECLK_DIV_4		0x00020000ul
#define PDL_IEB_FILTER_DISABLE			0x00040000ul

/* Reset control */
#define PDL_IEB_RESET_ENABLE			0x00080000ul
#define PDL_IEB_RESET_DISABLE			0x00100000ul

/* Slave state control */
#define PDL_IEB_SLAVE_TX_HALTED			0x00200000ul
#define PDL_IEB_SLAVE_TX_ENABLED		0x00400000ul

/* Broadcast receive error interrupt control */
#define PDL_IEB_BROADCAST_ERROR_ENABLE	0x00800000ul
#define PDL_IEB_BROADCAST_ERROR_DISABLE	0x01000000ul

/* Internal command control */
#define PDL_IEB_CANCEL_LOCK				0x02000000ul
#define PDL_IEB_CANCEL_TRANSFER			0x04000000ul

/* Unit address update control */
#define PDL_IEB_UPDATE_ADDRESS			0x08000000ul

/* Communication type */
#define PDL_IEB_NORMAL					0x0001u
#define PDL_IEB_BROADCAST				0x0002u

/* Data type and control (transmit only) */
#define PDL_IEB_COMMAND					0x0004u

/* Data type and control (transmit and receive) */
#define PDL_IEB_DATA					0x0008u

/* Data type and control (receive only) */
#define PDL_IEB_STATUS					0x0010u
#define PDL_IEB_LOCKED_ADDRESS_UPPER	0x0020u
#define PDL_IEB_LOCKED_ADDRESS_LOWER	0x0040u
#define PDL_IEB_UNLOCK					0x0080u

/* Re-transmission count */
#define PDL_IEB_RETRY_0					0x0100u
#define PDL_IEB_RETRY_1					0x0200u
#define PDL_IEB_RETRY_2					0x0400u
#define PDL_IEB_RETRY_3					0x0800u
#define PDL_IEB_RETRY_4					0x1000u
#define PDL_IEB_RETRY_5					0x2000u
#define PDL_IEB_RETRY_6					0x4000u
#define PDL_IEB_RETRY_7					0x8000u

#endif
/* End of file */
