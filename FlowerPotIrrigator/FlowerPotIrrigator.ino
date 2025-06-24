#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include "secrets.h"              // Aquí defines WIFI_SSID, WIFI_PASS, WRITE_API_KEY
#include "decision_tree_model.h"  // Modelo exportado con micromlgen

#define DHTPIN 27                 // Pin donde está conectado el DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int humsueloPin = 33;       // Pin analógico conectado al sensor FC-28
const int ledPin = 14;            // Pin donde está conectado el LED (o relay)

Eloquent::ML::Port::DecisionTree modelo;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(humsueloPin, INPUT);
  pinMode(ledPin, OUTPUT);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✓ Conectado al WiFi");
}

void loop() {
  // Lectura de sensores reales
  int rawHumsuelo = analogRead(humsueloPin);
  float humedadSuelo = map(rawHumsuelo, 4095, 0, 0, 100); // Convertir lectura a %
  float temperatura = dht.readTemperature();
  float humedadAire = dht.readHumidity();

  // Validar lectura del DHT22
  if (isnan(temperatura) || isnan(humedadAire)) {
    Serial.println("✗ Error al leer el sensor DHT22");
    delay(5000);
    return;
  }

  // Predicción con el modelo
  float input[] = {humedadSuelo, temperatura, humedadAire};
  int decision = modelo.predict(input); // 1 = RIEGO, 0 = NO RIEGO
  if (humedadSuelo < 15) {
  decision = 1;
}

  // Control de LED
  if (decision == 1) {
    digitalWrite(ledPin, HIGH);  // 🔴 Activa riego
  } else {
    digitalWrite(ledPin, LOW);   // ⚪ No riego
  }

  // Mostrar en el monitor serie
  Serial.println("---- SENSOR EN TIEMPO REAL ----");
  Serial.print("H. suelo: "); Serial.print(humedadSuelo); Serial.print("% | ");
  Serial.print("Temp: "); Serial.print(temperatura); Serial.print("°C | ");
  Serial.print("H. aire: "); Serial.print(humedadAire); Serial.print("% → ");
  Serial.println(decision == 1 ? "RIEGO" : "NO RIEGO");

  // Enviar a ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + String(WRITE_API_KEY) +
                 "&field1=" + String(humedadSuelo) +
                 "&field2=" + String(humedadAire) +
                 "&field3=" + String(temperatura) +
                 "&field4=" + String(decision);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.print("✓ Datos enviados (HTTP "); Serial.print(httpCode); Serial.println(")");
    } else {
      Serial.print("✗ Error HTTP: "); Serial.println(httpCode);
    }
    http.end();
  } else {
    Serial.println("✗ WiFi desconectado");
  }

  delay(20000); // ThingSpeak requiere mínimo 15s entre lecturas
}
