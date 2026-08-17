#include <SoftwareSerial.h>

SoftwareSerial gsm(7, 8);   // RX, TX

const int gasSensor = A0;
const int buzzer = 9;
const int led = 13;

const int threshold = 400;
bool alertSent = false;

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);

  pinMode(gasSensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);

  delay(1000);

  Serial.println("LPG Leakage Monitoring System Started");
}

void loop() {

  int gasValue = analogRead(gasSensor);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > threshold) {

    digitalWrite(buzzer, HIGH);
    digitalWrite(led, HIGH);

    Serial.println("WARNING: LPG LEAK DETECTED!");

    // Send SMS only once for the current leak
    if (!alertSent) {
      sendSMS();
      alertSent = true;
    }

  } else {

    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);

    Serial.println("Gas Level Normal");

    // Allow another SMS if a new leak occurs
    alertSent = false;
  }

  delay(1000);
}

void sendSMS() {

  gsm.println("AT+CMGF=1");
  delay(1000);

  gsm.println("AT+CMGS=\"+91XXXXXXXXXX\"");
  delay(1000);

  gsm.println("WARNING! LPG GAS LEAKAGE DETECTED. Please check immediately.");
  delay(500);

  gsm.write(26);   // CTRL+Z
  delay(5000);

  Serial.println("SMS Alert Sent");
}