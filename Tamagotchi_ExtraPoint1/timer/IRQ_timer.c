/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../joystick/joystick.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h" 
#include "../tamagotchi/tamagotchi.h"
#include <stdio.h>

extern int hours, minutes, seconds;
extern char age[50];
extern int barHappiness_count, barSatiety_count, offset;
extern int eating_flag, left;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

//TIMER 0, MR0 and MR1 manage the animation and the age
void TIMER0_IRQHandler (void)
{
	/* Match register 0 interrupt service routine */
	if (LPC_TIM0->IR & 01)
	{ 
		LCD_AgeAndAnimationFirst();
		
		LPC_TIM0->IR = 1;			/* clear interrupt flag */
	}
		/* Match register 1 interrupt service routine */
	else if(LPC_TIM0->IR & 02)
  {		
		LCD_AgeAndAnimationSecond();
		
		LPC_TIM0->IR =  2 ;			/* clear interrupt flag */	
	}

  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

//TIMER 1 and MR2 manage the flow of life and the decrease of batteries
void TIMER1_IRQHandler (void)
{
	/* Match register 2 interrupt service routine */
  if(LPC_TIM1->IR & 4)
  {
		//happiness battery
		LCD_DrawRectangle(29 + (11 * barHappiness_count), 68, 9, 15, White); //draw a white rectangle 9x15 in the correct position indicated by barHappiness_count
		barHappiness_count --; //update the number of bars
		
		//satiety battery (the same)
		LCD_DrawRectangle(159 + (11 * barSatiety_count), 68, 9, 15, White);
		barSatiety_count --;
		
		if((barHappiness_count == -1) || (barSatiety_count == -1)){
			//runaway sequence
			//TIMER0 and TIMER1 are disabled
	    disable_timer(0);
			disable_timer(1);
			LCD_RunAwaySequence();
		}
		
		LPC_TIM1->IR =  4 ;			/* clear interrupt flag */	
	}
	
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

//TIMER 2 manages the eating sketch which lasts 2 seconds
void TIMER2_IRQHandler (void)
{
     if(LPC_TIM2->IR & 01){
			 
     if(eating_flag == 0){

       //age continues to flow       
			 if((seconds != 0) && ((seconds % 59) == 0)){ minutes ++; seconds = -1;}
		   if((minutes != 0) && ((minutes % 59) == 0)){ hours ++; minutes = -1; }
		   seconds ++;
			 sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	     GUI_Text(60, 10, (uint8_t *)age, Blue, White); //display Lovelitchi's age
			 
	     LCD_DrawSushi(sushi, R4, C, 35, 180); //draw meal on the left of the display
	     //Lovelitchi moves towards meal...
	     LCD_DrawRectangle(100, 150, R, C+20, White);
	     LCD_DrawLovelitchi(lovelitchi, R, C, 85, 150);
			 LCD_DrawRectangle(85, 150, R, C+20, White);
			 LCD_DrawLovelitchi(lovelitchi, R, C, 70, 150);
			 
		 }
		 else if (eating_flag == -1){
			 
			 //age continues to flow       
			 if((seconds != 0) && ((seconds % 59) == 0)){ minutes ++; seconds = -1;}
		   if((minutes != 0) && ((minutes % 59) == 0)){ hours ++; minutes = -1; }
		   seconds ++;
			 sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	     GUI_Text(60, 10, (uint8_t *)age, Blue, White); //display Lovelitchi's age
			 
			 LCD_DrawIceCream(ice_cream, R5, C, 180, 180); //draw snack on the right of the display
			 //Lovelitchi moves towards snack...
			 LCD_DrawRectangle(100, 150, R, C+20, White);
			 LCD_DrawLovelitchi(lovelitchi, R, C, 115, 150);
			 LCD_DrawRectangle(115, 150, R, C+20, White);
			 LCD_DrawLovelitchi(lovelitchi, R, C, 130, 150);
			 
		 }
		
     LPC_TIM2->IR = 1;				/* clear interrupt flag */
		
	}
	
	else if(LPC_TIM2->IR & 02){
		
		if(eating_flag == 0){
		 eating_flag = 1;	
		 disable_timer(2);
			
			//age continues to flow       
			if((seconds != 0) && ((seconds % 59) == 0)){ minutes ++; seconds = -1;}
		  if((minutes != 0) && ((minutes % 59) == 0)){ hours ++; minutes = -1; }
		  seconds ++;
			sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	    GUI_Text(60, 10, (uint8_t *)age, Blue, White); //display Lovelitchi's age
		
		 //...Lovelitchi returns at the display center
		 LCD_DrawRectangle(35, 180, R4+20, C, White);
		 LCD_DrawRectangle(70, 150, R, C+20, White);
		 LCD_DrawLovelitchi(lovelitchi, R, C, 100, 150);
		}
		else if(eating_flag == -1){
			eating_flag = 1;	
			disable_timer(2);
			
			//age continues to flow       
			if((seconds != 0) && ((seconds % 59) == 0)){ minutes ++; seconds = -1;}
		  if((minutes != 0) && ((minutes % 59) == 0)){ hours ++; minutes = -1; }
		  seconds ++;
			sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	    GUI_Text(60, 10, (uint8_t *)age, Blue, White); //display Lovelitchi's age
			
			//...Lovelitchi returns at the display center
			LCD_DrawRectangle(180, 180, R4+20, C, White);
			LCD_DrawRectangle(130, 150, R, C+20, White);
			LCD_DrawLovelitchi(lovelitchi, R, C, 100, 150); 
		}
		
		LPC_TIM2->IR = 2;
		
	}
  return;
}





/******************************************************************************
**                            End Of File
******************************************************************************/
