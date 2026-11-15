# esp32-ascii-animation

## Projeto e destino

Este repositório contém quatro animações ASCII para a **Waveshare ESP32-S3-LCD-1.3**: árvore de Natal e água-viva, cada uma em tela cheia e com painel de código rolando.

- Repositório deste projeto: **vagner-cv/esp32-ascii-animation**.
- Tratar este projeto separadamente dos outros projetos ESP32 do usuário. Compartilhar o modelo de placa não torna os projetos parte do mesmo repositório.
- Antes de publicar alterações, conferir o `origin` deste repositório.

## Hardware e desenvolvimento

- ESP32-S3, ST7789 240 × 240 por SPI, flash 16 MB, PSRAM OPI 8 MB.
- GPIOs LCD: DC=38, CS=39, SCK=40, MOSI=41, RST=42, BL=20. Canvas com rotação 1; SPI 40 MHz.
- Arduino/C++ e **GFX Library for Arduino / Arduino_GFX** de Moon On Our Nation; execução offline.
- Fonte canônica em `engine/`. Após editar, executar `python3 tools/generate.py` para atualizar os quatro sketches independentes.
- Preservar a pasta de cada sketch com o mesmo nome do `.ino`, incluindo `src/`.
- Não editar apenas um header gerado, deixando os demais divergentes.
- Atualizar instruções, ZIPs (`python3 tools/package.py`) e prévias quando as alterações afetarem a distribuição ou o visual.
- `VALIDACAO.md` documenta as verificações existentes e os comandos para reproduzi-las. Não afirmar upload, FPS ou funcionamento físico sem o teste correspondente na placa.
- Nunca adicionar credenciais ou segredos.
