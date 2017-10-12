#include <Windows.h>
#include <math.h>
#include <time.h>
#include "resource.h"

typedef struct{
	int type;
	POINT point[4];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
	BOOL eat;
}SHAPE;

float LengthPts(int, int, int, int);
BOOL InCircle(int x, int y, int mx, int my, float rad);

SHAPE pie[2];
SHAPE food[30];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 4 - 2";

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);

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
	static int shape, dircetion, size, Speed;
	static int GridX, GridY, rad, Check, count;
	static float temp, c_X, c_Y;
	static BOOL Grid, Food;

	switch (uMsg) {
	case WM_CREATE:
		pie[0].r = 255; pie[0].g = 255; pie[0].b = 255;
		pie[0].point[0].x = 100; pie[0].point[0].y = 100;
		pie[0].point[1].x = 200; pie[0].point[1].y = 200;
		pie[0].point[2].x = 200; pie[0].point[2].y = 110;
		pie[0].point[3].x = 200; pie[0].point[3].y = 190;
		
		pie[1].point[0].x = 100; pie[1].point[0].y = 100;
		pie[1].point[1].x = 200; pie[1].point[1].y = 200;
		pie[1].point[2].x = 200; pie[1].point[2].y = 140;
		pie[1].point[3].x = 200; pie[1].point[3].y = 160;

		shape = 1;
		Speed = 25;
		for (int i = 0; i < 30; ++i){
			food[i].eat = FALSE;
		}
		Check = 1;
		count = 0;
		srand(time(NULL));
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_GAME_START:
			SetTimer(hWnd, 0, 200, NULL);
			SetTimer(hWnd, 4, 200, NULL);
			break;
		case ID_GAME_END:
			KillTimer(hWnd, 0);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			break;
		case ID_COLOR_CYAN:
			pie[0].r = 0; pie[0].g = 255; pie[0].b = 255;
			break;
		case ID_COLOR_MAGENTA:
			pie[0].r = 255; pie[0].g = 0; pie[0].b = 255;
			break;
		case ID_COLOR_YELLOW:
			pie[0].r = 255; pie[0].g = 255; pie[0].b = 0;
			break;
		case ID_COLOR_RANDOM:
			pie[0].r = rand()%255; pie[0].g = rand()%255; pie[0].b = rand()%255;
			break;
		case ID_FOOD_10:
			size = 10;
			SetTimer(hWnd, 5, 200, NULL);
			Food = TRUE;
			break;
		case ID_FOOD_20:
			size = 20;
			SetTimer(hWnd, 5, 200, NULL);
			Food = TRUE;
			break;
		case ID_FOOD_30:
			size = 30;
			SetTimer(hWnd, 5, 200, NULL);
			Food = TRUE;
			break;
		case ID_GRID_ON:
			Grid = TRUE;
			break;
		case ID_GRID_OFF:
			Grid = FALSE;
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		switch (wParam)
		{
		// 오른쪽 이동
		case 0:
			for (int i = 0; i < 4; ++i){
				pie[0].point[i].x += Speed;
				pie[1].point[i].x += Speed;
			}break;
		// 왼쪽 이동
		case 1:
			for (int i = 0; i < 4; ++i){
				pie[0].point[i].x -= Speed;
				pie[1].point[i].x -= Speed;
			}
			break;
		// 아래쪽 이동
		case 2:
			for (int i = 0; i < 4; ++i){
				pie[0].point[i].y += Speed;
				pie[1].point[i].y += Speed;
			}
			break;
		// 위쪽 이동
		case 3:
			for (int i = 0; i < 4; ++i){
				pie[0].point[i].y -= Speed;
				pie[1].point[i].y -= Speed;
			}
			break;
		// 팩맨 모양
		case 4:
			if (shape == 1) {
				shape = 2;
			} else if ( shape == 2) {
				shape = 1;
			}
			break;
		// FOOD 좌표값 지정
		case 5:
			for (int i = 0; i < size; ++i){
				if ( food[i].draw == FALSE){
					food[i].draw = TRUE;
					food[i].c_x = rand() % 12 + 2;
					food[i].c_y = rand() % 9;
					food[i].c_x = food[i].c_x * 50;
					food[i].c_y = food[i].c_y * 50;
					food[i].size = 15;
					food[i].r = 255; food[i].g = 0; food[i].b = 0;
					count++;
				}
				
				c_X = (pie[0].point[1].x + pie[0].point[0].x) / 2;
				c_Y = (pie[0].point[1].y + pie[0].point[0].y) / 2;
				
				rad = LengthPts(c_X, c_Y, pie[0].point[1].x, pie[0].point[1].y);
				
				if (InCircle(c_X,c_Y, food[i].c_x, food[i].c_y, rad)){
					food[i].eat = TRUE;
					Speed++;
				}
				
				if (food[i].eat == TRUE){
					food[i].draw = FALSE;
					if (Check == 1){
						pie[0].point[0].x -= 30; pie[0].point[0].y -= 30;
						pie[0].point[1].x += 30; pie[0].point[1].x += 30;
						Check = 2;
					}
					else if (Check == 2){
						pie[1].point[0].x -= 30; pie[1].point[0].y -= 30;
						pie[1].point[1].x += 30; pie[1].point[1].x += 30;
						Check = 3;
					}
					count--;
				}
				if (count == 0){
					KillTimer(hWnd, 0);
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					KillTimer(hWnd, 3);
					KillTimer(hWnd, 4);
				}
			}
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// 그리드 그리기
		if (Grid == TRUE){
			for (GridX = 50; GridX <= 800; GridX += 49){
				for (GridY = 50; GridY <= 540; GridY += 50){
					MoveToEx(hdc, 0, GridY, NULL);
					LineTo(hdc, 800, GridY);
				}
				MoveToEx(hdc, GridX, 0, NULL);
				LineTo(hdc, GridX, 540);
			}
		}

		// 푸드 그리기
		if (Food == TRUE){
			for (int i = 0; i < size; ++i){
				if ( food[i].draw == TRUE){
					hBrush = CreateSolidBrush(RGB(food[i].r, food[i].g, food[i].b));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, food[i].c_x - food[i].size, food[i].c_y - food[i].size
						, food[i].c_x + food[i].size, food[i].c_y + food[i].size);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
			}
		}

		// 팩맨 그리기
		hBrush = CreateSolidBrush(RGB(pie[0].r, pie[0].g, pie[0].b));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		if (shape == 1){
			Pie(hdc, pie[0].point[0].x, pie[0].point[0].y, pie[0].point[1].x, pie[0].point[1].y,
				pie[0].point[2].x, pie[0].point[2].y, pie[0].point[3].x, pie[0].point[3].y);
		}
		else if (shape == 2){
			Pie(hdc, pie[1].point[0].x, pie[1].point[0].y, pie[1].point[1].x, pie[1].point[1].y,
				pie[1].point[2].x, pie[1].point[2].y, pie[1].point[3].x, pie[1].point[3].y);
		}
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if ( wParam == VK_UP){
			pie[0].point[2].x = pie[0].point[1].x - 90; pie[0].point[2].y = pie[0].point[0].y;
			pie[0].point[3].x = pie[0].point[1].x - 10; pie[0].point[3].y = pie[0].point[0].y;

			pie[1].point[2].x = pie[0].point[1].x - 60; pie[1].point[2].y = pie[0].point[0].y;
			pie[1].point[3].x = pie[0].point[1].x - 40; pie[1].point[3].y = pie[0].point[0].y;

			//Check = 4;

			KillTimer(hWnd, 0);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			SetTimer(hWnd, 3, 200, NULL);
		}
		else if ( wParam == VK_DOWN){
			pie[0].point[2].x = pie[0].point[0].x + 90; pie[0].point[2].y = pie[0].point[1].y;
			pie[0].point[3].x = pie[0].point[0].x + 10; pie[0].point[3].y = pie[0].point[1].y;

			pie[1].point[2].x = pie[0].point[0].x + 60; pie[1].point[2].y = pie[0].point[1].y;
			pie[1].point[3].x = pie[0].point[0].x + 40; pie[1].point[3].y = pie[0].point[1].y;

			//Check = 2;

			KillTimer(hWnd, 0);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 3);
			SetTimer(hWnd, 2, 200, NULL);
		}
		else if ( wParam == VK_LEFT){
			pie[0].point[2].x = pie[0].point[0].x; pie[0].point[2].y = pie[0].point[0].y + 90;
			pie[0].point[3].x = pie[0].point[0].x; pie[0].point[3].y = pie[0].point[0].y + 10;

			pie[1].point[2].x = pie[0].point[0].x; pie[1].point[2].y = pie[0].point[0].y + 60;
			pie[1].point[3].x = pie[0].point[0].x; pie[1].point[3].y = pie[0].point[0].y + 40;

			//Check = 3;

			KillTimer(hWnd, 0);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			SetTimer(hWnd, 1, 200, NULL);
		}
		else if ( wParam == VK_RIGHT){
			pie[0].point[2].x = pie[0].point[1].x; pie[0].point[2].y = pie[0].point[1].y - 90;
			pie[0].point[3].x = pie[0].point[1].x; pie[0].point[3].y = pie[0].point[1].y - 10;

			pie[1].point[2].x = pie[0].point[1].x; pie[1].point[2].y = pie[0].point[1].y - 60;
			pie[1].point[3].x = pie[0].point[1].x; pie[1].point[3].y = pie[0].point[1].y - 40;
			
			//Check = 1;

			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			SetTimer(hWnd, 0, 200, NULL);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0);
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
