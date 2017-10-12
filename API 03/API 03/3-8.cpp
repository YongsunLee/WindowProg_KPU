#include <Windows.h>
#include <math.h>

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
LPCTSTR lpszClass = "연습문제 3 - 8";

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
	HBRUSH hBrush, oldBrush;
	static int oldX, oldY, endX, endY, startX, startY;
	static int N_x, N_y, temp;
	static BOOL Drag, Selection;

	switch (uMsg) {
	case WM_CREATE:
		startX = oldX = shape[0].c_x = 20;
		startY = oldY = shape[0].c_y = 20;
		shape[0].size = 20;
		shape[0].r = rand() % 255;
		shape[0].g = rand() % 255;
		shape[0].b = rand() % 255;
		Drag = FALSE;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			if (N_x < 0){
				shape[0].c_x += N_x;
				if (shape[0].c_x <= endX){
					KillTimer(hWnd, 0);
					shape[0].c_x = endX;
					shape[0].c_y = endY;
				}
			}
			else if (N_x > 0){
				shape[0].c_x += N_x;
				if (shape[0].c_x >= endX){
					KillTimer(hWnd, 0);
					shape[0].c_x = endX;
					shape[0].c_y = endY;
				}
			}
			if (N_y < 0){
				shape[0].c_y += N_y;
				if (shape[0].c_y <= endY){
					KillTimer(hWnd, 0);
					shape[0].c_x = endX;
					shape[0].c_y = endY;
				}
			}
			else if (N_y > 0){
				shape[0].c_y += N_y;
				if (shape[0].c_x >= endY){
					KillTimer(hWnd, 0);
					shape[0].c_x = endX;
					shape[0].c_y = endY;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (Selection == TRUE){
			hBrush = CreateSolidBrush(RGB(shape[0].r, shape[0].g, shape[0].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);
			SelectObject(hdc, hBrush);
			DeleteObject(hBrush);
		}
		Ellipse(hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);

		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		oldX = LOWORD(lParam);
		oldY = HIWORD(lParam);
		if (InCircle(shape[0].c_x, shape[0].c_y, oldX, oldY, 20)){
			Selection = TRUE;
		}
		Drag = TRUE;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Drag = FALSE;
		if (Selection == TRUE){
			SetTimer(hWnd, 0, 1, NULL);
			N_x = (oldX - startX) / 10;
			N_y = (oldY - startY) / 10;
		}
		Selection = FALSE;
		break;
	case WM_MOUSEMOVE:
		KillTimer(hWnd, 0);
		hdc = GetDC(hWnd);
		if (Drag){
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);

			startX = shape[0].c_x; startY = shape[0].c_y;
			oldX = endX; oldY = endY;
		}
		ReleaseDC(hWnd, hdc);
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
