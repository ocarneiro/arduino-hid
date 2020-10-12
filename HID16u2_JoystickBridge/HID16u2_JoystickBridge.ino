/*
 * Converte mensagens recebidas do ArduinoUno para comandos do Joystick
 * As mensagens devem ser recebidas no formato <0123452345><bbdvvvdvvv>.
 * b = estado de um botão (0 = solto; 1 = pressionado)
 * d = direção horizontal (0 = neutro, 1 = esquerda, 2 = direita)
 *     ou vertical (0 = neutro, 1 = cima, 2 = baixo)
 * vvv = valor ou intensidade. 0 = mínimo, 512 = máximo.
 * Exemplos:
 * <0020271164> - botões soltos, direita 27, cima 164
 * <1111432325> - botões pressionados, esquerda 143, baixo 325
*/

#include "HID-Project.h"

const char simboloInicial = '<';
const char simboloFinal = '>';

const String valorInicial = "0000000000";
const int tamanhoMaximo = 11; // tamanho real é isso menos 1
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

    // se o segundo valor for 1, aperta botão 2
    if (mensagem[1] == '1') {
        Gamepad.press(2);
    } else if (mensagem[1] == '0') {
        Gamepad.release(2);
    }

    char valor[3];
    valor[0] = mensagem[3];
    valor[1] = mensagem[4];
    valor[2] = mensagem[5];
    int16_t valorInt = atoi(valor);
    
    if (mensagem[2] == '1') {
        valorInt = 0 - valorInt;
    }
    valorInt = valorInt * 50;
    Gamepad.xAxis(valorInt);

    valor[0] = mensagem[7];
    valor[1] = mensagem[8];
    valor[2] = mensagem[9];
    valorInt = atoi(valor);
    
    if (mensagem[6] == '1') {
        valorInt = 0 - valorInt;
    }
    valorInt = valorInt * 50;
    // Serial.println(valorInt);
    Gamepad.yAxis(valorInt);

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
