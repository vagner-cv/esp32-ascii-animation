#include "../engine/Scene.h"
#include <cassert>
#include <cstdio>
#include <memory>
#include <initializer_list>
using namespace ascii_show;
static uint64_t checksum(const Scene &s) {
  uint64_t h=1469598103934665603ULL;
  for(int i=0;i<COLS*s.rows;++i) {h^=s.cells[i].color;h*=1099511628211ULL;h^=s.cells[i].glyph;}
  return h;
}
int main() {
  std::unique_ptr<Point[]> storage(new Point[MAX_POINTS]);
  Scene globe(false,storage.get(),true);globe.buildGlobe();
  assert(!globe.overflow && globe.count>12000 && globe.count<=MAX_POINTS);
  int windows=0,architecture=0,temple=0;
  for(int i=0;i<globe.count;++i) {
    const Point &p=globe.points[i];
    float n=sqrtf(float(p.nx*p.nx+p.ny*p.ny+p.nz*p.nz))/127;
    assert(n>0.97f && n<1.02f && p.born==0);
    if(p.y>=SNOW_FLOOR && p.kind!=WOOD) {
      float d=p.x*p.x+(p.y-GLOBE_Y)*(p.y-GLOBE_Y)+p.z*p.z;
      assert(d<=GLOBE_R*GLOBE_R+0.001f);
    }
    if(p.kind==LIGHT)++windows;
    if(p.kind==ARCH)++architecture;
    if(p.kind==SNOW)++temple;
  }
  assert(windows>60 && architecture>1000 && temple>1000);
  int tested=0,falling=0;
  for(float time:{0.0f,2.0f,15.0f,24.0f,10000.0f,3000000.0f}) for(int i=0;i<190;++i) {
    float x,y,z;
    if(!Scene::snowflake(i,time,x,y,z))continue;
    ++tested;
    assert(isfinite(x) && isfinite(y) && isfinite(z) && y>=SNOW_FLOOR);
    assert(x*x+(y-GLOBE_Y)*(y-GLOBE_Y)+z*z<GLOBE_R*GLOBE_R);
    if(time==0) {
      float x1,y1,z1;if(Scene::snowflake(i,time+0.1f,x1,y1,z1) && y1<y)++falling;
    }
  }
  assert(tested>1000 && falling>170);
  for(bool split:{false,true}) {
    globe.render(0,split);auto first=checksum(globe);
    globe.render(6,split);assert(checksum(globe)!=first && globe.hasGlow);
    for(int i=0;i<160;++i) {
      globe.render(i*0.19f,split);
      for(int j=0;j<COLS*MAX_ROWS;++j) {
        assert(isfinite(globe.cells[j].depth));
        if(j>=COLS*globe.rows)assert(globe.cells[j].glyph==' ');
      }
    }
    // Transparent centre preserves an interior glyph; foreground remains in front.
    for(auto &cell:globe.cells)cell={10000,0,' ',0};
    globe.glass();int edgeCount=0;
    for(int i=0;i<COLS*globe.rows;++i)if(globe.cells[i].glyph!=' ')++edgeCount;
    assert(edgeCount>COLS/2);
    int cx=COLS/2,cy=int(globe.rows*0.42f);
    globe.put(cx,cy,0,'@',rgb(255,0,0));globe.glass();
    assert(globe.cells[cy*COLS+cx].glyph=='@');
    assert(globe.cells[0].glyph==' ');
    globe.put(cx,cy,-4,'#',rgb(0,255,0));globe.glass();
    assert(globe.cells[cy*COLS+cx].glyph=='#' && globe.cells[cy*COLS+cx].color==rgb(0,255,0));
  }
  Scene empty(false,nullptr,true);empty.buildGlobe();assert(empty.overflow && empty.count==0);
  for(int capacity:{512,2048,3072}) {
    std::unique_ptr<Point[]> small(new Point[capacity]);
    Scene light(false,small.get(),true,capacity);light.buildGlobe();
    assert(!light.overflow && light.count<=capacity && light.count>capacity*3/4);
    bool tower=false,temple=false,swing=false,bulb=false;
    for(int i=0;i<light.count;++i) {
      tower|=light.points[i].kind==ARCH;temple|=light.points[i].kind==SNOW;
      swing|=light.points[i].kind==RED_STEEL;bulb|=light.points[i].kind==LIGHT;
    }
    assert(tower && temple && swing && bulb);
    light.render(6,true);assert(light.hasGlow);
  }
  printf("PASS: %d globe points, confined falling snow, transparent glass/depth and 320 frames in both layouts.\n",globe.count);
}
