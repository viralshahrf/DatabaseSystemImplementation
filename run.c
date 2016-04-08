#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "randomize.h"
#include "buildTree.h"
#include "debug.h"

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
        int32_t **tree = buildTree(keys, fanout, nKeys, levels);
        if (tree == NULL) {
            printf("Error in building tree\n");
            return -1;
        }
        //printTree(tree, fanout, levels);

        rand32_t *genProbes = rand32_init(time(NULL));
        int32_t *probes = generate(nProbes, genProbes);
        free(genProbes);

        return 0;
    }
}
