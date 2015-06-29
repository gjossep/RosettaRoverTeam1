#include "Arduino.h";
#include "Temp.h";
#include "OneWire.h";

Temp::Temp(int pin1, int pin2, int pin3) {
	OneWire ds1(pin1);
	OneWire ds2(pin2);
	OneWire ds3(pin3);

}

float Temp::getTemp1() {
	//returns the temperature from one DS18S20 in DEG Celsius

 byte data[12];
 byte addr[8];

 if ( !ds1.search(addr)) {
   //no more sensors on chain, reset search
   ds1.reset_search();
   return -1000;
 }

 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }

 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }

 ds1.reset();
 ds1.select(addr);
 ds1.write(0x44,1); // start conversion, with parasite power on at the end

 byte present = ds1.reset();
 ds1.select(addr);  
 ds1.write(0xBE); // Read Scratchpad

 
 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds1.read();
 }
 
 ds1.reset_search();
 
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;

}

float Temp::getTemp2() {
	//returns the temperature from one DS18S20 in DEG Celsius

 byte data[12];
 byte addr[8];

 if ( !ds2.search(addr)) {
   //no more sensors on chain, reset search
   ds2.reset_search();
   return -1000;
 }

 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }

 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }

 ds2.reset();
 ds2.select(addr);
 ds2.write(0x44,1); // start conversion, with parasite power on at the end

 byte present = ds2.reset();
 ds2.select(addr);  
 ds2.write(0xBE); // Read Scratchpad

 
 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds2.read();
 }
 
 ds2.reset_search();
 
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;

}

float Temp::getTemp3() {
	//returns the temperature from one DS18S20 in DEG Celsius

 byte data[12];
 byte addr[8];

 if ( !ds3.search(addr)) {
   //no more sensors on chain, reset search
   ds3.reset_search();
   return -1000;
 }

 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }

 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }

 ds3.reset();
 ds3.select(addr);
 ds3.write(0x44,1); // start conversion, with parasite power on at the end

 byte present = ds3.reset();
 ds3.select(addr);  
 ds3.write(0xBE); // Read Scratchpad

 
 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds3.read();
 }
 
 ds3.reset_search();
 
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;

}