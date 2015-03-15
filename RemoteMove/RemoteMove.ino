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


//Set up the char holder
char val;

void setup() {
  //Set up the serial port for bluetooth
  Serial.begin(9600);
  
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
  /*
  ----------------------------------------------------------
  Char = f = forward
  Char = b = backward
  Char = r = right
  Char = l = left
  Char = s = stop
  ----------------------------------------------------------
  */
 if(Serial.available()) {
   val = Serial.read();
   
   if(val == 'f') {
     //All motor full forward
     setMotor(1, 1, 1);
     setMotor(2, 1, 1);
     setMotor(3, 1, 1);
     setMotor(4, 1, 1);
     
   } else if(val == 'b') {
     //All motors full backward
     setMotor(1, 1, 0);
     setMotor(2, 1, 0);
     setMotor(3, 1, 0);
     setMotor(4, 1, 0);
     
   } else if(val == 'r') {
     //All left motor full forward, all right motor full backward
     setMotor(1, 1, 0);
     setMotor(2, 1, 1);
     setMotor(3, 1, 0);
     setMotor(4, 1, 1);
     
   } else if(val == 'l') {
     //All right motor full forward, all left motor full backward
     setMotor(1, 1, 1);
     setMotor(2, 1, 0);
     setMotor(3, 1, 1);
     setMotor(4, 1, 0);
     
   } else {
     //All motors 0 speed.
     setMotor(1, 0, 1);
     setMotor(2, 0, 1);
     setMotor(3, 0, 1);
     setMotor(4, 0, 1);
     
   }
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
 int hsp = 127;
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
     digitalWrite(L2fr, LOW);
     Serial.println("Forward Motor FR");
   } else {
     //Change the leads and print out
     digitalWrite(L1fr, LOW);
     digitalWrite(L2fr, HIGH);
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
     digitalWrite(L2fl, LOW);
     Serial.println("Forward Motor FL");
   } else {
     //Change the leads and print out
     digitalWrite(L1fl, LOW);
     digitalWrite(L2fl, HIGH);
     Serial.println("Backward Motor FL");
   }
  
 } else if(motorID == 3) {
   
   //Check speed
   if(motorSpeed == 1) {
    analogWrite(PWMbr, hsp); 
   } else {
    analogWrite(PWMbr, lsp); 
   }
   //Check direction
   if(dir == 1) {
     //Change the leads and print out
     digitalWrite(L1br, HIGH);
     digitalWrite(L2br, LOW);
     Serial.println("Forward Motor BR");
   } else {
     //Change the leads and print out
     digitalWrite(L1br, LOW);
     digitalWrite(L2br, HIGH);
     Serial.println("Backward Motor BR");
   }
   
 } else {
   
   //Check speed
   if(motorSpeed == 1) {
    analogWrite(PWMbl, hsp); 
   } else {
    analogWrite(PWMbl, lsp); 
   }
   //Check direction
   if(dir == 1) {
     //Change the leads and print out
     digitalWrite(L1bl, HIGH);
     digitalWrite(L2bl, LOW);
     Serial.println("Forward Motor BL");
   } else {
     //Change the leads and print out
     digitalWrite(L1bl, LOW);
     digitalWrite(L2bl, HIGH);
     Serial.println("Backward Motor BL");
   }
   
 }
  
}



