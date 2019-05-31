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

PololuA4983 stepperFront(STEP_PIN_FRONT, DIR_PIN_FRONT, EN_PIN_FRONT, MIN_DELAY);
PololuA4983 stepperBack(STEP_PIN_BACK, DIR_PIN_BACK, EN_PIN_BACK, MIN_DELAY);

StepperControl controlFront(&stepperFront, END_STOP_PIN_F, MAX_STEP_F, MAX_HEIGHT_F, MAX_HEIGHT_F/*STANDBY_HEIGHT_F*/, END_STOP_POS_F, MOTOR_POS_F, POS_MM_OFFSET_F);

Servo servoPlatform0;
Servo servoPlatform1;
Servo servoPuckTaker;

void setup() {
  // Serial frames
  //serialHandler.begin(57600);
  Serial.begin(57600);
  controlFront.begin();
  //controlBack.begin();
  controlFront.launchCalibration();
  //controlBack.launchCalibration();

  servoPlatform0.attach(PIN_SERVO_PLATFORM0);
  servoPlatform1.attach(PIN_SERVO_PLATFORM1);
  servoPuckTaker.attach(PIN_SERVO_PUCK_TAKER);

  setServoPlatform(90);
  servoPuckTaker.write(70);
  delay(3000);
  servoPuckTaker.write(90);
  delay(3000);
  servoPuckTaker.write(180);

  //for setup stepper values :
  while (controlFront.getStatus() < 2) {
    controlFront.update();
    //Serial.println(digitalRead(END_STOP_PIN_F));
  }

}

void loop() {
  //For setup stepper values :
  if (Serial.available()) {
    int tmp = (int)Serial.parseInt();
    stepperFront.moveStep(abs(tmp), (1+tmp/abs(tmp))/2);
    Serial.println(stepperFront.getPositionStep());
  }

  //Communication :
  /*  std::vector<int> frame = serialHandler.read();
	if (serialHandler.is(frame, SET_DOCK_HEIGHT)) {
    int front = frame[1];
    int back = frame[2];

    // TODO set height for both sides

    serialHandler.send(YOUR_DOCK_HAS_FULLFILLED_YOUR_REQUEST);
	}*/

  //update stepper motors and stepper motor controls :
  controlFront.update();
  //controlBack.update();

}

void setServoPlatform(int val)
{
  servoPlatform0.write(val);
  servoPlatform1.write(180 - val);
}
