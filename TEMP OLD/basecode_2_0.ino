#include <Stepper.h>
#include <OneWire.h>

#include <Wire.h>
#define motorSteps 96 //is what tells the Arduino how many steps to expect out of the stepper motor. 
                      //This is where you would input how many steps your specific stepper motor has.
#define motorPin1 6
#define motorPin2 7
#define motorPin3 8
#define motorPin4 9

#define NOFIELD 747L    // Analog output with no applied field, calibrate this
#define TOMILLIGAUSS 1953L  // For A1301: 2.5mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 1953mG

#define ledPin 13
 
// initialize of the Stepper library:
Stepper myStepper(motorSteps, motorPin1,motorPin2,motorPin3,motorPin4);
OneWire ds(11);
void setup() {
  // set the motor speed at 60 RPMS.In this portion of the code you can control the speed of the motor.:
  myStepper.setSpeed(30);
 
  // Initialize the Serial port:
  Serial.begin(9600);
 
  // set up the LED pin:
  pinMode(ledPin, OUTPUT);
  // blink the LED:
  blink(3);
}
 
void loop() {
  
  
  //Step forward 100 steps:
  Serial.println("Forward");
  myStepper.step(500);
  delay(1000);
  
  
  
  Serial.println(getTemp());
  
  // Step backward 100 steps: 
  Serial.println("Backward");
  myStepper.step(-500);
  delay(1000);
  
  //Rover will move to the next hole.
  
  //It will drop every 10 cm and get the magnetic field.
  
  for(int i = 0; i < 3; i++) {
    
    myStepper.step(150);
    getMagneticField();
    delay(1000);
  
  }
  
  //retrive the rod.
  
  myStepper.step(-450);
  
  delay(1000);
  
 
}

void getMagneticField() {
  // measure magnetic field
  int raw = analogRead(0);   // Range : 0..1024

  //  Uncomment this to get a raw reading for calibration of no-field point
  Serial.print("Raw reading: ");
  Serial.println(raw);

  long compensated = raw - NOFIELD;                 // adjust relative to no applied field 
  long gauss = compensated * TOMILLIGAUSS / 1000;   // adjust scale to Gauss

  Serial.print(gauss);
  Serial.print(" Gauss ");

  if (gauss > 0)     Serial.println("(South pole)");
  else if(gauss < 0) Serial.println("(North pole)");
  else               Serial.println();
  
}



float getTemp(){
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;
  //anything availible
  int failcount = 0;
  while ( !ds.search(addr)) {
    failcount++;
    if(failcount>5){
      return 0;
      Serial.println("Fail count");
    }
    ds.reset_search();
    delay(250);
    
  }
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      
  }
  
 //checking what kind of censor
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return 0;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
   
  }
  Serial.println(raw);
  celsius = (float)raw / 16.0;
  
  return celsius;
  
}
 
// Blink the reset LED.  It simply flashes the LED located at Pin 13 
// (built-in on many arduinos) to alert the user that it is done with setup,
// and is about to start moving the stepper.:
void blink(int howManyTimes) {
  int i;
  for (i=0; i< howManyTimes; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);  
  }
}
