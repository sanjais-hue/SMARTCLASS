#define BLYNK_TEMPLATE_ID "TMPL3gWfc-P2A"
#define BLYNK_TEMPLATE_NAME "smart class"
#define BLYNK_AUTH_TOKEN "A9y5qq4Dw1BWGiZz2lY9tX5lT7A7UYj5"

// Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <HCSR04.h>

// WiFi credentials
char ssid[] = "1234";
char pass[] = "12345678";

// Pin definitions
#define RELAY1_PIN D1
#define RELAY2_PIN D2
#define TRIGGER_PIN1 D3
#define ECHO_PIN1 D4
#define TRIGGER_PIN2 D5
#define ECHO_PIN2 D6

// Distance threshold for detecting object (in cm)
#define OBJECT_DISTANCE 20

// Initialize ultrasonic sensors
HCSR04 ultrasonic1(TRIGGER_PIN1, ECHO_PIN1);
HCSR04 ultrasonic2(TRIGGER_PIN2, ECHO_PIN2);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  
  // Initialize the relays as outputs
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  
  // Set initial state for relays
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  
  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

BLYNK_WRITE(V1) {
  int relayState = param.asInt();  // Get the value from Blynk
  digitalWrite(RELAY1_PIN, relayState);
  digitalWrite(RELAY2_PIN, relayState); // Ensure both relays are in the same state
  Blynk.virtualWrite(V1, relayState);  // Update Blynk with the relay state
  
  // Print relay state to Serial Monitor
  Serial.print("Relay 1 and Relay 2 state: ");
  Serial.println(relayState);
}

BLYNK_WRITE(V2) {
  int relayState = param.asInt();  // Get the value from Blynk
  digitalWrite(RELAY1_PIN, relayState);
  digitalWrite(RELAY2_PIN, relayState); // Ensure both relays are in the same state
  Blynk.virtualWrite(V2, relayState);  // Update Blynk with the relay state
  
  // Print relay state to Serial Monitor
  Serial.print("Relay 1 and Relay 2 state: ");
  Serial.println(relayState);
}

void loop() {
  Blynk.run();
  checkUltrasonicSensors();
}

void checkUltrasonicSensors() {
  int distance1 = ultrasonic1.dist();
  int distance2 = ultrasonic2.dist();

  // Print distances to Serial Monitor
  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print(" cm, ");
  Serial.print("Distance2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  if (distance1 <= OBJECT_DISTANCE) {
    digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);
    Blynk.virtualWrite(V1, HIGH);
    Blynk.virtualWrite(V2, HIGH);
    Serial.println("Object detected by Sensor 1 - Relays ON");
  } 
  
  if (distance2 <= OBJECT_DISTANCE) {
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    Blynk.virtualWrite(V1, LOW);
    Blynk.virtualWrite(V2, LOW);
    Serial.println("Object detected by Sensor 2 - Relays OFF");
  }
  
  delay(1000);  // 1 second delay
}
