#ifndef Gripper
#define Gripper

#include "Arduino.h"
#include "Stepper.h"

class Gripper 
{
	public:
		Gripper(int steps, int pin1, int pin2, int pin3, int pin4);
		void moveUp();
		void moveDown();
	private:
		int _pin1;
		int _pin2;
		int _pin3;
		int _pin4;
		int _steps;

};

#endif