// pROJECT bY kARTIK mADOTRA
#define BLYNK_PRINT Serial   
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int sense_Pin = 0; // for soil moisture analog pin
int pin=0;

char auth[] ="zHcONA0GzZDuodUcrP7QBI6aGy6Qh7c9";  
char ssid[] = "Qwerty";  
char pass[] = "qwerty!@#$";  
int soil_Moisture;

int relay= 12;
#define DHTPIN 4
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);
// float h = dht.readHumidity();
//   float t = dht.readTemperature();
void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
   Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());  
 //WIFI SETUP OK!
   pinMode(relay, OUTPUT);
Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);  
  dht.begin();
  Serial.begin(9600);

   sensors.begin();


  // if (isnan(h) || isnan(t)) {
  //   Blynk.notify("Failed to read from DHT sensor!");
  //   return;
  // }

}
float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

void sendTemps()
{
Serial.print("MOISTURE LEVEL : ");
   soil_Moisture= analogRead(sense_Pin);
   soil_Moisture= soil_Moisture/10;
   Serial.println(soil_Moisture);
sensors.requestTemperatures();
float temp = t/25.6*10; 
Blynk.virtualWrite(V1, temp);
Blynk.virtualWrite(V2,soil_Moisture);
Blynk.virtualWrite(V0,pin);

  
   
}
void DHT() 
{
   
float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Blynk.virtualWrite(V5, h);  
 Blynk.virtualWrite(V6, t/25.6*10); 
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));   
}


void loop()
{  
  DHT();
  Blynk.run(); 
  sendTemps();
 

  
if (soil_Moisture>45) {
  Blynk.notify("Watering Your Plants");
  digitalWrite(relay,LOW);
   pin = 1;
  delay(100);    
  } 
  else {
   
    pin = 0;
 digitalWrite(relay,HIGH);
  Serial.print("Water Start Flowing ");

    delay(100);
  }
  delay(100);
}
