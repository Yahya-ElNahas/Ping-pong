#include <windows.h>
#include "utils.cpp"

struct RenderState {
	void* ptr;
	int width;
	int height;
	BITMAPINFO bitmapinfo;
};

global_variable bool running = true;
global_variable RenderState renderState;

#include "platform.cpp"
#include "renderer.cpp"
#include "game_methods.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (uMsg) {
		case WM_CLOSE :
		case WM_DESTROY: 
			running = false;
		break;
		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			renderState.width = rect.right - rect.left;
			renderState.height = rect.bottom - rect.top;
			int bufferSize = renderState.width * renderState.height * sizeof(u32);
			if (renderState.ptr)
				VirtualFree(renderState.ptr, 0, MEM_RELEASE);
			renderState.ptr = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			renderState.bitmapinfo.bmiHeader.biSize = sizeof(renderState.bitmapinfo.bmiHeader);
			renderState.bitmapinfo.bmiHeader.biWidth = renderState.width;
			renderState.bitmapinfo.bmiHeader.biHeight = renderState.height;
			renderState.bitmapinfo.bmiHeader.biPlanes = 1;	
			renderState.bitmapinfo.bmiHeader.biBitCount = 32;
			renderState.bitmapinfo.bmiHeader.biCompression = BI_RGB;
		}
		break;
		default: 
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = L"Game Window Class";
	windowClass.lpfnWndProc = window_callback;

	RegisterClass(&windowClass);

	HWND window = CreateWindow(windowClass.lpszClassName, L"The Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 
		CW_USEDEFAULT, 1200, 700, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	Input input = {};

	float deltaTime = 0.016666f;
	LARGE_INTEGER frameBegin;
	QueryPerformanceCounter(&frameBegin);

	float performanceFreq; {
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFreq = (float)perf.QuadPart;
	}

	while (running) {
		MSG msg;
		for (int i = 0; i < BTN_COUNT; i++) {
			input.buttons[i].isChanged = false;
		}
		while (PeekMessage(&msg, window, 0, 0, PM_REMOVE)) {
			switch (msg.message) {
				case WM_KEYUP:
				case WM_KEYDOWN: {
					u32 vk_code = (u32)msg.wParam;
					bool isPressed = (msg.lParam & (1 << 31)) == 0;

#define processButton(b, vk)\
case vk: {\
input.buttons[b].isChanged = isPressed != input.buttons[b].isPressed;\
input.buttons[b].isPressed = isPressed;\
}break;

					switch (vk_code) {
						processButton(BTN_UP, VK_UP);
						processButton(BTN_DOWN, VK_DOWN);
						processButton(BTN_W, 'W');
						processButton(BTN_S, 'S');
					}
				}break;
				default: {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

		updateScreen(&input, deltaTime);

		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, 
			renderState.ptr, &renderState.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frameEnd;
		QueryPerformanceCounter(&frameEnd);
		deltaTime = ((float)frameEnd.QuadPart - (float)frameBegin.QuadPart) / performanceFreq;
		frameBegin = frameEnd;
	}
}