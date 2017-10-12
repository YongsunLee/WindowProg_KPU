#include <Windows.h>
#include <math.h>
#include "resource.h"

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
}SHAPE;

float LengthPts(int, int, int, int);
BOOL InCircle(int x, int y, int mx, int my, float rad);

SHAPE shape[1];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 6 - 2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC DLG_hdc;
	PAINTSTRUCT ps;
	static BOOL Draw_Line, MoveToLine, Drag, Draw_Rect, Draw_Ellipse, Selection;
	static int oldX, oldY, endX, endY, startX, startY;
	static int N_x, N_y, temp;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, 0, 100, NULL);
		startX = oldX = shape[0].c_x = 50;
		startY = oldY = shape[0].c_y = 50;
		shape[0].size = 20;
		Drag = FALSE;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			Draw_Line = TRUE;
			break;
		case IDCANCEL:
			Draw_Line = FALSE;
			break;
		case IDC_MOVE:
			MoveToLine = TRUE;
			break;
		case IDC_RECT:
			Draw_Rect = TRUE;
			Draw_Ellipse = FALSE;
			break;
		case IDC_ELLIPSE:
			Draw_Ellipse = TRUE;
			Draw_Rect = FALSE;
			break;
		case IDC_EXIT:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		if ( MoveToLine == TRUE){
			if (N_x < 0){
				shape[0].c_x += N_x;
				if (shape[0].c_x <= endX){
					shape[0].c_x = endX;
					shape[0].c_y = endY;
				}
			}
			else if (N_x > 0){
				shape[0].c_x += N_x;
				if (shape[0].c_x >= endX){
					shape[0].c_x = endX;
					shape[0].c_y = endY;
				}
			}
			if (N_y < 0){
				shape[0].c_y += N_y;
				if (shape[0].c_y <= endY){
					shape[0].c_x = endX;
					shape[0].c_y = endY;
				}
			}
			else if (N_y > 0){
				shape[0].c_y += N_y;
				if (shape[0].c_x >= endY){
					shape[0].c_x = endX;
					shape[0].c_y = endY;
				}
			}
		}
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	case WM_PAINT:
		DLG_hdc = BeginPaint(hDlg, &ps);

		if (Draw_Rect == TRUE){
			Rectangle(DLG_hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size
				, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);
			MoveToEx(DLG_hdc, startX, startY, NULL);
			LineTo(DLG_hdc, oldX, oldY);
		}

		if (Draw_Ellipse == TRUE){
			Ellipse(DLG_hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size
				, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);
			MoveToEx(DLG_hdc, startX, startY, NULL);
			LineTo(DLG_hdc, oldX, oldY);
		}

		EndPaint(hDlg, &ps);
		break;
	case WM_LBUTTONDOWN:
		oldX = LOWORD(lParam);
		oldY = LOWORD(lParam);
		if (InCircle(shape[0].c_x, shape[0].c_y, oldX, oldY, 20)){
			Selection = TRUE;
		}
		Draw_Line = TRUE;
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Draw_Line = FALSE;
		N_x = (oldX - startX)/ 10;
		N_y = (oldY - startY)/ 10;
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		DLG_hdc = GetDC(hDlg);
		if (Draw_Line){
			SetROP2(DLG_hdc, R2_XORPEN);
			SelectObject(DLG_hdc, (HPEN)GetStockObject(WHITE_PEN));

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			MoveToEx(DLG_hdc, startX, startY, NULL);
			LineTo(DLG_hdc, oldX, oldY);

			MoveToEx(DLG_hdc, startX, startY, NULL);
			LineTo(DLG_hdc, endX, endY);

			startX = shape[0].c_x; startY = shape[0].c_y;
			oldX = endX; oldY = endY;
		}
		ReleaseDC(hDlg, DLG_hdc);
		break;
	default:
		break;
	}
	return false;
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
