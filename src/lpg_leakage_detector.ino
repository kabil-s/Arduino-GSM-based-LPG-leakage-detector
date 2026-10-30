#include <SoftwareSerial.h>

// SoftwareSerial(RX, TX): Arduino receives on D3 and transmits on D2.
SoftwareSerial GSM(3, 2);

// Replace this placeholder with the phone number that should receive the alert.
char phone_no[] = "+91XXXXXXXXXX";

// MQ-series sensor digital output connected to Arduino digital pin D5.
#define SENSOR_PIN 5

void setup() {
  Serial.begin(9600);
  GSM.begin(9600);

  pinMode(SENSOR_PIN, INPUT_PULLUP);

  Serial.println("Initializing GSM module...");
  initModule("AT", "OK", 1000);
}

void loop() {
  // The original project report treats a HIGH sensor output as gas detection.
  // Confirm the actual output polarity of your sensor module before deployment.
  if (digitalRead(SENSOR_PIN) == HIGH) {
    callUp(phone_no);
  }

  delay(5);
}

void callUp(char *number) {
  GSM.print("ATD");
  GSM.print(number);
  GSM.println(";");  // Start an outgoing voice call.

  delay(20000);       // Keep the call active for approximately 20 seconds.

  GSM.println("ATH"); // Hang up the call.
  delay(100);
}

void initModule(String cmd, char *response, int timeoutMs) {
  while (true) {
    Serial.println(cmd);
    GSM.println(cmd);

    delay(100);

    while (GSM.available() > 0) {
      if (GSM.find(response)) {
        Serial.println(response);
        delay(timeoutMs);
        return;
      } else {
        Serial.println("GSM response not received; retrying...");
      }
    }

    delay(timeoutMs);
  }
}
