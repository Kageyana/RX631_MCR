/*""FILE COMMENT""*************************************************************
* System Name	: Serial Communication Interface API for RX63Nxx
* File Name		: r_pdl_sci.h
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

#ifndef R_PDL_SCI_H
#define R_PDL_SCI_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_SCI_Set(
	uint8_t,
	uint16_t
);
bool R_SCI_Create(
	uint8_t,
	uint32_t,
	uint32_t,
	uint8_t,
	uint8_t
);
bool R_SCI_Destroy(
	uint8_t
);
bool R_SCI_Send(
	uint8_t,
	uint16_t,
	uint8_t *,
	uint16_t,
	void *
);
bool R_SCI_Receive(
	uint8_t,
	uint16_t,
	uint8_t *,
	uint16_t,
	void *,
	void *
);
bool R_SCI_Control(
	uint8_t,
	uint8_t
);
bool R_SCI_GetStatus(
	uint8_t,
	uint8_t *,
	uint8_t *,
	uint16_t *,
	uint16_t *
);
bool R_SCI_SPI_Transfer(
	uint8_t,
	uint16_t,
	uint16_t,
	uint8_t*,
	void *,
	uint8_t*,
	void *,
	void *
);
bool R_SCI_IIC_Write(
	uint8_t,
	uint16_t,
	uint16_t,
	uint16_t,
	uint8_t *,
	void *
);
bool R_SCI_IIC_Read(
	uint8_t,
	uint16_t,
	uint16_t,
	uint16_t,
	uint8_t *,
	void *
);
bool R_SCI_IIC_ReadLastByte(
	uint8_t,
	uint8_t * 
);

/* ------------------------ */
/* R_SCI_Set, Configuration */
/* ------------------------ */
/* Pin selection (See HW manual Table 20.1)*/


/***SCI0***/


/*RXD0 (SMISO0, SSCL0) */
#define PDL_SCI_PIN_SCI0_RXD0_P21		0x0001
#define PDL_SCI_PIN_SCI0_RXD0_P33		0x0002
/*Duplicate pin names*/
/*SMISO0*/
#define PDL_SCI_PIN_SCI0_SMISO0_P21 	PDL_SCI_PIN_SCI0_RXD0_P21
#define PDL_SCI_PIN_SCI0_SMISO0_P33 	PDL_SCI_PIN_SCI0_RXD0_P33
/*SSCL01*/
#define PDL_SCI_PIN_SCI0_SSCL0_P21		PDL_SCI_PIN_SCI0_RXD0_P21
#define PDL_SCI_PIN_SCI0_SSCL0_P33 		PDL_SCI_PIN_SCI0_RXD0_P33

/*TXD0 (SMOSI0, SSDA0)*/
#define PDL_SCI_PIN_SCI0_TXD0_P20		0x0004
#define PDL_SCI_PIN_SCI0_TXD0_P32		0x0008
/*Duplicate pin names*/
/*SMOSI0*/
#define PDL_SCI_PIN_SCI0_SMOSI0_P20 	PDL_SCI_PIN_SCI0_TXD0_P20
#define PDL_SCI_PIN_SCI0_SMOSI0_P32 	PDL_SCI_PIN_SCI0_TXD0_P32
/*SSDA0*/
#define PDL_SCI_PIN_SCI0_SSDA0_P20		PDL_SCI_PIN_SCI0_TXD0_P20
#define PDL_SCI_PIN_SCI0_SSDA0_P32 		PDL_SCI_PIN_SCI0_TXD0_P32

/*SCK0*/
#define PDL_SCI_PIN_SCI0_SCK0_P22		0x0010
#define PDL_SCI_PIN_SCI0_SCK0_P34		0x0020

/*CTS0 (RTS0,SS0) */
#define PDL_SCI_PIN_SCI0_CTS0_P23		0x0040
#define PDL_SCI_PIN_SCI0_CTS0_PJ3		0x0080
/*Duplicate pin names*/
/*RTS0*/
#define PDL_SCI_PIN_SCI0_RTS0_P23		PDL_SCI_PIN_SCI0_CTS0_P23
#define PDL_SCI_PIN_SCI0_RTS0_PJ3		PDL_SCI_PIN_SCI0_CTS0_PJ3
/*SS0*/
#define PDL_SCI_PIN_SCI0_SS0_P23		PDL_SCI_PIN_SCI0_CTS0_P23
#define PDL_SCI_PIN_SCI0_SS0_PJ3		PDL_SCI_PIN_SCI0_CTS0_PJ3


/***SCI1***/


/*RXD1 (SMISO1, SSCL1) */
#define PDL_SCI_PIN_SCI1_RXD1_P15		0x0001
#define PDL_SCI_PIN_SCI1_RXD1_P30		0x0002
#define PDL_SCI_PIN_SCI1_RXD1_PF2		0x0004
/*Duplicate pin names*/
/*SMISO1*/
#define PDL_SCI_PIN_SCI1_SMISO1_P15 	PDL_SCI_PIN_SCI1_RXD1_P15
#define PDL_SCI_PIN_SCI1_SMISO1_P30 	PDL_SCI_PIN_SCI1_RXD1_P30
#define PDL_SCI_PIN_SCI1_SMISO1_PF2 	PDL_SCI_PIN_SCI1_RXD1_PF2
/*SSCL1*/
#define PDL_SCI_PIN_SCI1_SSCL1_P15		PDL_SCI_PIN_SCI1_RXD1_P15
#define PDL_SCI_PIN_SCI1_SSCL1_P30 		PDL_SCI_PIN_SCI1_RXD1_P30
#define PDL_SCI_PIN_SCI1_SSCL1_PF2 		PDL_SCI_PIN_SCI1_RXD1_PF2

