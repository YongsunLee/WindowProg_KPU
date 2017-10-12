#include <Windows.h>

#define IDC_BUTTON_R 100
#define IDC_BUTTON_L 101
#define IDC_BUTTON_T 102
#define IDC_BUTTON_B 103
#define IDC_BUTTON_EXIT 104
#define IDC_BUTTON_GRID 105
#define IDC_BUTTON_RED 106
#define IDC_BUTTON_GREEN 107
#define IDC_BUTTON_YELLOW 108
#define IDC_BUTTON_BLACK 109

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
}SHAPE;

SHAPE shape[1];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습 문제 1 - 2";

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
	HDC hdc;
	PAINTSTRUCT ps;
	static HBRUSH hBrush, oldBrush;
	static HWND hButton[5], hGrid[1], hRadio[4];
	static int GridX, GridY;
	static BOOL Grid_ON;
	static COLORREF rgb = RGB(255,255,255);

	switch (uMsg) {
	case WM_CREATE:
		hButton[0] = CreateWindow("button", "R", WS_CHILD | WS_VISIBLE | WS_BORDER, 700, 40, 30, 30, hWnd,(HMENU)IDC_BUTTON_R, g_hinst,NULL);
		hButton[1] = CreateWindow("button", "L", WS_CHILD | WS_VISIBLE | WS_BORDER, 640, 40, 30, 30, hWnd, (HMENU)IDC_BUTTON_L, g_hinst, NULL);
		hButton[2] = CreateWindow("button", "T", WS_CHILD | WS_VISIBLE | WS_BORDER, 670, 25, 30, 30, hWnd, (HMENU)IDC_BUTTON_T, g_hinst, NULL);
		hButton[3] = CreateWindow("button", "B", WS_CHILD | WS_VISIBLE | WS_BORDER, 670, 60, 30, 30, hWnd, (HMENU)IDC_BUTTON_B, g_hinst, NULL);
		hButton[4] = CreateWindow("button", "EXIT", WS_CHILD | WS_VISIBLE | WS_BORDER, 640, 500, 100, 30, hWnd, (HMENU)IDC_BUTTON_EXIT, g_hinst, NULL);
		hGrid[0] = CreateWindow("button", "Grid", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 640, 100, 100,30, hWnd, (HMENU)IDC_BUTTON_GRID, g_hinst, NULL);
		hRadio[0] = CreateWindow("button", "Red", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 640, 150, 100, 30, hWnd, (HMENU)IDC_BUTTON_RED, g_hinst, NULL);
		hRadio[1] = CreateWindow("button", "Green", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 640, 170, 100, 30, hWnd, (HMENU)IDC_BUTTON_GREEN, g_hinst, NULL);
		hRadio[2] = CreateWindow("button", "Yellow", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 640, 190, 100, 30, hWnd, (HMENU)IDC_BUTTON_YELLOW, g_hinst, NULL);
		hRadio[3] = CreateWindow("button", "Black", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 640, 210, 100, 30, hWnd, (HMENU)IDC_BUTTON_BLACK, g_hinst, NULL);
		
		shape[0].c_x = 75;
		shape[0].c_y = 75;
		shape[0].size = 23;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_R:
			shape[0].c_x += 50;
			break;
		case IDC_BUTTON_L:
			shape[0].c_x -= 50;
			break;
		case IDC_BUTTON_T:
			shape[0].c_y -= 50;
			break;
		case IDC_BUTTON_B:
			shape[0].c_y += 50;
			break;
		case IDC_BUTTON_GRID:
			if (SendMessage(hGrid[0], BM_GETCHECK, 0, 0) == BST_UNCHECKED){
				SendMessage(hGrid[0], BM_SETCHECK, BST_CHECKED, 0);
				Grid_ON = TRUE;
			}
			else{
				SendMessage(hGrid[0], BM_SETCHECK, BST_UNCHECKED, 0);
				Grid_ON = FALSE;
			}
			break;
		case IDC_BUTTON_RED:
			rgb = RGB(255,0,0);
			break;
		case IDC_BUTTON_GREEN:
			rgb = RGB(0,255,0);
			break;
		case IDC_BUTTON_YELLOW:
			rgb = RGB(255,255,0);
			break;
		case IDC_BUTTON_BLACK:
			rgb = RGB(0,0,0);
			break;
		case IDC_BUTTON_EXIT:
			PostQuitMessage(0);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (Grid_ON){
			for (GridX = 0; GridX <= 500; GridX += 50){
				MoveToEx(hdc, GridX, 0, NULL);
				LineTo(hdc, GridX, 300);
			}
			for (GridY = 0; GridY <= 300; GridY += 50){
				MoveToEx(hdc, 0, GridY, NULL);
				LineTo(hdc, 500, GridY);
			}
		}

		hBrush = CreateSolidBrush(rgb);
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size,
			shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}