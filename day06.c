#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

enum op { ADD, MUL };

int *loadNumbers(char *line) {
    int *nums = NULL;
    while (strpbrk(line, "0123456789") != NULL) {
        int num = strtol(line, &line, 10);
        arrput(nums, num);
    }
    return nums;
}

enum op *loadOps(char *line) {
    enum op *ops = NULL;
    while (*line) {
        if (*line == '+') arrput(ops, ADD);
        else if (*line == '*') arrput(ops, MUL);
        line++;
    }
    return ops;
}

// return an array of rows
char **loadFile(const char *path) {
    FILE *file = fopen(path, "r");
    assert(file != NULL);

    char *row = NULL;
    char **rows = NULL;
    for (;;) {
        char ch;
        fread(&ch, 1, 1, file);
        if (feof(file) || ferror(file)) break;
        if (ch == '\n') {
            arrput(row, '\0');
            arrput(rows, row);
            row = NULL;
            continue;
        } else {
            arrput(row, ch);
        }
    }
    if (row != NULL) {
        arrput(row, '\0');
        arrput(rows, row);
    }

    int len = arrlen(rows[0]);
    for (int i = 0; i < arrlen(rows); i++) {
        assert(arrlen(rows[i]) == len);
    }

    fclose(file);
    return rows;
}

void loadHomework1(char **rows, int ***outNums, enum op **outOps) {
    *outNums = NULL;
    *outOps = loadOps(rows[arrlen(rows) - 1]);
    for (int i = 0; i + 1 < arrlen(rows); i++) {
        int *row = loadNumbers(rows[i]);
        arrput(*outNums, row);
    }
}

unsigned long long makeOp(int **nums, enum op *ops, int idx) {
    enum op op = ops[idx];
    if (op == ADD) {
        unsigned long long res = 0;
        for (int i = 0; i < arrlen(nums); i++) {
            res += nums[i][idx];
        }
        return res;
    } else {
        unsigned long long res = 1;
        for (int i = 0; i < arrlen(nums); i++) {
            res *= nums[i][idx];
        }
        return res;
    }
}

unsigned long long calcTot2(char **rows) {
    enum op *ops = loadOps(rows[arrlen(rows) - 1]);

    unsigned long long tot = 0;
    int blankIdx = -1;
    int nums[10];
    int opIdx = 0;
    int rowLen = arrlen(rows[0]) - 1;
    while (blankIdx < rowLen) {
        int startIdx = ++blankIdx;
        while (blankIdx < rowLen) {
            bool isBlank = true;
            for (int i = 0; i < arrlen(rows); i++) {
                if (rows[i][blankIdx] != ' ') {
                    isBlank = false;
                    break;
                }
            }
            if (isBlank) break;
            blankIdx++;
        }

        int numCount = blankIdx - startIdx;
        int maxDigits = arrlen(rows) - 1;
        enum op op = ops[opIdx++];
        unsigned long long opTotal = op == ADD ? 0 : 1;
        for (int i = startIdx; i < blankIdx; i++) {
            int num = 0;
            for (int j = 0; j < maxDigits; j++) {
                char ch = rows[j][i];
                if (rows[j][i] == ' ') continue;
                num *= 10;
                num += ch - '0';
            }
            if (op == ADD) opTotal += num;
            else opTotal *= num;
        }
        tot += opTotal;
    }

    return tot;
}

int main(void) {
    char **rows = loadFile("input06.txt");

    int **nums;
    enum op *ops;
    loadHomework1(rows, &nums, &ops);

    for (int i = 0; i < arrlen(nums); i++) {
        assert(arrlen(nums[i]) == arrlen(ops));
    }

    unsigned long long total = 0;
    for (int i = 0; i < arrlen(ops); i++) {
        total += makeOp(nums, ops, i);
    }
    printf("Total 1: %llu\n", total);

    printf("Total 2: %llu\n", calcTot2(rows));

    return 0;
}