/*TXD1 (SMOSI1, SSDA1)*/
#define PDL_SCI_PIN_SCI1_TXD1_P16		0x0008
#define PDL_SCI_PIN_SCI1_TXD1_P26		0x0010
#define PDL_SCI_PIN_SCI1_TXD1_PF0		0x0020
/*Duplicate pin names*/
/*SMOSI1*/
#define PDL_SCI_PIN_SCI1_SMOSI1_P16 	PDL_SCI_PIN_SCI1_TXD1_P16
#define PDL_SCI_PIN_SCI1_SMOSI1_P26 	PDL_SCI_PIN_SCI1_TXD1_P26
#define PDL_SCI_PIN_SCI1_SMOSI1_PF0 	PDL_SCI_PIN_SCI1_TXD1_PF0
/*SSDA1*/
#define PDL_SCI_PIN_SCI1_SSDA1_P16		PDL_SCI_PIN_SCI1_TXD1_P16
#define PDL_SCI_PIN_SCI1_SSDA1_P26 		PDL_SCI_PIN_SCI1_TXD1_P26
#define PDL_SCI_PIN_SCI1_SSDA1_PF0 		PDL_SCI_PIN_SCI1_TXD1_PF0

/*SCK1*/
#define PDL_SCI_PIN_SCI1_SCK1_P17		0x0040
#define PDL_SCI_PIN_SCI1_SCK1_P27		0x0080
#define PDL_SCI_PIN_SCI1_SCK1_PF1		0x0100

/*CTS1 (RTS1,SS1) */
#define PDL_SCI_PIN_SCI1_CTS1_P14		0x0200
#define PDL_SCI_PIN_SCI1_CTS1_P31		0x0400
/*Duplicate pin names*/
/*RTS1*/
#define PDL_SCI_PIN_SCI1_RTS1_P14		PDL_SCI_PIN_SCI1_CTS1_P14
#define PDL_SCI_PIN_SCI1_RTS1_P31		PDL_SCI_PIN_SCI1_CTS1_P31
/*SS1*/
#define PDL_SCI_PIN_SCI1_SS1_P14		PDL_SCI_PIN_SCI1_CTS1_P14
#define PDL_SCI_PIN_SCI1_SS1_P31		PDL_SCI_PIN_SCI1_CTS1_P31


/***SCI2***/


/*RXD2 (SMISO2, SSCL2) */
#define PDL_SCI_PIN_SCI2_RXD2_P12		0x0001
#define PDL_SCI_PIN_SCI2_RXD2_P52		0x0002
/*Duplicate pin names*/
/*SMISO2*/
#define PDL_SCI_PIN_SCI2_SMISO2_P12 	PDL_SCI_PIN_SCI2_RXD2_P12
#define PDL_SCI_PIN_SCI2_SMISO2_P52 	PDL_SCI_PIN_SCI2_RXD2_P52
/*SSCL2*/
#define PDL_SCI_PIN_SCI2_SSCL2_P12		PDL_SCI_PIN_SCI2_RXD2_P12
#define PDL_SCI_PIN_SCI2_SSCL2_P52 		PDL_SCI_PIN_SCI2_RXD2_P52

/*TXD2 (SMOSI2, SSDA2)*/
#define PDL_SCI_PIN_SCI2_TXD2_P13		0x0004
#define PDL_SCI_PIN_SCI2_TXD2_P50		0x0008
/*Duplicate pin names*/
/*SMOSI2*/
#define PDL_SCI_PIN_SCI2_SMOSI2_P13 	PDL_SCI_PIN_SCI2_TXD2_P13
#define PDL_SCI_PIN_SCI2_SMOSI2_P50 	PDL_SCI_PIN_SCI2_TXD2_P50
/*SSDA2*/
#define PDL_SCI_PIN_SCI2_SSDA2_P13		PDL_SCI_PIN_SCI2_TXD2_P13
#define PDL_SCI_PIN_SCI2_SSDA2_P50 		PDL_SCI_PIN_SCI2_TXD2_P50

/*SCK2*/
#define PDL_SCI_PIN_SCI2_SCK2_P11		0x0010
#define PDL_SCI_PIN_SCI2_SCK2_P51		0x0020

/*CTS2 (RTS2,SS2) */
#define PDL_SCI_PIN_SCI2_CTS2_P54		0x0040
/*Duplicate pin names*/
/*RTS2*/
#define PDL_SCI_PIN_SCI2_RTS2_P54		PDL_SCI_PIN_SCI2_CTS2_P54
/*SS2*/
#define PDL_SCI_PIN_SCI2_SS2_P54		PDL_SCI_PIN_SCI2_CTS2_P54


/***SCI3***/


