#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct { int x, y; } Tile;
typedef unsigned long long u64;
Tile *tiles = NULL;
bool clockwise = true;

void loadTiles(const char *path) {
    FILE *file = fopen(path, "r");
    assert(file != NULL);

    char *buf = NULL;
    size_t bufSize = 0;
    ssize_t bufLen = 0;

    while ((bufLen = getline(&buf, &bufSize, file)) != -1) {
        char *p = buf;
        int x = strtol(p, &p, 10);
        p++;
        int y = strtol(p, &p, 10);
        Tile tile = { x, y };
        arrput(tiles, tile);
    }
    fclose(file);

    // Find topmost edge
    Tile a, b;
    bool horizFound = false;

    for (int i = 0; i < arrlen(tiles); i++) {
        Tile e1 = tiles[i];
        Tile e2 = tiles[(i + 1) % arrlen(tiles)];
        if (e1.y != e2.y) continue; // only care about horizontal edges
        if (!horizFound || e1.y < a.y) {
            a = e1;
            b = e2;
            horizFound = true;
        }
    }
    clockwise = a.x < b.x;
}

int absi32(int a) {
    return a < 0 ? -a : a;
}

u64 getArea(Tile a, Tile b) {
    int width = abs(a.x - b.x) + 1;
    int height = abs(a.y - b.y) + 1;
    return (u64)width * height;
}

int max_i32(int a, int b) {
    return a > b ? a : b;
}

int min_i32(int a, int b) {
    return a < b ? a : b;
}

void pTile(Tile t, bool newline) {
    printf("%d,%d  %s", t.x, t.y, newline ? "\n" : "");
}

typedef enum {
    LEFT = 1,
    RIGHT = -1,
    UP = 2,
    DOWN = -2
} Direction;

void getNormals(int tileIdx, Direction *outPrevNorm, Direction *outNextNorm) {
    Tile tile = tiles[tileIdx];
    Tile next = tiles[(tileIdx + 1) % arrlen(tiles)];
    Tile prev = tiles[tileIdx == 0 ? arrlen(tiles) - 1 : tileIdx - 1];
    Direction prevNorm = UP;
    Direction nextNorm = UP;

    if (prev.x == tile.x) {
        prevNorm = prev.y < tile.y ? RIGHT : LEFT;
    } else {
        prevNorm = prev.x < tile.x ? UP : DOWN;
    }

    if (tile.x == next.x) {
        nextNorm = tile.y < next.y ? RIGHT : LEFT;
    } else {
        nextNorm = tile.x < next.x ? UP : DOWN;
    }
    if (clockwise) {
        *outPrevNorm = prevNorm;
        *outNextNorm = nextNorm;
    } else {
        *outPrevNorm = -nextNorm;
        *outNextNorm = -prevNorm;
    }
}

bool checkInside(int aIdx, int bIdx) {
    Tile a = tiles[aIdx];
    Tile b = tiles[bIdx];
    int left = min_i32(a.x, b.x);
    int right = max_i32(a.x, b.x);
    int top = min_i32(a.y, b.y);
    int bottom = max_i32(a.y, b.y);

    for (int i = 0; i < arrlen(tiles); i++) {
        Tile e1 = tiles[i];
        Tile e2 = tiles[(i + 1) % arrlen(tiles)];

        int eLeft = min_i32(e1.x, e2.x);
        int eRight = max_i32(e1.x, e2.x);
        int eTop = min_i32(e1.y, e2.y);
        int eBottom = max_i32(e1.y, e2.y);
        if (left >= eRight || right <= eLeft || top >= eBottom || bottom <= eTop)
            continue;
        // Edge colliding
        return false;
    }

    // Either fully inside or outside (except for maybe the borders)
    if (left == right || top == bottom) return true;

    int topIdx = a.y < b.y ? aIdx : bIdx;
    Direction prevNorm, nextNorm;
    getNormals(topIdx, &prevNorm, &nextNorm);
    if (tiles[topIdx].x == left) {
        return !(prevNorm == RIGHT || nextNorm == RIGHT
            || (prevNorm == DOWN && nextNorm == DOWN));
    } else {
        return !(prevNorm == DOWN || nextNorm == DOWN
            || (prevNorm == LEFT && nextNorm == LEFT));
    }
}

int main(void) {
    loadTiles("input09.txt");

    u64 maxArea = 0;
    for (int i = 0; i + 1 < arrlen(tiles); i++) {
        for (int j = i + 1; j < arrlen(tiles); j++) {
            u64 area = getArea(tiles[i], tiles[j]);
            if (area > maxArea) maxArea = area;
        }
    }
    printf("Max area 1: %llu\n", maxArea);

    maxArea = 0;
    for (int i = 0; i + 1 < arrlen(tiles); i++) {
        for (int j = i + 1; j < arrlen(tiles); j++) {
            u64 area = getArea(tiles[i], tiles[j]);
            if (area > maxArea && checkInside(i, j)) maxArea = area;
        }
    }
    printf("Max area 2: %llu\n", maxArea);

    return 0;
}
