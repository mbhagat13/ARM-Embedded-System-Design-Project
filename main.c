

#include <stdio.h>
#include <stdlib.h>
#include "LPC17xx.H"                       
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "logo1.c"
#include "logo2.c"
#include "logo3.c"
#include "play_icon.c"
#include "Main_Logo.c"
#include "snake_food.c"
#include "snake_skin.c"
#include "Black_End.c"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */
#define delay_shift 15
#define MAX_LENGTH 30



//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

extern int music_init(void);

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

int counter =0,screen_number=0;
char KBD_Direction[12];
int high_score =3;


void delay_func( int d) {
d <<= delay_shift;
while (d--);
}

void menu_Screen_Init(){
//#ifdef __USE_LCD
		GLCD_Clear(White);                         /* Clear graphical LCD display   */
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0, 0, __FI, " COE718 Meher Bhagat ");
		GLCD_SetTextColor(White);
		GLCD_DisplayString(1, 0, __FI, "  Snakes on a Plane    ");

}

int run_Snake(){
	
		int x=50, y=50, x_next = 10 ,y_next = 0;
		int snake_size_tail = 3;
		int food_x =0, food_y=0;
		int i =0, x_prev=0,y_prev=0;
		int flag = 0;
	  int tail_coordinate_x[MAX_LENGTH];
	  int tail_coordinate_y[MAX_LENGTH];
		int speed =70;
	
			int count = 0;
			int theme =0, score = 3;
		while(1){

						if (count==0){	
							GLCD_SetBackColor(Blue);	
							GLCD_DisplayString(5,  0, __FI,  "        Black        ");
							GLCD_SetBackColor(Black);	
							GLCD_DisplayString(6,  0, __FI,  "        White        ");
							
						}else if(count == 1){
							GLCD_SetBackColor(Blue);	
							GLCD_DisplayString(6,  0, __FI,  "        White        ");
							GLCD_SetBackColor(Black);	
							GLCD_DisplayString(5,  0, __FI,  "        Black        ");
						}
						
						
					// Joystick up direction response
						if (get_button() == KBD_UP){
							
							if(count >0){
									count =0;
							}
							// Joystick right direction response
						} else if (get_button() == KBD_DOWN) {	
							if(count < 1){
									count =1;
							}
						} else if (get_button() == KBD_SELECT) {
							theme = count;
							break;
						}

						
					}
					
		//Loading Screen:
		if(theme==0){
			GLCD_Clear(Black);
		}else{
			GLCD_Clear(White);
		}
		GLCD_Bitmap(65,35,200,170,(unsigned char *)MAIN_LOGO_pixel_data);
		GLCD_SetBackColor(Blue);	
		GLCD_SetTextColor(White);
		GLCD_DisplayString(9,  0, __FI,  "       LOAD         ");
		delay_func(30);
		GLCD_DisplayString(9,  0, __FI,  "       LOADING      ");
		delay_func(1000);
		
		if(theme==0){
			GLCD_Clear(Black);
		}else{
			GLCD_Clear(White);
		}
		GLCD_DisplayString(9,  0, __FI,  "        GO!         ");
		//delay_func(120);
		
		//GAME SCREEN 
		if(theme==0){
			GLCD_Clear(Black);
		}else{
			GLCD_Clear(White);
		}
	
		food_x= (rand() % (32))*10;
		food_y= (rand() % (24))*10;

		
		
			while(1){
			
			GLCD_Bitmap(food_x,food_y,10,10,(unsigned char *)snake_food_img); 
	
			if (get_button() == KBD_UP){
				x_next = 0;
				y_next = -10;
				
			}else if(get_button() == KBD_DOWN){
				x_next = 0;
				y_next = 10;
				
			}else if(get_button() == KBD_LEFT){
				x_next = -10;
				y_next=0;

			}else if(get_button() == KBD_RIGHT){
				x_next=10;
				y_next=0;	
			}
		
			x_prev=x;
			y_prev=y;
			
			x=x+x_next;
			y=y+y_next;
			
		//increase snake size, snake speed, and generate food if hit
		if((x == food_x) && (y == food_y)){
			snake_size_tail = snake_size_tail+1;
			food_x = (rand() % (32))*10;
			food_y = (rand() % (24))*10;
			score =score + 1;
			if(speed >50)
				speed=speed-2;
		}
		//Draw head
		GLCD_Bitmap(x,y,10,10,(unsigned char *)snake_skin_img); 
		
		//Draw tail after moving it all
		for (i=snake_size_tail;i>0;i--){
				tail_coordinate_x[i]=tail_coordinate_x[i-1];
				tail_coordinate_y[i]=tail_coordinate_y[i-1];	
		}			

			tail_coordinate_x[0]=x_prev;
			tail_coordinate_y[0]=y_prev;

		
		for (i=0;i<snake_size_tail;i++){
							
			if(i==(snake_size_tail-1)){
				if(theme==0){
					GLCD_Bitmap(tail_coordinate_x[i],tail_coordinate_y[i],10,10,(unsigned char *)BLACK_END_pixel_data); 			
				}else{
					GLCD_Bitmap(tail_coordinate_x[i],tail_coordinate_y[i],10,10,(unsigned char *)PLAY_ICON_pixel_data); 		
				}
			}else{
				GLCD_Bitmap(tail_coordinate_x[i],tail_coordinate_y[i],10,10,(unsigned char *)snake_skin_img); 		
			}
		}	
				//Collision Detection Algorithim
		
		for (i=0;i<snake_size_tail;i++){

			if((tail_coordinate_x[i]==x)&&(tail_coordinate_y[i]==y)){
				flag=1;
			}
			
		}
		
		if(x<0 || x>310){
			break;
			}
		if(y<0 || y>230 || flag==1){
			break;
			}
		
		
		
			delay_func(speed);

	}
		return score;
			
}
	

