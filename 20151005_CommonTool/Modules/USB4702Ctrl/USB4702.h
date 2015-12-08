#ifndef USB4702_H
#define USB4702_H
#include "adsapi\driver.h"

//display message diaglog when any error occur
//#define MSG_DIALOG_DISPLAY


//Initial USB Board, should return "SUCCESS"
LRESULT UB_init();

//Close USB Board
void UB_close();

//Get the DO port current value
LRESULT UB_get_DO_value(USHORT * val);

//Set DO value
LRESULT UB_set_DO_value(USHORT val);

//Set specified bit (0~7)
LRESULT UB_set_DO_bit(USHORT bit);

//Clear specified bit (0~7)
LRESULT UB_clear_DO_bit(USHORT bit);

//Get ADC voltage value (channel 0~7)
LRESULT UB_get_ADC_voltage(USHORT channel, float * voltage);

//2010-10-28 Mick:
//Set ADC voltage value (channel 0,1)
LRESULT UB_set_AO_voltage(USHORT channel, float * voltage);

#endif //USB4702_H

