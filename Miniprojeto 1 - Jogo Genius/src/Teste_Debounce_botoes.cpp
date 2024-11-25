#include <Arduino.h>
#define Led_01 1
#define Led_02 2
#define Led_03 3
#define Led_04 4
const int debounce_delay = 50;

// Estrutura para representar um botão
struct sBotao {
  uint8_t pino;              // Pino ao qual o botão está conectado
  int ultimo_estado;         // Último estado do botão
  int estado_atual;          // Estado atual do botão
  unsigned long ultima_mudanca; // Última mudança de estado em milissegundos
};

typedef struct sBotao Botao;
int ler_botao_pressionado(Botao* botao);
void inicializar_Botao(Botao* botao, uint8_t pino_botao);

// Função para inicializar o botão
void inicializar_Botao(Botao* botao, uint8_t pino_botao) {
  botao->pino = pino_botao;
  botao->ultimo_estado = HIGH;
  botao->estado_atual = HIGH;
  botao->ultima_mudanca = 0;
  pinMode(botao->pino, INPUT_PULLUP);
}

// Função para leitura do botão com debounce
int ler_botao_pressionado(Botao* botao) {
  int leitura = digitalRead(botao->pino);

  if (leitura != botao->ultimo_estado) {
    botao->ultima_mudanca = millis();
  }

  if ((millis() - botao->ultima_mudanca) > debounce_delay) {
    if (leitura != botao->estado_atual) {
      botao->estado_atual = leitura;
      if (botao->estado_atual == LOW) {
        return 1; // Botão pressionado
      }
    }
  }

  botao->ultimo_estado = leitura;
  return 0; // Botão não pressionado
}

//////////////////////////////////////////////////////////////////////////////////////

Botao botao1, botao2, botao3, botao4; // Declaração global do botão
long randNumber;
int pinLed[] = {Led_01, Led_02, Led_03, Led_04};
bool estadoLed[] = {false, false, false, false};


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  int vetor[9];

  inicializar_Botao(&botao1, A0); // Inicializa o pino botão 
  inicializar_Botao(&botao2, A1);
  inicializar_Botao(&botao3, A2);
  inicializar_Botao(&botao4, A3);

  for(int i = 0; i < 4; i++){
  pinMode(pinLed(i), OUTPUT);   // Inicicializa pinos Led
  digitalWrite(pinLed[i], estadoLed[i]);
  }

}

void loop() {
  randNumber = random(4);

  for(int i = 0; i < 4; i++){
    if(randNumber == i){
      digitalWrite(pinLed[i], !estadoLed[i]);
      Serial.println("numero Aleatório: ", randNumber);
    }

  }

    if (ler_botao_pressionado(&botao1)) { // Verifica se o botão 01 foi pressionado
    Serial.println("Botão 01 pressionado!");
    }
    if (ler_botao_pressionado(&botao2)) { // Verifica se o botão 02 foi pressionado
    Serial.println("Botão 02 pressionado!");
    }
    if (ler_botao_pressionado(&botao3)) { // Verifica se o botão 03 foi pressionado
    Serial.println("Botão 03 pressionado!");
    }
    if (ler_botao_pressionado(&botao4)) { // Verifica se o botão 04 foi pressionado
    Serial.println("Botão 04 pressionado!");
    }
}
