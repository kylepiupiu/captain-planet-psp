#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspiofilemgr.h>
#include <psppower.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "render.h"
#include "audio.h"
PSP_MODULE_INFO("Planet Element Mission",0,0,1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
PSP_MAIN_THREAD_STACK_SIZE_KB(256);
PSP_HEAP_SIZE_KB(8192);
static volatile int exit_=0;
static uint32_t frame[512*272] __attribute__((aligned(64)));
static char boot_path[540]="ms0:/PLANET_BOOT.LOG";
static void log_stage(const char *s){int fd=sceIoOpen(boot_path,PSP_O_WRONLY|PSP_O_CREAT|PSP_O_APPEND,0777);if(fd>=0){sceIoWrite(fd,s,strlen(s));sceIoWrite(fd,"\r\n",2);sceIoClose(fd);}}
static int exit_cb(int a,int b,void *p){(void)a;(void)b;(void)p;exit_=1;return 0;}
static int callbacks(SceSize a,void *b){(void)a;(void)b;int id=sceKernelCreateCallback("planet_exit",exit_cb,NULL);if(id>=0)sceKernelRegisterExitCallback(id);sceKernelSleepThreadCB();return 0;}
int main(int argc,char *argv[]){pspDebugScreenInit();pspDebugScreenPrintf("CAPTAIN PLANET PSP\nStarting native adventure...\n");char folder[512]="ms0:/PSP/GAME/PLANET";if(argc>0&&argv&&argv[0]&&strchr(argv[0],'/')){snprintf(folder,sizeof(folder),"%s",argv[0]);*strrchr(folder,'/')=0;}if(!strncmp(folder,"ef0:",4))snprintf(boot_path,sizeof(boot_path),"ef0:/PLANET_BOOT.LOG");int fd=sceIoOpen(boot_path,PSP_O_CREAT|PSP_O_TRUNC|PSP_O_WRONLY,0777);if(fd>=0)sceIoClose(fd);log_stage("P01 MAIN; debug display ready");sceIoChdir(folder);log_stage("P02 DIRECTORY");int tid=sceKernelCreateThread("planet_callbacks",callbacks,0x11,0x2000,PSP_THREAD_ATTR_USER,NULL);if(tid>=0)sceKernelStartThread(tid,0,NULL);sceCtrlSetSamplingCycle(0);sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);scePowerSetClockFrequency(333,333,166);
 char assetdir[600];snprintf(assetdir,sizeof(assetdir),"%s/assets",folder);game_init();game_load("save.dat");int assets_ok=render_load(assetdir);g.asset_error=!assets_ok;log_stage(assets_ok?"P03 ALL_ASSETS_READY":"P03 ASSETS_MISSING");if(!assets_ok){pspDebugScreenPrintf("Missing assets. Copy the WHOLE PLANET folder.\nPress O to exit.\n");while(!exit_){SceCtrlData pad;sceCtrlPeekBufferPositive(&pad,1);if(pad.Buttons&PSP_CTRL_CIRCLE)break;sceKernelDelayThread(16000);}render_shutdown();sceKernelExitGame();return 1;}
 log_stage("P04 GAME_READY");render_frame(frame,512);memcpy((void*)0x44000000,frame,sizeof(frame));int rc=sceDisplaySetFrameBuf((void*)0x04000000,512,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_IMMEDIATE);if(rc<0){log_stage("P05 DISPLAY_FAILED");sceKernelExitGame();return 2;}log_stage("P05 FIRST_FRAME_READY");sceDisplayWaitVblankStartCB();log_stage("P06 VBLANK_READY");int sound=audio_start();log_stage(sound?"P07 AUDIO_READY":"P07 AUDIO_DISABLED");int frames=0;
 while(!exit_&&!g.exit_request){SceCtrlData pad;memset(&pad,0,sizeof(pad));pad.Lx=pad.Ly=128;sceCtrlPeekBufferPositive(&pad,1);game_input(pad.Buttons,pad.Lx,pad.Ly);game_update();if(g.dirty){game_save("save.dat");if(g.save_error)log_stage("SAVE_FAILED");}audio_control(g.save.music,g.sound_event);g.sound_event=0;render_frame(frame,512);memcpy((void*)0x44000000,frame,sizeof(frame));sceDisplayWaitVblankStartCB();sceKernelDelayThread(16000);if(++frames==2)log_stage("P08 LOOP_RUNNING");}
 game_save("save.dat");audio_stop();render_shutdown();log_stage("P09 NORMAL_EXIT");sceKernelExitGame();return 0;}
