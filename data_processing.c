#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include <string.h>

void sortPlaylist(Playlist *playlist) {
    if (playlist->head == NULL) return;

    int option;
    printf("Lua chon tieu chi sap xep:\n");
    printf("1. Theo thoi luong (tang dan)\n");
    printf("2. Theo ten bai hat (a-z)\n");
    printf("3. Theo ten ca si (a-z)\n");
    printf("Chon: ");
    scanf("%d", &option);

    for (Song *i = playlist->head; i != NULL; i = i->next) {
        for (Song *j = i->next; j != NULL; j = j->next) {
            int swap = 0;

            if (option == 1 && i->duration > j->duration) swap = 1;
            else if (option == 2 && strcmp(i->tittle, j->tittle) > 0) swap = 1;
            else if (option == 3 && strcmp(i->artist, j->artist) > 0) swap = 1;

            if (swap) {
                Song temp = *i;
                *i = *j;
                *j = temp;

                Song *tempNext = i->next;
                i->next = j->next;
                j->next = tempNext;
            }
        }
    }
    saveToFile(playlist, "playlist.bin");
    printf("Sap xep thanh cong.\n");
}

void searchSong(Playlist *playlist) {
    int id;
    printf("Nhap ma bai hat can tim: ");
    scanf("%d", &id);
    Song *current = playlist->head;
    while (current != NULL) {
        if (current->id == id) {
            printf("Tim thay bai hat: %s - %s - %s - %d giay\n",
                   current->tittle, current->artist, current->path, current->duration);
            return;
        }
        current = current->next;
    }
    printf("Khong tim thay bai hat.\n");
}

void editSong(Playlist *playlist) {
    char id[37];
    printf("Nhap ID bai hat can sua: ");
    scanf(" %[^\n]", id);

    Song *current = playlist->head;

    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            printf("Nhap ten moi: ");
            scanf(" %[^\n]", current->tittle);
            printf("Nhap ca si moi: ");
            scanf(" %[^\n]", current->artist);
            printf("Nhap duong dan moi: ");
            scanf(" %[^\n]", current->path);
            printf("Nhap thoi luong moi (giay): ");
            scanf("%d", &current->duration);

            saveToFile(playlist, "playlist.bin");
            printf("Cap nhat bai hat thanh cong va da luu vao file.\n");
            return;
        }
        current = current->next;
    }
    printf("Khong tim thay bai hat can sua.\n");
}

void deleteSong(Playlist *playlist) {
    int id;
    printf("Nhap ma bai hat can xoa: ");
    scanf("%d", &id);
    Song *current = playlist->head;

    while (current != NULL) {
        if (current->id == id) {
            if (current->prev) current->prev->next = current->next;
            if (current->next) current->next->prev = current->prev;
            if (current == playlist->head) playlist->head = current->next;
            if (current == playlist->tail) playlist->tail = current->prev;
            free(current);
            saveToFile(playlist, "playlist.bin");
            printf("Xoa bai hat thanh cong.\n");
            return;
        }
        current = current->next;
    }
    printf("Khong tim thay bai hat can xoa.\n");
}


