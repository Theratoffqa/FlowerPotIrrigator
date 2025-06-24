#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#include "mqtt_secrets.h"
#include "DHT.h"

#define DHTPIN 27       // Pin donde está el DHT22
#define DHTTYPE DHT22   // Tipo de sensor

DHT dht(DHTPIN, DHTTYPE);

const int humsuelo = 33; // Pin ADC conectado a A0 del HW-080
const int ledPin = 14;       // Pin donde está el LED
int valHumsuelo;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(humsuelo, INPUT);
  pinMode(ledPin, OUTPUT);
  dht.begin();
  Serial.println("Iniciando sensor DHT22...");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado al WiFi.");
}

void loop() {
  //FC28
  valHumsuelo = map(analogRead(humsuelo), 4092, 0, 0, 100);

  Serial.print("Humedad del suelo: ");
  Serial.print(valHumsuelo);
  Serial.println(" %");

  //DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer del sensor DHT22");
    return;
  }

  Serial.print("Humedad del aire: ");
  Serial.print(h);
  Serial.println(" %");

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C");

  //HTTP
  if (WiFi.status() == WL_CONNECTED) {
     HTTPClient httpSend;
    // Construir la URL de publicación
    String url = "http://api.thingspeak.com/update?api_key=" + String(WRITE_API_KEY) + 
                  "&field1=" + String(valHumsuelo) +
                  "&field2=" + String(h) +
                  "&field3=" + String(t);

    // Inicializar solicitud HTTP
    httpSend.begin(url); // ← Usamos la URL completa para enviar datos por GET
    int httpResponseCode = httpSend.GET(); // ← Ejecuta la solicitud GET

    // Revisar la respuesta
    if (httpResponseCode > 0) {
      Serial.print("Código de respuesta: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error al enviar datos. Código: ");
      Serial.println(httpResponseCode);
    }
    httpSend.end();

    HTTPClient httpRead;
    String readUrl= "https://api.thingspeak.com/channels/"+ String(CHANNEL_ID) + 
                    "/fields/4/last.txt?api_key="+ String(READ_API_KEY);
    httpRead.begin(readUrl);
    int httpCode = httpRead.GET();

    if (httpCode == 200) {
      String payload = httpRead.getString();
      int comando = payload.toInt();
      Serial.println("Valor recibido desde ThingSpeak (field4): " + String(comando));

      if (comando == 1) {
        digitalWrite(ledPin, HIGH); // Enciende el LED
      } else {
        digitalWrite(ledPin, LOW);  // Apaga el LED
      }
    } else {
      Serial.print("Error al obtener el valor. Código HTTP: ");
      Serial.println(httpCode);
    }

    httpRead.end(); // Liberar recursos

  } else {
    Serial.println("WiFi no conectado. Intentando reconectar...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  delay(45000); // Esperar 15 segundos para cumplir con los límites de ThingSpeak
}