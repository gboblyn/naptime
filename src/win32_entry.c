#include <windows.h>
#include <wingdi.h>
#include <xinput.h>

#include "win32_vulkan_loader.h"

#define APP_NAME L"gfg"
#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600

void resizeDIBSection(int width, int height) {
	HDC device = CreateCompatibleDC(NULL);

	BITMAPINFO bitmapInfo = {
		.bmiHeader = {
			.biSize = sizeof(bitmapInfo.bmiHeader);
			.biWidth = width;
			.biHeight = height;
			.biPlanes = 1;
			.biBitCout = 32;
			.biCompression = BI_RGB;
			.biSizeImage = 0;
			.biXPelsPerMeter = 0;
			.biYPelsPerMeter = 0;
			.biClrUsed = 0;
			.biClrImportant = 0;
		},
		REGQUAD = NULL
	};

	void **bitmap;
	HBITMAP dibHandle = CreateDIBSection(device, bitmapInfo, DIB_RGB_COLORS, bitmap, NULL, 0);
}

LRESULT windowCallback(HWND window, UINT message,
											 WPARAM wParam, LPARAM lParam) {
	HDC hdc;

	switch(message) {
		case WM_CLOSE:
		case WM_DESTROY:
			DestroyWindow(window);
			break;
		case WM_SIZE:
			RECT drawableArea;
			GetClientRect(window, &drawableArea);
			int width = drawableArea.right - drawableArea.left;
			int height = drawableArea.bottom - drawableArea.top;
			resizeDIBSection(width, height);
			break;
		case WM_ACTIVATEAPP:
			OutputDebugStringW(L"WM_ACTIVATEAPP\n");
			break;
		case WM_PAINT:
			PAINTSTRUCT ps;
			hdc = BeginPaint(window, &ps);
			int x = ps.rcPaint.left;
			int y = ps.rcPaint.right;
			int width = ps.rcPaint.right - ps.rcPaint.left;
			int height = ps.rcPaint.bottom - ps.rcPaint.top;

			// StretchDIBits(hdc, x, y, width, height, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT,
			//     bits, bitmap, DIB_RGB_COLORS, );
			TextOutW(hdc, 0, 0, L"Hello, Windows!", 15);
			EndPaint(window, &ps);
			return 0L;
			break;
		default:
			break;
	}

	return DefWindowProcW(window, message, wParam, lParam);
}

void alertMessage(LPCWSTR msg) {
		MessageBoxW(NULL, msg, APP_NAME, MB_ICONWARNING|MB_OK);
}

int CALLBACK wWinMain(HINSTANCE instance, HINSTANCE prev,
											PWSTR cmdLine, int cmdShow) {
	int error = loadVulkan();

	if (error == 1) {
		alertMessage(L"Failed to load vulkan-1.dll");
	} else if (error == 2) {
		alertMessage(L"Failed to create vulkan instance");
	} else {
		alertMessage(L"Successfully initialized vulkan");
	}

	WNDCLASSW wc = {
		.lpfnWndProc = (WNDPROC)windowCallback,
		.hInstance = instance,
		.lpszClassName = APP_NAME
	};

	RegisterClassW(&wc);

	HWND window = CreateWindowExW(0, wc.lpszClassName, APP_NAME,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0, instance, NULL);

	if (window == NULL) {
		// TODO: do something with this error
		DWORD error = GetLastError();
		alertMessage(L"Failed to open win32 window.");
		return 0;
	}

	ShowWindow(window, cmdShow);
	UpdateWindow(window);

	int ret;
	MSG message;
	while((ret = GetMessage(&message, window, 0, 0)) != 0) {
		if (ret == -1) {
			DWORD error = GetLastError();
			return 0;
		} else {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	for(DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
		XINPUT_STATE controllerState;
		if (XInputGetState(i, &controllerState) == ERROR_SUCCESS) {
			XINPUT_GAMEPAD controller = controllerState.Gamepad;

			// bool dpadUp = controller.wButtons & XINPUT_GAMEPAD_DPAD_UP;
			// bool dpadDown = controller.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			// bool dpadLeft = controller.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			// bool dpadRight = controller.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
			// bool start = controller.wButtons & XINPUT_GAMEPAD_START;
			// bool back = controller.wButtons & XINPUT_GAMEPAD_BACK;
			// bool a = controller.wButtons & XINPUT_GAMEPAD_A;
			// bool b = controller.wButtons & XINPUT_GAMEPAD_B;
			// bool x = controller.wButtons & XINPUT_GAMEPAD_X;
			// bool y = controller.wButtons & XINPUT_GAMEPAD_Y;

			if (controller.wButtons & XINPUT_GAMEPAD_BACK) {
				DestroyWindow(window);
				return 0;
			}
		}
	}

	return 0;
}
