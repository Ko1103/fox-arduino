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
  Serial.println(F("Waiting 2 seconds...."));
  delay(2000);
}

void loop() {
//  int pin = detectPin();
  int pin = autoDetectPin();
  int val = getSensorValue(pin);
  if (val > 0) {
    setValueToPin(val, pin); 
  }
  sendMessage();
//  dummyMessage();
}



void setValueToPin(int val, int pin){
  Serial.println("---------------Start setValueToPin ---------------");
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
  Serial.println("sensor value table");
  Serial.print("sensorPin1: ");
  Serial.println(sensorValue1);
  Serial.print("sensorPin2: ");
  Serial.println(sensorValue2);
  Serial.print("sensorPin3: ");
  Serial.println(sensorValue3);
  Serial.print("sensorPin4: ");
  Serial.println(sensorValue4);
  Serial.print("sensorPin5: ");
  Serial.println(sensorValue5);
  Serial.println("---------------End setValueToPin ---------------");
}

//センサーの値の取得
int getSensorValue(int pin) {
  Serial.println("---------------Start getSensorValue ---------------");
  int val = -1;
  int interval = 1000;
  int count = 0;
  int values[10];
  while(count < 9) {
    delay(1000);
    int value = analogRead(pin);
    values[count] = value;
    Serial.println(value);
    count++;
  }
  while(count < 200) {
    delay(1000);
    int value = analogRead(pin);
    int i = count % 10;
    values[i] = value;
    Serial.println(value);
    // check value
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
  Serial.print("sensor value: ");
  Serial.println(val);
  Serial.println("---------------End getSensorValue ---------------");
  return val;
}

//利用するピン番号を返す
int detectPin() {
  Serial.println("---------------Start detectPin---------------");
  // set pin manually
  Serial.println("Which sensor you want to masure??");
  int targetPin = -1;
  while(true) {
    // ここがなぜか二回trueになる　二回目はavairableが0のままだが通ってしまう
    if (Serial.available() > 0) { 
      char val = Serial.read();
      int value = int(val) - 48;
      switch (value) {
        case 1:
          targetPin = A0; break;
        case 2:
          targetPin = A1; break;
        case 3:
          targetPin = A2; break;
        case 4:
          targetPin = A3; break;
        case 5:
          targetPin = A4; break;
      }
      delay(100);
      if (targetPin < 0) {
        Serial.println("Please try again");
      } else { break; }
    }
  }
  
  // set pin automatically 
//  int targetPin = A0;
//  if (sensorValue1 == 0) {
//    targetPin = A0;
//  } else if (sensorValue2 == 0) {
//    targetPin = A1;
//  } else if (sensorValue3 == 0) {
//    targetPin = A2;
//  } else if (sensorValue4 == 0) {
//    targetPin = A3;
//  } else if (sensorValue5 == 0) {
//    targetPin = A4;
//  }
  Serial.print("targetPin is ");
  Serial.println(targetPin - 13);
  Serial.println("---------------End detectPin---------------");
  return targetPin;
}

// send values to sigfox
void sendMessage() {
  Serial.println("---------------Start sendMessage---------------");
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
//  transceiver.sendMessage(message);
  
  transceiver.sendMessageAndGetResponse(message, response);
  Serial.println("ここまできてるよー");
  Serial.print(F("Downlink response is "));  Serial.println(response);
  delay(10000);
  Serial.println("Sending message to sigfox");
  Serial.print("message value: ");
  Serial.println(message);
  // init values 
  sensorValue1 = 0;
  sensorValue2 = 0;
  sensorValue3 = 0;
  sensorValue4 = 0;
  sensorValue5 = 0;
//  Serial.print("wait for 12 minutes");
//  int t = 0;
//  while(t < 240){
//    delay(30000);
//    t++;
//  }
  delay(10000);
  Serial.println("---------------End sendMessage---------------");
}

void dummyMessage() {
  sensorValue1 = 811;
  sensorValue2 = 356;
  sensorValue3 = 524;
  sensorValue4 = 524;
  sensorValue5 = 741;
  sendMessage();
  delay(2000);
}
