#ifndef RUN_H
#define RUN_H

//Randomize
int32_t* generate_random(int n, int sorted);

//Building an Index Tree
int32_t** buildTree(int *keys, int *fanout, int nKeys, int levels);

//Printing an Index Tree
void printTree(int32_t **tree, int *fanout, int levels);

//Search an Index Tree
int searchTree(int32_t **tree, int *fanout, int levels, int32_t value);

//Search an SIMD Index Tree
int searchSIMDTree(int32_t **tree, int *fanout, int levels, int32_t value);

//Search an SIMD 959 Tree
int32_t* searchSIMD959Tree(int32_t **tree, int32_t* probes, int nProbes);

//Timing
long getCurrentTime();

#endif
