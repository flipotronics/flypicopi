#pragma once

#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif

extern time_t epochtime;

void time_init();
void setrtc(datetime_t *t);

#ifdef __cplusplus
}
#endif
