

#include "speaker.h"



void SPEAKER_Init(){
    ForceClearPWMFIFO();
    CancelClearPWMFIFO();
   
    PWM1PinAlter();

    //PWM1OutEnable();

   
    PWM1OutPolarLowAct();

   
    SetPWM1Dat(0x10);
}

void SPEAKER_Generate(uint8_t duration){
    PWM1OutEnable();
    SetPWMClk(240);
    DLY_ms(1000);
    DsiablePWM1Out();
}



