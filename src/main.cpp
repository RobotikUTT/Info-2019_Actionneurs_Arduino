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

//Leds :
#include <Adafruit_NeoPixel.h>

//void setServoPlatform(int val);
void stripLedUpdate();
void ledSetColor(uint8_t r, uint8_t g, uint8_t b);

uint8_t colorTab[9][3] = {
{255,255,000},
{000,255,255},
{255,000,255},
{255,128,000},
{000,255,128},
{128,000,255},
{128,255,000},
{000,128,255},
{255,000,128}
};

Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NB_LED_SEGS, PIN_LED, NEO_GRB + NEO_KHZ800);

// Serial handler for frames coming from serial
SerialHandler serialHandler;

PololuA4983 stepperFront(STEP_PIN_FRONT, DIR_PIN_FRONT, EN_PIN_FRONT, MIN_DELAY_F);
PololuA4983 stepperBack(STEP_PIN_BACK, DIR_PIN_BACK, EN_PIN_BACK, MIN_DELAY_B);

StepperControl controlFront(&stepperFront, END_STOP_PIN_F, MAX_STEP_F, MAX_HEIGHT_F, STANDBY_HEIGHT_F, END_STOP_POS_F, MOTOR_POS_F, POS_MM_OFFSET_F);
StepperControl controlBack(&stepperBack, END_STOP_PIN_B, MAX_STEP_B, MAX_HEIGHT_B, STANDBY_HEIGHT_B, END_STOP_POS_B, MOTOR_POS_B, POS_MM_OFFSET_B);

//Servo servoPlatform0;
//Servo servoPlatform1;
Servo servoPuckTaker;

uint8_t currentAction = 0;//0:action finished

bool lastSWStepperState = LOW;
bool heightGoalSet = false;

uint8_t ledState = 255;
//long t = 0;

void setup() {
  // Serial frames
  //serialHandler.begin(57600);
  Serial.begin(57600);
  controlFront.begin();
  controlBack.begin();
  //controlFront.launchCalibration(); //done by sw stepper test
  //controlBack.launcgetPositionStephCalibration();

  //servoPlatform0.attach(PIN_SERVO_PLATFORM0);
  //servoPlatform1.attach(PIN_SERVO_PLATFORM1);
  servoPuckTaker.attach(PIN_SERVO_PUCK_TAKER);

  pinMode(PIN_SW_STEPPER_STATE, INPUT_PULLUP);
  pinMode(PIN_LED_STEPPER_STATE, OUTPUT);

  //setServoPlatform(40);
  servoPuckTaker.write(SERVO_REPLIED);
  /*delay(3000);
  servoPuckTaker.write(90);
  delay(3000);
  servoPuckTaker.write(180);*/

  ledStrip.begin();
  ledStrip.show(); // Initialize all pixels to 'off'

  //for setup stepper values :
  /*while (controlBack.getStatus() < 2) {
    controlBack.update();
    //Serial.println(stepperFront.getRemainingStep());
  }*/

}

void loop() {
  //For setup stepper values :
  /*if (Serial.available()) {
    int tmp = (int)Serial.parseInt();
    stepperFront.moveStep(abs(tmp), (1+tmp/abs(tmp))/2);
    Serial.println(stepperFront.getPositionStep());
  }*/

  // If a height command is processing
  if (heightGoalSet) {
    // And it's done
    if (stepperBack.getRemainingStep() == 0 && stepperFront.getRemainingStep() == 0) {
      // Send callback
      serialHandler.send(YOUR_DOCK_HAS_FULLFILLED_YOUR_REQUEST);
      heightGoalSet = false;
    }

  }

  // Communication :
  std::vector<int> frame = serialHandler.read();
	if (serialHandler.is(frame, SET_DOCK_HEIGHT)) {
    int16_t front = frame[1];
    int16_t back = frame[2];

    // TODO set height for both sides
    heightGoalSet = true;
    controlFront.setHeightInMM(front);
    controlBack.setHeightInMM(back);
	} else if (serialHandler.is(frame, FETCH_PUCK)) {
    ledSetColor(255,255,0);
    ledStrip.show();
    servoPuckTaker.write(SERVO_DEPLIED);
    delay(2000);
    servoPuckTaker.write(SERVO_REPLIED);
    delay(2000);
    serialHandler.send(PUCK_FETCHED_CHIEF);
  }
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

  if (stepperFront.getRemainingStep() > 0) {
    ledState = 3;
  }
  else if (stepperFront.getRemainingStep() < 0) {
    ledState = 4;
  }
  else{
    ledState = 255;
  }

  //update stepper motors and stepper motor controls :
  controlFront.update();
  controlBack.update();

  stripLedUpdate();

}

/*void setServoPlatform(int val)
{
  servoPlatform0.write(val);
  servoPlatform1.write(180 - val);
}*/

void stripLedUpdate()
{
  if (ledState == 0) {//OFF
    ledSetColor(0,0,0);
  }
  else if (ledState == 1) {//JAUNE
    ledSetColor(255,255,0);
  }
  else if (ledState == 2) {//MAGENTA
    ledSetColor(255,0,255);
  }
  else if (ledState == 3) {//JAUNE chenillard
    ledSetColor(0,0,0);
    for (uint8_t i = (uint8_t)((4*millis()/1000)%NB_LED_SEGS); i < (uint8_t)((4*millis()/1000 + 4)%NB_LED_SEGS); i++) {
      ledStrip.setPixelColor(i, 255, 255, 0);
    }
  }
  else if (ledState == 4) {//MAGENTA chenillard
    ledSetColor(0,0,0);
    for (uint8_t i = (uint8_t)((4*millis()/1000)%NB_LED_SEGS); i < (uint8_t)((4*millis()/1000 + 4)%NB_LED_SEGS); i++) {
      ledStrip.setPixelColor(i, 255, 255, 0);
    }
  }
  else//DEFAUT
  {
    uint8_t tmp = (uint8_t)(4*millis()/1000)%NB_LED_SEGS;
    for (uint8_t i = 0; i < NB_LED_SEGS; i++) {
      uint8_t tmp2 = (tmp+i)%NB_LED_SEGS;
      ledStrip.setPixelColor(i, colorTab[tmp2][0], colorTab[tmp2][1], colorTab[tmp2][2]);
    }
  }

  ledStrip.show();
}

void ledSetColor(uint8_t r, uint8_t g, uint8_t b)
{
  for (size_t i = 0; i < NB_LED_SEGS; i++) {
    ledStrip.setPixelColor(i, r, g, b);
  }
}
