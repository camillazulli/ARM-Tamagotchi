#include <string.h>
#include <stdint.h>
#include "../GLCD/GLCD.h" 
#include "../GLCD/AsciiLib.h"

#define V1 16
#define V2 11

static uint16_t volume_icon[V1][V2]={
	                  0,0,0,0,0,0,0,0,0,1,1, 
                    0,0,0,0,0,0,0,0,1,1,1, 
                    0,0,0,0,0,0,0,1,1,1,1, 
                    0,0,0,0,0,0,1,1,1,1,1, 
                    0,0,0,0,0,1,1,1,1,1,1, 
                    1,1,1,1,1,1,1,1,1,1,1, 
                    1,1,1,1,1,1,1,1,1,1,1, 
                    1,1,1,1,1,1,1,1,1,1,1, 
                    1,1,1,1,1,1,1,1,1,1,1, 
                    1,1,1,1,1,1,1,1,1,1,1, 
                    1,1,1,1,1,1,1,1,1,1,1, 
                    0,0,0,0,0,1,1,1,1,1,1, 
                    0,0,0,0,0,0,1,1,1,1,1, 
                    0,0,0,0,0,0,0,1,1,1,1, 
                    0,0,0,0,0,0,0,0,1,1,1, 
                    0,0,0,0,0,0,0,0,0,1,1};

void LCD_Display_VolumeIcon(uint16_t matrix[][V2], int r, int c, int cord_x, int cord_y);
void LCD_VolumeLevel(int level);
float return_volume(void);

/* lib_adc.c */
void ADC_init (void);
void ADC_start_conversion (void);										

/* IRQ_adc.c */
void ADC_IRQHandler(void);
