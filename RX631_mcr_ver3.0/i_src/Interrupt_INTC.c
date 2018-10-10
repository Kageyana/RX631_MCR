/*""FILE COMMENT""*************************************************************
* System Name	: Interrupt program for RX63Nxx
* File Name		: Interrupt_INTC.c
* Version		: 1.21
* Contents		: Interrupt handlers for the external interrupts.
*				: Group interrupts are also handled here.
*				: Group interrupts used by the MTU have MTU specific code.	
*				:   	
* Customer		: 
* Model			:
* Order		 	:
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

#include "r_pdl_intc.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/* External functions */
extern void PowerON_Reset_PC(void);
#if !defined (DEVICE_PACKAGE_64_PIN) && !defined (DEVICE_PACKAGE_48_PIN)
extern void rpdl_ADC_10_Start(void);
#endif

/* CMT control register definition */
#define CMSTR_ADDRESS(a) ( (volatile uint16_t __evenaccess *)&CMT.CMSTR0 + ((0x10u * (a >> 1)) / sizeof(uint16_t)) )

/*""FUNC COMMENT""*************************************************************
* Module outline: External interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_XXXX(void)
*------------------------------------------------------------------------------
* Function		: Interrupt processing function for External interrupts
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input			: 
* Output		: 
*------------------------------------------------------------------------------
* Use function	: rpdl_INTC_callback_func[n]()
*------------------------------------------------------------------------------
* Notes		 	:
*------------------------------------------------------------------------------
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_ICU_IRQ0
#pragma interrupt Interrupt_IRQ0(vect=VECT_ICU_IRQ0, fint)
#else
#pragma interrupt Interrupt_IRQ0(vect=VECT_ICU_IRQ0)
#endif
void Interrupt_IRQ0(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ0] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ0]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ1
#pragma interrupt Interrupt_IRQ1(vect=VECT_ICU_IRQ1, fint)
#else
#pragma interrupt Interrupt_IRQ1(vect=VECT_ICU_IRQ1)
#endif
void Interrupt_IRQ1(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ1] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ1]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ2
#pragma interrupt Interrupt_IRQ2(vect=VECT_ICU_IRQ2, fint)
#else
#pragma interrupt Interrupt_IRQ2(vect=VECT_ICU_IRQ2)
#endif
void Interrupt_IRQ2(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ2] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ2]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ3
#pragma interrupt Interrupt_IRQ3(vect=VECT_ICU_IRQ3, fint)
#else
#pragma interrupt Interrupt_IRQ3(vect=VECT_ICU_IRQ3)
#endif
void Interrupt_IRQ3(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ3] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ3]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ4
#pragma interrupt Interrupt_IRQ4(vect=VECT_ICU_IRQ4, fint)
#else
#pragma interrupt Interrupt_IRQ4(vect=VECT_ICU_IRQ4)
#endif
void Interrupt_IRQ4(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ4] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ4]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ5
#pragma interrupt Interrupt_IRQ5(vect=VECT_ICU_IRQ5, fint)
#else
#pragma interrupt Interrupt_IRQ5(vect=VECT_ICU_IRQ5)
#endif
void Interrupt_IRQ5(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ5] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ5]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ6
#pragma interrupt Interrupt_IRQ6(vect=VECT_ICU_IRQ6, fint)
#else
#pragma interrupt Interrupt_IRQ6(vect=VECT_ICU_IRQ6)
#endif
void Interrupt_IRQ6(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ6] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ6]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ7
#pragma interrupt Interrupt_IRQ7(vect=VECT_ICU_IRQ7, fint)
#else
#pragma interrupt Interrupt_IRQ7(vect=VECT_ICU_IRQ7)
#endif
void Interrupt_IRQ7(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ7] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ7]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ8
#pragma interrupt Interrupt_IRQ8(vect=VECT_ICU_IRQ8, fint)
#else
#pragma interrupt Interrupt_IRQ8(vect=VECT_ICU_IRQ8)
#endif
void Interrupt_IRQ8(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ8] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ8]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ9
#pragma interrupt Interrupt_IRQ9(vect=VECT_ICU_IRQ9, fint)
#else
#pragma interrupt Interrupt_IRQ9(vect=VECT_ICU_IRQ9)
#endif
void Interrupt_IRQ9(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ9] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ9]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ10
#pragma interrupt Interrupt_IRQ10(vect=VECT_ICU_IRQ10, fint)
#else
#pragma interrupt Interrupt_IRQ10(vect=VECT_ICU_IRQ10)
#endif
void Interrupt_IRQ10(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ10] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ10]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ11
#pragma interrupt Interrupt_IRQ11(vect=VECT_ICU_IRQ11, fint)
#else
#pragma interrupt Interrupt_IRQ11(vect=VECT_ICU_IRQ11)
#endif
void Interrupt_IRQ11(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ11] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ11]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ12
#pragma interrupt Interrupt_IRQ12(vect=VECT_ICU_IRQ12, fint)
#else
#pragma interrupt Interrupt_IRQ12(vect=VECT_ICU_IRQ12)
#endif
void Interrupt_IRQ12(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ12] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ12]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ13
#pragma interrupt Interrupt_IRQ13(vect=VECT_ICU_IRQ13, fint)
#else
#pragma interrupt Interrupt_IRQ13(vect=VECT_ICU_IRQ13)
#endif
void Interrupt_IRQ13(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ13] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ13]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ14
#pragma interrupt Interrupt_IRQ14(vect=VECT_ICU_IRQ14, fint)
#else
#pragma interrupt Interrupt_IRQ14(vect=VECT_ICU_IRQ14)
#endif
void Interrupt_IRQ14(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ14] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ14]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_IRQ15
#pragma interrupt Interrupt_IRQ15(vect=VECT_ICU_IRQ15, fint)
#else
#pragma interrupt Interrupt_IRQ15(vect=VECT_ICU_IRQ15)
#endif
void Interrupt_IRQ15(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_IRQ15] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_IRQ15]();
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline: Group interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_Group_X(void)
*------------------------------------------------------------------------------
* Function		: Interrupt processing function for Group interrupts
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input			: 
* Output		: 
*------------------------------------------------------------------------------
* Use function	: rpdl_INTC_group_callback_func[n]()
*------------------------------------------------------------------------------
* Notes		 	:
*------------------------------------------------------------------------------
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FUNC COMMENT END""*********************************************************/
#if FAST_INTC_VECTOR == VECT_ICU_GROUP0
#pragma interrupt Interrupt_Group_0(vect=VECT_ICU_GROUP0, fint)
#else
#pragma interrupt Interrupt_Group_0(vect=VECT_ICU_GROUP0)
#endif
void Interrupt_Group_0(void)
{
	/* Call the user function */
	if (rpdl_INTC_group_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_INTC_group_callback_func[0]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_GROUP1
#pragma interrupt Interrupt_Group_1(vect=VECT_ICU_GROUP1, fint)
#else
#pragma interrupt Interrupt_Group_1(vect=VECT_ICU_GROUP1)
#endif
void Interrupt_Group_1(void)
{
	/* The MTU generates this interrupt.*/
	/* Read the Group Interrupt Source Register to determine
	the specific cause of the interrupt */
	if(0 != ICU.GRP[GRP_MTU0_TCIV0].BIT.IS_MTU0_TCIV0)
	{
		/* MTU channel 0 TCIV0 Overflow */
		/* Call the user function */
		if (rpdl_MTU2_Overflow_callback_func[0] != PDL_NO_FUNC)
		{
			rpdl_MTU2_Overflow_callback_func[0]();
		}
		
		/* Clear the flag by writing to it's clear bit */
		ICU.GCR[GCR_MTU0_TCIV0].BIT.CLR_MTU0_TCIV0 = 1;
	}

	if(0 != ICU.GRP[GRP_MTU1_TCIV1].BIT.IS_MTU1_TCIV1)
	{
		/* MTU channel 1 TCIV1 Overflow */
		/* Call the user function */
		if (rpdl_MTU2_Overflow_callback_func[1] != PDL_NO_FUNC)
		{
			rpdl_MTU2_Overflow_callback_func[1]();
		}
		
		/* Clear the flag by writing to it's clear bit */
		ICU.GCR[GCR_MTU1_TCIV1].BIT.CLR_MTU1_TCIV1 = 1;
	}
		
	if(0 != ICU.GRP[GRP_MTU1_TCIU1].BIT.IS_MTU1_TCIU1)
	{
		/* MTU Channel 1 TCIU1 Underflow */
		/* Call the user function */
		if (rpdl_MTU2_Underflow_callback_func[1] != PDL_NO_FUNC)
		{
			rpdl_MTU2_Underflow_callback_func[1]();
		}
		
		/* Clear the flag by writing to it's clear bit */
		ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1 = 1;
	}
	
	/* Call the user function registerd with INTC (not MTU) */
	if (rpdl_INTC_group_callback_func[1] != PDL_NO_FUNC)
	{
		rpdl_INTC_group_callback_func[1]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_GROUP2
#pragma interrupt Interrupt_Group_2(vect=VECT_ICU_GROUP2, fint)
#else
#pragma interrupt Interrupt_Group_2(vect=VECT_ICU_GROUP2)
#endif
void Interrupt_Group_2(void)
{
	/* The MTU generates this interrupt.*/
	/* Read the Group Interrupt Source Register to determine
	the specific cause of the interrupt */
	if(0 != ICU.GRP[GRP_MTU2_TCIV2].BIT.IS_MTU2_TCIV2)
	{
		/* MTU Channel 2 TCIV2 Overflow */
		if (rpdl_MTU2_Overflow_callback_func[2] != PDL_NO_FUNC)
		{
			/* Call the user function */
			rpdl_MTU2_Overflow_callback_func[2]();
		}
		
		/* Clear the flag by writing to it's clear bit */
		ICU.GCR[GCR_MTU2_TCIV2].BIT.CLR_MTU2_TCIV2 = 1;
	}
	
	if(0 != ICU.GRP[GRP_MTU2_TCIU2].BIT.IS_MTU2_TCIU2)
	{
		/* MTU Channel 2 TCIU2 Underflow */
		if (rpdl_MTU2_Underflow_callback_func[2] != PDL_NO_FUNC)
		{
			/* Call the user function */
			rpdl_MTU2_Underflow_callback_func[2]();
		}
		
		/* Clear the flag by writing to it's clear bit */
		ICU.GCR[GCR_MTU2_TCIU2].BIT.CLR_MTU2_TCIU2 = 1;
	}
	
	if(0 != ICU.GRP[GRP_MTU3_TCIV3].BIT.IS_MTU3_TCIV3)
	{
		/* MTU channel 3 TCIV3 Underflow*/
		if (rpdl_MTU2_Overflow_callback_func[3] != PDL_NO_FUNC)
		{
			/* Call the user function */
			rpdl_MTU2_Overflow_callback_func[3]();
		}
		
		/* Clear the flag by writing to it's clear bit */
		ICU.GCR[GCR_MTU3_TCIV3].BIT.CLR_MTU3_TCIV3 = 1;
	}
	
	/* Call the user function registerd with INTC (not MTU) */
	if (rpdl_INTC_group_callback_func[2] != PDL_NO_FUNC)
	{
		rpdl_INTC_group_callback_func[2]();
	}
}

#if FAST_INTC_VECTOR == VECT_ICU_GROUP3
#pragma interrupt Interrupt_Group_3(vect=VECT_ICU_GROUP3, fint)
#else
#pragma interrupt Interrupt_Group_3(vect=VECT_ICU_GROUP3)
#endif
void Interrupt_Group_3(void)
{
	uint32_t GRP_copy;

	do
	{
		GRP_copy = ICU.GRP[GRP_TPU0_TCI0V].LONG;

		if ((GRP_copy & BIT_0) != 0)
		{
			ICU.GCR[GCR_TPU0_TCI0V].BIT.CLR_TPU0_TCI0V = 1;

			if (rpdl_TPU_Overflow_callback_func[0] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[0]();
			}
		}
		else if ((GRP_copy & BIT_1) != 0)
		{
			ICU.GCR[GCR_TPU1_TCI1V].BIT.CLR_TPU1_TCI1V = 1;
			
			if (rpdl_TPU_Overflow_callback_func[1] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[1]();
			}
		}
		else if ((GRP_copy & BIT_2) != 0)
		{
			ICU.GCR[GCR_TPU1_TCI1U].BIT.CLR_TPU1_TCI1U = 1;
		
			if (rpdl_TPU_Underflow_callback_func[1] != PDL_NO_FUNC)
			{
				rpdl_TPU_Underflow_callback_func[1]();
			}
		}
		else if ((GRP_copy & BIT_3) != 0)
		{
			ICU.GCR[GCR_TPU5_TCI5V].BIT.CLR_TPU5_TCI5V = 1;

			if (rpdl_TPU_Overflow_callback_func[5] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[5]();
			}
		}
		else if ((GRP_copy & BIT_4) != 0)
		{
			ICU.GCR[GCR_TPU5_TCI5U].BIT.CLR_TPU5_TCI5U = 1;

			if (rpdl_TPU_Underflow_callback_func[5] != PDL_NO_FUNC)
			{
				rpdl_TPU_Underflow_callback_func[5]();
			}
		}

		/* Call the user function */
		if (rpdl_INTC_group_callback_func[3] != PDL_NO_FUNC)
		{
			rpdl_INTC_group_callback_func[3]();
		}
	}while(ICU.IR[IR_ICU_GROUP3].BYTE != 0x00u);
}

#if FAST_INTC_VECTOR == VECT_ICU_GROUP4
#pragma interrupt Interrupt_Group_4(vect=VECT_ICU_GROUP4, fint)
#else
#pragma interrupt Interrupt_Group_4(vect=VECT_ICU_GROUP4)
#endif
void Interrupt_Group_4(void)
{
	uint32_t GRP_copy;

	do
	{
		GRP_copy = ICU.GRP[GRP_TPU2_TCI2V].LONG;

		if ((GRP_copy & BIT_0) != 0)
		{
			ICU.GCR[GCR_TPU2_TCI2V].BIT.CLR_TPU2_TCI2V = 1;

			if (rpdl_TPU_Overflow_callback_func[2] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[2]();
			}
		}
		else if ((GRP_copy & BIT_1) != 0)
		{
			ICU.GCR[GCR_TPU2_TCI2U].BIT.CLR_TPU2_TCI2U = 1;

			if (rpdl_TPU_Underflow_callback_func[2] != PDL_NO_FUNC)
			{
				rpdl_TPU_Underflow_callback_func[2]();
			}
		}
		else if ((GRP_copy & BIT_2) != 0)
		{
			ICU.GCR[GCR_TPU3_TCI3V].BIT.CLR_TPU3_TCI3V = 1;

			if (rpdl_TPU_Overflow_callback_func[3] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[3]();
			}
		}
		else if ((GRP_copy & BIT_3) != 0)
		{
			ICU.GCR[GCR_TPU4_TCI4V].BIT.CLR_TPU4_TCI4V = 1;

			if (rpdl_TPU_Overflow_callback_func[4] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[4]();
			}
		}
		else if ((GRP_copy & BIT_4) != 0)
		{
			ICU.GCR[GCR_TPU4_TCI4U].BIT.CLR_TPU4_TCI4U = 1;

			if (rpdl_TPU_Underflow_callback_func[4] != PDL_NO_FUNC)
			{
				rpdl_TPU_Underflow_callback_func[4]();
			}
		}
		
		/* Call the user function */
		if (rpdl_INTC_group_callback_func[4] != PDL_NO_FUNC)
		{
			rpdl_INTC_group_callback_func[4]();
		}
	}while(ICU.IR[IR_ICU_GROUP4].BYTE != 0x00u);
}

#if FAST_INTC_VECTOR == VECT_ICU_GROUP5
#pragma interrupt Interrupt_Group_5(vect=VECT_ICU_GROUP5, fint)
#else
#pragma interrupt Interrupt_Group_5(vect=VECT_ICU_GROUP5)
#endif
void Interrupt_Group_5(void)
{
#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)
	uint32_t GRP_copy;
#endif

	do
	{

#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)

		GRP_copy = ICU.GRP[GRP_TPU6_TCI6V].LONG;

		if ((GRP_copy & BIT_0) != 0)
		{	
			ICU.GCR[GCR_TPU6_TCI6V].BIT.CLR_TPU6_TCI6V = 1;

			if (rpdl_TPU_Overflow_callback_func[6] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[6]();
			}
		}
		else if ((GRP_copy & BIT_1) != 0)
		{	
			ICU.GCR[GCR_TPU7_TCI7V].BIT.CLR_TPU7_TCI7V = 1;

			if (rpdl_TPU_Overflow_callback_func[7] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[7]();
			}
		}
		else if ((GRP_copy & BIT_2) != 0)
		{
			ICU.GCR[GCR_TPU7_TCI7U].BIT.CLR_TPU7_TCI7U = 1;

			if (rpdl_TPU_Underflow_callback_func[7] != PDL_NO_FUNC)
			{
				rpdl_TPU_Underflow_callback_func[7]();
			}
		}
		else if ((GRP_copy & BIT_3) != 0)
		{
			ICU.GCR[GCR_TPU11_TCI11V].BIT.CLR_TPU11_TCI11V = 1;

			if (rpdl_TPU_Overflow_callback_func[11] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[11]();
			}
		}
		else if ((GRP_copy & BIT_4) != 0)
		{
			ICU.GCR[GCR_TPU11_TCI11U].BIT.CLR_TPU11_TCI11U = 1;

			if (rpdl_TPU_Underflow_callback_func[11] != PDL_NO_FUNC)
			{
				rpdl_TPU_Underflow_callback_func[11]();
			}
		}

#endif
		
		/* Call the user function */
		if (rpdl_INTC_group_callback_func[5] != PDL_NO_FUNC)
		{
			rpdl_INTC_group_callback_func[5]();
		}
	}while(ICU.IR[IR_ICU_GROUP5].BYTE != 0x00u);
}

#if FAST_INTC_VECTOR == VECT_ICU_GROUP6
#pragma interrupt Interrupt_Group_6(vect=VECT_ICU_GROUP6, fint)
#else
#pragma interrupt Interrupt_Group_6(vect=VECT_ICU_GROUP6)
#endif
void Interrupt_Group_6(void)
{
#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)
	uint32_t GRP_copy;
#endif

	do
	{

#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)

		GRP_copy = ICU.GRP[GRP_TPU8_TCI8V].LONG;

		if ((GRP_copy & BIT_0) != 0)
		{
			ICU.GCR[GCR_TPU8_TCI8V].BIT.CLR_TPU8_TCI8V = 1;

			if (rpdl_TPU_Overflow_callback_func[8] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[8]();
			}
		}
		else if ((GRP_copy & BIT_1) != 0)
		{
			ICU.GCR[GCR_TPU8_TCI8U].BIT.CLR_TPU8_TCI8U = 1;

			if (rpdl_TPU_Underflow_callback_func[8] != PDL_NO_FUNC)
			{
				rpdl_TPU_Underflow_callback_func[8]();
			}
		}
		else if ((GRP_copy & BIT_2) != 0)
		{
			ICU.GCR[GCR_TPU9_TCI9V].BIT.CLR_TPU9_TCI9V = 1;

			if (rpdl_TPU_Overflow_callback_func[9] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[9]();
			}
		}
		else if ((GRP_copy & BIT_3) != 0)
		{
			ICU.GCR[GCR_TPU10_TCI10V].BIT.CLR_TPU10_TCI10V = 1;

			if (rpdl_TPU_Overflow_callback_func[10] != PDL_NO_FUNC)
			{
				rpdl_TPU_Overflow_callback_func[10]();
			}
		}
		else if ((GRP_copy & BIT_4) != 0)
		{
			ICU.GCR[GCR_TPU10_TCI10U].BIT.CLR_TPU10_TCI10U = 1;

			if (rpdl_TPU_Underflow_callback_func[10] != PDL_NO_FUNC)
			{
				rpdl_TPU_Underflow_callback_func[10]();
			}
		}

#endif
		
		/* Call the user function */
		if (rpdl_INTC_group_callback_func[6] != PDL_NO_FUNC)
		{
			rpdl_INTC_group_callback_func[6]();
		}
	}while(ICU.IR[IR_ICU_GROUP6].BYTE != 0x00u);
}

#if FAST_INTC_VECTOR == VECT_ICU_GROUP12
#pragma interrupt Interrupt_Group_12(vect=VECT_ICU_GROUP12, fint)
#else
#pragma interrupt Interrupt_Group_12(vect=VECT_ICU_GROUP12)
#endif
void Interrupt_Group_12(void)
{
	do
	{
		/* RSPI0 error interrupt */
		if (ICU.GRP[GRP_RSPI0_SPEI0].BIT.IS_RSPI0_SPEI0 == 1)
		{		
			/* Call the user function */
			if (rpdl_SPI_error_callback_func[0] != PDL_NO_FUNC)
			{
				rpdl_SPI_error_callback_func[0]();
			}
		}
		/* RSPI1 error interrupt */
		else if (ICU.GRP[GRP_RSPI1_SPEI1].BIT.IS_RSPI1_SPEI1 == 1)
		{		
			/* Call the user function */
			if (rpdl_SPI_error_callback_func[1] != PDL_NO_FUNC)
			{
				rpdl_SPI_error_callback_func[1]();
			}
		}
#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)
		/* RSPI2 error interrupt */
		else if (ICU.GRP[GRP_RSPI2_SPEI2].BIT.IS_RSPI2_SPEI2 == 1)
		{
			/* Call the user function */
			if (rpdl_SPI_error_callback_func[2] != PDL_NO_FUNC)
			{
				rpdl_SPI_error_callback_func[2]();
			}
		}
#endif
		/* SCI0 error interrupt */
		else if (ICU.GRP[GRP_SCI0_ERI0].BIT.IS_SCI0_ERI0 == 1)
		{
			if (sci_global[0].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[0].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI0.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI0.SSR.BYTE & (uint8_t)BIT_0));
			}
		}

		/* SCI1 error interrupt */
		else if (ICU.GRP[GRP_SCI1_ERI1].BIT.IS_SCI1_ERI1 == 1)
		{
			if (sci_global[1].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[1].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI1.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI1.SSR.BYTE & (uint8_t)BIT_0));
			}
		}

		/* SCI2 error interrupt */
		else if (ICU.GRP[GRP_SCI2_ERI2].BIT.IS_SCI2_ERI2 == 1)
		{
			if (sci_global[2].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[2].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI2.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI2.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
		/* SCI3 error interrupt */
		else if (ICU.GRP[GRP_SCI3_ERI3].BIT.IS_SCI3_ERI3 == 1)
		{
			if (sci_global[3].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[3].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI3.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI3.SSR.BYTE & (uint8_t)BIT_0));
			}
		}

#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)
		/* SCI4 error interrupt */
		else if (ICU.GRP[GRP_SCI4_ERI4].BIT.IS_SCI4_ERI4 == 1)
		{
			if (sci_global[4].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[4].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI4.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI4.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
#endif
		/* SCI5 error interrupt */
		else if (ICU.GRP[GRP_SCI5_ERI5].BIT.IS_SCI5_ERI5 == 1)
		{
			if (sci_global[5].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[5].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI5.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI5.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
		/* SCI6 error interrupt */
		else if (ICU.GRP[GRP_SCI6_ERI6].BIT.IS_SCI6_ERI6 == 1)
		{
			if (sci_global[6].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[6].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI6.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI6.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)
		/* SCI7 error interrupt */
		else if (ICU.GRP[GRP_SCI7_ERI7].BIT.IS_SCI7_ERI7 == 1)
		{
			if (sci_global[7].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[7].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI7.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI7.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
#endif
		/* SCI8 error interrupt */
		else if (ICU.GRP[GRP_SCI8_ERI8].BIT.IS_SCI8_ERI8 == 1)
		{
			if (sci_global[8].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[8].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI8.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI8.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
		/* SCI9 error interrupt */
		else if (ICU.GRP[GRP_SCI9_ERI9].BIT.IS_SCI9_ERI9 == 1)
		{
			if (sci_global[9].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[9].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI9.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI9.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
#if defined (DEVICE_PACKAGE_177_PIN) || defined (DEVICE_PACKAGE_176_PIN) || defined (DEVICE_PACKAGE_145_PIN) || defined (DEVICE_PACKAGE_144_PIN)
		/* SCI10 error interrupt */
		else if (ICU.GRP[GRP_SCI10_ERI10].BIT.IS_SCI10_ERI10 == 1)
		{
			if (sci_global[10].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[10].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI10.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI10.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
		/* SCI11 error interrupt */
		else if (ICU.GRP[GRP_SCI11_ERI11].BIT.IS_SCI11_ERI11 == 1)
		{
			if (sci_global[11].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[11].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI11.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI11.SSR.BYTE & (uint8_t)BIT_0));
			}
		}
#endif
		/* SCI12 error interrupt */
		else if (ICU.GRP[GRP_SCI12_ERI12].BIT.IS_SCI12_ERI12 == 1)
		{
			if (sci_global[12].RX_Error_callback_func != PDL_NO_FUNC)
			{
				/* Call the error handler */
				sci_global[12].RX_Error_callback_func();
			}
			else
			{
				/* Clear the SSR error flags;
				Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
				SCI12.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) | (SCI12.SSR.BYTE & (uint8_t)BIT_0));
			}
		}

		/* Call the user function */
		if (rpdl_INTC_group_callback_func[7] != PDL_NO_FUNC)
		{
			rpdl_INTC_group_callback_func[7]();
		}
	}while(ICU.IR[IR_ICU_GROUP12].BYTE != 0x00u);
}

/*""FUNC COMMENT""*************************************************************
* Module outline: Software interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_SWINT(void)
*------------------------------------------------------------------------------
* Function		: Interrupt processing function for the software interrupt
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input			: 
* Output		: 
*------------------------------------------------------------------------------
* Use function	: rpdl_INTC_callback_func[PDL_INTC_SWINT]()
*------------------------------------------------------------------------------
* Notes		 	:
*------------------------------------------------------------------------------
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FUNC COMMENT END""*********************************************************/
#if FAST_INTC_VECTOR == VECT_ICU_SWINT
#pragma interrupt Interrupt_SWINT(vect=VECT_ICU_SWINT, fint)
#else
#pragma interrupt Interrupt_SWINT(vect=VECT_ICU_SWINT)
#endif
void Interrupt_SWINT(void)
{
	/* Call the user function */
	if (rpdl_INTC_callback_func[PDL_INTC_SWINT] != PDL_NO_FUNC)
	{
		rpdl_INTC_callback_func[PDL_INTC_SWINT]();
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline: Break interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_BRK(void)
*------------------------------------------------------------------------------
* Function		: Interrupt processing function for the break interrupt
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input			: 
* Output		: 
*------------------------------------------------------------------------------
* Use function	: 
*------------------------------------------------------------------------------
* Notes		 	:
*------------------------------------------------------------------------------
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FUNC COMMENT END""*********************************************************/
#pragma interrupt Interrupt_BRK(vect=0)
void Interrupt_BRK(void)
{
	uint32_t * vector_location;
	volatile uint32_t * stacked_psw_ptr;
	uint32_t psw_copy;

	/* The low-power mode cases assume that register
	   write protection was enabled (in R_LPC_Control) */

	switch (rpdl_INTC_brk_command)
	{
#if !defined (DEVICE_PACKAGE_64_PIN) && !defined (DEVICE_PACKAGE_48_PIN)
		case BRK_START_ADC_10:
			rpdl_ADC_10_Start();
			break;
		case BRK_START_ADC_10_AND_SLEEP:
			/* Start the ADC */
			rpdl_ADC_10_Start();
			/* Prevent all-module clock stop */
			SYSTEM.MSTPCRA.BIT.ACSE = 0;
			/* Select sleep or all-module clock stop */
			SYSTEM.SBYCR.BIT.SSBY = 0;
			/* Prevent out-of-order execution */
			while (SYSTEM.SBYCR.BIT.SSBY != 0);
			/* Initiate the low-power mode */
			wait();
			break;
#endif
		case BRK_SLEEP:
			/* Prevent all-module clock stop */
			SYSTEM.MSTPCRA.BIT.ACSE = 0;
			/* Select sleep or all-module clock stop */
			SYSTEM.SBYCR.BIT.SSBY = 0;
			/* Prevent out-of-order execution */
			while (SYSTEM.SBYCR.BIT.SSBY != 0);
			/* Initiate the low-power mode */
			wait();
			break;
		case BRK_ALL_MODULE_CLOCK_STOP:
			/* Select sleep or all-module clock stop */
			SYSTEM.SBYCR.BIT.SSBY = 0;
			/* Prevent out-of-order execution */
			while (SYSTEM.SBYCR.BIT.SSBY != 0);
			/* Initiate the low-power mode */
			wait();
			break;
		case BRK_STANDBY:
			/* Prevent deep standby mode */
			SYSTEM.DPSBYCR.BIT.DPSBY = 0;
			/* Select standby mode */
			SYSTEM.SBYCR.BIT.SSBY = 1;
			/* Prevent out-of-order execution */
			while (SYSTEM.SBYCR.BIT.SSBY != 1);
			/* Initiate the low-power mode */
			wait();
			break;
		case BRK_DEEP_STANDBY:
			/* Stop the RAM clock */
			SYSTEM.MSTPCRC.BIT.MSTPC0 = 1;
			SYSTEM.MSTPCRC.BIT.MSTPC1 = 1;
			/* Select deep standby mode */
			SYSTEM.DPSBYCR.BIT.DPSBY = 1;
			/* Select standby mode */
			SYSTEM.SBYCR.BIT.SSBY = 1;
			/* Prevent out-of-order execution */
			while (SYSTEM.SBYCR.BIT.SSBY != 1);
			/* Initiate the low-power mode */
			wait();
			break;
		case BRK_LOAD_FINTV_REGISTER:
			/* Find the start of the maskable interrupt vector table */
			vector_location = (uint32_t *)get_intb();
			/* Move to the location of the address of the selected interrupt */
			vector_location += ICU.FIR.BIT.FVCT;
			/* Store the address of this vector in the FINTV register */
			set_fintv((void *)*vector_location);
			break;
		case BRK_WRITE_IPL:
			/* Find the stacked PSW */
			stacked_psw_ptr = (uint32_t *)(rpdl_INTC_saved_isp) - 1;
			/* Read the stacked PSW */
			psw_copy = *stacked_psw_ptr;
			/* Clear the IPL bits */
			psw_copy &= (uint32_t)(INV_BIT_27 & INV_BIT_26 & INV_BIT_25 & INV_BIT_24);
			/* Modify the IPL bits */
			psw_copy |= (uint32_t)(rpdl_INTC_brk_data8 << 24);
			/* Modify the saved IPL */
			*stacked_psw_ptr = psw_copy;
			break;
		case BRK_CMT_START:
			/* Set STRn to 1 */
			*(CMSTR_ADDRESS(rpdl_INTC_brk_data8)) |= (uint16_t)((rpdl_INTC_brk_data8 & 0x01u) + 1);
			break;
		case BRK_CMT_STOP:
			/* Set STRn to 0 */
			*(CMSTR_ADDRESS(rpdl_INTC_brk_data8)) &= (uint16_t)(0x2u >> (rpdl_INTC_brk_data8 & 0x01u));
			break;
		case BRK_MODIFY_PROTECTION:
			/* Call the modification function */
			rpdl_modify_protection(rpdl_INTC_brk_data8);
			break;
		default:
			/* This should never occur when triggered by RPDL */
			break;
	}

	rpdl_INTC_brk_command = BRK_NO_COMMAND;
}

/* Exception (Privileged Instruction) */
#pragma interrupt Excep_PrivilegedInst
void Excep_PrivilegedInst(void)
{
	if (rpdl_INTC_callback_func[PDL_INTC_PRIVILEGED] != PDL_NO_FUNC)
	{
		/* Call the user function */
		rpdl_INTC_callback_func[PDL_INTC_PRIVILEGED]();
	}
}

/* Exception (Access) */
#pragma interrupt Excep_Access
void Excep_Access(void)
{
	if (rpdl_INTC_callback_func[PDL_INTC_ACCESS] != PDL_NO_FUNC)
	{
		/* Call the user function */
		rpdl_INTC_callback_func[PDL_INTC_ACCESS]();
	}
}

/* Exception (Undefined Instruction) */
#pragma interrupt Excep_UndefinedInst
void Excep_UndefinedInst(void)
{
	if (rpdl_INTC_callback_func[PDL_INTC_UNDEFINED] != PDL_NO_FUNC)
	{
		/* Call the user function */
		rpdl_INTC_callback_func[PDL_INTC_UNDEFINED]();
	}
}

/* Exception(Floating Point) */
#pragma interrupt Excep_FloatingPoint
void Excep_FloatingPoint(void)
{
	if (rpdl_INTC_callback_func[PDL_INTC_FLOATING_POINT] != PDL_NO_FUNC)
	{
		/* Call the user function */
		rpdl_INTC_callback_func[PDL_INTC_FLOATING_POINT]();
	}
}

#pragma interrupt Interrupt_NMI
void Interrupt_NMI(void)
{
	if (rpdl_INTC_callback_func[PDL_INTC_NMI] != PDL_NO_FUNC)
	{
		/* Call the user function */
		rpdl_INTC_callback_func[PDL_INTC_NMI]();
	}
}

/* Padding function for the fixed-vector table */
void Dummy(void){}

#pragma section C FIXEDVECT

void (* const Fixed_Vectors[12])(void) = {
/* 0xFFFFFFD0	Privileged Instruction */
	Excep_PrivilegedInst,
/* 0xFFFFFFD4	Reserved */
	Excep_Access,
/* 0xFFFFFFD8	Reserved */
	Dummy,
/* 0xFFFFFFDC	Undefined Instruction */
	Excep_UndefinedInst,
/* 0xFFFFFFE0	Reserved */
	Dummy,
/* 0xFFFFFFE4	Floating Point */
	Excep_FloatingPoint,
/* 0xFFFFFFE8	Reserved */
	Dummy,
/* 0xFFFFFFEC	Reserved */
	Dummy,
/* 0xFFFFFFF0	Reserved */
	Dummy,
/* 0xFFFFFFF4	Reserved */
	Dummy,
/* 0xFFFFFFF8	NMI */
	Interrupt_NMI,
/* 0xFFFFFFFC	RESET */
	PowerON_Reset_PC
};

/* Set the endian mode */
#pragma address MDES_register = 0xFFFFFF80 /* MDES register (Single Chip Mode) */
#ifdef __BIG
const unsigned long MDES_register = 0xFFFFFFF8u; // Big endian
#else
const unsigned long MDES_register = 0xFFFFFFFFu; // Little endian
#endif
/* End of file */
