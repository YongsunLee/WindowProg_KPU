#include <Windows.h>
#include <stdio.h>

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
	RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;
	static SIZE size;
	static char str[100];
	static int x1, y1, x2, y2;
	static int count, figure;
	static char *temp;
	char zero = '0';
	int i, j;

	switch (uMsg) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		count = 0;
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetTextExtentPoint(hdc, str, strlen(str), &size);
		Rectangle(hdc, 0, 530, 200, 550);
		TextOut(hdc, 0, 530, str, strlen(str));

		switch (figure){
		case 1:
			MoveToEx(hdc, x1, y1, NULL);
			LineTo(hdc, x2, y2);
			break;
		case 2:
			Ellipse(hdc, x1, y1, x2, y2);
			break;
		case 3:
			Rectangle(hdc, x1, y1, x2, y2);
			break;
		}
		SetCaretPos(size.cx, 530);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK){
			count--;
			if (count < 0)
				count++;
		}
		else {
			str[count++] = wParam;
			if (count > 99){
				count--;
			}
		}
		str[count] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN){
			sscanf(str, "%d %d %d %d %d", &figure, &x1, &y1, &x2, &y2);
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}