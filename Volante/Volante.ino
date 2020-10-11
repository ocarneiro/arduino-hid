/*
 * Envia caracteres para o segundo chip do arduino (16u2),
 * que deve rodar programa de ponte HID para repassar
 * os caracteres como comandos de joystick para
 * a porta USB.
 */

const int pinLed = LED_BUILTIN;
const int botaoVermelho = 3;
const int botaoVerde = 2;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(botaoVermelho, INPUT);
  pinMode(botaoVerde, INPUT);

  Serial.begin(115200);

  // Para saber que é o meu código
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
  delay(200);  
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
}


void loop() {

    if (digitalRead(botaoVermelho)) {
      digitalWrite(pinLed, HIGH);
      Serial.write("<100000>");
    } else if (digitalRead(botaoVerde)) {
      digitalWrite(pinLed, HIGH);
      Serial.write("<010000>");
    } else {
      Serial.write("<000000>");
    }

    //
    delay(100);
    digitalWrite(pinLed, LOW);
}
