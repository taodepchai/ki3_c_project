#include "database.h"
#include "input_output.h"
#include "data_processing.h"
#include <stdio.h>
#include <stdlib.h>

void displayMenu() {
    printf("=== Playlist Menu ===\n");
    printf("1. Them bai hat\n");
    printf("2. Hien thi playlist\n");
    printf("3. Sap xep playlist\n");
    printf("4. Tim kiem bai hat theo ma\n");
    printf("5. Xoa bai hat\n");
    printf("6. Chinh sua bai hat\n");
    printf("7. Thoat\n");
    printf("=====================\n");
}

void handleMenu(Playlist *playlist) {
    int choice;
    do {
        displayMenu();
        printf("Chon chuc nang: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addSong(playlist);
                break;
            case 2:
                displayPlaylist(playlist);
                chooseAndPlaySong(playlist);
                
                break;
            case 3:
                sortPlaylist(playlist);
                break;
            case 4:
                searchSong(playlist);
                break;
            case 5:
                deleteSong(playlist);
                break;
            case 6:
                editSong(playlist);
                break;
            case 7:
                printf("Thoat chuong trinh.\n");
                break;
            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
                break;
        }
    } while (choice != 8);
}


