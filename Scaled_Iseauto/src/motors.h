#include <Arduino.h>
#include "RP2040_PWM.h"

//H-bridge control pins
#define NFET_1 18
#define NFET_2 21
#define PFET_1 19
#define PFET_2 20


const int freq = 500; //PWM switching frequency

void init_motors();
void set_motor_PWM(unsigned int pwm);
//void set_motor_PWM_smooth(unsigned int pwm, int step);
