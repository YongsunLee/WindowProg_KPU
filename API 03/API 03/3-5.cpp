#include <Windows.h>
#include <time.h>
#include <math.h>

typedef struct{
	int type;
	POINT point[3];
	int x, y;
	int size;
	BOOL draw;
	int r, g, b;
}SHAPE;

float LengthPts(int, int, int, int);
BOOL InCircle(int x, int y, int mx, int my, float rad);
SHAPE shape[10] = {};

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 3 - 5";

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
	HPEN hPen, oldPen;
	static int mx, my;
	static int i, c_x[10], c_y[10];
	static BOOL selection;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 100, NULL);
		srand(time(NULL));
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			for (i = 0; i < 10; ++i){
				if (shape[i].draw == FALSE){
					shape[i].draw = TRUE;
					shape[i].x = rand() % 700;
					shape[i].y = rand() % 500;
					shape[i].size = rand() % 100;
					shape[i].r = rand() % 255;
					shape[i].g = rand() % 255;
					shape[i].b = rand() % 255;
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

		for (i = 0; i < 10; ++i){
			if (shape[i].draw == TRUE){
				hPen = CreatePen(PS_SOLID, 5, RGB(shape[i].r, shape[i].g, shape[i].b));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Ellipse(hdc, shape[i].x - shape[i].size, shape[i].y - shape[i].size,
					shape[i].x + shape[i].size, shape[i].y + shape[i].size);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (int i = 0; i < 10; ++i){
			if (InCircle(shape[i].x, shape[i].y, mx, my, shape[i].size)){
				shape[i].draw = FALSE;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
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