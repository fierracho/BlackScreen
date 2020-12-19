#include <windows.h>
#include <vector> 

using namespace std;

BOOL CALLBACK EnumDispProc(HMONITOR hMon, HDC dcMon, RECT* pRcMon, LPARAM lParam)
{
	vector<RECT>* pArg = reinterpret_cast<vector<RECT>*>(lParam);

	pArg->push_back(*pRcMon);

	return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_LBUTTONUP:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void createWindow(HWND windowHandle, HINSTANCE hInstance, int nCmdShow, RECT rect) {
	if (rect.right <= 0) rect.right = (-rect.left) * 2;
	if (rect.bottom <= 0) rect.bottom = (-rect.top) * 2;

	windowHandle = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_TOPMOST, "mainWindow", "Black Screen", WS_POPUP, rect.left, rect.top, rect.left + rect.right, rect.top + rect.bottom, NULL, NULL, hInstance, 0);
	ShowWindow(windowHandle, nCmdShow);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow) {
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(104));
	windowClass.lpszClassName = "mainWindow";

	RegisterClassEx(&windowClass);
	HWND windowHandle = 0;

	vector<RECT> screen;
	EnumDisplayMonitors(0, 0, EnumDispProc, reinterpret_cast<LPARAM>(&screen));

	POINT pnt;
	GetCursorPos(&pnt);

	LPWSTR* szArglist;
	int nArgs;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	if (nArgs == 2 && wcscmp(L"-all", szArglist[1]) == 0) {
		for (int i = 0; i < screen.size(); i++) {
			createWindow(windowHandle, hInstance, nCmdShow, screen[i]); //all
		}
	}
	else {
		for (int i = 0; i < screen.size(); i++) {
			if ((pnt.x > screen[i].left) & (pnt.x < screen[i].right) & (pnt.y > screen[i].top) & (pnt.y < screen[i].bottom)) {
				createWindow(windowHandle, hInstance, nCmdShow, screen[i]); //this
				break;
			}
		}
	}

	LocalFree(szArglist);

	ShowCursor(false);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return 0;
}