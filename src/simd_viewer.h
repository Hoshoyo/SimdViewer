#pragma once
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <raylib.h>
#include <raymath.h>

#define BYTE_SIZE 48
#define SPACING 1
#define BORDER_SIZE 4
#define Y_SPACING 16
#define BACKGROUND_COLOR (Color) { 0x50, 0x50, 0x50, 255 }
#define FONT_COLOR (Color) { 0x10, 0x10, 0x10, 255 }

typedef enum {
	REGISTER_TYPE_NONE,
	
	REGISTER_TYPE_S8,
	REGISTER_TYPE_S16,
	REGISTER_TYPE_S32,
	REGISTER_TYPE_S64,

	REGISTER_TYPE_U8,
	REGISTER_TYPE_U16,
	REGISTER_TYPE_U32,
	REGISTER_TYPE_U64,

	REGISTER_TYPE_F32,
	REGISTER_TYPE_F64,
} RegisterType;

typedef enum {
	FREGISTER_TYPE_NONE,
	FREGISTER_TYPE_F32,
	FREGISTER_TYPE_F64,
} FRegisterType;

typedef struct {
	RegisterType type; // division type

	uint32_t register_size_bytes;
	union {
		uint8_t  u8val;
		int8_t   s8val;
		uint16_t u16val;
		int16_t  s16val;
		uint32_t u32val;
		int32_t  s32val;
		uint64_t u64val;
		int64_t  s64val;
		__m128i  i128;
		__m128   f128;
		__m128d  f128d;
		__m256i  i256;
		__m256   f256;
		__m256d  f256d;
	};
} AnyValue;

typedef struct {
	int32_t  frame_index;
	AnyValue frame_value;

	int32_t  last_index;
	AnyValue last_value;
} ValueHovered;

typedef uint32_t RenderFlag;

static const RenderFlag SIMD_VIEWER_RENDER_HEX = (1 << 0);
static const RenderFlag SIMD_VIEWER_RENDER_BORDER = (1 << 1);
static const RenderFlag SIMD_VIEWER_RENDER_HIGHLIGHT = (1 << 2);
static const RenderFlag SIMD_VIEWER_RENDER_VALUE_INTENSITY = (1 << 3);

typedef struct {
	Font font;
	RenderFlag default_render_flags;
	RenderFlag pushed_flags;

	ValueHovered hovered;

	uint32_t highlight_size;

	uint32_t stack_index;
} SimdViewer;

// Initialization
void simd_viewer_init(SimdViewer* simd_viewer);

// Flush
void simd_viewer_flush(SimdViewer* simd_viewer);

// ----------------------------------------------------------------------------------------------
// Push

// Helpers and Markers
void simd_viewer_push_highlighter(SimdViewer* simd_viewer);
void simd_viewer_push_operation(SimdViewer* simd_viewer, RegisterType regtype, const char* name);
void simd_viewer_push_empty(SimdViewer* simd_viewer);

// 256 bits
void simd_viewer_push(SimdViewer* simd_viewer, __m256i reg, RegisterType regtype);
void simd_viewer_push_bold(SimdViewer* simd_viewer, __m256i reg, RegisterType regtype);
void simd_viewer_pushf(SimdViewer* simd_viewer, __m256 reg, RegisterType regtype);
void simd_viewer_pushf_bold(SimdViewer* simd_viewer, __m256 reg, FRegisterType regtype);
void simd_viewer_pushd(SimdViewer* simd_viewer, __m256d reg, RegisterType regtype);
void simd_viewer_pushd_bold(SimdViewer* simd_viewer, __m256d reg, RegisterType regtype);

// 128 bits
void simd_viewer_push128(SimdViewer* simd_viewer, __m128i reg, RegisterType regtype);
void simd_viewer_push128f(SimdViewer* simd_viewer, __m128 reg, RegisterType regtype);
void simd_viewer_push128_bold(SimdViewer* simd_viewer, __m128i reg, RegisterType regtype);
void simd_viewer_push128f_bold(SimdViewer* simd_viewer, __m128 reg, RegisterType regtype);


// ----------------------------------------------------------------------------------------------
// Rendering
void simd_viewer_reset_flags(SimdViewer* simd_viewer);
void simd_viewer_set_hexadecimal_render(SimdViewer* simd_viewer);
void simd_viewer_set_decimal_render(SimdViewer* simd_viewer);
void simd_viewer_set_highlight_size(SimdViewer* simd_viewer, RegisterType regtype);
void simd_viewer_reset_hightlight_size(SimdViewer* simd_viewer);
void simd_viewer_enable_hightlight_size(SimdViewer* simd_viewer);
void simd_viewer_disable_hightlight_size(SimdViewer* simd_viewer);