#include <Windows.h>
#include "resource.h"

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 5 - 4";

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
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	RECT rect_cat, rect_mouse;
	BITMAP bmp_cat, bmp_mouse;
	static HBITMAP hBitmap_cat, hBitmap_mouse;
	static int x1, y1, x2, y2;
	static int bmp_x1, bmp_y1, bmp_x2, bmp_y2;
	static BOOL Selection;
	static int Move_x, Move_y;

	switch (uMsg) {
	case WM_CREATE:
		hBitmap_cat = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBitmap_mouse = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		GetObject(hBitmap_cat, sizeof(BITMAP), (LPSTR)&bmp_cat);
		GetObject(hBitmap_mouse, sizeof(BITMAP), (LPSTR)&bmp_mouse);
		SetTimer(hWnd, 1, 100, NULL);
		x1 = 100;
		y1 = 100;
		x2 = 0;
		y2 = 0;
		bmp_x1 = bmp_cat.bmWidth;
		bmp_y1 = bmp_cat.bmHeight;
		bmp_x2 = bmp_mouse.bmWidth;
		bmp_y2 = bmp_mouse.bmHeight;
		Selection = FALSE;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (Selection == TRUE){
				if (x1 < Move_x){
					x1++;
				}
				else if (x1 > Move_x){
					x1--;
				}
				if (y1 < Move_y){
					y1++;
				}
				else if (y1 > Move_y){
					y1--;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap_cat);
		rect_cat.left = x1 - 10;
		rect_cat.top = y1 - 10;
		
		StretchBlt(hdc, rect_cat.left, rect_cat.top, bmp_x1, bmp_y1,
				memdc, 0, 0, bmp_x1, bmp_y1, SRCCOPY);
		
		if (Selection == TRUE){
			SelectObject(memdc, hBitmap_mouse);
			rect_mouse.left = x2 - 10;
			rect_mouse.top = y2 - 10;
			
			StretchBlt(hdc, rect_mouse.left, rect_mouse.top, bmp_y2, bmp_x2,
				memdc, 0,0, bmp_x2, bmp_y2, SRCCOPY);
		}
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		break;
	case WM_LBUTTONDOWN:
		Move_x = LOWORD(lParam);
		Move_y = HIWORD(lParam);
		x2 = Move_x;
		y2 = Move_y;
		Selection = TRUE;
		break;
	case WM_MOUSEMOVE:
		Move_x = LOWORD(lParam);
		Move_y = HIWORD(lParam);
		if (Selection){
			x2 = Move_x;
			y2 = Move_y;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}