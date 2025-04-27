#include "simd_viewer.h"
#include "simd_utils.h"
#include <assert.h>

#define ARRAY_LENGTH(A) (sizeof(A) / sizeof(*(A)))
#define function static

function Vector2
vec2(float v)
{
	return (Vector2) { v, v };
}

function inline int
box_width(int byte_count)
{
	return (BYTE_SIZE + SPACING) * byte_count - SPACING;
}

function const char*
fmt_signed(bool hex)
{
	return (hex) ? "0x%x" : "%d";
}
function const char*
fmt_unsigned(bool hex)
{
	return (hex) ? "0x%x" : "%u";
}
function const char*
fmt_signed64(bool hex)
{
	return (hex) ? "0x%llx" : "%lld";
}
function const char*
fmt_unsigned64(bool hex)
{
	return (hex) ? "0x%llx" : "%llu";
}

function bool
regtype_is_signed(RegisterType rtype)
{
	return rtype >= REGISTER_TYPE_S8 && rtype <= REGISTER_TYPE_S64;
}

function bool
regtype_is_64bit(RegisterType rtype)
{
	return rtype == REGISTER_TYPE_S64 || rtype == REGISTER_TYPE_U64;
}

function const char*
fmt(bool hex, RegisterType rtype)
{
	if (hex)
	{
		return (regtype_is_64bit(rtype)) ? "0x%llx" : "0x%x";
	}
	else
	{
		if(regtype_is_signed(rtype))
			return (regtype_is_64bit(rtype)) ? "%lld" : "%d";
		else
			return (regtype_is_64bit(rtype)) ? "%llu" : "%u";
	}
}

function int
regtype_to_bytesize(RegisterType regtype)
{
	switch (regtype)
	{
	default:
		case REGISTER_TYPE_S8:  case REGISTER_TYPE_U8: return sizeof(int8_t);
		case REGISTER_TYPE_S16: case REGISTER_TYPE_U16: return sizeof(int16_t);
		case REGISTER_TYPE_S32: case REGISTER_TYPE_U32: return sizeof(int32_t);
		case REGISTER_TYPE_S64: case REGISTER_TYPE_U64: return sizeof(int64_t);
		case REGISTER_TYPE_F32: return sizeof(float);
		case REGISTER_TYPE_F64: return sizeof(double);
	}
}

function Vector2
line_position(int index)
{
	return (Vector2) { 20.0f, 20.0f + (float)((BYTE_SIZE + Y_SPACING) * index) };
}

function Rectangle
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

function void
render_linebox_colored(Vector2 pos, int byte_size, Color color)
{
	DrawRectangleLines((int)pos.x, (int)pos.y + 1, box_width(byte_size), BYTE_SIZE - 2, color);
}

function Rectangle
render_box(Vector2 pos, int byte_size)
{
	Color c = LIGHTGRAY;
	c.r += 20;
	return render_box_colored(pos, byte_size, c);
}

function Rectangle
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

function void
render_text_centered(Vector2 pos, Font font, const char* text, int width)
{
	Vector2 measure = MeasureTextEx(font, text, (float)font.baseSize, 0);
	DrawTextEx(font, text, Vector2Add(pos, (Vector2) { width / 2 - measure.x / 2, BYTE_SIZE / 2 - measure.y / 2 }), (float)font.baseSize, 0, FONT_COLOR);
}

function void
render_text_rightalign(Vector2 pos, Font font, const char* text, int width)
{
	Vector2 measure = MeasureTextEx(font, text, (float)font.baseSize, 0);
	DrawTextEx(font, text, Vector2Add(pos, (Vector2) { width - measure.x - 4, BYTE_SIZE / 2 - measure.y / 2 }), (float)font.baseSize, 0, FONT_COLOR);
}

