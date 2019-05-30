

/** Includes **/
/**************/
#include "PololuA4983.h"

/** Constructor **/
/*****************/

StepperControl::StepperControl(PololuA4983 *stepper, int maxStep, int standbyHeight, bool endStopPos, bool motorPos, int posMMOffSet)
{
	this->stepper = stepper;
	this->maxStep = maxStep;
	this->standbyHeight = standbyHeight;
	this->endStopPos = endStopPos;
	this->motorPos = motorPos;
	this->posMMOffSet = posMMOffSet;
}

/*** Destructor **/
/*****************/
StepperControl::~StepperControl()
{

}

/** Public Methods **/
/********************/

void StepperControl::update()
{
	if (calibrationState == IN_PROGRESS) {
		if (digitalRead(pin_end_stop) == LOW) {
			calibrationState = DONE;
			setHeightMM(standbHeight);
		}
		else if (stepper->getRemainingStep() == 0) {
			stepper->moveStep(1, endStopPos);
		}
	}
	stepper->update();//At, int posMMOffSet the end of the StepperControl::update()
}

void StepperControl::launchCalibration()
{
	calibrationState = IN_PROGRESS;
}

void StepperControl::setPositionStep(int pos)
{
	stepper->moveStep(pos - stepper->getGoalStep(), 1);
}

void StepperControl::setHeightMM(int height)
{
	setPositionStep(, bool endStopPos)
}
