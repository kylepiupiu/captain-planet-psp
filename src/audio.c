#include "audio.h"
#include <pspkernel.h>
#include <pspaudio.h>
#include <stdint.h>
static volatile int active=0,enabled=1,event_=0;
static int thread=-1,channel=-1;
static short buffer[1024] __attribute__((aligned(64)));
static int tri(unsigned phase){int x=(phase>>16)&65535;return x<32768?x*2-32768:98302-x*2;}
static int audio_thread(SceSize n,void *p){(void)n;(void)p;unsigned phase=0,bass=0,fxphase=0;int clock=0,step=0,fxleft=0,kind=0;const int melody[32]={330,392,440,392,330,294,262,294,330,392,523,494,440,392,330,294,262,330,392,440,523,440,392,330,294,330,392,330,294,262,247,294};const int roots[4]={131,98,110,87};
 while(active){int ev=event_;if(ev){event_=0;kind=ev;fxleft=kind==6?18000:kind==5?22000:3500;fxphase=0;}
 for(int i=0;i<1024;i++){clock++;if(clock>=11025){clock=0;step=(step+1)%32;}phase+=(unsigned)((uint64_t)melody[step]*4294967296ULL/44100);bass+=(unsigned)((uint64_t)roots[step/8]*4294967296ULL/44100);int env=clock<700?clock:clock>9000?(11025-clock)*700/2025:700;int sample=tri(phase)*env/700/22+tri(bass)/35;
 if(fxleft>0){int freq=kind==1?620+fxleft/6:kind==4?100+fxleft/22:kind==6?300+(18000-fxleft)/20:kind==5?440+(22000-fxleft)/18:kind==7?220+fxleft/4:800;fxphase+=(unsigned)((uint64_t)freq*4294967296ULL/44100);sample+=tri(fxphase)/(kind==4?12:18);fxleft--;}
 buffer[i]=enabled?(short)sample:0;}
 sceAudioOutputBlocking(channel,0x6000,buffer);
 }return 0;}
int audio_start(void){channel=sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL,1024,PSP_AUDIO_FORMAT_MONO);if(channel<0)return 0;active=1;thread=sceKernelCreateThread("planet_audio",audio_thread,0x18,0x2000,PSP_THREAD_ATTR_USER,NULL);if(thread<0){active=0;sceAudioChRelease(channel);return 0;}if(sceKernelStartThread(thread,0,NULL)<0){active=0;sceKernelDeleteThread(thread);sceAudioChRelease(channel);return 0;}return 1;}
void audio_control(int en,int ev){enabled=en;if(ev)event_=ev;}
void audio_stop(void){active=0;if(thread>=0){sceKernelWaitThreadEnd(thread,NULL);sceKernelDeleteThread(thread);}if(channel>=0)sceAudioChRelease(channel);}
