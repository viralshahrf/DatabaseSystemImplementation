#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include "randomize.h"
#include "buildTree.h"
#include "debug.h"
#include "searchTree.h"

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

        rand32_t *genKeys = rand32_init(time(NULL));
        int32_t *keys = generate_sorted_unique(nKeys, genKeys);
        free(genKeys);
        for (i = 1 ; i < nKeys; ++i) {
            assert(keys[i - 1] < keys[i]);
        }

        struct timeval time0;
        gettimeofday(&time0, 0);
        long phase0 = 1000000 * time0.tv_sec + time0.tv_usec;

        int32_t **tree = buildTree(keys, fanout, nKeys, levels);
        if (tree == NULL) {
            printf("Error in building tree\n");
            return -1;
        }
        //printTree(tree, fanout, levels);

        rand32_t *genProbes = rand32_init(time(NULL));
        int32_t *probes = generate(nProbes, genProbes);
        free(genProbes);

        struct timeval time1;
        gettimeofday(&time1, 0);
        long phase1 = 1000000 * time1.tv_sec + time1.tv_usec;
        printf("Phase 1 Time: %ld usecs\n", phase1 - phase0);

        int ranges[nProbes];
        for (i = 0; i < nProbes; i++) {
            ranges[i] = searchTree(tree, fanout, levels, probes[i]);
        }

        struct timeval time2;
        gettimeofday(&time2, 0);
        long phase2 = 1000000 * time2.tv_sec + time2.tv_usec;
        printf("Phase 2 Time: %ld usecs\n", phase2 - phase1);

        for (i = 0; i < nProbes; i++) {
            printf("Probe: %d | Range: %d\n", probes[i], ranges[i]);
        }

        struct timeval time3;
        gettimeofday(&time3, 0);
        long phase3 = 1000000 * time3.tv_sec + time3.tv_usec;
        printf("Phase 3 Time: %ld usecs\n", phase3 - phase2);

        return 0;
    }
}
