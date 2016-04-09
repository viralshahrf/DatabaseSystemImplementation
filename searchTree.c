#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

int* binarySearch(int32_t *level, int sIndex, int eIndex, int32_t value) {
    int *range = (int*)malloc(2*sizeof(int));
    if (sIndex <= eIndex) {
        int mIndex = (sIndex + eIndex)/2;
        if (level[mIndex] == value) {
            range[0] = mIndex;
            range[1] = mIndex;
            return range;
            /*
            if (mIndex - 1 < sIndex) {
                range[0] = -1;
                range[1] = mIndex;
                return range;
            } else {
                range[0] = mIndex - 1;
                range[1] = mIndex;
                return range;
            }
            */
        } else if (level[mIndex] > value) {
            range = binarySearch(level, sIndex, mIndex-1, value);
            if (range[1] == -1) {
                range[1] = mIndex;
            }
            return range;
        } else {
            range = binarySearch(level, mIndex+1, eIndex, value);
            if (range[0] == -1) {
                range[0] = mIndex;
            }
            return range;
        }
    } else {
        range[0] = -1;
        range[1] = -1;
        return range;
    }
}

int getRange(int level, int offset, int *fanout, int levels) {
    int i;
    int lSkip = 1;
    for (i = level+1; i < levels; i++) {
        lSkip *= fanout[i];
    }
    int skips = offset/(fanout[level]-1);
    return lSkip + lSkip*(skips+offset) - 1;
}
