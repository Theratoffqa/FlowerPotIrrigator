#include "DHT.h"

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
