#define LEFT 0
#define RIGHT 1

//Connect each encoders cable to the connection on the motor shield.
#define leftPin = A2;
#define rightPin = A3;
 
 
long coder[2] = {
  0,0};
int lastSpeed[2] = {
  0,0};  
 
 
void setup(){
   
  Serial.begin(9600);                            //init the Serial port to print the data
  attachInterrupt(leftPin, LwheelSpeed, CHANGE);    //init the interrupt mode for the digital pin 2
  attachInterrupt(rightPin, RwheelSpeed, CHANGE);   //init the interrupt mode for the digital pin 3
   
}
 
void loop(){
   
  static unsigned long timer = 0;                //print manager timer
   
  if(millis() - timer > 100){                   
    Serial.print("Coder value: ");
    Serial.print(coder[LEFT]);
    Serial.print("[Left Wheel] ");
    Serial.print(coder[RIGHT]);
    Serial.println("[Right Wheel]");
     
    lastSpeed[LEFT] = coder[LEFT];   //record the latest speed value
    lastSpeed[RIGHT] = coder[RIGHT];
    coder[LEFT] = 0;                 //clear the data buffer
    coder[RIGHT] = 0;
    timer = millis();
  }
   
}
 
 
void LwheelSpeed()
{
  coder[LEFT] ++;  //count the left wheel encoder interrupts
}
 
 
void RwheelSpeed()
{
  coder[RIGHT] ++; //count the right wheel encoder interrupts
}



