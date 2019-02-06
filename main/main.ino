#include "SIGFOX.h"

// value for sigfox
static const String device = "NOTUSED";
static const bool useEmulator = false;
static const bool echo = true;
static const Country country = COUNTRY_JP;
static UnaShieldV2S transceiver(country, useEmulator, device, echo);
static String response;

// value for sensor
static int sensorPin1 = A0;
static int sensorPin2 = A1;
static int sensorPin3 = A2;
static int sensorPin4 = A3;
static int sensorPin5 = A4;
int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;
int sensorValue4 = 0;
int sensorValue5 = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Running setup...."));
  
  // setup sigfox
  if(!transceiver.begin()) stop(F("Unable to init SIGFOX module"));
  Serial.println(F("Waiting 5 seconds...."));
  delay(5000);
}

void loop() {
  int pin = detectPin();
  int val = getSensorValue(pin);
  if (val < 0) {
    setValueToPin(val, pin); 
  }
  delay(3000);
  sendMessage();
  delay(3000);
}

void setValueToPin(int val, int pin){
  if (pin == sensorPin1) {
    sensorValue1 = val;
  } else if (pin == sensorPin2) {
    sensorValue2 = val;
  } else if (pin == sensorPin3) {
    sensorValue3 = val;
  } else if (pin == sensorPin4) {
    sensorValue4 = val;
  } else if (pin == sensorPin5) {
    sensorValue5 = val;
  }
}

int getSensorValue(int pin) {
 // get sensorValue
 int val = -1;
 int interval = 1000;
 int count = 0;
 int values[10];
 while(count < 200) {
  delay(1000);
  int value = analogRead(pin);
  int i = count % 10;
  values[i] = value;
  int minValue = values[0];
  int maxValue = values[0];
  for (int t=1;t < 10; t++) {
    minValue = min(values[t],minValue);
    maxValue = max(values[t],maxValue);
  }
  if ((maxValue - minValue) < 2) {
    val = maxValue;
    break;
  }
  count++;
 }
 return val;
}

int detectPin() {
  Serial.println("Which sensor you want to masure??");
  while (true) {
    if (Serial.available() > 0) { 
      break;
    }
  }
  int pinNumber = Serial.read();
  
  if (pinNumber < 0) { // fail to get pin number;
    Serial.println("Fail to get sensor number");
    return -1;
  }
  Serial.print("OK, Start get value of sensor");
  Serial.println(pinNumber);
  int targetPin = -1;
  if (pinNumber == 1) {
    targetPin = A0;
  } else if (pinNumber == 2) {
    targetPin = A1;
  } else if (pinNumber == 3) {
    targetPin = A2;
  } else if (pinNumber == 4) {
    targetPin = A3;
  } else if (pinNumber == 5) {
    targetPin = A4;
  } else {
    Serial.println("fail to detect pin..");
    return -1;
  }
  return targetPin;
}

// send values to sigfox
void sendMessage() {
  bool canSend = true;
  if (sensorValue1 == 0) {
    canSend = false;
  } else if (sensorValue2 == 0) {
    canSend = false;
  } else if (sensorValue3 == 0) {
    canSend = false;
  } else if (sensorValue4 == 0) {
    canSend = false;
  } else if (sensorValue5 == 0) {
    canSend = false;
  }
  if (canSend == false) {
    Serial.println("There are some zero value, so stop sending..");
    return;
  }
  /// create Hexadecimal
  String value1 = String(sensorValue1, HEX);
  String value2 = String(sensorValue2, HEX);
  String value3 = String(sensorValue3, HEX);
  String value4 = String(sensorValue4, HEX);
  String value5 = String(sensorValue5, HEX);

  String message = "1";
  message.concat(value1);
  message.concat("2");
  message.concat(value2);
  message.concat("3");
  message.concat(value3);
  message.concat("4");
  message.concat(value4);
  message.concat("5");
  message.concat(value5);
  transceiver.sendMessage(message);
  Serial.println("Sending message to sigfox");
  Serial.print("message value: ");
  Serial.println(message);
  // init values 
  sensorValue1 = 0;
  sensorValue2 = 0;
  sensorValue3 = 0;
  sensorValue4 = 0;
  sensorValue5 = 0;
}
