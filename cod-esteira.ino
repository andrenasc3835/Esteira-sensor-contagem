#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>

// Define os pinos
const int potPin = A0;  // Pino analógico onde o potenciômetro está conectado
const int in1Pin = 9;   // Pino digital para IN1 no L298N
const int in2Pin = 10;  // Pino digital para IN2 no L298N
const int enPin = 5;    // Pino PWM para ENA no L298N

// Pinos do sensor ultrassônico
const int trigPin = 7;
const int echoPin = 6;

// Configuração do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variáveis para contagem
int itemCount = 0;
long duration;
int distance;
int previousDistance = 0;
const int thresholdDistance = 20; // Nova distância em cm para detectar passagem
const int hysteresis = 2; // Histerese para evitar flutuações

void setup() {
  // Define os pinos como saída ou entrada
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicializa a comunicação serial (opcional)
  Serial.begin(9600);

  // Inicializa o LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Itens: ");
  lcd.setCursor(0, 1);
  lcd.print(itemCount);
}

void loop() {
  // Lê o valor do potenciômetro (0 a 1023)
  int potValue = analogRead(potPin);
  
  // Converte o valor lido para um valor PWM (0 a 255)
  // Para limitar a 5.8V, usamos um fator de escala de aproximadamente 48.3% (5.8V / 12V = 0.483)
  int speed = map(potValue, 0, 1023, 0, 255) * 0.483;
  
  // Controla a direção do motor
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  
  // Controla a velocidade do motor
  analogWrite(enPin, speed);
  
  // Leitura do sensor ultrassônico
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Detecção de passagem de itens com histerese
  if (distance < thresholdDistance && previousDistance >= thresholdDistance + hysteresis) {
    itemCount++;
    lcd.setCursor(0, 1);
    lcd.print("                "); // Limpa a linha do LCD
    lcd.setCursor(0, 1);
    lcd.print(itemCount);
  }
  
  previousDistance = distance;
  // Pequeno atraso antes da próxima leitura
  delay(100);
}