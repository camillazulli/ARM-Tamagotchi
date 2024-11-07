/*MY TAMAGOTCHI LIBRARY*/
/*Created on 03/01/2023*/

#include "../GLCD/GLCD.h" 
#include "../GLCD/AsciiLib.h"
#include "../joystick/joystick.h"
#include "../RIT/RIT.h"
#include "tamagotchi.h"
#include "../timer/timer.h"

int hours, minutes, seconds;
char age[50];
int barHappiness_count, barSatiety_count, offset;
volatile int eating_flag, left = 0;

//function which draws Lovelitchi (both open eyes and closed eyes version)
void LCD_DrawLovelitchi(uint16_t matrix[][C], int r, int c, int cord_x, int cord_y){
	int i, j;
	for(i = 0; i < r; i ++){
		for(j = 0; j < c; j ++){
			if(matrix[i][j] == 0)
				LCD_SetPoint(cord_x + j, cord_y + i, White);
      else if(matrix[i][j] == 1)
				     LCD_SetPoint(cord_x + j, cord_y + i, Blue);
			else if(matrix[i][j] == 2)
				     LCD_SetPoint(cord_x + j, cord_y + i, Cyan);
			else if(matrix[i][j] == 3)
				     LCD_SetPoint(cord_x + j, cord_y + i, Pink);
			else if(matrix[i][j] == 4)
				     LCD_SetPoint(cord_x + j, cord_y + i, Magenta);
		}
	}
}

//function which draws Lovelitchi's sushi
void LCD_DrawSushi(uint16_t matrix[][C], int r, int c, int cord_x, int cord_y){
	int i, j;
	for(i = 0; i < r; i ++){
		for(j = 0; j < c; j ++){
			if(matrix[i][j] == 0)
				LCD_SetPoint(cord_x + j, cord_y + i, White);
      else if(matrix[i][j] == 1)
				     LCD_SetPoint(cord_x + j, cord_y + i, Black);
			else if(matrix[i][j] == 2)
				     LCD_SetPoint(cord_x + j, cord_y + i, Orange);
			else if(matrix[i][j] == 3)
				     LCD_SetPoint(cord_x + j, cord_y + i, Red);
		}
	}
}

//function which draws Lovelitchi's ice-cream
void LCD_DrawIceCream(uint16_t matrix[][C], int r, int c, int cord_x, int cord_y){
	int i, j;
	for(i = 0; i < r; i ++){
		for(j = 0; j < c; j ++){
			if(matrix[i][j] == 0)
				LCD_SetPoint(cord_x + j, cord_y + i, White);
      else if(matrix[i][j] == 1)
				     LCD_SetPoint(cord_x + j, cord_y + i, Black);
			else if(matrix[i][j] == 2)
				     LCD_SetPoint(cord_x + j, cord_y + i, Pink);
			else if(matrix[i][j] == 3)
				     LCD_SetPoint(cord_x + j, cord_y + i, Yellow);
		}
	}
}

//functions which draws full batteries (happiness and satiety)
//happiness
void LCD_HappinessFullCharge(void){
	int i;
	barHappiness_count = 4; //this counter goes from 0 to 4 handling 5 bars
	GUI_Text(25, 45, "Happiness", Blue, White);
	LCD_DrawEmptyRectangle(25, 65, 60, 20, Blue); //25x65 rectangle battery...
	LCD_DrawRectangle(85, 70, 5, 10, Blue); //...adding a small rectangle 5x10, giving to the bayyery a more realistic shape
	offset = 0;
	for(i = 0; i < 5; i ++){ //filling of battery with 5 colorful bars 9x15
		if(i == 0) {LCD_DrawRectangle(29 + offset, 68, 9, 15, Red); offset += 11; continue;}
		else if(i == 1){LCD_DrawRectangle(29 + offset, 68, 9, 15, Orange); offset += 11; continue;}
		else if(i == 2){LCD_DrawRectangle(29 + offset, 68, 9, 15, Yellow); offset += 11; continue;}
		else {LCD_DrawRectangle(29 + offset, 68, 9, 15, Green); offset += 11;}
	}
}

//satiety
void LCD_SatietyFullCharge(void){
	int i;
	barSatiety_count = 4;
	GUI_Text(155, 45, "Satiety", Blue, White);
	LCD_DrawEmptyRectangle(155, 65, 60, 20, Blue);
	LCD_DrawRectangle(215, 70, 5, 10, Blue);
	offset = 0;
	for(i = 0; i < 5; i ++){
		if(i == 0) {LCD_DrawRectangle(159 + offset, 68, 9, 15, Red); offset += 11; continue;}
		else if(i == 1){LCD_DrawRectangle(159 + offset, 68, 9, 15, Orange); offset += 11; continue;}
		else if(i == 2){LCD_DrawRectangle(159 + offset, 68, 9, 15, Yellow); offset += 11; continue;}
		else {LCD_DrawRectangle(159 + offset, 68, 9, 15, Green); offset += 11;}
	}
}

