#include "lpc17xx.h"
#include "adc.h"
#include "../GLCD/GLCD.h" 
#include "../GLCD/AsciiLib.h"

int volume_level;

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_init (void) {

  LPC_PINCON->PINSEL3 |=  (3UL<<30);      /* P1.31 is AD0.5                     */

  LPC_SC->PCONP       |=  (1<<12);      /* Enable power to ADC block          */

  LPC_ADC->ADCR        =  (1<< 5) |     /* select AD0.5 pin                   */
                          (4<< 8) |     /* ADC clock is 25MHz/5               */
                          (1<<21);      /* enable ADC                         */ 

  LPC_ADC->ADINTEN     =  (1<< 8);      /* global enable interrupt            */

  NVIC_EnableIRQ(ADC_IRQn);             /* enable ADC Interrupt               */
	NVIC_SetPriority(ADC_IRQn,1);
}

void ADC_start_conversion (void) {
	LPC_ADC->ADCR |=  (1<<24);            /* Start A/D Conversion 				*/
}

/*function that draws the volume icon*/
void LCD_Display_VolumeIcon(uint16_t matrix[][V2], int r, int c, int cord_x, int cord_y){
	
	int i, j;
	for(i = 0; i < r; i ++){
		for(j = 0; j < c; j ++){
			if(matrix[i][j] == 0)
				LCD_SetPoint(cord_x + j, cord_y + i, White);
      else if(matrix[i][j] == 1)
				     LCD_SetPoint(cord_x + j, cord_y + i, Blue);
		}
	}
}

/*function that draws volume level*/
void LCD_VolumeLevel(int level){
	int off = 1, i;
	
		for(i = 0; i < 4; i ++){
		LCD_DrawLine(10+V1+off, 10, 10+V1+off, 14+V2, White);
		off +=3;
	}
		
	off = 1;
	
		for(i = 0; i < level; i ++){
		LCD_DrawLine(10+V1+off, 10, 10+V1+off, 13+V2, Blue);
		off +=3;
	}
}

