#include <Servo.h> // Includes servo library
#include <dht.h> // include dht library

float temperature;

//for the buzzer
//int buzzer=14; 

//====for ultrasonic sensor:
const int trigPin1 = 9;   // HC-SR04 trigger pin is connected to digital 9
const int echoPin1 = 8;   // HC-SR04 echo pin is connected to digital 8

const int trigPin2 = 12  ;   // HC-SR04 trigger pin is connected to digital 9
const int echoPin2 = 11;   // HC-SR04 echo pin is connected to digital 8

const int trigPin3 = 13;   // HC-SR04 trigger pin is connected to digital 9
const int echoPin3 = 3;   // HC-SR04 echo pin is connected to digital 8

//====for DHT11
#define dhtPin A0     // Analog Pin A0 of Arduino is connected to DHT11 out pin
dht DHT;

//l293D pins to arduino digital
const int leftIn_1 = 7; // (pin 7 of l293D motor driver)
const int leftIn_2 = 6; // (pin 2 of l293D motor driver)
const int rightIn_1 = 5; // (pin 10 of l293D motor driver)
const int rightIn_2 = 4; // (pin 15 of l293D motor driver)

const int servoPin = 10;  // orange signal pin of servo to digital 10
//--------------------------------------------------------------
Servo myservo;
// defines variables

long duration;
long duration2;
long duration3;
int during;
int distance;
int distance2;
int distance3;
int distanceTotal;
int distanceFront;
int distanceRev; 
int distance_1;
float cm1;
float cm2;
int socialThreshold = 180; //this is the distance value required by the public space given in cm (here 6ft)
int leastDistance = 70; // This is the threshold least distance to initiate stop and turn

void setup() {
  //buzzer
  //pinMode(buzzer, OUTPUT);
//HC-SR04
  //autosteer
  pinMode (trigPin1, OUTPUT); // trigger Pin is set as output to send ultrasonic waves
  pinMode (echoPin1, INPUT); // echo Pin is set as input to receive ultrasonic waves
  //social distance check
  pinMode (trigPin2, OUTPUT); // trigger Pin is set as output to send ultrasonic waves
  pinMode (echoPin2, INPUT); // echo Pin is set as input to receive ultrasonic waves

  pinMode (trigPin3, OUTPUT); // trigger Pin is set as output to send ultrasonic waves
  pinMode (echoPin3, INPUT); // echo Pin is set as input to receive ultrasonic waves
 
//DHT11
  delay(500); // delay for sensor reading boot-up
  Serial.println("DHT11 (Humidity and temperature) Sensor\n");
  delay(1000); // delay for interfacing with the sensor

//set l293D pins for OUTPUT 
  pinMode(leftIn_1, OUTPUT); // pin 7 of l293D motor driver is set to act as output
  pinMode(leftIn_2, OUTPUT); // pin 2 of l293D motor driver is set to act as output
  pinMode(rightIn_1, OUTPUT); // pin 10 of l293D motor driver is set to act as output
  pinMode(rightIn_2, OUTPUT); // pin 15 of l293D motor driver is set to act as output
  
  myservo.attach(servoPin); 
  myservo.write(90); //initiate the angle of rotation for servo wing at 90deg
  Serial.begin(9600); // starts serial communication at 9600 baud rate for arduino
}

