////センサーの値の取得
//int getSensorValueCoincidently(int pin) {
//  Serial.println("---------------Start getSensorValueCoincidently ---------------");
//  int val = -1;
//  int interval = 1000;
//  int count = 0;
//  int sensor1values[10];
//  int sensor2values[10];
//  int sensor3values[10];
//  int sensor4values[10];
//  int sensor5values[10];
//  while(count < 9) {
//    // sensorPin1,2,3,4,5
//    delay(1000);
//    int value1 = analogRead(sensorPin1);
//    int value2 = analogRead(sensorPin2);
//    int value3 = analogRead(sensorPin3);
//    int value4 = analogRead(sensorPin4);
//    int value5 = analogRead(sensorPin5);
//    sensor1values[count] = value1;
//    sensor2values[count] = value2;
//    sensor3values[count] = value3;
//    sensor4values[count] = value4;
//    sensor5values[count] = value5;
//    values = [value1, value2, value3, value4, value5];
//    for (int i=0; i<5;i++) {
//      Serial.print("pin");
//      int t = i + 1;
//      Serial.print(t);
//      Serial.print(" ");
//      Serial.println(values[i]);
//    }
//    count++;
//  }
//  while(count < 200) {
//    delay(1000);
//    int value = analogRead(pin);
//    int i = count % 10;
//    values[i] = value;
//    Serial.println(value);
//    // check value
//    int minValue = values[0];
//    int maxValue = values[0];
//    for (int t=1;t < 10; t++) {
//      minValue = min(values[t],minValue);
//      maxValue = max(values[t],maxValue);
//    }
//    if ((maxValue - minValue) < 2) {
//      val = maxValue;
//      break;
//    }
//    count++;
//  }
//  Serial.print("sensor value: ");
//  Serial.println(val);
//  Serial.println("---------------End getSensorValueCoincidently ---------------");
//  return val;
//}
