#include <stdio.h>
#include "menu.h"
#include "database.h"
#include "audio.h"
int main(){

    Playlist playlist;
    initializePlaylist(&playlist);
    loadFromFile(&playlist, "playlist.bin");
    initializeAudio();
    handleMenu(&playlist);
    cleanupAudio();
    freePlaylist(&playlist);
    return 0;
}
