TARGET = planet_psp
OBJS = src/main.o src/game.o src/render.o src/audio.o
CFLAGS = -O2 -G0 -Wall -Wextra -MMD -MP -std=gnu99 -Wno-misleading-indentation
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
BUILD_PRX = 1
PSP_FW_VERSION = 660
PSP_LARGE_MEMORY = 0
LIBS = -lpspaudio -lpsppower -lm
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Captain Planet - Element Mission v0.1
PSP_EBOOT_ICON = ICON0.PNG
PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
-include $(OBJS:.o=.d)
