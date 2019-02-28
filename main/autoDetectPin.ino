int autoDetectPin() {
 int pin = A0;
 if (sensorValue1 != 0) {
  pin = A1;
 }
 if (sensorValue2 != 0) {
  pin = A2;
 }
 if (sensorValue3 != 0) {
  pin = A3;
 }
 if (sensorValue4 != 0) {
  pin = A4;
 }
 Serial.print("target pin is ");
 Serial.println((pin - 13));
 return pin;
}
