#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef LIBSNES_DEBUG

#include <sys/timer.h>

#ifdef __cplusplus
extern "C" {
#endif

void dprintf_console(const char *fmt, ...);
void dprintf(float x, float y, float scale, const char *fmt, ...);

#define SNES_DBG(fmt, args...) do {\
   dprintf(0.1f, 0.1f, 1.0f, "libSNES: " fmt, ##args);\
   sys_timer_usleep(2000000);\
   } while(0)


#ifdef __cplusplus
}
#endif

#else
#define SNES_DBG(fmt, args...) ((void)0)
#endif // LIBSNES_DEBUG

#endif // __DEBUG_H
