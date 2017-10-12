#include <Windows.h>

extern "C" __declspec(dllexport)POINT FindCenter(RECT rect);

extern "C" __declspec(dllexport)int Rect_area(RECT rect);

extern "C" __declspec(dllexport)RECT Size_Up(RECT rect);

extern "C" __declspec(dllexport)RECT Size_Dwon(RECT rect);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect1, rect2;
	static BOOL draw;
	POINT center1, center2;
	static char str1[30], str2[30], str3[30], str4[30], str5[30], str6[30];
	int area1, area2;
	int size = 10;

	switch (uMsg) {
	case WM_CREATE:
		rect1.left = 50;
		rect1.top = 50;
		rect1.right = 100;
		rect1.bottom = 100;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Rectangle(hdc, rect1.left, rect1.top, rect1.right, rect1.bottom);
		
		center1 = FindCenter(rect1);
		area1 = Rect_area(rect1);

		wsprintf(str1, "%d", center1.x);
		TextOut(hdc, rect1.right, rect1.top + 20, str1, strlen(str1));

		wsprintf(str2, "%d", center1.y);
		TextOut(hdc, rect1.right + 30, rect1.top + 20, str2, strlen(str2));

		wsprintf(str3, "%d", area1);
		TextOut(hdc, rect1.right, rect1.top + 35, str3, strlen(str3));

		size = (center1.x - rect1.right) / 2;

		if (draw){
			center2 = FindCenter(rect2);
			area2 = Rect_area(rect2);

			Rectangle(hdc, center2.x - size, center2.y - size, center2.x + size, center2.y + size);

			wsprintf(str4, "%d", center2.x);
			TextOut(hdc, rect2.right, rect2.top + 20, str4, strlen(str4));

			wsprintf(str5, "%d", center2.y);
			TextOut(hdc, rect2.right + 30, rect2.top + 20, str5, strlen(str5));

			wsprintf(str6, "%d", area2);
			TextOut(hdc, rect2.right, rect2.top + 35, str6, strlen(str6));

		}

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		rect1 = Size_Up(rect1);
		rect2 = Size_Dwon(rect1);
		draw = TRUE;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}