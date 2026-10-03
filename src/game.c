#include "game.h"
#include "content.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
Game g;
static float clampf(float a,float l,float h){return a<l?l:a>h?h:a;}
static int rng(int n){g.rng=g.rng*1664525u+1013904223u;return (int)((g.rng>>8)%(unsigned)n);}
static float distance(float x,float y,float a,float b){return hypotf(x-a,y-b);}
void game_toast(const char *s){snprintf(g.toast,sizeof(g.toast),"%s",s);g.toast_time=130;}
void game_event(int type){g.sound_event=type;}
static void burst(float x,float y,int t,int n){for(int i=0;i<MAXFX&&n;i++)if(!g.fx[i].life){Particle *p=&g.fx[i];p->x=x;p->y=y;p->vx=(rng(101)-50)/28.f;p->vy=(rng(101)-50)/28.f;p->life=18+rng(14);p->type=t;n--;}}
static void mark(int x,int y,int t){if(x>0&&y>0&&x<MW-1&&y<MH-1)g.tiles[y][x]=t;}
static void cleararea(int x,int y,int r){for(int yy=y-r;yy<=y+r;yy++)for(int xx=x-r;xx<=x+r;xx++)mark(xx,yy,2);}
static void corridor(int x,int y,int a,int b){while(x!=a){cleararea(x,y,1);x+=a>x?1:-1;}while(y!=b){cleararea(x,y,1);y+=b>y?1:-1;}cleararea(a,b,2);}
static void move(float *x,float *y,float vx,float vy){if(!game_blocked(*x+vx,*y))*x+=vx;if(!game_blocked(*x,*y+vy))*y+=vy;}
static void escort_follow(void){
 int from=(int)g.escorty/TILE*MW+(int)g.escortx/TILE,to=(int)g.y/TILE*MW+(int)g.x/TILE;
 float tx=g.x,ty=g.y;
 if(from!=to){int queue[MW*MH],parent[MW*MH],head=0,tail=0;for(int i=0;i<MW*MH;i++)parent[i]=-1;parent[to]=to;queue[tail++]=to;int ds[4]={1,-1,MW,-MW};
 while(head<tail&&parent[from]<0){int a=queue[head++];for(int k=0;k<4;k++){int b=a+ds[k];if(b<0||b>=MW*MH||abs(b%MW-a%MW)>1||parent[b]>=0||game_blocked(b%MW*TILE+10,b/MW*TILE+10))continue;parent[b]=a;queue[tail++]=b;}}
 if(parent[from]<0)return;int next=parent[from];tx=next%MW*TILE+10;ty=next/MW*TILE+10;
 }
 float d=distance(g.escortx,g.escorty,tx,ty);if(d>2)move(&g.escortx,&g.escorty,(tx-g.escortx)/d*2.1f,(ty-g.escorty)/d*2.1f);
}
int game_blocked(float x,float y){int xs[2]={(int)(x-7)/TILE,(int)(x+7)/TILE},ys[2]={(int)(y-5)/TILE,(int)(y+3)/TILE};for(int a=0;a<2;a++)for(int b=0;b<2;b++){if(xs[a]<1||ys[b]<1||xs[a]>=MW-1||ys[b]>=MH-1||g.tiles[ys[b]][xs[a]]==1)return 1;}return 0;}
static void add_enemy(float x,float y,int kind){for(int i=0;i<MAXEN;i++)if(!g.enemies[i].active){Enemy *e=&g.enemies[i];memset(e,0,sizeof(*e));e->x=x;e->y=y;e->kind=kind;e->active=1;e->hp=e->maxhp=kind==3?36+episodes[g.level].season*3:kind==2?7:4;e->cool=40+rng(100);return;}}
static void shoot(float x,float y,float vx,float vy,int hero,int enemy,int damage){for(int i=0;i<MAXSHOT;i++)if(!g.shots[i].active){Shot *s=&g.shots[i];s->x=x;s->y=y;s->vx=vx;s->vy=vy;s->hero=hero;s->enemy=enemy;s->damage=damage;s->active=1;s->life=enemy?120:38;return;}}
static void reward(Object *o){o->state=1;g.done++;g.energy=(int)clampf(g.energy+26,0,100);g.hp=(int)clampf(g.hp+8,0,g.maxhp);burst(o->x,o->y,o->hero,16);game_event(3);}
static void damage(int amount){if(g.invincible||g.power||g.dash)return;g.hp-=amount;g.invincible=42;burst(g.x,g.y,6,8);game_event(4);if(g.hp<=0){g.hp=0;g.mode=FAIL;g.menu=0;}}
static void complete(void){if(g.won)return;g.won=1;g.mode=CLEAR;g.stars=1+(g.hp>=g.maxhp/2)+(g.elapsed<180*30);if(g.stars>g.save.stars[g.level])g.save.stars[g.level]=(uint8_t)g.stars;if(g.save.unlocked<g.level+1&&g.level<112)g.save.unlocked=g.level+1;g.save.current=g.level<112?g.level+1:112;g.dirty=1;game_event(5);}
void game_init(void){memset(&g,0,sizeof(g));g.save.magic=0x43504C54;g.save.version=1;g.save.music=1;g.rng=127;g.mode=TITLE;g.hero=0;g.maxhp=g.hp=100;}
int game_readable(int level){return level>=0&&level<EP_COUNT;}
void game_start(int level){if(!game_readable(level))return;Save save=g.save;int err=g.asset_error;memset(&g,0,sizeof(g));g.save=save;g.asset_error=err;g.level=level;const Episode *e=&episodes[level];g.hero=e->lead;g.rng=0x431054u+level*9137u;g.mode=BRIEF;g.maxhp=g.hp=g.save.difficulty?100:140;g.needed=e->count;g.energy=18;g.has_ring=e->mode!=5;g.restricted=e->mode==5;g.mission_timer=(g.save.difficulty?270:390)*30;g.startx=g.x=4*TILE+10;g.starty=g.y=16*TILE+10;g.exitx=44*TILE+10;g.exity=16*TILE+10;
 for(int y=0;y<MH;y++)for(int x=0;x<MW;x++){g.tiles[y][x]=(x==0||y==0||x==MW-1||y==MH-1)?1:0;}
 for(int i=0;i<120;i++){int x=2+rng(MW-4),y=2+rng(MH-4);mark(x,y,1);if(rng(3)==0)mark(x+1,y,1);}
 // Two landform bands create separate exploration regions. Carved passages remain stable.
 for(int x=15;x<=32;x+=17)for(int y=1;y<MH-1;y++)for(int w=0;w<2;w++)mark(x+w,y,(e->biome==5||e->biome==9||e->biome==11)?1:3);
 int points[8][2]={{11,6},{23,25},{35,6},{11,25},{35,25},{23,7},{7,12},{40,21}};
 int shift=level%3;for(int i=0;i<3;i++){points[i][0]+=((level/3+i)%3)-1;points[i][1]+=(shift-1);}
 corridor(4,16,44,16);corridor(4,16,10,7);corridor(10,7,36,7);corridor(10,7,10,25);corridor(10,25,37,25);corridor(37,25,44,16);
 for(int i=0;i<g.needed;i++){Object *o=&g.objs[i];int px=points[i][0],py=points[i][1];corridor(px,16,px,py);o->x=px*TILE+10;o->y=py*TILE+10;o->active=1;o->hero=(e->lead+i*2)%5;o->type=e->mode;o->hp=1;cleararea(px,py,2);}
 // All core objectives are on guaranteed paths. Decoration/hazards never close the paths.
 for(int i=0;i<48;i++){int x=2+rng(MW-4),y=2+rng(MH-4);if(g.tiles[y][x]==0)g.tiles[y][x]=4;}
 for(int i=g.needed;i<g.needed+3;i++){Object *o=&g.objs[i];int p=i-g.needed+3;int x=points[p][0],y=points[p][1];corridor(x,16,x,y);o->x=x*TILE+10;o->y=y*TILE+10;o->active=1;o->type=10;o->hero=i%5;}
 cleararea(4,16,2);cleararea(44,16,3);g.escortx=g.objs[0].x;g.escorty=g.objs[0].y;
 if(e->mode!=8&&e->mode!=4){int count=6+e->season+(g.save.difficulty?3:0);for(int i=0;i<count;i++){int x=10+rng(32),y=4+rng(24);if(g.tiles[y][x]==1){x=10+i*2;y=16;}add_enemy(x*TILE+10,y*TILE+10,i%3);}}
 if(e->mode==4){for(int i=0;i<5;i++)add_enemy((15+i*5)*TILE,16*TILE,i%2);}
 g.save.current=level;g.dirty=1;game_toast("L/R切换队员，方块靠近互动，X使用元素。");
}
static void interact(void){const Episode *e=&episodes[g.level];for(int i=0;i<MAXOBJ;i++){Object *o=&g.objs[i];if(!o->active||o->state||distance(g.x,g.y,o->x,o->y)>39)continue;
 if(o->type==10){o->state=1;g.hp=(int)clampf(g.hp+30,0,g.maxhp);g.energy=(int)clampf(g.energy+20,0,100);game_toast("补给已回收：恢复生命与团队能量。");game_event(3);return;}
 if(e->mode==1&&g.hero!=4&&!g.power){game_toast("切换马蒂，用心灵之力安抚并救援。");return;}
 if(e->mode==0||e->mode==3||e->mode==6){if(g.hero!=o->hero&&!g.power){const char *tips[]={"需要夸米：土之力稳固机关。","需要惠勒：火之力驱动机关。","需要琳卡：风之力疏通机关。","需要吉：水之力冷却或净化。","需要马蒂：心灵之力建立联系。"};game_toast(tips[o->hero]);return;}}
 if(e->mode==7&&g.phase!=(i%2)){game_toast("这个时间锚尚未显现：按SELECT切换时相。");return;}
 if(e->mode==9){if(o->timer<30*10){game_toast("守在装置附近：抵挡袭击，等待修复完成。");return;}}
 reward(o);
 if(e->mode==5&&g.done==g.needed){g.has_ring=1;g.energy=100;game_toast("装备恢复！现在可以使用戒指与召唤。");}
 else if(e->mode==1)game_toast("救援成功！伙伴已将它送往安全地点。");
 else if(e->mode==2)game_toast("线索已记录。找到其余证据，再返回撤离点。");
 else if(e->mode==8)game_toast("倾听并记录了证言。继续联系其他当事者。");
 else if(e->mode==4){game_toast("路线检查点已开放。返回同伴身边接应。");}
 else game_toast("目标已完成，地图上的下一个标记已更新。");
 if(g.done==g.needed){if(e->mode==4)g.escort_started=1;game_toast(e->mode==4?"路线打通！带着同伴前往东侧撤离点。":"主要目标完成！前往东侧绿色撤离点。");}
 return;
 }if(distance(g.x,g.y,g.exitx,g.exity)<50){if(g.done<g.needed){game_toast("还有任务没有完成：按SELECT查看全区地图。");return;}if(e->mode==4&&distance(g.escortx,g.escorty,g.exitx,g.exity)>65){game_toast("同伴还没到，返回接应再一起撤离。");return;}if(g.boss_spawned==1){game_toast("撤离区被封锁，先阻止守卫装置。");return;}complete();return;}game_toast("靠近发光任务标记后按方块。SELECT查看地图。");}
