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
LPCTSTR lpszClass = "연습문제 5 - 6";

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
	static HBITMAP hBit1, hBit_BG_1, hBit_BG_2, hBit_Sp_1[4], hBit_Sp_2[4], hBit_Sp_3[4], hBit_Sp_4[4],
						oldBit1, oldBit_BG_1, oldBit_BG_2, oldBit_Sp_1[4], oldBit_Sp_2[4], oldBit_Sp_3[4], oldBit_Sp_4[4];
	RECT rect;
	static int Win_x, Win_y, BG1_x, BG1_y, BG2_x, BG2_y;
	static int fbg_x, fbg_y, bbg_x, bbg_y;
	static int i = 0;
	static BITMAP Sp_1, Sp_2;
	static int Sp_1_x, Sp_1_y, Sp_2_x, Sp_2_y;
	static int Sp1_pos_x, Sp2_pos_x, Sp1_pos_y, Sp2_pos_y;
	static int jump = 10, count = 0, Check_j = 0;
	static int Sp1_trans = 0, Sp2_trans = 0;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		hBit_BG_1 = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit_BG_2 = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBit_Sp_1[0] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3));
		hBit_Sp_1[1] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP4));
		hBit_Sp_1[2] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP5));
		hBit_Sp_1[3] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP6));
		hBit_Sp_2[0] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP7));
		hBit_Sp_2[1] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP8));
		hBit_Sp_2[2] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP9));
		hBit_Sp_2[3] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP10));

		hBit_Sp_3[0] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP11));
		hBit_Sp_3[1] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP12));
		hBit_Sp_3[2] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP13));
		hBit_Sp_3[3] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP14));

		hBit_Sp_4[0] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP15));
		hBit_Sp_4[1] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP16));
		hBit_Sp_4[2] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP17));
		hBit_Sp_4[3] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP18));

		GetObject(hBit_Sp_1[0], sizeof(BITMAP), (LPSTR)&Sp_1);
		GetObject(hBit_Sp_2[0], sizeof(BITMAP), (LPSTR)&Sp_2);
		GetClientRect(hWnd, &rect);
		Win_x = rect.right;
		Win_y = rect.bottom;
		Sp_1_x = Sp_1.bmWidth;
		Sp_1_y = Sp_1.bmHeight;
		Sp_2_x = Sp_2.bmWidth;
		Sp_2_y = Sp_2.bmHeight;
		fbg_x = 0;
		fbg_y = 0;
		bbg_x = Win_x;
		bbg_y = Win_y;
		Sp1_pos_x = 100;
		Sp2_pos_x = 600;
		Sp1_pos_y = 400;
		Sp2_pos_y = 400;
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);

		if (hBit1 == NULL) hBit1 = CreateCompatibleBitmap(hdc, Win_x, Win_y);

		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		
		oldBit_BG_1 = (HBITMAP)SelectObject(mem2dc, hBit_BG_1);
		BitBlt(mem1dc, fbg_x, fbg_y, Win_x, Win_y, mem2dc, 0, 0, SRCCOPY);

		oldBit_BG_2 = (HBITMAP)SelectObject(mem2dc, hBit_BG_2);
		BitBlt(mem1dc, bbg_x, 0, (bbg_x + Win_x), (bbg_y + Win_y), mem2dc, 0, 0, SRCCOPY);

		fbg_x -= 3;
		bbg_x -= 3;

		if (fbg_x <= -Win_x){
			fbg_x = Win_x;
		}
		if (bbg_x <= -Win_x){
			bbg_x = Win_x;
		}

		if ( Sp1_trans == 0){
			oldBit_Sp_1[i] = (HBITMAP)SelectObject(mem2dc, hBit_Sp_1[i]);
			TransparentBlt(mem1dc, Sp1_pos_x, Sp1_pos_y, Sp_1_x, Sp_1_y, mem2dc, 0, 0, Sp_1_x, Sp_1_y, RGB(0, 0, 0));
		}
		else if ( Sp1_trans == 1){
			oldBit_Sp_3[i] = (HBITMAP)SelectObject(mem2dc, hBit_Sp_3[i]);
			TransparentBlt(mem1dc, Sp1_pos_x, Sp1_pos_y, Sp_1_x, Sp_1_y, mem2dc, 0, 0, Sp_1_x, Sp_1_y, RGB(0, 0, 0));
		}
		
		if ( Sp2_trans == 0){
			oldBit_Sp_2[i] = (HBITMAP)SelectObject(mem2dc, hBit_Sp_2[i]);
			TransparentBlt(mem1dc, Sp2_pos_x, Sp2_pos_y, Sp_2_x, Sp_2_y, mem2dc, 0, 0, Sp_2_x, Sp_2_y, RGB(0, 0, 0));
		}
		else if ( Sp2_trans == 1){
			oldBit_Sp_4[i] = (HBITMAP)SelectObject(mem2dc, hBit_Sp_4[i]);
			TransparentBlt(mem1dc, Sp2_pos_x, Sp2_pos_y, Sp_2_x, Sp_2_y, mem2dc, 0, 0, Sp_2_x, Sp_2_y, RGB(0, 0, 0));
		}
		
		// 배경이동 및 리스크롤용 뒤로 보내기
		

		i++;

		if ( i == 4 ){
			i = 0;
		}

		SelectObject(mem2dc, oldBit_BG_1);
		DeleteDC(mem2dc);
		SelectObject(mem2dc, oldBit_BG_2);
		DeleteDC(mem2dc);
		SelectObject(mem2dc, oldBit_Sp_1[i]);
		DeleteDC(mem2dc);
		SelectObject(mem2dc, oldBit_Sp_2[i]);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		switch (wParam)
		{
		case 1:
			if ( Check_j == 0){
				Sp1_pos_y -= jump;
			}
			else if ( Check_j == 1){
				Sp2_pos_y -= jump;
			}
			if (jump >= -10){
				jump--;
			}
			else if (jump <= -10){
				jump++;
			}
			count++;
			if (count == 21){
				KillTimer(hWnd, 1);
				count = 0; jump = 10;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			break;
		}		
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

		BitBlt(hdc, 0, 0, Win_x, Win_y, mem1dc, 0, 0, SRCCOPY);

		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem2dc);
		EndPaint(hWnd, &ps);

		break;
	case WM_KEYDOWN:
		if ( wParam == VK_LEFT){
			Sp1_pos_x-=5;
			if (Sp1_pos_x == 0){
				Sp1_pos_x += 5;
			}
		}
		else if ( wParam == VK_RIGHT){
			Sp1_pos_x+=5;
			if (Sp1_pos_x == 700){
				Sp1_pos_x -= 5;
			}
		}
		else if ( wParam == VK_SPACE){
			Check_j = 0;
			SetTimer(hWnd, 1, 100, NULL);
		}
		// sp2 모습 변경
		else if (wParam == 's' || wParam == 'S'){
			Sp1_trans = 1;
		}
		// sp1 모습 변경
		if (wParam == 't' || wParam == 'T'){
			Sp2_trans = 1;
		}
		break;
	case WM_CHAR:
		if (wParam == 'a' || wParam == 'A'){
			Sp2_pos_x-=5;
			if (Sp2_pos_x == 0){
				Sp2_pos_x += 5;
			}
		}
		else if ( wParam == 'd' || wParam == 'D'){
			Sp2_pos_x+=5;
			if (Sp2_pos_x == 700){
				Sp2_pos_x -= 5;
			}
		}
		else if ( wParam == 'w' || wParam == 'W'){
			Check_j = 1;
			SetTimer(hWnd, 1, 100, NULL);
		}
		
		break;
	case WM_KEYUP:
		Sp1_trans = 0;
		Sp2_trans = 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}