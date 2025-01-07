#include <stdio.h>
#include <stdlib.h>
#include "database.h"

void initializePlaylist(Playlist *playlist) {
    playlist->head = NULL;
    playlist->tail = NULL;
}

void freePlaylist(Playlist *playlist) {
    Song *current = playlist->head;
    while (current != NULL) {
        Song *temp = current;
        current = current->next;
        free(temp);
    }
    playlist->head = playlist->tail = NULL;
}

void saveToFile(Playlist *playlist, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Khong the mo file de ghi.\n");
        return;
    }

    Song *current = playlist->head;
    while (current) {
        fwrite(current, sizeof(Song), 1, file);
        current = current->next;
    }
    fclose(file);
    printf("Da luu du lieu vao file: %s\n", filename);
}

void loadFromFile(Playlist *playlist, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Khong the mo file de doc.\n");
        return;
    }

    Song *newSong;
    while ((newSong = (Song *)malloc(sizeof(Song))) != NULL && fread(newSong, sizeof(Song), 1, file) == 1) {
        newSong->prev = newSong->next = NULL;

        if (playlist->head == NULL) {
            playlist->head = playlist->tail = newSong;
        } else {
            playlist->tail->next = newSong;
            newSong->prev = playlist->tail;
            playlist->tail = newSong;
        }
    }
    fclose(file);
    printf("Da tai du lieu tu file: %s\n", filename);
}


