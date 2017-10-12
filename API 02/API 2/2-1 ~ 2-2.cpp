//		14.3.18
//	

#include <Windows.h>

HINSTANCE g_hinst;
LPCTSTR lpszClass = "Window program 1 - 2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hinstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass,
		lpszClass,
		WS_OVERLAPPEDWINDOW,
		100,
		50,
		800,
		600,
		NULL,
		(HMENU)NULL,
		hinstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

//CallBack 함수 = OS가 사용하는 함수
// 기존의 함수는 프로그램이 사용
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	COLORREF color;
COLORREF: DWORD;
	RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;
	char LeftUp[] = "LeftUp";
	char LeftDown[] = "LeftDown";
	char RightUp[] = "RightUp";
	char RightDown[] = "RightDown";
	char Center[] = "Center";
	int i, j;

	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 0, 0, LeftUp, strlen(LeftUp));
		TextOut(hdc, 0, 540, LeftDown, strlen(LeftDown));
		TextOut(hdc, 730, 0, RightUp, strlen(RightUp));
		TextOut(hdc, 710, 540, RightDown, strlen(RightDown));
		TextOut(hdc, 350, 250, Center, strlen(Center));

		rect.left = 50;
		rect.top = 40;
		rect.right = 200;
		rect.bottom = 120;
		SetTextColor(hdc, RGB(155, 0, 0));
		SetBkColor(hdc, RGB(100, 255, 255));
		DrawText(hdc, LeftUp, strlen(LeftUp), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect.left = 1100;
		rect.top = 40;
		rect.right = 200;
		rect.bottom = 120;
		SetTextColor(hdc, RGB(155, 0, 0));
		SetBkColor(hdc, RGB(100, 255, 255));
		DrawText(hdc, RightUp, strlen(RightUp), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect.left = 50;
		rect.top = 800;
		rect.right = 200;
		rect.bottom = 120;
		SetTextColor(hdc, RGB(0, 100, 0));
		SetBkColor(hdc, RGB(255, 155, 255));
		DrawText(hdc, LeftDown, strlen(LeftDown), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect.left = 1100;
		rect.top = 800;
		rect.right = 200;
		rect.bottom = 120;
		SetTextColor(hdc, RGB(0, 100, 155));
		SetBkColor(hdc, RGB(255, 155, 100));
		DrawText(hdc, RightDown, strlen(RightDown), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}