#include <Wire.h> //BIBLIOTECA P USAR O I2C
#include <LiquidCrystal_I2C.h> // BIBLIOTECA DO I2C

#define col 16
#define lin 2
#define ende 0x27

LiquidCrystal_I2C lcd(ende, col, lin); // INICIANDO O LED

const int pinoBuzzer = 13; // VAR BUZZER
const int pinoSensor = A0; // VAR SENSOR
const int leitura = 300; // VAR PPM
const int ledR = 6; // VARIÁVEL DO LED VERMELHOR
const int ledG = 7; // VARIÁVEL DO LED VERDE
const int botton = 3;  // VARIÁVEL DO BOTÃO

void setup() {
  Serial.begin(9600);
  pinMode(pinoSensor, INPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(botton, INPUT);  // Defina o pino do botão como entrada

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  int sensor = analogRead(pinoSensor);
  int bnt = digitalRead(botton);  

  Serial.print("LEITURA SENSOR:");
  Serial.println(sensor);
  Serial.print("LEITURA BOTTON:");
  Serial.println(bnt);

  lcd.setCursor(0, 0);
  lcd.print("Estado:");

  // CONDIÇÃO DE PARADA 1
  if (sensor > leitura && bnt == HIGH) {
    digitalWrite(pinoBuzzer, LOW);
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);    
  if (bnt == HIGH) { // CONTAGEM REGRESSIVA
    for (int i = 15; i >= 1; i--) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Contagem: ");
      lcd.setCursor(0, 1);
      lcd.print(i);
      delay(1000);
      lcd.clear();
    }
  }
  // DETECTAR SEM A CONTAGEM
  } else if (sensor > leitura) {
    digitalWrite(pinoBuzzer, HIGH);
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Gas Detectado");

// CONSEQUÊNCIA
  } else {
    digitalWrite(pinoBuzzer, LOW);
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Livre de Gas");
  }
}
