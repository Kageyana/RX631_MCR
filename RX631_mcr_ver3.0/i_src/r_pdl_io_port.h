/*""FILE COMMENT""*************************************************************
* System Name	: I/O Port API
* File Name		: r_pdl_io_port.h
* Version		: 1.21
* Contents		: I/O Port API header
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

#ifndef R_PDL_IO_PORT_H
#define R_PDL_IO_PORT_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_IO_PORT_Set(
	uint16_t,
	uint16_t
);
bool R_IO_PORT_ReadControl(
	uint16_t,
	uint8_t,
	uint16_t *
);
bool R_IO_PORT_ModifyControl(
	uint16_t,
	uint8_t,
	uint16_t
);
bool R_IO_PORT_Read(
	uint16_t,
	uint8_t *
);
bool R_IO_PORT_Write(
	uint16_t,
	uint8_t
);
bool R_IO_PORT_Compare(
	uint16_t,
	uint8_t,
	void *
);
bool R_IO_PORT_Modify(
	uint16_t,
	uint16_t,
	uint8_t
);
bool R_IO_PORT_Wait(
	uint16_t,
	uint8_t
);
bool R_IO_PORT_NotAvailable(
	void
);
bool R_IO_PORT_Switch(
	uint16_t
);

/* I/O Ports (NOTE: LSB of value is used to calculate register address)*/
#define PDL_IO_PORT_0	0xF000u	/* Port 0 */
#define PDL_IO_PORT_1	0xF001u	/* Port 1 */
#define PDL_IO_PORT_2	0xF002u	/* Port 2 */
#define PDL_IO_PORT_3	0xF003u	/* Port 3 */
#define PDL_IO_PORT_4	0xF004u	/* Port 4 */
#define PDL_IO_PORT_5	0xF005u	/* Port 5 */
#define PDL_IO_PORT_6	0xF006u	/* Port 6 */
#define PDL_IO_PORT_7	0xF007u	/* Port 7 */
#define PDL_IO_PORT_8	0xF008u	/* Port 8 */
#define PDL_IO_PORT_9	0xF009u	/* Port 9 */
#define PDL_IO_PORT_A	0xF00Au	/* Port A */
#define PDL_IO_PORT_B	0xF00Bu	/* Port B */
#define PDL_IO_PORT_C	0xF00Cu	/* Port C */
#define PDL_IO_PORT_D	0xF00Du	/* Port D */
#define PDL_IO_PORT_E	0xF00Eu	/* Port E */
#define PDL_IO_PORT_F	0xF00Fu	/* Port F */
#define PDL_IO_PORT_G	0xF010u	/* Port G */
#define PDL_IO_PORT_J	0xF012u	/* Port J */

/* I/O port pins
(NOTE: MSB = port (see above), LSB is mask of bit value) */
/* Port 0 */
#define PDL_IO_PORT_0_0 0x0001u	/* P00 */
#define PDL_IO_PORT_0_1 0x0002u	/* P01 */
#define PDL_IO_PORT_0_2 0x0004u	/* P02 */
#define PDL_IO_PORT_0_3 0x0008u	/* P03 */
#define PDL_IO_PORT_0_5 0x0020u	/* P05 */
#define PDL_IO_PORT_0_7 0x0080u	/* P07 */

/* Port 1 */
#define PDL_IO_PORT_1_0 0x0101u	/* P10 */
#define PDL_IO_PORT_1_1 0x0102u	/* P11 */
#define PDL_IO_PORT_1_2 0x0104u	/* P12 */
#define PDL_IO_PORT_1_3 0x0108u	/* P13 */
#define PDL_IO_PORT_1_4 0x0110u	/* P14 */
#define PDL_IO_PORT_1_5 0x0120u	/* P15 */
#define PDL_IO_PORT_1_6 0x0140u	/* P16 */
#define PDL_IO_PORT_1_7 0x0180u	/* P17 */

