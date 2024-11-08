# ARM-Tamagotchi
Project of a Tamagotchi made on an ARM board LandTiger LPC1768. Final score: 30.
# Application Note: ExtraPoint_2 – Camilla Zulli S315159
The sheet contains an explaination of a particular feature implemented in the famous ’90 game: Tamagotchi.
# Gameplay Tutorial
Gamer can cuddle own pet, increasing happiness level of one bar.
Animation description:
Touching the area in which Tamagotchi is drawn, around at the center of the screen, cuddles animation starts. Lovelitchi (Tamagotchi’s name) goes down and closes her eyes, a heart is drawn above her head and the phrase “I love you!” appears on the display.
The animation lasts 2 seconds, then, game continues its flow.
# Feature Implementation
Main libraries used (library from the course material): 

1. 06_sample_TIMER_2

It contains an advanced managment of timers.
The timer which has been chosen for handling cuddles is TIMER 2. It is initialized to count until 2, in sample.c:
“init_timer(2, 0, 0, 1, 0x17D7840); //MR = 1s”;
“init_timer(2, 0, 1, 3, 0x2FAF080); //MR = 2s”.
When user’s touch is relevated by RIT, a control on the position of the touched point starts.
The function “getDisplayPoint(...)” detecs point’s coordinates, display.x and display.y (the 2 fields of the Coordinate struct, defined in TouchPanel.h). If coordinates are within the correct area, the one which includes Lovelitchi, the following functions are called:
- “LCD_DrawHeart(...)”
- “LCD_Cuddles(...)”: it enables TIMER 2 and set “is_cuddling” flag to 1, so cuddles animation can start.
TIMER 2 matches twice, the first one after 1 second. The phrase “I love you!” is written and closed eyes version of Lovelitchi is drawn. Instead, after the second match, the main display is restored.
At the end of TIMER 2 handler, happiness battery is decremented of one bar.

2. 12_sample_GLCD_TP
  
It contains primitive functions for Touchscreen on Landtiger board.
In sample.c, the functions “TP_Init()” and “TouchPanel_Calibrate()” are invoked to inizialize touchscreen.

