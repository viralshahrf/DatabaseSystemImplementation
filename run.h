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

//Timing
long getCurrentTime();

#endif
