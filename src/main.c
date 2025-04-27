#include "simd_viewer.h"

void
simd_compare(SimdViewer* sv)
{
	__m256i value0 = _mm256_set_epi64x(1, -127, 3, 4); 
	simd_viewer_push(sv, value0, REGISTER_TYPE_S64);
	__m256i value1 = _mm256_set_epi64x(0x12345678ABCDEF, 6, 3, 8);
	simd_viewer_push(sv, value1, REGISTER_TYPE_S64);
	__m256i result = _mm256_cmpeq_epi64(value0, value1);
	simd_viewer_push_operation(sv, REGISTER_TYPE_S64, "_mm256_cmpeq_epi64");

	simd_viewer_set_hexadecimal_render(sv);
	simd_viewer_push_bold(sv, result, REGISTER_TYPE_U64);

	simd_viewer_push_empty(sv);
	simd_viewer_set_decimal_render(sv);
	simd_viewer_push(sv, value1, REGISTER_TYPE_S64);
}

void
simd_unpack(SimdViewer* sv)
{
	simd_viewer_push_highlighter(sv);

	{
		__m256i value0 = _mm256_set_epi8(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
		simd_viewer_push(sv, value0, REGISTER_TYPE_U8);

		__m256i value1 = _mm256_set_epi8(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);
		simd_viewer_push(sv, value1, REGISTER_TYPE_U8);

		__m256i result = _mm256_unpacklo_epi8(value0, value1);
		simd_viewer_push_operation(sv, REGISTER_TYPE_U8, "_mm256_unpacklo_epi8");
		simd_viewer_push_bold(sv, result, REGISTER_TYPE_U8);
	}

	simd_viewer_push_empty(sv);
	
	{
		__m256i value0 = _mm256_set_epi8(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
		simd_viewer_push(sv, value0, REGISTER_TYPE_U8);

		__m256i value1 = _mm256_set_epi8(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);
		simd_viewer_push(sv, value1, REGISTER_TYPE_U8);

		__m256i result = _mm256_unpackhi_epi8(value0, value1);
		simd_viewer_push_operation(sv, REGISTER_TYPE_U8, "_mm256_unpackhi_epi8");
		simd_viewer_push_bold(sv, result, REGISTER_TYPE_U8);
	}
}

void
simd_average(SimdViewer* sv)
{
	simd_viewer_push_highlighter(sv);

	{
		__m256i value0 = _mm256_set_epi8(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
		simd_viewer_push(sv, value0, REGISTER_TYPE_U8);
		__m256i value1 = _mm256_set_epi8(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);
		simd_viewer_push(sv, value1, REGISTER_TYPE_U8);

		__m256i result = _mm256_avg_epu8(value0, value1);
		simd_viewer_push_operation(sv, REGISTER_TYPE_U8, "_mm256_avg_epu8");
		simd_viewer_push_bold(sv, result, REGISTER_TYPE_U8);
	}

	simd_viewer_push_empty(sv);

	{
		__m256i value0 = _mm256_set_epi16(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		simd_viewer_push(sv, value0, REGISTER_TYPE_U16);
		__m256i value1 = _mm256_set_epi16(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48);
		simd_viewer_push(sv, value1, REGISTER_TYPE_U16);

		__m256i result = _mm256_avg_epu16(value0, value1);
		simd_viewer_push_operation(sv, REGISTER_TYPE_U16, "_mm256_avg_epu16");
		simd_viewer_push_bold(sv, result, REGISTER_TYPE_U16);
	}
}

void
simd_movehdup(SimdViewer* sv)
{
	simd_viewer_push_highlighter(sv);

	__m256 value = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
	simd_viewer_pushf(sv, value, FREGISTER_TYPE_F32);
	__m256 result = _mm256_movehdup_ps(value);
	simd_viewer_pushf_bold(sv, result, FREGISTER_TYPE_F32);
}

void
simd_multiply(SimdViewer* sv)
{
	simd_viewer_push_highlighter(sv);
	
	__m256i value0 = _mm256_set_epi8(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
	simd_viewer_push(sv, value0, REGISTER_TYPE_U8);
	__m256i value1 = _mm256_set_epi8(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);
	simd_viewer_push(sv, value1, REGISTER_TYPE_U8);
	__m256i value2 = _mm256_set_epi8(65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96);
	simd_viewer_push(sv, value1, REGISTER_TYPE_U8);

	__m256i result = _mm256_dpbssd_epi32(value0, value1, value2);
	simd_viewer_push_operation(sv, REGISTER_TYPE_U32, "_mm256_dpbssd_epi32");
	simd_viewer_push_bold(sv, result, REGISTER_TYPE_U8);
}

void
simd_compare_string(SimdViewer* sv)
{
	//_mm_cmpestrz()

}

int main()
{
	Font font = {0};
	InitWindow(1600, 900, "Intrinsics");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);
	//SetExitKey(KEY_NULL);

	SimdViewer sv = { 0 };
	simd_viewer_init(&sv);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BACKGROUND_COLOR);

		simd_unpack(&sv);
		//simd_compare(&sv);
		//simd_average(&sv);
		//simd_movehdup(&sv);
		//simd_multiply(&sv);
		//simd_compare_string(&sv);
		simd_viewer_flush(&sv);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}