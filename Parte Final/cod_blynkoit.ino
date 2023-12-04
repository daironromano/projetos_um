#include <ESP8266WiFi.h> //BIBLIOTECA WIFI ESP
#include <BlynkSimpleEsp8266.h> //BIBLIOTECA BLYNK
#include <Wire.h> //BIBLIOTECA P USAR O I2C
#include <LiquidCrystal_I2C.h> // BIBLIOTECA DO I2C

#define col 16
#define lin 2
#define ende 0x27

#define BLYNK_TEMPLATE_ID "TMPL2IXzeMiKo"
#define BLYNK_TEMPLATE_NAME "sensor de gas esp8266"
#define BLYNK_AUTH_TOKEN "q3OJG3BVAOydMW0mLGsBoGpoHSsSGlgL"
#define BLYNK_PRINT Serial 

LiquidCrystal_I2C lcd(ende, col, lin); // INICIANDO O LED

const int pinoBuzzer = D7; // VAR BUZZER
const int pinoSensor = A0; // VAR SENSOR
const int leitura = 300; // VAR PPM
const int ledR = D5; // VARIÁVEL DO LED VERMELHOR
const int ledG = D4; // VARIÁVEL DO LED VERDE
const int botton = D6;  // VARIÁVEL DO BOTÃO


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = ".......";  // NOME DA REDE DE WIFI
char pass[] = ".......";  // PASSWORD DO WIFI
int data = 0;
 

BlynkTimer timer; 
void sendSensor(){ 
  int data = analogRead(pinoSensor);
  int sensor = analogRead(pinoSensor);
  Blynk.virtualWrite(V0, data);
  Serial.print("Pin A0: ");
  Serial.println(data);

  if (sensor > leitura) {
    Serial.println("Fire in the House");  
    
    Blynk.logEvent("alerta_gas","Fire Detected");
  }
}

void setup() {
 Blynk.begin(auth, ssid, pass);
  //dht.begin();
  timer.setInterval(5000L, sendSensor);
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
  Blynk.run();
  timer.run();
  int sensor = analogRead(pinoSensor);
  int bnt = digitalRead(botton);  
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