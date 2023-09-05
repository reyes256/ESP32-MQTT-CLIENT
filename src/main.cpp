#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

const char ssid[]    = "";
const char password[] = "";

const char server[] = "broker.emqx.io";
const int port = 1883;

int payload_int = 0;
String payload_string = "";

const int LED = 2;

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
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");

  // Convert payload into integer
  char payload_array[length + 1];
  int resultI;

  memcpy(payload_array, payload, length);
  payload_array[length] = '\0';
  resultI = atoi(payload_array);
  
  payload_int = resultI;

  Serial.println(payload_int);
  
  // Convert payload into string
  // payload_string = "";

  // for (int i=0;i<length;i++) {
  //   payload_string = payload_string + (char)payload[i];
  // }
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.println("\nIniciando conexion con el Broker...");

    if (mqttClient.connect("clientid123")) {
      Serial.println("Conexion al Broker establecida.");

      if(mqttClient.subscribe("led/status")){
        Serial.println("Suscripción exitosa\n");
      }else{
        Serial.println("falló Suscripción");
      }

    } else {
      Serial.print("Fallo, rc=");
      Serial.print(mqttClient.state());

      Serial.println("\nintentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(921600);
  pinMode(LED,OUTPUT);

  delay(10);

  wifiInit();
  
  mqttClient.setServer(server, port);
  mqttClient.setCallback(callback);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.loop();

  if(payload_int == 0) {
    digitalWrite(LED,LOW);
  } else if (payload_int == 1) {
    digitalWrite(LED,HIGH);
  } else {
    Serial.println("Instrucción no reconocida");
  }

  delay(2500);
}