void loop() {
  // Ultrasonic1
  digitalWrite(trigPin2, LOW);
  delay(500);
  digitalWrite(trigPin2, HIGH);
  delay(1000);
  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);   // Convert the time into a distance
  cm1 = (distance2/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  
  Serial.print("Distance of object1= ");
  Serial.print(cm1);
  Serial.print("cm");
  Serial.println(); //serial.println prints the command after it in the next line
  delay(2000);
  
  // Ultrasonic2
  digitalWrite(trigPin3, LOW);
  delay(500);
  digitalWrite(trigPin3, HIGH);
  delay(1000);
  digitalWrite(trigPin3, LOW);

  duration3 = pulseIn(echoPin3, HIGH);   // Convert the time into a distance
  cm2 = (distance3/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  
  Serial.print("Distance of object2= ");
  Serial.print(cm2);
  Serial.print("cm");
  Serial.println(); //serial.println prints the command after it in the next line
  delay(2000);
  
  // put your main code here, to run repeatedly:
  //this delay of 5000 is the time in milliseconds for the robot to traverse 4 feet
 
    front();
    delay(5000);
    left();
    front();
    delay(2500);
    //if the distance between people is less than social threshold it stops till it is greater
    distanceTotal = distance2+distance3;
    if (distanceTotal<socialThreshold){
      while (distanceTotal<socialThreshold){
        if(distanceTotal<socialThreshold){
          while (distanceTotal<socialThreshold)
          //digitalWrite(buzzer, HIGH);   //Setting the pin HIGH
          Break();
          }
          else{
            front();
            delay(2500);
          }
          right();
          front();
          delay(5000);
          right();
          front();
          delay(2500);
          //if the distance between people is less than social threshold it stops till it is greater
          if(distanceTotal<socialThreshold){
            while (distanceTotal<socialThreshold)
            //digitalWrite(buzzer, HIGH);   //Setting the pin HIGH
            Break();
          }
          else{
            front();
            delay(2500);
          }
          left();
        }
     }
  
  else {
    //defines sweep rotation according to least distance detected by ultrasonic for autosteer
  distanceFront = ping (); //ping sends and receives a pulse by the ultrasonic sensor
  if (distanceFront > leastDistance) {
    front(); //moves forward
    delay(300); 
  }
  else {
    Break(); 
    getDistance(); 
    //
    if (distanceRev > leastDistance) { 
      right (); 
      delay(400); 
      front(); 
    }
    else if(distance_1 > leastDistance) {
      left();
      delay (1000); 
      front();
    }
    else {
      back(); 
      delay(900);
      Break();
    }
  }
  
  //--------------------DHT11---------------------
  DHT.read11(dhtPin);
    temperature = DHT.temperature;
    Serial.print("Humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("% \t");
    Serial.print("Temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println(" C");
    //Serial.println(temperature);
    delay(3000);
  }
 }

// define ping()
int ping (){
  //initiating the ultrasonic sensor and sending a test ping
  digitalWrite(trigPin1, LOW); // clears the trigger Pin
  delayMicroseconds(2); //delay by 2 microseconds
  digitalWrite(trigPin1, HIGH); // sends a test pulse
  delayMicroseconds(10); // send a pulse for 10 microseconds
  digitalWrite(trigPin1, LOW); // turns off again
  duration = pulseIn(echoPin1, HIGH);   // Convert the time into a distance by reading the echo pin for the 10ms pulse sent
  distance = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343 for distance in centimetres
  Serial.print("Distance of object= ");
  Serial.print(distance);
  Serial.print("cm"); //prints the distance in cm
  Serial.println("");

}

void displayDistance () { 
  // display detected distances in serial monitor
  Serial.print("Right Distance: ");
  Serial.print (distanceRev);
  Serial.println(""); 
  Serial.print("Front Distance: "); 
  Serial.print (distanceFront) ;
  Serial.println(""); 
  Serial.print("Left Distance: ");
  Serial.print (distance_1);
  Serial.println("");
}

//for forward movement
void front(){
  Serial.println("Move Forward");
  digitalWrite(leftIn_1, LOW); //make pin 7 of driver LOW
  digitalWrite(leftIn_2, HIGH); //make pin 2 of driver HIGH
  digitalWrite(rightIn_1, LOW); //make pin 10 LOW
  digitalWrite(rightIn_2, HIGH); //make pin 15 HIGH
}
//to reverse
void back(){
  Serial.println("Move Back");
  digitalWrite(leftIn_1, HIGH);
  digitalWrite(leftIn_2, LOW); 
  digitalWrite(rightIn_1, HIGH);
  digitalWrite(rightIn_2, LOW);
}

void left(){
  digitalWrite(leftIn_1, HIGH);
  digitalWrite(leftIn_2, LOW); 
  digitalWrite(rightIn_1, LOW);
  digitalWrite(rightIn_2, HIGH);
}

void right() {
  digitalWrite(leftIn_1, LOW);
  digitalWrite(leftIn_2, HIGH); 
  digitalWrite(rightIn_1, HIGH); 
  digitalWrite(rightIn_2, LOW);
}
//to stop
void Break(){
  digitalWrite(leftIn_2, LOW);
  digitalWrite(leftIn_1, LOW);
  digitalWrite(rightIn_1, LOW);
  digitalWrite(rightIn_2, LOW);
}
//respective distance check and servo head turn
void getDistance() {
  myservo.write(0); //back to 0deg from initial 90deg. This initiates right side sweep
  delay(500);
  int tempRight_l = ping(); //a temporary ping is received and stored from right sweep
  myservo.write(45); //sweep 45deg (right sweep)
  delay(500);
  int tempRight_2 = ping(); //another temporary ping is received and stored from right sweep
  //if the first ping is less than second, it is set as the distanceRight distance for forward decision at the top
  if(tempRight_l < tempRight_2) {
    distanceRight = tempRight_l;
  }
  //if the second ping is less than first, it is set as the distanceRight distance for forward decision at the top
  else {
    distanceRight = tempRight_2;
  }
  myservo.write(90); //rotate back to central 90deg position
  delay(500);
  distanceFront = ping(); //send and read ping for forward distance
  
  myservo.write(135); //sweep 135deg (left sweep)
  delay(500);
  int tempLeft_1 = ping(); //a temporary ping is received and stored from left sweep
  myservo.write(180); //sweep 180deg to complete the left sweep
  delay(500);
  int tempLeft_2 = ping(); //another temporary ping is received and stored from left sweep
  //if the first ping is less than second, it is set as the distanceLeft distance for forward decision at the top
  if (tempLeft_1 < tempLeft_2) {
    distanceLeft = tempLeft_1;
  }
  //if the second ping is less than first, it is set as the distanceLeft distance for forward decision at the top
  else{
    distanceLeft = tempLeft_2;
  }
  myservo.write(90); //servo rotation back to original 90deg position
}
