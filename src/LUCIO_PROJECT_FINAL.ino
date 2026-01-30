/**
 * PROJETO: Pistola Musical - Amplificador Sônico (Lúcio - Overwatch)
 * AUTORA: Manuela Nunes Simião
 * INSTITUIÇÃO: UFPA - Campus Castanhal
 * DESCRIÇÃO: Sistema unificado de áudio e iluminação para Arduino Mega 2560.
 * DATA: 2025
 */

#include <Adafruit_NeoPixel.h>          // Biblioteca para controle de LEDs WS2812
#include <DFRobotDFPlayerMini.h>        // Biblioteca DFPlayer Mini

// -------------------- LUZ --------------------
#define SWITCH_POWER 2                 // Chave liga/desliga LEDs
#define SWITCH_COLOR 3                 // Chave troca cor (verde <-> amarelo)
#define LED_PIN 9                      // Pino de saída dos LEDs
#define NUM_LEDS 36                    // Quantidade de LEDs
#define POT_PIN A4                     // Potenciômetro no A4

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // Objeto da fita de LEDs

// Estado inicial do sistema
bool corVerde = true;                  // Flag para cor atual: verde/amarelo
bool ultimaLeituraChave = false;       // Última leitura do switch de cor
unsigned long beatInterval = 468;      // Intervalo do beat para efeitos (128 BPM)

// Grupos de LEDs para efeito em ondas
int grupos[3][2] = { {0, 8}, {9, 27}, {28, 35} }; // Definição dos grupos de LEDs para onda

// Cor base do amarelo (mantém proporção mesmo em brilho baixo)
const uint8_t AMARELO_R = 255;         // Componente vermelho do amarelo
const uint8_t AMARELO_G = 191;         // Componente verde do amarelo
const uint8_t AMARELO_B = 0;           // Componente azul do amarelo (0)

// ---- Função para leitura do potenciômetro com correção gama ----
int brilhoCorrigido() {
  int leitura = analogRead(POT_PIN);   // Lê valor do potenciômetro (0 a 1023)
  float normalizado = leitura / 1023.0; // Normaliza para 0.0 a 1.0
  float corrigido = pow(normalizado, 2.2); // Aplica correção gama (2.2)
  return (int)(corrigido * 127);       // Retorna brilho máximo 50% (0 a 127)
}

// ---- Efeito verde pulsante ----
void efeitoVerde() {
  unsigned long now = millis();                                // Tempo atual em ms
  float phase = fmod((now % beatInterval) / (float)beatInterval, 1.0); // Calcula fase do beat

  float brightness = sin(phase * PI);                          // Pulso senoidal (0 a 1)
  if (brightness < 0) brightness = 0;                          // Garante que não seja negativo

  int potValue = brilhoCorrigido();                            // Brilho ajustado pelo potenciômetro
  uint8_t value = (uint8_t)(brightness * potValue);           // Valor final do brilho

  for (int i = 0; i < NUM_LEDS; i++) {                        // Aplica brilho verde a todos LEDs
    strip.setPixelColor(i, 0, value, 0);                       // Configura cor RGB (verde)
  }
  strip.show();                                                // Atualiza LEDs
}

// ---- Efeito amarelo em ondas suaves ----
void efeitoAmarelo() {
  unsigned long now = millis();                                // Tempo atual em ms
  float beatPhase = fmod((now % beatInterval) / (float)beatInterval, 1.0); // Fase do beat

  strip.clear();                                               // Limpa LEDs
  int potValue = brilhoCorrigido();                            // Brilho do potenciômetro
  float fatorPot = potValue / 127.0;                           // Normaliza para 0.0 a 1.0

  for (int g = 0; g < 3; g++) {                                // Itera pelos 3 grupos de LEDs
    int start = grupos[g][0];                                   // Início do grupo
    int end = grupos[g][1];                                     // Fim do grupo
    int count = end - start + 1;                                 // Quantidade de LEDs no grupo

    for (int i = 0; i < count; i++) {                           // Itera LEDs do grupo
      float localPhase = ((float)i / count) - beatPhase;         // Calcula fase local
      if (localPhase < 0) localPhase += 1.0;                    // Corrige se negativo

      float brightness = 0.2 + 0.8 * sin(localPhase * PI);       // Pulso suave 0.2 a 1.0
      if (brightness > 1.0) brightness = 1.0;                   // Limita máximo

      uint8_t r = (uint8_t)(AMARELO_R * brightness * fatorPot); // Calcula R
      uint8_t g = (uint8_t)(AMARELO_G * brightness * fatorPot); // Calcula G
      uint8_t b = (uint8_t)(AMARELO_B * brightness * fatorPot); // Calcula B

      strip.setPixelColor(start + i, r, g, b);                   // Aplica cor ao LED
    }
  }
  strip.show();                                                // Atualiza LEDs
}