/*RXD3 (SMISO3, SSCL3) */
#define PDL_SCI_PIN_SCI3_RXD3_P16		0x0001
#define PDL_SCI_PIN_SCI3_RXD3_P25		0x0002
/*Duplicate pin names*/
/*SMISO3*/
#define PDL_SCI_PIN_SCI3_SMISO3_P16 	PDL_SCI_PIN_SCI3_RXD3_P16
#define PDL_SCI_PIN_SCI3_SMISO3_P25 	PDL_SCI_PIN_SCI3_RXD3_P25
/*SSCL3*/
#define PDL_SCI_PIN_SCI3_SSCL3_P16		PDL_SCI_PIN_SCI3_RXD3_P16
#define PDL_SCI_PIN_SCI3_SSCL3_P25 		PDL_SCI_PIN_SCI3_RXD3_P25

/*TXD3 (SMOSI3, SSDA3)*/
#define PDL_SCI_PIN_SCI3_TXD3_P17		0x0004
#define PDL_SCI_PIN_SCI3_TXD3_P23		0x0008
/*Duplicate pin names*/
/*SMOSI3*/
#define PDL_SCI_PIN_SCI3_SMOSI3_P17 	PDL_SCI_PIN_SCI3_TXD3_P17
#define PDL_SCI_PIN_SCI3_SMOSI3_P23 	PDL_SCI_PIN_SCI3_TXD3_P23
/*SSDA3*/
#define PDL_SCI_PIN_SCI3_SSDA3_P17		PDL_SCI_PIN_SCI3_TXD3_P17
#define PDL_SCI_PIN_SCI3_SSDA3_P23 		PDL_SCI_PIN_SCI3_TXD3_P23

/*SCK3*/
#define PDL_SCI_PIN_SCI3_SCK3_P15		0x0010
#define PDL_SCI_PIN_SCI3_SCK3_P24		0x0020

/*CTS3 (RTS3SS3) */
#define PDL_SCI_PIN_SCI3_CTS3_P26		0x0040
/*Duplicate pin names*/
/*RTS3*/
#define PDL_SCI_PIN_SCI3_RTS3_P26		PDL_SCI_PIN_SCI3_CTS3_P26
/*SS3*/
#define PDL_SCI_PIN_SCI3_SS3_P26		PDL_SCI_PIN_SCI3_CTS3_P26


/***SCI4***/


/*RXD4 (SMISO4, SSCL4) */
#define PDL_SCI_PIN_SCI4_RXD4_PB0		0x0001
/*Duplicate pin names*/
/*SMISO4*/
#define PDL_SCI_PIN_SCI4_SMISO4_PB0 	PDL_SCI_PIN_SCI4_RXD4_PB0
/*SSCL3*/
#define PDL_SCI_PIN_SCI4_SSCL4_PB0		PDL_SCI_PIN_SCI4_RXD4_PB0

/*TXD4 (SMOSI4, SSDA4)*/
#define PDL_SCI_PIN_SCI4_TXD4_PB1		0x0004
/*Duplicate pin names*/
/*SMOSI4*/
#define PDL_SCI_PIN_SCI4_SMOSI4_PB1 	PDL_SCI_PIN_SCI4_TXD4_PB1
/*SSDA4*/
#define PDL_SCI_PIN_SCI4_SSDA4_PB1		PDL_SCI_PIN_SCI4_TXD4_PB1

/*SCK4*/
#define PDL_SCI_PIN_SCI4_SCK4_PB3		0x0020

/*CTS4 (RTS3SS4) */
#define PDL_SCI_PIN_SCI4_CTS4_PB2		0x0040
/*Duplicate pin names*/
/*RTS4*/
#define PDL_SCI_PIN_SCI4_RTS4_PB2		PDL_SCI_PIN_SCI4_CTS4_PB2
/*SS4*/
#define PDL_SCI_PIN_SCI4_SS4_PB2		PDL_SCI_PIN_SCI4_CTS4_PB2


/***SCI5***/


/*RXD5 (SMISO5, SSCL5) */
#define PDL_SCI_PIN_SCI5_RXD5_PA2		0x0001
#define PDL_SCI_PIN_SCI5_RXD5_PA3		0x0002
#define PDL_SCI_PIN_SCI5_RXD5_PC2		0x0004
/*Duplicate pin names*/
/*SMISO5*/
#define PDL_SCI_PIN_SCI5_SMISO5_PA2 	PDL_SCI_PIN_SCI5_RXD5_PA2
#define PDL_SCI_PIN_SCI5_SMISO5_PA3 	PDL_SCI_PIN_SCI5_RXD5_PA3
#define PDL_SCI_PIN_SCI5_SMISO5_PC2 	PDL_SCI_PIN_SCI5_RXD5_PC2
/*SSCL5*/
#define PDL_SCI_PIN_SCI5_SSCL5_PA2 		PDL_SCI_PIN_SCI5_RXD5_PA2
#define PDL_SCI_PIN_SCI5_SSCL5_PA3 		PDL_SCI_PIN_SCI5_RXD5_PA3
#define PDL_SCI_PIN_SCI5_SSCL5_PC2 		PDL_SCI_PIN_SCI5_RXD5_PC2

