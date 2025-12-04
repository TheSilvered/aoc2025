#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define MAX_SIDE 200

bool map[MAX_SIDE][MAX_SIDE] = { 0 };
int side = MAX_SIDE;

void loadMap(const char *path) {
    FILE *file = fopen(path, "r");
    assert(file != NULL);

    for (int i = 0; i < MAX_SIDE; i++) {
        char c;
        for (int j = 0; j < MAX_SIDE; j++) {
            fread(&c, 1, 1, file);
            if (c == '@') {
                map[i][j] = true;
            } else if (c == '\n') {
                side = i;
                break;
            }
        }
        if (feof(file)) {
            break;
        }
    }

    fclose(file);
}

bool isFree1(int m_i, int m_j) {
    int count = 0;
    if (!map[m_i][m_j]) return false;
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            int i = m_i + di;
            int j = m_j + dj;
            if (i == m_i && j == m_j) continue;
            else if (i < 0 || i >= side || j < 0 || j >= side) continue;
            else if (map[i][j]) count++;
        }
    }
    return count < 4;
}

bool isFree2(int m_i, int m_j) {
    if (isFree1(m_i, m_j)) {
        map[m_i][m_j] = false;
        return true;
    }
    return false;
}

int main(void) {
    loadMap("input04.txt");
    int tot = 0;
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            if (isFree1(i, j)) tot++;
        }
    }
    printf("Total 1: %d\n", tot);

    tot = 0;
    bool changed = false;

    do {
        changed = false;
        for (int i = 0; i < side; i++) {
            for (int j = 0; j < side; j++) {
                if (isFree2(i, j)) {
                    tot++;
                    changed = true;
                }
            }
        }
    } while (changed);
    printf("Total 2: %d\n", tot);

    return 0;
}
