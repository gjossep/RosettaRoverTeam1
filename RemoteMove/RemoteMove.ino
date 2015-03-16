//Set up varablies
//-----------------------------
//Edit all the PINS!!!!!!!!
//-----------------------------
//PWM pins for the front two motors
int PWMfr = 3;
int PWMfl = 11;
//The leads for the front right motor
int L1fr = 12;
//int L2fr = 7;
//The leads for the front left motor
int L1fl = 13;
//int L2fl = 5; 


//Setting up the pins for the line tracking sensors.
int IR1pin = A2; //Left side
int IR2pin = A3; //Left side
int IR3pin = A4; //Left side
int IR4pin = A5; //Middle pin
int IR5pin = A6; //Right side
int IR6pin = A7; //Right side
int IR7pin = A8; //Right side


//Set up the char holder
String inputString = "";
boolean stringComplete = false;

void setup() {
  //Set up the serial port for bluetooth
  Serial.begin(9600);
  
  //Setting all the pins of the motor to output modes
  //All front motors
  pinMode(PWMfr, OUTPUT);
  pinMode(PWMfl, OUTPUT);
  pinMode(L1fr, OUTPUT);
  pinMode(L1fl, OUTPUT);
  
  //Setting modes the input sensors
  pinMode(IR1pin, INPUT);
  pinMode(IR2pin, INPUT);
  pinMode(IR3pin, INPUT);
  pinMode(IR4pin, INPUT);
  pinMode(IR5pin, INPUT);
  pinMode(IR6pin, INPUT);
  pinMode(IR7pin, INPUT);
}

void loop() {
  if(stringComplete) {
   changeMotor(inputString);
   inputString = "";
   stringComplete = false; 
  }
  
}

void changeMotor(String data) {
    /*
  ----------------------------------------------------------
  0 = forward
  1 = backward
  2 = right
  3 = left
  4 = stop
  ----------------------------------------------------------
  */
   int val = data.toInt();
   Serial.println("Got: " + val);
   if(val == '0') {
     //All motor full forward
     setMotor(1, 1, 1);
     setMotor(2, 1, 1);  
     
   } else if(val == '1') {
     //All motors full backward
     setMotor(1, 1, 0);
     setMotor(2, 1, 0);
     
   } else if(val == '2') {
     //All left motor full forward, all right motor full backward
     setMotor(1, 1, 0);
     setMotor(2, 1, 1);
     
   } else if(val == '3') {
     //All right motor full forward, all left motor full backward
     setMotor(1, 1, 1);
     setMotor(2, 1, 0);
     
   } else if(val == '4') {
     //All motors 0 speed.
     setMotor(1, 0, 1);
     setMotor(2, 0, 1);
   }
}

void setMotor(int motorID, int motorSpeed, int dir) {
  /*
  ----------------------------------------------------------
  Motor 1 = FR
  Motor 2 = FL
  Motor 3 = BR
  Motor 4 = BL
  ---------------
  Speed 1 = 127 PWM
  Speed 0 = 0 PWM
  ---------------
  Direction 1 = Forward
  Direction 0 = Backward
  ----------------------------------------------------------
  */
 int hsp = 256;
 int lsp = 0;
  
 if(motorID == 1) {
   
   //Check speed
   if(motorSpeed == 1) {
    analogWrite(PWMfr, hsp); 
   } else {
    analogWrite(PWMfr, lsp); 
   }
   //Check direction
   if(dir == 1) {
     //Change the leads and print out
     digitalWrite(L1fr, HIGH);
     //digitalWrite(L2fr, LOW);
     Serial.println("Forward Motor FR");
   } else {
     //Change the leads and print out
     digitalWrite(L1fr, LOW);
     //digitalWrite(L2fr, HIGH);
     Serial.println("Backward Motor FR");
   }
   
  
 } else if(motorID == 2) {
   
   //Check speed
   if(motorSpeed == 1) {
    analogWrite(PWMfl, hsp); 
   } else {
    analogWrite(PWMfl, lsp); 
   }
   //Check direction
   if(dir == 1) {
     //Change the leads and print out
     digitalWrite(L1fl, HIGH);
     //digitalWrite(L2fl, LOW);
     Serial.println("Forward Motor FL");
   } else {
     //Change the leads and print out
     digitalWrite(L1fl, LOW);
     //digitalWrite(L2fl, HIGH);
     Serial.println("Backward Motor FL");
   }
  
 } 
  
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}


