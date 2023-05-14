#include "motors.h"

int motor_pwm = 0;
RP2040_PWM* Motor_PWM[2];

void init_motors(){
    pinMode(NFET_1, OUTPUT);
    pinMode(NFET_2, OUTPUT);
    pinMode(PFET_1, OUTPUT);
    pinMode(PFET_2, OUTPUT);


    digitalWrite(NFET_1, HIGH);
    digitalWrite(NFET_2, HIGH);
    digitalWrite(PFET_1, LOW);
    digitalWrite(PFET_2, LOW);


    Motor_PWM[0] = new RP2040_PWM(PFET_1, freq, 0);
    Motor_PWM[1] = new RP2040_PWM(PFET_2, freq, 0);
}

/*
void set_motor_PWM_smooth(unsigned int pwm, int step){
    int temp_pwm = motor_pwm;
    if (pwm > 511) pwm = 511;

    if (pwm > 255){
        if (motor_pwm > 255){
            digitalWrite(NFET_2, LOW);
            int temp_pwm = motor_pwm-256;
            if (motor_pwm > pwm) {
                while (temp_pwm > pwm-256){
                    temp_pwm -= step;
                    temp_pwm = constrain(temp_pwm,0,255);
                    Serial.print("PWM: ");
                    Serial.println(temp_pwm);
                    ledcWrite(pfet_1_channel, temp_pwm);
                    delay(10);
                }
            }
            else {
                while (temp_pwm < pwm-256){
                    temp_pwm += step;
                    temp_pwm = constrain(temp_pwm,0,255);
                    Serial.print("PWM: ");
                    Serial.println(temp_pwm);
                    ledcWrite(pfet_1_channel, temp_pwm);
                    delay(10);
                }
            }
            ledcWrite(pfet_1_channel, pwm-256);
        }
        else {
            while (temp_pwm > 0){
                temp_pwm -= step;
                temp_pwm = constrain(temp_pwm,0,255);
                Serial.print("PWM: ");
                Serial.println(temp_pwm);
                ledcWrite(pfet_2_channel, temp_pwm);
                delay(10);
            }

            digitalWrite(NFET_1, HIGH);
            ledcWrite(pfet_2_channel, 0);
            delay(400);
            digitalWrite(NFET_2, LOW);  
            //digitalWrite(PFET_2, HIGH);

            while (temp_pwm < (pwm-256)){
                temp_pwm += step;
                temp_pwm = constrain(temp_pwm,0,255);
                Serial.print("PWM: ");
                Serial.println(temp_pwm);
                ledcWrite(pfet_1_channel, temp_pwm);
                delay(10);
            }
            ledcWrite(pfet_1_channel, pwm-256);
        }
    }
    else {
        
        if (motor_pwm < 256){
            digitalWrite(NFET_1, LOW);
            if (motor_pwm > pwm) {
                while (temp_pwm > pwm){
                    temp_pwm -= step;
                    temp_pwm = constrain(temp_pwm,0,255);
                    Serial.print("PWM: ");
                    Serial.println(temp_pwm);
                    ledcWrite(pfet_2_channel, temp_pwm);
                    delay(10);
                }
            }
            else {
                while (temp_pwm < pwm){
                    temp_pwm += step;
                    temp_pwm = constrain(temp_pwm,0,255);
                    Serial.print("PWM: ");
                    Serial.println(temp_pwm);
                    ledcWrite(pfet_2_channel, temp_pwm);
                    delay(10);
                }
            }
            ledcWrite(pfet_2_channel, pwm);;
        }

        else {
            int temp_pwm = motor_pwm-256;
            while (temp_pwm > 0){
                temp_pwm -= step;
                temp_pwm = constrain(temp_pwm,0,255);
                Serial.print("PWM: ");
                Serial.println(temp_pwm);
                ledcWrite(pfet_1_channel, temp_pwm);
                delay(10);
            }
            digitalWrite(NFET_2, HIGH);
            ledcWrite(pfet_1_channel, 0);
            delay(400);

            digitalWrite(NFET_1, LOW);  
            //digitalWrite(PFET_2, HIGH);

            while (temp_pwm < pwm){
                temp_pwm += step;
                temp_pwm = constrain(temp_pwm,0,255);
                Serial.print("PWM: ");
                Serial.println(temp_pwm);
                ledcWrite(pfet_2_channel, temp_pwm);
                delay(10);
            }

            ledcWrite(pfet_2_channel, pwm);
        }
    }
    motor_pwm = pwm;
    Serial.println("Motor PWM: "+ String(motor_pwm));
}
*/

void set_motor_PWM(unsigned int pwm){
    pwm = constrain(pwm,0,511);


    //Move backwards range
    if (pwm > 255){
        //If already moving backwards
        if (motor_pwm > 255){
            digitalWrite(NFET_2, LOW);
            Serial.print("PWM: ");
            Serial.println(pwm-256);
            Motor_PWM[0]->setPWM(PFET_1, freq, (float)(pwm-256)/255*100);

        }
        //If moving forwards
        else {
            digitalWrite(NFET_1, HIGH);
            Motor_PWM[1]->setPWM(PFET_2, freq, 0);
            delay(20);
            digitalWrite(NFET_2, LOW);  
            //digitalWrite(PFET_2, HIGH);

            Serial.print("PWM: ");
            Serial.println(pwm-256);
            Motor_PWM[0]->setPWM(PFET_1, freq, (float)(pwm-256)/255*100);
        }
    }
    //Move forwards range
    else {
        //If already moving forwards
        if (motor_pwm < 256){
            digitalWrite(NFET_1, LOW);
            Serial.print("PWM: ");
            Serial.println(pwm);
            Motor_PWM[1]->setPWM(PFET_2, freq, (float)(pwm)/255*100);
        }
        //if moving backwards
        else {
            digitalWrite(NFET_2, HIGH);
            Motor_PWM[0]->setPWM(PFET_1, freq, 0);
            //ledcWrite(pfet_1_channel, 0);
            delay(20);
            digitalWrite(NFET_1, LOW);  
            //digitalWrite(PFET_2, HIGH);
            
            Serial.print("PWM: ");
            Serial.println(pwm);
            Motor_PWM[1]->setPWM(PFET_2, freq, (float)(pwm)/255*100);
        }
    }
    motor_pwm = pwm;
    Serial.println("Motor PWM: "+ String(motor_pwm));
}