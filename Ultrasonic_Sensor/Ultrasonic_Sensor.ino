#include <WiFi.h> 
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ThingSpeak.h>


char ssid[] = "Samsung S10E";
char pass[] = "mncj8576";
int status = WL_IDLE_STATUS;
unsigned long myChannelNumber = 1655466;
const char * myWriteAPIKey = "UIURAROPIHAWQ65Z";
WiFiClient client;
 
int trigPin = 2;    // Trigger
int echoPin = 5;    // Echo
int ledPin = 18;
long duration, cm, inches;
 
void setup() {
  //Serial Port begin
  Serial.begin (115200);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.print("Trying to Connect with ");
 Serial.println(ssid);
 WiFi.begin(ssid,pass);
 while (WiFi.status() != WL_CONNECTED){
 delay(500);
 Serial.print(".");
}

Serial.println(" ");
 Serial.println("WiFi connected");
 ThingSpeak.begin(client); // Initialize ThingSpeak
 
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration*0.0343)/2;     // Divide by 29.1 or multiply by 0.0343
 
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(250);

  //Turn on light
if (cm < 15) {
  digitalWrite (ledPin, LOW);  // turn on the LED
}
else {
  digitalWrite (ledPin, HIGH); // turn off the LED
}

ThingSpeak.setField(1, long(cm));
 int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 if (x == 200) {
 Serial.println("Channel update successful. ");
 }
 else {
 Serial.println("Problem updating channel. HTTP error code " + 
 String(x));
 }
 delay(1000);
}
