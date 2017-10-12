#include <Windows.h>
#include <time.h>
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

SHAPE shape[10];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 3 - 7";

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
	static int mx, my;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 1, NULL);
		SetTimer(hWnd, 1, 200, NULL);
		srand(time(NULL));
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			for (int i = 0; i < 10; ++i){
				if (shape[i].draw == FALSE){
					shape[i].draw = TRUE;
					shape[i].c_x = rand() % 700;
					shape[i].c_y = rand() % 500 + 120;
					shape[i].size = rand() % 30 + 10;
					shape[i].point[0].x = shape[i].c_x;
					shape[i].point[0].y = shape[i].c_y + shape[i].size;
					shape[i].point[1].x = shape[i].c_x - 10;
					shape[i].point[1].y = shape[i].point[0].y + 10;
					shape[i].point[2].x = shape[i].c_x + 10;
					shape[i].point[2].y = shape[i].point[0].y + 10;
				}

				if (shape[i].c_y <= 0){
					shape[i].draw = FALSE;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 1:
			for (int i = 0; i < 10; ++i){
				shape[i].c_y -= 10;
				shape[i].point[0].y -= 10;
				shape[i].point[1].y -= 10;
				shape[i].point[2].y -= 10;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 10; ++i){
			MoveToEx(hdc, shape[i].c_x, shape[i].c_y, NULL);
			LineTo(hdc, shape[i].c_x, shape[i].c_y + 60);

			Polygon(hdc, shape[i].point, 3);

			Ellipse(hdc, shape[i].c_x - shape[i].size, shape[i].c_y - shape[i].size
				, shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);

			if (shape[i].size >= 100){
				shape[i].draw = FALSE;
			}
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		for (int i = 0; i < 10; ++i){
			if (InCircle(shape[i].c_x, shape[i].c_y, mx, my, shape[i].size)){
				shape[i].size += 20;
			}
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
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