//    /$$$$$$                  /$$                                        
//   /$$__  $$                | $$                                        
//  | $$  \ $$  /$$$$$$   /$$$$$$$ /$$   /$$  /$$$$$$$  /$$$$$$   /$$$$$$ 
//  | $$$$$$$$ /$$__  $$ /$$__  $$| $$  | $$ /$$_____/ |____  $$ /$$__  $$
//  | $$__  $$| $$  \__/| $$  | $$| $$  | $$| $$        /$$$$$$$| $$  \__/
//  | $$  | $$| $$      | $$  | $$| $$  | $$| $$       /$$__  $$| $$      
//  | $$  | $$| $$      |  $$$$$$$|  $$$$$$/|  $$$$$$$|  $$$$$$$| $$      
//  |__/  |__/|__/       \_______/ \______/  \_______/ \_______/|__/      

#include <SoftwareSerial.h>
#include <Servo.h>

//Configuration des commandes bluetooth
#define FORWARD 'F'
#define BACKWARD 'B'
#define LEFT 'L'
#define RIGHT 'R'
#define STOP 'S'
#define CIRCLE 'C'
#define CROSS 'X'
#define TRIANGLE 'T'
#define SQUARE 'Q'
#define START 'A'
#define PAUSE 'P'

//Branchements moteurs
int in1 = 7;
int in2 = 8;
int in3 = 9;
int in4 = 10;
int enA = 6;
int enB = 5;

//Branchements servo
int servoPin = 11;

//Capteur de distance
int echo = 12;
int trig = 13;

//Variables
SoftwareSerial bluetooth(2, 3); // RX, TX
Servo myServo;  // create servo object to control a servo
bool autoMode = false;

void forward() {
  //Moteur 1
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  //Moteur 2  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  //Moteur 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  //Moteur 2  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left() {
  //Moteur 1
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  //Moteur 2  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right() {
  //Moteur 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  //Moteur 2  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stop() {
  //Moteur 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  //Moteur 2  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void setSpeed(int speed) {
  //vitesse
  analogWrite(enA, constrain(speed, 0, 255));
  analogWrite(enB, constrain(speed, 0, 255));
}

void executeCommand(char command) {
  switch (command) {
    case FORWARD:
      // Perform action for moving forward
      forward();
      break;
    case BACKWARD:
      // Perform action for moving backward
      backward();
      break;
    case LEFT:
      // Perform action for turning left
      left();
      break;
    case RIGHT:
      // Perform action for turning right
      right();
      break;
    case STOP:
      // Perform action for pausing a process or operation
      stop();
      break;
    case CIRCLE:
      // Perform action for circle
      break;
    case CROSS:
      // Perform action for immediate stop or crossing
      break;
    case TRIANGLE:
      // Perform action for toggling a state (e.g., LED on/off)
      break;
    case SQUARE:
      // Perform action for retrieving and sending status information
      break;
    case START:
      // Perform action for starting a process or operation
      autoMode = true;
      break;
    case PAUSE:
      // Perform action for pausing a process or operation
      autoMode = false;
      stop();
      break;
    default:
      // Invalid command received
      break;
  }
}

int getDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  int pulseDelay = pulseIn(echo, HIGH);
  
  return pulseDelay*0.034/2;
}

void autoAction() {
  forward();
  
  if(getDistance() < 40) {
    stop();

    myServo.write(0);
    delay(200);
    int rightDistance = getDistance();

    myServo.write(90);
    delay(200);
    myServo.write(180);
    delay(200);
    int leftDistance = getDistance();

    myServo.write(90);

    if(rightDistance < 40 && leftDistance < 40) {
      left();
      delay(400);
    } else if (rightDistance < 40) {
      left();
      delay(200);
    } else if (leftDistance < 40) {
      right();
      delay(200);
    }
  }
}

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 

  myServo.attach(servoPin);
  myServo.write(90);
  
  bluetooth.begin(9600);

  setSpeed(255);
}

void loop() {
  if (bluetooth.available()) {
    char command = bluetooth.read();
    executeCommand(command);
  }
  
  if(autoMode) {
    autoAction();
  }
}