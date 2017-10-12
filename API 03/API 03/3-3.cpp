#include <Windows.h>
#include <time.h>
#include <math.h>

typedef struct{
	BOOL draw;
	int type;
	POINT point[2];
	int direct;
	int rand_size[4];
	DWORD color;
}SHAPE;

float LengthPts(int x1, int y1, int x2, int y2);

SHAPE shape[4] = {};

HINSTANCE g_hinst;
LPCTSTR lpszClass = "Window program 1 - 2";

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
	static int size_x, size_y, c_x, c_y;
	static int rad[4], temp;
	static int mc_size[4];
	static double seta[4], move_x[4], move_y[4];

	switch (uMsg) {
	case WM_CREATE:
		// Áß¾Ó¿¡ »¡°£»ö ¿ø
		SetTimer(hWnd, 0, 1, NULL);
		// ¹ð±Û¹ð±Û µµ´Â ¿ø
		SetTimer(hWnd, 1, 1, NULL);

		srand(time(NULL));
		shape[0].point[0].x = 180;
		shape[0].point[0].y = 130;
		shape[0].point[1].x = 200;
		shape[0].point[1].y = 150;
		shape[0].rand_size[0] = rand() % 100 + 30;
		size_x = 400;
		size_y = 280;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			for (int i = 1; i < 4; ++i){
				if (shape[i].draw == FALSE){
					if (i == 1){
						shape[i].draw = TRUE;
						shape[i].point[0].x = shape[0].point[0].x + size_x;
						shape[i].point[0].y = shape[0].point[0].y;
						shape[i].point[1].x = shape[0].point[1].x + size_x;
						shape[i].point[1].y = shape[0].point[1].y;
						shape[i].rand_size[i] = rand() % 100 + 20;
					}
					else if (i == 2){
						shape[i].draw = TRUE;
						shape[i].point[0].x = shape[0].point[0].x;
						shape[i].point[0].y = shape[0].point[0].y + size_y;
						shape[i].point[1].x = shape[0].point[1].x;
						shape[i].point[1].y = shape[0].point[1].y + size_y;
						shape[i].rand_size[i] = rand() % 100 + 20;
					}
					else if (i == 3){
						shape[i].draw = TRUE;
						shape[i].point[0].x = shape[0].point[0].x + size_x;
						shape[i].point[0].y = shape[0].point[0].y + size_y;
						shape[i].point[1].x = shape[0].point[1].x + size_x;
						shape[i].point[1].y = shape[0].point[1].y + size_y;
						shape[i].rand_size[i] = rand() % 100 + 20;
					}
					mc_size[i] = rand() % 30;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 1:
			for (int i = 0; i < 4; ++i){
				rad[i] = shape[i].rand_size[i];
				move_x[i] = rad[i] * sin(seta[i]);
				move_y[i] = rad[i] * cos(seta[i]);
				if (i == 0){
					seta[i] += 0.01;
				}
				else if (i == 1){
					seta[i] -= 0.02;
				}
				else if (i == 2){
					seta[i] += 0.04;
				}
				else if (i == 3){
					seta[i] -= 0.03;
				}
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 4; ++i){
			Ellipse(hdc, shape[i].point[0].x - shape[i].rand_size[i], shape[i].point[0].y - shape[i].rand_size[i],
				shape[i].point[1].x + shape[i].rand_size[i], shape[i].point[1].y + shape[i].rand_size[i]);

			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, shape[i].point[0].x, shape[i].point[0].y
				, shape[i].point[1].x, shape[i].point[1].y);
			SelectObject(hdc, oldBrush);

			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, (shape[i].point[0].x + move_x[i]) - mc_size[i], (shape[i].point[0].y + move_y[i]) - mc_size[i],
				(shape[i].point[1].x + move_x[i]) + mc_size[i], (shape[i].point[1].y + move_y[i]) + mc_size[i]);
			SelectObject(hdc, oldBrush);

		}

		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == 'q'){
			PostQuitMessage(0);
		}
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