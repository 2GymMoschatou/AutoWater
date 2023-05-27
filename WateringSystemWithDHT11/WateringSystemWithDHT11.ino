/*
 2GM Roboteam
 Automatic Watering System
*/
//Including libraries
#include <Servo.h>
#include <DHT.h>

//Creating servos and setting initial positions
Servo servo0;
Servo servo1;
Servo servo2;
int servo0Pos = 90;
int servo1Pos = 90;
int servo2Pos = 0;

//Setting pins
const int servo0Pin=2;
const int servo1Pin=3;
const int servo2Pin=4;
const int relay0Pin=5;
const int relay1Pin=6;
const int dhtPin=10;
const int echoPin=11;
const int trigPin=12;
const int hygro0Pin=A0;
const int hygro1Pin=A1;
const int lightPin=A2;

//Setting the variables
int hygro0;
int hygro1;
int light;
long duration;
long distance;
bool water0;
bool water1;
float hum;
float temp;

//Setting the DHT sensor type
#define dhtType DHT11
DHT dht(dhtPin, dhtType);

//Setting the critical values
const int minDistance=5;
const int maxDistance=10;
const int minLight=15;

void setup() {
  //Initialize serial communication and DHT sensor
  Serial.begin(9600);
  dht.begin();

  //Attaching servos and writing initial positions
  servo0.attach(servo0Pin);
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo0.write(servo0Pos);
  servo1.write(servo1Pos);
  servo2.write(servo2Pos);

  //Initialize digital pins
  pinMode(relay0Pin, OUTPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);   
  
}

void loop() {
  //Read water level with ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  delay(100);

  //Read moisture of plants at floor 0
  hygro0 = analogRead(hygro0Pin);
  hygro0 = constrain(hygro0,400,1023);
  hygro0 = map(hygro0,400,1023,100,0);
  delay(100);

  //Read moisture of plants at floor 1
  hygro1 = analogRead(hygro1Pin);
  hygro1 = constrain(hygro1,400,1023);
  hygro1 = map(hygro1,400,1023,100,0);
  delay(100);

  //Read light
  light = analogRead(lightPin);
  delay(100);

  //Reading environment temperature and humidity
  //Reading temperature or humidity takes about 250 milliseconds!
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  delay(250);
  
  //Check tank overflow
  if (distance<=minDistance){
    servo2Pos=90;
    servo2.write(servo2Pos);
    delay(100);
  }
  else{
    servo2Pos=0;
    servo2.write(servo2Pos);
    delay(100);
  }
  
  //Check plants in floor 0
  if (light<minLight){
    if (hygro0<30){
      water0=true;
    }
  }
  if (hygro0>90){
    water0=false;
  }
  
  //Watering plants in floor 0
  if (water0==true){
    if (distance<maxDistance){
      digitalWrite(relay0Pin, HIGH);
      delay(100);
      servo0Pos=90;
      servo0.write(servo0Pos);
      delay(100);
    }
    else{
      digitalWrite(relay0Pin, LOW);
      delay(100);
      servo0Pos=0;
      servo0.write(servo0Pos);
      delay(100);
    }
  }
  
  //Check plants in floor 1
  if (light<minLight){
    if (hygro1<30){
      water1=true;
    }
  }
  if (hygro1>90){
    water1=false;
  }
  
  //Watering plants in floor 1
  if (water1==true){
    if (distance<maxDistance){
      digitalWrite(relay1Pin, HIGH);
      delay(100);
      servo1Pos=90;
      servo1.write(servo1Pos);
      delay(100);
    }
    else{
      digitalWrite(relay1Pin, LOW);
      delay(100);
      servo1Pos=0;
      servo1.write(servo1Pos);
      delay(100);
    }
  }
  
  //Stop watering in floor 0
  if (water0==false){
    digitalWrite(relay0Pin, LOW);
    delay(100);
    servo0Pos=90;
    servo0.write(servo0Pos);
    delay(100);
  }
  
  //Stop watering in floor 1
  if (water1==false){
    digitalWrite(relay1Pin, LOW);
    delay(100);
    servo1Pos=90;
    servo1.write(servo1Pos);
    delay(100);
  }
  
  //Print out the values
  Serial.print("Water depth ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.print("Soil humidity 0: ");
  Serial.print(hygro0);
  Serial.println("%");
  Serial.print("Soil humidity 1: ");
  Serial.print(hygro1);
  Serial.println("%");
  Serial.print("Light: ");
  Serial.println(light);
  Serial.print("Environment Humidity: ");
  Serial.print(hum);
  Serial.println("%");
  Serial.print("Environment Temperature: ");
  Serial.print(temp);
  Serial.println("°C ");

  delay(1000);
}