void init_Snake(){
		int x=50, y=50, x_next = 10 ,y_next = 0;
		int snake_size_tail = 3;
		int food_x =0, food_y=0;
		int i =0, x_prev=0,y_prev=0;
		int flag = 0;
		int s =0, th =0;
		char message[20];
		char message2[20];
	
		int tail_coordinate_x[MAX_LENGTH];
		int tail_coordinate_y[MAX_LENGTH];
		int speed =70;

		
		s =run_Snake();	

		menu_Screen_Init();

		//HIGH SCORE CODE
		GLCD_SetBackColor(Blue);	
		GLCD_SetTextColor(White);
		sprintf(message, "    SCORE:      %d   ", s);
		GLCD_DisplayString(5,  0, __FI,  (unsigned char *)message);
			if(s>high_score){
				high_score =s;
				  GLCD_DisplayString(6,  0, __FI,  "   NEW HIGH SCORE!  ");
			}else{
				sprintf(message2, "HIGH SCORE:      %d   ", high_score);
				GLCD_DisplayString(6,  0, __FI,  (unsigned char *)message2);
			}
			
			delay_func(2500);

	
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {

	int welcome_count =0;
  //LED_Init();                                /* LED Initialization            */
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
						delay_func(30);	
					counter--;
					if(counter<0)
						counter =0;
				}
				// Joystick right direction response
			}  else if (get_button() == KBD_DOWN) {	
				if(counter < 2){
				counter++;			
					if(counter == 1)
						delay_func(30);
				
					if(counter>2)
						counter =2;
				}

			} else if (get_button() == KBD_SELECT) {
					screen_number = counter+1;
					
				menu_Screen_Init();

		}
			
		
		}//END OF MENU SCREEN
		
		
		if(screen_number==1){//Game Started
				init_Snake();
				menu_Screen_Init();
				screen_number=0;
				counter =0;
		}
		
		if(screen_number==2){//Media Player Screen
			
			GLCD_SetBackColor(Blue);
			GLCD_SetTextColor(Black);			
		  GLCD_DisplayString(3, 1, __FI,  "Press->Play");

			if (get_button() == KBD_SELECT) {
			GLCD_Bitmap(50,100,150,145,(unsigned char *)PLAY_ICON_pixel_data); 
				music_init();
			}
			
			
			if (get_button() == KBD_LEFT) {	
					menu_Screen_Init();
					screen_number=0;
			}
		}//END OF MENU SCREEN
	
		
		
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
						delay_func(30);	
					counter--;
					if(counter<0)
						counter =0;
				}
				
				// Joystick down direction response
			}  else if (get_button() == KBD_DOWN) {	
				if(counter < 2){
				counter++;			
					if(counter == 1)
						delay_func(30);
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
		}//END OF PICTURES SCREEN



  }
}
