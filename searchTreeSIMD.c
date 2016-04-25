#include <stdio.h>
#include <stdlib.h>

#include <x86intrin.h> 
//#include <xmmintrin.h> // SSE
//#include <emmintrin.h> // SSE2
//#include <pmmintrin.h> // SSE3
//#include <tmmintrin.h> // SSSE3
//#include <smmintrin.h> // SSE4.1
#include <nmmintrin.h> // SSE4.2
//#include <ammintrin.h> // SSE4A

int searchTreeSIMD(int32_t **tree, int *fanout, int levels, int32_t value) {
    __m128i key = _mm_cvtsi32_si128(value);
    key = _mm_shuffle_epi32(key, _MM_SHUFFLE(0,0,0,0));

    int32_t *val = (int32_t *)&key;
    printf("Key broadcasted: %d %d %d %d\n", val[0], val[1], val[2], val[3]);

    int i;
    for (i = 0; i < levels; i++) {
        int j, offset = 0;
        for (j = offset; j < fanout[i] - 1; j = j + 4) {
            __m128i lElems = _mm_load_si128((__m128i const *)&tree[i][j]);
            int32_t *val = (int32_t *)&lElems;
            printf("Values for comparison on level %d at offset %d: %d %d %d %d\n", i, j, val[0], val[1], val[2], val[3]);
        }
    }

    return 0;
}
