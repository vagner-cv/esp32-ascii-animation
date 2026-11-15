#pragma once
#include "Scene.h"

namespace ascii_show {
constexpr float GLOBE_R=0.91f,GLOBE_Y=0.24f,SNOW_FLOOR=-0.40f;

inline void Scene::buildGlobe() {
  resetPoints();
  // Shaded wooden cylinder, turned rim and warm band of lights.
  for(int row=0;row<25;++row) for(int k=0;k<160;++k) {
    float u=row/24.0f,a=k*TAU/160,y=-1.10f+u*0.43f;
    float r=0.73f+0.025f*cosf(u*TAU);
    float grain=0.72f+0.18f*sinf(k*0.38f+u*8)+0.10f*sinf(k*1.7f-u*3);
    add(cosf(a)*r,y,sinf(a)*r,shade(rgb(151,91,49),grain),0,'#',WOOD,cosf(a),0,sinf(a));
  }
  for(int ring=1;ring<=16;++ring) for(int k=0;k<112;++k) {
    float a=k*TAU/112,r=ring*0.047f;
    add(cosf(a)*r,-0.665f,sinf(a)*r,rgb(106,65,40),0,'#',WOOD,0,1,0);
  }
  for(int row=0;row<2;++row) for(int k=0;k<180;++k) {
    float a=k*TAU/180,r=0.758f;
    add(cosf(a)*r,-0.79f+row*0.016f,sinf(a)*r,rgb(246,185,67),0,'=',GOLD,cosf(a),0,sinf(a));
    if(row==0 && k%5==0) add(cosf(a)*0.768f,-0.775f,sinf(a)*0.768f,rgb(255,226,132),0,'*',LIGHT);
  }
  // Rounded snow bed conforms to the bottom of the sphere.
  for(int row=0;row<18;++row) for(int k=0;k<110;++k) {
    float y=-0.655f+row*0.255f/17,a=k*TAU/110;
    float r=sqrtf(clamp(GLOBE_R*GLOBE_R-(y-GLOBE_Y)*(y-GLOBE_Y),0,1))*0.98f;
    add(cosf(a)*r,y,sinf(a)*r,rgb(195,210,244),0,'#',SNOW,cosf(a),0.35f,sinf(a));
  }
  for(int i=0;i<1500;++i) {
    float r=sqrtf((i+0.5f)/1500)*0.632f,a=i*2.39996f;
    add(cosf(a)*r,SNOW_FLOOR,sinf(a)*r,rgb(228,233,255),0,'#',SNOW,0,1,0);
  }
  // Sample all visible faces, with physical normals for directional lighting.
  auto box=[&](float cx,float base,float cz,float w,float h,float d,uint16_t color,uint8_t kind) {
    int vertical=int(h/0.025f)+1,horizontal=9;
    for(int face=0;face<5;++face) {
      int ny=face==4?horizontal:vertical;
      for(int iy=0;iy<=ny;++iy) for(int ix=0;ix<horizontal;++ix) {
        float u=ix/float(horizontal-1)-0.5f,v=iy/float(ny);
        float x=cx+u*w,y=base+h*v,z=cz+(face==0?-d/2:d/2);
        float nx=0,nz=face==0?-1:1,normalY=0;
        if(face==2 || face==3) {x=cx+(face==2?-w/2:w/2);z=cz+u*d;nx=face==2?-1:1;nz=0;}
        if(face==4) {x=cx+u*w;z=cz+(v-0.5f)*d;y=base+h;nx=0;nz=0;normalY=1;}
        add(x,y,z,color,0,'#',kind,nx,normalY,nz);
        if(kind==ARCH && face<4 && ix%3==1 && iy%3==1) {
          add(x+nx*0.004f,y,z+nz*0.004f,rgb(239,191,104),0,'o',LIGHT,nx,normalY,nz);
        }
      }
    }
  };
  // A stepped city tower, a pale temple spire and the red Giant Swing.
  box(-0.11f,SNOW_FLOOR,0.15f,0.23f,0.62f,0.18f,rgb(105,132,165),ARCH);
  box(-0.14f,0.22f,0.16f,0.17f,0.30f,0.15f,rgb(129,147,174),ARCH);
  box(-0.11f,0.52f,0.14f,0.12f,0.23f,0.13f,rgb(144,159,183),ARCH);
  box(-0.11f,0.75f,0.14f,0.07f,0.12f,0.08f,rgb(185,190,212),ARCH);
  for(int tier=0;tier<10;++tier) {
    float u=tier/10.0f,w=0.27f*(1-u)+0.025f;
    box(0.31f,SNOW_FLOOR+tier*0.059f,-0.17f,w,0.032f,w,rgb(239,225,199),SNOW);
  }
  for(int j=0;j<16;++j) {
    float y=0.19f+j*0.012f,r=0.027f*(1-j/16.0f);
    for(int k=0;k<12;++k) {float a=k*TAU/12;add(0.31f+r*cosf(a),y,-0.17f+r*sinf(a),rgb(247,229,178),0,'+',SNOW,cosf(a),0.45f,sinf(a));}
  }
  for(int side=-1;side<=1;side+=2) box(-0.38f+side*0.082f,SNOW_FLOOR,-0.11f,0.026f,0.59f,0.034f,rgb(217,42,48),RED_STEEL);
  box(-0.38f,0.17f,-0.11f,0.24f,0.035f,0.044f,rgb(237,53,50),RED_STEEL);
  // Small decorated Christmas tree in the city scene.
  for(int row=0;row<18;++row) for(int k=0;k<36;++k) {
    float u=row/18.0f,a=k*TAU/36,r=(0.18f*(1-u))*(0.85f+0.15f*cosf(row*1.8f));
    add(0.28f+r*cosf(a),SNOW_FLOOR+0.02f+u*0.68f,0.29f+r*sinf(a),rgb(34,182,78),0,'v',NEEDLE,cosf(a),0.4f,sinf(a));
  }
  for(int k=0;k<32;++k) {
    float u=(k+0.5f)/32,a=k*2.39996f,r=0.19f*(1-u);
    add(0.28f+r*cosf(a),SNOW_FLOOR+0.02f+u*0.68f,0.29f+r*sinf(a),k%4?rgb(255,215,87):rgb(244,48,58),0,'*',LIGHT);
  }
  add(0.28f,0.32f,0.29f,rgb(255,231,142),0,'*',STAR,0,1,0);
  finishPoints();
}

inline bool Scene::snowflake(int i,float t,float &x,float &y,float &z) {
  float a=noise(i*7+700)*TAU,r=sqrtf(noise(i*7+701))*0.82f;
  x=cosf(a)*r+0.025f*sinf(t*0.65f+i);z=sinf(a)*r+0.025f*cosf(t*0.43f+i*1.7f);
  float half=sqrtf(clamp(GLOBE_R*GLOBE_R-x*x-z*z,0,1));
  float top=GLOBE_Y+half-0.025f,bottom=SNOW_FLOOR+0.02f,height=top-bottom;
  if(height<0.02f) return false;
  y=top-fmodf(noise(i*7+702)*height+t*(0.09f+0.09f*noise(i+1000)),height);
  return x*x+(y-GLOBE_Y)*(y-GLOBE_Y)+z*z<GLOBE_R*GLOBE_R;
}

inline void Scene::glass() {
  // Camera rays intersect the actual sphere. The transparent shell preserves
  // the interior depth, while its thin outline and reflections sit in front.
  constexpr float cam=4.4f,ct=0.985f,st=0.174f;
  constexpr float centerY=GLOBE_Y*ct,centerZ=-GLOBE_Y*st;
  float scale=viewScale();
  for(int row=0;row<rows;++row) for(int col=0;col<COLS;++col) {
    float dx=((col+0.5f)*CELL_W-120)/(scale*cam);
    float dy=(rows*CELL_H*0.48f-(row+0.5f)*CELL_H)/(scale*cam);
    float a=dx*dx+dy*dy+1,b=-centerY*dy-cam-centerZ;
    float c=centerY*centerY+(cam+centerZ)*(cam+centerZ)-GLOBE_R*GLOBE_R;
    float discriminant=b*b-a*c;if(discriminant<=0) continue;
    float distance=(-b-sqrtf(discriminant))/a;
    float x=dx*distance,y=dy*distance,z=distance-cam;
    float worldY=y*ct-z*st;if(worldY<-0.615f) continue;
    Cell &cell=cells[row*COLS+col];if(cell.depth<z-0.002f) continue;
    float nx=x/GLOBE_R,ny=(y-centerY)/GLOBE_R,nz=(z-centerZ)/GLOBE_R;
    float rim=clamp(1+nz,0,1),rim2=rim*rim;
    cell.color=additive(cell.color,shade(rgb(52,74,124),0.02f+rim2*0.20f));
    float u=(nx+0.48f)/0.19f,v=(ny-0.48f)/0.25f;
    float u2=(nx-0.70f)/0.11f,v2=(ny-0.05f)/0.25f;
    float shine=expf(-2*(u*u+v*v))*0.94f+expf(-2*(u2*u2+v2*v2))*0.50f;
    if(shine>0.065f) {
      put(col,row,z-0.003f,toneGlyph(shine+0.1f),highlight(shade(rgb(158,177,232),shine),shine*0.35f),uint8_t(shine*70));
    } else {
      // A whole character beside the silhouette is kept, avoiding gaps where a
      // very thin Fresnel band falls between cells at 240x240 resolution.
      float stepX=CELL_W/(scale*cam),stepY=CELL_H/(scale*cam);
      auto hits=[&](float rx,float ry) {
        float aa=rx*rx+ry*ry+1,bb=-centerY*ry-cam-centerZ;
        return bb*bb-aa*c>0;
      };
      bool edge=!hits(dx+stepX,dy)||!hits(dx-stepX,dy)||!hits(dx,dy+stepY)||!hits(dx,dy-stepY);
      if(!edge) continue;
      char ch=fabsf(ny)>fabsf(nx)?'-':(nx*ny>0?'/':'\\');
      put(col,row,z-0.003f,ch,shade(rgb(111,139,221),0.65f+rim2*0.35f));
    }
  }
}

inline void Scene::drawGlobe(float t) {
  for(int iz=0;iz<12;++iz) for(int ix=-15;ix<=15;++ix)
    project(ix*0.12f,-1.13f,(iz-6)*0.17f,'.',rgb(23,31,53),false);
  // Rotate the geometry and its normals together; key light stays at the camera.
  for(int i=0;i<count;++i) {
    const Point &p=points[i];
    float nx=(p.nx*spinC-p.nz*spinS)/127.0f,nz=(p.nx*spinS+p.nz*spinC)/127.0f,ny=p.ny/127.0f;
    float lam=diffuse(nx,ny,nz),light=(0.18f+0.94f*lam)*(p.ao/255.0f);
    float spec=p.kind==GOLD?specular(nx,ny,nz)*0.6f:0;
    if(p.kind==SNOW) light=0.30f+0.78f*lam;
    if(p.kind==STAR) light=1;
    if(p.kind==LIGHT) light=0.65f+0.35f*sinf(t*2.2f+i)*sinf(t*2.2f+i);
    uint16_t col=highlight(shade(p.color,light),spec);
    char glyph=p.kind==NEEDLE?toneGlyph(light,true):toneGlyph(light);
    if(p.kind==LIGHT || p.kind==STAR) glyph=p.glyph;
    if(p.kind==GOLD) glyph='=';
    project(p.x,p.y,p.z,glyph,col,true,(p.kind==LIGHT || p.kind==STAR)?180:0);
  }
  for(int i=0;i<190;++i) {
    float x,y,z;if(!snowflake(i,t,x,y,z)) continue;
    float depth=x*spinS+z*spinC;
    float bright=0.45f+0.55f*clamp((GLOBE_R-depth)/(2*GLOBE_R),0,1);
    project(x,y,z,i%6==0?'*':'.',shade(rgb(221,231,255),bright));
  }
  glass();
}
} // namespace ascii_show
