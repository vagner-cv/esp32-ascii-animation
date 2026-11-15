// Firmware unico: seis animacoes. Botao curto = proxima; segurar 1s = luminosidade.
#define ASCII_TREE 1
#define ASCII_CODE 0
#define ASCII_GLOBE 0
#define ASCII_HIGH_DETAIL 1
#define ASCII_ALL 1
#define ASCII_BUTTON_PIN 12 // Botao externo "proximo" documentado; ativo em LOW.
#include "src/AsciiShow.h"
void setup(){ascii_show::begin();}
void loop(){ascii_show::tick();}
