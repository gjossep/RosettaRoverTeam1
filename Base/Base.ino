//Set up varablies
//-----------------------------
//Edit all the PINS!!!!!!!!
//-----------------------------
//PWM pins for the front two motors
int PWMfr = 10;
int PWMfl = 9;
//The leads for the front right motor
int L1fr = 8;
int L2fr = 7;
//The leads for the front left motor
int L1fl = 6;
int L2fl = 5; 


//PWM pins for the back two motors
int PWMbr = 20;
int PWMbl = 19;
//The leads for the right back motor
int L1br = 18;
int L2br = 17;
//The leads for the left back motor
int L1bl = 16;
int L2bl = 15; 

//Setting up the pins for the line tracking sensors.
int IR1pin = 25; //Left side
int IR2pin = 26; //Left side
int IR3pin = 27; //Left side
int IR4pin = 28; //Middle pin
int IR5pin = 29; //Right side
int IR6pin = 30; //Right side
int IR7pin = 31; //Right side

void setup() {
  //Setting all the pins of the motor to output modes
  //All front motors
  pinMode(PWMfr, OUTPUT);
  pinMode(PWMfl, OUTPUT);
  pinMode(L1fr, OUTPUT);
  pinMode(L2fr, OUTPUT);
  pinMode(L1fl, OUTPUT);
  pinMode(L2fl, OUTPUT);
  
  //All the back motors
  pinMode(PWMbr, OUTPUT);
  pinMode(PWMbl, OUTPUT);
  pinMode(L1br, OUTPUT);
  pinMode(L2br, OUTPUT);
  pinMode(L1bl, OUTPUT);
  pinMode(L2bl, OUTPUT);
  
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
  //Check if the middle pin has the sensor. 
  if(ab(analogRead(IR4pin)) {
    
  } else if(readLeftSensors) {
    
  } else if(readRightSensors) {
    
  } else {
   //No Line found! 
  }
}

boolean aB(int PWMIn) {
  //Convert the analog signal from the sensors to a true or false. 
  if(PWMin <= 511) {
    return(true)
   } else {
    return(false);
   }
}


