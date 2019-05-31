

/** Includes **/
/**************/
#include "StepperControl.h"

/** Constructor **/
/*****************/

StepperControl::StepperControl(PololuA4983 *stepper, uint8_t endStopPin, int maxStep, int maxheight, int standbyHeight, bool endStopPos, bool motorPos, int posMMOffSet)
{
	this->stepper = stepper;
	this->endStopPin = endStopPin;
	this->maxStep = maxStep;
	this->maxheight = maxheight;
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
void StepperControl::begin()
{
	pinMode(endStopPin, INPUT_PULLUP);
}

void StepperControl::update()
{
	if (calibrationState == IN_PROGRESS) {
		if (digitalRead(endStopPin) == LOW) {
			calibrationState = DONE;
			if (motorPos == HIGH) {
				stepper->setPositionStep(maxStep);
			}
			else
			{
				stepper->setPositionStep(0);
			}
			setHeightMM(standbyHeight);
		}
		else if (stepper->getRemainingStep() == 0) {
			if (calibrationProgressState == 0) {
				/* code */
			}
			stepper->moveStep(1, (endStopPos+motorPos)%2);
		}
	}
	stepper->update();//At, int posMMOffSet the end of the StepperControl::update()
}

void StepperControl::launchCalibration()
{
	calibrationState = IN_PROGRESS;
	calibrationProgressState = 0;
}

void StepperControl::setPositionStep(int pos)
{
	stepper->moveStep(pos - stepper->getGoalStep(), motorPos);
}

void StepperControl::setHeightMM(int height)
{
	setPositionStep(map(height, posMMOffSet, maxheight, 0, maxStep));
}

uint8_t StepperControl::getStatus()
{
	return calibrationState;
}
