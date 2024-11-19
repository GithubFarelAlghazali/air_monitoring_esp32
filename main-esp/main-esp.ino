#include <DHT.h>
#include <WiFi.h>
#include <WebServer.h>
#include <string>
#include <ArduinoJson.h>

DHT dht(23, DHT22);
WebServer server(80)

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

int espLed = 2;
int buzzer = 22;
int sensor_in = 13;
int ledDingin= 18;
int ledNormal= 19;
int ledPanas= 21;


void setup() {
  Serial.begin(9600);

  pinMode(espLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledDingin, OUTPUT);
  pinMode(ledNormal, OUTPUT);
  pinMode(ledPanas, OUTPUT);

  digitalWrite(ledDingin, LOW);
  digitalWrite(ledNormal, LOW);
  digitalWrite(ledPanas, LOW);

  dht.begin();
  delay(2000);

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi...");

  while(WiFi.status() != WL_CONNECTED){
    digitalWrite(espLed, LOW);
    delay(1000);
    Serial.print(".");
  }
  digitalWrite(espLed, HIGH);
  Serial.println("");
  Serial.println("Terhubung ke WiFi");
  Serial.println(WiFi.localIP());

  server.on("/suhu", HTTP_GET, getSuhu);   
  server.on("/gas", HTTP_GET, getGas);   
  server.begin();
}

void loop() {

  float suhu = dht.readTemperature();

  if(suhu < 25){
    digitalWrite(ledDingin, HIGH);
    digitalWrite(ledNormal, LOW);
    digitalWrite(ledPanas, LOW);
  } else if(suhu > 25 && suhu < 31){
    digitalWrite(ledDingin, LOW);
    digitalWrite(ledNormal, HIGH);
    digitalWrite(ledPanas, LOW);
  } else if(suhu > 31){
    digitalWrite(ledDingin, LOW);
    digitalWrite(ledNormal, LOW);
    digitalWrite(ledPanas, HIGH);
  }
  
  int sensor_Dout = digitalRead(sensor_in);
  if (sensor_Dout == HIGH){
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledPanas, HIGH);
    delay(1000);
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledPanas, LOW);
    delay(1000);
  }
  else{
    digitalWrite(buzzer, LOW);
  }
  
  server.handleClient();

}

void getSuhu(){
  float suhu = dht.readTemperature();
  float lembap = dht.readHumidity();

  
  DynamicJsonDocument doc(100);
  doc["suhu"] = suhu;
  doc["lembap"] = lembap;

  String jsonString;
  serializeJson(doc, jsonString);

   // Send the JSON response
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", jsonString);

}
void getGas(){
  int sensor_Dout = digitalRead(sensor_in);

  DynamicJsonDocument doc(100);
  // doc["gas"] = 1;
  doc["gas"] = sensor_Dout;
  
  String jsonString;
  serializeJson(doc, jsonString);

   // Send the JSON response
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", jsonString);

}
