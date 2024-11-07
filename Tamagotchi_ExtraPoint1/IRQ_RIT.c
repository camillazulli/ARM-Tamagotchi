/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"
#include "../tamagotchi/tamagotchi.h"
#include <stdio.h>

extern int hours, minutes, seconds;
extern char age[50];
extern int barHappiness_count, barSatiety_count, offset;
extern int left;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{						
	
	
	static int select = 0;
	static int J_left = 0;
  static int J_right = 0;
  static int food_flag; //this variable serves to distinguish if SELECT has been pressed for meal or snack
	
	/*joystick select pressed p1.25*/
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){
		if((barHappiness_count == -1) || (barSatiety_count == -1)) //SELECT has been pressed for a new game restart
			select ++;
		else //SELECT has been pressed to feed Lovelitchi
			select += 2;
		
		switch(select){
			case 1:
			{ 
				LCD_DrawEmptyRectangle(0, 280, 240, 40, Red);
				GUI_Text(100, 294, "RESET", Red, White); //RESET button is colored by red...
				//...and initial conditions are restored
				
				LCD_Clear(White); //clear all
	      LCD_DrawLovelitchi(lovelitchi, R, C, 100, 150);
	
	      sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	      GUI_Text(60, 10, (uint8_t *)age, Blue, White); //display Lovelitchi's age

				left = 0;
				
	      //happiness charge
	      LCD_HappinessFullCharge();
	
	     //satiety charge -> implemented as the happiness one
	     LCD_SatietyFullCharge();
	
	     //meal and snack
	     LCD_DrawEmptyRectangle(0, 280, 120, 40, Blue);
	     GUI_Text(40, 294, "Meal", Blue, White);
       LCD_DrawEmptyRectangle(120, 280, 120, 40, Blue);
       GUI_Text(160, 294, "Snack", Blue, White);
			 
			 //TIMER0 and TIMER1 are enabled again
			 enable_timer(0);
			 enable_timer(1);
			 
				break;
			}
			
			case 2:
			{
				if(food_flag == 0){ //give Lovelitchi a meal
					
					disable_RIT(); //disable food selection during eating
					disable_timer(0);
          disable_timer(1);
					reset_timer(1);
					
          LCD_GiveMeal();
					
					enable_RIT();
					enable_timer(0);
					enable_timer(1);
				}
				else if(food_flag == 1){ //give Lovelitchi a snack
					
					disable_RIT(); //disable food selection during eating
					disable_timer(0);
          disable_timer(1);
					reset_timer(1);
					
					LCD_GiveSnack();
					
					enable_RIT();
					enable_timer(0);
					enable_timer(1);
				}
				break;
			}
			
			default:
				break;
		}
	}
	else{
		select = 0;
	}
	
	/*Joystick Left*/
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		if(left == 0){
		
		J_left ++;
		switch(J_left){
			case 1:
			 food_flag = 0; //meal has been chosen
			//meal botton is colored by red 
			 LCD_DrawEmptyRectangle(120, 280, 120, 40, Blue);
       GUI_Text(160, 294, "Snack", Blue, White);
			 LCD_DrawEmptyRectangle(0, 280, 120, 40, Red);
	     GUI_Text(40, 294, "Meal", Red, White);
			 break;
			default:
				break;
		}
	}
	}
	else{
		J_left = 0;
	}
	
	/*Joystick Right*/
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		if(left == 0){
		
		J_right ++;
		switch(J_right){
			case 1:
				food_flag = 1; //snack has been chosen
			  //snack botton is colored by red
				LCD_DrawEmptyRectangle(0, 280, 120, 40, Blue);
	      GUI_Text(40, 294, "Meal", Blue, White);
				LCD_DrawEmptyRectangle(120, 280, 120, 40, Red);
        GUI_Text(160, 294, "Snack", Red, White);
			  break;
			default:
				break;
		}
	}
	}
	else{
		J_right = 0;
	}
	
	LPC_RIT->RICTRL |= 0x1; /*clear interrupt flag*/
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
