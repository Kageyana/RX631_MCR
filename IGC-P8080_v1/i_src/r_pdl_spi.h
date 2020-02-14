/*""FILE COMMENT""*************************************************************
* System Name	: Serial Peripheral Interface API for RX63Nxx
* File Name		: r_pdl_spi.h
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

#ifndef R_PDL_SPI_H
#define R_PDL_SPI_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_SPI_Create(
	uint8_t,
	uint32_t,
	uint32_t,
	uint32_t,
	uint32_t
);
bool R_SPI_Destroy(
	uint8_t
);
bool R_SPI_Command(
	uint8_t,
	uint8_t,
	uint32_t,
	uint8_t
);
bool R_SPI_Transfer(
	uint8_t,
	uint8_t,
	uint32_t *,
	uint32_t *,
	uint16_t,
	void *,
	uint8_t,
	void *,
	uint8_t
);
bool R_SPI_Control(
	uint8_t,
	uint8_t,
	uint32_t
);
bool R_SPI_GetStatus(
	uint8_t,
	uint16_t *,
	uint16_t *
);

bool R_SPI_Set(
	uint8_t,
	uint32_t,
	uint32_t
);


/* Pin selection for channel 0*/
/* Pin RSPCKA selection*/
#define PDL_SPI_RSPCKA_PA5				0x00000001ul
#define PDL_SPI_RSPCKA_PB0				0x00000002ul
#define PDL_SPI_RSPCKA_PC5				0x00000004ul

/* Pin MOSIA selection*/
#define PDL_SPI_MOSIA_P16				0x00000008ul
#define PDL_SPI_MOSIA_PA6				0x00000010ul
#define PDL_SPI_MOSIA_PC6				0x00000020ul

/* Pin MISOA selection*/
#define PDL_SPI_MISOA_P17				0x00000040ul
#define PDL_SPI_MISOA_PA7				0x00000080ul
#define PDL_SPI_MISOA_PC7				0x00000100ul

/* Pin SSLA0 selection*/
#define PDL_SPI_SSLA0_PA4				0x00000200ul
#define PDL_SPI_SSLA0_PC4				0x00000400ul

/* Pin SSLA1 selection*/
#define PDL_SPI_SSLA1_PA0				0x00000800ul
#define PDL_SPI_SSLA1_PC0				0x00001000ul

/* Pin SSLA2 selection*/
#define PDL_SPI_SSLA2_PA1				0x00002000ul
#define PDL_SPI_SSLA2_PC1				0x00004000ul

/* Pin SSLA3 selection*/
#define PDL_SPI_SSLA3_PA2				0x00008000ul
#define PDL_SPI_SSLA3_PC2				0x00010000ul

/* Pin selection for channel 1*/
/* Pin RSPCKB selection*/
#define PDL_SPI_RSPCKB_P27				0x00000001ul
#define PDL_SPI_RSPCKB_PE1				0x00000002ul
#define PDL_SPI_RSPCKB_PE5				0x00000004ul

/* Pin MOSIB selection*/
#define PDL_SPI_MOSIB_P26				0x00000008ul
#define PDL_SPI_MOSIB_PE2				0x00000010ul
#define PDL_SPI_MOSIB_PE6				0x00000020ul

/* Pin MISOB selection*/
#define PDL_SPI_MISOB_P30				0x00000040ul
#define PDL_SPI_MISOB_PE3				0x00000080ul
#define PDL_SPI_MISOB_PE7				0x00000100ul

/* Pin SSLB0 selection*/
#define PDL_SPI_SSLB0_P31				0x00000200ul
#define PDL_SPI_SSLB0_PE4				0x00000400ul

/* Pin SSLB1 selection*/
#define PDL_SPI_SSLB1_P50				0x00000800ul
#define PDL_SPI_SSLB1_PE0				0x00001000ul

/* Pin SSLB2 selection*/
#define PDL_SPI_SSLB2_P51				0x00002000ul
#define PDL_SPI_SSLB2_PE1				0x00004000ul

/* Pin SSLB3 selection*/
#define PDL_SPI_SSLB3_P52				0x00008000ul
#define PDL_SPI_SSLB3_PE2				0x00010000ul

