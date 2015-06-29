

//Set up varablies
//-----------------------------
//Edit all the PINS!!!!!!!!
//-----------------------------

Temp temp(5,6,7);


//PWM pins for the front two motors
int PWMr = 3;
int PWMl = 11;

//The leads for the motors
int L1r = 12;
int L1l = 13;

//Setting up the pins for the line tracking sensors.

//Front
int IR2pin = A8; //Left side
int IR3pin = A9; //Left side
int IR4pin = A10; //Middle pin
int IR5pin = A11; //Right side
int IR6pin = A12; //Right side

//Back
int IR7pin = A13; //Right Back
int IR8pin = A14; //Back middle
int IR9pin = A15; //Left Back

int splits = 0;
int left = 0;
int right = 0;

int speedM = 100;

unsigned long rightOldTime;
unsigned long leftOldTime;

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

  Serial.begin(9600);

  //Setting modes the input sensors

  pinMode(IR2pin, INPUT);
  pinMode(IR3pin, INPUT);
  pinMode(IR4pin, INPUT);
  pinMode(IR5pin, INPUT);
  pinMode(IR6pin, INPUT);
  
  pinMode(IR7pin, INPUT);
  pinMode(IR8pin, INPUT);
  pinMode(IR9pin, INPUT);



}

void loop() {

  
  setMotor(1,speedM,1);
  setMotor(2,speedM,1);
  if(followLine) {
    //  Serial.print("Spilts: ");
    //  Serial.println(splits);
    //  Serial.print("Lefts: ");
    //  Serial.println(left);
    //  Serial.print("Right: ");
    //Serial.println(right);
    //  Serial.println("------------------");


       //Serial.print(aB(analogRead(IR2pin)));
       Serial.print(aB(analogRead(IR3pin)));
        Serial.print(aB(analogRead(IR4pin)));
        Serial.println(aB(analogRead(IR5pin)));
        //Serial.println(aB(analogRead(IR6pin)));


    //Check if the middle pin has the sensor.     
    if(checkTsplit()) {
      //Serial.println("1");
      turnLeftT();
      splits++;
      
      if(splits == 2) {
          tempMeasurement();
      }
      
    } 
    else if(aB(analogRead(IR4pin))) {
       //Serial.println("2");
      setMotor(1,speedM,1);
      setMotor(2,speedM,1);
    } 
    else if(readLeftSensors()) {
     // Serial.println("3");
      setMotor(1,speedM,0);
      setMotor(2,speedM,1);
    } 
    else if(readRightSensors()) {
      //Serial.println("4");
      setMotor(1,speedM,1);
      setMotor(2,speedM,0);
    } 
    else  {
      setMotor(1,0,1);
      setMotor(2,0,1);
    } 
    if(checkLeft()) {
      //Serial.println("5");
      turnLeft();
      left++;
    } 
    else if(checkRight()) {
      //Serial.println("6");
      turnRight();
      right ++;
    }
    delay(50);
  }
}

void turnLeft() {
  boolean turn = false;
  if(left == 0) {
    turn = true;
  }
  if(left == 1) {
    turn = false; 
  }
  if(left == 2){
    turn = true; 
  }
  if(left == 3) {
    turn = true;
  } 
  if(left == 4) {
    turn = true;
    //turn 360.
  }
  
  if((millis() - leftOldTime) > 10000) {
    if(turn == true) {
      turn = true;
    } else {
      turn = false;
    }
  } else {
   turn = false; 
  }

  if(turn == true) {
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(1000);
    setMotor(1,speedM,0);
    setMotor(2,speedM,1);
    delay(3200);
  } 
  else {
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(300); 
  }
}

void turnRight() {
  boolean turn = false;
  if(right == 0) {
    turn = false;
  }
  if(right == 1) {
    turn = true;
  }
  if(right == 2) {
    turn = false;
  }
  if(right == 3) {
    turn = false;
  }
  
  if((millis() - rightOldTime) > 10000) {
    if(turn == true) {
      turn = true;
    } else {
      turn = false;
    }
  } else {
   turn = false; 
  }

  if(turn == true) {
    rightOldTime = millis();
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(1000);
    setMotor(1,speedM,1);
    setMotor(2,speedM,0);
    delay(3200);
  } 
  else {
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(600); 
  }
}

void turnLeftT() {
  
  boolean turn = false;
    if(splits == 2) {
      turn = false;
    } else {
      turn = true; 
    }
  
  if(turn == true) {
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(700);
    setMotor(1,speedM,0);
    setMotor(2,speedM,1);
    delay(2200);
  }
  
  
}

void tempMeasurement() {
  speedM = 50;
 // Go backwards code.
 
 //While pushbutton is not pressed
 //Follow line with back sensors
 
 //After while.
 //Do measurement and stop.
 
 while(true) {
   if(aB(analogRead(IR8pin))) {
       //Serial.println("2");
      setMotor(1,speedM,0);
      setMotor(2,speedM,0);
    } 
    else if(aB(analogRead(IR9pin))) {
     // Serial.println("3");
      setMotor(1,speedM,1);
      setMotor(2,speedM,0);
    } 
    else if(aB(analogRead(IR7pin))) {
      //Serial.println("4");
      setMotor(1,speedM,0);
      setMotor(2,speedM,1);
    } 
    else  {
      setMotor(1,0,1);
      setMotor(2,0,1);
    } 
 }
 
 
 speedM = 100;
}

void turn360() {

}

boolean checkLeft() {
  if(aB(analogRead(IR3pin)) && aB(analogRead(IR4pin)) && !aB(analogRead(IR5pin))) {
    Serial.println("LEFT");
    return true;
  } 
  else {
    return false;
  }
}

boolean checkRight() {
  //Serial.println("CHCECK 1");
  if(aB(analogRead(IR5pin)) && aB(analogRead(IR4pin)) && !aB(analogRead(IR3pin))) {
    Serial.println("RIGHT");
    return true;
  } 
  else {
    return false;
  }
}

boolean checkTsplit() {
  if(aB(analogRead(IR3pin)) && aB(analogRead(IR4pin)) && aB(analogRead(IR5pin))) {
    Serial.println("TSplit");
    return true;
  } 
  else {
    return false; 
  }

}

boolean aB(int PWMIn) {
  //Convert the analog signal from the sensors to a true or false. 
  if(PWMIn <= 700) {
    return(false);
  } 
  else {
    return(true);
  }
}

boolean readLeftSensors() {
if(aB(analogRead(IR3pin))) {
    //Serial.println("2.2");
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
  else {
    return false; 
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
      // Serial.println("Forward Motor Right");
    } 
    else {
      //Change the leads and print out
      digitalWrite(L1r, LOW);
      //Serial.println("Backward Motor Right");
    }


  } 
  else if(motorID == 2) {

    //Set speed
    analogWrite(PWMl, motorSpeed); 

    //Check direction
    if(dir == 1) {
      //Change the leads and print out
      digitalWrite(L1l, HIGH);
      // Serial.println("Forward Motor Left");
    } 
    else {
      //Change the leads and print out
      digitalWrite(L1l, LOW);
      // Serial.println("Backward Motor Left");
    }

  } 

}











