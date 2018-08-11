/*""FILE COMMENT""*************************************************************
* System Name	: 12-bit ADC Interrupt program for RX63Nxx
* File Name 	: Interrupt_ADC_12.c
* Version		: 1.21
* Contents		: Interrupt handler for ADC_12
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

#include "r_pdl_adc_12.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""*************************************************************
* Module outline: ADC interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_ADCn(void)
*------------------------------------------------------------------------------
* Function		: Interrupt processing function for ADC unit n
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input			: 
* Output		: 
*------------------------------------------------------------------------------
* Use function	: rpdl_ADC_12_callback_func[n]()
*------------------------------------------------------------------------------
* Notes			: 
*------------------------------------------------------------------------------
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_S12AD_S12ADI0
#pragma interrupt Interrupt_ADC_12_0(vect=VECT_S12AD_S12ADI0, fint)
#else
#pragma interrupt Interrupt_ADC_12_0(vect=VECT_S12AD_S12ADI0)
#endif
void Interrupt_ADC_12_0(void)
{
	/* Call the user function */
	if (rpdl_ADC_12_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_ADC_12_callback_func[0]();
	}
}

/* End of file */