// -------------------- SOM PRINCIPAL --------------------
const int chavePin = 2;                                         // Chave liga/desliga DFPlayer
const int switchPin = 3;                                        // Alterna músicas
const int potPin   = A4;                                        // Potenciômetro para volume música

DFRobotDFPlayerMini playerMusica;                               // Objeto DFPlayer música

bool sistemaLigado = false;                                     // Flag sistema ligado
int musicaAtual = 1;                                            // Música atual (1 ou 2)
int lastSwitchState = HIGH;                                      // Último estado do switch

// -------------------- FX --------------------
const int botao1 = A0;                                           // Botão 1 FX
const int botao2 = A1;                                           // Botão 2 FX
const int botao3 = A2;                                           // Botão 3 FX

DFRobotDFPlayerMini playerFX;                                    // Objeto DFPlayer FX

bool lastB1 = HIGH;                                              // Último estado botão 1
bool lastB2 = HIGH;                                              // Último estado botão 2
bool lastB3 = HIGH;                                              // Último estado botão 3

void setup() {
  // --- Configuração LEDs ---
  pinMode(SWITCH_POWER, INPUT_PULLUP);                           // Configura pino switch de power
  pinMode(SWITCH_COLOR, INPUT_PULLUP);                           // Configura pino switch de cor
  strip.begin();                                                  // Inicializa LEDs
  strip.show();                                                   // Limpa LEDs
  corVerde = true;                                                // Inicializa cor verde
  ultimaLeituraChave = digitalRead(SWITCH_COLOR);                // Salva estado inicial switch

  // --- Configuração botões ---
  pinMode(chavePin, INPUT_PULLUP);                                // Chave liga/desliga música
  pinMode(switchPin, INPUT_PULLUP);                               // Switch alterna música
  pinMode(botao1, INPUT_PULLUP);                                  // Botão FX 1
  pinMode(botao2, INPUT_PULLUP);                                  // Botão FX 2
  pinMode(botao3, INPUT_PULLUP);                                  // Botão FX 3

  // --- Serial / DFPlayer Música ---
  Serial.begin(9600);                                             // Serial monitor
  delay(500);                                                     // Aguarda Serial iniciar
  Serial2.begin(9600);                                            // DFPlayer Música (pinos 16/17)
  delay(500);                                                     // Aguarda DFPlayer Música iniciar
  if (!playerMusica.begin(Serial2, true)) {                       // Inicializa DFPlayer Música
    Serial.println("Erro ao iniciar DFPlayer Música.");           
    while(true);                                                  // Trava se falhar
  }
  playerMusica.volume(15);                                         // Define volume metade
  Serial.println("DFPlayer Música pronto.");                      // Mensagem sucesso

  // --- Serial / DFPlayer FX ---
  Serial1.begin(9600);                                            // DFPlayer FX (pinos 18/19)
  delay(500);                                                     // Aguarda inicialização
  if (!playerFX.begin(Serial1, true)) {                           // Inicializa DFPlayer FX
    Serial.println("Erro ao iniciar DFPlayer FX.");              
    while(true);                                                  // Trava se falhar
  }
  playerFX.volume(15);                                            // Define volume FX
  Serial.println("DFPlayer FX pronto.");                          // Mensagem sucesso
}

