#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

int *readMoves(const char *fileName) {
    FILE *input = fopen(fileName, "r");
    assert(input != NULL);

    size_t len = 10;
    char *buf = malloc(len * sizeof(char));
    int *moves = NULL;

    ssize_t bytesRead = -1;
    while ((bytesRead = getline(&buf, &len, input)) != -1) {
        char dir = buf[0];
        int move = atoi(buf + 1);
        if (dir == 'L') {
            arrput(moves, -move);
        } else {
            arrput(moves, move);
        }
    }

    free(buf);
    return moves;
}

int imod(int a, int b) {
    return ((a % b) + b) % b;
}

int main(void) {
    int *moves = readMoves("input01.txt");
    int dial = 50;
    int zeroCountP1 = 0;

    for (int i = 0; i < arrlen(moves); i++) {
        dial = imod(dial + moves[i], 100);
        if (dial == 0) {
            zeroCountP1++;
        }
    }
    printf("Password 1: %d\n", zeroCountP1);

    dial = 50;
    int zeroCountP2 = 0;

    for (int i = 0; i < arrlen(moves); i++) {
        int leftOver;
        if (moves[i] < 0) {
            zeroCountP2 -= moves[i] / 100;
            leftOver = -imod(-moves[i], 100);
        } else {
            zeroCountP2 += moves[i] / 100;
            leftOver = imod(moves[i], 100);
        }
        if (dial + leftOver > 100 || dial + leftOver < 0 || dial == 0) {
            zeroCountP2++;
        }
        dial = imod(dial + moves[i], 100);
    }
    printf("Password 2: %d\n", zeroCountP2);

    arrfree(moves);
    return 0;
}
