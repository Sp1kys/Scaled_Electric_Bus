#define RP2040_ISR_SERVO_USING_MBED     false

#include <Arduino.h>
#include "motors.h"
#include "RP2040_ISR_Servo.h"

//#include <Servo.h>
#define MIN_MICROS  800
#define MAX_MICROS  2450 
#define NUM_SERVOS  2

#define SERVO_PIN_1 2
#define SERVO_PIN_2 3 

typedef struct
{
  int     servoIndex;
  uint8_t servoPin;
} ISR_servo_t;


ISR_servo_t ISR_servo[NUM_SERVOS] =
{
  { -1, SERVO_PIN_1 }, { -1, SERVO_PIN_2 }
};


const char motor = 'M';

void setup() {
  
  for (int index = 0; index < NUM_SERVOS; index++)
  {
    pinMode(ISR_servo[index].servoPin, OUTPUT);
    digitalWrite(ISR_servo[index].servoPin, LOW);
  }

  Serial.begin(115200); //Debug serial
  Serial1.begin(9600, SERIAL_8N1); //Bluetooth module serial 
  init_motors();
  
  for (int index = 0; index < NUM_SERVOS; index++)
  {
    ISR_servo[index].servoIndex = RP2040_ISR_Servos.setupServo(ISR_servo[index].servoPin, MIN_MICROS, MAX_MICROS);

    if (ISR_servo[index].servoIndex != -1)
    {
      Serial.print(F("Setup OK Servo index = ")); Serial.println(ISR_servo[index].servoIndex);

      RP2040_ISR_Servos.setPosition(ISR_servo[index].servoIndex, 90);
    }
    else
    {
      Serial.print(F("Setup Failed Servo index = ")); Serial.println(ISR_servo[index].servoIndex);
    }
  }

}

void loop() {

  if (Serial1.available() > 3) {//waits, until there is 4 characters in the buffer
    byte c = Serial1.read(); //Read first byte (Command) 

    unsigned int PWM_value;
    delay(80);
    switch (c)
    {
      case motor:
        Serial.println("Message from app recieved! Data Recieved:");
        PWM_value=Serial1.parseInt();
        unsigned int pos = Serial1.parseInt();
        Serial.print("\n");
        Serial1.print("OK!");
        
        set_motor_PWM(PWM_value);

        Serial.println("Servo position: " + String(pos));

        for (int index = 0; index < NUM_SERVOS; index++)
        {
          RP2040_ISR_Servos.setPosition(ISR_servo[index].servoIndex, pos);
        }
        break;
        
    }

    //Empty buffer
    while (Serial1.available()){
      Serial1.read();
    }
    
  }
}