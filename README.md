# Semaforo-
Este projeto simula um semáforo utilizando um Raspberry Pi Pico. O código foi desenvolvido em C com o Pico SDK e simulado no Wokwi.
🚦 Projeto Semáforo com Raspberry Pi Pico

Este projeto simula um semáforo utilizando um Raspberry Pi Pico. O código foi desenvolvido em C com o Pico SDK e testado no Wokwi.
✨ Como Funciona

O semáforo alterna entre três estados, com cada LED permanecendo aceso por 3 segundos:

🔴 Vermelho → 🚦 Pare
🟡 Amarelo → ⚠️ Atenção
🟢 Verde → ✅ Siga

Os LEDs trocam automaticamente de acordo com um temporizador.
🛠️ Requisitos

✔️ Hardware

    Raspberry Pi Pico
    3 LEDs (vermelho, amarelo, verde)
    3 resistores de 330 Ω
    Protoboard e jumpers

✔️ Software

    VS Code com a extensão Wokwi
    Pico SDK configurado

🚀 Como Executar

1️⃣ Clone este repositório:

git clone https://github.com/seu-usuario/semaforo-pico.git

2️⃣ Abra o projeto no VS Code
3️⃣ Execute a simulação no Wokwi

Caso esteja rodando no Raspberry Pi Pico real, compile o código e carregue-o na placa.

📌 Dica: Para mais detalhes sobre o código, consulte o arquivo main.c.

📌 Autor: Alisson Nonato De Oliveira
📌 Simulação: Wokwi
