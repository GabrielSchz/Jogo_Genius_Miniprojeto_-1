#include <Arduino.h>
// Definicoes de LEDs e Botoes
#define Led_01 2
#define Led_02 3
#define Led_03 4
#define Led_04 5
const int debounce_delay = 50;
int pinLed[] = {Led_01, Led_02, Led_03, Led_04};
int sequencia[10];          
int nivel = 1;

// Definicao dos segmentos do display
const byte digitos[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 0, 0, 1, 1}   // 9
};
const int segmentos[7] = {13, 12, 11, 10, 9, 8, 7}; // Pinos conectados ao display

                                //ESTRUTURA PARA O DEBOUNCE DO BOTAO
//////////////////////////////////////////////////////////////////////////////////////////////
struct sBotao {
  uint8_t pino;
  int ultimo_estado;
  int estado_atual;
  unsigned long ultima_mudanca;
};

typedef struct sBotao Botao;
Botao botao1, botao2, botao3, botao4;
int ler_botao_pressionado(Botao* botao);
void inicializar_Botao(Botao* botao, uint8_t pino_botao);

void inicializar_Botao(Botao* botao, uint8_t pino_botao) {
  botao->pino = pino_botao;
  botao->ultimo_estado = HIGH;
  botao->estado_atual = HIGH;
  botao->ultima_mudanca = 0;
  pinMode(botao->pino, INPUT_PULLUP); // resistor interno
}

int ler_botao_pressionado(Botao* botao) {
  int leitura = digitalRead(botao->pino);
  if (leitura != botao->ultimo_estado) {
    botao->ultima_mudanca = millis();
  }
  if ((millis() - botao->ultima_mudanca) > debounce_delay) {
    if (leitura != botao->estado_atual) {
      botao->estado_atual = leitura;
      if (botao->estado_atual == LOW) {
        return 1;
      }
    }
  }
  botao->ultimo_estado = leitura;
  return 0;
}

                                //ESTRUTURA PARA COMPARAR ENTRADA COM SEQUENCIA
//////////////////////////////////////////////////////////////////////////////////////////////////////

void piscarLed(int led) {
  digitalWrite(pinLed[led], HIGH);   //piscarLed
  delay(500);
  digitalWrite(pinLed[led], LOW);
  delay(250);
}

void mostrarSequencia(int nivel) {
  Serial.print("Mostrando sequencia ate o nivel ");     //MostrarSequencia
  Serial.println(nivel);
  for (int i = 0; i < nivel; i++) {
    piscarLed(sequencia[i]);
  }
}

int capturarEntrada() {
  if (ler_botao_pressionado(&botao1)) return 0;
  if (ler_botao_pressionado(&botao2)) return 1;     // Captura botao precionado
  if (ler_botao_pressionado(&botao3)) return 2;
  if (ler_botao_pressionado(&botao4)) return 3;
  return -1;
}

bool verificarEntrada(int nivel) {
  Serial.println("Insira a sequencia:");
  for (int i = 0; i < nivel; i++) {
    int entrada = -1;
    while (entrada == -1) {             // Aguarda até que uma entrada válida seja detectada
      entrada = capturarEntrada();
    }
    if (entrada != sequencia[i]) {
      return false;                     // Entrada != Sequência
    }
  }
  return true;                          // Entrada == Sequência
}

                            //FUNCAO PARA EXIBIR NUMERO NO DISPLAY
///////////////////////////////////////////////////////////////////////////////////////////
void exibirNumero(int numero) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentos[i], digitos[numero][i]);
  }
}

                            //FUNCAO PARA GERAR SEQUENCIA ALEATORIA
///////////////////////////////////////////////////////////////////////////////////////////
void gerarSequenciaAleatoria() {
  for (int i = 0; i < 10; i++) {
    sequencia[i] = random(4); // Gera valores de 0 a 3
  }
  Serial.println("Nova sequencia gerada!");
}

                                //MAIN
//////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A5));

  inicializar_Botao(&botao1, A0);
  inicializar_Botao(&botao2, A1);           //Botao
  inicializar_Botao(&botao3, A2);
  inicializar_Botao(&botao4, A3);

  for (int i = 0; i < 4; i++) {
    pinMode(pinLed[i], OUTPUT);        // Led
    digitalWrite(pinLed[i], LOW);
  }

  for (int i = 0; i < 7; i++) {
    pinMode(segmentos[i], OUTPUT);      //Display 
    digitalWrite(segmentos[i], LOW);
  }

  gerarSequenciaAleatoria(); // Gera a sequência inicial
  Serial.println("Bem-Vindo ao Genius! PREPARESSE!!");
}

void loop() {
  exibirNumero(nivel);
  mostrarSequencia(nivel);

  if (verificarEntrada(nivel)) {         // Condicao so vai ser valida quando a entrada for igual a Sequencia.
    Serial.println("Correto! Avancando para o proximo nivel.");
    nivel++;                                                    // Sequencia Correta -->
    if (nivel > 9) {
      Serial.println("Parabens! Voce completou o jogo!");
      while (true);
    }
  } else {
    Serial.println("Erro! Jogo reiniciando.");
    delay(5000);
    nivel = 1;
    gerarSequenciaAleatoria(); // Nova sequência ao reiniciar
  }
}