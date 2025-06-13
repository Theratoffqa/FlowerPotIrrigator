#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#include "mqtt_secrets.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado al WiFi.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
     HTTPClient http;
      int valor = random(30, 80);  // Valor simulado (puedes reemplazarlo por la lectura real)
    Serial.print("Enviando valor: ");
    Serial.println(valor);

    // Construir la URL de publicación
    String url = "http://api.thingspeak.com/update?api_key=" + String(WRITE_API_KEY) + "&field1=" + String(valor);

    // Inicializar solicitud HTTP
    http.begin(url); // ← Usamos la URL completa para enviar datos por GET
    int httpResponseCode = http.GET(); // ← Ejecuta la solicitud GET

    // Revisar la respuesta
    if (httpResponseCode > 0) {
      Serial.print("Código de respuesta: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error al enviar datos. Código: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Liberar recursos
  } else {
    Serial.println("WiFi no conectado. Intentando reconectar...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  delay(15000); // Esperar 15 segundos para cumplir con los límites de ThingSpeak
}


/*#include "DHT.h"

#define DHTPIN 27       // Cambia por el pin que estás usando
#define DHTTYPE DHT22   // Tipo de sensor

DHT dht(DHTPIN, DHTTYPE);

const int humsuelo = 33; // Pin ADC conectado a A0 del HW-080
const int ledPin = 14;       // Pin donde está el LED
int valHumsuelo;

void setup() {
  Serial.begin(115200);
  pinMode(humsuelo, INPUT);
  pinMode(ledPin, OUTPUT);
  dht.begin();
  Serial.println("Iniciando sensor DHT22...");
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

  // Umbral: Si la humedad es menor a 30%, encender LED
  if (valHumsuelo < 30) {
    digitalWrite(ledPin, HIGH);  // Encender LED
  } else {
    digitalWrite(ledPin, LOW);   // Apagar LED
  }
  
  delay(2000); // Lee cada 2 segundos
  
}
*/