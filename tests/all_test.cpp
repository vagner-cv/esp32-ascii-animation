#define ASCII_TREE 1
#define ASCII_GLOBE 0
#define ASCII_CODE 0
#define ASCII_ALL 1
#include "../engine/AsciiShow.h"
#include <cassert>
using namespace ascii_show;
void sample(uint32_t t,int level){hostMillis=t;hostButton=level;pollButton();}
int main(){
 begin();assert(ready && modeIndex==0);
 Point* original=storage;
 for(int i=0;i<12;++i){selectMode(i);renderFrame(15);assert(modeIndex==i%6);assert(storage==original);assert(scene->rows==((i%2)?26*6/CELL_H:240/CELL_H));assert(!scene->overflow);}
 selectMode(0);
 sample(100,LOW);sample(110,HIGH);sample(150,HIGH);assert(modeIndex==0);
 sample(200,LOW);sample(240,LOW);sample(300,HIGH);sample(340,HIGH);assert(modeIndex==1);
 sample(400,LOW);sample(440,LOW);sample(1440,LOW);assert(exposure==1.25f && modeIndex==1);
 sample(1500,HIGH);sample(1540,HIGH);assert(modeIndex==1);
 for(int i=0;i<6;++i){selectMode(i);}
 selectMode(6);assert(modeIndex==0);
 puts("PASS mode cycle, shared cache, debounce, short/long press and exposure.");
}
