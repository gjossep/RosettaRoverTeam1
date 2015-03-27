//Set up varablies
//-----------------------------
//Edit all the PINS!!!!!!!!
//-----------------------------
//PWM pins for the front two motors
int PWMr = 3;
int PWMl = 11;

//The leads for the motors
int L1r = 12;
int L1l = 13;



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
boolean followLine = true;

void setup() {
  //Setting all the pins of the motor to output modes
  //All front motors
  pinMode(PWMr, OUTPUT);
  pinMode(PWMl, OUTPUT);
  pinMode(L1r, OUTPUT);
  pinMode(L1l, OUTPUT);



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
    Serial.println(inputString);
    command(inputString);
    inputString = "";
    stringComplete = false; 
  }

  if(followLine) {
    //Check if the middle pin has the sensor. 
    if(checkTsplit()) {
      turnLeft();
    } 
    else {
      if(aB(analogRead(IR4pin))) {
        setMotor(1,200,1);
        setMotor(2,200,1);
      } 
      else if(readLeftSensors()) {
        setMotor(1,200,1);
        setMotor(2,200,0);
      } 
      else if(readRightSensors()) {
        setMotor(1,200,0);
        setMotor(2,200,1);
      } 
      else {
        //No line!
        setMotor(1,0,1);
        setMotor(2,0,0);
      }
    }
  }
}

void turnLeft() {
 setMotor(1,200,0);
 setMotor(2,200,1);
 delay(1000); 
}

boolean checkTsplit() {
  if(aB(analogRead(IR1pin)) && aB(analogRead(IR2pin)) && aB(analogRead(IR3pin)) && aB(analogRead(IR4pin)) && aB(analogRead(IR5pin)) && aB(analogRead(IR6pin)) && aB(analogRead(IR7pin))) {
    return true;
  } 
  else {
    return false; 
  }

}
boolean aB(int PWMIn) {
  //Convert the analog signal from the sensors to a true or false. 
  if(PWMIn <= 511) {
    return(true);
  } 
  else {
    return(false);
  }
}

boolean readLeftSensors() {
  if(aB(analogRead(IR1pin))) {
    return true;
  } 
  else if(aB(analogRead(IR2pin))) {
    return true;
  } 
  else if(aB(analogRead(IR3pin))) {
    return true;
  } 
  else {
    return false; 
  }
}

boolean readRightSensors() {
  if(aB(analogRead(IR5pin))) {
    return true;
  } 
  else if(aB(analogRead(IR6pin))) {
    return true;
  } 
  else if(aB(analogRead(IR7pin))) {
    return true;
  } 
  else {
    return false; 
  }
}

void command(String data) {
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
  if(val == 0) {
    if(followLine) {
      followLine = false; 
    } 
    else if(!followLine) {
      followLine = true; 
    }

  } 
  else if(val == 1) {
    //All motors full backward


  } 
  else if(val == 2) {
    //All left motor full forward, all right motor full backward


  } 
  else if(val == 3) {
    //All right motor full forward, all left motor full backward


  } 
  else if(val == 4) {
    //All motors 0 speed.

  }
}

void setMotor(int motorID, int motorSpeed, int dir) {
  /*
  ----------------------------------------------------------
   Motor 1 = R
   Motor 2 = L
   ---------------
   Speed 1 = 127 PWM
   Speed 0 = 0 PWM
   ---------------
   Direction 1 = Forward
   Direction 0 = Backward
   ----------------------------------------------------------
   */
  if(motorID == 1) {
    //Set speed
    analogWrite(PWMr, motorSpeed); 

    //Check direction
    if(dir == 1) {
      //Change the leads and print out
      digitalWrite(L1r, HIGH);
      Serial.println("Forward Motor Right");
    } 
    else {
      //Change the leads and print out
      digitalWrite(L1r, LOW);
      Serial.println("Backward Motor Right");
    }


  } 
  else if(motorID == 2) {

    //Set speed
    analogWrite(PWMl, motorSpeed); 

    //Check direction
    if(dir == 1) {
      //Change the leads and print out
      digitalWrite(L1l, HIGH);
      Serial.println("Forward Motor Left");
    } 
    else {
      //Change the leads and print out
      digitalWrite(L1l, LOW);
      Serial.println("Backward Motor Left");
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




