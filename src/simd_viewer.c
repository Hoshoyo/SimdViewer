#include "simd_viewer.h"

#define ARRAY_LENGTH(A) (sizeof(A) / sizeof(*(A)))

inline Vector2
vec2(float v)
{
	return (Vector2) { v, v };
}

inline int
box_width(int byte_count)
{
	return (BYTE_SIZE + SPACING) * byte_count - SPACING;
}

inline const char*
fmt_signed(bool hex)
{
	return (hex) ? "0x%x" : "%d";
}
inline const char*
fmt_unsigned(bool hex)
{
	return (hex) ? "0x%x" : "%u";
}
inline const char*
fmt_signed64(bool hex)
{
	return (hex) ? "0x%llx" : "%lld";
}
inline const char*
fmt_unsigned64(bool hex)
{
	return (hex) ? "0x%llx" : "%llu";
}

inline int
regtype_to_bytesize(RegisterType regtype)
{
	switch (regtype)
	{
	default:
		case REGISTER_TYPE_S8:case REGISTER_TYPE_U8: return sizeof(int8_t);
		case REGISTER_TYPE_S16:case REGISTER_TYPE_U16: return sizeof(int16_t);
		case REGISTER_TYPE_S32:case REGISTER_TYPE_U32: return sizeof(int32_t);
		case REGISTER_TYPE_S64:case REGISTER_TYPE_U64: return sizeof(int64_t);
	}
}

inline Vector2
line_position(int index)
{
	return (Vector2) { 20.0f, 20.0f + (float)((BYTE_SIZE + Y_SPACING) * index) };
}

Rectangle
render_box_colored(Vector2 pos, int byte_size, Color color)
{
	Rectangle rect = {
		.x = pos.x,
		.y = pos.y,
		.width = (float)box_width(byte_size),
		.height = (float)BYTE_SIZE
	};
	DrawRectangleRec(rect, color);
	return rect;
}

void
render_linebox_colored(Vector2 pos, int byte_size, Color color)
{
	DrawRectangleLines((int)pos.x, (int)pos.y + 1, box_width(byte_size), BYTE_SIZE - 2, color);
}

Rectangle
render_box(Vector2 pos, int byte_size)
{
	Color c = LIGHTGRAY;
	c.r += 20;
	return render_box_colored(pos, byte_size, c);
}

Rectangle
render_box_highlight(Vector2 pos, int byte_size, bool should_highlight)
{
	Color c = LIGHTGRAY;
	c.r += 20;
	if (should_highlight)
	{
		c.r = 255;
		c.g += 30;
	}
	return render_box_colored(pos, byte_size, c);
}

void
render_text_centered(Vector2 pos, Font font, const char* text, int width)
{
	Vector2 measure = MeasureTextEx(font, text, (float)font.baseSize, 0);
	DrawTextEx(font, text, Vector2Add(pos, (Vector2) { width / 2 - measure.x / 2, BYTE_SIZE / 2 - measure.y / 2 }), (float)font.baseSize, 0, FONT_COLOR);
}

void
render_text_rightalign(Vector2 pos, Font font, const char* text, int width)
{
	Vector2 measure = MeasureTextEx(font, text, (float)font.baseSize, 0);
	DrawTextEx(font, text, Vector2Add(pos, (Vector2) { width - measure.x - 4, BYTE_SIZE / 2 - measure.y / 2 }), (float)font.baseSize, 0, FONT_COLOR);
}

