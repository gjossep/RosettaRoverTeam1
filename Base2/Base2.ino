
/*
  ---------------MAIN PROBLEMS FOUND IN THE CODE---------------
  
  - The time it takes for the rover to move forward before a turn. (So called the turning point) is diffrent in the normal left/right turn than the Tsplit turn. Will need testing to know the real value.
  
  - Code contains a old function to check with time that there hasnt been a turn detected twice that are the same. Left turn does not have is, but right turn does.
  
  - All timing of the turns need to be reconfigured as this depends on speed. Testing is required.
  
  - Front and back end-buttons are not added in code and have no pins linked to them.
  
  - Implementing (When to do it) Ultrasonic measurement code is missing.
  
  - Temp Code needs testing.
  
  - T-Split checker if is only checking the middle + one sensor on each side. This should be both sensors on each side.
  
  - All of the servo/stepper code to move the arm is missing.
  
  --------------------------------------------------------------

*/

//Any header files we need to have to import any library's we need.
#include <OneWire.h>
#include <NewPing.h>

//PWM pins needed the front two motors
int PWMr = 3;
int PWMl = 11;

//The polerity leads for the motors
int L1r = 12;
int L1l = 13;

//Setting up the pins for the line tracking sensors.

//Front Sensors
int IR2pin = A8; //Left side
int IR3pin = A9; //Left side
int IR4pin = A10; //Middle pin
int IR5pin = A11; //Right side
int IR6pin = A12; //Right side

//Back Sensors
int IR7pin = A13; //Right Back
int IR8pin = A14; //Back middle
int IR9pin = A15; //Left Back

//Counters to count the amount of T-Splits, Left, Right turns.
int splits = 0;
int left = 0;
int right = 0;

//Speed for the motors to drive
int speedM = 100;

//Time pointers to make sure the sensors dont detect the same left,right turn twice.
unsigned long rightOldTime;
unsigned long leftOldTime;

//Set up the char holder for any input commands
String inputString = "";
boolean stringComplete = false;
boolean followLine = false;

//Temp sensor pins
int leftTempPin = 9;
int middleTempPin = 10;
int rightTempPin = 11;

//OneWire objects created to use for the temp measurements. This is why we include OneWire.h in the top of the code.
OneWire leftTempSensor(leftTempPin);
OneWire middleTempSensor(middleTempPin);
OneWire rightTempSensor(rightTempPin);

//Ultrasonic sensor pins and max distance
#define TRIGGER_PIN  10
#define ECHO_PIN     11
#define MAX_DISTANCE 400

//Creating the NewPing object to use for the ultrasoic measurements. 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//Setup method to run once before main code starts.
void setup() {
  
  //Seting up the serial output to send data and debug info.
  Serial.begin(9600);
  
  //All front motors pin types.
  pinMode(PWMr, OUTPUT);
  pinMode(PWMl, OUTPUT);
  pinMode(L1r, OUTPUT);
  pinMode(L1l, OUTPUT);
  
  //Setting modes the front line sensors
  pinMode(IR2pin, INPUT);
  pinMode(IR3pin, INPUT);
  pinMode(IR4pin, INPUT);
  pinMode(IR5pin, INPUT);
  pinMode(IR6pin, INPUT);
  
  //Setting modes for the back line sensors
  pinMode(IR7pin, INPUT);
  pinMode(IR8pin, INPUT);
  pinMode(IR9pin, INPUT);
  
  
}

