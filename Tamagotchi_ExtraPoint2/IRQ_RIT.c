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
#include "../TouchPanel/TouchPanel.h"
#include <stdio.h>
#include "../ADC/adc.h"
#include "../music/music.h"

#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1

extern int hours, minutes, seconds, songnum, sound;
extern char age[50];
extern int barHappiness_count, barSatiety_count, offset;
extern int left;

int length=0;
NOTE *notes = NULL;
NOTE select_note = {d3, time_semicroma};
static int current_note=0;

void playSong(NOTE *song, int *sound, int length){
	static int ticks = 0;
	if(length==0){
		return;
	}
	
	if(*sound==1 && !isNotePlaying()){
		++ticks;
		if(ticks== UPTICKS){
			ticks=0;
			playNote(song[current_note++]);
		}
	}
	
	if(current_note>length) {
		*sound=0;
		songnum=0;
		current_note = 0;
		ticks=0;
		disable_timer(1);
		disable_timer(3);
	}	
}

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
  static int food_flag = -1; //this variable serves to distinguish if SELECT has been pressed for meal or snack
	
	ADC_start_conversion();	//potentiometer
	
	if(songnum==1){
		NOTE cuddle_song[] = {{c6, time_croma},{c6, time_croma}, {d6, time_croma},{e6, time_croma},{d6, time_croma},{c6, time_croma}};
		playSong(cuddle_song, &sound, 6);
	}else if(songnum==2){
		NOTE eat_song[] = {{fdies6, time_semicroma},{gdies6, time_semicroma}, {fdies6, time_semicroma},{gdies6, time_semicroma},{fdies6, time_semicroma},{ddies6, time_semicroma},{fdies6, time_semicroma}, {b6, time_semicroma}};
		playSong(eat_song, &sound, 8);
	}else if(songnum==3){
		NOTE death_song[] = {{fdies6, time_croma},{c6, time_croma}, {a5, time_croma},{d6, time_croma},{c6, time_croma}};
		playSong(death_song, &sound, 5);
	}

	
	//Touch Panel handler
	if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){ //read the point touched
	    //if the point is within a given display
			if((display.y > 150) && (display.x < 150+C)){
				if((display.x > 100) && (display.x < 100+R)){
					if(left == 0){ //if Lovelitchi is alive	
						sound=1;
				    songnum=1;
						
	//				disable_RIT(); //It is not possible disable RIT for disabling food selection during animations because RIT handle the sound
	//			  disable_timer(0);
						
					//start cuddles animation...	
					LCD_DrawHeart(heart, N, N, 115, 120);	
				  LCD_Cuddles();
						
	//				enable_RIT();
	//				enable_timer(0);
						
					}
				}
			}
		
	  else{
		//do nothing if touch returns values out of bounds
	  }
	}
	
	/*joystick select pressed p1.25*/
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){
		
		playNote(select_note); //select sound
		
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
			 
			 LCD_Display_VolumeIcon(volume_icon, V1, V2, 10, 10);
			 
			 //TIMER0 and TIMER1 are enabled again
			 enable_timer(0);
//			 enable_timer(1);
			 
				break;
			}
			
			case 2:
			{
				if(food_flag == 0){					//give Lovelitchi a meal
					  sound = 1;
					  songnum = 2;
					
//					disable_RIT(); //It is not possible disable RIT for disabling food selection during animations because RIT handle the sound
//					disable_timer(0);
					
					enable_timer(2);
          LCD_GiveMeal();
					
					enable_RIT();
	//				enable_timer(0);
				}
				else if(food_flag == 1){ //give Lovelitchi a snack
					
					  sound = 1;
					  songnum = 2;
					
//					disable_RIT(); //It is not possible disable RIT for disabling food selection during animations because RIT handle the sound
//					disable_timer(0);
					
					LCD_GiveSnack();
					
					enable_RIT();
					enable_timer(0);
 		//		enable_timer(1);
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
