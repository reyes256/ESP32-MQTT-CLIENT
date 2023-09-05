#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

const char ssid[]    = "";
const char password[] = "";

const char server[] = "broker.emqx.io";
const int port = 1883;

char ip[] = "";

void wifiInit() {
  Serial.print("\n\nConectándose a ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);  
  }

  Serial.println("\n\nConexion Establecida");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Serial.print("Mensaje recibido [");
  // Serial.print(topic);
  // Serial.print("] ");

  // char payload_string[length + 1];
  
  // int resultI;

  // memcpy(payload_string, payload, length);
  // payload_string[length] = '\0';
  // resultI = atoi(payload_string);
  
  // var = resultI;

  // resultS = "";
  
  // for (int i=0;i<length;i++) {
  //   resultS= resultS + (char)payload[i];
  // }
  // Serial.println();
}

void setup() {
  Serial.begin(921600);
  pinMode(2,OUTPUT);

  delay(10);

  wifiInit();
  
  mqttClient.setServer(server, port);
  mqttClient.setCallback(callback);
}

void loop() {

}