void loop() {

  //Checks if the input command string is complete and is a full line.
  if(stringComplete) {
    
    //DEBUG PRINT LINE OUT AGAIN.
    Serial.println(inputString);
    
    //Find and run any command depending what is in the command string.
    command(inputString);
    
    //Reset the inputstring and stringcomplete variables so new data can be recived.
    inputString = "";
    stringComplete = false; 
  }
  
  sendData(""+1,""+1,""+38);
  delay(1000);

  //Main if statement to check if we should follow a line or not. Start or stop command is linked to this.
  if(followLine) {
    
    //Checks if the line sensors are picking up a T-Split. Also START of the big IF statement to keep the rover on the line.
    if(checkTsplit()) {
      
      //Special Function to turn on a T-Split
      turnTSplit();
      
      //Add one to the T-Splits counter.
      splits++;
      
      //Checks if the rover passed its second split to start the function to go backwards and run temp meuserments. 
      if(splits == 2) {
          tempMeasurement();
      } 
    } 
    
    //Statement to check if only the middle front sensor is seeing the line. 
    else if(analogToBool(analogRead(IR4pin))) {
      
      //Tells the motors both to go forward at the same speed. 
      setMotor(1,speedM,1);
      setMotor(2,speedM,1);
    } 
    
    //Statement to check if any of the left side of the front sensors is seeing the line. This is to correct and make the rover stay on the line.
    else if(readLeftSensors()) {
      
      //Tells the right motor to go forward at full speed, but the left motor to go full speed backwards. This will create the effect of a small left turn.
      setMotor(1,speedM,0);
      setMotor(2,speedM,1);
    } 
     
    //Statement to check if any of the right side of the front sensors is seeing the line. This is to correct and make the rovover stay on the line.
    else if(readRightSensors()) {
      
      //Tells the left motor to go forward at full speed, but the right motor to go full speed backwards. This will create the effect of a small right turn.
      setMotor(1,speedM,1);
      setMotor(2,speedM,0);
    } 
     
    //This is to make sure if no line is found the rover will stop. END of the IF statement that keeps the rover on the line.
    else  {
      
      //Tells both motors to stand still.
      setMotor(1,0,1);
      setMotor(2,0,1);
    } 
    
    //Statement to check if all of the sensors on the left have a line. This means that theres a left turn availble.
    if(checkLeft()) {
      
      //Will execute the left turn function. This function itself will determine if its nessary to turn or not. THIS DOES NOT MEAN IT WILL TURN LEFT!
      turnLeft();
      
      //Add one to the left counter.
      left++;
    } 
    
    //Statement to check if all the the sensors on the right have a line. This means that theres a right turn availble.
    else if(checkRight()) {
      
      //Will execute the right turn function. This function itself will determine if its nessary to turn or not. THIS DOES NOT MEAN IT WILL TURN RIGHT!
      turnRight();
      
      //Add one to the right counter.
      right ++;
    }
    
    //Final delay for the system before it restarts this loop again. All of the extra function like turning and measuring distance and arm movment is done in other smaller functions.
    delay(50);
  }
  
//END OF THE MAIN LOOP!
}