/*TXD5 (SMOSI5, SSDA5)*/
#define PDL_SCI_PIN_SCI5_TXD5_PA4		0x0008
#define PDL_SCI_PIN_SCI5_TXD5_PC3		0x0010
/*Duplicate pin names*/
/*SMOSI5*/
#define PDL_SCI_PIN_SCI5_SMOSI5_PA4 	PDL_SCI_PIN_SCI5_TXD5_PA4
#define PDL_SCI_PIN_SCI5_SMOSI5_PC3 	PDL_SCI_PIN_SCI5_TXD5_PC3
/*SSDA5*/
#define PDL_SCI_PIN_SCI5_SSDA5_PA4		PDL_SCI_PIN_SCI5_TXD5_PA4
#define PDL_SCI_PIN_SCI5_SSDA5_PC3 		PDL_SCI_PIN_SCI5_TXD5_PC3

/*SCK5*/
#define PDL_SCI_PIN_SCI5_SCK5_PA1		0x0020
#define PDL_SCI_PIN_SCI5_SCK5_PC1		0x0040
#define PDL_SCI_PIN_SCI5_SCK5_PC4		0x0080

/*CTS5 (RTS5,SS15) */
#define PDL_SCI_PIN_SCI5_CTS5_PA6		0x0100
#define PDL_SCI_PIN_SCI5_CTS5_PC0		0x0200
/*Duplicate pin names*/
/*RTS5*/
#define PDL_SCI_PIN_SCI5_RTS5_PA6		PDL_SCI_PIN_SCI5_CTS5_PA6
#define PDL_SCI_PIN_SCI5_RTS5_PC0		PDL_SCI_PIN_SCI5_CTS5_PC0
/*SS5*/
#define PDL_SCI_PIN_SCI5_SS5_PA6		PDL_SCI_PIN_SCI5_CTS5_PA6
#define PDL_SCI_PIN_SCI5_SS5_PC0		PDL_SCI_PIN_SCI5_CTS5_PC0


/***SCI6***/


/*RXD6 (SMISO6, SSCL6) */
#define PDL_SCI_PIN_SCI6_RXD6_P01		0x0001
#define PDL_SCI_PIN_SCI6_RXD6_P33		0x0002
#define PDL_SCI_PIN_SCI6_RXD6_PB0		0x0004
/*Duplicate pin names*/
/*SMISO6*/
#define PDL_SCI_PIN_SCI6_SMISO6_P01 	PDL_SCI_PIN_SCI6_RXD6_P01
#define PDL_SCI_PIN_SCI6_SMISO6_P33 	PDL_SCI_PIN_SCI6_RXD6_P33
#define PDL_SCI_PIN_SCI6_SMISO6_PB0 	PDL_SCI_PIN_SCI6_RXD6_PB0
/*SSCL6*/
#define PDL_SCI_PIN_SCI6_SSCL6_P01 		PDL_SCI_PIN_SCI6_RXD6_P01
#define PDL_SCI_PIN_SCI6_SSCL6_P33 		PDL_SCI_PIN_SCI6_RXD6_P33
#define PDL_SCI_PIN_SCI6_SSCL6_PB0 		PDL_SCI_PIN_SCI6_RXD6_PB0

/*TXD6 (SMOSI6, SSDA6)*/
#define PDL_SCI_PIN_SCI6_TXD6_P00		0x0008
#define PDL_SCI_PIN_SCI6_TXD6_P32		0x0010
#define PDL_SCI_PIN_SCI6_TXD6_PB1		0x0020
/*Duplicate pin names*/
/*SMOSI6*/
#define PDL_SCI_PIN_SCI6_SMOSI6_P00 	PDL_SCI_PIN_SCI6_TXD6_P00
#define PDL_SCI_PIN_SCI6_SMOSI6_P32 	PDL_SCI_PIN_SCI6_TXD6_P32
#define PDL_SCI_PIN_SCI6_SMOSI6_PB1 	PDL_SCI_PIN_SCI6_TXD6_PB1
/*SSDA6*/
#define PDL_SCI_PIN_SCI6_SSDA6_P00		PDL_SCI_PIN_SCI6_TXD6_P00
#define PDL_SCI_PIN_SCI6_SSDA6_P32		PDL_SCI_PIN_SCI6_TXD6_P32
#define PDL_SCI_PIN_SCI6_SSDA6_PB1 		PDL_SCI_PIN_SCI6_TXD6_PB1

/*SCK6*/
#define PDL_SCI_PIN_SCI6_SCK6_P02		0x0040
#define PDL_SCI_PIN_SCI6_SCK6_P34		0x0080
#define PDL_SCI_PIN_SCI6_SCK6_PB3		0x0100

/*CTS6 (RTS6,SS6) */
#define PDL_SCI_PIN_SCI6_CTS6_PB2		0x0200
#define PDL_SCI_PIN_SCI6_CTS6_PJ3		0x0400
/*Duplicate pin names*/
/*RTS6*/
#define PDL_SCI_PIN_SCI6_RTS6_PB2		PDL_SCI_PIN_SCI6_CTS6_PB2
#define PDL_SCI_PIN_SCI6_RTS6_PJ3		PDL_SCI_PIN_SCI6_CTS6_PJ3
/*SS6*/
#define PDL_SCI_PIN_SCI6_SS6_PB2		PDL_SCI_PIN_SCI6_CTS6_PB2
#define PDL_SCI_PIN_SCI6_SS6_PJ3		PDL_SCI_PIN_SCI6_CTS6_PJ3


