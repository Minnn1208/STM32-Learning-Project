#ifndef __AD_H
#define __AD_H

extern uint16_t AD_value[4];

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);



#endif
