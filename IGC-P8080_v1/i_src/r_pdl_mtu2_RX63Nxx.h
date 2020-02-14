/*""FILE COMMENT""*************************************************************
* System Name	: Multi-function Timer Pulse Unit API for RX63N
* File Name		: r_pdl_MTU2_RX63Nxx.h
* Version		: 1.21
* Contents		: MTU2 API header
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

#ifndef R_PDL_MTU2_RX63Nxx_H
#define R_PDL_MTU2_RX63Nxx_H

#define MTU2_UNITS 				1
#define MTU2_CHANNELS_PER_UNIT	6
#define MTU2_CHANNELS (uint8_t)(MTU2_UNITS * MTU2_CHANNELS_PER_UNIT)

/* Callback function storage */
extern VoidCallBackFunc rpdl_MTU2_func1_callback_func[MTU2_CHANNELS];
extern VoidCallBackFunc rpdl_MTU2_func2_callback_func[MTU2_CHANNELS];
extern VoidCallBackFunc rpdl_MTU2_func3_callback_func[MTU2_CHANNELS];
extern VoidCallBackFunc rpdl_MTU2_func4_callback_func[MTU2_CHANNELS];
extern VoidCallBackFunc rpdl_MTU2_TGRE_callback_func[MTU2_CHANNELS];
extern VoidCallBackFunc rpdl_MTU2_TGRF_callback_func[MTU2_CHANNELS];
extern VoidCallBackFunc rpdl_MTU2_Overflow_callback_func[MTU2_CHANNELS];
extern VoidCallBackFunc rpdl_MTU2_Underflow_callback_func[MTU2_CHANNELS];

typedef struct R_MTU2_Create_parameters
{
	uint32_t channel_mode;
	uint32_t counter_operation;
	uint32_t ADC_trigger_operation;
	uint16_t buffer_operation;
	uint32_t TGR_A_B_operation;
	uint32_t TGR_C_D_operation;
	uint32_t TGR_U_V_W_operation;
	uint16_t noise_filter_operation;
	uint16_t TCNT_TCNTU_value;
	uint16_t TGRA_TCNTV_value;
	uint16_t TGRB_TCNTW_value;
	uint16_t TGRC_TGRU_value;
	uint16_t TGRD_TGRV_value ;
	uint16_t TGRE_TGRW_value;
	uint16_t TGRF_TADCORA_value;
	uint16_t TADCORB_value;
	uint16_t TADCOBRA_value;
	uint16_t TADCOBRB_value;
	VoidCallBackFunc func1;
	VoidCallBackFunc func2;
	VoidCallBackFunc func3;
	VoidCallBackFunc func4;
	uint8_t interrupt_priority_1;
	VoidCallBackFunc func5;
	VoidCallBackFunc func6;
	uint8_t interrupt_priority_2;
	VoidCallBackFunc func7;
	VoidCallBackFunc func8;
	uint8_t interrupt_priority_3;
} R_MTU2_Create_structure;

typedef struct R_MTU2_ControlChannel_parameters
{
	uint8_t control_setting;
	uint16_t register_selection;
	uint16_t TCNT_TCNTU_value;
	uint16_t TGRA_TCNTV_value;
	uint16_t TGRB_TCNTW_value;
	uint16_t TGRC_TGRU_value;
	uint16_t TGRD_TGRV_value;
	uint16_t TGRE_TGRW_value;
	uint16_t TGRF_value;
	uint16_t TADCOBRA_value;
	uint16_t TADCOBRB_value;
} R_MTU2_ControlChannel_structure;

typedef struct R_MTU2_ControlUnit_parameters
{
	uint16_t simultaneous_control;
	uint32_t output_control;
	uint32_t buffer_control;
	uint16_t brushless_DC_motor_control ;
	uint32_t general_control;
	uint8_t register_selection;
	uint16_t TDDR_value;
	uint16_t TCDR_value;
	uint16_t TCBR_value;
} R_MTU2_ControlUnit_structure;

/* Library prototypes */
bool R_MTU2_SetAll(
	const uint8_t,
	const uint32_t,
	const uint16_t
);
bool R_MTU2_CreateAll(
	const uint8_t,
	R_MTU2_Create_structure * const
);
bool R_MTU2_DestroyAll(
	const uint8_t
);
bool R_MTU2_ControlChannelAll(
	const uint8_t,
	R_MTU2_ControlChannel_structure * const
);
bool R_MTU2_ControlUnitAll(
	const uint8_t,
	R_MTU2_ControlUnit_structure * const
);
bool R_MTU2_ReadChannelAll(
	const uint8_t,
	volatile uint8_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const,
	volatile uint16_t * const
);
bool R_MTU2_ReadUnitAll(
	const uint8_t,
	volatile uint16_t * const,
	volatile uint8_t * const
);
bool ReturnFalse(void);
void R_MTU2_Create_load_defaults(R_MTU2_Create_structure * const);

/* Macro definitions */

#define R_MTU2_Set(a, b, c) \
( \
( (a) < MTU2_CHANNELS ) ? \
R_MTU2_SetAll( (a), (b), (c) ) : \
ReturnFalse() \
)

#define R_MTU2_Create(a, b) \
( \
( (a) < MTU2_CHANNELS ) ? \
R_MTU2_CreateAll( (a), (b) ) : \
ReturnFalse() \
)

#define R_MTU2_ControlChannel(a, b) \
( \
( (a) < MTU2_CHANNELS ) ? \
R_MTU2_ControlChannelAll( (a), (b) ) : \
ReturnFalse() \
)

#define R_MTU2_ControlUnit(a, b) \
( \
( (a) < MTU2_UNITS ) ? \
R_MTU2_ControlUnitAll( (a), (b) ) : \
ReturnFalse() \
)

#define R_MTU2_ReadChannel(a, b, c, d, e, f, g, h, i) \
( \
( (a) < MTU2_CHANNELS ) ? \
R_MTU2_ReadChannelAll( (a), (b), (c), (d), (e), (f), (g), (h), (i) ) : \
ReturnFalse() \
)

#define R_MTU2_ReadUnit(a, b, c) \
( \
( (a) < MTU2_UNITS ) ? \
R_MTU2_ReadUnitAll( (a), (b), (c) ) : \
ReturnFalse() \
)

#define R_MTU2_Destroy(a) \
( \
( (a) < MTU2_UNITS ) ? \
R_MTU2_DestroyAll( (a) ) : \
ReturnFalse() \
)

#endif
/* End of file */
