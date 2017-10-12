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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	RECT rect1[4] = { 100, 100, 140, 140 };
	RECT rect2[4] = { 60, 140, 100, 180 };
	RECT rect3[4] = { 140, 140, 180, 180 };
	RECT rect4[4] = { 100, 180, 140, 220 };
	PAINTSTRUCT ps;

	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Rectangle(hdc, 100, 100, 140, 140);
		Rectangle(hdc, 60, 140, 100, 180);
		Rectangle(hdc, 140, 140, 180, 180);
		Rectangle(hdc, 100, 180, 140, 220);

		TextOut(hdc, 110, 110, "��", 2);
		TextOut(hdc, 60, 150, "����", 4);
		TextOut(hdc, 140, 150, "����", 4);
		TextOut(hdc, 100, 190, "�Ʒ�", 4);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		hdc = GetDC(hWnd);
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		if (wParam == VK_UP){
			FillRect(hdc, rect1, hBrush);
			TextOut(hdc, 110, 110, "��", 2);
		}
		else if (wParam == VK_LEFT){
			FillRect(hdc, rect2, hBrush);
			TextOut(hdc, 60, 150, "����", 4);
		}
		else if (wParam == VK_RIGHT){
			FillRect(hdc, rect3, hBrush);
			TextOut(hdc, 140, 150, "����", 4);
		}
		else if (wParam == VK_DOWN){
			FillRect(hdc, rect4, hBrush);
			TextOut(hdc, 100, 190, "�Ʒ�", 4);
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_KEYUP:
		hdc = GetDC(hWnd);
		if (wParam == VK_UP){
			Rectangle(hdc, 100, 100, 140, 140);
			TextOut(hdc, 110, 110, "��", 2);
		}
		else if (wParam == VK_LEFT){
			Rectangle(hdc, 60, 140, 100, 180);
			TextOut(hdc, 60, 150, "����", 4);
		}
		else if (wParam == VK_RIGHT){
			Rectangle(hdc, 140, 140, 180, 180);
			TextOut(hdc, 140, 150, "����", 4);
		}
		else if (wParam == VK_DOWN){
			Rectangle(hdc, 100, 180, 140, 220);
			TextOut(hdc, 100, 190, "�Ʒ�", 4);
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
/*
2 -11
Ű���� ��ɿ� ���� �׸��� �׸��� �̵��ϴ� ���α׷�
(2- 10�� ����߰�)
����Ű���� : �׷��� ������� ������ ���õȴ�.
1. ù���� ���� 2. �ι�° ���� 3 ����°..
���õ� ������ �ѷ��� ǥ�õȴ�.

ȭ��ǥ Ű���� : ���õ� ������ ȭ��ǥ�� ���� �� / �Ʒ� / �� / �� �� �̵��Ѵ�.
*/