#ifndef DATABASE_H
#define DATABASE_H

typedef struct Song {
    int id;
    char tittle[100];
    char artist[100];
    char path[100];
    int duration; //seconds
    struct Song *prev, *next;
} Song;

typedef struct Playlist {
    Song *head;
    Song *tail;
} Playlist;

void initializePlaylist(Playlist *playlist);
void freePlaylist(Playlist *playlist);

void saveToFile(Playlist *playlist, const char *filename);
void loadFromFile(Playlist *playlist, const char *filename);

#endif


