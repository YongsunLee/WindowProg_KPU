#include <Windows.h>
#include "resource.h"

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 5 - 1";

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
	static HBITMAP hBitmap;
	BITMAP bmp;
	RECT rect;
	static int Status, num;
	static int window_ex, window_ey;
	static int bmp_x, bmp_y;
	static int dv_ex, dv_ey, dv_sx, dv_sy;

	switch (uMsg) {
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		num = 0;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FULLSCREEN_START:
			Status = 1;
			break;
		case ID_BORAD_3:
			Status = 2;
			num = 3;
			break;
		case ID_BORAD_4:
			Status = 2;
			num = 4;
			break;
		case ID_BORAD_5:
			Status = 2;
			num = 5;
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);
		GetClientRect(hWnd, &rect);
		window_ex = rect.right;
		window_ey = rect.bottom;
		bmp_x = bmp.bmWidth;
		bmp_y = bmp.bmHeight;
		SelectObject(memdc, hBitmap);

		if ( Status == 1 ){
			StretchBlt(hdc, 0, 0, window_ex, window_ey, memdc, 0, 0, bmp_x, bmp_y, SRCCOPY);
		}
		else if (Status == 2){
			dv_sx = 0;
			dv_sy = 0;
			dv_ex = window_ex / num;
			dv_ey = window_ey / num;
			while (1){
				StretchBlt(hdc, dv_sx, dv_sy, dv_ex, dv_ey, memdc, 0, 0, bmp_x, bmp_y, SRCCOPY);
				if ( dv_sx >= window_ex){
					dv_sy += dv_ey;
					dv_sx = 0;
				}
				else 
					dv_sx += dv_ex;
				if ( dv_sy >= window_ey){
					break;
				}
			}
		}

		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*
여러개의 이미지를 그려넣기

메뉴 만들기
파일
그림 갯수 3 / 4/ 5
그림 움직이기 시작/ 멈추기 (그림 마다 위치 변환)
종료 프로그램 종료

래스터 변환
대상 색상 반전
소스와 대상을 OR 연산
소스 값 그대로
소스의 대상을 AND 연산
*/