#include <stdio.h>
#include <pthread.h> // Thu vi?n pthread cho da lu?ng
#include <windows.h> // S? d?ng Sleep() cho Windows
#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_GENERATION
#define MA_ENABLE_MP3
#define MA_ENABLE_WAV
#define MA_ENABLE_FLAC
#include "miniaudio.h"
#include "database.h"
// Bi?n toàn c?c
int isRunning = 1;
volatile int isPlaying = 1; // Tr?ng thái nh?c (1: Ðang phát, 0: T?m d?ng)
float volume = 1.0f;        // Âm lu?ng m?c d?nh

ma_sound sound;
ma_uint32 sampleRate; // T?n s? m?u th?c t?

// Hàm hi?n th? ti?n trình phát nh?c
void *update_progress(void *arg) {
    while (isRunning) {
        if (isPlaying) {
            ma_uint64 cursor, length;
            ma_sound_get_cursor_in_pcm_frames(&sound, &cursor);
            ma_sound_get_length_in_pcm_frames(&sound, &length);
            printf("\rThoi gian: %llu/%llu giay", cursor / sampleRate, length / sampleRate);
            fflush(stdout);
        }
        Sleep(1000); // C?p nh?t m?i giây
    }
    return NULL;
}

// Hàm x? lý d?u vào t? ngu?i dùng
void *handle_input(void *arg) {
    while (isRunning) {
    	system("cls"); 
        printf("\n=== Trinh phat nhac MiniAudio ===\n");
        printf("1. Tam dung\n");
        printf("2. Tiep tuc phat\n");
        printf("3. Tang am luong\n");
        printf("4. Giam am luong\n");
        printf("5. Thoat\n");
        printf("=================================\n");
        printf("Lua chon cua ban: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Lua chon khong hop le. Vui long nhap so.\n");
            while (getchar() != '\n'); // Xóa b? d?m nh?p
            continue;
        }

        switch (choice) {
            case 1: // T?m d?ng
                ma_sound_stop(&sound);
                isPlaying = 0;
                printf("\nDa tam dung phat nhac.\n");
                break;

            case 2: // Ti?p t?c phát
                ma_sound_start(&sound);
                isPlaying = 1;
                printf("\nTiep tuc phat nhac.\n");
                break;

            case 3: // Tang âm lu?ng
                if (volume < 2.0f) {
                    volume += 0.1f;
                    ma_sound_set_volume(&sound, volume);
                    printf("\nAm luong hien tai: %.0f%%\n", volume * 100);
                } else {
                    printf("\nAm luong da o muc toi da.\n");
                }
                break;

            case 4: // Gi?m âm lu?ng
                if (volume > 0.0f) {
                    volume -= 0.1f;
                    ma_sound_set_volume(&sound, volume);
                    printf("\nAm luong hien tai: %.0f%%\n", volume * 100);
                } else {
                    printf("\nAm luong da o muc toi thieu.\n");
                }
                break;

            case 5: // Thoát
                printf("\nThoat chuong trinh.\n");
                isRunning = 0; // K?t thúc chuong trình
                break;

            default:
                printf("\nLua chon khong hop le. Vui long thu lai.\n");
                break;
        }
    }
    return NULL;
}

int playSong(Song *song ) {
    ma_result result;
    ma_engine engine;

    // Kh?i t?o engine
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Khong the khoi tao MiniAudio engine. Loi: %s\n", ma_result_description(result));
        return -1;
    }

    // Ðu?ng d?n t?i file nh?c
    
    const char *filePath = song->path;
    result = ma_sound_init_from_file(&engine, filePath, MA_SOUND_FLAG_STREAM, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        printf("Khong the load file nhac. Loi: %s\n", ma_result_description(result));
        ma_engine_uninit(&engine);
        return -1;
    }

    // L?y t?n s? m?u th?c t?
    sampleRate = ma_engine_get_sample_rate(&engine);

    // B?t d?u phát nh?c
    ma_sound_start(&sound);

    // Kh?i t?o các lu?ng
    pthread_t progress_thread, input_thread;
    pthread_create(&progress_thread, NULL, update_progress, NULL);
    pthread_create(&input_thread, NULL, handle_input, NULL);

    // Ch? các lu?ng k?t thúc
    pthread_join(progress_thread, NULL);
    pthread_join(input_thread, NULL);

    // Gi?i phóng tài nguyên
    ma_sound_stop(&sound);
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);

    return 0;
}

