![Lúcio](https://github.com/ManuelaNunes/lucio-sonic-amplifier-arduino/raw/main/media/photos/lucio.png)
# Pistola Sônica Musical (Inspirada no Lúcio - Overwatch)

Protótipo de um sistema embarcado que integra iluminação LED endereçável e reprodução de áudio sincronizada. O projeto simula as mecânicas de "cura" e "velocidade" do personagem Lúcio, utilizando hardware acessível para criar um dispositivo de alta fidelidade visual e sonora para aplicações em Wearable Tech e Cosplay.

## 🎯 Objetivo
Desenvolver um dispositivo de entretenimento funcional que demonstre a viabilidade de sistemas de áudio paralelo e sincronização rítmica de luzes em microcontroladores de 8 bits, voltado para a comunidade geek e artística.

## 📝 Tecnologias utilizadas
- **Microcontrolador:** Arduino Mega 2560
- **Linguagem:** C/C++ (Arduino Framework)
- **Bibliotecas:** Adafruit_NeoPixel, DFRobotDFPlayerMini
- **Design de Hardware:** KiCad (Esquemático)
- **Técnicas de Software:** Correção Gama 2.2, Sincronização por BPM, Pulso Senoidal.

## ⚙️ Funcionamento
O sistema opera através de uma máquina de estados que alterna entre dois modos (Speed e Heal). 
1. **Lógica de Áudio:** Dois módulos DFPlayer Mini operam em paralelo; um mantém a trilha sonora em loop enquanto o outro dispara efeitos sonoros (FX) por interrupções de botões.
2. **Sincronização Visual:** A fita LED WS2812B recebe dados processados em tempo real que calculam o brilho baseado em uma função seno, garantindo que a pulsação da luz siga o ritmo de 128 BPM das músicas.

## 🧠 Arquitetura de Software e Hardware

Abaixo estão os diagramas que detalham a lógica de estados e a conexão dos componentes.

### Fluxograma de Software (Lógica de Prioridades)
![Fluxograma de Software do Funcionamento da Pistola](./docs/fluxograma_pistola.svg)

### Diagrama de Blocos (Hardware)
![Diagrama de Blocos da Divisão de Energização](./docs/diagrama_blocos.svg)

## 🔌 Hardware e Conectividade

O sistema utiliza um Arduino Mega 2560 para gerenciar múltiplas comunicações seriais e entradas analógicas simultâneas.

### 📍 Mapeamento de Pinos (Pinagem)

| Componente | Pino Arduino | Função / Mecânica (Lúcio) |
| :--- | :---: | :--- |
| **Switch Power** | 2 | Liga/Desliga Sistema |
| **Switch Cor** | 3 | Alterna Modo: Speed (Verde) / Heal (Amarelo) |
| **LED Strip** | 9 | Saída de Dados WS2812B |
| **Potenciômetro** | A4 | Controle de Brilho e Volume (Gama 2.2) |
| **DFPlayer 1** | 16/17 (TX2/RX2) | Trilha Sonora (Loop) |
| **DFPlayer 2** | 18/19 (TX1/RX1) | Efeitos Sonoros (FX) |
| **Push-button 1** | A0 | Disparo Sônico (Som Randômico 3-4) |
| **Push-button 2** | A1 | Disparo Sônico (Som Randômico 5-6) |
| **Combo A0 + A1** | - | **Ultimate: Sound Barrier** (Som 7) |
| **Push-button 3** | A2 | Reload (Som 8) |

> 💰 A lista detalhada de custos e especificações técnicas pode ser encontrada no arquivo: [Lista de Materiais (BOM)](./hardware/BOM.md)

## ▶️ Como executar
1. **Cartão SD:** Carregue as músicas no cartão micro SD conforme o padrão do DFPlayer.
2. **Hardware:** Monte o circuito conforme o esquemático na pasta [/hardware/esquema_eletrico](./hardware/esquema_eletrico.svg). Atente-se às portas seriais (Serial1 e Serial2 do Mega).
3. **Software:** - Instale as bibliotecas `Adafruit_NeoPixel` e `DFRobotDFPlayerMini` na sua Arduino IDE.
   - Carregue o arquivo [/src/LUCIO_PROJECT_FINAL.ino](./src) no seu Arduino Mega.

## 📁 Estrutura do Repositório
- [/src](./src): Código-fonte (Firmware).
- [/hardware](./hardware): Esquema elétrico e Lista de Materiais (BOM).
- [/docs](./docs): Monografia, Fluxogramas e Diagramas de Blocos.
- [/media](./media): Galeria de fotos do protótipo e vídeos de demonstração.

## 📺 Demonstração

### 🎥 Vídeo Principal: Protótipo em Funcionamento
[![Assista o vídeo do Protótipo](https://github.com/ManuelaNunes/lucio-sonic-amplifier-arduino/raw/main/media/photos/17_Lucio_Gabinete_Montado_Sistema_Incluso.jpeg)](https://youtu.be/3OYdRO0IcG0)
> **Clique na imagem acima** para abrir o vídeo de demonstração das funcionalidades (Som e Luz).

## 📄 Documentação
O arquivo completo do Trabalho de Conclusão de Curso (TCC), contendo a fundamentação teórica e os testes de campo, está disponível na pasta `/docs`.

## 👩‍💻 Autora
**Manuela Nunes Simião** – Bacharel em Engenharia de Computação (UFPA).
> Este projeto foi desenvolvido em processo de superação e luto, inspirado pelo meu gatinho Lucinho e dedicado à memória de meu pai, Francisco de Souza Simião, que queria muito ver esse trabalho concluído.

[LinkedIn](https://www.linkedin.com/in/manuelanunessimiao)

![Lúcio](https://github.com/ManuelaNunes/lucio-sonic-amplifier-arduino/raw/main/media/videos/Lucio.gif)
