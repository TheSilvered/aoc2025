#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef unsigned long long int u64;

struct range { u64 first, last; };

struct range *readRanges(const char *file) {
    FILE *input = fopen(file, "r");
    assert(input != NULL);

    char *line = NULL;
    size_t len;
    getline(&line, &len, input);

    if (len != 0 && line[len - 1] == '\n') {
        line[--len] = '\0';
    }

    struct range *ranges = NULL;
    char *p = line;
    char *end = line + len;
    while (p < end) {
        u64 first = (u64)strtoll(p, &p, 10);
        if (first == 0) {
            break;
        }
        p++;
        u64 last = (u64)strtoll(p, &p, 10);
        p++;
        struct range newRange = { first, last };
        arrput(ranges, newRange);
    }

    free(line);
    fclose(input);

    return ranges;
}

bool isFake1(u64 n) {
    // only check numbers with an even number of digits
    if ((int)log10(n) % 2 != 1) {
        return false;
    }

    char buf[32];
    size_t len = sprintf(buf, "%lld", n);

    size_t mid = len / 2;
    for (size_t i = 0; i < mid; i++) {
        if (buf[i] != buf[i + mid]) {
            return false;
        }
    }
    return true;
}

bool isFake2(u64 n) {
    char buf[32];
    size_t len = sprintf(buf, "%lld", n);

    for (size_t patLen = 1; patLen <= len / 2; patLen++) {
        if (len % patLen != 0) {
            continue;
        }

        size_t repTimes = len / patLen;
        for (size_t i = 0; i < patLen; i++) {
            char ch = buf[i];
            for (size_t j = 1; j < repTimes; j++) {
                if (buf[i + patLen*j] != ch) {
                    goto nextLen;
                }
            }
        }
        return true;
nextLen:
    }
    return false;
}

int main(void) {
    struct range *ranges = readRanges("input02.txt");

    u64 total = 0;
    for (int i = 0; i < arrlen(ranges); i++) {
        for (u64 n = ranges[i].first; n <= ranges[i].last; n++) {
            if (isFake1(n)) {
                total += n;
            }
        }
    }

    printf("Total 1: %lld\n", total);

    total = 0;
    for (int i = 0; i < arrlen(ranges); i++) {
        for (u64 n = ranges[i].first; n <= ranges[i].last; n++) {
            if (isFake2(n)) {
                total += n;
            }
        }
    }

    printf("Total 2: %lld\n", total);
    arrfree(ranges);

    return 0;
}
