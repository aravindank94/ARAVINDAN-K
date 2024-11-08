#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char name[100];
    char author[100];
    struct Song *prev;
    struct Song *next;
} Song;

typedef struct Playlist {
    Song *head;
    Song *tail;
    Song *current;
} Playlist;

void initPlaylist(Playlist *playlist) {
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->current = NULL;
}

Song* createSong(const char *name, const char *author) {
    Song *newSong = (Song*)malloc(sizeof(Song));
    strcpy(newSong->name, name);
    strcpy(newSong->author, author);
    newSong->prev = NULL;
    newSong->next = NULL;
    return newSong;
}

void addSong(Playlist *playlist, const char *name, const char *author) {
    Song *newSong = createSong(name, author);
    if (playlist->tail == NULL) {
        playlist->head = newSong;
        playlist->tail = newSong;
    } else {
        playlist->tail->next = newSong;
        newSong->prev = playlist->tail;
        playlist->tail = newSong;
    }
}

void addSongAtIndex(Playlist *playlist, const char *name, const char *author, int index) {
    Song *newSong = createSong(name, author);
    if (index == 0) {
        newSong->next = playlist->head;
        if (playlist->head != NULL) {
            playlist->head->prev = newSong;
        }
        playlist->head = newSong;
        if (playlist->tail == NULL) {
            playlist->tail = newSong;
        }
        return;
    }

    Song *temp = playlist->head;
    for (int i = 0; temp != NULL && i < index - 1; i++) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Index out of range!\n");
        free(newSong);
        return;
    }

    newSong->next = temp->next;
    newSong->prev = temp;
    if (temp->next != NULL) {
        temp->next->prev = newSong;
    }
    temp->next = newSong;
    if (newSong->next == NULL) {
        playlist->tail = newSong;
    }
}

void removeSong(Playlist *playlist, const char *name) {
    Song *temp = playlist->head;
    while (temp != NULL && strcmp(temp->name, name) != 0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Song not found!\n");
        return;
    }
    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    } else {
        playlist->head = temp->next;
    }
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    } else {
        playlist->tail = temp->prev;
    }
    free(temp);
}

void removeSongAtIndex(Playlist *playlist, int index) {
    if (index == 0 && playlist->head != NULL) {
        Song *temp = playlist->head;
        playlist->head = playlist->head->next;
        if (playlist->head != NULL) {
            playlist->head->prev = NULL;
        } else {
            playlist->tail = NULL;
        }
        free(temp);
        return;
    }

    Song *temp = playlist->head;
    for (int i = 0; temp != NULL && i < index; i++) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Index out of range!\n");
        return;
    }

    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    }
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }
    if (temp == playlist->tail) {
        playlist->tail = temp->prev;
    }
    free(temp);
}

void playSong(Playlist *playlist, const char *name) {
    Song *temp = playlist->head;
    while (temp != NULL && strcmp(temp->name, name) != 0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Song not found!\n");
        return;
    }
    playlist->current = temp;
    printf("Now playing: %s by %s\n", playlist->current->name, playlist->current->author);
}

void playNextSong(Playlist *playlist) {
    if (playlist->current == NULL || playlist->current->next == NULL) {
        printf("No next song in the playlist!\n");
        return;
    }
    playlist->current = playlist->current->next;
    printf("Now playing: %s by %s\n", playlist->current->name, playlist->current->author);
}

void playPreviousSong(Playlist *playlist) {
    if (playlist->current == NULL || playlist->current->prev == NULL) {
        printf("No previous song in the playlist!\n");
        return;
    }
    playlist->current = playlist->current->prev;
    printf("Now playing: %s by %s\n", playlist->current->name, playlist->current->author);
}

void displayPlaylist(Playlist *playlist) {
    Song *temp = playlist->head;
    while (temp != NULL) {
        printf("%s by %s\n", temp->name, temp->author);
        temp = temp->next;
    }
}

void handleCommands(Playlist *playlist) {
    char command[100];
    char name[100];
    char author[100];
    int index;

    while (1) {
        printf("\nEnter command (add, addat, remove, removeat, play, next, prev, display, quit): ");
        scanf("%s", command);

        if (strcmp(command, "add") == 0) {
            printf("Enter song name: ");
            scanf(" %[^\n]", name);
            printf("Enter author name: ");
            scanf(" %[^\n]", author);
            addSong(playlist, name, author);
        } else if (strcmp(command, "addat") == 0) {
            printf("Enter song name: ");
            scanf(" %[^\n]", name);
            printf("Enter author name: ");
            scanf(" %[^\n]", author);
            printf("Enter index: ");
            scanf("%d", &index);
            addSongAtIndex(playlist, name, author, index);
        } else if (strcmp(command, "remove") == 0) {
            printf("Enter song name: ");
            scanf(" %[^\n]", name);
            removeSong(playlist, name);
        } else if (strcmp(command, "removeat") == 0) {
            printf("Enter index: ");
            scanf("%d", &index);
            removeSongAtIndex(playlist, index);
        } else if (strcmp(command, "play") == 0) {
            printf("Enter song name: ");
            scanf(" %[^\n]", name);
            playSong(playlist, name);
        } else if (strcmp(command, "next") == 0) {
            playNextSong(playlist);
        } else if (strcmp(command, "prev") == 0) {
            playPreviousSong(playlist);
        } else if (strcmp(command, "display") == 0) {
            displayPlaylist(playlist);
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Invalid command!\n");
        }
    }
}

int main() {
    Playlist myPlaylist;
    initPlaylist(&myPlaylist);

    handleCommands(&myPlaylist);

    return 0;
}
