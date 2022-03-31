/*""FILE COMMENT""*************************************************************
* System Name	: Interrupt program for RX63N
* File Name		: Interrupt_SCI.c
* Version		: 1.21
* Contents		: Interrupt handlers for all SCI channels
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

#include "r_pdl_sci.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/* This file uses some SCI utility functions */
extern bool rpdl_SCI_SetTxPinGeneralIO(const uint8_t channel,
									   const bool bGeneralIO);
extern void rpdl_SCI_ModifyTx_IER(const uint8_t, const uint8_t);
extern void rpdl_SCI_ModifyTxEnd_IER(const uint8_t, const uint8_t);
extern void R_SCI_IIC_Handler_RXI(const uint8_t channel);
extern void R_SCI_IIC_Handler_TXI(const uint8_t channel);

/*""FUNC COMMENT""*************************************************************
* Module outline: SCIn receive data interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_SCIn_RXIn(void)
*------------------------------------------------------------------------------
* Function		: 
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Output		: 
*------------------------------------------------------------------------------
* Use function	: rpdl_SCI_RX_End_callback_func
*------------------------------------------------------------------------------
* Notes			: 
*------------------------------------------------------------------------------
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FUNC COMMENT END""*********************************************************/
#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI0_RXI0
#pragma interrupt Interrupt_SCI0_RXI0(vect=VECT_SCI0_RXI0, fint)
#else
#pragma interrupt Interrupt_SCI0_RXI0(vect=VECT_SCI0_RXI0)
#endif
void Interrupt_SCI0_RXI0(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI0.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(0);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[0].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[0].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI0.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI0.RDR;

				/* ID matching ? */
				if (id != sci_global[0].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI0.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI0.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[0].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[0].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[0].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
		
		/* Read and store the character */
		*sci_global[0].rx_string_pointer = SCI0.RDR;

		/* Increment the character counter */
		sci_global[0].rx_counter++;

		/* More characters expected? */
		if (sci_global[0].rx_counter < sci_global[0].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[0].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[0].rx_continuous == true)
			{
				sci_global[0].rx_string_pointer = sci_global[0].rx_string_start;
				sci_global[0].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI0.SPMR.BIT.CKPH == 1) && (SCI0.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[0].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI0.SMR.BIT.CM == 1) &&
			   ((SCI0.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI0.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[0].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI0.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI0.SMR.BIT.MP) != 0) &&
				(sci_global[0].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI0.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[0].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[0].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[0].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[0].RX_End_callback_func();
		}
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI1_RXI1
#pragma interrupt Interrupt_SCI1_RXI1(vect=VECT_SCI1_RXI1, fint)
#else
//#pragma interrupt Interrupt_SCI1_RXI1(vect=VECT_SCI1_RXI1)
#endif
void Interrupt_SCI1_RXI1(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI1.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(1);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[1].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[1].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI1.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI1.RDR;

				/* ID matching ? */
				if (id != sci_global[1].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI1.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI1.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[1].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[1].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[1].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
		
		/* Read and store the character */
		*sci_global[1].rx_string_pointer = SCI1.RDR;

		/* Increment the character counter */
		sci_global[1].rx_counter++;

		/* More characters expected? */
		if (sci_global[1].rx_counter < sci_global[1].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[1].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[1].rx_continuous == true)
			{
				sci_global[1].rx_string_pointer = sci_global[1].rx_string_start;
				sci_global[1].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI1.SPMR.BIT.CKPH == 1) && (SCI1.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[1].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI1.SMR.BIT.CM == 1) &&
			   ((SCI1.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI1.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[1].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI1.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI1.SMR.BIT.MP) != 0) &&
				(sci_global[1].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI1.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[1].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[1].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[1].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[1].RX_End_callback_func();
		}
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI2_RXI2
#pragma interrupt Interrupt_SCI2_RXI2(vect=VECT_SCI2_RXI2, fint)
#else
#pragma interrupt Interrupt_SCI2_RXI2(vect=VECT_SCI2_RXI2)
#endif
void Interrupt_SCI2_RXI2(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI2.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(2);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[2].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[2].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI2.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI2.RDR;

				/* ID matching ? */
				if (id != sci_global[2].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI2.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI2.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[2].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[2].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[2].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
		
		/* Read and store the character */
		*sci_global[2].rx_string_pointer = SCI2.RDR;

		/* Increment the character counter */
		sci_global[2].rx_counter++;

		/* More characters expected? */
		if (sci_global[2].rx_counter < sci_global[2].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[2].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[2].rx_continuous == true)
			{
				sci_global[2].rx_string_pointer = sci_global[2].rx_string_start;
				sci_global[2].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI2.SPMR.BIT.CKPH == 1) && (SCI2.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[2].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI2.SMR.BIT.CM == 1) &&
			   ((SCI2.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI2.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[2].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI2.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI2.SMR.BIT.MP) != 0) &&
				(sci_global[2].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI2.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[2].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[2].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[2].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[2].RX_End_callback_func();
		}
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI3_RXI3
#pragma interrupt Interrupt_SCI3_RXI3(vect=VECT_SCI3_RXI3, fint)
#else
#pragma interrupt Interrupt_SCI3_RXI3(vect=VECT_SCI3_RXI3)
#endif
void Interrupt_SCI3_RXI3(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI3.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(3);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[3].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[3].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI3.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI3.RDR;

				/* ID matching ? */
				if (id != sci_global[3].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI3.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI3.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[3].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[3].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[3].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
		
		/* Read and store the character */
		*sci_global[3].rx_string_pointer = SCI3.RDR;

		/* Increment the character counter */
		sci_global[3].rx_counter++;

		/* More characters expected? */
		if (sci_global[3].rx_counter < sci_global[3].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[3].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[3].rx_continuous == true)
			{
				sci_global[3].rx_string_pointer = sci_global[3].rx_string_start;
				sci_global[3].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI3.SPMR.BIT.CKPH == 1) && (SCI3.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[3].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI3.SMR.BIT.CM == 1) &&
			   ((SCI3.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI3.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[3].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI3.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI3.SMR.BIT.MP) != 0) &&
				(sci_global[3].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI3.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[3].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[3].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[3].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[3].RX_End_callback_func();
		}
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI4_RXI4
#pragma interrupt Interrupt_SCI4_RXI4(vect=VECT_SCI4_RXI4, fint)
#else
#pragma interrupt Interrupt_SCI4_RXI4(vect=VECT_SCI4_RXI4)
#endif
void Interrupt_SCI4_RXI4(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI4.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(4);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[4].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[4].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI4.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI4.RDR;

				/* ID matching ? */
				if (id != sci_global[4].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI4.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI4.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[4].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[4].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[4].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
		
		/* Read and store the character */
		*sci_global[4].rx_string_pointer = SCI4.RDR;

		/* Increment the character counter */
		sci_global[4].rx_counter++;

		/* More characters expected? */
		if (sci_global[4].rx_counter < sci_global[4].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[4].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[4].rx_continuous == true)
			{
				sci_global[4].rx_string_pointer = sci_global[4].rx_string_start;
				sci_global[4].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI4.SPMR.BIT.CKPH == 1) && (SCI4.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[4].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI4.SMR.BIT.CM == 1) &&
			   ((SCI4.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI4.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[4].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI4.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI4.SMR.BIT.MP) != 0) &&
				(sci_global[4].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI4.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[4].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[4].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[4].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[4].RX_End_callback_func();
		}
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI5_RXI5
#pragma interrupt Interrupt_SCI5_RXI5(vect=VECT_SCI5_RXI5, fint)
#else
#pragma interrupt Interrupt_SCI5_RXI5(vect=VECT_SCI5_RXI5)
#endif
void Interrupt_SCI5_RXI5(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI5.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(5);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[5].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[5].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI5.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI5.RDR;

				/* ID matching ? */
				if (id != sci_global[5].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI5.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI5.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[5].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[5].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[5].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
				
		/* Read and store the character */
		*sci_global[5].rx_string_pointer = SCI5.RDR;

		/* Increment the character counter */
		sci_global[5].rx_counter++;

		/* More characters expected? */
		if (sci_global[5].rx_counter < sci_global[5].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[5].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[5].rx_continuous == true)
			{
				sci_global[5].rx_string_pointer = sci_global[5].rx_string_start;
				sci_global[5].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI5.SPMR.BIT.CKPH == 1) && (SCI5.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[5].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI5.SMR.BIT.CM == 1) &&
			   ((SCI5.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI5.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[5].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI5.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI5.SMR.BIT.MP) != 0) &&
				(sci_global[5].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI5.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[5].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[5].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[5].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[5].RX_End_callback_func();
		}
	}
}

#if FAST_INTC_VECTOR == VECT_SCI6_RXI6
#pragma interrupt Interrupt_SCI6_RXI6(vect=VECT_SCI6_RXI6, fint)
#else
// #pragma interrupt Interrupt_SCI6_RXI6(vect=VECT_SCI6_RXI6)
#endif
void Interrupt_SCI6_RXI6(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI6.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(6);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[6].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[6].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI6.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI6.RDR;

				/* ID matching ? */
				if (id != sci_global[6].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI6.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI6.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[6].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[6].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[6].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
				
		/* Read and store the character */
		*sci_global[6].rx_string_pointer = SCI6.RDR;

		/* Increment the character counter */
		sci_global[6].rx_counter++;

		/* More characters expected? */
		if (sci_global[6].rx_counter < sci_global[6].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[6].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[6].rx_continuous == true)
			{
				sci_global[6].rx_string_pointer = sci_global[6].rx_string_start;
				sci_global[6].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI6.SPMR.BIT.CKPH == 1) && (SCI6.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[6].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE.*/
			if((SCI6.SMR.BIT.CM == 1) &&
			   ((SCI6.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI6.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[6].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI6.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}


			/* Async MP mode ? */
			if (((SCI6.SMR.BIT.MP) != 0) &&
				(sci_global[6].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI6.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[6].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[6].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[6].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[6].RX_End_callback_func();
		}
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI7_RXI7
#pragma interrupt Interrupt_SCI7_RXI7(vect=VECT_SCI7_RXI7, fint)
#else
#pragma interrupt Interrupt_SCI7_RXI7(vect=VECT_SCI7_RXI7)
#endif
void Interrupt_SCI7_RXI7(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI7.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(7);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[7].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[7].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI7.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI7.RDR;

				/* ID matching ? */
				if (id != sci_global[7].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI7.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI7.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[7].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[7].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[7].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
		
		/* Read and store the character */
		*sci_global[7].rx_string_pointer = SCI7.RDR;

		/* Increment the character counter */
		sci_global[7].rx_counter++;

		/* More characters expected? */
		if (sci_global[7].rx_counter < sci_global[7].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[7].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[7].rx_continuous == true)
			{
				sci_global[7].rx_string_pointer = sci_global[7].rx_string_start;
				sci_global[7].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI7.SPMR.BIT.CKPH == 1) && (SCI7.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[7].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI7.SMR.BIT.CM == 1) &&
			   ((SCI7.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI7.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[7].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI7.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI7.SMR.BIT.MP) != 0) &&
				(sci_global[7].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI7.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[7].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[7].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[7].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[7].RX_End_callback_func();
		}
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI8_RXI8
#pragma interrupt Interrupt_SCI8_RXI8(vect=VECT_SCI8_RXI8, fint)
#else
#pragma interrupt Interrupt_SCI8_RXI8(vect=VECT_SCI8_RXI8)
#endif
void Interrupt_SCI8_RXI8(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI8.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(8);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[8].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[8].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI8.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI8.RDR;

				/* ID matching ? */
				if (id != sci_global[8].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI8.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI8.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[8].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[8].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[8].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
				
		/* Read and store the character */
		*sci_global[8].rx_string_pointer = SCI8.RDR;

		/* Increment the character counter */
		sci_global[8].rx_counter++;

		/* More characters expected? */
		if (sci_global[8].rx_counter < sci_global[8].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[8].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[8].rx_continuous == true)
			{
				sci_global[8].rx_string_pointer = sci_global[8].rx_string_start;
				sci_global[8].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI8.SPMR.BIT.CKPH == 1) && (SCI8.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[8].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI8.SMR.BIT.CM == 1) &&
			   ((SCI8.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI8.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[8].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI8.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI8.SMR.BIT.MP) != 0) &&
				(sci_global[8].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI8.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[8].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[8].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[8].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[8].RX_End_callback_func();
		}
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN)
#if FAST_INTC_VECTOR == VECT_SCI9_RXI9
#pragma interrupt Interrupt_SCI9_RXI9(vect=VECT_SCI9_RXI9, fint)
#else
#pragma interrupt Interrupt_SCI9_RXI9(vect=VECT_SCI9_RXI9)
#endif
void Interrupt_SCI9_RXI9(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI9.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(9);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[9].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[9].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI9.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI9.RDR;

				/* ID matching ? */
				if (id != sci_global[9].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI9.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI9.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[9].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[9].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[9].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
				
		/* Read and store the character */
		*sci_global[9].rx_string_pointer = SCI9.RDR;

		/* Increment the character counter */
		sci_global[9].rx_counter++;

		/* More characters expected? */
		if (sci_global[9].rx_counter < sci_global[9].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[9].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[9].rx_continuous == true)
			{
				sci_global[9].rx_string_pointer = sci_global[9].rx_string_start;
				sci_global[9].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI9.SPMR.BIT.CKPH == 1) && (SCI9.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[9].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI9.SMR.BIT.CM == 1) &&
			   ((SCI9.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI9.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[9].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI9.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI9.SMR.BIT.MP) != 0) &&
				(sci_global[9].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI9.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[9].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[9].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[9].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[9].RX_End_callback_func();
		}
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI10_RXI10
#pragma interrupt Interrupt_SCI10_RXI10(vect=VECT_SCI10_RXI10, fint)
#else
#pragma interrupt Interrupt_SCI10_RXI10(vect=VECT_SCI10_RXI10)
#endif
void Interrupt_SCI10_RXI10(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI10.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(10);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[10].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[10].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI10.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI10.RDR;

				/* ID matching ? */
				if (id != sci_global[10].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI10.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI10.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[10].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[10].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[10].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
		
		/* Read and store the character */
		*sci_global[10].rx_string_pointer = SCI10.RDR;

		/* Increment the character counter */
		sci_global[10].rx_counter++;

		/* More characters expected? */
		if (sci_global[10].rx_counter < sci_global[10].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[10].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[10].rx_continuous == true)
			{
				sci_global[10].rx_string_pointer = sci_global[10].rx_string_start;
				sci_global[10].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI10.SPMR.BIT.CKPH == 1) && (SCI10.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[10].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI10.SMR.BIT.CM == 1) &&
			   ((SCI10.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI10.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[10].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI10.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI10.SMR.BIT.MP) != 0) &&
				(sci_global[10].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI10.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[10].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[10].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[10].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[10].RX_End_callback_func();
		}
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI11_RXI11
#pragma interrupt Interrupt_SCI11_RXI11(vect=VECT_SCI11_RXI11, fint)
#else
#pragma interrupt Interrupt_SCI11_RXI11(vect=VECT_SCI11_RXI11)
#endif
void Interrupt_SCI11_RXI11(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI11.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(11);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[11].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[11].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI11.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI11.RDR;

				/* ID matching ? */
				if (id != sci_global[11].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI11.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI11.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[11].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[11].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[11].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
		
		/* Read and store the character */
		*sci_global[11].rx_string_pointer = SCI11.RDR;

		/* Increment the character counter */
		sci_global[11].rx_counter++;

		/* More characters expected? */
		if (sci_global[11].rx_counter < sci_global[11].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[11].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[11].rx_continuous == true)
			{
				sci_global[11].rx_string_pointer = sci_global[11].rx_string_start;
				sci_global[11].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI11.SPMR.BIT.CKPH == 1) && (SCI11.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[11].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI11.SMR.BIT.CM == 1) &&
			   ((SCI11.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI11.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[11].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI11.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI11.SMR.BIT.MP) != 0) &&
				(sci_global[11].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI11.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[11].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[11].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[11].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[11].RX_End_callback_func();
		}
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI12_RXI12
#pragma interrupt Interrupt_SCI12_RXI12(vect=VECT_SCI12_RXI12, fint)
#else
#pragma interrupt Interrupt_SCI12_RXI12(vect=VECT_SCI12_RXI12)
#endif
void Interrupt_SCI12_RXI12(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI12.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(12);
		return;
	}
	
	/* Ok to process the data? */
	if (sci_global[12].rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (sci_global[12].MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI12.SSR.BIT.MPB == 1)
			{
				uint8_t id;
				
				/* Read the ID */
				id = SCI12.RDR;

				/* ID matching ? */
				if (id != sci_global[12].MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI12.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
					
					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					sci_global[12].MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (sci_global[12].RX_End_callback_func != PDL_NO_FUNC)
					{
						sci_global[12].RX_End_callback_func();
					}					
				}	
				
				/* Exit ISR */
				return;	
			}
		}
				
		/* Read and store the character */
		*sci_global[12].rx_string_pointer = SCI12.RDR;

		/* Increment the character counter */
		sci_global[12].rx_counter++;

		/* More characters expected? */
		if (sci_global[12].rx_counter < sci_global[12].rx_threshold)
		{
			/* Move to the next location in the buffer */
			sci_global[12].rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected,  data will be stored to the top of receive buffer 
			after callback function is triggerred. */
			if (sci_global[12].rx_continuous == true)
			{
				sci_global[12].rx_string_pointer = sci_global[12].rx_string_start;
				sci_global[12].rx_counter = 0;
			}
			
			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI12.SPMR.BIT.CKPH == 1) && (SCI12.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)sci_global[12].baud_rate);
			}
			
			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI12.SMR.BIT.CM == 1) &&
			   ((SCI12.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);	
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (sci_global[12].rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI12.SMR.BIT.MP) != 0) &&
				(sci_global[12].MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI12.SCR.BIT.MPIE = 1;		
			}

			/* Notify the user */
			if (sci_global[12].RX_End_callback_func != PDL_NO_FUNC)
			{
				sci_global[12].RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[12].RX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[12].RX_End_callback_func();
		}
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline: SCIn transmit data interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_SCIn_TXIn(void)
*------------------------------------------------------------------------------
* Function		: 
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Output		: TDR for that SCI channel
*------------------------------------------------------------------------------
* Use function	: 
*------------------------------------------------------------------------------
* Notes			: 
*------------------------------------------------------------------------------
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FUNC COMMENT END""*********************************************************/

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI0_TXI0
#pragma interrupt Interrupt_SCI0_TXI0(vect=VECT_SCI0_TXI0, fint)
#else
#pragma interrupt Interrupt_SCI0_TXI0(vect=VECT_SCI0_TXI0)
#endif
void Interrupt_SCI0_TXI0(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI0.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(0);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[0].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[0].tx_threshold == 0) && (*sci_global[0].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[0].tx_counter < sci_global[0].tx_threshold)
		)
		{
			/* Send the character */
			SCI0.TDR = *sci_global[0].tx_string_pointer;

			/* Increment the pointer */
			sci_global[0].tx_string_pointer++;

			/* Increment the counter */
			sci_global[0].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[0].tx_threshold == 0) &&
				(*sci_global[0].tx_string_pointer == NULL)) ||
				(sci_global[0].tx_counter == sci_global[0].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI0.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI0.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(0, 0);
					
					/*Enable Transmit End Interrupt */
					SCI0.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(0, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI0.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI0.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI0.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[0].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[0].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[0].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[0].TX_End_callback_func();
		}
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI1_TXI1
#pragma interrupt Interrupt_SCI1_TXI1(vect=VECT_SCI1_TXI1, fint)
#else
#pragma interrupt Interrupt_SCI1_TXI1(vect=VECT_SCI1_TXI1)
#endif
void Interrupt_SCI1_TXI1(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI1.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(1);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[1].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[1].tx_threshold == 0) && (*sci_global[1].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[1].tx_counter < sci_global[1].tx_threshold)
		)
		{
			/* Send the character */
			SCI1.TDR = *sci_global[1].tx_string_pointer;

			/* Increment the pointer */
			sci_global[1].tx_string_pointer++;

			/* Increment the counter */
			sci_global[1].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[1].tx_threshold == 0) &&
				(*sci_global[1].tx_string_pointer == NULL)) ||
				(sci_global[1].tx_counter == sci_global[1].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI1.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI1.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(1, 0);
					
					/*Enable Transmit End Interrupt */
					SCI1.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(1, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI1.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI1.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI1.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[1].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[1].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[1].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[1].TX_End_callback_func();
		}
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI2_TXI2
#pragma interrupt Interrupt_SCI2_TXI2(vect=VECT_SCI2_TXI2, fint)
#else
#pragma interrupt Interrupt_SCI2_TXI2(vect=VECT_SCI2_TXI2)
#endif
void Interrupt_SCI2_TXI2(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI2.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(2);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[2].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[2].tx_threshold == 0) && (*sci_global[2].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[2].tx_counter < sci_global[2].tx_threshold)
		)
		{
			/* Send the character */
			SCI2.TDR = *sci_global[2].tx_string_pointer;

			/* Increment the pointer */
			sci_global[2].tx_string_pointer++;

			/* Increment the counter */
			sci_global[2].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[2].tx_threshold == 0) &&
				(*sci_global[2].tx_string_pointer == NULL)) ||
				(sci_global[2].tx_counter == sci_global[2].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI2.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI2.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(2, 0);
					
					/*Enable Transmit End Interrupt */
					SCI2.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(2, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI2.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI2.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI2.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[2].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[2].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[2].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[2].TX_End_callback_func();
		}
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI3_TXI3
#pragma interrupt Interrupt_SCI3_TXI3(vect=VECT_SCI3_TXI3, fint)
#else
#pragma interrupt Interrupt_SCI3_TXI3(vect=VECT_SCI3_TXI3)
#endif
void Interrupt_SCI3_TXI3(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI3.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(3);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[3].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[3].tx_threshold == 0) && (*sci_global[3].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[3].tx_counter < sci_global[3].tx_threshold)
		)
		{
			/* Send the character */
			SCI3.TDR = *sci_global[3].tx_string_pointer;

			/* Increment the pointer */
			sci_global[3].tx_string_pointer++;

			/* Increment the counter */
			sci_global[3].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[3].tx_threshold == 0) &&
				(*sci_global[3].tx_string_pointer == NULL)) ||
				(sci_global[3].tx_counter == sci_global[3].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI3.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI3.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(3, 0);
					
					/*Enable Transmit End Interrupt */
					SCI3.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(3, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI3.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI3.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI3.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[3].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[3].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[3].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[3].TX_End_callback_func();
		}
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI4_TXI4
#pragma interrupt Interrupt_SCI4_TXI4(vect=VECT_SCI4_TXI4, fint)
#else
#pragma interrupt Interrupt_SCI4_TXI4(vect=VECT_SCI4_TXI4)
#endif
void Interrupt_SCI4_TXI4(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI4.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(4);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[4].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[4].tx_threshold == 0) && (*sci_global[4].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[4].tx_counter < sci_global[4].tx_threshold)
		)
		{
			/* Send the character */
			SCI4.TDR = *sci_global[4].tx_string_pointer;

			/* Increment the pointer */
			sci_global[4].tx_string_pointer++;

			/* Increment the counter */
			sci_global[4].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[4].tx_threshold == 0) &&
				(*sci_global[4].tx_string_pointer == NULL)) ||
				(sci_global[4].tx_counter == sci_global[4].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI4.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI4.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(4, 0);
					
					/*Enable Transmit End Interrupt */
					SCI4.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(4, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI4.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI4.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI4.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[4].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[4].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[4].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[4].TX_End_callback_func();
		}
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI5_TXI5
#pragma interrupt Interrupt_SCI5_TXI5(vect=VECT_SCI5_TXI5, fint)
#else
#pragma interrupt Interrupt_SCI5_TXI5(vect=VECT_SCI5_TXI5)
#endif
void Interrupt_SCI5_TXI5(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI5.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(5);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[5].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[5].tx_threshold == 0) && (*sci_global[5].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[5].tx_counter < sci_global[5].tx_threshold)
		)
		{
			/* Send the character */
			SCI5.TDR = *sci_global[5].tx_string_pointer;

			/* Increment the pointer */
			sci_global[5].tx_string_pointer++;

			/* Increment the counter */
			sci_global[5].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[5].tx_threshold == 0) &&
				(*sci_global[5].tx_string_pointer == NULL)) ||
				(sci_global[5].tx_counter == sci_global[5].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI5.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI5.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(5, 0);
					
					/*Enable Transmit End Interrupt */
					SCI5.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(5, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI5.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI5.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI5.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[5].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[5].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[5].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[5].TX_End_callback_func();
		}
	}
}

#if FAST_INTC_VECTOR == VECT_SCI6_TXI6
#pragma interrupt Interrupt_SCI6_TXI6(vect=VECT_SCI6_TXI6, fint)
#else
#pragma interrupt Interrupt_SCI6_TXI6(vect=VECT_SCI6_TXI6)
#endif
void Interrupt_SCI6_TXI6(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI6.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(6);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[6].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[6].tx_threshold == 0) && (*sci_global[6].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[6].tx_counter < sci_global[6].tx_threshold)
		)
		{
			/* Send the character */
			SCI6.TDR = *sci_global[6].tx_string_pointer;

			/* Increment the pointer */
			sci_global[6].tx_string_pointer++;

			/* Increment the counter */
			sci_global[6].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[6].tx_threshold == 0) &&
				(*sci_global[6].tx_string_pointer == NULL)) ||
				(sci_global[6].tx_counter == sci_global[6].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI6.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI6.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(6, 0);
					
					/*Enable Transmit End Interrupt */
					SCI6.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(6, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI6.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI6.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI6.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[6].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[6].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[6].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[6].TX_End_callback_func();
		}
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI7_TXI7
#pragma interrupt Interrupt_SCI7_TXI7(vect=VECT_SCI7_TXI7, fint)
#else
#pragma interrupt Interrupt_SCI7_TXI7(vect=VECT_SCI7_TXI7)
#endif
void Interrupt_SCI7_TXI7(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI7.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(7);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[7].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[7].tx_threshold == 0) && (*sci_global[7].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[7].tx_counter < sci_global[7].tx_threshold)
		)
		{
			/* Send the character */
			SCI7.TDR = *sci_global[7].tx_string_pointer;

			/* Increment the pointer */
			sci_global[7].tx_string_pointer++;

			/* Increment the counter */
			sci_global[7].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[7].tx_threshold == 0) &&
				(*sci_global[7].tx_string_pointer == NULL)) ||
				(sci_global[7].tx_counter == sci_global[7].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI7.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI7.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(7, 0);
					
					/*Enable Transmit End Interrupt */
					SCI7.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(7, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI7.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI7.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI7.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[7].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[7].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[7].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[7].TX_End_callback_func();
		}
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI8_TXI8
#pragma interrupt Interrupt_SCI8_TXI8(vect=VECT_SCI8_TXI8, fint)
#else
#pragma interrupt Interrupt_SCI8_TXI8(vect=VECT_SCI8_TXI8)
#endif
void Interrupt_SCI8_TXI8(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI8.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(8);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[8].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[8].tx_threshold == 0) && (*sci_global[8].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[8].tx_counter < sci_global[8].tx_threshold)
		)
		{
			/* Send the character */
			SCI8.TDR = *sci_global[8].tx_string_pointer;

			/* Increment the pointer */
			sci_global[8].tx_string_pointer++;

			/* Increment the counter */
			sci_global[8].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[8].tx_threshold == 0) &&
				(*sci_global[8].tx_string_pointer == NULL)) ||
				(sci_global[8].tx_counter == sci_global[8].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI8.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI8.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(8, 0);
					
					/*Enable Transmit End Interrupt */
					SCI8.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(8, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI8.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI8.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI8.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[8].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[8].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[8].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[8].TX_End_callback_func();
		}
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN)
#if FAST_INTC_VECTOR == VECT_SCI9_TXI9
#pragma interrupt Interrupt_SCI9_TXI9(vect=VECT_SCI9_TXI9, fint)
#else
#pragma interrupt Interrupt_SCI9_TXI9(vect=VECT_SCI9_TXI9)
#endif
void Interrupt_SCI9_TXI9(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI9.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(9);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[9].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[9].tx_threshold == 0) && (*sci_global[9].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[9].tx_counter < sci_global[9].tx_threshold)
		)
		{
			/* Send the character */
			SCI9.TDR = *sci_global[9].tx_string_pointer;

			/* Increment the pointer */
			sci_global[9].tx_string_pointer++;

			/* Increment the counter */
			sci_global[9].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[9].tx_threshold == 0) &&
				(*sci_global[9].tx_string_pointer == NULL)) ||
				(sci_global[9].tx_counter == sci_global[9].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI9.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI9.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(9, 0);
					
					/*Enable Transmit End Interrupt */
					SCI9.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(9, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI9.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI9.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI9.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[9].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[9].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[9].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[9].TX_End_callback_func();
		}
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI10_TXI10
#pragma interrupt Interrupt_SCI10_TXI10(vect=VECT_SCI10_TXI10, fint)
#else
#pragma interrupt Interrupt_SCI10_TXI10(vect=VECT_SCI10_TXI10)
#endif
void Interrupt_SCI10_TXI10(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI10.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(10);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[10].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[10].tx_threshold == 0) && (*sci_global[10].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[10].tx_counter < sci_global[10].tx_threshold)
		)
		{
			/* Send the character */
			SCI10.TDR = *sci_global[10].tx_string_pointer;

			/* Increment the pointer */
			sci_global[10].tx_string_pointer++;

			/* Increment the counter */
			sci_global[10].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[10].tx_threshold == 0) &&
				(*sci_global[10].tx_string_pointer == NULL)) ||
				(sci_global[10].tx_counter == sci_global[10].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI10.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI10.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(10, 0);
					
					/*Enable Transmit End Interrupt */
					SCI10.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(10, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI10.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI10.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI10.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[10].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[10].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[10].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[10].TX_End_callback_func();
		}
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI11_TXI11
#pragma interrupt Interrupt_SCI11_TXI11(vect=VECT_SCI11_TXI11, fint)
#else
#pragma interrupt Interrupt_SCI11_TXI11(vect=VECT_SCI11_TXI11)
#endif
void Interrupt_SCI11_TXI11(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI11.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(11);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[11].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[11].tx_threshold == 0) && (*sci_global[11].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[11].tx_counter < sci_global[11].tx_threshold)
		)
		{
			/* Send the character */
			SCI11.TDR = *sci_global[11].tx_string_pointer;

			/* Increment the pointer */
			sci_global[11].tx_string_pointer++;

			/* Increment the counter */
			sci_global[11].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[11].tx_threshold == 0) &&
				(*sci_global[11].tx_string_pointer == NULL)) ||
				(sci_global[11].tx_counter == sci_global[11].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI11.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI11.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(11, 0);
					
					/*Enable Transmit End Interrupt */
					SCI11.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(11, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI11.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI11.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI11.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[11].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[11].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[11].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[11].TX_End_callback_func();
		}
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI12_TXI12
#pragma interrupt Interrupt_SCI12_TXI12(vect=VECT_SCI12_TXI12, fint)
#else
#pragma interrupt Interrupt_SCI12_TXI12(vect=VECT_SCI12_TXI12)
#endif
void Interrupt_SCI12_TXI12(void)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if (SCI12.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(12);
		return;
	}
	
	/* Ok to process the string? */
	if (sci_global[12].tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((sci_global[12].tx_threshold == 0) && (*sci_global[12].tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(sci_global[12].tx_counter < sci_global[12].tx_threshold)
		)
		{
			/* Send the character */
			SCI12.TDR = *sci_global[12].tx_string_pointer;

			/* Increment the pointer */
			sci_global[12].tx_string_pointer++;

			/* Increment the counter */
			sci_global[12].tx_counter++;
			
			/*If have just written last byte out (end of string or count reached)*/
			if (((sci_global[12].tx_threshold == 0) &&
				(*sci_global[12].tx_string_pointer == NULL)) ||
				(sci_global[12].tx_counter == sci_global[12].tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if (SCI12.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI12.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(12, 0);
					
					/*Enable Transmit End Interrupt */
					SCI12.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(12, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI12.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI12.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (sci_global[12].TX_End_callback_func != PDL_NO_FUNC)
				{
					sci_global[12].TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */ 
	else
	{
		/* Call the callback function */
		if (sci_global[12].TX_End_callback_func != PDL_NO_FUNC)
		{
			sci_global[12].TX_End_callback_func();
		}
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline: SCIn transmit end interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_SCIn_TEIn(void)
*------------------------------------------------------------------------------
* Function		: 
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Output		: SCR for that channel
*------------------------------------------------------------------------------
* Use function	: rpdl_SCI_TX_End_callback_func()
*------------------------------------------------------------------------------
* Notes			: 
*------------------------------------------------------------------------------
* History		: 2014.04.04
*				: Ver 1.21
*				: CS release
*""FUNC COMMENT END""*********************************************************/
#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI0_TEI0
#pragma interrupt Interrupt_SCI0_TEI0(vect=VECT_SCI0_TEI0, fint)
#else
#pragma interrupt Interrupt_SCI0_TEI0(vect=VECT_SCI0_TEI0)
#endif
void Interrupt_SCI0_TEI0(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[0].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI0.SMR.BIT.CM == 0) && (SCI0.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(0, true);
	}

	/* If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI0.SMR.BIT.CM == 1) &&
	   ((SCI0.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI0.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI0.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}
	
	/* Call the callback function */
	if (sci_global[0].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[0].TX_End_callback_func();
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI1_TEI1
#pragma interrupt Interrupt_SCI1_TEI1(vect=VECT_SCI1_TEI1, fint)
#else
#pragma interrupt Interrupt_SCI1_TEI1(vect=VECT_SCI1_TEI1)
#endif
void Interrupt_SCI1_TEI1(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[1].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI1.SMR.BIT.CM == 0) && (SCI1.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(1, true);
	}
	
	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI1.SMR.BIT.CM == 1) &&
	   ((SCI1.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI1.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI1.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[1].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[1].TX_End_callback_func();
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI2_TEI2
#pragma interrupt Interrupt_SCI2_TEI2(vect=VECT_SCI2_TEI2, fint)
#else
#pragma interrupt Interrupt_SCI2_TEI2(vect=VECT_SCI2_TEI2)
#endif
void Interrupt_SCI2_TEI2(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[2].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI2.SMR.BIT.CM == 0) && (SCI2.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(2, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI2.SMR.BIT.CM == 1) &&
	   ((SCI2.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI2.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI2.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}
	
	/* Call the callback function */
	if (sci_global[2].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[2].TX_End_callback_func();
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN)
#if FAST_INTC_VECTOR == VECT_SCI3_TEI3
#pragma interrupt Interrupt_SCI3_TEI3(vect=VECT_SCI3_TEI3, fint)
#else
#pragma interrupt Interrupt_SCI3_TEI3(vect=VECT_SCI3_TEI3)
#endif
void Interrupt_SCI3_TEI3(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[3].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI3.SMR.BIT.CM == 0) && (SCI3.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(3, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI3.SMR.BIT.CM == 1) &&
	   ((SCI3.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI3.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI3.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[3].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[3].TX_End_callback_func();
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI4_TEI4
#pragma interrupt Interrupt_SCI4_TEI4(vect=VECT_SCI4_TEI4, fint)
#else
#pragma interrupt Interrupt_SCI4_TEI4(vect=VECT_SCI4_TEI4)
#endif
void Interrupt_SCI4_TEI4(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[4].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI4.SMR.BIT.CM == 0) && (SCI4.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(4, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI4.SMR.BIT.CM == 1) &&
	   ((SCI4.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI4.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI4.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[4].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[4].TX_End_callback_func();
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI5_TEI5
#pragma interrupt Interrupt_SCI5_TEI5(vect=VECT_SCI5_TEI5, fint)
#else
#pragma interrupt Interrupt_SCI5_TEI5(vect=VECT_SCI5_TEI5)
#endif
void Interrupt_SCI5_TEI5(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[5].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI5.SMR.BIT.CM == 0) && (SCI5.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(5, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI5.SMR.BIT.CM == 1) &&
	   ((SCI5.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI5.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI5.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[5].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[5].TX_End_callback_func();
	}
}

#if FAST_INTC_VECTOR == VECT_SCI6_TEI6
#pragma interrupt Interrupt_SCI6_TEI6(vect=VECT_SCI6_TEI6, fint)
#else
#pragma interrupt Interrupt_SCI6_TEI6(vect=VECT_SCI6_TEI6)
#endif
void Interrupt_SCI6_TEI6(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[6].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/

	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI6.SMR.BIT.CM == 0) && (SCI6.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(6, true);
	}
	
	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI6.SMR.BIT.CM == 1) &&
	   ((SCI6.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI6.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI6.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[6].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[6].TX_End_callback_func();
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI7_TEI7
#pragma interrupt Interrupt_SCI7_TEI7(vect=VECT_SCI7_TEI7, fint)
#else
#pragma interrupt Interrupt_SCI7_TEI7(vect=VECT_SCI7_TEI7)
#endif
void Interrupt_SCI7_TEI7(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[7].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI7.SMR.BIT.CM == 0) && (SCI7.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(7, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI7.SMR.BIT.CM == 1) &&
	   ((SCI7.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI7.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI7.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}
	
	/* Call the callback function */
	if (sci_global[7].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[7].TX_End_callback_func();
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI8_TEI8
#pragma interrupt Interrupt_SCI8_TEI8(vect=VECT_SCI8_TEI8, fint)
#else
#pragma interrupt Interrupt_SCI8_TEI8(vect=VECT_SCI8_TEI8)
#endif
void Interrupt_SCI8_TEI8(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[8].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI8.SMR.BIT.CM == 0) && (SCI8.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(8, true);
	}
	
	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI8.SMR.BIT.CM == 1) &&
	   ((SCI8.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI8.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI8.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[8].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[8].TX_End_callback_func();
	}
}

#if !defined (DEVICE_PACKAGE_48_PIN)
#if FAST_INTC_VECTOR == VECT_SCI9_TEI9
#pragma interrupt Interrupt_SCI9_TEI9(vect=VECT_SCI9_TEI9, fint)
#else
#pragma interrupt Interrupt_SCI9_TEI9(vect=VECT_SCI9_TEI9)
#endif
void Interrupt_SCI9_TEI9(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[9].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI9.SMR.BIT.CM == 0) && (SCI9.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(9, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI9.SMR.BIT.CM == 1) &&
	   ((SCI9.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI9.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI9.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[9].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[9].TX_End_callback_func();
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI10_TEI10
#pragma interrupt Interrupt_SCI10_TEI10(vect=VECT_SCI10_TEI10, fint)
#else
#pragma interrupt Interrupt_SCI10_TEI10(vect=VECT_SCI10_TEI10)
#endif
void Interrupt_SCI10_TEI10(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[10].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI10.SMR.BIT.CM == 0) && (SCI10.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(10, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI10.SMR.BIT.CM == 1) &&
	   ((SCI10.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI10.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI10.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[10].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[10].TX_End_callback_func();
	}
}
#endif

#if !defined (DEVICE_PACKAGE_48_PIN) && !defined (DEVICE_PACKAGE_64_PIN) && \
	!defined (DEVICE_PACKAGE_100_PIN)
#if FAST_INTC_VECTOR == VECT_SCI11_TEI11
#pragma interrupt Interrupt_SCI11_TEI11(vect=VECT_SCI11_TEI11, fint)
#else
#pragma interrupt Interrupt_SCI11_TEI11(vect=VECT_SCI11_TEI11)
#endif
void Interrupt_SCI11_TEI11(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[11].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI11.SMR.BIT.CM == 0) && (SCI11.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(11, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI11.SMR.BIT.CM == 1) &&
	   ((SCI11.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI11.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI11.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[11].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[11].TX_End_callback_func();
	}
}
#endif

#if FAST_INTC_VECTOR == VECT_SCI12_TEI12
#pragma interrupt Interrupt_SCI12_TEI12(vect=VECT_SCI12_TEI12, fint)
#else
#pragma interrupt Interrupt_SCI12_TEI12(vect=VECT_SCI12_TEI12)
#endif
void Interrupt_SCI12_TEI12(void)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / (double)sci_global[channel].baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / (double)sci_global[12].baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/
	
	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if ( (SCI12.SMR.BIT.CM == 0) && (SCI12.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(12, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if ((SCI12.SMR.BIT.CM == 1) &&
	   ((SCI12.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))
		
	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);	
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (sci_global[12].TX_End_callback_func != PDL_NO_FUNC)
	{
		sci_global[12].TX_End_callback_func();
	}
}

/* End of file */