AnyValue
render_register256(SimdViewer* sv, Vector2 pos, __m256i x, RegisterType rtype, RenderFlag flag)
{
	uint32_t highlight_size = sv->highlight_size;
	AnyValue render_hovered = sv->hovered;
	Font font = sv->font;

	Vector2 initial_pos = pos;

	bool hex = flag & SIMD_VIEWER_RENDER_HEX;

	if (flag & SIMD_VIEWER_RENDER_BORDER)
	{
		Vector2 border_pos = (Vector2){ pos.x - BORDER_SIZE, pos.y - BORDER_SIZE };
		Rectangle border_rect = (Rectangle){ .x = pos.x - BORDER_SIZE, .y = pos.y - BORDER_SIZE, .width = (float)(box_width(sizeof(__m256i)) + BORDER_SIZE * 2), BYTE_SIZE + BORDER_SIZE * 2 };
		DrawRectangleLinesEx(border_rect, BORDER_SIZE, (Color) { 0x20, 0x20, 0x20, 0xff });
	}

	Vector2 mouse = GetMousePosition();
	Rectangle rect = { 0 };
	AnyValue hovered_value = { 0 };

	switch (rtype)
	{
		case REGISTER_TYPE_S8: {
			for (int i = sizeof(__m256i) - 1; i >= 0; --i)
			{
				if (flag & SIMD_VIEWER_RENDER_VALUE_INTENSITY)
					rect = render_box_colored(pos, sizeof(int8_t), (Color){ x.m256i_u8[i], x.m256i_u8[i], x.m256i_u8[i], 255 });
				else
					rect = render_box_highlight(pos, sizeof(int8_t), render_hovered.type == rtype && x.m256i_i8[i] == render_hovered.s8val);
				render_text_rightalign(pos, font, TextFormat(fmt_signed(hex), x.m256i_i8[i]), box_width(sizeof(int8_t)));

				pos = Vector2Add(pos, (Vector2) { BYTE_SIZE + SPACING, 0 });

				if (CheckCollisionPointRec(mouse, rect))
				{
					hovered_value.s8val = x.m256i_i8[i];
					hovered_value.type = rtype;
				}
			}
		} break;
		case REGISTER_TYPE_U8: {
			for (int i = sizeof(__m256i) - 1; i >= 0; --i)
			{
				if (flag & SIMD_VIEWER_RENDER_VALUE_INTENSITY)
					rect = render_box_colored(pos, sizeof(int8_t), (Color) { x.m256i_u8[i], x.m256i_u8[i], x.m256i_u8[i], 255 });
				else
					rect = render_box_highlight(pos, sizeof(uint8_t), render_hovered.type == rtype && x.m256i_u8[i] == render_hovered.u8val);
				render_text_rightalign(pos, font, TextFormat(fmt_unsigned(hex), x.m256i_u8[i]), box_width(sizeof(uint8_t)));

				pos = Vector2Add(pos, (Vector2) { BYTE_SIZE + SPACING, 0 });

				if (CheckCollisionPointRec(mouse, rect))
				{
					hovered_value.u8val = x.m256i_u8[i];
					hovered_value.type = rtype;
				}
			}
		} break;
		case REGISTER_TYPE_S16: {
			for (int i = sizeof(__m256i) / sizeof(int16_t) - 1; i >= 0; --i)
			{
				rect = render_box(pos, sizeof(int16_t));
				render_text_rightalign(pos, font, TextFormat(fmt_signed(hex), x.m256i_i16[i]), box_width(sizeof(int16_t)));

				pos = Vector2Add(pos, (Vector2) { (BYTE_SIZE + SPACING) * sizeof(int16_t), 0 });

				if (CheckCollisionPointRec(mouse, rect))
				{
					hovered_value.s16val = x.m256i_i16[i];
					hovered_value.type = rtype;
				}
			}
		} break;
		case REGISTER_TYPE_U16: {
			for (int i = sizeof(__m256i) / sizeof(uint16_t) - 1; i >= 0; --i)
			{
				rect = render_box(pos, sizeof(int16_t));
				render_text_rightalign(pos, font, TextFormat(fmt_unsigned(hex), x.m256i_u16[i]), box_width(sizeof(int16_t)));

				pos = Vector2Add(pos, (Vector2) { (BYTE_SIZE + SPACING) * sizeof(int16_t), 0 });

				if (CheckCollisionPointRec(mouse, rect))
				{
					hovered_value.u16val = x.m256i_u16[i];
					hovered_value.type = rtype;
				}
			}
		} break;
		case REGISTER_TYPE_S32: {
			for (int i = sizeof(__m256i) / sizeof(int32_t) - 1; i >= 0; --i)
			{
				rect = render_box(pos, sizeof(int32_t));
				render_text_rightalign(pos, font, TextFormat(fmt_signed(hex), x.m256i_i32[i]), box_width(sizeof(int32_t)));

				pos = Vector2Add(pos, (Vector2) { (BYTE_SIZE + SPACING) * sizeof(int32_t), 0 });

				if (CheckCollisionPointRec(mouse, rect))
				{
					hovered_value.s32val = x.m256i_i32[i];
					hovered_value.type = rtype;
				}
			}
		} break;
		case REGISTER_TYPE_U32: {
			for (int i = sizeof(__m256i) / sizeof(uint32_t) - 1; i >= 0; --i)
			{
				rect = render_box(pos, sizeof(uint32_t));
				render_text_rightalign(pos, font, TextFormat(fmt_unsigned(hex), x.m256i_u32[i]), box_width(sizeof(uint32_t)));

				pos = Vector2Add(pos, (Vector2) { (BYTE_SIZE + SPACING) * sizeof(uint32_t), 0 });

				if (CheckCollisionPointRec(mouse, rect))
				{
					hovered_value.u32val = x.m256i_u32[i];
					hovered_value.type = rtype;
				}
			}
		} break;
		case REGISTER_TYPE_S64: {
			for (int i = sizeof(__m256i) / sizeof(int64_t) - 1; i >= 0; --i)
			{
				rect = render_box(pos, sizeof(int64_t));
				render_text_rightalign(pos, font, TextFormat(fmt_signed64(hex), x.m256i_i64[i]), box_width(sizeof(int64_t)));

				pos = Vector2Add(pos, (Vector2) { (BYTE_SIZE + SPACING) * sizeof(int64_t), 0 });

				if (CheckCollisionPointRec(mouse, rect))
				{
					hovered_value.s64val = x.m256i_i64[i];
					hovered_value.type = rtype;
				}
			}
		} break;
		case REGISTER_TYPE_U64: {
			for (int i = sizeof(__m256i) / sizeof(uint64_t) - 1; i >= 0; --i)
			{
				rect = render_box(pos, sizeof(uint64_t));
				render_text_rightalign(pos, font, TextFormat(fmt_unsigned64(hex), x.m256i_u64[i]), box_width(sizeof(uint64_t)));

				pos = Vector2Add(pos, (Vector2) { (BYTE_SIZE + SPACING) * sizeof(uint64_t), 0 });

				if (CheckCollisionPointRec(mouse, rect))
				{
					hovered_value.u64val = x.m256i_u64[i];
					hovered_value.type = rtype;
				}
			}
		} break;
	}

	if (flag & SIMD_VIEWER_RENDER_HIGHLIGHT && highlight_size > 0)
	{
		Vector2 sz_pos = initial_pos;
		Color highlight_color = GOLD;
		highlight_color.a = 60;

		for (int i = 0; i < sizeof(__m256) / highlight_size; ++i)
		{
			if (i % 2 == 0)
				render_box_colored(sz_pos, highlight_size, highlight_color);
			sz_pos = Vector2Add(sz_pos, (Vector2) { (BYTE_SIZE + SPACING) * highlight_size, 0 });
		}
	}

	return hovered_value;
}

