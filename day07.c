#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DIAG_SIDE 1000
char diag[MAX_DIAG_SIDE][MAX_DIAG_SIDE];
unsigned long long prevlvl[MAX_DIAG_SIDE] = { 0 };
unsigned long long currlvl[MAX_DIAG_SIDE] = { 0 };
int width = 0;
int height = 0;

void loadDiag(const char *path) {
    FILE *file = fopen(path, "r");
    assert(file != NULL);

    char *buf = NULL;
    size_t size = 0;
    ssize_t len = 0;

    while (!feof(file) && !ferror(file) && height < MAX_DIAG_SIDE) {
        for (int i = 0; i < MAX_DIAG_SIDE; i++) {
            char ch = getc(file);
            if (ch == '\n' || ch == '\0') {
                width = i;
                break;
            }
            diag[height][i] = ch;
        }
        height++;
    }
    height--;
    fclose(file);
}

int main(void) {
    loadDiag("input07.txt");

    for (int i = 0; i < width; i++) {
        if (diag[0][i] == 'S') {
            prevlvl[i] = 1;
            break;
        }
    }

    int splits = 0;
    for (int i = 1; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (prevlvl[j] && diag[i][j] == '^') {
                currlvl[j - 1] += prevlvl[j];
                currlvl[j + 1] += prevlvl[j];
                splits++;
            } else if (prevlvl[j]) {
                currlvl[j] += prevlvl[j];
            }
        }
        memcpy(prevlvl, currlvl, width * sizeof(unsigned long long));
        memset(currlvl, 0, width * sizeof(unsigned long long));
    }
    printf("Splits: %d\n", splits);
    unsigned long long timelines = 0;
    for (int i = 0; i < width; i++) {
        timelines += prevlvl[i];
    }
    printf("Timelines: %llu\n", timelines);
    return 0;
}