static void ring_attack(void){if(g.cool)return;g.cool=g.power?5:(g.has_ring?12:20);float dx=0,dy=1;if(g.dir==1)dx=-1,dy=0;if(g.dir==2)dy=-1;if(g.dir==3)dx=1,dy=0;int hero=g.power?5:g.hero;
 if(!g.has_ring){for(int i=0;i<MAXEN;i++){Enemy *e=&g.enemies[i];if(e->active&&distance(g.x,g.y,e->x,e->y)<42)e->stun=90;}burst(g.x+dx*12,g.y+dy*12,2,6);game_event(1);return;}
 // Mild aim assistance preserves four-direction movement while making PSP combat comfortable.
 float best=180;Enemy *target=NULL;for(int i=0;i<MAXEN;i++){Enemy *e=&g.enemies[i];float d=distance(g.x,g.y,e->x,e->y);if(e->active&&d<best&&((e->x-g.x)*dx+(e->y-g.y)*dy)>0){best=d;target=e;}}
 if(target){dx=(target->x-g.x)/best;dy=(target->y-g.y)/best;}
 shoot(g.x,g.y-10,dx*(g.power?9:7),dy*(g.power?9:7),hero,0,g.power?7:g.hero==1?3:2);
 burst(g.x+dx*14,g.y+dy*14-10,hero,3);game_event(1);
}
void game_input(unsigned buttons,int ax,int ay){g.pressed=(int)(buttons&~(unsigned)g.prev);g.prev=(int)buttons;g.moving=0;int p=g.pressed;
 if(g.mode==TITLE){if(p&(UP|DOWN))g.menu=(g.menu+(p&DOWN?1:4))%5;if(p&(CROSS|START)){game_event(2);if(g.menu==0)game_start(g.save.current);if(g.menu==1){g.mode=SELECTOR;g.selection=g.save.current;}if(g.menu==2){g.return_mode=TITLE;g.mode=HELP;g.help_page=0;}if(g.menu==3){g.save.difficulty=!g.save.difficulty;g.dirty=1;}if(g.menu==4)g.mode=CREDITS;}return;}
 if(g.mode==SELECTOR){if(p&UP)g.selection=(g.selection+112)%113;if(p&DOWN)g.selection=(g.selection+1)%113;if(p&LB)g.selection=(g.selection+100)%113;if(p&RB)g.selection=(g.selection+13)%113;if(p&CIRCLE){g.mode=TITLE;g.menu=0;}if(p&CROSS)game_start(g.selection);return;}
 if(g.mode==BRIEF){if(p&(CROSS|START|SQUARE)){g.mode=PLAY;g.prev=buttons;game_event(2);}if(p&CIRCLE){g.mode=SELECTOR;g.selection=g.level;}return;}
 if(g.mode==HELP){if(p&(CIRCLE|SELECT|START))g.mode=g.return_mode;if(p&(RIGHT|RB|CROSS))g.help_page=(g.help_page+1)%2;if(p&(LEFT|LB))g.help_page=(g.help_page+1)%2;return;}
 if(g.mode==CREDITS){if(p&(CIRCLE|CROSS|START))g.mode=TITLE;return;}
 if(g.mode==CLEAR){if(p&CROSS){if(g.level<112)game_start(g.level+1);else {g.mode=CREDITS;g.menu=0;}}if(p&CIRCLE){g.mode=SELECTOR;g.selection=g.level;}return;}
 if(g.mode==FAIL){if(p&CROSS)game_start(g.level);if(p&CIRCLE){g.mode=SELECTOR;g.selection=g.level;}return;}
 if(g.mode==PAUSE){if(p&(UP|DOWN))g.menu=(g.menu+(p&DOWN?1:5))%6;if(p&(START|CIRCLE)){g.mode=PLAY;return;}if(p&CROSS){if(g.menu==0)g.mode=PLAY;if(g.menu==1){g.return_mode=PAUSE;g.mode=HELP;g.help_page=0;}if(g.menu==2){g.save.music=!g.save.music;g.dirty=1;}if(g.menu==3)game_start(g.level);if(g.menu==4){g.mode=SELECTOR;g.selection=g.level;g.dirty=1;}if(g.menu==5){g.exit_request=1;g.dirty=1;}}return;}
 if(g.mode!=PLAY)return;if(p&START){g.mode=PAUSE;g.menu=0;return;}if(p&SELECT){if(episodes[g.level].mode==7){g.phase=!g.phase;burst(g.x,g.y,5,30);game_toast(g.phase?"时相：未来。寻找相同颜色的时间锚。":"时相：现在。寻找相同颜色的时间锚。");}else{g.return_mode=PLAY;g.mode=HELP;g.help_page=1;}return;}
 if(p&(LB|RB)){g.hero=(g.hero+(p&RB?1:4))%5;burst(g.x,g.y,g.hero,12);game_event(2);}
 if(p&TRIANGLE){if(g.energy>=100&&g.has_ring&&!g.power){g.energy=0;g.power=12*30;g.invincible=0;burst(g.x,g.y,5,60);game_toast("五种力量汇聚：地球超人！");game_event(6);}else if(!g.power)game_toast("完成目标、回收补给和战斗，可积满团队能量。");}
 if(p&CIRCLE&&g.dashcool<=0){g.dash=8;g.dashcool=38;game_event(7);}
 float dx=0,dy=0;if(buttons&LEFT||ax<72)dx=-1;if(buttons&RIGHT||ax>184)dx=1;if(buttons&UP||ay<72)dy=-1;if(buttons&DOWN||ay>184)dy=1;
 if(dx||dy){g.moving=1;g.anim++;g.dir=fabsf(dx)>fabsf(dy)?(dx>0?3:1):(dy>0?0:2);float l=hypotf(dx,dy);dx/=l;dy/=l;float speed=g.power?3.5f:2.3f;int tile=g.tiles[(int)g.y/TILE][(int)g.x/TILE];if(tile==3&&g.hero!=3&&!g.power)speed*=.65f;if(g.dash)speed=6;move(&g.x,&g.y,dx*speed,dy*speed);}
 if(buttons&CROSS)ring_attack();if(p&SQUARE)interact();
}
void game_update(void){g.frame++;if(g.toast_time)g.toast_time--;if(g.mode!=PLAY)return;g.tick++;g.elapsed++;if(g.power)g.power--;if(g.invincible)g.invincible--;if(g.cool)g.cool--;if(g.dash)g.dash--;if(g.dashcool)g.dashcool--;const Episode *ep=&episodes[g.level];
 if(ep->mode==6&&g.done<g.needed&&--g.mission_timer<=0){g.mode=FAIL;game_toast("时间耗尽。重试时优先寻找目标装置。");return;}
 if(g.tick%45==0&&g.tiles[(int)g.y/TILE][(int)g.x/TILE]==4){if(g.hero==3||g.power){g.tiles[(int)g.y/TILE][(int)g.x/TILE]=0;g.energy=(int)clampf(g.energy+2,0,100);burst(g.x,g.y,3,4);}else damage(g.save.difficulty?5:3);}
 if(ep->mode==9){for(int i=0;i<g.needed;i++){Object *o=&g.objs[i];if(!o->state&&distance(g.x,g.y,o->x,o->y)<85){o->timer++;if(o->timer%90==1&&o->timer<300)add_enemy(o->x+75,o->y+60,0);if(o->timer==300)game_toast("修复完成！靠近装置按方块确认。");}}}
 if(ep->mode==4&&g.escort_started&&distance(g.x,g.y,g.escortx,g.escorty)>28)escort_follow();
 if(g.done>=g.needed&&!g.boss_spawned&&ep->villain!=9&&ep->mode!=8&&ep->mode!=4&&ep->mode!=1&&ep->mode!=2){add_enemy(g.exitx-60,g.exity,3);g.boss_spawned=1;g.energy=100;game_toast("撤离区出现守卫装置！三角召唤地球超人。");}
 for(int i=0;i<MAXEN;i++){Enemy *e=&g.enemies[i];if(!e->active)continue;if(e->hp<=0){e->active=0;g.kills++;g.energy=(int)clampf(g.energy+9,0,100);burst(e->x,e->y,6,14);if(e->kind==3){g.boss_spawned=2;game_toast("封锁解除，前往绿色撤离点按方块。");}continue;}if(e->stun){e->stun--;continue;}float d=distance(g.x,g.y,e->x,e->y);if(d>310)continue;if(d<22)damage(e->kind==3?15:8);if(e->kind!=1&&d>30){float speed=e->kind==3?.5f:e->kind==2?.7f:1.f;move(&e->x,&e->y,(g.x-e->x)/d*speed,(g.y-e->y)/d*speed);}
 if(--e->cool<=0&&d>2){e->cool=e->kind==3?36:(g.save.difficulty?90:130);float dx=(g.x-e->x)/d,dy=(g.y-e->y)/d;shoot(e->x,e->y-10,dx*2.3f,dy*2.3f,6,1,8);if(e->kind==3){shoot(e->x,e->y-10,(dx*.86f-dy*.5f)*2.3f,(dy*.86f+dx*.5f)*2.3f,6,1,8);shoot(e->x,e->y-10,(dx*.86f+dy*.5f)*2.3f,(dy*.86f-dx*.5f)*2.3f,6,1,8);}}
 }
 for(int i=0;i<MAXSHOT;i++){Shot *s=&g.shots[i];if(!s->active)continue;s->x+=s->vx;s->y+=s->vy;if(--s->life<=0||s->x<20||s->y<20||s->x>=MW*TILE-20||s->y>=MH*TILE-20){s->active=0;continue;}if(g.tiles[(int)s->y/TILE][(int)s->x/TILE]==1){s->active=0;burst(s->x,s->y,s->hero,2);continue;}
 if(s->enemy){if(distance(s->x,s->y,g.x,g.y-12)<14){damage(s->damage);s->active=0;}}
 else {for(int j=0;j<MAXEN;j++){Enemy *e=&g.enemies[j];if(e->active&&distance(s->x,s->y,e->x,e->y-10)<(e->kind==3?26:17)){e->hp-=s->damage;e->stun=s->hero==4?100:s->hero==0?30:6;s->active=0;burst(s->x,s->y,s->hero,6);game_event(8);break;}}}
 }
 for(int i=0;i<MAXFX;i++){Particle *p=&g.fx[i];if(p->life){p->life--;p->x+=p->vx;p->y+=p->vy;p->vy+=.035f;}}
 g.camera_x=(int)clampf(g.x-240,0,MW*TILE-480);g.camera_y=(int)clampf(g.y-133,0,MH*TILE-226);
}
static uint32_t checksum(const Save *s){const uint8_t *p=(const uint8_t *)s;uint32_t h=2166136261u;for(size_t i=0;i<offsetof(Save,checksum);i++){h^=p[i];h*=16777619u;}return h;}
int game_validate_save(const Save *s){if(s->magic!=0x43504C54||s->version!=1||s->current<0||s->current>112||s->unlocked<0||s->unlocked>112||s->difficulty<0||s->difficulty>1||s->music<0||s->music>1||s->checksum!=checksum(s))return 0;for(int i=0;i<113;i++)if(s->stars[i]>3)return 0;return 1;}
static int readsave(const char *path,Save *s){FILE *f=fopen(path,"rb");if(!f)return 0;int ok=fread(s,1,sizeof(*s),f)==sizeof(*s);int tail=fgetc(f);fclose(f);return ok&&tail==EOF&&game_validate_save(s);}
int game_load(const char *path){Save s;char backup[540];if(readsave(path,&s)){g.save=s;return 1;}snprintf(backup,sizeof(backup),"%s.bak",path);if(readsave(backup,&s)){g.save=s;game_toast("主存档异常，已恢复备份。");return 2;}return 0;}
int game_save(const char *path){char temp[540],backup[540];if(strlen(path)>500)return 0;snprintf(temp,sizeof(temp),"%s.tmp",path);snprintf(backup,sizeof(backup),"%s.bak",path);Save s=g.save;s.sequence++;s.checksum=checksum(&s);FILE *f=fopen(temp,"wb");if(!f){g.save_error=1;return 0;}int ok=fwrite(&s,1,sizeof(s),f)==sizeof(s);if(fflush(f))ok=0;if(fclose(f))ok=0;Save verify;if(!ok||!readsave(temp,&verify)){remove(temp);g.save_error=1;return 0;}Save old;if(readsave(path,&old)){remove(backup);if(rename(path,backup)){remove(temp);g.save_error=1;return 0;}}else remove(path);if(rename(temp,path)){g.save_error=1;return 0;}g.save=s;g.dirty=0;g.save_error=0;return 1;}
