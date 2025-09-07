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
simd_add_float256(SimdViewer* sv)
{
	simd_viewer_push_highlighter(sv);

#if 0
	__m256 value0 = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
	simd_viewer_pushf(sv, value0, REGISTER_TYPE_F32);

	__m256 value1 = _mm256_set_ps(10.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f);
	simd_viewer_pushf(sv, value1, REGISTER_TYPE_F32);

	__m256 result = _mm256_add_ps(value0, value1);
	simd_viewer_push_operation(sv, REGISTER_TYPE_F32, "_mm256_add_ps");
	simd_viewer_pushf_bold(sv, result, REGISTER_TYPE_F32);
#else
	__m256d value0 = _mm256_set_pd(1.0, 2.0, 3.0, 4.0);
	simd_viewer_pushd(sv, value0, REGISTER_TYPE_F64);

	__m256d value1 = _mm256_set_pd(10.0, 12.0, 13.0, 14.0);
	simd_viewer_pushd(sv, value1, REGISTER_TYPE_F64);

	__m256d result = _mm256_add_pd(value0, value1);
	simd_viewer_push_operation(sv, REGISTER_TYPE_F64, "_mm256_add_pd");
	simd_viewer_pushd_bold(sv, result, REGISTER_TYPE_F64);
#endif
}

void
simd_add_int128(SimdViewer* sv)
{
	simd_viewer_push_highlighter(sv);

	__m128i value0 = _mm_set_epi32(1, 2, 3, 4);
	simd_viewer_push128(sv, value0, REGISTER_TYPE_S32);
	__m128i value1 = _mm_set_epi32(5, 6, 7, 8);
	simd_viewer_push128(sv, value1, REGISTER_TYPE_S32);
	
	__m128i result = _mm_add_epi32(value0, value1);
	simd_viewer_push_operation(sv, REGISTER_TYPE_S32, "_mm_add_epi32");
	simd_viewer_push128_bold(sv, result, REGISTER_TYPE_S32);
}

void
simd_add_float128(SimdViewer* sv)
{
	simd_viewer_push_highlighter(sv);

	__m128 value0 = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
	simd_viewer_push128f(sv, value0, REGISTER_TYPE_F32);
	__m128 value1 = _mm_set_ps(5.0f, 6.0f, 7.0f, 8.0f);
	simd_viewer_push128f(sv, value1, REGISTER_TYPE_F32);

	__m128 result = _mm_add_ps(value0, value1);
	simd_viewer_push_operation(sv, REGISTER_TYPE_F32, "_mm_add_ps");
	simd_viewer_push128f_bold(sv, result, REGISTER_TYPE_F32);
}

void
horizontal_max(SimdViewer* sv)
{
	//simd_viewer_push_highlighter(sv);
	simd_viewer_set_hexadecimal_render(sv);

	__m256d value0 = _mm256_castsi256_pd(_mm256_set_epi64x(1, 2, 3, 4));
	simd_viewer_push(sv, _mm256_castpd_si256(value0), REGISTER_TYPE_U64);

	__m256d y = _mm256_permute2f128_pd(value0, value0, 1); // permute 128-bit values
	simd_viewer_push_operation(sv, REGISTER_TYPE_U64, "_mm256_permute2f128_pd[1]");
	simd_viewer_push_bold(sv, _mm256_castpd_si256(y), REGISTER_TYPE_U64);

	simd_viewer_push_empty(sv);

	__m256d m1 = _mm256_max_pd(value0, y); // m1[0] = max(x[0], x[2]), m1[1] = max(x[1], x[3]), etc.
	simd_viewer_push(sv, _mm256_castpd_si256(value0), REGISTER_TYPE_U64);
	simd_viewer_push_operation(sv, REGISTER_TYPE_U64, "_mm256_max_pd");
	simd_viewer_push(sv, _mm256_castpd_si256(y), REGISTER_TYPE_U64);

	simd_viewer_push_bold(sv, _mm256_castpd_si256(m1), REGISTER_TYPE_U64);

	__m256d m2 = _mm256_permute_pd(m1, 5); // set m2[0] = m1[1], m2[1] = m1[0], etc.
	simd_viewer_push_operation(sv, REGISTER_TYPE_U64, "_mm256_permute_pd[5]");
	simd_viewer_push_bold(sv, _mm256_castpd_si256(m2), REGISTER_TYPE_U64);

	__m256d m = _mm256_max_pd(m1, m2); // all m[0] ... m[3] contain the horizontal max(x[0], x[1], x[2], x[3])
	simd_viewer_push_operation(sv, REGISTER_TYPE_U64, "_mm256_max_pd");
	simd_viewer_push(sv, _mm256_castpd_si256(m), REGISTER_TYPE_U64);
}

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define MIN(A, B) (((A) < (B)) ? (A) : (B))

int main()
{
	Font font = {0};
	InitWindow(1600, 900, "Intrinsics");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);
	//SetExitKey(KEY_NULL);

	SimdViewer sv = { 0 };
	simd_viewer_init(&sv);

	sv.camera.zoom = 1.0f;
	sv.camera.target = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	Vector2 position = sv.camera.target;

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BACKGROUND_COLOR);

		BeginMode2D(sv.camera);

		float wheel = GetMouseWheelMove();
		if (wheel != 0)
		{
			sv.camera.zoom += (wheel * 0.1f);
			sv.camera.zoom = MIN(MAX(sv.camera.zoom, 0.1f), 4.0f);
		}

		Vector2 center = {
			-GetScreenWidth() / 2.0f,
			-GetScreenHeight() / 2.0f
		};
		center = Vector2Scale(center, 1.0f / sv.camera.zoom);
		sv.camera.target = Vector2Add(center, position);

		sv.camera.target.x = round(sv.camera.target.x);
		sv.camera.target.y = round(sv.camera.target.y);

		Vector2 mouse_position = GetMousePosition();
		Vector2 mouse_delta = GetMouseDelta();
		mouse_delta = Vector2Scale(mouse_delta, 1.0f / sv.camera.zoom);

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			//camera.target = Vector2Subtract(camera.target, mouse_delta);
			position = Vector2Subtract(position, mouse_delta);
		}

		{
			// Examples, uncomment to see
			//simd_unpack(&sv);
			//simd_compare(&sv);
			//simd_average(&sv);
			//simd_movehdup(&sv);
			//simd_compare_string(&sv);
			//simd_add_float256(&sv);
			//simd_add_int128(&sv);
			//simd_add_float128(&sv);

			horizontal_max(&sv);
		}

		simd_viewer_flush(&sv);

		EndMode2D();

		DrawText(TextFormat("Zoom: %f | %f %f", sv.camera.zoom, sv.camera.target.x, sv.camera.target.y), 0, 0, 20, WHITE);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void
find_32x_u8(uint8_t input[32], uint8_t output[32], const bool input_ignore[32], const bool output_ignore[32])
{
	
}

void
find()
{
	//Instruction* i = find_binop_32x_u8(3, 4, 7);
	//Instruction* i = find_binop_16x_u16(3, 4, 7);
	//Instruction* i = find_binop_8x_u32(3, 4, 7);
	//Instruction* i = find_binop_4x_u64(3, 4, 7);
	
	uint8_t mask_ignore_none[32] = { 0 };

	uint8_t input[32] = { 0 };
	uint8_t output[32] = { 0 };

	input[0] = 1;
	output[1] = 1;

	find_32x_u8(input, output, mask_ignore_none, mask_ignore_none);
}