#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef unsigned int bits;

typedef struct {
    int *joltages;
    bits *buttons;
    bits endState;
    int bitCount;
} Machine;

Machine *machs = NULL;

void printBits(bits b, int count);

void parseMachine(char *line) {
    line++;
    Machine mach = { 0 };
    int count = 0;
    while (*line != ']') {
        mach.endState <<= 1;
        if (*line == '#') {
            mach.endState |= 1;
        } else
            assert(*line == '.');
        line++;
        count++;
    }
    mach.bitCount = count;
    line += 2;
    while (*line != '{') {
        line++;
        bits button = 0;
        while (*line != ' ') {
            int shift = strtol(line, &line, 10);
            line++;
            button |= 1 << (count - shift - 1);
        }
        line++;
        arrput(mach.buttons, button);
    }
    line++;
    while (*line && *line != '\n') {
        int joltage = strtol(line, &line, 10);
        line++;
        arrput(mach.joltages, joltage);
    }
    arrput(machs, mach);
}

void loadMachs(const char *path) {
    FILE *file = fopen(path, "r");
    assert(file != NULL);

    char *buf = NULL;
    size_t bufSize = 0;
    ssize_t bufLen = 0;
    while ((bufLen = getline(&buf, &bufSize, file)) != -1) {
        parseMachine(buf);
    }
    free(buf);
    fclose(file);
}

void printBits(bits b, int count) {
    putchar('[');
    for (int i = 0; i < count; i++) {
        int shift = count - i - 1;
        if ((b >> shift) & 1) putchar('#');
        else putchar('.');
    }
    putchar(']');
}

int minSteps(Machine mach, int count, bits endState) {
    int min = -1;
    for (int i = 0; i < count; i++) {
        int idx = count - i - 1;
        if ((endState ^ mach.buttons[idx]) == 0) return 1;
        int steps = minSteps(mach, idx, endState ^ mach.buttons[idx]);
        if (steps < 0) continue;
        else if (steps + 1 < min || min < 0) min = steps + 1;
    }
    return min;
}

int main(void) {
    loadMachs("input10.txt");

    int tot = 0;
    for (int i = 0; i < arrlen(machs); i++) {
        tot += minSteps(machs[i], arrlen(machs[i].buttons), machs[i].endState);
    }
    printf("Total 1: %d\n", tot);

    return 0;
}
