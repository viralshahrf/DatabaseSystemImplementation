#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <xmmintrin.h> // SSE
#include <emmintrin.h> // SSE2
#include <pmmintrin.h> // SSE3
#include <tmmintrin.h> // SSSE3
#include <smmintrin.h> // SSE4.1
#include <nmmintrin.h> // SSE4.2
#include <ammintrin.h> // SSE4A

int searchSIMDTree(int32_t **tree, int *fanout, int levels, int32_t value) {
    int iLevel = 0;
    int lOffset = 0;
    int pOffset = 0;

     __m128i key = _mm_cvtsi32_si128(value);
    key = _mm_shuffle_epi32(key, _MM_SHUFFLE(0,0,0,0));

    while (iLevel < levels) {
        int f = fanout[iLevel];
        pOffset = lOffset;
        lOffset *= f - 1;
        int iter = 0;
        int position = 0;
        while (iter < f/4) {
            __m128i delimiters = _mm_load_si128((__m128i const*)&tree[iLevel][lOffset + iter*4]);
            __m128i compare = _mm_cmpgt_epi32(delimiters, key);
            int32_t mask = _mm_movemask_ps(_mm_castsi128_ps(compare));
            if (mask) {
                position = _bit_scan_forward(mask);
                break;
            }
            iter++;
        }
        int offset = lOffset + iter*4 + position;
        lOffset = offset + pOffset;
        iLevel++;
    }
    return lOffset - 1;
}
