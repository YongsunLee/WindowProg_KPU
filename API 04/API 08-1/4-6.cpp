#include <Windows.h>
#include <math.h>
#include "resource.h"

#pragma warning (disable : 4996)

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	COLORREF rgb;
}SHAPE;

float LengthPts(int, int, int, int);
BOOL InCircle(int x, int y, int mx, int my, float rad);

SHAPE shape[10];
int rgb = RGB(0,0,0);

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 4 - 6";

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU5);
	
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
	static HMENU hMenu, hSubMenu;
	HPEN hPen ,oldPen;
	HBRUSH hBrush, oldBrush;
	static BOOL paste;
	static int shape_num[10] = {};
	static int mx, my;
	static int i;
	static int copy_num;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		i = 0;
		hMenu = GetMenu(hWnd);
		hSubMenu = GetSubMenu(hMenu, 2);
		EnableMenuItem(hSubMenu, ID_EDIT_PASTE, MF_GRAYED);
		paste = FALSE;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			for (int i = 0; i < 10; ++i){
				if (shape[i].draw == FALSE){
					shape[i].draw = TRUE;
					shape[i].c_x = rand() % 800;
					shape[i].c_y = rand() % 520;
					shape[i].size = rand() % 50;
					shape[i].rgb = RGB(0,0,0);
				}
			}
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_DRAW_LINE:
			shape_num[i] = 1;
			break;
		case ID_DRAW_ELLIPSE:
			shape_num[i] = 2;
			break;
		case ID_DRAW_RECTANGLE:
			shape_num[i] = 3;
			break;
		case ID_COLOR_RED:
			rgb = RGB(255,0,0);
			break;
		case ID_COLOR_GREEN:
			rgb = RGB(0, 255, 0);
			break;
		case ID_COLOR_BLUE:
			rgb = RGB(0, 0, 255);
			break;
		case ID_COLOR_YELLOW:
			rgb = RGB(255, 255, 0);
			break;
		case ID_COLOR_CLARET:
			rgb = RGB(255, 0, 255);
			break;
		case ID_COLOR_CYAN:
			rgb = RGB(0, 255, 255);
			break;
		case ID_COLOR_BLACK:
			rgb = RGB(0, 0, 0);
			break;
		case ID_COPY_0:
			copy_num = shape_num[0];
			rgb = shape[0].rgb;
			paste = TRUE;
			break;
		case ID_COPY_1:
			copy_num = shape_num[1];
			rgb = shape[1].rgb;
			paste = TRUE;
			break;
		case ID_COPY_2:
			copy_num = shape_num[2];
			rgb = shape[2].rgb;
			paste = TRUE;
			break;
		case ID_COPY_3:
			copy_num = shape_num[3];
			rgb = shape[3].rgb;
			paste = TRUE;
			break;
		case ID_COPY_4:
			copy_num = shape_num[4];
			rgb = shape[4].rgb;
			paste = TRUE;
			break;
		case ID_COPY_5:
			copy_num = shape_num[5];
			rgb = shape[5].rgb;
			paste = TRUE;
			break;
		case ID_COPY_6:
			copy_num = shape_num[6];
			rgb = shape[6].rgb;
			paste = TRUE;
			break;
		case ID_COPY_7:
			copy_num = shape_num[7];
			rgb = shape[7].rgb;
			paste = TRUE;
			break;
		case ID_COPY_8:
			copy_num = shape_num[8];
			rgb = shape[8].rgb;
			paste = TRUE;
			break;
		case ID_COPY_9:
			copy_num = shape_num[9];
			rgb = shape[9].rgb;
			paste = TRUE;
			break;
		case ID_EDIT_PASTE:
			shape_num[i] = copy_num;
			break;
		case ID_EDIT_CANCEL:
			if ( shape_num[i-1] == copy_num){
				shape_num[i-1] = 0;
			}
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		EnableMenuItem(hSubMenu, ID_EDIT_PASTE, paste?MF_ENABLED:MF_GRAYED);
		
		// 값을 저장
		if (shape[i].draw == TRUE){
			if (shape_num[i] == 1){
				shape[i].rgb = rgb;
				hPen = CreatePen(PS_SOLID, 2, shape[i].rgb);
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, shape[i].c_x, shape[i].c_y, NULL);
				LineTo(hdc, shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
				++i;
			}
			else if (shape_num[i] == 2){
				shape[i].rgb = rgb;
				hBrush = CreateSolidBrush(shape[i].rgb);
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, shape[i].c_x - shape[i].size, shape[i].c_y - shape[i].size
					, shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				++i;

			}
			else if (shape_num[i] == 3){
				shape[i].rgb = rgb;
				hBrush = CreateSolidBrush(shape[i].rgb);
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, shape[i].c_x - shape[i].size, shape[i].c_y - shape[i].size
					, shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				++i;
			}
		}

		// 출력
		for (int i = 0; i < 10; ++i){
			if (shape_num[i] == 1){
				hPen = CreatePen(PS_SOLID, 4, shape[i].rgb);
				oldPen = (HPEN)SelectObject(hdc, hPen);
				MoveToEx(hdc, shape[i].c_x, shape[i].c_y, NULL);
				LineTo(hdc, shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
			else if (shape_num[i] == 2){
				hBrush = CreateSolidBrush(shape[i].rgb);
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, shape[i].c_x - shape[i].size, shape[i].c_y - shape[i].size
					, shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
			else if (shape_num[i] == 3){
				hBrush = CreateSolidBrush(shape[i].rgb);
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, shape[i].c_x - shape[i].size, shape[i].c_y - shape[i].size
					, shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}

		EndPaint(hWnd, &ps);
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

float LengthPts(int x1, int y1, int x2, int y2)
{
	return((sqrt((float)(x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))));
}
BOOL InCircle(int x, int y, int mx, int my, float rad)
{
	if (LengthPts(x, y, mx, my) < rad)
		return TRUE;
	else return FALSE;
}
