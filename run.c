#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "run.h"

int main(int argc, char **argv) {
    int arguments = argc;
    if (arguments < 4) {
        printf("Too few arguments\n");
        return -1;
    } else {
        int i;
        int levels = arguments - 3;
        int fanout[levels];
        for (i = 0; i < levels; i++) {
            fanout[i] = atoi(argv[i+3]);
        }

        int nKeys = atoi(argv[1]);
        int nProbes = atoi(argv[2]);

        int32_t *keys = generate_random(nKeys, 1);
        if (keys == NULL) {
            printf("Could not generate keys\n");
            return -1;
        }

        long phase00 = getCurrentTime();

        int32_t **tree = buildTree(keys, fanout, nKeys, levels);
        if (tree == NULL) {
            printf("Error in building tree\n");
            return -1;
        }
        //printTree(tree, fanout, levels);

        int32_t *probes = generate_random(nProbes, 0);
        if (probes == NULL) {
            printf("Could not generate probes\n");
            return -1;
        }

        long phase10 = getCurrentTime();
        printf("Phase 1 Time: %ld usecs\n", phase10 - phase00);

        int ranges[nProbes];
        for (i = 0; i < nProbes; i++) {
            ranges[i] = searchTree(tree, fanout, levels, probes[i]);
        }

        long phase20 = getCurrentTime();
        printf("Phase 2|1 Time: %ld usecs\n", phase20 - phase10);

        for (i = 0; i < nProbes; i++) {
            printf("Probe: %d | Range: %d\n", probes[i], ranges[i]);
        }

        long phase30 = getCurrentTime();
        printf("Phase 3|1 Time: %ld usecs\n", phase30 - phase20);

        int SIMD = 1;
        for (i = 0; i < levels; i++) {
            if ((fanout[i]-1) % 4 != 0) {
                SIMD = 0;
            }
        }

        if (SIMD) {
            long phase11 = getCurrentTime();

            for (i = 0; i < nProbes; i++) {
                ranges[i] = searchSIMDTree(tree, fanout, levels, probes[i]);
            }

            long phase21 = getCurrentTime();
            printf("Phase 2|2 Time: %ld usecs\n", phase21 - phase11);

            for (i = 0; i < nProbes; i++) {
                printf("Probe: %d | Range: %d\n", probes[i], ranges[i]);
            }

            long phase31 = getCurrentTime();
            printf("Phase 3|2 Time: %ld usecs\n", phase31 - phase21);
        }

        if (levels == 3 && fanout[0] == 9 && fanout[1] == 5 && fanout[2] == 9) {
            long phase12 = getCurrentTime();

            int32_t *result = searchSIMD959Tree(tree, probes, nProbes);

            long phase22 = getCurrentTime();
            printf("Phase 2|3 Time: %ld usecs\n", phase22 - phase12);

            for (i = 0; i < nProbes; i++) {
                printf("Probe: %d | Range: %d\n", probes[i], result[i]);
            }

            long phase32 = getCurrentTime();
            printf("Phase 3|3 Time: %ld usecs\n", phase32 - phase22);
        }

        return 0;
    }
}
