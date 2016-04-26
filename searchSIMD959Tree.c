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

int searchSIMD959Tree(int32_t **tree, int32_t value) {
     __m128i key = _mm_cvtsi32_si128(value);
    key = _mm_shuffle_epi32(key, _MM_SHUFFLE(0,0,0,0));

    __m128i delimiters00 = _mm_load_si128((__m128i const*)&tree[0][0]);
    __m128i delimiters01 = _mm_load_si128((__m128i const*)&tree[0][4]);
    __m128i compare00 = _mm_cmpgt_epi32(delimiters00, key);
    __m128i compare01 = _mm_cmpgt_epi32(delimiters01, key);
    __m128i equals00 = _mm_cmpeq_epi32(delimiters00,key);
    __m128i equals01 = _mm_cmpeq_epi32(delimiters01,key);
    int32_t cmpmask = _mm_movemask_ps(_mm_castsi128_ps(compare01));
    cmpmask = cmpmask << 4;
    cmpmask |= _mm_movemask_ps(_mm_castsi128_ps(compare00));
    int32_t eqmask = _mm_movemask_ps(_mm_castsi128_ps(equals01));
    eqmask = eqmask << 4;
    eqmask |= _mm_movemask_ps(_mm_castsi128_ps(equals00));
    cmpmask = cmpmask | eqmask | 0x100;
    int32_t position = _bit_scan_forward(cmpmask);
    
    int32_t offset0 = position;
    int32_t loffset1 = (offset0 << 2);

    __m128i delimiters10 = _mm_load_si128((__m128i const*)&tree[1][loffset1]);
    __m128i compare10 = _mm_cmpgt_epi32(delimiters10, key);
    __m128i equals10 = _mm_cmpeq_epi32(delimiters10,key);
    cmpmask = _mm_movemask_ps(_mm_castsi128_ps(compare10));
    eqmask = _mm_movemask_ps(_mm_castsi128_ps(equals10));
    cmpmask = cmpmask | eqmask | 0x10;
    position = _bit_scan_forward(cmpmask);

    int32_t offset1 = loffset1 + position + offset0;
    int32_t loffset2 = (offset1 << 3);

    __m128i delimiters20 = _mm_load_si128((__m128i const*)&tree[2][loffset2]);
    __m128i delimiters21 = _mm_load_si128((__m128i const*)&tree[2][loffset2+4]);
    __m128i compare20 = _mm_cmpgt_epi32(delimiters20, key);
    __m128i compare21 = _mm_cmpgt_epi32(delimiters21, key);
    __m128i equals20 = _mm_cmpeq_epi32(delimiters20,key);
    __m128i equals21 = _mm_cmpeq_epi32(delimiters21,key);
    cmpmask = _mm_movemask_ps(_mm_castsi128_ps(compare21));
    cmpmask = cmpmask << 4;
    cmpmask |= _mm_movemask_ps(_mm_castsi128_ps(compare20));
    eqmask = _mm_movemask_ps(_mm_castsi128_ps(equals21));
    eqmask = eqmask << 4;
    eqmask |= _mm_movemask_ps(_mm_castsi128_ps(equals20));
    cmpmask = cmpmask | eqmask | 0x100;
    position = _bit_scan_forward(cmpmask);

    int32_t offset2 = loffset2 + position + offset1;

    return offset2;
}