function const char*
text_for_register_lane(AnyValue value, int32_t index, bool hex)
{
	const char* view_format = fmt(hex, value.type);
	switch (value.register_size_bytes * 8)
	{
		case 128: {
			switch (value.type)
			{
				case REGISTER_TYPE_S8:  return TextFormat(view_format, simd_extract_s8_from_128(value.i128, index));
				case REGISTER_TYPE_S16: return TextFormat(view_format, simd_extract_s16_from_128(value.i128, index));
				case REGISTER_TYPE_S32: return TextFormat(view_format, simd_extract_s32_from_128(value.i128, index));
				case REGISTER_TYPE_S64: return TextFormat(view_format, simd_extract_s64_from_128(value.i128, index));
				case REGISTER_TYPE_U8:  return TextFormat(view_format, simd_extract_u8_from_128(value.i128, index));
				case REGISTER_TYPE_U16: return TextFormat(view_format, simd_extract_u16_from_128(value.i128, index));
				case REGISTER_TYPE_U32: return TextFormat(view_format, simd_extract_u32_from_128(value.i128, index));
				case REGISTER_TYPE_U64: return TextFormat(view_format, simd_extract_u64_from_128(value.i128, index));
				case REGISTER_TYPE_F32: return TextFormat("%f", simd_extract_f32_from_128(value.f128, index));
				case REGISTER_TYPE_F64:	return TextFormat("%f", simd_extract_f64_from_128(value.f128d, index));
			}
		} break;
		case 256: {
			switch (value.type)
			{
				case REGISTER_TYPE_S8:  return TextFormat(view_format, simd_extract_s8_from_256(value.i256, index));
				case REGISTER_TYPE_S16: return TextFormat(view_format, simd_extract_s16_from_256(value.i256, index));
				case REGISTER_TYPE_S32: return TextFormat(view_format, simd_extract_s32_from_256(value.i256, index));
				case REGISTER_TYPE_S64: return TextFormat(view_format, simd_extract_s64_from_256(value.i256, index));
				case REGISTER_TYPE_U8:  return TextFormat(view_format, simd_extract_u8_from_256(value.i256, index));
				case REGISTER_TYPE_U16: return TextFormat(view_format, simd_extract_u16_from_256(value.i256, index));
				case REGISTER_TYPE_U32: return TextFormat(view_format, simd_extract_u32_from_256(value.i256, index));
				case REGISTER_TYPE_U64: return TextFormat(view_format, simd_extract_u64_from_256(value.i256, index));
				case REGISTER_TYPE_F32: return TextFormat("%f", simd_extract_f32_from_256(value.f256, index));
				case REGISTER_TYPE_F64: return TextFormat("%f", simd_extract_f64_from_256(value.f256d, index));
			}
		} break;
	}

	return "";
}

