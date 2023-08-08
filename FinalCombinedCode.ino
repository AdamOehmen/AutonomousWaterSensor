
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include "GravityTDS.h"
#include <SoftwareSerial.h>
#include"TimerOne.h"
int sensorloop = 0;
int aNum = 1900;

//pH setup
#define ONE_WIRE_BUS 4


//Temp Setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


//TDS setup
#define TdsSensorPin A1
GravityTDS gravityTds;
float tdsValue = 0;

//pH setup
#define SensorPin A0
#define LED 2
#define ArrayLenth 10 //times of collection
int pHArray[ArrayLenth]; //Store the average value of the sensor feedback
int pHsum=0;
float phAvg;
float pH;

//ORP setup
const int analogPin = A2;

//Ultrasonic Sensor 1
#define TRIGGER1_PIN 7
#define ECHO1_PIN 8


//Ultrasonic Sensor 2
#define TRIGGER2_PIN 12
#define ECHO2_PIN 13

//BLE sesnor
const int RX_PIN = 0; // Bluetooth RX digital PWM-supporting pin
const int TX_PIN = 1; // Bluetooth TX digital PWM-supporting pin
const int BLUETOOTH_BAUD_RATE = 38400;
SoftwareSerial bluetooth(RX_PIN, TX_PIN);

//Motor setup
#define motor1Pin 9
#define motor1Pin 10
void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  //Sensor 1 Temp
  sensors.begin();



  //senor 2 TDS
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

  //sensor pH
  pinMode(LED,OUTPUT);

  //BLE setup
  bluetooth.begin(BLUETOOTH_BAUD_RATE);

  //Ultrasonic 1 setup
  pinMode(TRIGGER1_PIN, OUTPUT);
  pinMode(ECHO1_PIN, INPUT);

  //Ultrasonic 2 setup
  pinMode(TRIGGER2_PIN, OUTPUT);
  pinMode(ECHO2_PIN, INPUT);

  //Motor setup
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
}

void loop(void){ 
  
  if(sensorloop = 1000)
  {
    //Sensor 1 Temp
    sensors.requestTemperatures(); 
    Serial.print("Celsius temperature: ");
    Serial.print(sensors.getTempCByIndex(0)); 
    Serial.print(" - Fahrenheit temperature: ");
    Serial.println(sensors.getTempFByIndex(0));
  
  
    
    //Sensor 2 TDS
    gravityTds.setTemperature(sensors.getTempCByIndex(0));  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print(tdsValue,0);
    Serial.println("ppm");
  
    //Sensor 3 pH
    
    for (int i = 0; i < 10; i++)
    {
      pHsum += analogRead(SensorPin);
      delay(2);
    }
    phAvg = pHsum / 10;
    pH = -0.0118*phAvg + 17;
    Serial.println(pH);
    float cpypH = pH;
    pHsum = 0;
    pH = 0;
    phAvg = 0;
  
  
    //Sensor 4 ORP
    int sensorValue = analogRead(analogPin); // Read the analog input value
    sensorValue = sensorValue * 39.5 - 1695;
    Serial.print("ORP: ");
    Serial.println(sensorValue);


    float temp = sensors.getTempCByIndex(0);
    float tds = tdsValue;
    String Result = String("Celsius temperature: ") + temp + String(" TDS: ") + tds + String("ppm pH: ") + cpypH + String(" ORP: ") + sensorValue;
    Serial.println(Result);
    Serial.println();
    bluetooth.println(Result);
  }
  sensorloop++;
  long duration1 = 0;
  long duration2 = 0;
  for( int i = 0; i < 20; i++)
  {
    digitalWrite(TRIGGER1_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER1_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER1_PIN, LOW);
    duration1 = duration1 + pulseIn(ECHO1_PIN, HIGH);
    digitalWrite(TRIGGER2_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER2_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER2_PIN, LOW);
    duration2 = duration2 + pulseIn(ECHO2_PIN, HIGH);
    
    delay(5);
  }
  duration1 = duration1 / 20;
  duration2 = duration2 / 20;
  if(duration1 > aNum || duration2 > aNum)
  {
    digitalWrite(motor1Pin, HIGH);
    digitalWrite(motor2Pin, LOW);
  }
  else
  {
    digitalWrite(motor1Pin, LOW);
    digitalWrite(motor2Pin, HIGH);
  }
  
}
