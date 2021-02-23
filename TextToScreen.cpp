#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
#include <string>
#include <vector>
using std::vector; using std::string;

static int countDraws = 0;
static vector<WCHAR> holdChar;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	// Register class
	const wchar_t CLASS_NAME[] = L"TextDisplay Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create Window

	HWND hwnd = CreateWindowEx(
		0,						   // Optional Window Styles
		CLASS_NAME,				   // Window Class
		L"Type to put your text on screen!",  //Window Name
		WS_OVERLAPPEDWINDOW,	   // Window Style

		// Size and Position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,		// Parent Window
		NULL,		// Menu
		hInstance,	// Instance Handle
		NULL		// Additional Application Data
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run Message Loop:
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_CHAR:
	{
		if (wParam == VK_BACK) {
			if (!holdChar.empty()) {
				holdChar.pop_back();
			}
		}
		else {
			holdChar.push_back(wParam);
			WCHAR str[] = { wParam, 0 };
			OutputDebugString(str);
			OutputDebugString(L"\n");
		}

		
		RECT rect;

		GetClientRect(hwnd, &rect);
		SetRect(&rect, 0, 0, 300, 300);

		InvalidateRect(hwnd, NULL, TRUE);


		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT :
	{

		std::wstring yep = L"About to paint! : ";
		yep += std::to_wstring(countDraws) + L"\n";
		vector<wchar_t> buf(yep.begin(), yep.end());
		buf.push_back(0);
		OutputDebugString(&buf[0]);

		countDraws++;

		RECT rect;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		SetTextColor(hdc, RGB(24, 74, 66));
		SetBkMode(hdc, TRANSPARENT);
		
		holdChar.push_back(0);

		OutputDebugString(&holdChar[0]);

		if (FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1)) == 0) {
			OutputDebugString(L"Failed to Draw Rect\n");
		}

		if (DrawText(hdc, &holdChar[0], holdChar.size() , &rect, DT_TOP) == 0) {
			OutputDebugString(L"Failed to Draw Text\n");
		}

		holdChar.pop_back();

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}