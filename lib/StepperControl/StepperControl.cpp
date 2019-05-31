

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
	if (calibrationState == IN_PROGRESS) {//si calibration en cours
		if ((digitalRead(endStopPin) == LOW) && (calibrationProgressState != 1)) {//si fin de course enclenche
			calibrationProgressState++;//on passe a l'etape suivante
			if (calibrationProgressState == 1) {
				stepper->moveStep(STEP_NB_FOR_SECOND_PART_CALIBRATION, 1 - (endStopPos+motorPos)%2);
			}
			else if (calibrationProgressState == 3) {//si on a touche pour la deuxieme fois le fin de course
				calibrationState = DONE;//on a fini la calibration
				if (motorPos == HIGH) {
					stepper->setPositionStep(maxStep);
				}
				else
				{
					stepper->setPositionStep(0);
				}
				setHeightInMM(standbyHeight);
			}
		}
		else if (stepper->getRemainingStep() == 0) {
			if (calibrationProgressState == 1) {
				calibrationProgressState = 2;
			}
			else
			{
				stepper->moveStep(1, (endStopPos+motorPos)%2);
			}
		}
	}
	stepper->update();//At, int posMMOffSet the end of the StepperControl::update()
}

void StepperControl::launchCalibration()
{
	calibrationState = IN_PROGRESS;
	calibrationProgressState = 0;
}

void StepperControl::setPositionInStep(int pos)
{
	stepper->moveStep(pos - stepper->getGoalStep(), 0);
}

void StepperControl::setHeightInMM(int height)
{
	setPositionInStep(map(height, posMMOffSet, maxheight, 0, maxStep));
}

uint8_t StepperControl::getStatus()
{
	return calibrationState;
}
