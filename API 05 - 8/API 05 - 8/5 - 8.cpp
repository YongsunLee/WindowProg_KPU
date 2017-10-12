#include <Windows.h>
#include "resource.h"

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
}SHAPE;

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 5 - 8";

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

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
	static HDC hdc, mem1dc, mem2dc;
	static HBITMAP hBit_all, hBit_BG, oldBit_all, oldBit_BG;
	PAINTSTRUCT ps;
	RECT rect;
	static BITMAP BG;
	static int Win_x, Win_y, Start_x, Start_y, BG_x, BG_y;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		hBit_BG = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(hBit_BG, sizeof(BITMAP), (LPSTR)&BG);
		GetClientRect(hWnd, &rect);
		Win_x = rect.right;
		Win_x = rect.bottom;
		Start_x = 0;
		Start_y = 0;
		BG_x = BG.bmWidth;
		BG_y = BG.bmHeight;
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_GAME_START:
			break;
		case ID_GAME_ANSWER:
			break;
		case ID_GAME_EXIT:
			break;
		case ID_MAP_3X3:
			break;
		case ID_MAP_4X4:
			break;
		case ID_MAP_5X5:
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		
		if (hBit_all == NULL) hBit_all = CreateCompatibleBitmap(hdc, Win_x, Win_y);

		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		oldBit_all = (HBITMAP)SelectObject(mem1dc, hBit_all);

		oldBit_BG = (HBITMAP)SelectObject(mem2dc, hBit_BG);
		//BitBlt(mem1dc, Start_x, Start_y, Win_x, Win_y, mem2dc, 0, 0, SRCCOPY);
		//StretchBlt(mem1dc, Start_x, Start_y, Win_x, Win_y, mem2dc, 0, 0, BG_x, BG_y, SRCCOPY);

		BitBlt(mem1dc, 0, 0, Win_x, Win_y, mem2dc, 0, 0, SRCCOPY);

		SelectObject(mem2dc, oldBit_BG);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBit_all);
		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		oldBit_all = (HBITMAP)SelectObject(mem1dc, hBit_all);

		BitBlt(hdc, 0, 0, Win_x, Win_y, mem1dc, 0, 0, SRCCOPY);

		SelectObject(mem1dc, oldBit_all);
		DeleteDC(mem2dc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}