#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

/** Includes **/
/**************/

#include "Arduino.h"
#include <PololuA4983.h>

/** Defines **/
/*************/
//calibration state :
#define TODO        0
#define IN_PROGRESS 1
#define DONE        2


/** Class Descritpion **/

class StepperControl
{
	private:
		uint8_t calibrationState = TODO;
		uint8_t calibrationProgressState;
		PololuA4983 *stepper;
		uint8_t endStopPin;
		int maxStep;
		int maxheight;//max heigh with offset
		int standbyHeight;
		bool endStopPos;
		bool motorPos;
		int posMMOffSet;

	public:
		StepperControl(PololuA4983 *stepper, uint8_t endStopPin, int maxStep, int maxheight, int standbyHeight, bool endStopPos, bool motorPos, int posMMOffSet);
		~StepperControl();
		void begin();
		void update();
		void launchCalibration();
		void setPositionStep(int pos);
		void setHeightMM(int height);
		uint8_t getStatus();

};


#endif //STEPPER_CONTROL_H
