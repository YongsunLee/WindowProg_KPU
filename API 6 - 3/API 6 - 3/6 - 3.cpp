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

SHAPE shape[2];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 6 - 3";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DiaLogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	switch (uMsg) {
	case WM_CREATE:
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DiaLogProc);
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK DiaLogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC DLG_hdc;
	PAINTSTRUCT ps;
	static BOOL Spwan_Sp1, Spwan_Sp2, c_jump, Grid, c_Move, active, active2;
	RECT rect;
	static int Win_x, Win_y, jump, count;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, 0, 100, NULL);
		shape[0].c_x = 50;
		shape[0].c_y = 50;
		shape[0].size = 10;
		GetClientRect(hDlg, &rect);
		Win_x = rect.right;
		Win_y = rect.bottom;
		jump = 10;
		count = 0;
		active = TRUE;
		active2 = TRUE;
		break;
	case WM_TIMER:
		if (c_jump == TRUE){
			shape[0].c_y -= jump;
			if ( jump >= -10){
				jump--;
			}
			else if ( jump <= -10){
				jump++;
			}
			count++;
			if (count == 21){
				count = 0; jump = 10; c_jump = FALSE;
			}
		}

		if (c_Move == TRUE){
			if ( active == TRUE){
				if (shape[0].c_x < (Win_x - shape[0].size)){
						shape[0].c_x += 10;
				}
			}
			if (shape[0].c_x >= (Win_x - shape[0].size) && shape[0].c_y <= Win_y){
				shape[0].c_y += 2;
			}
			if (active2 == TRUE){
				if (shape[0].c_y >= 100){
					active = FALSE;
					shape[0].c_x -= 10;
				}
			}
			if (shape[0].c_x <= shape[0].size){
				active2 = FALSE;
				shape[0].c_y += 2;
			}
			if (shape[0].c_y >= 150){
				shape[0].c_y -= 2;
				active = TRUE;
			}
		}
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_DIR_UP:
			shape[0].c_y -= 3;
			break;
		case IDC_DIR_RIGHT:
			shape[0].c_x += 3; 
			break;
		case IDC_DIR_DOWN:
			shape[0].c_y += 3;
			break;
		case IDC_DIR_LEFT:
			shape[0].c_x -= 3;
			break;
		case IDC_JUMP:
			c_jump = TRUE;
			break;
		case IDC_SP1:
			Spwan_Sp1 = TRUE;
			Spwan_Sp2 = FALSE;
			break;
		case IDC_SP2:
			Spwan_Sp1 = FALSE;
			Spwan_Sp2 = TRUE;
			break;
		case IDC_SIZE_UP:
			shape[0].size = 20;
			break;
		case IDC_SIZE_DOWN:
			shape[0].size = 5;
			break;
		case IDC_MOVE:
			c_Move = TRUE;
			break;
		case IDC_STOP:
			c_Move = FALSE;
			break;
		case IDC_RESET:
			shape[0].c_x = 50;
			shape[0].c_y = 50;
			break;
		case IDC_EXIT:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;
		case IDC_GRID:
			if ( Grid == FALSE){
				Grid = TRUE;
			} else {
				Grid = FALSE;
			}
			break;
		}
		break;
	case WM_PAINT:
		DLG_hdc = BeginPaint(hDlg, &ps);

		if (Spwan_Sp1 == TRUE){
			Ellipse(DLG_hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size
				, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);
		}
		if (Spwan_Sp2 == TRUE){
			Rectangle(DLG_hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size
				, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);
		}
		if (Grid == TRUE){
			for (int i = 0; i < Win_x; i += 50){
				MoveToEx(DLG_hdc, i, 0, NULL);
				LineTo(DLG_hdc, i, Win_y);
			}
			for (int j = 0; j < Win_y; j += 50){
				MoveToEx(DLG_hdc, 0, j, NULL);
				LineTo(DLG_hdc, Win_x, j);
			}
		}

		EndPaint(hDlg, &ps);
		break;
	}
	return false;
}