#include <Arduino.h>

//includes :
#include "pins.h"
#include "parameters.h"

//CAN :
#include <arduino/serial.hpp>

// Serial handler for frames coming from serial
SerialHandler serialHandler;

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
