#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>
#include "secrets.h"

// Utilize as constantes definidas no secrets.h
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* serverName = SECRET_SERVER;

// --- Pinos do Sensor DS18B20 ---
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// --- Pinos do Display TM1637 ---
const int CLK = 22;
const int DIO = 23;
TM1637Display display(CLK, DIO);

// Símbolos de Grau (°) e letra 'C'
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G, 
  SEG_A | SEG_D | SEG_E | SEG_F  
};

void setup() {
  Serial.begin(115200);

  // Inicia o sensor e o display
  sensors.begin();
  display.setBrightness(0x0f); 

  // Inicia a conexão Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConectado ao Wi-Fi com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 1. Leitura do Sensor
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  
  // 2. Atualização do Display TM1637
  int tempInteira = round(tempC);
  display.showNumberDec(tempInteira, false, 2, 0);
  display.setSegments(celsius, 2, 2);

  // 3. Envio dos Dados via JSON (HTTP POST)
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Monta o JSON: {"temperatura": 24.50}
    String jsonPayload = "{\"temperatura\":" + String(tempC, 2) + "}";

    // Dispara a requisição POST
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.print("JSON enviado! Código de resposta HTTP: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Erro ao enviar POST. Código: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Libera os recursos da conexão
  } else {
    Serial.println("Erro: Wi-Fi desconectado.");
  }

  // Intervalo de envio (ajustado para 10 segundos para não sobrecarregar o servidor)
  delay(10000); 
}