/* Pin selection for channel 2*/
#define PDL_SPI_SSLC0_PD4				0x00020000ul
#define PDL_SPI_SSLC1_PD5				0x00040000ul
#define PDL_SPI_SSLC2_PD6				0x00080000ul
#define PDL_SPI_SSLC3_PD7				0x00100000ul

/* Connection mode */
#define PDL_SPI_MODE_SPI_MASTER			0x00000001ul
#define PDL_SPI_MODE_SPI_MULTI_MASTER	0x00000002ul
#define PDL_SPI_MODE_SPI_SLAVE			0x00000004ul
#define PDL_SPI_MODE_SYNC_MASTER		0x00000008ul
#define PDL_SPI_MODE_SYNC_SLAVE			0x00000010ul

/* Reception control */
#define PDL_SPI_FULL_DUPLEX				0x00000020ul
#define PDL_SPI_TRANSMIT_ONLY			0x00000040ul

/* Pin selection and control */
#define PDL_SPI_PIN_SSL0_LOW			0x00000080ul
#define PDL_SPI_PIN_SSL0_HIGH			0x00000100ul
#define PDL_SPI_PIN_SSL1_LOW			0x00000200ul
#define PDL_SPI_PIN_SSL1_HIGH			0x00000400ul
#define PDL_SPI_PIN_SSL2_LOW			0x00000800ul
#define PDL_SPI_PIN_SSL2_HIGH			0x00001000ul
#define PDL_SPI_PIN_SSL3_LOW			0x00002000ul
#define PDL_SPI_PIN_SSL3_HIGH			0x00004000ul
#define PDL_SPI_PIN_MOSI_IDLE_LAST		0x00008000ul
#define PDL_SPI_PIN_MOSI_IDLE_LOW		0x00010000ul
#define PDL_SPI_PIN_MOSI_IDLE_HIGH		0x00020000ul

/* Buffer size */
#define PDL_SPI_BUFFER_64	0x00000001ul
#define PDL_SPI_BUFFER_128	0x00000002ul

/* Frame configuration selection  */
#define PDL_SPI_FRAME_1_1	0x00000004ul
#define PDL_SPI_FRAME_1_2	0x00000008ul
#define PDL_SPI_FRAME_1_3	0x00000010ul
#define PDL_SPI_FRAME_1_4	0x00000020ul
#define PDL_SPI_FRAME_2_1	0x00000040ul
#define PDL_SPI_FRAME_2_2	0x00000080ul
#define PDL_SPI_FRAME_3		0x00000100ul
#define PDL_SPI_FRAME_4		0x00000200ul
#define PDL_SPI_FRAME_5		0x00000400ul
#define PDL_SPI_FRAME_6		0x00000800ul
#define PDL_SPI_FRAME_7		0x00001000ul
#define PDL_SPI_FRAME_8		0x00002000ul

/* Parity bit control */
#define PDL_SPI_PARITY_NONE	0x00004000ul
#define PDL_SPI_PARITY_EVEN	0x00008000ul
#define PDL_SPI_PARITY_ODD	0x00010000ul

/* Extended clock delay */
#define PDL_SPI_CLOCK_DELAY_1	0x00000001ul
#define PDL_SPI_CLOCK_DELAY_2	0x00000002ul
#define PDL_SPI_CLOCK_DELAY_3	0x00000004ul
#define PDL_SPI_CLOCK_DELAY_4	0x00000008ul
#define PDL_SPI_CLOCK_DELAY_5	0x00000010ul
#define PDL_SPI_CLOCK_DELAY_6	0x00000020ul
#define PDL_SPI_CLOCK_DELAY_7	0x00000040ul
#define PDL_SPI_CLOCK_DELAY_8	0x00000080ul

/* Extended SSL negation delay */
#define PDL_SPI_SSL_DELAY_1		0x00000100ul
#define PDL_SPI_SSL_DELAY_2		0x00000200ul
#define PDL_SPI_SSL_DELAY_3		0x00000400ul
#define PDL_SPI_SSL_DELAY_4		0x00000800ul
#define PDL_SPI_SSL_DELAY_5		0x00001000ul
#define PDL_SPI_SSL_DELAY_6		0x00002000ul
#define PDL_SPI_SSL_DELAY_7		0x00004000ul
#define PDL_SPI_SSL_DELAY_8		0x00008000ul

