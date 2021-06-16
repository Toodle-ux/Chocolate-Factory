// include the library for stepper motor and servo motor
#include "Stepper.h"
#include <Servo.h> 

// name different servo motors according to what valve they're controlling
Servo blackChoco; 
Servo whiteChoco;
Servo strawberry;
Servo peach;

// the force sensor reading
int FSR = 0;

// the last reading of the force sensor, so that we can know the change between them
int lastFSR = 0;

// the number of tapping
int tapCount = 0;

// the time of tapping
int tapTime = 0;

// the time of the last tapping, used to count the cumulative tapping time
int lastTapTime = 0;

// the cumulative tapping time
int timeSum = 0;

// the tapping frequency. set the default to a negative number to indicate that the frequency is not calculated
int tapFrequency = -1;

// store the highest force sensor reading
int highest = 0;

// the starting and ending position of the servo motor
int posStart = 0;
int posEnd = 90;

const int stepsPerRevolution = 2300;  
                                     
Stepper myStepper(stepsPerRevolution, 8,9,10,11);   

// make chocolate according to different mood 
// move different motors accordingly
void anxious(){
  blackChoco.write(50);
  whiteChoco.write(0);
  delay(300);
  blackChoco.write(180);
  whiteChoco.write(120);

  delay(500);
  blackChoco.write(50);

  delay(1000);
  myStepper.step(stepsPerRevolution);

  delay(500);

  strawberry.write(posStart);
  peach.write(posStart);
  delay(300);
  strawberry.write(posEnd);
  peach.write(posEnd);

  delay(1000);
  myStepper.step(-stepsPerRevolution);
  
  // reset the program after making the chocolate
  tapCount = 0;
  timeSum = 0;
  tapTime = 0;
  lastTapTime = 0;
  highest = 0;
  tapFrequency = -1;
}

void angry(){
  blackChoco.write(50);
  delay(300);
  blackChoco.write(180);

  delay(500);
  blackChoco.write(50);
  delay(300);
  blackChoco.write(180);

  delay(500);
  blackChoco.write(50);

  delay(1000);
  myStepper.step(stepsPerRevolution);

  delay(500);

  strawberry.write(posStart);
  peach.write(posStart);
  delay(300);
  strawberry.write(posEnd);
  peach.write(posEnd);

  delay(1000);
  myStepper.step(-stepsPerRevolution);
  
  tapCount = 0;
  timeSum = 0;
  tapTime = 0;
  lastTapTime = 0;
  highest = 0;
  tapFrequency = -1;
}

void sad(){
  whiteChoco.write(0);
  delay(300);
  whiteChoco.write(120);

  delay(500);
  whiteChoco.write(0);
  delay(300);
  whiteChoco.write(120);

  delay(1000);
  myStepper.step(stepsPerRevolution);

  delay(500);

  strawberry.write(posStart);
  peach.write(posStart);
  delay(300);
  strawberry.write(posEnd);
  peach.write(posEnd);

  delay(1000);
  myStepper.step(-stepsPerRevolution);
  
  tapCount = 0;
  timeSum = 0;
  tapTime = 0;
  lastTapTime = 0;
  highest = 0;
  tapFrequency = -1;
}

void setup() {
  // set the speed of the stepper at 60 rpm
  myStepper.setSpeed(10);
  Serial.begin(9600);
  // set the servo for different ingredients
  blackChoco.attach(7);
  whiteChoco.attach(6);
  strawberry.attach(5);
  peach.attach(4);
}

void loop() {
  FSR = analogRead(A0);

  // record the highest reading
  if (FSR > highest){
    highest = FSR;
  }
  
  // if two successive readings have a significant difference, that means one tapping
  if (FSR - lastFSR > 200){
    tapCount = tapCount + 1;
    // record the tap time
    tapTime = millis() / 100;
    
    // if no tapping for more than 10s, restart the program
    if (tapTime - lastTapTime > 100){
      tapCount = 0;
      timeSum = 0;
    }
    
    // calculate the total tapping time only when successive tapping is detected
    // the total tapping time equals to the sum of intervals between tappings
    if (lastTapTime != 0){
      timeSum = timeSum + tapTime - lastTapTime;
    }
    lastTapTime = tapTime;
  }
  
  // calculate the frequency, *10 is just used to present the number as integers
  if (millis() / 100 - lastTapTime > 100) {
    tapFrequency = tapCount * 10 / timeSum;
  }
  
  Serial.print(timeSum);
  Serial.print('\t');
  Serial.print(tapCount);
  Serial.print('\t');
  Serial.print(highest);
  Serial.print('\t');
  Serial.println(tapFrequency);
  lastFSR = FSR;

  // classify the mood
  if (tapFrequency > 5){
    anxious();
  }
  
  if (tapFrequency >= 0 && tapFrequency <= 5){
    if (highest > 600){
      angry();
    }
    if (highest > 20 && highest <= 600){
      sad();
    }
  }
  
}