/***SCI7***/


/*RXD7 (SMISO7, SSCL7) */
#define PDL_SCI_PIN_SCI7_RXD7_P92		0x0001
/*Duplicate pin names*/
/*SMISO7*/
#define PDL_SCI_PIN_SCI7_SMISO7_P92 	PDL_SCI_PIN_SCI7_RXD7_P92
/*SSCL7*/
#define PDL_SCI_PIN_SCI7_SSCL7_P92 		PDL_SCI_PIN_SCI7_RXD7_P92

/*TXD7 (SMOSI7, SSDA7)*/
#define PDL_SCI_PIN_SCI7_TXD7_P90		0x0002
/*Duplicate pin names*/
/*SMOSI7*/
#define PDL_SCI_PIN_SCI7_SMOSI7_P90 	PDL_SCI_PIN_SCI7_TXD7_P90
/*SSDA7*/
#define PDL_SCI_PIN_SCI7_SSDA7_P90		PDL_SCI_PIN_SCI7_TXD7_P90

/*SCK7*/
#define PDL_SCI_PIN_SCI7_SCK7_P91		0x0004

/*CTS7 (RTS7,SS7) */
#define PDL_SCI_PIN_SCI7_CTS7_P93		0x0008
/*Duplicate pin names*/
/*RTS7*/
#define PDL_SCI_PIN_SCI7_RTS7_P93		PDL_SCI_PIN_SCI7_CTS7_P93
/*SS7*/
#define PDL_SCI_PIN_SCI7_SS7_P93		PDL_SCI_PIN_SCI7_CTS7_P93


/***SCI8***/


/*RXD8 (SMISO8, SSCL8) */
#define PDL_SCI_PIN_SCI8_RXD8_PC6		0x0001
/*Duplicate pin names*/
/*SMISO8*/
#define PDL_SCI_PIN_SCI8_SMISO8_PC6 	PDL_SCI_PIN_SCI8_RXD8_PC6
/*SSCL8*/
#define PDL_SCI_PIN_SCI8_SSCL8_PC6 		PDL_SCI_PIN_SCI8_RXD8_PC6

/*TXD8 (SMOSI8, SSDA8)*/
#define PDL_SCI_PIN_SCI8_TXD8_PC7		0x0002
/*Duplicate pin names*/
/*SMOSI8*/
#define PDL_SCI_PIN_SCI8_SMOSI8_PC7 	PDL_SCI_PIN_SCI8_TXD8_PC7
/*SSDA8*/
#define PDL_SCI_PIN_SCI8_SSDA8_PC7		PDL_SCI_PIN_SCI8_TXD8_PC7

/*SCK8*/
#define PDL_SCI_PIN_SCI8_SCK8_PC5		0x0004

/*CTS8 (RTS8,SS8) */
#define PDL_SCI_PIN_SCI8_CTS8_PC4		0x0008
/*Duplicate pin names*/
/*RTS8*/
#define PDL_SCI_PIN_SCI8_RTS8_PC4		PDL_SCI_PIN_SCI8_CTS8_PC4
/*SS8*/
#define PDL_SCI_PIN_SCI8_SS8_PC4		PDL_SCI_PIN_SCI8_CTS8_PC4


/***SCI9***/


/*RXD9 (SMISO9, SSCL9) */
#define PDL_SCI_PIN_SCI9_RXD9_PB6		0x0001
/*Duplicate pin names*/
/*SMISO9*/
#define PDL_SCI_PIN_SCI9_SMISO9_PB6 	PDL_SCI_PIN_SCI9_RXD9_PB6
/*SSCL9*/
#define PDL_SCI_PIN_SCI9_SSCL9_PB6 		PDL_SCI_PIN_SCI9_RXD9_PB6

/*TXD9 (SMOSI9, SSDA9)*/
#define PDL_SCI_PIN_SCI9_TXD9_PB7		0x0004
/*Duplicate pin names*/
/*SMOSI9*/
#define PDL_SCI_PIN_SCI9_SMOSI9_PB7 	PDL_SCI_PIN_SCI9_TXD9_PB7
/*SSDA9*/
#define PDL_SCI_PIN_SCI9_SSDA9_PB7		PDL_SCI_PIN_SCI9_TXD9_PB7

/*SCK9*/
#define PDL_SCI_PIN_SCI9_SCK9_PB5		0x0020

/*CTS9 (RTS9,SS9) */
#define PDL_SCI_PIN_SCI9_CTS9_PB4		0x0080
/*Duplicate pin names*/
/*RTS9*/
#define PDL_SCI_PIN_SCI9_RTS9_PB4		PDL_SCI_PIN_SCI9_CTS9_PB4
/*SS9*/
#define PDL_SCI_PIN_SCI9_SS9_PB4		PDL_SCI_PIN_SCI9_CTS9_PB4


/***SCI10***/