/* Port 2 */
#define PDL_IO_PORT_2_0 0x0201u	/* P20 */
#define PDL_IO_PORT_2_1 0x0202u	/* P21 */
#define PDL_IO_PORT_2_2 0x0204u	/* P22 */
#define PDL_IO_PORT_2_3 0x0208u	/* P23 */
#define PDL_IO_PORT_2_4 0x0210u	/* P24 */
#define PDL_IO_PORT_2_5 0x0220u	/* P25 */
#define PDL_IO_PORT_2_6 0x0240u	/* P26 */
#define PDL_IO_PORT_2_7 0x0280u	/* P27 */

/* Port 3 */
#define PDL_IO_PORT_3_0 0x0301u	/* P30 */
#define PDL_IO_PORT_3_1 0x0302u	/* P31 */
#define PDL_IO_PORT_3_2 0x0304u	/* P32 */
#define PDL_IO_PORT_3_3 0x0308u	/* P33 */
#define PDL_IO_PORT_3_4 0x0310u	/* P34 */
#define PDL_IO_PORT_3_5 0x0320u	/* P35 */
#define PDL_IO_PORT_3_6 0x0340u	/* P36 */
#define PDL_IO_PORT_3_7 0x0380u	/* P37 */

/* Port 4 */
#define PDL_IO_PORT_4_0 0x0401u	/* P40 */
#define PDL_IO_PORT_4_1 0x0402u	/* P41 */
#define PDL_IO_PORT_4_2 0x0404u	/* P42 */
#define PDL_IO_PORT_4_3 0x0408u	/* P43 */
#define PDL_IO_PORT_4_4 0x0410u	/* P44 */
#define PDL_IO_PORT_4_5 0x0420u	/* P45 */
#define PDL_IO_PORT_4_6 0x0440u	/* P46 */
#define PDL_IO_PORT_4_7 0x0480u	/* P47 */

/* Port 5 */
#define PDL_IO_PORT_5_0 0x0501u	/* P50 */
#define PDL_IO_PORT_5_1 0x0502u	/* P51 */
#define PDL_IO_PORT_5_2 0x0504u	/* P52 */
#define PDL_IO_PORT_5_3 0x0508u	/* P53 */
#define PDL_IO_PORT_5_4 0x0510u	/* P54 */
#define PDL_IO_PORT_5_5 0x0520u	/* P55 */
#define PDL_IO_PORT_5_6 0x0540u	/* P56 */
#define PDL_IO_PORT_5_7 0x0580u	/* P57 */

/* Port 6 */
#define PDL_IO_PORT_6_0 0x0601u	/* P60 */
#define PDL_IO_PORT_6_1 0x0602u	/* P61 */
#define PDL_IO_PORT_6_2 0x0604u	/* P62 */
#define PDL_IO_PORT_6_3 0x0608u	/* P63 */
#define PDL_IO_PORT_6_4 0x0610u	/* P64 */
#define PDL_IO_PORT_6_5 0x0620u	/* P65 */
#define PDL_IO_PORT_6_6 0x0640u	/* P66 */
#define PDL_IO_PORT_6_7 0x0680u	/* P67 */

/* Port 7 */
#define PDL_IO_PORT_7_0 0x0701u	/* P70 */
#define PDL_IO_PORT_7_1 0x0702u	/* P71 */
#define PDL_IO_PORT_7_2 0x0704u	/* P72 */
#define PDL_IO_PORT_7_3 0x0708u	/* P73 */
#define PDL_IO_PORT_7_4 0x0710u	/* P74 */
#define PDL_IO_PORT_7_5 0x0720u	/* P75 */
#define PDL_IO_PORT_7_6 0x0740u	/* P76 */
#define PDL_IO_PORT_7_7 0x0780u	/* P77 */

/* Port 8 */
#define PDL_IO_PORT_8_0 0x0801u	/* P80 */
#define PDL_IO_PORT_8_1 0x0802u	/* P81 */
#define PDL_IO_PORT_8_2 0x0804u	/* P82 */
#define PDL_IO_PORT_8_3 0x0808u	/* P83 */
#define PDL_IO_PORT_8_4 0x0810u	/* P84 */
#define PDL_IO_PORT_8_5 0x0820u	/* P85 */
#define PDL_IO_PORT_8_6 0x0840u	/* P86 */
#define PDL_IO_PORT_8_7 0x0880u	/* P87 */

