#include <stdio.h>
#include <stdlib.h>

void printTree(int32_t **tree, int *fanout, int levels) {
    int i, j;
    int f = 1;
    int lSize;
    for (i = 0; i < levels; i++) {
        lSize = f * (fanout[i] - 1);
        f *= fanout[i];
        for (j = 0; j < lSize; j++) {
            printf("%d\t", tree[i][j]);
        }
        printf("\n");
    }
}
