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

int32_t* searchSIMD959Tree(int32_t **tree, int32_t *probes, int nProbes) {

    int32_t iter = 0;
    int32_t *result = (int32_t*)malloc(sizeof(int32_t)*nProbes);
    __m128i delimiters00 = _mm_load_si128((__m128i const*)&tree[0][0]);
    __m128i delimiters01 = _mm_load_si128((__m128i const*)&tree[0][4]);

    while (iter < nProbes) {
        __m128i key = _mm_load_si128((__m128i*)&probes[iter]);
        __m128i key0 = _mm_shuffle_epi32(key, _MM_SHUFFLE(0,0,0,0));
        __m128i key1 = _mm_shuffle_epi32(key, _MM_SHUFFLE(1,1,1,1));
        __m128i key2 = _mm_shuffle_epi32(key, _MM_SHUFFLE(2,2,2,2));
        __m128i key3 = _mm_shuffle_epi32(key, _MM_SHUFFLE(3,3,3,3));

        __m128i compare00 = _mm_cmpgt_epi32(delimiters00, key0);
        __m128i compare01 = _mm_cmpgt_epi32(delimiters01, key0);
        __m128i compare10 = _mm_cmpgt_epi32(delimiters00, key1);
        __m128i compare11 = _mm_cmpgt_epi32(delimiters01, key1);
        __m128i compare20 = _mm_cmpgt_epi32(delimiters00, key2);
        __m128i compare21 = _mm_cmpgt_epi32(delimiters01, key2);
        __m128i compare30 = _mm_cmpgt_epi32(delimiters00, key3);
        __m128i compare31 = _mm_cmpgt_epi32(delimiters01, key3);
        __m128i equals00 = _mm_cmpeq_epi32(delimiters00,key0);
        __m128i equals01 = _mm_cmpeq_epi32(delimiters01,key0);
        __m128i equals10 = _mm_cmpeq_epi32(delimiters00,key1);
        __m128i equals11 = _mm_cmpeq_epi32(delimiters01,key1);
        __m128i equals20 = _mm_cmpeq_epi32(delimiters00,key2);
        __m128i equals21 = _mm_cmpeq_epi32(delimiters01,key2);
        __m128i equals30 = _mm_cmpeq_epi32(delimiters00,key3);
        __m128i equals31 = _mm_cmpeq_epi32(delimiters01,key3);
        int32_t cmpmask0 = _mm_movemask_ps(_mm_castsi128_ps(compare01));
        cmpmask0 = cmpmask0 << 4;
        cmpmask0 |= _mm_movemask_ps(_mm_castsi128_ps(compare00));
        int32_t eqmask0 = _mm_movemask_ps(_mm_castsi128_ps(equals01));
        eqmask0 = eqmask0 << 4;
        eqmask0 |= _mm_movemask_ps(_mm_castsi128_ps(equals00));
        cmpmask0 = cmpmask0 | eqmask0 | 0x100;
        int32_t position0 = _bit_scan_forward(cmpmask0);
        int32_t cmpmask1 = _mm_movemask_ps(_mm_castsi128_ps(compare11));
        cmpmask1 = cmpmask1 << 4;
        cmpmask1 |= _mm_movemask_ps(_mm_castsi128_ps(compare10));
        int32_t eqmask1 = _mm_movemask_ps(_mm_castsi128_ps(equals11));
        eqmask1 = eqmask1 << 4;
        eqmask1 |= _mm_movemask_ps(_mm_castsi128_ps(equals10));
        cmpmask1 = cmpmask1 | eqmask1 | 0x100;
        int32_t position1 = _bit_scan_forward(cmpmask1);
        int32_t cmpmask2 = _mm_movemask_ps(_mm_castsi128_ps(compare21));
        cmpmask2 = cmpmask2 << 4;
        cmpmask2 |= _mm_movemask_ps(_mm_castsi128_ps(compare20));
        int32_t eqmask2 = _mm_movemask_ps(_mm_castsi128_ps(equals21));
        eqmask2 = eqmask2 << 4;
        eqmask2 |= _mm_movemask_ps(_mm_castsi128_ps(equals20));
        cmpmask2 = cmpmask2 | eqmask2 | 0x100;
        int32_t position2 = _bit_scan_forward(cmpmask2);
        int32_t cmpmask3 = _mm_movemask_ps(_mm_castsi128_ps(compare31));
        cmpmask3 = cmpmask3 << 4;
        cmpmask3 |= _mm_movemask_ps(_mm_castsi128_ps(compare30));
        int32_t eqmask3 = _mm_movemask_ps(_mm_castsi128_ps(equals31));
        eqmask3 = eqmask3 << 4;
        eqmask3 |= _mm_movemask_ps(_mm_castsi128_ps(equals30));
        cmpmask3 = cmpmask3 | eqmask3 | 0x100;
        int32_t position3 = _bit_scan_forward(cmpmask3);

        int32_t offset00 = position0;
        int32_t loffset01 = (offset00 << 2);
        int32_t offset10 = position1;
        int32_t loffset11 = (offset10 << 2);
        int32_t offset20 = position2;
        int32_t loffset21 = (offset20 << 2);
        int32_t offset30 = position3;
        int32_t loffset31 = (offset30 << 2);

        __m128i delimiters10 = _mm_load_si128((__m128i const*)&tree[1][loffset01]);
        __m128i delimiters11 = _mm_load_si128((__m128i const*)&tree[1][loffset11]);
        __m128i delimiters12 = _mm_load_si128((__m128i const*)&tree[1][loffset21]);
        __m128i delimiters13 = _mm_load_si128((__m128i const*)&tree[1][loffset31]);
        compare00 = _mm_cmpgt_epi32(delimiters10, key0);
        compare10 = _mm_cmpgt_epi32(delimiters11, key1);
        compare20 = _mm_cmpgt_epi32(delimiters12, key2);
        compare30 = _mm_cmpgt_epi32(delimiters13, key3);
        equals00 = _mm_cmpeq_epi32(delimiters10,key0);
        equals10 = _mm_cmpeq_epi32(delimiters11,key1);
        equals20 = _mm_cmpeq_epi32(delimiters12,key2);
        equals30 = _mm_cmpeq_epi32(delimiters13,key3);
        cmpmask0 = _mm_movemask_ps(_mm_castsi128_ps(compare00));
        eqmask0 = _mm_movemask_ps(_mm_castsi128_ps(equals00));
        cmpmask0 = cmpmask0 | eqmask0 | 0x10;
        position0 = _bit_scan_forward(cmpmask0);
        cmpmask1 = _mm_movemask_ps(_mm_castsi128_ps(compare10));
        eqmask1 = _mm_movemask_ps(_mm_castsi128_ps(equals10));
        cmpmask1 = cmpmask1 | eqmask1 | 0x10;
        position1 = _bit_scan_forward(cmpmask1);
        cmpmask2 = _mm_movemask_ps(_mm_castsi128_ps(compare20));
        eqmask2 = _mm_movemask_ps(_mm_castsi128_ps(equals20));
        cmpmask2 = cmpmask2 | eqmask2 | 0x10;
        position2 = _bit_scan_forward(cmpmask2);
        cmpmask3 = _mm_movemask_ps(_mm_castsi128_ps(compare30));
        eqmask3 = _mm_movemask_ps(_mm_castsi128_ps(equals30));
        cmpmask3 = cmpmask3 | eqmask3 | 0x10;
        position3 = _bit_scan_forward(cmpmask3);

        int32_t offset01 = loffset01 + position0 + offset00;
        int32_t loffset02 = (offset01 << 3);
        int32_t offset11 = loffset11 + position1 + offset10;
        int32_t loffset12 = (offset11 << 3);
        int32_t offset21 = loffset21 + position2 + offset20;
        int32_t loffset22 = (offset21 << 3);
        int32_t offset31 = loffset31 + position3 + offset30;
        int32_t loffset32 = (offset31 << 3);

        __m128i delimiters200 = _mm_load_si128((__m128i const*)&tree[2][loffset02]);
        __m128i delimiters210 = _mm_load_si128((__m128i const*)&tree[2][loffset02+4]);
        __m128i delimiters201 = _mm_load_si128((__m128i const*)&tree[2][loffset12]);
        __m128i delimiters211 = _mm_load_si128((__m128i const*)&tree[2][loffset12+4]);
        __m128i delimiters202 = _mm_load_si128((__m128i const*)&tree[2][loffset22]);
        __m128i delimiters212 = _mm_load_si128((__m128i const*)&tree[2][loffset22+4]);
        __m128i delimiters203 = _mm_load_si128((__m128i const*)&tree[2][loffset32]);
        __m128i delimiters213 = _mm_load_si128((__m128i const*)&tree[2][loffset32+4]);
        compare00 = _mm_cmpgt_epi32(delimiters200, key0);
        compare01 = _mm_cmpgt_epi32(delimiters210, key0);
        compare10 = _mm_cmpgt_epi32(delimiters201, key1);
        compare11 = _mm_cmpgt_epi32(delimiters211, key1);
        compare20 = _mm_cmpgt_epi32(delimiters202, key2);
        compare21 = _mm_cmpgt_epi32(delimiters212, key2);
        compare30 = _mm_cmpgt_epi32(delimiters203, key3);
        compare31 = _mm_cmpgt_epi32(delimiters213, key3);
        equals00 = _mm_cmpeq_epi32(delimiters200,key0);
        equals01 = _mm_cmpeq_epi32(delimiters210,key0);
        equals10 = _mm_cmpeq_epi32(delimiters201,key1);
        equals11 = _mm_cmpeq_epi32(delimiters211,key1);
        equals20 = _mm_cmpeq_epi32(delimiters202,key2);
        equals21 = _mm_cmpeq_epi32(delimiters212,key2);
        equals30 = _mm_cmpeq_epi32(delimiters203,key3);
        equals31 = _mm_cmpeq_epi32(delimiters213,key3);
        cmpmask0 = _mm_movemask_ps(_mm_castsi128_ps(compare01));
        cmpmask0 = cmpmask0 << 4;
        cmpmask0 |= _mm_movemask_ps(_mm_castsi128_ps(compare00));
        eqmask0 = _mm_movemask_ps(_mm_castsi128_ps(equals01));
        eqmask0 = eqmask0 << 4;
        eqmask0 |= _mm_movemask_ps(_mm_castsi128_ps(equals00));
        cmpmask0 = cmpmask0 | eqmask0 | 0x100;
        position0 = _bit_scan_forward(cmpmask0);
        cmpmask1 = _mm_movemask_ps(_mm_castsi128_ps(compare11));
        cmpmask1 = cmpmask1 << 4;
        cmpmask1 |= _mm_movemask_ps(_mm_castsi128_ps(compare10));
        eqmask1 = _mm_movemask_ps(_mm_castsi128_ps(equals11));
        eqmask1 = eqmask1 << 4;
        eqmask1 |= _mm_movemask_ps(_mm_castsi128_ps(equals10));
        cmpmask1 = cmpmask1 | eqmask1 | 0x100;
        position1 = _bit_scan_forward(cmpmask1);
        cmpmask2 = _mm_movemask_ps(_mm_castsi128_ps(compare21));
        cmpmask2 = cmpmask2 << 4;
        cmpmask2 |= _mm_movemask_ps(_mm_castsi128_ps(compare20));
        eqmask2 = _mm_movemask_ps(_mm_castsi128_ps(equals21));
        eqmask2 = eqmask2 << 4;
        eqmask2 |= _mm_movemask_ps(_mm_castsi128_ps(equals20));
        cmpmask2 = cmpmask2 | eqmask2 | 0x100;
        position2 = _bit_scan_forward(cmpmask2);
        cmpmask3 = _mm_movemask_ps(_mm_castsi128_ps(compare31));
        cmpmask3 = cmpmask3 << 4;
        cmpmask3 |= _mm_movemask_ps(_mm_castsi128_ps(compare30));
        eqmask3 = _mm_movemask_ps(_mm_castsi128_ps(equals31));
        eqmask3 = eqmask3 << 4;
        eqmask3 |= _mm_movemask_ps(_mm_castsi128_ps(equals30));
        cmpmask3 = cmpmask3 | eqmask3 | 0x100;
        position3 = _bit_scan_forward(cmpmask3);

        int32_t offset0 = loffset02 + position0 + offset01;
        int32_t offset1 = loffset12 + position1 + offset11;
        int32_t offset2 = loffset22 + position2 + offset21;
        int32_t offset3 = loffset32 + position3 + offset31;

        result[iter++] = offset0;
        result[iter++] = offset1;
        result[iter++] = offset2;
        result[iter++] = offset3;
    }
    return result;
}
