#include "main.h"
#include <stdbool.h>
#include <stdlib.h>
#define PRESSED 0
#define GreenLED GPIO_PIN_5
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Toggle_Nucleo_LED();
bool Nucleo_Button_pressed();
bool Nucleo_Button_pushed_verbose();
bool take_action;
typedef enum {PUSHED,RELEASED,P2R,R2P} button_state_t;
button_state_t states = RELEASED;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  while (1)
  {
    // polling loop, waiting for button press
    // could implement as interrupt as a future implementaion
  	if (Nucleo_Button_pushed_verbose()){
  	Toggle_Nucleo_LED();
  	HAL_Delay(10); // delay for debounce!!
  	} 
  }
  /* USER CODE END 3 */
}
void Toggle_Nucleo_LED()
	{return (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5));}

bool Nucleo_Button_pressed()
	{return (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == PRESSED);}

bool Nucleo_Button_pushed_verbose(){
	static button_state_t states;
	switch(states)
	{
    	case PUSHED: // when button is pressed
    	{
        	if (!Nucleo_Button_pressed()){
            	states = P2R;
            	take_action = false; // boolean value determining whether to toggle LED or not
            	}
        	else{
            	states = PUSHED;
            	take_action = false;
            	}
        	break;
    	}
    	case P2R: // next state, pressed to released
    	{
        	if (!Nucleo_Button_pressed())
        	{
            	states = RELEASED;
            	take_action = false;
        	}
        	else
        	{
            	states = PUSHED;
            	take_action = false;
        	}
        	break;
    	}
    	case RELEASED: // state where button is released
    	{
        	if (Nucleo_Button_pressed())
        	{
            	states = R2P;
            	take_action = false;
        	}
        	else
        	{
            	states = RELEASED;
            	take_action = false;
        	}
        	break;
    	}
   	 
    	case R2P: // state between being released and pressed again
    	{
        	if (Nucleo_Button_pressed())
        	{
            	states = PUSHED;
            	take_action = true;
        	}
        	else
        	{
            	states = RELEASED;
            	take_action = false;
        	}
        	break;
    	}
	}
	return take_action;
}
