// Testando conversões de tipos
// Este código serve para testar no Serial Monitor se as mensagens
// estão sendo corretamente enviadas entre os componentes

const char simboloInicial = '<';
const char simboloFinal = '>';

const String valorInicial = "00000";
const int tamanhoMaximo = 6; // tamanho real é isso menos 1
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
    Serial.begin(9600);
    Serial.println("--- Arduino ligado ---");
    Serial.print("<x");
    int a = 123;
    Serial.print(a);
    Serial.println("y>");
    
    resetaMensagem();
}

void loop() {

    char recebido; // guarda um caractere recebido via serial
    
    while (Serial.available() > 0) {
        recebido = Serial.read();
        if (recebendo) {
            if (recebido == simboloFinal) {
                recebendo = false;
                mensagemPronta = true;
            } else {
                mensagem[indice] = recebido;
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
            Serial.println(mensagem);
            mensagemPronta = false;
        }
    }
}
