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
	BOOL check;
}SHAPE;

float LengthPts(int, int, int, int);
BOOL InCircle(int x, int y, int mx, int my, float rad);

SHAPE shape[20];
SHAPE circle[1];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 3 - 9";

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
	static BOOL Selection;
	static int mx, my, c_crash;
	char print[100];

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		srand(time(NULL));
		circle[0].c_x = 0;
		circle[0].c_y = 0;
		circle[0].size = 50;
		c_crash = 0;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			for (int i = 0; i < 20; ++i){
				if (shape[i].draw == FALSE){
					shape[i].draw = TRUE;
					shape[i].c_x = rand() % 800 + 50;
					shape[i].c_y = rand() % 500;
					shape[i].size = rand() % 100 + 20;
					shape[i].r = 0;
					shape[i].g = 255;
					shape[i].b = 0;
					shape[i].check = FALSE;
				}

				if ((InCircle(circle[0].c_x + circle[0].size, circle[0].c_y + circle[0].size, shape[i].c_x, shape[i].c_y, shape[i].size)) ||
					(InCircle(circle[0].c_x - circle[0].size, circle[0].c_y - circle[0].size, shape[i].c_x, shape[i].c_y, shape[i].size))){
					if (shape[i].check == FALSE){
						shape[i].check = TRUE;
						if (shape[i].check == TRUE){
							shape[i].r = 255; shape[i].g = 0;
							c_crash++;
						}
					}
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

		for (int i = 0; i < 20; ++i){
			hBrush = CreateSolidBrush(RGB(shape[i].r, shape[i].g, shape[i].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, shape[i].c_x - shape[i].size, shape[i].c_y - shape[i].size
				, shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			Ellipse(hdc, circle[0].c_x - circle[0].size, circle[0].c_y - circle[0].size
				, circle[0].c_x + circle[0].size, circle[0].c_y + circle[0].size);

			if (circle[0].c_x + circle[0].size >= 800 || circle[0].c_y - circle[0].size >= 560){
				wsprintf(print, "충돌된 원의 개수는 %d", c_crash);
				TextOut(hdc, 360, 260, print, strlen(print));
			}
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(circle[0].c_x, circle[0].c_y, mx, my, circle[0].size)){
			Selection = TRUE;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (Selection == TRUE){
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

			mx = LOWORD(lParam);
			my = HIWORD(lParam);

			Ellipse(hdc, circle[0].c_x - circle[0].size, circle[0].c_y - circle[0].size
				, circle[0].c_x + circle[0].size, circle[0].c_y + circle[0].size);

			Ellipse(hdc, mx - circle[0].size, my - circle[0].size
				, mx + circle[0].size, my + circle[0].size);

			circle[0].c_x = mx; circle[0].c_y = my;
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