//Function to check if its nessary to turn and if it is it will turn.
void turnLeft() {
  
  //Basic checking system. Uses the counting system to determine if it should turn. Only on turns 0,2,3,4 it should turn.
  boolean turn = false;
  if(left == 0) {
    turn = true;
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
  
  //OLD CODE -- WILL BE REMOVED.
  if((millis() - leftOldTime) > 10000) {
    if(turn == true) {
      turn = true;
    } else {
      turn = false;
    }
  } else {
   turn = false; 
  }
  //END OF OLD CODE.

  //Boolean if the counter is on the correct value so the rover needs to turn now.
  if(turn == true) {
    
    //First we set the motors to move forward to compensate that the turning point of the rover is in the middle, but the detection of the left turn is at the front.
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    
    //Let the motors run to get to the turning point.
    delay(1000);
    
    //Set the motors to start the turn. This is done by making the left motor go full speed backwards and making the right motor full speed. 
    setMotor(1,speedM,0);
    setMotor(2,speedM,1);
    
    //Delay to allow for the turn to happen, otherwise the main loop will reset the motor command.
    delay(3200);
  } 
  
  //If the count is not correct it means that we should not turn on this left turn and ignore it.
  else {
    
    //We add a buffer and move the rover a bit forward so that the sensors dont detect the same left turn twice. 
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    
    //Allow for the movement and it will go back to the Main loop.
    delay(300); 
  }
}

//Function to check if its nessary to turn and if it is it will turn.
void turnRight() {
  
  //Basic checking system. Uses the counting system to determine if it should turn. Only on turns 1 it should turn.
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
  
  //OLD CODE -- WILL BE REMOVED.
  if((millis() - rightOldTime) > 10000) {
    if(turn == true) {
      turn = true;
    } else {
      turn = false;
    }
  } else {
   turn = false; 
  }
  //END OF OLD CODE.

  //Boolean if the counter is on the correct value so the rover needs to turn now.
  if(turn == true) {
    
    //First we set the motors to move forward to compensate that the turning point of the rover is in the middle, but the detection of the right turn is at the front.
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    
    //Let the motors run to get to the turning point.
    delay(1000);
    
    //Set the motors to start the turn. This is done by making the right motor go full speed backwards and making the left motor full speed. 
    setMotor(1,speedM,1);
    setMotor(2,speedM,0);
    
     //Delay to allow for the turn to happen, otherwise the main loop will reset the motor command.
    delay(3200);
  } 
  
  //If the count is not correct it means that we should not turn on this right turn and ignore it.
  else {
    
    //We add a buffer and move the rover a bit forward so that the sensors dont detect the same left turn twice. 
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    
    //Allow for the movement and it will go back to the Main loop.
    delay(600); 
  }
}


//Function to turn on a T-Split. On a T-Split we always need to turn left.
void turnTSplit() {
  
  //Determines if we need to take the turn or ignore it. We will only not turn on the second turn. This is when the rover is a the temp measurements.
  boolean turn = false;
    if(splits == 2) {
      turn = false;
    } else {
      turn = true; 
    }
  
  //We need to turn as the check return true.
  if(turn == true) {
    
    //Set the motors to move forward to the turning point of the rover.
    setMotor(1,speedM,1);
    setMotor(2,speedM,1);
    
    //Allow time for the rover to move forward to the turning point.
    delay(700);
    
    //Set the motors to turn left by setting the left motor to go full speed backwards, and running the right motor at full speed. This will turn the rover left.
    setMotor(1,speedM,0);
    setMotor(2,speedM,1);
    
    //Allow time delay for the rover to turn. Then go back to main loop.
    delay(2200);
  }
  
  
}

//This function runs to make the rover slowly go backwards by looking at the back 3 sensors untill the end-button is pressed and a temp-measurement is done.
void tempMeasurement() {
  
  //Reset the speed to make the rover slowly to have make sure its folling the line the best it can.
  speedM = 50;
 
  //A while loop to make the rover go backwards until the end-button is pressed. 
  while(true) {
    
    //Looks if the middle back sensor sees the line to make the rover go straight.
     if(analogToBool(analogRead(IR8pin))) {
 
        //Sets the both motors at the same speed backwards.
        setMotor(1,speedM,0);
        setMotor(2,speedM,0);
      } 
      
      //Looks if the left back sensor sees the line. This means the rover is off the line and needs to be corrected.
      else if(analogToBool(analogRead(IR9pin))) {
        //Sets the left motor to go forwards and the right motor to go backwards.
        setMotor(1,speedM,1);
        setMotor(2,speedM,0);
      } 
      
      //Looks if the right back sensor sees the line. This means the rover is off the line and needs to be corrected.
      else if(analogToBool(analogRead(IR7pin))) {
        //Sets the right motor to go forwards and the left motor to go backwards.
        setMotor(1,speedM,0);
        setMotor(2,speedM,1);
      } 
      
      //If the Back sensors cant find the line to rover will stop.
      else  {
        
        //Sets both motors to have 0 speed.
        setMotor(1,0,1);
        setMotor(2,0,1);
      } 
  }
 
  //This will do the temp measurement for all three of the sensors. We send this data over the serial port.
  
  float leftTemp = getTemp(leftTempSensor);
  float middleTemp = getTemp(middleTempSensor);
  float rightTemp = getTemp(rightTempSensor);
  
  //DEBUG CODE
  Serial.print("LeftTemp: ");
  Serial.println(leftTemp);
 
  Serial.print("MiddleTemp: ");
  Serial.println(middleTemp);
 
  Serial.print("RightTemp: ");
  Serial.println(rightTemp);
  //END OF DEBUG
 
  //Resets the speed back to full speed for normal operation.
  speedM = 100;
}

//Function to check if all the left sensors including the middle one has the line in sight.
boolean checkLeft() {
  
  //Main check with AND statements meaning that they all have to be true to have the result be true.
  if(analogToBool(analogRead(IR3pin)) && analogToBool(analogRead(IR4pin)) && !analogToBool(analogRead(IR5pin))) {
    
    //Returns a true back to the main loop.
    return true;
  } 
  
  //Atleast one or more of the sensors didnt see the line so its false for a left turn.
  else {
    
    //Returns a false back the main loop.
    return false;
  }
}


//Function to check if all the right sensors including the middle one has the line in sight.
boolean checkRight() {
  
  //Main check with AND statments meaning they all have to be true to have the result be tru.
  if(analogToBool(analogRead(IR5pin)) && analogToBool(analogRead(IR4pin)) && !analogToBool(analogRead(IR3pin))) {
    
    //Returns a true back to the main loop.
    return true;
  } 
  
  //Atleast on or more of the sensors didnt see the line its a false for a right turn.
  else {
    
    //Returns a false back to the main loop.
    return false;
  }
}

//Function to check if the center and a left sensor and a right sensor has the line in sight. This means that its a both a left or right turn which = a T-split
boolean checkTsplit() {
  
  //If statment using ANDS to check if all of them are true. If one of them is false it means its not a T-Split
  if(analogToBool(analogRead(IR3pin)) && analogToBool(analogRead(IR4pin)) && analogToBool(analogRead(IR5pin))) {
    
    //Will return a true to the main loop.
    return true;
  } 
  
  //One of the sensors cant find the line so its not a T-Split
  else {
    
    //Will return a false to the main loop.
    return false; 
  }

}

//Funcation to check what the inner left sensor is seeing. Used for correcting when following the line.
boolean readLeftSensors() {
  
  //Statment that checks if the sensor is seeing the line.
  if(analogToBool(analogRead(IR3pin))) {
    
    //Will return true to the main-loop.
    return true;
  } 
  
  //The inner line doesnt see the line.
  else {
    
    //Will return a false to the main-loop.
    return false; 
  }
}

//Funcation to check what the inner right sensor is seeing. Used for correcting when following the line.
boolean readRightSensors() {
  
  //Statment that checks if the sensor is seeing the line.
  if(analogToBool(analogRead(IR5pin))) {
    
    //Will return true to the main-loop.
    return true;
  } 
  
  //The inner line doesnt see the line.
  else {
    
    //Will return a false to the main-loop.
    return false; 
  }
}

//Funcation that will convert the signal coming from the sensors into a boolean value.
boolean analogToBool(int PWMIn) {
  
  //Checks if the analog signal is less than or equal to a 700. This is value is determined by testing and distance from the ground.
  if(PWMIn <= 700) {
    
    //Will return a false to the main loop. It cant see the line.
    return(false);
  } 
  
  //The analog value is greater than 700 so this means it can see the line.
  else {
    
    //Will return a true to the main loop. It can see the line.
    return(true);
  }
}

/*

---------------------General Functions below--------------------------

*/

//Read the temp from one of the sensors.
float getTemp(OneWire sensor) {

  //Create arrays to store the address of the sensors.
  byte data[12];
  byte addr[8];

  //Search on the one-wire for any of the sensors.
  if ( !sensor.search(addr)) {
    //no more sensors on chain, reset search
    sensor.reset_search();
    return -1000;
  }
  
  //Make sure that the serial code of the sensor is correct size.
  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  //Check if the device serial code of the sensor makes sense.
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  //Reset the wire and select the temp sensor. Send it a command to start doing the measurement
  sensor.reset();
  sensor.select(addr);
  sensor.write(0x44,1); // start conversion, with parasite power on at the end

  //Clear the wire and then select the temp sensor. Send it a command to output the data.
  byte present = sensor.reset();
  sensor.select(addr);  
  sensor.write(0xBE); // Read Scratchpad

  //Read the bytes coming in by the sensor.
  for (int i = 0; i < 9; i++) { // we need 9 bytes
   data[i] = sensor.read();
  }
  
  //Reset the sensor back to the orginal state.
  sensor.reset_search();
  
  //Combine the array of the bytes into one number.
  byte MSB = data[1];
  byte LSB = data[0];
 
  //Convert the data from the 9 bytes into a real temp.
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
 
  //Return the value of the temp back.
  return TemperatureSum;
} 

int getUltrasonic() {
 int uS = sonar.ping();
 return (uS / US_ROUNDTRIP_CM);
}


//This function will send the data in a speical order to the serial port.
void sendData(String dataType, String sensorIdent, String sensorValue) {
  /*
  ----------------Order of data----------------
    - DataType
      - This means what kind of data is it? Temp, Distance, Pressure?
      
    -SensorIdent
      - This means which sensor is the data coming from. Some sensors only have one sensor attached, but others have more then one.
      
    -SensorValue
      - This stands for the real data. Expl: 39 Celcuis.
      
   ---------------------------------------------
  
  */
  
  //Prints without line-ending the dataType to the serial port.
  Serial.print(dataType);
  
  //Prints without line-ending the sensorIdent to the serial port.
  Serial.print(sensorIdent);
  
  //Prints with line-ending the sensorValue to the serial port.
  Serial.println(sensorValue); 
}

//This function waits for the serialEvent or any data incoming from the serialport. Becaues this data comes in by bytes we need get all bytes till a Line-Ending before we know what the command is.
void serialEvent() {
  
  //Checks if the SerialPort has bytes waiting to be read. Will read untill theres nothing left at the input.
  while (Serial.available()) {
    
    //Reads the char value of the incoming byte. This is one letter in the command.
    char inChar = (char)Serial.read(); 
    
    //We add this letter to a string so we can form a sentence.
    inputString += inChar;
    
    //If the letter we recived was Line-Ending we stop adding the sentence and let the main loop know that there is a command waiting.
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

//A function to find out what we need to do with a incoming command from the outside world.
void command(String data) {
  
  /*
  ------------------Table of commands possible.-----------------------
    - 0 = Start/Stop
  -------------------------------------------------------------------
   */
   
  //Read the input command and convert it to an integer.
  int val = data.toInt();
  
  //If statment to check if the command was a 0. Meaning a stop/start command.
  if(val == 0) {
    
    //If the followLine is already true, we need to set it to false to act like a stop command.
    if(followLine) {
      
      //Set the followLine bool to false which will disable the main-loop.
      followLine = false; 
    } 
    
    //FollowLine is false, which means the system is stoped. To start it we need to set it as a true.
    else if(!followLine) {
      
      //Sets the followLine bool to a true which will enable the main-loop.
      followLine = true; 
    }
  }
}


//Function to set a motor to a speed and direction.
void setMotor(int motorID, int motorSpeed, int dir) {
  
  /*
  ---------------------Guide------------------------------------------------------------------------------
  
    -When the motor is set to these speed/direction it will keep them unitll a new speed/direction is set.
  
   Motor 1 = R
   Motor 2 = L
   ---------------
   Direction 1 = Forward
   Direction 0 = Backward
   ----------------------------------------------------------------------------------------------------------
   */
   
  //If the command is trying to set motor 1, or the Right one. 
  if(motorID == 1) {
    
    //Sets the speed to the motor.
    analogWrite(PWMr, motorSpeed); 

    //This will check which direction is requesed, forward or backwards.
    if(dir == 1) {
      
      //Write a HIGH to the right motor lead to force it to go forwards.
      digitalWrite(L1r, HIGH);
    } 
    
    //The other direction is requesed.
    else {
      
      //Write a LOW to the right motor lead to force it to go backwards.
      digitalWrite(L1r, LOW);
    }
    
  } 
  
  //If the command is trying to set motor 2, or the left one.
  else if(motorID == 2) {

    //Sets the speed to the motor.
    analogWrite(PWMl, motorSpeed); 

    //This will check which direction is requesed, forward or backwards.
    if(dir == 1) {
      
      //Write a HIGH to the left motor lead to force it to go forwards.
      digitalWrite(L1l, HIGH);
    } 
    
    //The other direction is requested.
    else {
      
      //Write a LOW to the left motor lead to force it to go backwards.
      digitalWrite(L1l, LOW);
    }

  } 

}











