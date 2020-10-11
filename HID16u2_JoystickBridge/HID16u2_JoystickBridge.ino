/*
 * Converte mensagens recebidas do ArduinoUno para comandos do Joystick
 * As mensagens devem ser recebidas no formato <012345>.
 * O primeiro valor é referente a um botão.
 * <100000> indica que o primeiro botão deve ser apertado.
 * O segundo valor é referente ao segundo botão.
 * <010000> indica que o segundo valor deve ser apertado.
 * O terceiro valor indica a direção do potenciômetro:
 * 0 = neutro
 * 1 = esquerda
 * 2 = direita
 * Os últimos 3 valores indicam o valor do potenciômetro.
 * <000000> indica que o potenciômetro está na posição neutra.
 * <001512> indica que o potenciômetro está apontando para esquerda.
 * <002512> indica que o potenciômetro está apontando para a direita.
*/

#include "HID-Project.h"

const char simboloInicial = '<';
const char simboloFinal = '>';

const String valorInicial = "000000";
const int tamanhoMaximo = 7; // tamanho real é isso menos 1
char mensagem[tamanhoMaximo];

boolean recebendo = false;
boolean mensagemPronta = false;
int indice = 0; // posição atual sendo recebida
    
/**
 * Volta mensagem para o valor inicial
 */
void resetaMensagem() {
    valorInicial.toCharArray(mensagem, tamanhoMaximo);
}

void setup() {
  // Inicia a comunicação com o Arduino (ATMega)
  Serial1.begin(115200);
  // Serial.begin(9600); // debug
  resetaMensagem();

  // Reseta o joystick
  Gamepad.begin();
}

/**
 * Interpreta a mensagem em termos de 
 * ações para o Joystick
 */
void parse() {
    // Serial.println(mensagem);
    // se o primeiro valor for 1, aperta botão 1
    if (mensagem[0] == '1') {
        Gamepad.press(1);
    } else if (mensagem[0] == '0') {
        Gamepad.release(1);
    }

    // se o segundo valor for 1, aperta botão 1
    if (mensagem[1] == '1') {
        Gamepad.press(2);
    } else if (mensagem[1] == '0') {
        Gamepad.release(2);
    }

    char valorX[3];
    valorX[0] = mensagem[3];
    valorX[1] = mensagem[4];
    valorX[2] = mensagem[5];
    int16_t valorXint = atoi(valorX);
    
    if (mensagem[2] == '1') {
        valorXint = 0 - valorXint;
    }
    valorXint = valorXint * 50;

    // Serial.println(valorXint);
    Gamepad.xAxis(valorXint);

    // efetiva a ação definida
    Gamepad.write();
}

void loop() {

    char recebido; // guarda um caractere recebido via serial
   
    while (Serial1.available() > 0) {
        recebido = Serial1.read();
        if (recebendo) {
            if (recebido == simboloInicial) {
                // aqui houve algum erro, o símbolo inicial não deveria
                // ser recebido, então inicia de novo.
                resetaMensagem();
                indice = 0;
            } else if (recebido == simboloFinal) {
                recebendo = false;
                mensagemPronta = true;
            } else {
                mensagem[indice] = (char) recebido;
                indice += 1; // aumenta 1 no índice
            }
        } else {
            if (recebido == simboloInicial) {
                resetaMensagem();
                indice = 0;
                recebendo = true;
            }
        }
        if (mensagemPronta) {
            parse();
            mensagemPronta = false;
        }
    }

}
