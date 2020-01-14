#include <Arduino.h>
int echopins[] = { D5, D7 };        // Sensor1-ECHO, SENSOR2-ECHO, ...
int trigpins[] = { D0, D6 };        // Sensor1-TRIG, SENSOR2-TRIG, ...
int ledpins[] = { D1, D2, D3, D4 }; // Sensor1-LED-R, Sensor1-LED-G, Sensor2-LED-R, Sensor2-LED-G, ...

int sensorcount = sizeof(echopins) / sizeof(echopins[0]);
int ledcount = sizeof(ledpins) / sizeof(ledpins[0]);

const int REDZONE = 20;     // Led turns RED below this distance (in cm)
const int YELLOWZONE = 50;  // Led turns YELLOW below this distance (in cm)

void setup() {
  Serial.begin(460800);

  // Set LED pins as outputs
  for (int i = 0; i < ledcount; i++)
    pinMode(ledpins[i], OUTPUT);

  // Configure Sensor pins
  for (int i = 0; i < sensorcount; i++) {
    pinMode(echopins[i], INPUT);
    pinMode(trigpins[i], OUTPUT);
  }
}

float measure(int echoPin, int trigPin) {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH, 50000) / 58.2; //Calculate the distance (in cm) based on the speed of sound.
}

void setled(float result, int pin_r, int pin_g) {
  if (result < REDZONE) {
    digitalWrite(pin_r, HIGH);
    digitalWrite(pin_g, LOW);
  } else if (result < YELLOWZONE) {
    digitalWrite(pin_r, HIGH);
    digitalWrite(pin_g, HIGH);
  } else {
    digitalWrite(pin_r, LOW);
    digitalWrite(pin_g, HIGH);
  }
}

void loop() {
  unsigned long s = millis();

  // Read all sensors and set LEDs
  for (int i = 0; i < sensorcount; i++)
  {
    float result = measure(echopins[i], trigpins[i]);
    setled(result, ledpins[i * sensorcount], ledpins[(i * sensorcount) + 1]);
    Serial.print(result);  // Output distance
    Serial.print(",");
  }
  Serial.println();
  unsigned long e = millis();

  // Delay for 100ms (or immediately if the measurement(s) took longer than 100ms)
  int delaytime = 100 - (e-s);
  delay(max(0, delaytime));
}
