/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/
//#include <SoftwareSerial.h>
//SoftwareSerial EEBlue(10, 11); // RX | TX
// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
//const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
//int outputValue = 0;        // value output to the PWM (analog out)
char veri;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Serial1.begin(9600);
  //EEBlue.begin(9600); //Default Baud for comm, it may be different for your Module. Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:
  //analogWrite(analogOutPin, outputValue);

  // print the results to the Serial Monitor:
  
  
  if (sensorValue>200)
  {
    digitalWrite(2,HIGH);
    Serial1.println("sensor = ");
    Serial.println(sensorValue);
    Serial1.println(sensorValue);
  }
  else
  {
    digitalWrite(2,LOW);
  }
  //Serial.print("\t output = ");
  //Serial.println(outputValue);

  if (Serial1.available() > 0) {

    veri = Serial1.read();
    if (veri == '1') {
      digitalWrite(3, HIGH);
      Serial1.println("LED Yakildi.");
    }
    if (veri == '2') {
      digitalWrite(3, LOW);
      Serial1.println("LED KAPANDI.");
    }
  }

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(200);
}
