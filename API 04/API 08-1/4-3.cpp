#include <Windows.h>
#include "resource.h"

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	int p_r, p_g, p_b;
	BOOL draw;
	int r, g, b;
}SHAPE;

SHAPE shape[1];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 4 - 3";

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU3);

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
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	static BOOL Drag, Move, P_select;
	static int C_shape, answer;
	static int mx, my, oldX, oldY;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		C_shape = 0;
		oldX = shape[0].c_x = 200;
		oldY = shape[0].c_y = 200;
		shape[0].size = 20;
		shape[0].r = 255; shape[0].g = 255; shape[0].b = 255;
		Move = FALSE;
		P_select = FALSE;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			if (Move == TRUE){
				shape[0].c_x++;
				oldX++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		default:
			break;
		}
		
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_COLOR_GREEN:
			shape[0].r = 0; shape[0].g = 255; shape[0].b = 0;
			P_select = TRUE;
			break;
		case ID_COLOR_RED:
			shape[0].r = 255; shape[0].g = 0; shape[0].b = 0;
			P_select = TRUE;
			break;
		case ID_COLOR_YELLOW:
			shape[0].r = 255; shape[0].g = 255; shape[0].b = 0;
			P_select = TRUE;
			break;
		case ID_COLOR_RANDOM:
			shape[0].r = rand()%255; shape[0].g = rand()%255; shape[0].b = rand()%255;
			break;
		case ID_SHAPE_LINE:
			C_shape = 1;
			break;
		case ID_SHAPE_RECTANGLE:
			C_shape = 2;
			break;
		case ID_SHAPE_ELLIPSE:
			C_shape = 3;
			break;
		case ID_BORDER_ON:
			shape[0].p_r = 0; shape[0].p_g = 0; shape[0].p_b = 0;
			break;
		case ID_BORDER_OFF:
			shape[0].p_r = 255; shape[0].p_g = 255; shape[0].p_b = 255;
			break;
		case ID_MOVE_ON:
			Move = TRUE;
			break;
		case ID_MOVE_OFF:
			Move = FALSE;
			break;
		case ID_HELP_HELP:
			MessageBox(hWnd, "연습문제 4 - 3", " Help ", MB_OK);
			break;
		case ID_HELP_EXIT:
			answer = MessageBox(hWnd, "종료하시겠습니까?", "EXIT", MB_OKCANCEL);
			if (answer == IDOK){
				PostQuitMessage(0);
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// 직선
		if (C_shape == 1){
			if ( P_select == FALSE){
				hPen = CreatePen(PS_SOLID, 10, RGB(shape[0].p_r, shape[0].p_g, shape[0].p_b));
				oldPen = (HPEN)SelectObject(hdc, hPen);
			}
			else if (P_select == TRUE){
				hPen = CreatePen(PS_SOLID, 10, RGB(shape[0].r, shape[0].g, shape[0].b));
				oldPen = (HPEN)SelectObject(hdc, hPen);
			}
			MoveToEx(hdc, shape[0].c_x, shape[0].c_y, NULL);
			LineTo(hdc, oldX, oldY);
			//DeleteObject(hPen);
		}
		// 사각형
		else if (C_shape == 2){
			hBrush = CreateSolidBrush(RGB(shape[0].r, shape[0].g, shape[0].b));
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			hPen = CreatePen(PS_SOLID, 10, RGB(shape[0].p_r, shape[0].p_g, shape[0].p_b));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Rectangle(hdc, shape[0].c_x, shape[0].c_y, oldX, oldY);
			SelectObject(hdc, oldBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
		// 원
		else if (C_shape == 3){
			hBrush = CreateSolidBrush(RGB(shape[0].r, shape[0].g, shape[0].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			hPen = CreatePen(PS_SOLID, 10, RGB(shape[0].p_r, shape[0].p_g, shape[0].p_b));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, shape[0].c_x, shape[0].c_y, oldX, oldY);
			SelectObject(hdc, oldBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		Drag = TRUE;
		break;
	case WM_LBUTTONUP:
		Drag = FALSE;
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);

		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (Drag == TRUE){
			if (C_shape == 1){
				SetROP2(hdc, R2_XORPEN);
				SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

				MoveToEx(hdc, shape[0].c_x, shape[0].c_y, NULL);
				LineTo(hdc, oldX, oldY);

				MoveToEx(hdc, shape[0].c_x, shape[0].c_y, NULL);
				LineTo(hdc, mx, my);

				oldX = mx; oldY = my;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (C_shape == 2){
				//SetROP2(hdc, R2_XORPEN);
				SetROP2(hdc, R2_MASKPEN);
				SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

				Rectangle(hdc, shape[0].c_x, shape[0].c_y, oldX, oldY);
			
				Rectangle(hdc, shape[0].c_x, shape[0].c_y, mx, my);
				
				oldX = mx; oldY = my;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (C_shape == 3){
				//SetROP2(hdc, R2_XORPEN);
				SetROP2(hdc, R2_MASKPEN);
				SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

				Ellipse(hdc, shape[0].c_x, shape[0].c_y, oldX, oldY);
			
				Ellipse(hdc, shape[0].c_x, shape[0].c_y, mx, my);

				oldX = mx; oldY = my;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		
		
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}