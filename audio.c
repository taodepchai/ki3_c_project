#define MINIAUDIO_IMPLEMENTATION
#include <stdio.h>
#include <pthread.h>
#include <windows.h>
#include "audio.h"
#include "database.h"
#include "miniaudio.h"


volatile int isRunning = 1;
volatile int isPlaying = 1;
float volume = 1.0f;

ma_engine engine;
ma_sound sound;
ma_uint32 sampleRate;


int initializeAudio() {
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Khong the khoi tao MiniAudio engine. Loi: %s\n", ma_result_description(result));
        return -1;
    }
    sampleRate = ma_engine_get_sample_rate(&engine);
    printf("MiniAudio da khoi tao thanh cong.\n");
    return 0;
}

void createThreads() {
    pthread_t progress_thread, input_thread;
    isRunning = 1;
    pthread_create(&progress_thread, NULL, updateProgress, NULL);
    pthread_create(&input_thread, NULL, handleInput, NULL);


    pthread_join(progress_thread, NULL);
    pthread_join(input_thread, NULL);
}


int playSong(Song *song) {
    ma_result result = ma_sound_init_from_file(&engine, song->path, MA_SOUND_FLAG_STREAM, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        printf("Khong the load file nhac. Loi: %s\n", ma_result_description(result));
        return -1;
    }
    ma_sound_set_volume(&sound, volume); 
    ma_sound_start(&sound);            
    isPlaying = 1;
    printf("Dang phat: %s\n", song->tittle);
    return 0;
}


void *updateProgress(void *arg) {
    while (isRunning) {
        if (isPlaying) {
            ma_uint64 cursor, length;
            ma_sound_get_cursor_in_pcm_frames(&sound, &cursor);
            ma_sound_get_length_in_pcm_frames(&sound, &length);
            printf("\rThoi gian: %llu/%llu giay", cursor / sampleRate, length / sampleRate);
            fflush(stdout);
        }
        Sleep(1000);
    }
    return NULL;
}


void *handleInput(void *arg) {
    while (isRunning) {
        printf("\n1. Tam dung\n2. Tiep tuc phat\n3. Tang am luong\n4. Giam am luong\n5. Thoat\nLua chon: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Nhap so hop le.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                ma_sound_stop(&sound);
                isPlaying = 0;
                printf("\nDa tam dung phat nhac.\n");
                break;
            case 2:
                ma_sound_start(&sound);
                isPlaying = 1;
                printf("\nTiep tuc phat nhac.\n");
                break;
            case 3:
                if (volume < 2.0f) {
                    volume += 0.1f;
                    ma_sound_set_volume(&sound, volume);
                    printf("\nAm luong hien tai: %.0f%%\n", volume * 100);
                } else {
                    printf("\nAm luong da o muc toi da.\n");
                }
                break;
            case 4:
                if (volume > 0.0f) {
                    volume -= 0.1f;
                    ma_sound_set_volume(&sound, volume);
                    printf("\nAm luong hien tai: %.0f%%\n", volume * 100);
                } else {
                    printf("\nAm luong da o muc toi thieu.\n");
                }
                break;
            case 5:
                isRunning = 0;
                printf("\nThoat chuong trinh.\n");
                ma_sound_stop(&sound);
                ma_sound_uninit(&sound);
                break;
            default:
                printf("Lua chon khong hop le.\n");
                break;
        }
    }
    return NULL;
}



void cleanupAudio() {
    ma_sound_stop(&sound);
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
    printf("Da giai phong tai nguyen.\n");
}

