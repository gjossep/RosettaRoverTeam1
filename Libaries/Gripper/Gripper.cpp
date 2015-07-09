#include "Arduino.h"
#include "Stepper.h"
#include "Gripper.h"

Gripper::Gripper(int steps, int pin1, int pin2, int pin3, int pin4) {
	_steps = steps;
	_pin1 = pin1;
	_pin2 = pin2;
	_pin3 = pin3;
	_pin4 = pin4;
	
	Stepper stepper(_steps, _pin1, _pin2, _pin3, _pin4);
	stepper.setSpeed(30);
}

void moveUp() {
	stepper.step(50);
}

void moveDown() {
	stepper.step(-50);
}

