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
        register __m128i key0 = _mm_shuffle_epi32(key, _MM_SHUFFLE(0,0,0,0));
        register __m128i key1 = _mm_shuffle_epi32(key, _MM_SHUFFLE(1,1,1,1));
        register __m128i key2 = _mm_shuffle_epi32(key, _MM_SHUFFLE(2,2,2,2));
        register __m128i key3 = _mm_shuffle_epi32(key, _MM_SHUFFLE(3,3,3,3));

        register __m128i compare00 = _mm_cmpgt_epi32(key0, delimiters00);
        register __m128i compare01 = _mm_cmpgt_epi32(key0, delimiters01);
        register __m128i compare10 = _mm_cmpgt_epi32(key1, delimiters00);
        register __m128i compare11 = _mm_cmpgt_epi32(key1, delimiters01);
        register __m128i compare20 = _mm_cmpgt_epi32(key2, delimiters00);
        register __m128i compare21 = _mm_cmpgt_epi32(key2, delimiters01);
        register __m128i compare30 = _mm_cmpgt_epi32(key3, delimiters00);
        register __m128i compare31 = _mm_cmpgt_epi32(key3, delimiters01);

        register __m128i compare0 = _mm_packs_epi32(compare00, compare01);
        compare0 = _mm_packs_epi16(compare0, _mm_setzero_si128());
        register __m128i compare1 = _mm_packs_epi32(compare10, compare11);
        compare1 = _mm_packs_epi16(compare1, _mm_setzero_si128());
        register __m128i compare2 = _mm_packs_epi32(compare20, compare21);
        compare2 = _mm_packs_epi16(compare2, _mm_setzero_si128());
        register __m128i compare3 = _mm_packs_epi32(compare30, compare31);
        compare3 = _mm_packs_epi16(compare3, _mm_setzero_si128());

        int32_t cmpmask0 = _mm_movemask_epi8(compare0);
        int32_t position0 = _bit_scan_forward(cmpmask0 ^ 0x1FF);
        int32_t cmpmask1 = _mm_movemask_epi8(compare1);
        int32_t position1 = _bit_scan_forward(cmpmask1 ^ 0x1FF);
        int32_t cmpmask2 = _mm_movemask_epi8(compare2);
        int32_t position2 = _bit_scan_forward(cmpmask2 ^ 0x1FF);
        int32_t cmpmask3= _mm_movemask_epi8(compare3);
        int32_t position3 = _bit_scan_forward(cmpmask3 ^ 0x1FF);
 
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

        compare00 = _mm_cmpgt_epi32(key0, delimiters10);
        compare10 = _mm_cmpgt_epi32(key1, delimiters11);
        compare20 = _mm_cmpgt_epi32(key2, delimiters12);
        compare30 = _mm_cmpgt_epi32(key3, delimiters13);

        cmpmask0 = _mm_movemask_ps(_mm_castsi128_ps(compare00));
        position0 = _bit_scan_forward(cmpmask0 ^ 0x1F);
        cmpmask1 = _mm_movemask_ps(_mm_castsi128_ps(compare10));
        position1 = _bit_scan_forward(cmpmask1 ^ 0x1F);
        cmpmask2 = _mm_movemask_ps(_mm_castsi128_ps(compare20));
        position2 = _bit_scan_forward(cmpmask2 ^ 0x1F);
        cmpmask3 = _mm_movemask_ps(_mm_castsi128_ps(compare30));
        position3 = _bit_scan_forward(cmpmask3 ^ 0x1F);

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

        compare00 = _mm_cmpgt_epi32(key0, delimiters200);
        compare01 = _mm_cmpgt_epi32(key0, delimiters210);
        compare10 = _mm_cmpgt_epi32(key1, delimiters201);
        compare11 = _mm_cmpgt_epi32(key1, delimiters211);
        compare20 = _mm_cmpgt_epi32(key2, delimiters202);
        compare21 = _mm_cmpgt_epi32(key2, delimiters212);
        compare30 = _mm_cmpgt_epi32(key3, delimiters203);
        compare31 = _mm_cmpgt_epi32(key3, delimiters213);

        compare0 = _mm_packs_epi32(compare00, compare01);
        compare0 = _mm_packs_epi16(compare0, _mm_setzero_si128 ());
        compare1 = _mm_packs_epi32(compare10, compare11);
        compare1 = _mm_packs_epi16(compare1, _mm_setzero_si128 ());
        compare2 = _mm_packs_epi32(compare20, compare21);
        compare2 = _mm_packs_epi16(compare2, _mm_setzero_si128 ());
        compare3 = _mm_packs_epi32(compare30, compare31);
        compare3 = _mm_packs_epi16(compare3, _mm_setzero_si128 ());

        cmpmask0 = _mm_movemask_epi8(compare0);
        position0 = _bit_scan_forward(cmpmask0 ^ 0x1FF);
        cmpmask1 = _mm_movemask_epi8(compare1);
        position1 = _bit_scan_forward(cmpmask1 ^ 0x1FF);
        cmpmask2 = _mm_movemask_epi8(compare2);
        position2 = _bit_scan_forward(cmpmask2 ^ 0x1FF);
        cmpmask3 = _mm_movemask_epi8(compare3);
        position3 = _bit_scan_forward(cmpmask3 ^ 0x1FF);

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
