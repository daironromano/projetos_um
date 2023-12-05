#define BLYNK_TEMPLATE_ID "TMPL2IXzeMiKo"
#define BLYNK_TEMPLATE_NAME "sensor de gas esp8266"
#define BLYNK_AUTH_TOKEN "q3OJG3BVAOydMW0mLGsBoGpoHSsSGlgL"

#include <ESP8266WiFi.h> //BIBLIOTECA WIFI ESP
#include <BlynkSimpleEsp8266.h> //BIBLIOTECA BLYNK
#include <Wire.h> //BIBLIOTECA P USAR O I2C
#include <LiquidCrystal_I2C.h> // BIBLIOTECA DO I2C

#define col 16
#define lin 2
#define ende 0x27


#define BLYNK_PRINT Serial 
LiquidCrystal_I2C lcd(ende, col, lin); // INICIANDO O LED

const int pinoBuzzer = D7; // VAR BUZZER
const int pinoSensor = D8; // VAR SENSOR
const int leitura = 300; // VAR PPM
const int ledR = D5; // VARIÁVEL DO LED VERMELHOR
const int ledG = D4; // VARIÁVEL DO LED VERDE

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "motorola";  // NOME DA REDE DE WIFI
char pass[] = "12345678";  // PASSWORD DO WIFI
int data = 0;
 

BlynkTimer timer; 
void sendSensor(){ 
  int data = analogRead(pinoSensor);
  int sensor = analogRead(pinoSensor);
  Blynk.virtualWrite(V0, data);
  Serial.print("Pin A0: ");
  Serial.println(data);

  if (sensor > leitura) {
    Serial.println("Vazamento de Gás");     
    Blynk.logEvent("alerta_gas","Vazamento de Gás Detectado");
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  //dht.begin();
  timer.setInterval(5000L, sendSensor);
  pinMode(pinoSensor, INPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);


 lcd.init();
  lcd.backlight();
  lcd.clear();
}
void loop() {
  Blynk.run();
  timer.run();
  int sensor = analogRead(pinoSensor);
  Serial.print("LEITURA SENSOR:");
  Serial.println(sensor);  
  lcd.setCursor(0, 0);
  lcd.print("Estado:"); 
  
  // DETECTAR SEM A CONTAGEM
  if (sensor > leitura) {
    digitalWrite(pinoBuzzer, HIGH);
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Gas Detectado");
  }
// CONSEQUÊNCIA
   else {
    digitalWrite(pinoBuzzer, LOW);
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Livre de Gas  ");
  }
}