/*RXD10 (SMISO10, SSCL10) */
#define PDL_SCI_PIN_SCI10_RXD10_P81		0x0001
/*Duplicate pin names*/
/*SMISO10*/
#define PDL_SCI_PIN_SCI10_SMISO10_P81 	PDL_SCI_PIN_SCI10_RXD10_P81
/*SSCL10*/
#define PDL_SCI_PIN_SCI10_SSCL10_P81 	PDL_SCI_PIN_SCI10_RXD10_P81

/*TXD10 (SMOSI10, SSDA10)*/
#define PDL_SCI_PIN_SCI10_TXD10_P82		0x0002
/*Duplicate pin names*/
/*SMOSI10*/
#define PDL_SCI_PIN_SCI10_SMOSI10_P82 	PDL_SCI_PIN_SCI10_TXD10_P82
/*SSDA10*/
#define PDL_SCI_PIN_SCI10_SSDA10_P82	PDL_SCI_PIN_SCI10_TXD10_P82

/*SCK10*/
#define PDL_SCI_PIN_SCI10_SCK10_P80		0x0004

/*CTS10(RTS10,SS10) */
#define PDL_SCI_PIN_SCI10_CTS10_P83		0x0008
/*Duplicate pin names*/
/*RTS10*/
#define PDL_SCI_PIN_SCI10_RTS10_P83		PDL_SCI_PIN_SCI10_CTS10_P83
/*SS10*/
#define PDL_SCI_PIN_SCI10_SS10_P83		PDL_SCI_PIN_SCI10_CTS10_P83


/***SCI11***/


/*RXD11 (SMISO11, SSCL11) */
#define PDL_SCI_PIN_SCI11_RXD11_P76		0x0001
/*Duplicate pin names*/
/*SMISO11*/
#define PDL_SCI_PIN_SCI11_SMISO11_P76 	PDL_SCI_PIN_SCI11_RXD11_P76
/*SSCL11*/
#define PDL_SCI_PIN_SCI11_SSCL11_P76 	PDL_SCI_PIN_SCI11_RXD11_P76

/*TXD11 (SMOSI11, SSDA11)*/
#define PDL_SCI_PIN_SCI11_TXD11_P77		0x0002
/*Duplicate pin names*/
/*SMOSI11*/
#define PDL_SCI_PIN_SCI11_SMOSI11_P77 	PDL_SCI_PIN_SCI11_TXD11_P77
/*SSDA11*/
#define PDL_SCI_PIN_SCI11_SSDA11_P77	PDL_SCI_PIN_SCI11_TXD11_P77

/*SCK11*/
#define PDL_SCI_PIN_SCI11_SCK11_P75		0x0004

/*CTS11(RTS11,SS11) */
#define PDL_SCI_PIN_SCI11_CTS11_P74		0x0008
/*Duplicate pin names*/
/*RTS11*/
#define PDL_SCI_PIN_SCI11_RTS11_P74		PDL_SCI_PIN_SCI11_CTS11_P74
/*SS11*/
#define PDL_SCI_PIN_SCI11_SS11_P74		PDL_SCI_PIN_SCI11_CTS11_P74


/***SCI12***/


/*RXD12 (SMISO12, SSCL12) */
#define PDL_SCI_PIN_SCI12_RXD12_PE2		0x0001
/*Duplicate pin names*/
/*SMISO12*/
#define PDL_SCI_PIN_SCI12_SMISO12_PE2 	PDL_SCI_PIN_SCI12_RXD12_PE2
/*SSCL12*/
#define PDL_SCI_PIN_SCI12_SSCL12_PE2 	PDL_SCI_PIN_SCI12_RXD12_PE2

/*TXD12 (SMOSI12, SSDA12)*/
#define PDL_SCI_PIN_SCI12_TXD12_PE1		0x0002
/*Duplicate pin names*/
/*SMOSI12*/
#define PDL_SCI_PIN_SCI12_SMOSI12_PE1 	PDL_SCI_PIN_SCI12_TXD12_PE1
/*SSDA12*/
#define PDL_SCI_PIN_SCI12_SSDA12_PE1	PDL_SCI_PIN_SCI12_TXD12_PE1

/*SCK12*/
#define PDL_SCI_PIN_SCI12_SCK12_PE0		0x0004

/*CTS12(RTS12,SS12) */
#define PDL_SCI_PIN_SCI12_CTS12_PE3		0x0008
/*Duplicate pin names*/
/*RTS12*/
#define PDL_SCI_PIN_SCI12_RTS12_PE3		PDL_SCI_PIN_SCI12_CTS12_PE3
/*SS12*/
#define PDL_SCI_PIN_SCI12_SS12_PE3		PDL_SCI_PIN_SCI12_CTS12_PE3

/* Bits for open drain control */
#define ODR_P0					ODR0.BIT.B0
#define ODR_P1					ODR0.BIT.B2
#define ODR_P2					ODR0.BIT.B4
#define ODR_P3					ODR0.BIT.B6
#define ODR_P4					ODR1.BIT.B0
#define ODR_P5					ODR1.BIT.B2
#define ODR_P6					ODR1.BIT.B4
#define ODR_P7					ODR1.BIT.B6

/* ----------------------------------------- */
/* R_SCI_Create, data2 Channel configuration */
/* ----------------------------------------- */
/* Operation mode */
#define PDL_SCI_ASYNC			0x00000001ul
#define PDL_SCI_SYNC			0x00000002ul
#define PDL_SCI_SMART			0x00000004ul
#define PDL_SCI_ASYNC_MP		0x00000008ul