/* Port 9 */
#define PDL_IO_PORT_9_0 0x0901u	/* P90 */
#define PDL_IO_PORT_9_1 0x0902u	/* P91 */
#define PDL_IO_PORT_9_2 0x0904u	/* P92 */
#define PDL_IO_PORT_9_3 0x0908u	/* P93 */
#define PDL_IO_PORT_9_4 0x0910u	/* P94 */
#define PDL_IO_PORT_9_5 0x0920u	/* P95 */
#define PDL_IO_PORT_9_6 0x0940u	/* P96 */
#define PDL_IO_PORT_9_7 0x0980u	/* P97 */

/* Port A */
#define PDL_IO_PORT_A_0 0x0A01u	/* PA0 */
#define PDL_IO_PORT_A_1 0x0A02u	/* PA1 */
#define PDL_IO_PORT_A_2 0x0A04u	/* PA2 */
#define PDL_IO_PORT_A_3 0x0A08u	/* PA3 */
#define PDL_IO_PORT_A_4 0x0A10u	/* PA4 */
#define PDL_IO_PORT_A_5 0x0A20u	/* PA5 */
#define PDL_IO_PORT_A_6 0x0A40u	/* PA6 */
#define PDL_IO_PORT_A_7 0x0A80u	/* PA7 */

/* Port B */
#define PDL_IO_PORT_B_0 0x0B01u	/* PB0 */
#define PDL_IO_PORT_B_1 0x0B02u	/* PB1 */
#define PDL_IO_PORT_B_2 0x0B04u	/* PB2 */
#define PDL_IO_PORT_B_3 0x0B08u	/* PB3 */
#define PDL_IO_PORT_B_4 0x0B10u	/* PB4 */
#define PDL_IO_PORT_B_5 0x0B20u	/* PB5 */
#define PDL_IO_PORT_B_6 0x0B40u	/* PB6 */
#define PDL_IO_PORT_B_7 0x0B80u	/* PB7 */

/* Port C */
#define PDL_IO_PORT_C_0 0x0C01u	/* PC0 */
#define PDL_IO_PORT_C_1 0x0C02u	/* PC1 */
#define PDL_IO_PORT_C_2 0x0C04u	/* PC2 */
#define PDL_IO_PORT_C_3 0x0C08u	/* PC3 */
#define PDL_IO_PORT_C_4 0x0C10u	/* PC4 */
#define PDL_IO_PORT_C_5 0x0C20u	/* PC5 */
#define PDL_IO_PORT_C_6 0x0C40u	/* PC6 */
#define PDL_IO_PORT_C_7 0x0C80u	/* PC7 */

/* Port D */
#define PDL_IO_PORT_D_0 0x0D01u	/* PD0 */
#define PDL_IO_PORT_D_1 0x0D02u	/* PD1 */
#define PDL_IO_PORT_D_2 0x0D04u	/* PD2 */
#define PDL_IO_PORT_D_3 0x0D08u	/* PD3 */
#define PDL_IO_PORT_D_4 0x0D10u	/* PD4 */
#define PDL_IO_PORT_D_5 0x0D20u	/* PD5 */
#define PDL_IO_PORT_D_6 0x0D40u	/* PD6 */
#define PDL_IO_PORT_D_7 0x0D80u	/* PD7 */

/* Port E */
#define PDL_IO_PORT_E_0 0x0E01u	/* PE0 */
#define PDL_IO_PORT_E_1 0x0E02u	/* PE1 */
#define PDL_IO_PORT_E_2 0x0E04u	/* PE2 */
#define PDL_IO_PORT_E_3 0x0E08u	/* PE3 */
#define PDL_IO_PORT_E_4 0x0E10u	/* PE4 */
#define PDL_IO_PORT_E_5 0x0E20u	/* PE5 */
#define PDL_IO_PORT_E_6 0x0E40u	/* PE6 */
#define PDL_IO_PORT_E_7 0x0E80u	/* PE7 */