#define CONCATENATE(prefix, name, suffix)   prefix ## name ## suffix
#define EXTRACT_FUNCTION(TYPE, SIZE, SIGN)  function TYPE          \
extract_##SIGN##SIZE(AnyValue value, int32_t index)              \
{                                                                \
	switch (value.register_size_bytes * 8)                       \
	{                                                            \
		case 128: return CONCATENATE(simd_extract_, SIGN##SIZE, _from_128)(value.i128, index);  \
		case 256: return CONCATENATE(simd_extract_, SIGN##SIZE, _from_256)(value.i256, index);  \
		default: assert(false);                                  \
	}                                                            \
}

EXTRACT_FUNCTION(int8_t, 8, s)
EXTRACT_FUNCTION(uint8_t, 8, u)
EXTRACT_FUNCTION(int16_t, 16, s)
EXTRACT_FUNCTION(uint16_t, 16, u)
EXTRACT_FUNCTION(int32_t, 32, s)
EXTRACT_FUNCTION(uint32_t, 32, u)
EXTRACT_FUNCTION(int64_t, 64, s)
EXTRACT_FUNCTION(uint64_t, 64, u)

function float
extract_f32(AnyValue value, int32_t index)
{
	switch (value.register_size_bytes * 8)
	{
		case 128: return simd_extract_f32_from_128(value.f128, index);
		case 256: return simd_extract_f32_from_256(value.f256, index);
	}
}

function double
extract_f64(AnyValue value, int32_t index)
{
	switch (value.register_size_bytes * 8)
	{
		case 128: return simd_extract_f64_from_128(value.f128d, index);
		case 256: return simd_extract_f64_from_256(value.f256d, index);
	}
}

function bool
same_value(AnyValue value, int32_t index, AnyValue compared, int32_t compared_index)
{
	if (value.type != compared.type)
		return false;

	switch (value.type)
	{
		case REGISTER_TYPE_S8: return extract_s8(value, index) == extract_s8(compared, compared_index);
		case REGISTER_TYPE_U8: return extract_u8(value, index) == extract_u8(compared, compared_index);
		case REGISTER_TYPE_S16: return extract_s16(value, index) == extract_s16(compared, compared_index);
		case REGISTER_TYPE_U16: return extract_u16(value, index) == extract_u16(compared, compared_index);
		case REGISTER_TYPE_S32: return extract_s32(value, index) == extract_s32(compared, compared_index);
		case REGISTER_TYPE_U32: return extract_u32(value, index) == extract_u32(compared, compared_index);
		case REGISTER_TYPE_S64: return extract_s64(value, index) == extract_s64(compared, compared_index);
		case REGISTER_TYPE_U64: return extract_u64(value, index) == extract_u64(compared, compared_index);
		case REGISTER_TYPE_F32: return extract_f32(value, index) == extract_f32(compared, compared_index);
		case REGISTER_TYPE_F64: return extract_f64(value, index) == extract_f64(compared, compared_index);
	}

	return false;
}

function ValueHovered
render_register(SimdViewer* sv, Vector2 pos, AnyValue any, RenderFlag flag)
{
	Font font = sv->font;
	uint32_t highlight_size = sv->highlight_size;
	Vector2 mouse = GetMousePosition();

	if (flag & SIMD_VIEWER_RENDER_BORDER)
	{
		Vector2 border_pos = (Vector2){ pos.x - BORDER_SIZE, pos.y - BORDER_SIZE };
		Rectangle border_rect = (Rectangle){ .x = pos.x - BORDER_SIZE, .y = pos.y - BORDER_SIZE, .width = (float)(box_width(any.register_size_bytes) + BORDER_SIZE * 2), BYTE_SIZE + BORDER_SIZE * 2 };
		DrawRectangleLinesEx(border_rect, BORDER_SIZE, (Color) { 0x20, 0x20, 0x20, 0xff });
	}

	uint16_t division_size = regtype_to_bytesize(any.type);
	Vector2 render_position = pos;
	for (int i = any.register_size_bytes / division_size - 1; i >= 0; --i)
	{
		bool same = same_value(any, i, sv->hovered.last_value, sv->hovered.last_index);
		if(same)
		{
			printf("Same value: %d %d | %d %d\n", i, sv->hovered.last_index, extract_u8(any, i), extract_u8(sv->hovered.last_value, sv->hovered.last_index));
		}

		Rectangle boxrect = render_box_highlight(render_position, division_size, same);
		render_text_rightalign(render_position, font, text_for_register_lane(any, i, flag & SIMD_VIEWER_RENDER_HEX), box_width(division_size));

		if (CheckCollisionPointRec(mouse, boxrect))
		{
			sv->hovered.frame_index = i;
			sv->hovered.frame_value = any;
		}

		render_position = Vector2Add(render_position, (Vector2) { (BYTE_SIZE + SPACING) * division_size, 0 });
	}

	if (flag & SIMD_VIEWER_RENDER_HIGHLIGHT && highlight_size > 0)
	{
		Vector2 sz_pos = pos;
		Color highlight_color = GOLD;
		highlight_color.a = 60;

		for (int i = 0; i < any.register_size_bytes / highlight_size; ++i)
		{
			if (i % 2 == 0)
				render_box_colored(sz_pos, highlight_size, highlight_color);
			sz_pos = Vector2Add(sz_pos, (Vector2) { (BYTE_SIZE + SPACING)* highlight_size, 0 });
		}
	}
}

function void
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
	simd_viewer->font = LoadFontEx("res/LiberationMono-Regular.ttf", 20, 0, 1024);
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

	simd_viewer->hovered.last_index = simd_viewer->hovered.frame_index;
	simd_viewer->hovered.last_value = simd_viewer->hovered.frame_value;
	simd_viewer->hovered.frame_index = -1;
	simd_viewer->hovered.frame_value = (AnyValue){ 0 };
}

function AnyValue
make_anyvalue_from_i256(__m256i value, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_S8 && regtype <= REGISTER_TYPE_U64 && "Register type must be integer type");
	AnyValue result = {
		.type = regtype,
		.register_size_bytes = sizeof(__m256i),
		.i256 = value,
	};
	return result;
}

