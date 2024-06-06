internal void clear_screen(u32 color)
{
	u32* pixel = (u32*)renderState.ptr;
	for (int i = 0; i < renderState.height; i++)
		for (int j = 0; j < renderState.width; j++)
			*pixel++ = color;
}

internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, renderState.width);
	x1 = clamp(0, x1, renderState.width);
	y0 = clamp(0, y0, renderState.height);
	y1 = clamp(0, y1, renderState.height);
	for (int i = y0; i < y1; i++) {
		u32* pixel = (u32*)renderState.ptr + x0 + i * renderState.width;
		for (int j = x0; j < x1; j++)
			*pixel++ = color;
	}
}

global_variable float render_scale = 0.01f;

internal void draw_rect(float x, float y, float halfX, float halfY, u32 color)
{
	x *= renderState.height * render_scale;
	y *= renderState.height * render_scale;
	halfX *= renderState.height * render_scale;
	halfY *= renderState.height * render_scale;
	x += renderState.width / 2.f;
	y += renderState.height / 2.f;
	int x0 = x - halfX;
	int x1 = x + halfX;
	int y0 = y - halfY;
	int y1 = y + halfY;
	draw_rect_in_pixels(x0, y0, x1, y1, color);
}