/* Port F */
#define PDL_IO_PORT_F_0 0x0F01u	/* PF0 */
#define PDL_IO_PORT_F_1 0x0F02u	/* PF1 */
#define PDL_IO_PORT_F_2 0x0F04u	/* PF2 */
#define PDL_IO_PORT_F_3 0x0F08u	/* PF3 */
#define PDL_IO_PORT_F_4 0x0F10u	/* PF4 */
#define PDL_IO_PORT_F_5 0x0F20u	/* PF5 */

/* Port G */
#define PDL_IO_PORT_G_0 0x1001u	/* PG0 */
#define PDL_IO_PORT_G_1 0x1002u	/* PG1 */
#define PDL_IO_PORT_G_2 0x1004u	/* PG2 */
#define PDL_IO_PORT_G_3 0x1008u	/* PG3 */
#define PDL_IO_PORT_G_4 0x1010u	/* PG4 */
#define PDL_IO_PORT_G_5 0x1020u	/* PG5 */
#define PDL_IO_PORT_G_6 0x1040u	/* PG6 */
#define PDL_IO_PORT_G_7 0x1080u	/* PG7 */

/* Port J */
#define PDL_IO_PORT_J_3 0x1208u	/* PJ3 */
#define PDL_IO_PORT_J_5 0x1220u	/* PJ5 */

/* Settings */
#define PDL_IO_PORT_INPUT				0x0001u	/* Input port */
#define PDL_IO_PORT_OUTPUT				0x0002u	/* Output port */
#define PDL_IO_PORT_TYPE_CMOS			0x0004u	/* CMOS output */
#define PDL_IO_PORT_TYPE_NMOS			0x0008u	/* NMOS open-drain output */
#define PDL_IO_PORT_TYPE_PMOS			0x0010u	/* PMOS open-drain output */
#define PDL_IO_PORT_PULL_UP_ON			0x0020u	/* Pulled high */
#define PDL_IO_PORT_PULL_UP_OFF			0x0040u	/* Not pulled high */
#define PDL_IO_PORT_DRIVE_NORMAL		0x0080u	/* Normal drive level */
#define PDL_IO_PORT_DRIVE_HIGH			0x0100u	/* High drive level */  

/* Control register selection */
#define PDL_IO_PORT_DIRECTION			0x01u
#define PDL_IO_PORT_MODE				0x02u
#define PDL_IO_PORT_TYPE				0x04u
#define PDL_IO_PORT_PULL_UP				0x08u
#define PDL_IO_PORT_DRIVE				0x10u

/* switch register selection PSRA and PSRB */                                          
/* option valid on packages with 48 pins */
#define PDL_IO_PORT_SWITCH_PC0_TO_PB0	0x0001u	/* switch from PC0 to PB0 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PC1_TO_PB1	0x0002u	/* switch from PC1 to PB1 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PC2_TO_PB3	0x0004u	/* switch from PC2 to PB3 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PC3_TO_PB5	0x0008u	/* switch from PC3 to PB5 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PB0_TO_PC0	0x0010u	/* switch from PB0 to PC0 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PB1_TO_PC1	0x0020u	/* switch from PB1 to PC1 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PB3_TO_PC2	0x0040u	/* switch from PB3 to PC2 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PB5_TO_PC3	0x0080u	/* switch from PB5 to PC3 general I/O port function */ 

/* option valid on packages with 64 pins */
#define PDL_IO_PORT_SWITCH_PC0_TO_PB6	0x0100u	/* switch from PC0 to PB6 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PC1_TO_PB7	0x0200u	/* switch from PC1 to PB7 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PB6_TO_PC0	0x0400u	/* switch from PB6 to PC0 general I/O port function */ 
#define PDL_IO_PORT_SWITCH_PB7_TO_PC1	0x0800u	/* switch from PB7 to PC1 general I/O port function */ 

/* Logical operations */
#define PDL_IO_PORT_AND					0x20u
#define PDL_IO_PORT_OR					0x40u
#define PDL_IO_PORT_XOR					0x80u

#endif
/* End of file */