/* Extended next-access delay */
#define PDL_SPI_NEXT_DELAY_1	0x00010000ul
#define PDL_SPI_NEXT_DELAY_2	0x00020000ul
#define PDL_SPI_NEXT_DELAY_3	0x00040000ul
#define PDL_SPI_NEXT_DELAY_4	0x00080000ul
#define PDL_SPI_NEXT_DELAY_5	0x00100000ul
#define PDL_SPI_NEXT_DELAY_6	0x00200000ul
#define PDL_SPI_NEXT_DELAY_7	0x00400000ul
#define PDL_SPI_NEXT_DELAY_8	0x00800000ul

/* Channel control */
#define PDL_SPI_DISABLE				0x01u

/* Loopback control */
#define PDL_SPI_LOOPBACK_DISABLE	0x02u
#define PDL_SPI_LOOPBACK_DIRECT		0x04u
#define PDL_SPI_LOOPBACK_REVERSED	0x08u

/* Clock phase and polarity */
#define PDL_SPI_CLOCK_MODE_0	0x00000001ul
#define PDL_SPI_CLOCK_MODE_1	0x00000002ul
#define PDL_SPI_CLOCK_MODE_2	0x00000004ul
#define PDL_SPI_CLOCK_MODE_3	0x00000008ul

/* Clock division */
#define PDL_SPI_DIV_1			0x00000010ul
#define PDL_SPI_DIV_2			0x00000020ul
#define PDL_SPI_DIV_4			0x00000040ul
#define PDL_SPI_DIV_8			0x00000080ul

/* SSL assertion */
#define PDL_SPI_ASSERT_SSL0		0x00000100ul
#define PDL_SPI_ASSERT_SSL1		0x00000200ul
#define PDL_SPI_ASSERT_SSL2		0x00000400ul
#define PDL_SPI_ASSERT_SSL3		0x00000800ul

/* SSL negation */
#define PDL_SPI_SSL_NEGATE		0x00001000ul
#define PDL_SPI_SSL_KEEP		0x00002000ul

/* Frame data length */
#define PDL_SPI_LENGTH_8		0x00004000ul
#define PDL_SPI_LENGTH_9		0x00008000ul
#define PDL_SPI_LENGTH_10		0x00010000ul
#define PDL_SPI_LENGTH_11		0x00020000ul
#define PDL_SPI_LENGTH_12		0x00040000ul
#define PDL_SPI_LENGTH_13		0x00080000ul
#define PDL_SPI_LENGTH_14		0x00100000ul
#define PDL_SPI_LENGTH_15		0x00200000ul
#define PDL_SPI_LENGTH_16		0x00400000ul
#define PDL_SPI_LENGTH_20		0x00800000ul
#define PDL_SPI_LENGTH_24		0x01000000ul
#define PDL_SPI_LENGTH_32		0x02000000ul

/* Data transfer format */
#define PDL_SPI_MSB_FIRST		0x04000000ul
#define PDL_SPI_LSB_FIRST		0x08000000ul

/* Extended timing selection */
#define PDL_SPI_CLOCK_DELAY_MINIMUM		0x01u
#define PDL_SPI_CLOCK_DELAY_EXTENDED	0x02u

/* Extended timing selection */
#define PDL_SPI_SSL_DELAY_MINIMUM		0x04u
#define PDL_SPI_SSL_DELAY_EXTENDED		0x08u

/* Next-access delay */
#define PDL_SPI_NEXT_DELAY_MINIMUM		0x10u
#define PDL_SPI_NEXT_DELAY_EXTENDED		0x20u

/* DMAC / DTC trigger control */
#define PDL_SPI_DMAC_DTC_TRIGGER_DISABLE	0x01u
#define PDL_SPI_DMAC_TRIGGER_ENABLE			0x02u
#define PDL_SPI_DTC_TRIGGER_ENABLE			0x04u

#endif
/* End of file */
