#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "audio.h"

int getMaxID(Playlist *playlist)
{
    int maxID = 0; 
    Song *current = playlist->head;

    while (current != NULL)
    {
        if (current->id > maxID)
        {
            maxID = current->id;
        }
        current = current->next;
    }

    return maxID;
}

void addSong(Playlist *playlist)
{
    Song *newSong = (Song *)malloc(sizeof(Song));
    newSong->id = getMaxID(playlist) + 1;
    printf("Nhap ten bai hat: ");
    scanf(" %[^\n]", newSong->tittle);
    printf("Nhap ca si: ");
    scanf(" %[^\n]", newSong->artist);
    printf("Nhap duong dan: ");
    scanf(" %[^\n]", newSong->path);
    printf("Nhap thoi luong(giay): ");
    scanf("%d", &newSong->duration);
    newSong->prev = newSong->next = NULL;

    if (playlist->head == NULL)
    {
        playlist->head = playlist->tail = newSong;
    }
    else
    {
        playlist->tail->next = newSong;
        newSong->prev = playlist->tail;
        playlist->tail = newSong;
    }
    saveToFile(playlist, "playlist.bin");
    printf("Them bai hat thanh cong.\n");
}

void displayPlaylist(Playlist *playlist)
{
    int count=1;
    Song *current = playlist->head;
    if (current == NULL)
    {
        printf("Playlist rong.\n");
        return;
    }
    while (current != NULL)
    {
        printf("STT:%d Ma: %d, Ten: %s, Ca si: %s, Thoi luong: %d giay\n",
               count, current->id, current->tittle, current->artist, current->duration);
        current = current->next;
        count+=1;
    }
}
void chooseAndPlaySong(Playlist *playlist)
{
    if (playlist->head == NULL)
    {
        printf("Playlist rong. Khong co bai hat de phat.\n");
        return;
    }

    int choice, count = 1;
    Song *current = playlist->head;

    printf("Nhap so thu tu (STT) bai hat ma ban muon phat : ");
    scanf("%d", &choice);
	
    current = playlist->head;
    while (current != NULL)
    {
        if (count == choice)
        {
            playSong(current);
            createThreads();
            return;
        }
        current = current->next;
        count++;
    }

    printf("Khong tim thay bai hat voi so thu tutu: %d\n", choice);
}

