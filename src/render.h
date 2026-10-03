#ifndef RENDER_H
#define RENDER_H
#include <stdint.h>
int render_load(const char *dir);
void render_shutdown(void);
void render_frame(uint32_t *frame,int stride);
#endif
