#ifndef GAME_H
#define GAME_H
#include <stdint.h>
#define MW 48
#define MH 32
#define TILE 20
#define MAXOBJ 18
#define MAXEN 24
#define MAXSHOT 64
#define MAXFX 96
#define UP 0x10
#define RIGHT 0x20
#define DOWN 0x40
#define LEFT 0x80
#define START 0x8
#define SELECT 0x1
#define LB 0x100
#define RB 0x200
#define TRIANGLE 0x1000
#define CIRCLE 0x2000
#define CROSS 0x4000
#define SQUARE 0x8000
enum {TITLE,SELECTOR,BRIEF,PLAY,PAUSE,HELP,CLEAR,FAIL,CREDITS};
typedef struct {float x,y,vx,vy;int type,hero,hp,active,state,timer;} Object;
typedef struct {float x,y,vx,vy;int kind,hp,maxhp,active,cool,stun,phase;} Enemy;
typedef struct {float x,y,vx,vy;int active,life,hero,enemy,damage;} Shot;
typedef struct {float x,y,vx,vy;int life,type;} Particle;
typedef struct {uint32_t magic,version,sequence;int32_t current,unlocked,difficulty,music;uint8_t stars[113];uint8_t pad[3];uint32_t checksum;} Save;
typedef struct {
 Save save;uint8_t tiles[MH][MW];Object objs[MAXOBJ];Enemy enemies[MAXEN];Shot shots[MAXSHOT];Particle fx[MAXFX];
 int mode,return_mode,selection,season,menu,level,hero,dir,tick,anim,moving,prev,pressed,frame;
 int hp,maxhp,energy,power,invincible,cool,dash,dashcool,kills,done,needed,stage,timer,elapsed;
 int mission_timer,escort_started,escort_done,restricted,has_ring,won,exittimer,phase,dirty,save_error,asset_error,sound_event;
 int camera_x,camera_y,help_page,exit_request,boss_spawned,stars,healcount;
 unsigned rng;float x,y,startx,starty,exitx,exity,escortx,escorty;
 char toast[160];int toast_time;
} Game;
extern Game g;
void game_init(void);
void game_start(int level);
void game_input(unsigned buttons,int ax,int ay);
void game_update(void);
int game_blocked(float x,float y);
int game_load(const char *path);
int game_save(const char *path);
int game_validate_save(const Save *s);
int game_readable(int level);
void game_toast(const char *text);
void game_event(int type);
#endif
