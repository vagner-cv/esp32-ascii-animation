# Validação das seis animações

## Ambiente e compilação real

- Arduino CLI **1.6.0**.
- Core **esp32 by Espressif Systems 3.3.12**.
- **GFX Library for Arduino 1.6.9**, instalada pelo gerenciador Arduino.
- Alvo: `esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,PartitionScheme=huge_app,CDCOnBoot=cdc`.
- Construtores SPI/ST7789/Canvas, fonte e APIs compilados com a biblioteca real e a toolchain Xtensa do core.

As seis configurações finais compilaram e linkaram com sucesso com a toolchain do ESP32-S3:

| Configuração | Compilação | Flash (bytes) | Globais RAM (bytes) |
| --- | --- | ---: | ---: |
| `Arvore_ASCII` | Aprovado | 387,336 | 73,968 |
| `Arvore_ASCII_Codigo` | Aprovado | 392,140 | 74,016 |
| `AguaViva_ASCII` | Aprovado | 382,884 | 73,968 |
| `AguaViva_ASCII_Codigo` | Aprovado | 386,280 | 74,016 |
| `GloboNeve_ASCII` | Aprovado | 387,068 | 73,968 |
| `GloboNeve_ASCII_Codigo` | Aprovado | 389,300 | 74,016 |

A RAM global informada pelo compilador não inclui o framebuffer e a nuvem de pontos alocados durante a inicialização. A verificação final recompila os conteúdos dos seis `.ino`, com os mesmos headers, em uma pasta de compilação fixa para reutilizar os objetos da biblioteca; cada pasta distribuída mantém seu `.ino` de mesmo nome.

Para reproduzir, com o core e a biblioteca instalados:

```sh
bash tools/compile.sh
```

Se o CLI usar um arquivo de configuração específico, defina `ARDUINO_CONFIG=/caminho/arduino-cli.yaml` antes do comando.

## Testes da cena no computador

`tests/scene_test.cpp` verifica:

- 14.628 pontos finitos da árvore, sete tipos de material e capacidade máxima respeitada.
- Normais compactas com comprimento unitário, direção da luz lateral, mais de 12 tons verdes e diferença superior a 4× entre verdes claros e escuros no quadro de teste.
- Interpolação do halo nos limites da tela e presença de emissão nas luzes.
- Formação progressiva, mudança ao girar e reinício da sequência aos 40 segundos.
- Movimento da água-viva entre instantes diferentes.
- Partículas continuam dando a volta na tela após semanas de funcionamento simulado.
- 1.280 quadros nos dois layouts, limites da grade, profundidade finita e regiões externas limpas.
- Oclusão do ponto distante pelo mais próximo e descarte de coordenadas fora da tela/atrás da câmera.
- Tratamento de armazenamento de pontos indisponível.
- Cache leve com 512, 2.048 e 3.072 posições: respeita a capacidade e preserva todos os materiais da árvore, incluindo estrela e presentes.

`tests/globe_test.cpp` verifica:

- 17.351 pontos de geometria, normais compactas unitárias e cenário dentro da esfera, com o pedestal externo.
- Flocos confinados ao globo, queda vertical, piso e funcionamento após longo tempo simulado.
- 320 quadros nos dois layouts, rotação, emissão das luzes e profundidade finita.
- Vidro transparente preserva o cenário central, contorno permanece visível e um objeto na frente continua na frente do vidro.
- Cache leve com 512, 2.048 e 3.072 posições mantém torre, templo, estrutura vermelha e luzes.

O conjunto foi executado nos modos **80 × 60 suavizado** e **60 × 40 nítido**, com **AddressSanitizer e UndefinedBehaviorSanitizer**, sem erros de memória ou comportamento indefinido nos testes. A detecção de vazamentos do LeakSanitizer foi desativada porque este ambiente usa ptrace; a checagem de limites do AddressSanitizer permaneceu ativa.

```sh
g++ -std=c++17 -Wall -Wextra -Werror -O1 -g \
  -fsanitize=address,undefined tests/scene_test.cpp -o /tmp/ascii-scene-test
ASAN_OPTIONS=detect_leaks=0 /tmp/ascii-scene-test
```

Para repetir a outra densidade, acrescente `-DASCII_HIGH_DETAIL=0` à compilação.

Para o globo, repita o mesmo comando usando `tests/globe_test.cpp` e um nome de executável diferente.

## Renderizador, painel e controles

`tests/preview.cpp` executa o mesmo adaptador `engine/AsciiShow.h`, substituindo apenas tela, alocador e serial por implementações de computador. A fonte 5 × 7 vem da instalação real de Arduino_GFX.

Os seis executáveis de prévia são compilados com `-Wall -Wextra -Werror` e testam pausa/retomada, reinício pelo serial e transbordamento de `millis()`. As duas árvores e os dois globos também passam por inicialização com PSRAM indisponível, usando o cache leve na RAM interna, e por falha de todas as alocações, exibindo o aviso sem iniciar o renderizador. As prévias do globo têm 96 quadros de movimento; as demais têm 48, além de quadros de formação.

As quatro imagens PNG anteriores de árvore e água-viva foram comparadas byte a byte com a versão anterior e permanecem idênticas no caminho com PSRAM e detalhe completo. O modo leve foi testado em simulação; seu desempenho e disponibilidade de RAM ainda precisam de teste físico.

```sh
# Pillow só é necessário para gerar as prévias, não para usar a placa.
python3 tools/preview.py \
  --gfx-font-dir /caminho/GFX_Library_for_Arduino/src/font
```

As imagens mostram a orientação lógica da cena; a rotação e a transferência SPI no hardware não são simuladas.

## O que falta conferir na placa física

Não houve acesso à placa para upload ou teste físico. Compilar para ESP32-S3 e renderizar no computador não comprovam imagem, orientação ou desempenho no display real.

Depois de gravar cada sketch:

1. Verifique se a tela acende, ocupa os 240 × 240 pixels e aparece na orientação esperada.
2. Aguarde pelo menos 15 segundos para ver a árvore formada e decorada; confira rotação, presentes e estrela. Aguarde 40 segundos para conferir o reinício.
3. Na água-viva, confira pulsação, tentáculos, cores e as duas figuras menores ao fundo.
4. No globo, aguarde uma volta de 24 segundos e confira os edifícios, a árvore pequena, os reflexos, a neve e a base iluminada.
5. Nas versões com código, confirme que a cena continua animando e o texto rola apenas no painel inferior.
6. No serial a 115200, teste `p` e `r` e anote o **FPS medido** a cada cinco segundos. Não há FPS de hardware medido neste relatório.
7. Compare `ASCII_HIGH_DETAIL=1` e `0` caso queira escolher entre maior densidade e símbolos mais nítidos.

Para restaurar um firmware anterior, grave novamente o sketch do projeto correspondente.
