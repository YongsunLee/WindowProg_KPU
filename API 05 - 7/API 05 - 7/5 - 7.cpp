/* 5 - 8
메뉴 항목 "그림 나누기"를 선택하면
그림을 숫자에 맞게 분할하여 랜덤하게 화면에 배치한다.

메뉴 항목 "게임 시작"을 선택하면 아래와 같이 게임이 시작된다.
	마우스를 이용하여 조각 그림을 이용한다.
	이동은 단번에 이동하지 않고 단계별로 조금씩 이동한다.

메뉴 및 서브메뉴
게임 : 게임 시작/ 그림 전체보기 / 종료
그림 나누기 : 3 * 3 , 4 * 4 , 5 * 5

*/

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
LPCTSTR lpszClass = "연습 문제 5 - 7";

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
	static HBITMAP hBit_All, hBit_BG_1, hBit_BG_2, oldBit_All, oldBit_BG_1, oldBit_BG_2
		, hBit_Sp_1[4], hBit_CSp_1[4], hBit_Sp_2[4], oldBit_Sp_1[4], oldBit_CSp_1[4], oldBit_Sp_2[4];
	PAINTSTRUCT ps;
	RECT rect;
	static BITMAP BG_2, Sp_1, Sp_2;
	static int Win_x, Win_y, BG_2_x, BG_2_y, F_BG_x, B_BG_x;
	static int i;
	static BOOL Check_j, Check_c;
	static int Sp1_x, Sp1_y, Sp2_x;
	static int count = 0, jump = 15;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		hBit_BG_1 = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBit_BG_2 = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP11));

		hBit_Sp_1[0] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3));
		hBit_Sp_1[1] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP4));
		hBit_Sp_1[2] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP5));
		hBit_Sp_1[3] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP6));

		hBit_CSp_1[0] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP12));
		hBit_CSp_1[1] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP13));
		hBit_CSp_1[2] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP14));
		hBit_CSp_1[3] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP15));

		hBit_Sp_2[0] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP7));
		hBit_Sp_2[1] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP8));
		hBit_Sp_2[2] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP9));
		hBit_Sp_2[3] = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP10));

		GetObject(hBit_BG_2, sizeof(BITMAP), (LPSTR)&BG_2);
		GetObject(hBit_Sp_1[0], sizeof(BITMAP), (LPSTR)&Sp_1);
		GetObject(hBit_Sp_2[0], sizeof(BITMAP), (LPSTR)&Sp_2);
		GetClientRect(hWnd, &rect);
		Win_x = rect.right;
		Win_y = rect.bottom;
		F_BG_x = 0;
		B_BG_x = 0;
		BG_2_x = BG_2.bmWidth;
		BG_2_y = BG_2.bmHeight;
		Sp1_x = 600;
		Sp1_y = 400;
		Sp2_x = 0;
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);

		if (hBit_All == NULL) hBit_All = CreateCompatibleBitmap(hdc, Win_x, Win_y);

		// dc 호환
		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		oldBit_All = (HBITMAP)SelectObject(mem1dc, hBit_All);

		oldBit_BG_1 = (HBITMAP)SelectObject(mem2dc, hBit_BG_1);
		BitBlt(mem1dc, F_BG_x, 0, Win_x, Win_y, mem2dc, 0, 0, SRCCOPY);
		
		BitBlt(mem1dc, 0, 0, Win_x, Win_y, mem2dc, Win_x - F_BG_x, 0, SRCCOPY);

		oldBit_BG_2 = (HBITMAP)SelectObject(mem2dc, hBit_BG_2);
		StretchBlt(mem1dc, B_BG_x, 400, BG_2_x, BG_2_y, mem2dc, 0, 300, BG_2_x, BG_2_y, SRCCOPY);

		StretchBlt(mem1dc, 0, 400, BG_2_x, BG_2_y, mem2dc, Win_x - B_BG_x, 300, BG_2_x, BG_2_y, SRCCOPY);

		if ( Check_c == FALSE){
			oldBit_Sp_1[i] = (HBITMAP)SelectObject(mem2dc, hBit_Sp_1[i]);
			TransparentBlt(mem1dc, Sp1_x, Sp1_y, Sp_1.bmWidth, Sp_1.bmHeight, mem2dc, 0, 0, Sp_1.bmWidth, Sp_1.bmHeight, RGB(0, 0, 0));
		}
		else if(Check_c == TRUE){
			oldBit_CSp_1[i] = (HBITMAP)SelectObject(mem2dc, hBit_CSp_1[i]);
			TransparentBlt(mem1dc, Sp1_x, Sp1_y, Sp_1.bmWidth, Sp_1.bmHeight, mem2dc, 0, 0, Sp_1.bmWidth, Sp_1.bmHeight, RGB(0, 0, 0));
		}
		
		oldBit_Sp_2[i] = (HBITMAP)SelectObject(mem2dc, hBit_Sp_2[i]);
		TransparentBlt(mem1dc, Sp2_x, 400, Sp_2.bmWidth, Sp_2.bmHeight, mem2dc, 0,0, Sp_2.bmWidth, Sp_2.bmHeight, RGB(0,0,0));

		F_BG_x += 5;
		B_BG_x += 3;
		Sp2_x += 5;

		if ( (Win_x - F_BG_x) <= 0){
			F_BG_x = 0;
		}
		if ( (Win_x - B_BG_x) <= 0){
			B_BG_x = 0;
		}
		if ( Sp2_x >= Win_x){
			Sp2_x = 0;
		}
		i++;
		if ( i == 4){
			i = 0;
		}

		if ( Check_j == TRUE){
			Sp1_y -= jump;
			if (jump >= -15){
				jump--;
			}
			else if (jump <= -15){
				jump++;
			}
			count++;
			if (count == 31){
				count = 0; jump = 15; Check_j = FALSE;
			}
		}

		if ((Sp1_x - Sp2_x) <= 40 && (Sp1_x - Sp2_x)  >= -40 && Sp1_y == 400){
			Check_c = TRUE;
		}
		else {
			Check_c = FALSE;
		}

		SelectObject(mem2dc, oldBit_BG_1);
		DeleteDC(mem2dc);
		SelectObject(mem2dc, oldBit_BG_2);
		DeleteDC(mem2dc);
		SelectObject(mem2dc, oldBit_Sp_1[i]);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBit_All);
		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		if ( wParam == VK_UP ){
			Check_j = TRUE;
		}
		else if ( wParam == VK_DOWN ){
			
		}
		else if ( wParam == VK_LEFT ){
			Sp1_x--;
		}
		else if ( wParam == VK_RIGHT ){
			Sp1_x++;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		oldBit_All = (HBITMAP)SelectObject(mem1dc, hBit_All);

		BitBlt(hdc, 0, 0, Win_x, Win_y, mem1dc, 0, 0, SRCCOPY);

		SelectObject(mem1dc, oldBit_All);
		DeleteDC(mem2dc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}