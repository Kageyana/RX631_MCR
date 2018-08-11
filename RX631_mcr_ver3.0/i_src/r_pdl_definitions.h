/*""FILE COMMENT""*************************************************************
* System Name	: PDL driver API
* File Name		: r_pdl_definitions.h
* Version		: 1.21
* Contents		: API headers for RX63N (100-pin package)
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

#ifndef R_PDL_DEFS_H
#define R_PDL_DEFS_H

#define PDL_VERSION	0x0121u	/* v01.21 in BCD format */

#define DEVICE_PACKAGE_100_PIN

/* Check the precision setting */
#ifndef __DBL8
 #error "Double precision is not selected.  Please change from single to double precision."
#endif

/* Peripherals that are common to all device packages */
#include "r_pdl_adc_10_RX63Nxx.h"
#include "r_pdl_adc_12_RX63Nxx.h"
#include "r_pdl_bsc_RX63Nxx.h"
#include "r_pdl_cgc_RX63Nxx.h"
#include "r_pdl_cmt_RX63Nxx.h"
#include "r_pdl_crc_RX63Nxx.h"
#include "r_pdl_dac_10_RX63Nxx.h"
#include "r_pdl_dmac_RX63Nxx.h"
#include "r_pdl_dtc_RX63Nxx.h"
#include "r_pdl_exdmac_RX63Nxx.h"
#include "r_pdl_ieb_RX63Nxx.h"
#include "r_pdl_iic_RX63Nxx.h"
#include "r_pdl_intc_RX63Nxx.h"
#include "r_pdl_io_port_RX63Nxx.h"
#include "r_pdl_iwdt_RX63Nxx.h"
#include "r_pdl_lpc_RX63Nxx.h"
#include "r_pdl_lvd_RX63Nxx.h"
#include "r_pdl_mck_RX63Nxx.h"
#include "r_pdl_mcu_RX63Nxx.h"
#include "r_pdl_mpc_RX63Nxx.h"
#include "r_pdl_mtu2_RX63Nxx.h"
#include "r_pdl_poe_RX63Nxx.h"
#include "r_pdl_ppg_RX63Nxx.h"
#include "r_pdl_rtc_RX63Nxx.h"
#include "r_pdl_rwp_RX63Nxx.h"
#include "r_pdl_sci_RX63Nxx.h"
#include "r_pdl_spi_RX63Nxx.h"
#include "r_pdl_tmr_RX63Nxx.h"
#include "r_pdl_tpu_RX63Nxx.h"
#include "r_pdl_ts_RX63Nxx.h"
#include "r_pdl_wdt_RX63Nxx.h"

#endif
/* End of file */
