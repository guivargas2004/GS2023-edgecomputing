//Inclusão da biblioteca e configuração do LCD e definição dos pino

#include <LiquidCrystal.h>

const int rs = 5;
const int enable = 4;
const int d4 = A4;
const int d5 = A5;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

const int redPinT = 7;
const int greenPinT = 6;

const int redPinL = 9;
const int greenPinL = 8;

const int redPinU = 11;
const int greenPinU = 10;

const int temperaturaPin = A0;
const int ldrPin = A1;
const int sensorUmidadePin = A2;

const int buzzerPin = 12;
const int redPinA = 13;

//Enumeração dos estados de exibição do display.

enum DisplayState {
  TEMPERATURE_STATE,
  LDR_STATE,
  HUMIDITY_STATE
};

DisplayState currentState = TEMPERATURE_STATE;

//Inicialização do display LCD e configuração dos pinos.

void setup() {
  Serial.begin(9600);

  pinMode(redPinT, OUTPUT);
  pinMode(greenPinT, OUTPUT);

  pinMode(redPinL, OUTPUT);
  pinMode(greenPinL, OUTPUT);

  pinMode(redPinU, OUTPUT);
  pinMode(greenPinU, OUTPUT);

  pinMode(buzzerPin, OUTPUT);
  pinMode(redPinA, OUTPUT);

  lcd.begin(16, 2);
}

//Leitura e manipulação dos valores dos sensores.

void loop() {
  int sensorValue = analogRead(temperaturaPin);
  float voltage = (sensorValue / 1023.0) * 5.0;
  float temperatura = (voltage - 0.5) * 100;

  int ldrValue = analogRead(ldrPin);

  int sensorUmidade = analogRead(sensorUmidadePin);
  int umidade = map(sensorUmidade, 0, 1023, 0, 100);

  //Definição dos valores das variáveis para as cores de cada leds.
  
  int redValueT = 0;
  int greenValueT = 0;

  if (temperatura < 20) {
    greenValueT = 255;
  } else if (temperatura >= 20 && temperatura <= 27) {
    redValueT = 255;
    greenValueT = 165;
  } else {
    redValueT = 255;
  }

  int redValueL = 0;
  int greenValueL = 0;
  

  if (ldrValue < 920) {
    greenValueL = 255;
  } else if (ldrValue >= 920 && ldrValue <= 950) {
    redValueL = 255;
    greenValueL = 255;
  } else {
    redValueL = 255;
  }

  int redValueU = 0;
  int greenValueU = 0;

  if (umidade < 33) {
    greenValueU = 255;
  } else if (umidade >= 33 && umidade <= 66) {
    redValueU = 255;
    greenValueU = 165;
  } else {
    redValueU = 255;
  }
  
  //Verificação de valores críticos que ativam
  //o buzzer, o LED vermelho e a mensagem no display LCD.

  if (temperatura > 25 && ldrValue > 950 && umidade > 66) {
    tone(buzzerPin, 100, 1000);
    digitalWrite(redPinA, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alerta! Valores");
    lcd.setCursor(0, 1);
    lcd.print("criticos!");
    delay(1000);
    digitalWrite(redPinA, LOW);
    delay(1000);
  }

  //Atribuição dos valores de cor para cada LED.
  
  analogWrite(redPinT, redValueT);
  analogWrite(greenPinT, greenValueT);

  analogWrite(redPinL, redValueL);
  analogWrite(greenPinL, greenValueL);

  analogWrite(redPinU, redValueU);
  analogWrite(greenPinU, greenValueU);

  //Atualização do estado do display.
  
  switch (currentState) {
    case TEMPERATURE_STATE:
      displayTemperature(temperatura);
      currentState = LDR_STATE;
      break;

    case LDR_STATE:
      displayLDR(ldrValue);
      currentState = HUMIDITY_STATE;
      break;

    case HUMIDITY_STATE:
      displayHumidity(umidade);
      currentState = TEMPERATURE_STATE;
      break;
  }

  delay(1000);
}

//Definição das funções para exibir os valores dos sensores.

void displayTemperature(float temperatura) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatura, 2);
  lcd.print("C");
}

void displayLDR(int ldrValue) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luminosidade: ");
  lcd.print(ldrValue);
}

void displayHumidity(int umidade) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(umidade);
  lcd.print("%");
}
