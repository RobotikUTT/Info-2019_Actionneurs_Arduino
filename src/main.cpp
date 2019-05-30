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

// Serial handler for frames coming from serial
SerialHandler serialHandler;

PololuA4983 stepperFront(STEP_PIN_FRONT, DIR_PIN_FRONT, EN_PIN_FRONT, MIN_DELAY);
PololuA4983 stepperBack(STEP_PIN_BACK, DIR_PIN_BACK, EN_PIN_BACK, MIN_DELAY);

void setup() {
  // Serial frames
  serialHandler.begin(57600);
}

void loop() {

  std::vector<int> frame = serialHandler.read();
	if (serialHandler.is(frame, UPDATE_SCREEN)) {
    // SOME CODE HERE
	}

}
