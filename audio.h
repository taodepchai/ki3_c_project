#ifndef AUDIO_H
#define AUDIO_H
#include "miniaudio.h"
#include "database.h"

extern volatile int isRunning;
extern volatile int isPlaying;
extern float volume;

extern ma_engine engine;
extern ma_sound sound;
extern ma_uint32 sampleRate;




int initializeAudio();
int playSong(Song *song);
void createThreads();
void cleanupAudio();
void *updateProgress(void *arg);
void *handleInput(void *arg);


#endif

