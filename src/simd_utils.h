#include <immintrin.h>
#include <stdint.h>

// 256 bits

#define DEFINE_EXTRACT_256_SIMD_VALUE(TYPE, FUNCNAME) static inline TYPE \
FUNCNAME(__m256i reg, int32_t index) { \
    uint8_t temp[sizeof(__m256i)]; \
    _mm256_store_si256((__m256i*)temp, reg); \
    return ((TYPE*)temp)[index]; \
}

DEFINE_EXTRACT_256_SIMD_VALUE(uint8_t, simd_extract_u8_from_256)
DEFINE_EXTRACT_256_SIMD_VALUE(int8_t, simd_extract_s8_from_256)
DEFINE_EXTRACT_256_SIMD_VALUE(uint16_t, simd_extract_u16_from_256)
DEFINE_EXTRACT_256_SIMD_VALUE(int16_t, simd_extract_s16_from_256)
DEFINE_EXTRACT_256_SIMD_VALUE(uint32_t, simd_extract_u32_from_256)
DEFINE_EXTRACT_256_SIMD_VALUE(int32_t, simd_extract_s32_from_256)
DEFINE_EXTRACT_256_SIMD_VALUE(uint64_t, simd_extract_u64_from_256)
DEFINE_EXTRACT_256_SIMD_VALUE(int64_t, simd_extract_s64_from_256)

static inline float
simd_extract_f32_from_256(__m256 reg, int32_t index)
{
    float temp[sizeof(__m256) / sizeof(float)];
    _mm256_store_ps(temp, reg);
    return temp[index];
}

static inline double
simd_extract_f64_from_256(__m256d reg, int32_t index)
{
    double temp[sizeof(__m256d) / sizeof(double)];
    _mm256_store_pd(temp, reg);
    return temp[index];
}

// 128 bits

#define DEFINE_EXTRACT_128_SIMD_VALUE(TYPE, FUNCNAME) static inline TYPE \
FUNCNAME(__m128i reg, int32_t index) { \
    uint8_t temp[sizeof(__m128i)]; \
    _mm_store_si128((__m128i*)temp, reg); \
    return ((TYPE*)temp)[index]; \
}

DEFINE_EXTRACT_128_SIMD_VALUE(uint8_t, simd_extract_u8_from_128)
DEFINE_EXTRACT_128_SIMD_VALUE(int8_t, simd_extract_s8_from_128)
DEFINE_EXTRACT_128_SIMD_VALUE(uint16_t, simd_extract_u16_from_128)
DEFINE_EXTRACT_128_SIMD_VALUE(int16_t, simd_extract_s16_from_128)
DEFINE_EXTRACT_128_SIMD_VALUE(uint32_t, simd_extract_u32_from_128)
DEFINE_EXTRACT_128_SIMD_VALUE(int32_t, simd_extract_s32_from_128)
DEFINE_EXTRACT_128_SIMD_VALUE(uint64_t, simd_extract_u64_from_128)
DEFINE_EXTRACT_128_SIMD_VALUE(int64_t, simd_extract_s64_from_128)

static inline float
simd_extract_f32_from_128(__m128 reg, int32_t index)
{
    float temp[sizeof(__m128) / sizeof(float)];
    _mm_store_ps(temp, reg);
    return temp[index];
}

static inline double
simd_extract_f64_from_128(__m128d reg, int32_t index)
{
    double temp[sizeof(__m128d) / sizeof(double)];
    _mm_store_pd(temp, reg);
    return temp[index];
}