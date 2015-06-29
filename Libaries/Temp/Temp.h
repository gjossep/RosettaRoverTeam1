#ifndef Temp_h
#define Temp_h

#include "Arduino.h";
#include "OneWire.h";

class Temp {
	public:
		Temp(int pin1, int pin2, int pin3);
		float getTemp1();
		float getTemp2();
		float getTemp3();
	private:
		int _pin1;
		int _pin2;
		int _pin3;
		OneWire ds1;
		OneWire ds2;
		OneWire ds3;

};

#endif