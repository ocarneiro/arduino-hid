/*
 * Envia caracteres para o segundo chip do arduino (16u2),
 * que deve rodar programa de ponte HID para repassar
 * os caracteres como comandos de joystick para
 * a porta USB.
 */

const int pinLed = LED_BUILTIN;
const int botaoVermelho = 3;
const int botaoVerde = 2;

const int potenciometro = A0;

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

    char valorBotaoVermelho = '0';
    if (digitalRead(botaoVermelho)) {
      valorBotaoVermelho = '1';
    }
    char valorBotaoVerde = '0';
    if (digitalRead(botaoVerde)) {
      valorBotaoVerde = '1';
    }

    char direcao = '0';
    int valorPotenciometro = analogRead(potenciometro);
    int valorDirecao = 0;
    if (valorPotenciometro > 512) {
      direcao = '2';
      valorDirecao = valorPotenciometro - 512;
    } else if (valorPotenciometro < 512) {
      direcao = '1';
      valorDirecao = 512 - valorPotenciometro;
    }
    // cria uma saída com zeros à esquerda
    char *saida = malloc(4);
    sprintf(saida, "%03d", valorDirecao);
    
    Serial.write("<");
    Serial.write(valorBotaoVermelho);
    Serial.write(valorBotaoVerde);
    Serial.write(direcao);
    Serial.write(saida);
    Serial.write(">");

    //
    delay(100);
    digitalWrite(pinLed, LOW);
    // Serial.println();
}
