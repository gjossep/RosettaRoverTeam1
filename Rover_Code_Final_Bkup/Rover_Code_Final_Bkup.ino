

String inputString = "";
boolean stringComplete = false;

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

int IR2pin = 12; //Left side
int IR3pin = 11; //Left side
int IR4pin = 10; //Middle pin
int IR5pin = 9; //Right side
int IR6pin = 8; //Right side

int splits = 0;
int left = 0;
int right = 0;

int speedM = 42;

unsigned long rightOldTime;
unsigned long leftOldTime;

//Set up the char holder
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


}

void loop() {
  
  if(stringComplete) {
   handlePackage(inputString);
   inputString = "";
   stringComplete = false; 
  }

  if(followLine) {
    //  Serial.print("Spilts: ");
     //Serial.println(splits);
    //  Serial.print("Lefts: ");
    //  Serial.println(left);
    //  Serial.print("Right: ");
    //  Serial.println(right);
    //  Serial.println("------------------");


//    Serial.print(aB(analogRead(IR2pin)));
//    Serial.print(aB(analogRead(IR3pin)));
//    Serial.print(aB(analogRead(IR4pin)));
//    Serial.print(aB(analogRead(IR5pin)));
//    Serial.println(aB(analogRead(IR6pin)));


    //Check if the middle pin has the sensor.     
    if(checkTsplit()) {
      //Serial.println("1");
      turnLeftT();
      splits++;
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
  delay(700);
  setMotor(1,70,0);
  setMotor(2,70,1);
  delay(1250);
  setMotor(1,70,1);
  setMotor(2,70,1);
  delay(200);
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
    delay(700);
    setMotor(1,70,1);
    setMotor(2,70,0);
    delay(1250);
    setMotor(1,70,1);
    setMotor(2,70,1);
    delay(200);
  } 
  else {
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(200); 
  }
}

void turnLeftT() {
  if(splits == 1) {
    // Turn Left
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(350);
    setMotor(1,70,0);
    setMotor(2,70,1);
    delay(1000);
    setMotor(1,70,1);
    setMotor(2,70,1);
    delay(200);
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(200); 
    
    //stop, and go backwards.
    setMotor(1,speedM,0);
    setMotor(2,speedM,0);
    delay(3000);
    
    setMotor(1,0,0);
    setMotor(2,0,0);
    
    
  } else {
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(350);
    setMotor(1,70,0);
    setMotor(2,70,1);
    delay(1000);
    setMotor(1,70,1);
    setMotor(2,70,1);
    delay(200);
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    delay(200); 
  }
  
}


boolean checkLeft() {
  if(aB(analogRead(IR2pin)) && aB(analogRead(IR3pin)) && aB(analogRead(IR4pin)) && !aB(analogRead(IR5pin)) && !aB(analogRead(IR6pin))) {
    Serial.println("LEFT");
    return true;
  } 
  else {
    return false;
  }
}

boolean checkRight() {
  //Serial.println("CHCECK 1");
  if(aB(analogRead(IR5pin)) && aB(analogRead(IR6pin)) && aB(analogRead(IR4pin)) && !aB(analogRead(IR2pin))) {
    Serial.println("RIGHT");
    return true;
  } 
  else {
    return false;
  }
}

boolean checkTsplit() {
  if(aB(analogRead(IR2pin)) && aB(analogRead(IR3pin)) && aB(analogRead(IR4pin)) && aB(analogRead(IR5pin)) && aB(analogRead(IR6pin))) {
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
  if(aB(analogRead(IR2pin))) {
    //Serial.println("2.1");
    return true;
  } 
  else if(aB(analogRead(IR3pin))) {
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
  else if(aB(analogRead(IR6pin))) {
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


void handlePackage(String data){
  switch (data.substring(0,1).toInt()){
    case 0:
      Serial.println("Error Package: "+data.substring(0,1));
      if(followLine) {
        followLine = false;
      } else {
        followLine = true; 
      }
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:                                        
      break;
    default:
      Serial.println("Uknown package!"+data.substring(1)); //Unknown package
      break;
      
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