/* Common selections */

/* Transmit / Receive connections (Not applicable in IIC Mode)*/
#define PDL_SCI_TX_CONNECTED	0x00000010ul
#define PDL_SCI_TX_DISCONNECTED	0x00000020ul
#define PDL_SCI_RX_CONNECTED	0x00000040ul
#define PDL_SCI_RX_DISCONNECTED	0x00000080ul

/* Data clock source (Not applicable in IIC Mode)*/
#define PDL_SCI_CLK_INT_OUT		0x00000100ul
#define PDL_SCI_CLK_EXT			0x00000200ul

/* Parity selection (Applicable to Async and Smart Card Modes.)*/
#define PDL_SCI_PARITY_EVEN		0x00000400ul
#define PDL_SCI_PARITY_ODD		0x00000800ul

/* Data transfer format (Not applicable in IIC Mode)*/
#define PDL_SCI_LSB_FIRST		0x00001000ul
#define PDL_SCI_MSB_FIRST		0x00002000ul

/* Asynchronous-only selections */

/* Data length */
#define PDL_SCI_8_BIT_LENGTH	0x00004000ul
#define PDL_SCI_7_BIT_LENGTH	0x00008000ul

/* Parity selection */
#define PDL_SCI_PARITY_NONE		0x00010000ul

/* Stop bit length */
#define PDL_SCI_STOP_1			0x00020000ul
#define PDL_SCI_STOP_2			0x00040000ul

#define PDL_SCI_8N1	(PDL_SCI_8_BIT_LENGTH | PDL_SCI_PARITY_NONE | PDL_SCI_STOP_1)

/*Digital Noise Filter */
#define PDL_SCI_RX_FILTER_DISABLE	0x00080000ul
#define PDL_SCI_RX_FILTER_ENABLE	0x00100000ul

/* Data clock source */
#define PDL_SCI_CLK_INT_IO		0x00200000ul
#define PDL_SCI_CLK_TMR			0x00400000ul


/*NOTE: See "Asynchronous and Synchronous only selections" following
 "Synchronous only section".*/

/* Synchronous only selections */
#define PDL_SCI_SPI_MODE		0x00004000ul
#define PDL_SCI_IIC_MODE		0x00008000ul

/*Clock selection*/
#define PDL_SCI_CLOCK_POLARITY_INVERTED	0x00010000ul
#define PDL_SCI_CLOCK_PHASE_DELAYED		0x00020000ul

/* Synchronous SPI only selections */

/*SS pin Enable/Disable usage.
(NOTE: The SS pin can not be enabled in master mode
as mult-master mode is not supported.)*/
#define PDL_SCI_SPI_SS_ENABLE	0x00040000ul
#define PDL_SCI_SPI_SS_DISABLE	0x00080000ul


/* Synchronous IIC only selections */
/*Noise Filter: If using select clock.*/
#define PDL_SCI_IIC_FILTER_DISABLED		0x00010000ul
#define PDL_SCI_IIC_FILTER_CLOCK_DIV1	0x00020000ul
#define PDL_SCI_IIC_FILTER_CLOCK_DIV2	0x00040000ul
#define PDL_SCI_IIC_FILTER_CLOCK_DIV4	0x00080000ul
#define PDL_SCI_IIC_FILTER_CLOCK_DIV8	0x00100000ul

/*IIC SDA Delay Output Select*/
/*Note: These are not bit flags, they are values shifted
to next available bit position*/
#define PDL_SCI_IIC_DELAY_SDA_0_1		(1ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_1_2		(2ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_2_3		(3ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_3_4		(4ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_4_5		(5ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_5_6		(6ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_6_7		(7ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_7_8		(8ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_8_9		(9ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_9_10		(10ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_10_11		(11ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_11_12		(12ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_12_13		(13ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_13_14		(14ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_14_15		(15ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_15_16		(16ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_16_17		(17ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_17_18		(18ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_18_19		(19ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_19_20		(20ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_20_21		(21ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_21_22		(22ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_22_23		(23ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_23_24		(24ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_24_25		(25ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_25_26		(26ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_26_27		(27ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_27_28		(28ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_28_29		(29ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_29_30		(30ul << 21u)
#define PDL_SCI_IIC_DELAY_SDA_30_31		(31ul << 21u)

/* Asynchronous and Synchronous only selections*/

/*HW (CTS/RTS) Flow Control (Choose one of the following)*/
#define PDL_SCI_HW_FLOW_NONE	0x20000000ul
#define PDL_SCI_HW_FLOW_CTS		0x40000000ul
#define PDL_SCI_HW_FLOW_RTS		0x80000000ul

/* Smart card-only selections */

/* Base clock pulse cycle count */          
#define PDL_SCI_BCP_32			0x00004000ul
#define PDL_SCI_BCP_64			0x00008000ul
#define PDL_SCI_BCP_93			0x00010000ul
#define PDL_SCI_BCP_128			0x00020000ul
#define PDL_SCI_BCP_186			0x00040000ul
#define PDL_SCI_BCP_256			0x00080000ul
#define PDL_SCI_BCP_372			0x00100000ul
#define PDL_SCI_BCP_512			0x00200000ul

