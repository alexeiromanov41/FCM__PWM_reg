#include "soft.h"

/**********/
uint32_t i, i2, i3;
uint16_t cntOS;
uint8_t butElMode, butOs;
uint16_t butID, butIDNext, butIDNext2;
uint8_t indSost;
uint16_t iBut, iBut2;
uint8_t butElModeEnabled, butOsEnabled;
uint8_t readyForStart, start;
uint8_t step;
uint16_t pulse1, pulse2;
/**********/

/**********/
void inInit(void);
void inMain(void);
void inSysT(void);
/**********/
void osInd(void);
void joystON(void);
void joystOFF(void);
void moveUP(void);
void moveDOWN(void);
void moveSTOP(void);
void ledLineON(void);
void ledLineOFF(void);
void ledElModeON(void);
void ledElModeOFF(void);
void ledOsON(void);
void ledOsOFF(void);
void ledReservedON(void);
void ledReservedOFF(void);
void ledPcbON(void);
void ledPcbOFF(void);
void moveUPOs(void);
void moveDOWNOs(void);
/**********/
//Prototypes

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void inInit(void) {
  ledLineON();
  moveSTOP();
  readyForStart = 0;
  start = 0;
  step = 0;
  joystON();
  ledPcbOFF();
  pulse1 = 0;
  pulse2 = 0;
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse1);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); 
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void inMain(void) {
  if((butElModeEnabled) && (butOsEnabled == 0)  && (start == 0)) {
    ledElModeON();
    if(indSost != 1) {
      joystOFF();
      moveDOWN();
      readyForStart = 0;
    } else {
      moveSTOP();
      joystON();
      readyForStart = 1;
      butElModeEnabled = 0;
    }
  } else if(butElModeEnabled == 0) {
    ledElModeOFF();
  }
  
  if((butOsEnabled) && (butElModeEnabled == 0) && (start == 0)) {
    if(readyForStart) {
      ledOsON();
      joystOFF();
      start = 1;
      readyForStart = 0;
    } else {
        butOsEnabled = 0;
        ledOsOFF();
        start = 0;
    }
  } else if((butOsEnabled == 0) && (butElModeEnabled == 0)) {
      ledOsOFF();
      start = 0;
      joystON();
    }
  
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse1);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse2);
}  
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void inSysT(void) {
	butElMode = HAL_GPIO_ReadPin(but1_GPIO_Port, but1_Pin);
  butOs = HAL_GPIO_ReadPin(but2_GPIO_Port, but2_Pin);
	
	butID = (butElMode << 2) + (butOs << 1);
	
	if(butID != 6) {
		iBut ++;
		if(iBut >= BUT_COUNT_MAX) {
			iBut = BUT_COUNT_MAX;
			butIDNext = butID;
		}		
	} else if((iBut == BUT_COUNT_MAX) && (butID == 6)) {
			iBut2 ++;
			if(iBut2 >= BUT_COUNT_MAX) {
				iBut2 = BUT_COUNT_MAX;
			}

			if((iBut == BUT_COUNT_MAX) && (iBut2 == BUT_COUNT_MAX)) {
				
				switch (butIDNext)
					{
						case 2: // ElMode
              if(butOsEnabled == 0) {
                butElModeEnabled = ~butElModeEnabled;
              } else {
                butElModeEnabled = 0;
              }
							break;
						case 4: // OS
							butOsEnabled = ~butOsEnabled;
              butElModeEnabled = 0;
              if(butOsEnabled == 0) {
                moveSTOP();
              }
							break;
						default:
							iBut = 0;
							iBut2 = 0;
							butIDNext = 6;
							break;
					}				
				
				iBut = 0;
				iBut2 = 0;
				butIDNext = 6;
			}
			
	} else {
		iBut = 0;
		iBut2 = 0;
		butIDNext = 2046;
	}
	
	indSost = HAL_GPIO_ReadPin(inductionSensor_GPIO_Port, inductionSensor_Pin);
	
	cntOS ++;
	
	if(cntOS >= OScount) {
		cntOS = 0;
		osInd();	
	}

}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void osInd(void) {	
  if(start) {
    /********************/
		switch (step)
    {
    	case 0:
				if(indSost == 0) {
          moveDOWNOs();
				} else {
            moveSTOP();
            moveUPOs();
						step = 1;	
				}	
    		break;
		/********************/	
    	case 1:
				if(indSost == 1) {
          moveUPOs();
				} else {
            moveSTOP();
            moveDOWNOs();  
						step = 0;	
				}
    		break;
    }
		/********************/
  } else {    
    step = 0;
  }
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void joystON(void) {
  HAL_GPIO_WritePin(rele1_GPIO_Port, rele1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(rele2_GPIO_Port, rele2_Pin, GPIO_PIN_SET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void joystOFF(void) {
  HAL_GPIO_WritePin(rele1_GPIO_Port, rele1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(rele2_GPIO_Port, rele2_Pin, GPIO_PIN_RESET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void moveUP(void) { 
  pulse1 = 0;
  pulse2 = 200;
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void moveDOWN(void) {
  pulse2 = 0;
  pulse1 = 200;  
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void moveSTOP(void) {
  pulse2 = 0;
  pulse1 = 0;
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledLineON(void) {
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledLineOFF(void) {
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledElModeON(void) {
  HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledElModeOFF(void) {
  HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledOsON(void) {
  HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledOsOFF(void) {
  HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledReservedON(void) {
  HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledReservedOFF(void) {
  HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledPcbON(void) {
  HAL_GPIO_WritePin(ledPcb_GPIO_Port, ledPcb_Pin, GPIO_PIN_RESET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void ledPcbOFF(void) {
 HAL_GPIO_WritePin(ledPcb_GPIO_Port, ledPcb_Pin, GPIO_PIN_SET);
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void moveUPOs(void) { 
  pulse1 = 0;
  pulse2 = 50;
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void moveDOWNOs(void) {
  pulse2 = 0;
  pulse1 = 50; 
}
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/