void loop() {
  int chaveState = digitalRead(chavePin);                          // Lê chave liga/desliga DFPlayer

  // -------------------- Liga/Desliga Sistema --------------------
  if (chaveState == LOW && !sistemaLigado) {                       // Se chave ON e sistema desligado
    sistemaLigado = true;                                          // Ativa sistema
    musicaAtual = 1;                                               // Define música inicial
    playerMusica.loop(musicaAtual);                                // Toca música 1 em loop
    lastSwitchState = digitalRead(switchPin);                      // Atualiza último estado do switch
    Serial.println("Sistema ligado: tocando música 1");            // Mensagem Serial
  }

  if (chaveState == HIGH && sistemaLigado) {                       // Se chave OFF e sistema ligado
    sistemaLigado = false;                                         // Desativa sistema
    playerMusica.stop();                                           // Para música
    Serial.println("Sistema desligado");                           // Mensagem Serial

    // --- Apaga LEDs ao desligar sistema ---
    strip.clear();                                                  // Limpa LEDs
    strip.show();                                                   // Atualiza LEDs
    corVerde = true;                                                // Reseta cor para verde
    ultimaLeituraChave = digitalRead(SWITCH_COLOR);                 // Atualiza estado switch
  }

  // -------------------- Se Sistema Ligado --------------------
  if (sistemaLigado) {
    // --- Alterna músicas ---
    int currentSwitchState = digitalRead(switchPin);               // Lê switch
    if (currentSwitchState != lastSwitchState) {                   // Detecta mudança
      delay(50);                                                   // Debounce simples
      if (currentSwitchState != lastSwitchState) {                 // Confirma mudança
        musicaAtual = (musicaAtual == 1) ? 2 : 1;                 // Alterna música
        playerMusica.loop(musicaAtual);                            // Loop música
        Serial.print("Trocando para música ");                     // Mensagem Serial
        Serial.println(musicaAtual);
        lastSwitchState = currentSwitchState;                      // Atualiza último estado
      }
    }

    // --- Ajusta volume música ---
    int potValue = analogRead(potPin);                             // Lê potenciômetro (0 a 1023)
    int volume = map(potValue, 0, 1023, 0, 15);                    // Mapeia para 0 a 15
    playerMusica.volume(volume);                                    // Define volume

    // --- FX botões ---
    bool b1 = digitalRead(botao1);                                  // Lê botão 1
    bool b2 = digitalRead(botao2);                                  // Lê botão 2
    bool b3 = digitalRead(botao3);                                  // Lê botão 3

    if (b1 == LOW && b2 == LOW && !(lastB1 == LOW && lastB2 == LOW)) { // B1+B2
      Serial.println("Botões 1+2 pressionados → som 7");            // Serial
      playerFX.play(7);                                             // Toca FX 7
    } else if (b1 == LOW && lastB1 == HIGH) {                       // Apenas B1
      int som = random(3,5);                                        // Sorteia som 3 ou 4
      Serial.print("Botão 1 pressionado → som ");                    // Serial
      Serial.println(som);
      playerFX.play(som);                                           // Toca FX
    } else if (b2 == LOW && lastB2 == HIGH) {                       // Apenas B2
      int som = random(5,7);                                        // Sorteia som 5 ou 6
      Serial.print("Botão 2 pressionado → som ");                    // Serial
      Serial.println(som);
      playerFX.play(som);                                           // Toca FX
    } else if (b3 == LOW && lastB3 == HIGH) {                       // Apenas B3
      Serial.println("Botão 3 pressionado → som 8");                // Serial
      playerFX.play(8);                                             // Toca FX
    }

    lastB1 = b1;                                                     // Atualiza estado B1
    lastB2 = b2;                                                     // Atualiza estado B2
    lastB3 = b3;                                                     // Atualiza estado B3

    // --- LEDs ---
    if (digitalRead(SWITCH_POWER) == LOW) {                          // Se LEDs ligados
      bool leituraChave = digitalRead(SWITCH_COLOR);                // Lê switch de cor
      if (leituraChave != ultimaLeituraChave) {                     // Detecta mudança
        corVerde = !corVerde;                                       // Alterna cor
        ultimaLeituraChave = leituraChave;                          // Atualiza estado
      }

      if (corVerde) efeitoVerde();                                   // Efeito verde
      else efeitoAmarelo();                                          // Efeito amarelo

    } else {                                                          // LEDs desligados
      strip.clear();                                                 // Limpa LEDs
      strip.show();                                                  // Atualiza LEDs
      corVerde = true;                                               // Reseta cor para verde
      ultimaLeituraChave = digitalRead(SWITCH_COLOR);               // Atualiza switch
    }
  }
}
