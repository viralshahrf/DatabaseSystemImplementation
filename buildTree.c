#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int32_t** buildTree(int *keys, int *fanout, int nKeys, int levels) {
    int i;
    int maxKeys = 1;
    for (i = 0; i < levels; i++) {
        maxKeys *= fanout[i];
    }
    int minKeys = maxKeys/fanout[0];
    maxKeys -= 1;

    if (nKeys > maxKeys) {
        printf("Number of keys exceeds that supported by the current tree levels and fanouts\n");
        printf("Maximum Keys Supported: %d\n", maxKeys);
        return NULL;
    }

    if (nKeys < minKeys) {
        printf("Number of keys are insufficient to be supported by the current tree level and fanouts\n");
        printf("Minimum Keys Required: %d\n", minKeys);
        return NULL;
    }

    int error;

    int32_t **tree;
    void *ptMem = NULL;
    error = posix_memalign(&ptMem, 16, levels*sizeof(int32_t*));
    if (error != 0) {
        printf("Memory allocation error\n");
        return NULL;
    }
    tree = (int32_t**)ptMem;

    int f = 1;
    int lSize;
    for (i = 0; i < levels; i++) {
        lSize = f * (fanout[i] - 1);
        f *= fanout[i];
        void *plMem = NULL;
        error = posix_memalign(&plMem, 16, lSize*sizeof(int32_t));
        if (error != 0) {
            printf("Memory allocation error\n");
            return NULL;
        }
        int32_t *lMem = (int32_t*)plMem;
        tree[i] = lMem;
    }

    int iLevel = levels - 1;
    int lFanout;
    int lSkip = 1;
    while (iLevel >= 0) {
        lFanout = fanout[iLevel];
        int iKey = lSkip - 1;
        int lIter = 0;
        while (iKey < nKeys) {
            int fill;
            for (fill = 0; fill < lFanout - 1 && iKey < nKeys; fill++) {
                tree[iLevel][lIter++] = keys[iKey];
                iKey += lSkip;
            }
            iKey += lSkip;
        }

        f /= lFanout;

        while (lIter < (f * (lFanout - 1))) {
            tree[iLevel][lIter++] = INT_MAX;
        }
        lSkip *= lFanout;
        iLevel--;
    }

    return tree;
}
