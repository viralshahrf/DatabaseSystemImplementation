#include <stdio.h>
#include <stdlib.h>

int* binarySearch(int32_t *level, int sIndex, int eIndex, int32_t value) {
    int *range;
    if (sIndex <= eIndex) {
        int mIndex = (sIndex + eIndex)/2;
        if (level[mIndex] == value) {
            int *mRange = binarySearch(level, sIndex, mIndex-1, value);
            if (mRange[1] != -1) {
                mIndex = mRange[1];
            }
            range = (int*)malloc(2*sizeof(int));
            if (mIndex - 1 < sIndex) {
                range[0] = -1;
                range[1] = mIndex;
                return range;
            } else {
                range[0] = mIndex - 1;
                range[1] = mIndex;
                return range;
            }
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
        range = (int*)malloc(2*sizeof(int));
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

int searchTree(int32_t **tree, int *fanout, int levels, int32_t value) {
    int iLevel = 0;
    int lOffset = 0;
    int adj = 0;
    int rOffset = fanout[0] - 2;
    int *range = NULL;

    while (iLevel < levels) {
        range = binarySearch(tree[iLevel], lOffset, rOffset, value);
        if (iLevel == (levels - 1)) {
            int r = range[1] != -1 ? range[1] : range[0];
            int result = getRange(iLevel, r, fanout, levels);
            if (range[1] == -1) {
                result += 1;
            }
            return result;
        } else if (range[0] == -1) {
            rOffset = ((range[1] + 1 + adj) * (fanout[iLevel+1] - 1)) - 1;
            lOffset = rOffset - fanout[iLevel+1] + 2;
            adj += range[1];
        } else {
            lOffset = (range[0] + 1 + adj) * (fanout[iLevel+1] -1);
            rOffset = lOffset + fanout[iLevel+1] - 2;
            adj += range[0] + 1;
        }
        iLevel = iLevel + 1;
    }
}
