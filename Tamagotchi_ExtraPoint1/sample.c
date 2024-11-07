/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "tamagotchi/tamagotchi.h"
#include <stdio.h>

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

extern int hours, minutes, seconds;
extern char age[50];
extern int barHappiness_count, barSatiety_count, offset;

int main(void)
 {
	hours = minutes = seconds = 0;
	
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization(); //display initialization
		
	LCD_Clear(White); //clear all
	LCD_DrawLovelitchi(lovelitchi, R, C, 100, 150); //draw Lovelitchi at the display center (Landtiger display 240x320)
	
	sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	GUI_Text(60, 10, (uint8_t *)age, Blue, White); //display Lovelitchi's age

	//happiness charge
	LCD_HappinessFullCharge();
	
	//satiety charge -> implemented as the happiness one
	LCD_SatietyFullCharge();
	
	//meal and snack
	LCD_DrawEmptyRectangle(0, 280, 120, 40, Blue);
	GUI_Text(40, 294, "Meal", Blue, White);
  LCD_DrawEmptyRectangle(120, 280, 120, 40, Blue);
  GUI_Text(160, 294, "Snack", Blue, White);	
	
	//the age and the animation is handled by TIMER 0, refresh rate of 1 sec
	init_timer(0, 0, 0, 1, 0x17D7840); 						/* 1s * 25MHz = 1 * 25*10^6 =  0x17D7840 - only interrupt! */
	init_timer(0, 0, 1, 3, 0x2FAF080);						/* 2s * 25MHz = 2 * 25*10^6 = 0x2FAF080 - both interrupt and reset! */
	
	enable_timer(0); 
	
	//the flow of Lovelitchi's life is handled by TIMER 1, every 5 sec a bar for each battery is cancelled
	init_timer(1, 0, 2, 3, 0x7735940);                     /* 5s * 25MHz = 5 * 25*10^6 = 0x7735940 - both interrupt and reset! */
	
	enable_timer(1);
	
	//the eating sketch is handled by TIMER 2 and is lasts 2 seconds
	init_timer(2, 0, 0, 1, 0x17D7840); //MR = 1s
  init_timer(2, 0, 1, 3, 0x2FAF080); //MR = 2s
	
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
