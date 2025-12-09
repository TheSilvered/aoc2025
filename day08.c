#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

struct jbox { int x, y, z; int circuit_idx; };
struct dist { int box1, box2; double dist; };

struct jbox *boxes = NULL;
struct dist *dists = NULL;
int **circuits = NULL;

void load_jboxes(const char *path) {
    FILE *file = fopen(path, "r");
    assert(file != NULL);

    char *buf = NULL;
    size_t buf_sz = 0;
    ssize_t buf_len = 0;
    while ((buf_len = getline(&buf, &buf_sz, file)) != -1) {
        char *bufp = buf;
        int x = strtol(bufp, &bufp, 10);
        bufp++;
        int y = strtol(bufp, &bufp, 10);
        bufp++;
        int z = strtol(bufp, &bufp, 10);
        struct jbox box = { .x = x, .y = y, .z = z, .circuit_idx = -1 };
        arrput(boxes, box);
    }

    fclose(file);

    for (int i = 0; i < arrlen(boxes); i++) {
        arrput(circuits, NULL);
        arrput(circuits[i], i);
        boxes[i].circuit_idx = i;
    }
}

double jbox_dist(const struct jbox *box1, const struct jbox *box2) {
    double dx = box1->x - box2->x;
    double dy = box1->y - box2->y;
    double dz = box1->z - box2->z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

int cmp_dists(const void *d1, const void *d2) {
    if (((struct dist *)d1)->dist < ((struct dist *)d2)->dist)
        return -1;
    return 1;
}

void get_distances(const struct jbox *boxes) {
    for (int i = 0; i < arrlen(boxes) - 1; i++) {
        for (int j = i + 1; j < arrlen(boxes); j++) {
            struct dist dist = {
                .box1 = i,
                .box2 = j,
                .dist = jbox_dist(boxes + i, boxes + j)
            };
            arrput(dists, dist);
        }
    }
    qsort(dists, arrlen(dists), sizeof(struct dist), cmp_dists);
}

void print_boxes(void) {
    printf("Boxes:\n");
    for (int i = 0; i < arrlen(boxes); i++) {
        printf("%3d %d,%d,%d (%d)\n", i, boxes[i].x, boxes[i].y, boxes[i].z, boxes[i].circuit_idx);
    }
}

void print_circuits(void) {
    printf("Circuits:\n");
    for (int i = 0; i < arrlen(circuits); i++) {
        printf("%3d [%zi] ", i, arrlen(circuits[i]));
        if (circuits[i] == NULL) {
            printf("-\n");
            continue;
        }
        for (int j = 0; j < arrlen(circuits[i]); j++) {
            printf("%d", circuits[i][j]);
            if (j + 1 < arrlen(circuits[i])) printf("-");
        }
        printf("\n");
    }
}

bool connect(int b1, int b2) {
    int idx1 = boxes[b1].circuit_idx;
    int idx2 = boxes[b2].circuit_idx;
    if (idx1 == idx2) return false;

    int *first = arraddnptr(circuits[idx1], arrlen(circuits[idx2]));
    for (int i = 0; i < arrlen(circuits[idx2]); i++) {
        boxes[circuits[idx2][i]].circuit_idx = idx1;
    }
    memcpy(first, circuits[idx2], arrlen(circuits[idx2]) * sizeof(int));
    arrfree(circuits[idx2]);
    return true;
}

int cmp_llu_desc(const void *a, const void *b) {
    if (*(unsigned long long *)a <= *(unsigned long long *)b) return 1;
    return -1;
}

int main(void) {
#if 0
    const char *path = "testInput.txt";
    const int reps = 10;
#else
    const char *path = "input08.txt";
    const int reps = 1000;
#endif
    load_jboxes(path);
    get_distances(boxes);

    int last1 = 0, last2 = 0;
    for (int i = 0; i < reps; i++) {
        // printf("%d<->%d = %f\n", dists[i].box1, dists[i].box2, dists[i].dist);
        if (connect(dists[i].box1, dists[i].box2)) {
            last1 = dists[i].box1;
            last2 = dists[i].box2;
        }
        // print_boxes();
        // print_circuits();
        // printf("\n");
    }

    unsigned long long *sizes = NULL;
    for (int i = 0; i < arrlen(circuits); i++) {
        arrput(sizes, arrlen(circuits[i]));
    }
    qsort(sizes, arrlen(sizes), sizeof(*sizes), cmp_llu_desc);
    printf("Total 1: %llu\n", sizes[0]*sizes[1]*sizes[2]);

    // finish connecting
    for (int i = reps; i < arrlen(dists); i++) {
        // printf("%d<->%d = %f\n", dists[i].box1, dists[i].box2, dists[i].dist);
        if (connect(dists[i].box1, dists[i].box2)) {
            last1 = dists[i].box1;
            last2 = dists[i].box2;
        }
        if (arrlen(circuits[boxes[dists[i].box1].circuit_idx]) == arrlen(boxes)) break;
        // print_boxes();
        // print_circuits();
        // printf("\n");
    }
    printf("Total 2: %llu\n", (unsigned long long)boxes[last1].x * boxes[last2].x);

    return 0;
}
