#include <Servo.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTTYPE DHT22 
#define DHTPIN 13 // D7 which is GPIO13 for NodeMCU

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

char ssid[] = "Samsung S10E";
char pass[] = "mncj8576";
int status = WL_IDLE_STATUS;
unsigned long myChannelNumber = 1640395;
const char * myWriteAPIKey = "7VMW1UMNFHT1PVGE";
WiFiClient client;

int RED = 2;
int servoPin = 14;

Servo servo;
int servoAngle = 0;

//Initiate DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// variables for storing data
float Temperature;
float Humidity;

void setup() {
 pinMode(RED, OUTPUT);
 Serial.begin(115200);
 delay(100);
 dht.begin();
 servo.attach(servoPin);
 
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
 Temperature = dht.readTemperature(); // Get the values of the temperature
 Humidity = dht.readHumidity(); // Get the values of the humidity
 
 // Printing the results on the serial monitor
 Serial.print("Temperature = ");
 Serial.print(Temperature);
 Serial.print("o");
 Serial.println("C");
 Serial.print("Humidity = ");
 Serial.print(Humidity);
 Serial.println("%");
 Serial.println("");
 delay(4000);

//LED
 if (Temperature > 22){
 digitalWrite(RED,LOW);   // LED On
 delay (50);
 digitalWrite(RED,HIGH);  // LED Off
 delay (50);
 }
 else{
   digitalWrite(RED,HIGH);
 }

//SERVO MOTOR
 if (Temperature > 23){
  Serial.println("open");
  servo.write(180);
 }
 else{
  Serial.println("close");
  servo.write(0);
 }
 
 // Write to ThingSpeak channel
 ThingSpeak.setField(1, String(Temperature));
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
