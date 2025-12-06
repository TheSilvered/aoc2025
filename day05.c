#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef unsigned long long u64;

struct range { u64 first, last; };

void loadDB(const char *path, struct range **outRanges, u64 **outIDs) {
    FILE *file = fopen(path, "r");
    assert(file != NULL);

    *outRanges = NULL;
    *outIDs = NULL;

    ssize_t read;
    char *buf = NULL;
    size_t len = 0;
    bool readingRanges = true;
    while ((read = getline(&buf, &len, file)) != -1) {
        if (read != 0 && buf[read - 1] == '\n') {
            buf[--read] = '\0';
        }
        if (read == 0) {
            readingRanges = false;
            continue;
        }

        if (readingRanges) {
            char *p;
            u64 first = strtoull(buf, &p, 10);
            p++;
            u64 last = strtoull(p, NULL, 10);
            struct range r = { first, last };
            arrput(*outRanges, r);
        } else {
            u64 id = strtoull(buf, NULL, 10);
            arrput(*outIDs, id);
        }
    }

    free(buf);
    fclose(file);
}

bool isFresh(struct range *ranges, u64 id) {
    for (int i = 0; i < arrlen(ranges); i++) {
        if (id >= ranges[i].first && id <= ranges[i].last) return true;
    }
    return false;
}

int cmpRanges(const void *r1, const void *r2) {
    u64 f1 = ((struct range *)r1)->first;
    u64 f2 = ((struct range *)r2)->first;
    if (f1 == f2) return 0;
    if (f1 < f2) return -1;
    return 1;
}

u64 max64(u64 a, u64 b) {
    return a > b ? a : b;
}

void printranges(const struct range *ranges) {
    printf("Ranges (%zi):\n", arrlen(ranges));
    for (int i = 0; i < arrlen(ranges); i++) {
        printf("%llu-%llu\n", ranges[i].first, ranges[i].last);
    }
}

u64 countIds(struct range *ranges) {
    qsort(ranges, arrlenu(ranges), sizeof(*ranges), cmpRanges);
    for (int i = 0; i + 1 < arrlen(ranges); i++) {
        if (ranges[i].last < ranges[i + 1].first) continue;

        ranges[i].last = max64(ranges[i].last, ranges[i + 1].last);
        arrdel(ranges, i + 1);
        i--;
    }

    u64 tot = 0;
    for (int i = 0; i < arrlen(ranges); i++) {
        tot += ranges[i].last - ranges[i].first + 1;
    }
    return tot;
}

int main(void) {
    struct range *ranges;
    u64 *ids;
    loadDB("input05.txt", &ranges, &ids);

    int tot = 0;
    for (int i = 0; i < arrlen(ids); i++) {
        if (isFresh(ranges, ids[i])) tot++;
    }
    printf("Total 1: %d\n", tot);
    printf("Total 2: %llu\n", countIds(ranges));

    arrfree(ranges);
    arrfree(ids);
    return 0;
}

