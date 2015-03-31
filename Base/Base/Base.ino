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

// CHANGE THESE IF NESSARARY!!! I DONT KNOW THE THE SENORS NUMBERS
int IR1pin = 31; // Right sensor in 2 sensor row
int IR2pin = 32; // Left sensor in 2 sensor row
int IR3pin = 33; // Left sensor in 3 Sensor row
int IR4pin = 34; // Middle Sensor in 3 Sensor row
int IR5pin = 35; // Right Sensor in 3 Sensor row

//Set up the char holder
String inputString = "";
boolean stringComplete = false;
boolean followLine = false;

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

}

void loop() {
  if(stringComplete) {
    Serial.println(inputString);
    command(inputString);
    inputString = "";
    stringComplete = true; 
  }

  if(followLine) {
    //Check if the middle pin has the sensor. 
    if(checkTsplit()) {
      turnLeft();
    } 
    else {
      if(aB(digitalRead(IR4pin))) {
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
        Serial.println("No line!");
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
  if(aB(digitalRead(IR1pin)) && aB(digitalRead(IR2pin)) && aB(digitalRead(IR3pin)) && aB(digitalRead(IR4pin)) && aB(digitalRead(IR5pin))) {
    return true;
  } 
  else {
    return false; 
  }

}
boolean aB(int PWMIn) {
  //Convert the analog signal from the sensors to a true or false. 
  Serial.println(PWMIn);
  if(PWMIn == 1) {
    return(true);
  } 
  else {
    return(false);
  }
}

boolean readLeftSensors() {
  if(aB(digitalRead(IR2pin))) {
    return true;
  } 
  else if(aB(digitalRead(IR3pin))) {
    return true;
  } 
  else {
    return false; 
  }
}

boolean readRightSensors() {
  if(aB(digitalRead(IR1pin))) {
    return true;
  } 
  else if(aB(digitalRead(IR5pin))) {
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