//functions which update Lovelitchi's age and create animation
//1.
void LCD_AgeAndAnimationFirst(void){
	
	  if((seconds != 0) && ((seconds % 59) == 0)){ minutes ++; seconds = -1;}
		if((minutes != 0) && ((minutes % 59) == 0)){ hours ++; minutes = -1; }
		seconds ++;
		sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	  GUI_Text(60, 10, (uint8_t *)age, Blue, White); //display Lovelitchi's age
		LCD_DrawRectangle(100, 200, 25, 9, White); //delete eyes...
		LCD_DrawLovelitchi(lovelitchi_closedEyes, R2, C, 100, 150); //...draw closed eyes
}

//2.
void LCD_AgeAndAnimationSecond(void){
	  
    if((seconds != 0) && ((seconds % 59) == 0)){ minutes ++; seconds = -1;}
		if((minutes != 0) && ((minutes % 59) == 0)){ hours ++; minutes = -1; }
		seconds ++;
		sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	  GUI_Text(60, 10, (uint8_t *)age, Blue, White); //display Lovelitchi's age
		LCD_DrawRectangle(100, 200, 25, 9, White); //delete closed eyes...
		LCD_DrawLovelitchi(lovelitchi, R2, C, 100, 150); //...draw open eyes
}

//Run away sequence -> Lovelitchi is dead
void LCD_RunAwaySequence(void){
	
	    int i, off = 50;
	
	    //Lovelitchi leaves the screen
	    LCD_DrawRectangle(100, 150, R, C+20, White);
	    
     for(i = 0; i < 2; i ++){
				LCD_DrawLovelitchi(lovelitchi_left, R, C, 100-(off*(i+1)), 150);
			  LCD_DrawRectangle(100-(off*(i+1)), 150, R, C+20, White);
			}
		 
			left = 1;
	 
			LCD_Clear(White);
			LCD_DrawLovelitchi(lovelitchi_ghost, R3, C, 105, 150);
			seconds = minutes = hours = 0;
			sprintf(age, "Age: %02d:%02d:%02d", hours, minutes, seconds);
	    GUI_Text(60, 10, (uint8_t *)age, Blue, White);
			LCD_DrawEmptyRectangle(25, 65, 60, 20, Blue); //25x65 rectangle battery...
	    LCD_DrawRectangle(85, 70, 5, 10, Blue); //...adding a small rectangle 5x10, giving to the bayyery a more realistic shape
			LCD_DrawEmptyRectangle(155, 65, 60, 20, Blue);
	    LCD_DrawRectangle(215, 70, 5, 10, Blue);
			LCD_DrawEmptyRectangle(0, 280, 240, 40, Blue);
      GUI_Text(100, 294, "RESET", Blue, White);
}

//function which simulates the meal sketch
void LCD_GiveMeal(void){
	        
	        eating_flag = 0;
	        enable_timer(2);
				
          while(eating_flag == 0){
						//"wait for interrupt"
					} 	
					
					//update satiety battery
					if(barSatiety_count == 0){
						LCD_DrawRectangle(159 + 11*(barSatiety_count + 1), 68, 9, 15, Orange);
						barSatiety_count ++;
					}
					else if(barSatiety_count == 1){
						LCD_DrawRectangle(159 + 11*(barSatiety_count + 1), 68, 9, 15, Yellow);
						barSatiety_count ++;
					}
					else if((barSatiety_count == 2)){
						LCD_DrawRectangle(159 + 11*(barSatiety_count + 1), 68, 9, 15, Green);
						barSatiety_count ++;
					}
          else if((barSatiety_count == 3)){
						LCD_DrawRectangle(159 + 11*(barSatiety_count + 1), 68, 9, 15, Green);
						barSatiety_count ++;
					}					
					//if barSatiety_count is already full do not anything!
		
}

//function which simulates the snack sketch
void LCD_GiveSnack(void){
	
	   eating_flag = -1;
	   enable_timer(2);
					
	      while(eating_flag == -1){
						//"wait for interrupt"
					}
					
					//update happiness battery
					if(barHappiness_count == 0){
						LCD_DrawRectangle(29 + 11*(barHappiness_count + 1), 68, 9, 15, Orange);
						barHappiness_count ++;
					}
					else if(barHappiness_count == 1){
						LCD_DrawRectangle(29 + 11*(barHappiness_count + 1), 68, 9, 15, Yellow);
						barHappiness_count ++;
					}
					else if((barHappiness_count == 2)){
						LCD_DrawRectangle(29 + 11*(barHappiness_count + 1), 68, 9, 15, Green);
						barHappiness_count ++;
					}
          else if((barHappiness_count == 3)){
						LCD_DrawRectangle(29 + 11*(barHappiness_count + 1), 68, 9, 15, Green);
						barHappiness_count ++;
					}					
					//if barHappiness_count is already full do not anything!
}

