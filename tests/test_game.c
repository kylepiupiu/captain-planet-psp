#include "../src/game.h"
#include "../src/render.h"
#include "../src/content.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
static uint32_t frame[512*272];
static void press(int b){game_input(0,128,128);game_input(b,128,128);}
static void shot(const char *name){render_frame(frame,512);FILE *f=fopen(name,"wb");assert(f);fprintf(f,"P6\n480 272\n255\n");for(int y=0;y<272;y++)for(int x=0;x<480;x++){uint32_t v=frame[y*512+x];fputc(v&255,f);fputc((v>>8)&255,f);fputc((v>>16)&255,f);}fclose(f);}
static int q[MW*MH],prev[MW*MH];
static int path(float x,float y,float tx,float ty,int follow){int start=(int)y/TILE*MW+(int)x/TILE,end=(int)ty/TILE*MW+(int)tx/TILE,head=0,tail=0;for(int i=0;i<MW*MH;i++)prev[i]=-1;prev[start]=start;q[tail++]=start;int dd[4]={1,-1,MW,-MW};while(head<tail&&prev[end]<0){int p=q[head++];for(int k=0;k<4;k++){int n=p+dd[k];if(n<0||n>=MW*MH||prev[n]>=0||abs(n%MW-p%MW)>1)continue;if(game_blocked((n%MW)*TILE+10,(n/MW)*TILE+10))continue;prev[n]=p;q[tail++]=n;}}if(prev[end]<0)return 0;if(!follow)return 1;int rev[MW*MH],len=0,p=end;while(p!=start){rev[len++]=p;p=prev[p];}while(len){p=rev[--len];float xx=(p%MW)*TILE+10,yy=(p/MW)*TILE+10;int safety=200;while(hypotf(g.x-xx,g.y-yy)>3&&safety--){float dx=xx-g.x,dy=yy-g.y;int b=fabsf(dx)>fabsf(dy)?(dx>0?RIGHT:LEFT):(dy>0?DOWN:UP);game_input(b,128,128);game_update();if(g.mode!=PLAY)return 0;}if(safety<=0)return 0;}return 1;}
static void no_enemies(void){memset(g.enemies,0,sizeof(g.enemies));memset(g.shots,0,sizeof(g.shots));}
int main(void){game_init();assert(render_load("assets"));shot("tests/title.ppm");int counts[6]={0};int modes[10]={0};
 for(int level=0;level<113;level++){
  game_start(level);const Episode *e=&episodes[level];counts[e->season-1]++;modes[e->mode]++;assert(g.mode==BRIEF);press(CROSS);assert(g.mode==PLAY);no_enemies();
  for(int i=0;i<g.needed;i++){assert(path(g.x,g.y,g.objs[i].x,g.objs[i].y,0));}
  assert(path(g.x,g.y,g.exitx,g.exity,0));
  for(int i=0;i<g.needed;i++){
   Object *o=&g.objs[i];assert(path(g.x,g.y,o->x,o->y,1));g.hero=e->mode==1?4:o->hero;
   if(e->mode==7&&g.phase!=i%2)press(SELECT);
   if(e->mode==9){for(int k=0;k<301;k++){no_enemies();game_input(0,128,128);game_update();}}
   if(e->mode==0||e->mode==3||e->mode==6){int correct=g.hero;g.hero=(correct+1)%5;int done=g.done;press(SQUARE);assert(done==g.done);g.hero=correct;}
   press(SQUARE);assert(o->state);no_enemies();
  }
  assert(g.done==g.needed);if(e->mode==5)assert(g.has_ring);
  // Route/goal logic tests isolate enemy damage; combat is checked separately below.
  g.boss_spawned=2;no_enemies();assert(path(g.x,g.y,g.exitx,g.exity,1));
  if(e->mode==4){for(int t=0;t<4000&&hypotf(g.escortx-g.exitx,g.escorty-g.exity)>60;t++){game_input(0,128,128);game_update();}assert(hypotf(g.escortx-g.exitx,g.escorty-g.exity)<65);}press(SQUARE);assert(g.mode==CLEAR);assert(g.save.stars[level]);
  if(level==112)shot("tests/ending.ppm");
 }
 assert(counts[0]==26&&counts[1]==26&&counts[2]==13&&counts[3]==22&&counts[4]==13&&counts[5]==13);for(int i=0;i<10;i++)assert(modes[i]>0);
 printf("PASS 113 campaign entries; per-season counts; all 10 mission types; reachable objectives and exits; objective gates; clear/replay progress\n");
 // Controlled battle: use the real attack and projectile collision code, not direct HP changes.
 game_start(0);press(CROSS);no_enemies();g.x=200;g.y=330;g.hero=1;g.dir=3;Enemy *en=&g.enemies[0];en->x=280;en->y=330;en->kind=1;en->active=1;en->hp=en->maxhp=12;en->cool=9999;
 for(int i=0;i<120;i++){game_input(CROSS,128,128);game_update();}assert(!en->active);assert(g.kills==1);
 g.energy=100;press(TRIANGLE);assert(g.power==360&&g.energy==0);for(int i=0;i<360;i++){game_input(0,128,128);game_update();}assert(!g.power);
 int ticks=g.tick;press(START);for(int i=0;i<100;i++)game_update();assert(g.tick==ticks);press(START);assert(g.mode==PLAY);
 printf("PASS projectile hits, enemy defeat, energy summon, summon expiration, pause freezes simulation\n");
 // Valid primary, backup recovery, corrupt data, malicious bounds and write failure.
 g.save.current=72;assert(game_save("tests/check_save.dat"));g.save.current=88;assert(game_save("tests/check_save.dat"));g.save.current=0;assert(game_load("tests/check_save.dat")==1&&g.save.current==88);FILE *f=fopen("tests/check_save.dat","wb");fputs("broken",f);fclose(f);assert(game_load("tests/check_save.dat")==2&&g.save.current==72);Save bad=g.save;bad.current=9999;assert(!game_validate_save(&bad));assert(!game_save("/not/a/directory/save.dat"));g.save_error=0;
 printf("PASS save checksum, atomic staging, backup recovery, rejected corrupt bounds and failed write reporting\n");
 game_start(0);shot("tests/brief.ppm");press(CROSS);for(int i=0;i<2;i++)game_update();shot("tests/coast.ppm");g.x=230;g.y=160;game_update();shot("tests/puzzle.ppm");g.energy=100;press(TRIANGLE);game_update();shot("tests/captain.ppm");g.return_mode=PLAY;g.mode=HELP;g.help_page=1;shot("tests/map.ppm");
 game_start(6);press(CROSS);g.x=215;g.y=160;game_update();shot("tests/rescue.ppm");
 // Random input and rendering exercise bounds and mode transitions with sanitizer instrumentation.
 srand(271);for(int t=0;t<3200;t++){if(t%100==0){game_start(rand()%113);press(CROSS);}int buttons[]={0,LEFT,RIGHT,UP,DOWN,CROSS,CIRCLE,TRIANGLE,SQUARE,LB,RB,START,SELECT};game_input(buttons[rand()%13],128,128);game_update();if(t%2==0)render_frame(frame,512);}
 render_shutdown();printf("PASS 3200 randomized input steps, 1600 renders; no sanitizer findings\n");return 0;}
