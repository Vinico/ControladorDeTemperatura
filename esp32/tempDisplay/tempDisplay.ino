#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>

// --- Pinos do Sensor DS18B20 ---
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// --- Pinos do Display TM1637 ---
const int CLK = 22;
const int DIO = 23;
TM1637Display display(CLK, DIO);

// Criação dos segmentos para o símbolo de Grau (°) e a letra 'C'
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Desenha a bolinha do Grau (°)
  SEG_A | SEG_D | SEG_E | SEG_F   // Desenha a letra 'C'
};

void setup() {
  // Inicia o sensor
  sensors.begin();
  
  // Inicia o display e define o brilho máximo (0x00 é o mínimo, 0x0f é o máximo)
  display.setBrightness(0x0f); 
}

void loop() {
  // Pede a leitura do sensor
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  
  // Arredonda a temperatura para um número inteiro (ex: 24.7 vira 25)
  int tempInteira = round(tempC);

  // Mostra o número da temperatura nos dois primeiros dígitos do display
  // false = não colocar zeros à esquerda (mostra " 8" em vez de "08")
  display.showNumberDec(tempInteira, false, 2, 0);
  
  // Desenha os símbolos "°C" nos dois últimos espaços do display
  display.setSegments(celsius, 2, 2);
  
  // O chip do TM1637 tem memória própria, então o delay() não apaga a tela!
  delay(2000); 
}