void
render_register256f(Font font, Vector2 pos, __m256 x, FRegisterType rtype)
{
	switch (rtype)
	{
	case FREGISTER_TYPE_F32: {
		for (int i = sizeof(__m256) / sizeof(float) - 1; i >= 0; --i)
		{
			render_box(pos, sizeof(float));
			render_text_rightalign(pos, font, TextFormat("%.12f", x.m256_f32[i]), box_width(sizeof(float)));

			pos = Vector2Add(pos, (Vector2) { (BYTE_SIZE + SPACING) * sizeof(float), 0 });
		}
	} break;
	case FREGISTER_TYPE_F64: {
		for (int i = sizeof(__m256d) / sizeof(double) - 1; i >= 0; --i)
		{
			render_box(pos, sizeof(double));
			render_text_rightalign(pos, font, TextFormat("%.16f", (*(__m256d*) & x).m256d_f64[i]), box_width(sizeof(double)));

			pos = Vector2Add(pos, (Vector2) { (BYTE_SIZE + SPACING) * sizeof(double), 0 });
		}
	} break;
	}
}

void
render_operation256(Font font, Vector2 pos, const char* description, int byte_size)
{
	Color redish = RED;
	redish.a = 200;
	render_box_colored(pos, sizeof(__m256i), redish);

	Vector2 inner_pos = pos;
	for (int i = sizeof(__m256d) / byte_size - 1; i >= 0; --i)
	{
		render_linebox_colored(inner_pos, byte_size, (Color) { 0x50, 0x50, 0x50, 0x30 });
		inner_pos = Vector2Add(inner_pos, (Vector2) { (float)((BYTE_SIZE + SPACING) * byte_size), 0 });
	}

	render_text_centered(pos, font, description, box_width(sizeof(__m256i)));
}

// Initialization
void 
simd_viewer_init(SimdViewer* simd_viewer)
{
	simd_viewer->font = LoadFontEx("C:\\Windows\\Fonts\\consola.ttf", 20, 0, 1024);
	simd_viewer->default_render_flags = 0;
	simd_viewer->stack_index = 0;
	simd_viewer->highlight_size = 0;
}

// Flush
void
simd_viewer_flush(SimdViewer* simd_viewer)
{
	simd_viewer->stack_index = 0;
	simd_viewer->pushed_flags = 0;
	simd_viewer->hovered = simd_viewer->frame_hovered;
	simd_viewer->frame_hovered = (AnyValue){ 0 };
}

// Push
void
simd_viewer_push(SimdViewer* simd_viewer, __m256i reg, RegisterType regtype)
{
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags;
	AnyValue hovered = render_register256(simd_viewer, line_position(index), reg, regtype, flags);
	if (hovered.type != REGISTER_TYPE_NONE)
		simd_viewer->frame_hovered = hovered;
}

