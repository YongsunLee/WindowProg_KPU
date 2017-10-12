#include <Windows.h>
#include "resource.h"

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 5 - 5";

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
	static HDC hdc, mem1dc, mem2dc;
	PAINTSTRUCT ps;
	static HBITMAP hBit1, hBitmap_BG, hBitmap_Sp[4], oldBit1, oldBit2, oldBit3[4];
	BITMAP bmp_BG, bmp_Sp;
	RECT rect;
	static int BG_x, BG_y, Sp_x, Sp_y, Win_x, Win_y;
	static int Sp_left, Sp_top, Sp_right, Sp_bottom;
	static int jump = 10;
	static int count = 0;
	POINT PtSrc;
	static int i = 0;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 70, NULL);
		hBitmap_BG = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBitmap_Sp[0] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBitmap_Sp[1] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3));
		hBitmap_Sp[2] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP4));
		hBitmap_Sp[3] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP5));

		GetObject(hBitmap_BG, sizeof(BITMAP), (LPSTR)&bmp_BG);
		GetObject(hBitmap_Sp[0], sizeof(BITMAP), (LPSTR)&bmp_Sp);
		GetClientRect(hWnd, &rect);
		Win_x = rect.right;
		Win_y = rect.bottom;
		BG_x = bmp_BG.bmWidth;
		BG_y = bmp_BG.bmHeight;
		Sp_x = bmp_Sp.bmWidth;
		Sp_y = bmp_Sp.bmHeight;
		Sp_left = 100;
		Sp_top = 100;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			Sp_top -= jump;
			if ( jump >= -10 ){
				jump--;
			}
			else if ( jump <= -10){
				jump++;
			}
			count++;
			if ( count == 21 ){
				KillTimer(hWnd, 1);
				count = 0; jump = 10;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			break;
		}
		
		hdc = GetDC(hWnd);
		if (hBit1 == NULL) hBit1 = CreateCompatibleBitmap(hdc, Win_x, Win_y);
		
		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);
		
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBitmap_BG);
		
		BitBlt(mem1dc, 0, 0, Win_x, Win_y, mem2dc, 0, 0, SRCCOPY);
		
		oldBit3[i] = (HBITMAP)SelectObject(mem2dc, hBitmap_Sp[i]);
		TransparentBlt(mem1dc, Sp_left, Sp_top, Sp_x, Sp_y, mem2dc, 0,0, Sp_x, Sp_y, RGB(0,0,0));
		
		i++;
		
		if ( i == 4){
			i = 0;
		}
		SelectObject(mem2dc, oldBit2);
		DeleteDC(mem2dc);
		SelectObject(mem2dc, oldBit3);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		mem1dc = CreateCompatibleDC(hdc);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

		BitBlt(hdc, 0,0, Win_x, Win_y, mem1dc, 0,0, SRCCOPY);

		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem2dc);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if ( wParam == VK_UP){
			Sp_top-=5;
		}
		else if ( wParam == VK_DOWN ){
			Sp_top+=5;
		}
		else if ( wParam == VK_LEFT ){
			Sp_left-=5;
		}
		else if ( wParam == VK_RIGHT ){
			Sp_left+=5;
		}
		break;
	case WM_CHAR:
		if (wParam == 'j' || wParam == 'J'){
			SetTimer(hWnd, 1, 100, NULL);
		}

		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0);
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}