/*""FILE COMMENT""*************************************************************
* System Name   : POE API for RX63N
* File Name     : r_pdl_poe.h
* Version		: 1.21
* Contents		: POE API header
* Customer		: 
* Model			: 
* Order			: 
* CPU			: RX
* Compiler		: RXC
* OS            : Nothing
* Programmer    : 
* Note          : 
*******************************************************************************
* Copyright, 2014. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FILE COMMENT END""*********************************************************/

#ifndef R_PDL_POE_H
#define R_PDL_POE_H

#include "r_pdl_common_defs_RX63Nxx.h"

/* Function prototypes */
bool R_POE_Set(
	uint32_t,
	uint16_t,
	uint16_t
);
bool R_POE_Create(
	uint8_t,
	void *,
	void *,
	uint8_t
);
bool POE_Control(
	uint8_t,
	uint16_t,
	uint8_t
);
bool R_POE_GetStatus(
	uint16_t *
);

/* Pin selection */
#define PDL_POE_PINS_0_TO_3             0x01u
#define PDL_POE_PIN_8                   0x02u

/* Pin POE0# selection */                     
#define PDL_POE_0_PORT_C_4              0x0001ul       
#define PDL_POE_0_PORT_D_7              0x0002ul       
                                                       
/* Pin POE1# selection */                              
#define PDL_POE_1_PORT_B_5              0x0004ul       
#define PDL_POE_1_PORT_D_6              0x0008ul       
                                                       
/* Pin POE2# selection */                              
#define PDL_POE_2_PORT_3_4              0x0010ul       
#define PDL_POE_2_PORT_A_6              0x0020ul       
#define PDL_POE_2_PORT_D_5              0x0040ul       
                                                       
/* Pin POE3# selection */                              
#define PDL_POE_3_PORT_3_3              0x0080ul       
#define PDL_POE_3_PORT_B_3              0x0100ul       
#define PDL_POE_3_PORT_D_4              0x0200ul       
                                                       
/* Pin POE8# selection */                              
#define PDL_POE_8_PORT_1_7              0x0400ul       
#define PDL_POE_8_PORT_3_0              0x0800ul       
#define PDL_POE_8_PORT_D_3              0x1000ul       
#define PDL_POE_8_PORT_E_3              0x2000ul       
                                                                   
/* Input pin detection */                                  
#define PDL_POE_0_MODE_EDGE             0x00000001ul               
#define PDL_POE_0_MODE_LOW_8            0x00000002ul               
#define PDL_POE_0_MODE_LOW_16           0x00000004ul               
#define PDL_POE_0_MODE_LOW_128          0x00000008ul               
                                                              
#define PDL_POE_1_MODE_EDGE             0x00000010ul               
#define PDL_POE_1_MODE_LOW_8            0x00000020ul               
#define PDL_POE_1_MODE_LOW_16           0x00000040ul               
#define PDL_POE_1_MODE_LOW_128          0x00000080ul               
                                                              
#define PDL_POE_2_MODE_EDGE             0x00000100ul               
#define PDL_POE_2_MODE_LOW_8            0x00000200ul               
#define PDL_POE_2_MODE_LOW_16           0x00000400ul               
#define PDL_POE_2_MODE_LOW_128          0x00000800ul               
                                        
#define PDL_POE_3_MODE_EDGE             0x00001000ul
#define PDL_POE_3_MODE_LOW_8            0x00002000ul
#define PDL_POE_3_MODE_LOW_16           0x00004000ul
#define PDL_POE_3_MODE_LOW_128          0x00008000ul
                                        
#define PDL_POE_8_MODE_EDGE             0x00010000ul
#define PDL_POE_8_MODE_LOW_8            0x00020000ul
#define PDL_POE_8_MODE_LOW_16           0x00040000ul
#define PDL_POE_8_MODE_LOW_128          0x00080000ul

/* Pin output control */

/* High impedance request detection */
#define PDL_POE_HI_Z_REQ_8_ENABLE	    0x0001ul
#define PDL_POE_HI_Z_REQ_MTIOC0A	    0x0002ul
#define PDL_POE_HI_Z_REQ_MTIOC0B	    0x0004ul
#define PDL_POE_HI_Z_REQ_MTIOC0C	    0x0008ul
#define PDL_POE_HI_Z_REQ_MTIOC0D	    0x0010ul
#define PDL_POE_HI_Z_REQ_OSTSTE         0x0020ul 
                                                                            
/* Output short detection */            
#define PDL_POE_SHORT_3_4_HI_Z		    0x0040ul
#define PDL_POE_SHORT_MTIOC4BD_A	    0x0080ul
#define PDL_POE_SHORT_MTIOC4AC_A	    0x0100ul
#define PDL_POE_SHORT_MTIOC3BD_A	    0x0200ul
                                               
/* High impedance request response */          
#define PDL_POE_IRQ_HI_Z_0_3_DISABLE	0x01u
#define PDL_POE_IRQ_HI_Z_0_3_ENABLE		0x02u
#define PDL_POE_IRQ_HI_Z_8_DISABLE		0x04u
#define PDL_POE_IRQ_HI_Z_8_ENABLE		0x08u

/* Output short detection response */
#define PDL_POE_IRQ_SHORT_3_4_ENABLE	0x10u
#define PDL_POE_IRQ_SHORT_3_4_DISABLE	0x20u
                                            
/* MTU channel high impedance control */    
#define PDL_POE_MTU3_MTU4_HI_Z_ON       0x01u
#define PDL_POE_MTU3_MTU4_HI_Z_OFF      0x02u
#define PDL_POE_MTU0_HI_Z_ON            0x04u
#define PDL_POE_MTU0_HI_Z_OFF           0x08u

/* Event flag control */
#define PDL_POE_FLAG_POE0_CLEAR         0x0001u
#define PDL_POE_FLAG_POE1_CLEAR         0x0002u
#define PDL_POE_FLAG_POE2_CLEAR         0x0004u
#define PDL_POE_FLAG_POE3_CLEAR         0x0008u
#define PDL_POE_FLAG_POE8_CLEAR         0x0100u
#define PDL_POE_FLAG_OSTSTF_CLEAR       0x0400u
#define PDL_POE_FLAG_SHORT_3_4_CLEAR    0x8000u

#endif
/* End of file */
