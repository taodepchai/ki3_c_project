#ifndef PLAYSONG_H
#define PLAYSONG_H
#include "database.h"
void *update_progress(void *arg);
void *handle_input(void *arg);
int playSong(Song *song );
#endif