void 
simd_viewer_push_bold(SimdViewer* simd_viewer, __m256i reg, RegisterType regtype)
{
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags | SIMD_VIEWER_RENDER_BORDER;
	AnyValue hovered = render_register256(simd_viewer, line_position(index), reg, regtype, flags);
	if (hovered.type != REGISTER_TYPE_NONE)
		simd_viewer->frame_hovered = hovered;
}

void
simd_viewer_push_operation(SimdViewer* simd_viewer, RegisterType regtype, const char* name)
{
	uint32_t index = simd_viewer->stack_index++;
	render_operation256(simd_viewer->font, line_position(index), name, regtype_to_bytesize(regtype));
}

void 
simd_viewer_push_empty(SimdViewer* simd_viewer)
{
	simd_viewer->stack_index++;
}

void
simd_viewer_push_highlighter(SimdViewer* simd_viewer)
{
	Color overlay = (Color){ 50, 50, 50, 50 };
	Vector2 mouse = GetMousePosition();

	uint32_t index = simd_viewer->stack_index++;

	simd_viewer->pushed_flags |= SIMD_VIEWER_RENDER_HIGHLIGHT;

	Vector2 pos = line_position(index);
	render_box_colored(pos, sizeof(__m256i), ORANGE);

	Rectangle rects[] = {
		{.x = pos.x, .y = pos.y, box_width(sizeof(uint8_t)), BYTE_SIZE },
		{.x = pos.x, .y = pos.y, box_width(sizeof(uint16_t)), BYTE_SIZE },
		{.x = pos.x, .y = pos.y, box_width(sizeof(uint32_t)), BYTE_SIZE },
		{.x = pos.x, .y = pos.y, box_width(sizeof(uint64_t)), BYTE_SIZE },
		{.x = pos.x, .y = pos.y, box_width(sizeof(__m128i)), BYTE_SIZE },
		{.x = pos.x, .y = pos.y, box_width(sizeof(__m256i)), BYTE_SIZE },
	};
	int sizes[] = {
		sizeof(uint8_t),
		sizeof(uint16_t),
		sizeof(uint32_t),
		sizeof(uint64_t),
		sizeof(__m128i),
		sizeof(__m256i),
	};

	for (int i = 0; i < ARRAY_LENGTH(rects); ++i)
	{
		if (CheckCollisionPointRec(mouse, rects[i]))
		{
			overlay = ColorBrightness(overlay, -4.0f);
			render_box_colored(pos, sizes[i], overlay);
			render_linebox_colored(pos, sizes[i], overlay);
			render_text_centered(pos, simd_viewer->font, TextFormat("Register %d bits", sizes[i] * 8), box_width(sizeof(__m256i)));
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				simd_viewer->highlight_size = sizes[i];
			}
			if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
			{
				simd_viewer->highlight_size = 0;
			}
			return;
		}
	}

	if (simd_viewer->highlight_size > 0)
	{
		render_linebox_colored(pos, simd_viewer->highlight_size, RAYWHITE);
	}
	
	render_box_colored(pos, sizeof(uint8_t), overlay);
	render_box_colored(pos, sizeof(uint16_t), overlay);
	render_box_colored(pos, sizeof(uint32_t), overlay);
	render_box_colored(pos, sizeof(uint64_t), overlay);
	render_box_colored(pos, sizeof(__m128i), overlay);
}

// Flags
void 
simd_viewer_reset_flags(SimdViewer* simd_viewer)
{
	simd_viewer->pushed_flags;
}

void 
simd_viewer_set_hexadecimal_render(SimdViewer* simd_viewer)
{
	simd_viewer->pushed_flags |= SIMD_VIEWER_RENDER_HEX;
}

void 
simd_viewer_set_decimal_render(SimdViewer* simd_viewer)
{
	simd_viewer->pushed_flags &= ~SIMD_VIEWER_RENDER_HEX;
}

void
simd_viewer_enable_hightlight_size(SimdViewer* simd_viewer)
{
	simd_viewer->pushed_flags |= SIMD_VIEWER_RENDER_HIGHLIGHT;
}

void
simd_viewer_disable_hightlight_size(SimdViewer* simd_viewer)
{
	simd_viewer->pushed_flags &= ~SIMD_VIEWER_RENDER_HIGHLIGHT;
}

void
simd_viewer_set_highlight_size(SimdViewer* simd_viewer, RegisterType regtype)
{
	simd_viewer->highlight_size = regtype_to_bytesize(regtype);
	simd_viewer_enable_hightlight_size(simd_viewer);
}

void 
simd_viewer_reset_hightlight_size(SimdViewer* simd_viewer)
{
	simd_viewer->highlight_size = 0;
	simd_viewer_disable_hightlight_size(simd_viewer);
}