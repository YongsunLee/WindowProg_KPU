#include <Windows.h>
#include <time.h>
#include <math.h>
#include "resource.h"

#define Speed 400

#pragma warning (disable : 4996)

typedef struct{
	int type;
	int c_x, c_y;
	int size;
	BOOL draw;
	COLORREF rgb;
}SHAPE;

SHAPE Block[150];

float LengthPts(int x1, int y1, int x2, int y2);
BOOL InCircle(int x, int y, int mx, int my, float rad);

HINSTANCE g_hinst;
LPCTSTR lpszClass = "API HomeWork 01";

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

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

static int i;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	RECT rect;
	static BOOL col;
	char str[50];
	static int score;
	static int R_num = 4;
	static int GridX, GridY;
	int color[5][3] = { { 255, 0, 0 },
						{ 0, 255, 0 },
						{ 0, 0, 255 },
						{ 255, 255, 255 },
						{ 255, 255, 0} };
	int S_color;
	static int R_count = 0;
	static int answer;
	static int GameStatus = 0;
	static int Plane = 0;
	static int Y_last = 400;
	static int X_start = 100;
	static int X_last = 300;
	static int Lv = 1;
	static int C_Right = 0;
	
	switch (uMsg) {
	case WM_CREATE:
		srand(time(NULL));
		SetTimer(hWnd, 0, 100, NULL);
		SetTimer(hWnd, 2, Speed, NULL);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		// Make Block
		case 0:
			if (GameStatus == 1){
				if (Block[i].draw == FALSE){
					Block[i].c_x = 190;
					Block[i].c_y = 90;
					Block[i].size = 10;
					Block[i].draw = TRUE;
					S_color = rand() % R_num;
					Block[i].rgb = RGB(color[S_color][0], color[S_color][1], color[S_color][2]);
					SetTimer(hWnd, 1, Speed, NULL);
					if ( Lv == 2){
						Block[i+1].c_x = 190;
						Block[i+1].c_y = 110;
						Block[i+1].size = 10;
						Block[i+1].draw = TRUE;
						S_color = rand() % R_num;
						Block[i+1].rgb = RGB(color[S_color][0], color[S_color][1], color[S_color][2]);
					}
					else if ( Lv == 3){

					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		// Move Block And Chage Count
		case 1:
			if ( GameStatus == 1 ){
				if (Block[i].c_y + Block[i].size < Y_last && Block[i+1].c_y + Block[i+1].size < Y_last){
					if (Lv == 1){
						Block[i].c_y += 20;
					}
					else if (Lv == 2){
						Block[i].c_y += 20;
						Block[i + 1].c_y += 20;
					}
					if ( Lv == 1){
						for (int j = 0; j < i; ++j){
							if (InCircle(Block[j].c_x, Block[j].c_y, Block[i].c_x, Block[i].c_y, 20)){
								col = TRUE;
								break;
							}
						}
					}
					else if (Lv == 2){
						for (int j = 0; j < i + 1; ++j){
							if (InCircle(Block[j].c_x, Block[j].c_y, Block[i + 1].c_x, Block[i + 1].c_y, 20)){
								col = TRUE;
								break;
							}
						}
					}
				}
				if (col == TRUE){
					if (Lv == 1){
						Block[i].c_y -= 20;
					}
					else if (Lv == 2){
						Block[i].c_y -= 20;
						Block[i + 1].c_y -= 20;
					}
					if (Lv ==2){
						i += 2;
					} else 
						++i;
					col = FALSE;
					KillTimer(hWnd, 1);
				}
				if (Lv == 1){
					if (Block[i].c_y + Block[i].size >= Y_last){
						if (i == 0){
							i += 2;
						}
						else{
							++i;
						}
						KillTimer(hWnd, 1);
					}
				}
				else if (Lv == 2){
					if (Block[i+1].c_y + Block[i+1].size >= Y_last){
						i += 2;
						KillTimer(hWnd, 1);
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 2:
			
			break;
		default:
			break;
		}
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_HOME_START:
			GameStatus = 1;
			break;
		case ID_HOME_PAUSE:
			GameStatus = 2;
			break;
		case ID_HOME_END:
			GameStatus = 2;
			wsprintf(str, "Score : %d", score);
			MessageBox(hWnd, "Score", str, MB_OK);
			break;
		case ID_HOME_EXIT:
			answer = MessageBox(hWnd, "종료하시겠습니까?", "Exit", MB_OKCANCEL);
			
			if ( answer ==  IDOK){
				PostQuitMessage(0);
			}
			break;
		case ID_PLANE_15:
			Plane = 0;
			Y_last = 400;
			X_last = 300;
			break;
		case ID_PLANE_18:
			Plane = 1;
			Y_last = 460;
			X_start = 80;
			X_last = 320;
			break;
		case ID_PLANE_20:
			Plane = 2;
			Y_last = 500;
			X_start = 80;
			X_last = 360;
			break;
		case ID_LEVEL_1:
			Lv = 1;
			break;
		case ID_LEVEL_2:
			Lv = 2;
			break;
		case ID_LEVEL_3:
			Lv = 3;
			break;
		case ID_COLOR_4:
			R_num = 4;
			break;
		case ID_COLOR_5:
			R_num = 5;
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		// Make Plane
		for (GridX = X_start; GridX <= X_last; GridX += 20){
			MoveToEx(hdc, GridX, 100, NULL);
			LineTo(hdc, GridX, Y_last);
		}
		for (GridY = 100; GridY <= Y_last; GridY += 20){
			MoveToEx(hdc, X_start, GridY, NULL);
			LineTo(hdc, X_last, GridY);
		}
		
		// Draw Stone
		for (int j = 0; j <= i+1; ++j){
			if (Lv == 1){
				hBrush = CreateSolidBrush(Block[j].rgb);
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, Block[j].c_x - Block[j].size, Block[j].c_y - Block[j].size
					, Block[j].c_x + Block[j].size, Block[j].c_y + Block[j].size);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
			else if (Lv == 2){
				hBrush = CreateSolidBrush(Block[j].rgb);
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, Block[j].c_x - Block[j].size, Block[j].c_y - Block[j].size
					, Block[j].c_x + Block[j].size, Block[j].c_y + Block[j].size);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush(Block[j+1].rgb);
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, Block[j+1].c_x - Block[j+1].size, Block[j+1].c_y - Block[j+1].size
					, Block[j+1].c_x + Block[j+1].size, Block[j+1].c_y + Block[j+1].size);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}
		
		// Socre Board
		hBrush = CreateSolidBrush(RGB(0,255,255));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 500, 280, 600, 320);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		rect.left = 500;
		rect.top = 280;
		rect.right = 600;
		rect.bottom = 320;
		wsprintf(str, "Score : %d", score);
		SetBkColor(hdc, RGB(0,255,255));
		DrawText(hdc, str, strlen(str), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		// Next Block


		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if ( wParam == VK_LEFT){
			if (Lv == 1){
				Block[i].c_x -= 20;
			}
			else if (Lv == 2){
				Block[i].c_x -= 20;
				Block[i+1].c_x -= 20;
			}
			if (Block[i].c_x <= X_start){
				if (Lv == 1){
					Block[i].c_x += 20;
				}
				else if (Lv == 2){
					Block[i].c_x += 20;
					Block[i+1].c_x += 20;
				}
			}
			if (Lv == 1){
				for (int j = 0; j < i; ++j){
					if (InCircle(Block[j].c_x, Block[j].c_y, Block[i].c_x, Block[i].c_y, 20)){
						Block[i].c_x += 20;
						break;
					}
				}
			}
			else if (Lv == 2){
				for (int j = 0; j < i+1; ++j){
					if (InCircle(Block[j].c_x, Block[j].c_y, Block[i+1].c_x, Block[i+1].c_y, 20)){
						Block[i].c_x += 20;
						Block[i + 1].c_x += 20;
						break;
					}
				}
			}
		}
		else if ( wParam == VK_RIGHT){
			if (Lv == 1){
				Block[i].c_x += 20;
			}
			else if (Lv == 2){
				Block[i].c_x += 20;
				Block[i + 1].c_x += 20;
			}
			if (Block[i].c_x >= X_last){
				if (Lv == 1){
					Block[i].c_x -= 20;
				}
				else if (Lv == 2){
					Block[i].c_x -= 20;
					Block[i+1].c_x -= 20;
				}
				
			}
			if (Lv == 1){
				for (int j = 0; j < i; ++j){
					if (InCircle(Block[j].c_x, Block[j].c_y, Block[i].c_x, Block[i].c_y, 20)){
						Block[i].c_x -= 20;
						break;
					}
				}
			}
			else if (Lv == 2){
				for (int j = 0; j < i + 1; ++j){
					if (InCircle(Block[j].c_x, Block[j].c_y, Block[i + 1].c_x, Block[i + 1].c_y, 20)){
						Block[i].c_x -= 20;
						Block[i + 1].c_x -= 20;
						break;
					}
				}
			}
		}
		else if (wParam == VK_DOWN){
			if (Lv == 1){
				Block[i].c_y = Y_last - 10;
			}
			else if (Lv == 2){
				for (int j = 0; j < i; ++j){
					if (InCircle(Block[j].c_x, Block[j].c_y, Block[i].c_x, Block[i].c_y, 20)){
						Block[i].c_y = Y_last - 30;
					}
					else{
						Block[i].c_y = Y_last - 10;
					}
				}
				for (int j = 0; j < i + 1; ++j){
					if (InCircle(Block[j].c_x, Block[j].c_y, Block[i + 1].c_x, Block[i + 1].c_y, 20)){
						Block[i + 1].c_y = Y_last - 10;
					}
					else {
						Block[i + 1].c_y = Y_last - 10;
					}
				}
			}
			if (Lv == 1){
				for (int j = 0; j < i; ++j){
					if (InCircle(Block[j].c_x, Block[j].c_y, Block[i].c_x, Block[i].c_y, 20)){
						Block[i].c_y -= 20;
					}
				}
			}
			else if (Lv == 2){	
				for (int j = 0; j < i+1; ++j){
					if (InCircle(Block[j].c_x, Block[j].c_y, Block[i+1].c_x, Block[i+1].c_y, 20)){
						Block[i].c_y -= 40;
						Block[i+1].c_y -= 40;
					}
				}
			}
		}
		break;
	case WM_CHAR:
		if ( wParam == 'r' || wParam == 'R'){
			if (Lv == 2){
				if (R_count == 0){
					Block[i + 1].c_x = Block[i].c_x + 20;
					Block[i + 1].c_y = Block[i].c_y;
					R_count = 1;
				}
				else if ( R_count == 1){
					Block[i + 1].c_x = Block[i].c_x;
					Block[i + 1].c_y = Block[i].c_y - 20;
					R_count = 2;
				}
				else if ( R_count == 2){
					Block[i + 1].c_x = Block[i].c_x - 20;
					Block[i + 1].c_y = Block[i].c_y;
					R_count = 3;
				}
				else if ( R_count == 3){
					Block[i + 1].c_x = Block[i].c_x;
					Block[i + 1].c_y = Block[i].c_y + 20;
					R_count = 0;
				}
				
			}
			else if (Lv == 3){

			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
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