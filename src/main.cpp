#include <Arduino.h>

//includes :
#include "pins.h"
#include "parameters.h"

//CAN :
#include <arduino/serial.hpp>

//Servos :
#include <Servo.h>

#include <StepperControl.h>
//Steppers :
#include <PololuA4983.h>

void setServoPlatform(int val);

// Serial handler for frames coming from serial
SerialHandler serialHandler;

PololuA4983 stepperFront(STEP_PIN_FRONT, DIR_PIN_FRONT, EN_PIN_FRONT, MIN_DELAY_F);
PololuA4983 stepperBack(STEP_PIN_BACK, DIR_PIN_BACK, EN_PIN_BACK, MIN_DELAY_B);

StepperControl controlFront(&stepperFront, END_STOP_PIN_F, MAX_STEP_F, MAX_HEIGHT_F, STANDBY_HEIGHT_F, END_STOP_POS_F, MOTOR_POS_F, POS_MM_OFFSET_F);
StepperControl controlBack(&stepperBack, END_STOP_PIN_B, MAX_STEP_B, MAX_HEIGHT_B, STANDBY_HEIGHT_B, END_STOP_POS_B, MOTOR_POS_B, POS_MM_OFFSET_B);

Servo servoPlatform0;
Servo servoPlatform1;
Servo servoPuckTaker;

uint8_t currentAction = 0;//0:action finished

bool lastSWStepperState = LOW;

//long t = 0;

void setup() {
  // Serial frames
  //serialHandler.begin(57600);
  Serial.begin(57600);
  controlFront.begin();
  controlBack.begin();
  //controlFront.launchCalibration(); //done by sw stepper test
  //controlBack.launchCalibration();

  servoPlatform0.attach(PIN_SERVO_PLATFORM0);
  servoPlatform1.attach(PIN_SERVO_PLATFORM1);
  servoPuckTaker.attach(PIN_SERVO_PUCK_TAKER);

  pinMode(PIN_SW_STEPPER_STATE, INPUT_PULLUP);
  pinMode(PIN_LED_STEPPER_STATE, OUTPUT);

  setServoPlatform(100);
  servoPuckTaker.write(180);
  /*delay(3000);
  servoPuckTaker.write(90);
  delay(3000);
  servoPuckTaker.write(180);*/

  //for setup stepper values :
  /*while (controlBack.getStatus() < 2) {
    controlBack.update();
    //Serial.println(stepperFront.getRemainingStep());
  }*/

}

void loop() {
  //For setup stepper values :
  if (Serial.available()) {
    int tmp = (int)Serial.parseInt();
    stepperBack.moveStep(abs(tmp), (1+tmp/abs(tmp))/2);
    Serial.println(stepperBack.getPositionStep());
  }

  //Communication :
  /*  std::vector<int> frame = serialHandler.read();
	if (serialHandler.is(frame, SET_DOCK_HEIGHT)) {
    int front = frame[1];
    int back = frame[2];

    // TODO set height for both sides

    serialHandler.send(YOUR_DOCK_HAS_FULLFILLED_YOUR_REQUEST);
	}*/
  /*if(millis() - t > 100)
  {
    Serial.println(stepperFront.getRemainingStep());
    t = millis();
  }*/

  //motor switch off :
  if (digitalRead(PIN_SW_STEPPER_STATE) == HIGH) {
    digitalWrite(PIN_LED_STEPPER_STATE, HIGH);
    stepperFront.enable();
    stepperBack.enable();
    if (lastSWStepperState == LOW) {
      controlFront.launchCalibration();
      controlBack.launchCalibration();
    }
    lastSWStepperState = HIGH;
  }
  else
  {
    digitalWrite(PIN_LED_STEPPER_STATE, LOW);
    stepperFront.disable();
    stepperBack.disable();
    lastSWStepperState = LOW;
  }

  //update stepper motors and stepper motor controls :
  controlFront.update();
  controlBack.update();

}

void setServoPlatform(int val)
{
  servoPlatform0.write(val);
  servoPlatform1.write(180 - val);
}
