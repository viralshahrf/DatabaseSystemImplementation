#include <time.h>
#include <stdlib.h>

long getCurrentTime() {
    struct timeval t;
    gettimeofday(&t, 0);

    return 1000000 * t.tv_sec + t.tv_usec;
}
