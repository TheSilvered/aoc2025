#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

char **readBanks(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buf = malloc(fileSize + 1);
    size_t len = fread(buf, 1, fileSize, file);
    fclose(file);
    buf[len + 1] = '\0';
    char *p = buf, *lfPos;
    char **banks = NULL;
    while ((lfPos = strchr(p, '\n')) != NULL) {
        *lfPos = 0;
        arrput(banks, p);
        p = lfPos + 1;
    }

    return banks;
}

int maxPower1(const char *bank) {
    int len = strlen(bank);

    int maxI = 0;
    char max = '0';

    for (int i = 0; i < len - 1; i++) {
        if (bank[i] > max) {
            max = bank[i];
            maxI = i;
        }
    }

    int num = (max - '0') * 10;
    max = '0';

    for (int i = maxI + 1; i < len; i++) {
        if (bank[i] > max) {
            max = bank[i];
        }
    }

    return num + (max - '0');
}

int maxPower2(const char *bank) {
    int len = strlen(bank);

    int num = 0;
    int lastDigitIdx = 0;
    for (int offset = 11; offset >= 0; offset--) {
        char max = '0';
        for (int i = lastDigitIdx; i < len - offset; i++) {
            if (bank[i] > max) {
                max = bank[i];
                lastDigitIdx = i;
            }
        }
        num *= 10;
        num += max - '0';
    }

    return num;
}

int main(void) {
    char **banks = readBanks("input03.txt");

    int tot = 0;
    for (int i = 0; i < arrlen(banks); i++) {
        tot += maxPower(banks[i]);
    }
    printf("Total 1: %d\n", tot);

    int tot = 0;
    for (int i = 0; i < arrlen(banks); i++) {
        tot += maxPower(banks[i]);
    }
    printf("Total 1: %d\n", tot);

    free(banks[0]);
    arrfree(banks);
    return 0;
}
