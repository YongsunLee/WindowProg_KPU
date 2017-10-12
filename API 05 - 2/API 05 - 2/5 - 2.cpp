#include <Windows.h>
#include "resource.h"

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 5 - 2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	HACCEL hAcc;
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

	hAcc = LoadAccelerators(hinstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&Message, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAcc, &Message)){
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static HBITMAP hBitmap;
	BITMAP bmp;
	RECT rect;
	static BOOL Drag;
	static BOOL Copy;
	static int B_sx, B_sy, B_ex, B_ey;
	static int S_mx, S_my, E_mx, E_my;
	static int window_ex, window_ey;
	static int bmp_x, bmp_y;

	switch (uMsg) {
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_OBJECT_COPY:
			Copy = TRUE;
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		
		// 윈도우 크기
		GetClientRect(hWnd, &rect);
		window_ex = rect.right;
		window_ey = rect.bottom;
		
		//비트맵 크기
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);
		bmp_x = bmp.bmWidth;
		bmp_y = bmp.bmHeight;
		SelectObject(memdc, hBitmap);

		StretchBlt(hdc, 0, 0, window_ex, window_ey, memdc, 0, 0, bmp_x, bmp_y, SRCCOPY);

		if (Copy == TRUE){
			BitBlt(hdc, S_mx, S_my, E_mx, E_my, memdc, S_mx, S_my, SRCCOPY);
		}

		Rectangle(hdc, S_mx, S_my, B_ex, B_ey);

		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		Drag = TRUE;
		B_ex = S_mx = LOWORD(lParam);
		B_ey = S_my = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		Drag = FALSE;
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);

		if ( Drag ){
			SetROP2(hdc, R2_NOP);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

			E_mx = LOWORD(lParam);
			E_my = HIWORD(lParam);

			Rectangle(hdc, S_mx, S_my, B_ex, B_ey);
			
			Rectangle(hdc, S_mx, S_my, E_mx, E_my);

			B_ex = E_mx; B_ey = E_my;
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}