/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): __USE_LCD   - enable Output on LCD, uncomment #define in code to use
 *  				for demo (NOT for analysis purposes)
 *----------------------------------------------------------------------------
 * Copyright (c) 2008-2011 Keil - An ARM Company.
 * Name: Anita Tino
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC17xx.H"                       
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "logo1.c"
#include "logo2.c"
#include "logo3.c"


#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
/////////////////////////////////////////////////////////

char text[10];
int counter =0,count1=0,screen_number=0;
char KBD_Direction[12];


void delay_func() {
	for (count1 = 0;count1 < 1000000;count1++) {
	
	}
}

void menu_Screen_Init(){
//#ifdef __USE_LCD
		GLCD_Clear(White);                         /* Clear graphical LCD display   */
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0, 0, __FI, " COE718 Meher Bhgat  ");
		GLCD_SetTextColor(White);
		GLCD_DisplayString(1, 0, __FI, "     Interim Demo           ");

//		GLCD_SetBackColor(Black);		
//    GLCD_SetTextColor(Red); 
//    GLCD_DisplayString(5,  0, __FI,  "         Game       ");
//    GLCD_DisplayString(6,  0, __FI,  "     Media Player       ");
//    GLCD_DisplayString(7,  0, __FI,  "       Pictures       ");	
//#endif
	
	
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t ad_avg = 0;
  uint16_t ad_val = 0, ad_val_ = 0xFFFF;

  //LED_Init();                                /* LED Initialization            */
 // ADC_Init();                                /* ADC Initialization            */
	GLCD_Init();                               /* Initialize graphical LCD (if enabled */

	menu_Screen_Init();
  while (1) {                                /* Loop forever                  */

		
		//MENU SCREEN ALONG WITH SCROLL
		if(screen_number==0){
		
			
			if (counter==0){	
				GLCD_SetBackColor(Blue);	
				GLCD_DisplayString(5,  0, __FI,  "         Game       ");
				GLCD_SetBackColor(Black);	
				GLCD_DisplayString(6,  0, __FI,  "     Media Player       ");
				GLCD_DisplayString(7,  0, __FI,  "       Pictures       ");
				
			}else if(counter == 1){
				GLCD_SetBackColor(Blue);	
				GLCD_DisplayString(6,  0, __FI,  "     Media Player       ");
				GLCD_SetBackColor(Black);	
				GLCD_DisplayString(5,  0, __FI,  "         Game       ");
				GLCD_DisplayString(7,  0, __FI,  "       Pictures       ");
				
			}else if(counter == 2){
				GLCD_SetBackColor(Blue);	
				GLCD_DisplayString(7,  0, __FI,  "       Pictures       ");
				
				GLCD_SetBackColor(Black);	
				GLCD_DisplayString(5,  0, __FI,  "         Game       ");
				GLCD_DisplayString(6,  0, __FI,  "     Media Player       ");
			}	
			
			
		// Joystick up direction response
			if (get_button() == KBD_UP){
				
				if(counter >0){
					if(counter == 1)
						delay_func();	
					counter--;
					if(counter<0)
						counter =0;
				}
				// Joystick right direction response
			}  else if (get_button() == KBD_DOWN) {	
				if(counter < 2){
				counter++;			
					if(counter == 1)
						delay_func();
				
					if(counter>2)
						counter =2;
				}

			} else if (get_button() == KBD_SELECT) {
				if(counter==0)
					screen_number=0;//change to 1 to start the game
				else
					screen_number = counter+1;
					
				menu_Screen_Init();

		}
			
		
		
		
		}//END OF MENU SCREEN
		
		if(screen_number==2){//Media Player Screen
			
			GLCD_SetBackColor(Blue);
			GLCD_SetTextColor(Black);			
			GLCD_DisplayString(3, 1, __FI,  "Press->Play");
			
			if (get_button() == KBD_SELECT) {
			//stuff to play music goes here
			}
			
			
			if (get_button() == KBD_LEFT) {	
					menu_Screen_Init();
					screen_number=0;
			}
		}
	
		
		
		if(screen_number==3){//Pictures Screen
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);			
			GLCD_DisplayString(3, 1, __FI,  "Pictures:");
			GLCD_SetTextColor(White);			
			
			if (counter==0){	
				GLCD_SetBackColor(Blue);	
				GLCD_DisplayString(4, 1, __FI,  "#1");
				GLCD_SetBackColor(Black);	
				GLCD_DisplayString(5, 1, __FI,  "#2");
				GLCD_DisplayString(6, 1, __FI,  "#3");
				
			}else if(counter == 1){
				GLCD_SetBackColor(Blue);	
				GLCD_DisplayString(5, 1, __FI,  "#2");
				GLCD_SetBackColor(Black);	
				GLCD_DisplayString(4, 1, __FI,  "#1");
				GLCD_DisplayString(6, 1, __FI,  "#3");
				
			}else if(counter == 2){
				GLCD_SetBackColor(Blue);	
				GLCD_DisplayString(6, 1, __FI,  "#3");
				GLCD_SetBackColor(Black);	
				GLCD_DisplayString(4, 1, __FI,  "#1");
				GLCD_DisplayString(5, 1, __FI,  "#2");
			}	
						
			if (get_button() == KBD_UP){
				
				if(counter >0){
					if(counter == 1)
						delay_func();	
					counter--;
					if(counter<0)
						counter =0;
				}
				
				// Joystick down direction response
			}  else if (get_button() == KBD_DOWN) {	
				if(counter < 2){
				counter++;			
					if(counter == 1)
						delay_func();
					if(counter>2)
						counter = 2;
				}
			} else if (get_button() == KBD_SELECT) {
				if(counter == 0){
									GLCD_Bitmap(75,100,220,124,(unsigned char *)logo1); 
				}else if (counter == 1){
									GLCD_Bitmap(75,100,240,135,(unsigned char *)logo2); 
				}else{
									GLCD_Bitmap(75,100,220,124,(unsigned char *)logo3); 
				}
			
		}							
			if (get_button() == KBD_LEFT) {	
					menu_Screen_Init();
					screen_number=0;
			}
		}
		

		
		

  }
}