/* Data inversion */
/*NOTE: This option is also available in SPI mode*/
#define PDL_SCI_INVERSION_OFF	0x00400000ul
#define PDL_SCI_INVERSION_ON	0x00800000ul
                                            
/* Block transfer mode */                   
#define PDL_SCI_BLOCK_MODE_OFF	0x01000000ul
#define PDL_SCI_BLOCK_MODE_ON	0x02000000ul
                                            
/* GSM mode */                              
#define PDL_SCI_GSM_MODE_OFF	0x04000000ul
#define PDL_SCI_GSM_MODE_ON		0x08000000ul
                                            
/* Clock output control */                  
#define PDL_SCI_SCK_OUTPUT_OFF	0x10000000ul
#define PDL_SCI_SCK_OUTPUT_LOW	0x20000000ul
#define PDL_SCI_SCK_OUTPUT_ON	0x40000000ul
#define PDL_SCI_SCK_OUTPUT_HIGH	0x80000000ul

/* ----------------------------------- */
/* R_SCI_Create, data3 register value  */
/* ----------------------------------- */
/* Transfer clock control */

/* CKS selection (optional) */
/*WARNING: THESE BIT VALUES MUST NOT CHANGE*/
#define PDL_SCI_PCLK_DIV_1		0x01000000ul
#define PDL_SCI_PCLK_DIV_4		0x02000000ul
#define PDL_SCI_PCLK_DIV_16		0x03000000ul
#define PDL_SCI_PCLK_DIV_64		0x04000000ul

/* ABCS selection (optional) */
/*WARNING: THESE BIT VALUES MUST NOT CHANGE*/
#define PDL_SCI_CYCLE_BIT_16	0x10000000ul
#define PDL_SCI_CYCLE_BIT_8		0x20000000ul

/* ----------------------------------------------- */
/* R_SCI_Send/R_SCI_Receive, Channel configuration */
/* ----------------------------------------------- */
/* DMAC / DTC trigger control */
#define PDL_SCI_DMAC_DTC_TRIGGER_DISABLE	0x0001u
#define PDL_SCI_DMAC_TRIGGER_ENABLE			0x0002u
#define PDL_SCI_DTC_TRIGGER_ENABLE			0x0004u
#define PDL_SCI_MP_ID_CYCLE					0x0008u

/* Continuous receive mode */
#define PDL_SCI_RX_CONTINUOUS_DISABLE 		0x0010ul
#define PDL_SCI_RX_CONTINUOUS_ENABLE		0x0020ul

/* ----------------------------------------------- */
/* R_SCI_SPI_TransferAll */
/* ----------------------------------------------- */
/* DMAC / DTC trigger control */
/*Tx Trigger*/
#define PDL_SCI_SPI_TX_DMAC_DTC_TRIGGER_DISABLE	0x0001u
#define PDL_SCI_SPI_TX_DMAC_TRIGGER_ENABLE		0x0002u
#define PDL_SCI_SPI_TX_DTC_TRIGGER_ENABLE		0x0004u
/*Rx Trigger*/
#define PDL_SCI_SPI_RX_DMAC_DTC_TRIGGER_DISABLE	0x0008u
#define PDL_SCI_SPI_RX_DMAC_TRIGGER_ENABLE		0x0010u
#define PDL_SCI_SPI_RX_DTC_TRIGGER_ENABLE		0x0020u

/* ----------------------------------------------- */
/* R_SCI_IIC_TransferAll						   */
/* ----------------------------------------------- */
#define PDL_SCI_IIC_7_BIT_SLAVE_ADDRESS			0x0001u
#define PDL_SCI_IIC_10_BIT_SLAVE_ADDRESS		0x0002u

#define PDL_SCI_IIC_DMAC_DTC_TRIGGER_DISABLE	0x0004u
#define PDL_SCI_IIC_DMAC_TRIGGER_ENABLE			0x0008u
#define PDL_SCI_IIC_DTC_TRIGGER_ENABLE			0x0010u

#define PDL_SCI_IIC_RESTART						0x0020u
#define PDL_SCI_IIC_NOSTOP						0x0040u

/* ------------------------------ */
/* R_SCI_Control, Channel control */
/* ------------------------------ */
/* Stop control */
#define PDL_SCI_STOP_TX						0x0001u
#define PDL_SCI_STOP_RX						0x0002u
#define PDL_SCI_STOP_TX_AND_RX	(PDL_SCI_STOP_TX | PDL_SCI_STOP_RX)

/* Break output control */
#define PDL_SCI_OUTPUT_MARK					0x0004u
#define PDL_SCI_OUTPUT_SPACE				0x0008u

/* Error flag control */
#define PDL_SCI_CLEAR_RECEIVE_ERROR_FLAGS	0x0010u

/* Manual SCK control */
#define PDL_SCI_GSM_SCK_STOP				0x0020u
#define PDL_SCI_GSM_SCK_START 				0x0040u

/*IIC Specific*/
#define PDL_SCI_IIC_STOP					0x0080u
#define PDL_SCI_IIC_CLOCK_SYNC_ENABLE		0x0100u
#define PDL_SCI_IIC_CLOCK_SYNC_DISABLE		0x0200u

#endif
/* End of file */