function AnyValue
make_anyvalue_from_f256(__m256 value, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	AnyValue result = {
		.type = regtype,
		.register_size_bytes = sizeof(__m256),
		.f256 = value,
	};
	return result;
}

function AnyValue
make_anyvalue_from_f256d(__m256d value, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	AnyValue result = {
		.type = regtype,
		.register_size_bytes = sizeof(__m256d),
		.f256d = value,
	};
	return result;
}

function AnyValue
make_anyvalue_from_i128(__m128i value, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_S8 && regtype <= REGISTER_TYPE_U64 && "Register type must be integer type");
	AnyValue result = {
		.type = regtype,
		.register_size_bytes = sizeof(__m128i),
		.i128 = value,
	};
	return result;
}

function AnyValue
make_anyvalue_from_f128(__m128 value, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	AnyValue result = {
		.type = regtype,
		.register_size_bytes = sizeof(__m128),
		.f128 = value,
	};
	return result;
}

// Push
void
simd_viewer_push(SimdViewer* simd_viewer, __m256i reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_S8 && regtype <= REGISTER_TYPE_U64 && "Register type must be integer type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_i256(reg, regtype), flags);
}

void 
simd_viewer_push_bold(SimdViewer* simd_viewer, __m256i reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_S8 && regtype <= REGISTER_TYPE_U64 && "Register type must be integer type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags | SIMD_VIEWER_RENDER_BORDER;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_i256(reg, regtype), flags);
}

void
simd_viewer_pushf(SimdViewer* simd_viewer, __m256 reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_f256(reg, regtype), flags);
}

void
simd_viewer_pushd(SimdViewer* simd_viewer, __m256d reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_f256d(reg, regtype), flags);
}

void
simd_viewer_pushd_bold(SimdViewer* simd_viewer, __m256d reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags | SIMD_VIEWER_RENDER_BORDER;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_f256d(reg, regtype), flags);
}

void
simd_viewer_pushf_bold(SimdViewer* simd_viewer, __m256 reg, FRegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags | SIMD_VIEWER_RENDER_BORDER;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_f256(reg, regtype), flags);
}

void 
simd_viewer_push128(SimdViewer* simd_viewer, __m128i reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_S8 && regtype <= REGISTER_TYPE_U64 && "Register type must be integer type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_i128(reg, regtype), flags);
}

void 
simd_viewer_push128f(SimdViewer* simd_viewer, __m128 reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_f128(reg, regtype), flags);
}

void 
simd_viewer_push128_bold(SimdViewer* simd_viewer, __m128i reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_S8 && regtype <= REGISTER_TYPE_U64 && "Register type must be integer type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags | SIMD_VIEWER_RENDER_BORDER;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_i128(reg, regtype), flags);
}

void 
simd_viewer_push128f_bold(SimdViewer* simd_viewer, __m128 reg, RegisterType regtype)
{
	assert(regtype >= REGISTER_TYPE_F32 && regtype <= REGISTER_TYPE_F64 && "Register type must be float type");
	uint32_t index = simd_viewer->stack_index++;
	uint32_t flags = simd_viewer->default_render_flags | simd_viewer->pushed_flags | SIMD_VIEWER_RENDER_BORDER;
	render_register(simd_viewer, line_position(index), make_anyvalue_from_f128(reg, regtype), flags);
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