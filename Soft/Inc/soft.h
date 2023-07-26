#ifndef SOFT_H
#define SOFT_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tim.h"
#include "gpio.h"

/*defines*/

#define BUT_COUNT_MAX 100
#define OScount 20
#define PAUSE_MAX 1200

/********/

/**********/
extern uint32_t i, i2, i3;
extern uint16_t cntOS;
extern uint8_t butElMode, butOs;
extern uint16_t butID, butIDNext, butIDNext2;
extern uint8_t indSost;
extern uint16_t iBut, iBut2;
extern uint8_t butElModeEnabled, butOsEnabled;
extern uint8_t readyForStart, start;
extern uint8_t step;
extern uint16_t pulse1, pulse2;
/**********/

/**********/

/**********/

extern void inInit(void);
extern void inMain(void);
extern void inSysT(void);
/**********/
extern void osInd(void);
extern void joystON(void);
extern void joystOFF(void);
extern void moveUP(void);
extern void moveDOWN(void);
extern void moveSTOP(void);
extern void ledLineON(void);
extern void ledLineOFF(void);
extern void ledElModeON(void);
extern void ledElModeOFF(void);
extern void ledOsON(void);
extern void ledOsOFF(void);
extern void ledReservedON(void);
extern void ledReservedOFF(void);
extern void ledPcbON(void);
extern void ledPcbOFF(void);
extern void moveUPOs(void);
extern void moveDOWNOs(void);
/**********/

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* End of SOFT_H */