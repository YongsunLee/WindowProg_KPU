#include <Windows.h>
#include "resource.h"

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	int speed;
	BOOL draw;
	int r, g, b;
	int Dir;
}SHAPE;

SHAPE shape[3];
SHAPE Enemy[20];
SHAPE Block[150];
SHAPE bullet[100];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "HomeWork 02";

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
		1000,
		1000,
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
	static RECT rect, rect1, rect2, rcTemp;
	static BOOL MR, Shoot, Spawn, GameStatus;
	static int Win_x, Win_y, dv_x, dv_y;
	static int GridX, GridY;
	static int Dir;
	static int bullet_c = 0;
	static int E_count = 10;
	char str[10];
	int score = 0;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		SetTimer(hWnd, 1, 100, NULL);
		SetTimer(hWnd, 2, 100, NULL);
		SetTimer(hWnd, 4, 1000, NULL);
		MR = FALSE;
		// Window 값
		GetClientRect(hWnd, &rect);
		Win_x = rect.right;
		Win_y = rect.bottom;
		dv_x = Win_x / 20;
		dv_y = Win_y / 20;

		// 사각형 , 포신, 봉인사각형 좌표값
		shape[0].c_x = 515;
		shape[0].c_y = 525;
		shape[0].size = 10;
		shape[1].c_x = 515;
		shape[1].c_y = 440;
		shape[2].c_x = shape[1].c_x;
		shape[2].c_y = shape[1].c_y + 5;
		shape[2].size = 12;
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_OBSTACLE_MAKE:
			break;
		case ID_ENEMY_10:
			E_count = 10;
			SetTimer(hWnd, 3, 100, NULL);
			break;
		case ID_ENEMY_15:
			E_count = 15;
			SetTimer(hWnd, 3, 100, NULL);
			break;
		case ID_ENEMY_20:
			E_count = 20;
			SetTimer(hWnd, 3, 100, NULL);
			break;
		case ID_OBSTACLE_EXIT:
			PostQuitMessage(0);
			break;
		case ID_PLAY_START:
			GameStatus = TRUE;
			break;
		case ID_PLAY_SCORE:
			wsprintf(str, "score = %d", score);
			MessageBox(hWnd, str, "Score", MB_OK);
			break;
		case ID_PLAY_END:
			GameStatus = FALSE;
			break;
		}
		break;
	case WM_TIMER:
		switch (wParam)
		{
			// 상자 좌표 갱신
		case 0:
			rect1.left = shape[0].c_x - shape[0].size;
			rect1.top = shape[0].c_y - shape[0].size;
			rect1.right = shape[0].c_x + shape[0].size;
			rect1.bottom = shape[0].c_y + shape[0].size;

			rect2.left = shape[2].c_x - shape[2].size;
			rect2.top = shape[2].c_y - shape[2].size;
			rect2.right = shape[2].c_x + shape[2].size;
			rect2.bottom = shape[2].c_y + shape[2].size;
			break;
			// 총알생성
		case 1:
			if ( Shoot == TRUE){
				if (bullet[bullet_c].draw == FALSE){
					bullet[bullet_c].draw = TRUE;
					if (Dir == 1){
						bullet[bullet_c].c_x = shape[1].c_x;
						bullet[bullet_c].c_y = shape[1].c_y - 10;
						bullet[bullet_c].Dir = 1;
					}
					else if (Dir == 2){
						bullet[bullet_c].c_x = shape[1].c_x + 10;
						bullet[bullet_c].c_y = shape[1].c_y;
						bullet[bullet_c].Dir = 2;
					}
					else if (Dir == 3){
						bullet[bullet_c].c_x = shape[1].c_x;
						bullet[bullet_c].c_y = shape[1].c_y + 10;
						bullet[bullet_c].Dir = 3;
					}
					else if (Dir == 4){
						bullet[bullet_c].c_x = shape[1].c_x - 10;
						bullet[bullet_c].c_y = shape[1].c_y;
						bullet[bullet_c].Dir = 4;
					}
					bullet[bullet_c].size = 5;
				}
			}
			break;
		// 총알이동
		case 2:
			if ( GameStatus == TRUE){
				for (int i = 0; i < 100; ++i){
					if (bullet[i].draw == TRUE){
						if (bullet[i].Dir == 2){
							bullet[i].c_y -= 10;
						}
						else if (bullet[i].Dir == 3){
							bullet[i].c_x += 10;
						}
						else if (bullet[i].Dir == 4){
							bullet[i].c_y += 10;
						}
						else if (bullet[i].Dir == 1){
							bullet[i].c_x -= 10;
						}
						if (bullet[i].c_x >= Win_x){
							bullet[i].draw = FALSE;
						}
						if (bullet[i].c_y >= Win_y){
							bullet[i].draw = FALSE;
						}
						if (bullet[i].c_x <= 0){
							bullet[i].draw = FALSE;
						}
						if (bullet[i].c_y <= 0){
							bullet[i].draw = FALSE;
						}
					}
				}
			}
			break;
		// 적 생성
		case 3:
			for (int j = 0; j < E_count; ++j){
				if (Enemy[j].draw == FALSE){
					Enemy[j].draw = TRUE;
					Enemy[j].c_x = 950;
					Enemy[j].c_y = 50;
					Enemy[j].size = 15;
					Enemy[j].speed = rand() % 3 + 1;
					Enemy[j].Dir = rand() % 2;
					if (j >= 5){
						Enemy[j].c_x = 950;
						Enemy[j].c_y = 950;
					}
					if (j >= 10){
						Enemy[j].c_x = 50;
						Enemy[j].c_y = 950;
					}
					if ( j >= 15 ){
						Enemy[j].c_x = 70;
						Enemy[j].c_y = 50;
					}
				}
			}
			break;
		// 적이동
		case 4:
			if ( GameStatus == TRUE){
				for (int j = 0; j < E_count; ++j){
					if (Enemy[j].Dir == 0){
						Enemy[j].c_x -= Enemy[j].speed * 25;
					}
					else if (Enemy[j].Dir == 1){
						Enemy[j].c_y += Enemy[j].speed * 25;
					}
					if ( j >= 5){
						if (Enemy[j].Dir == 0){
							Enemy[j].c_x -= Enemy[j].speed * 25;
							
						}
						else if (Enemy[j].Dir == 1){
							Enemy[j].c_y -= Enemy[j].speed * 25;
							
						}
					}
					if ( j >= 10){
						if (Enemy[j].Dir == 0){
							Enemy[j].c_x += Enemy[j].speed * 25;	
						}
						else if (Enemy[j].Dir == 1){
							Enemy[j].c_y -= Enemy[j].speed * 25;
						}
					}
					if ( j >= 15 ){
						if (Enemy[j].Dir == 0){
							Enemy[j].c_x += Enemy[j].speed * 25;
							
						}
						else if (Enemy[j].Dir == 1){
							Enemy[j].c_y += Enemy[j].speed * 25;
						}
					}
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
		// Grid
		for (GridX = 0; GridX < Win_x; GridX += dv_x){
			MoveToEx(hdc, GridX, 0, NULL);
			LineTo(hdc, GridX, Win_y);
		}
		for (GridY = 0; GridY < Win_y; GridY += dv_y){
			MoveToEx(hdc, 0, GridY, NULL);
			LineTo(hdc, Win_x, GridY);
		}

		// 사각형 1
		hBrush = CreateSolidBrush(RGB(255, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size, 
			shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		// 포신
		if ( MR == FALSE ){
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			if (Dir == 2 || Dir == 4){
				Rectangle(hdc, shape[1].c_x - 5, shape[1].c_y - 5,
					shape[1].c_x + 5, shape[1].c_y + 20);
			}
			else if (Dir == 1 || Dir == 3){
				Rectangle(hdc, shape[1].c_x - 5, shape[1].c_y - 5,
					shape[1].c_x + 20, shape[1].c_y + 5);
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		// 봉인 사각형
		else if ( MR == TRUE){
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, shape[2].c_x - shape[2].size, shape[2].c_y - shape[2].size, 
				shape[2].c_x + shape[2].size, shape[2].c_y + shape[2].size);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		
		// 적
		for (int i = 0; i < E_count; ++i){
			hBrush = CreateSolidBrush(RGB(0, 255, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, Enemy[i].c_x - Enemy[i].size, Enemy[i].c_y - Enemy[i].size
				, Enemy[i].c_x + Enemy[i].size, Enemy[i].c_y + Enemy[i].size);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		// 장애물 만들기
		

		// 총알
		if (Shoot == TRUE){
			for (int i = 0; i < 100; ++i){
				if (bullet[i].draw == TRUE){
					hBrush = CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, bullet[i].c_x - bullet[i].size, bullet[i].c_y - bullet[i].size,
						bullet[i].c_x + bullet[i].size, bullet[i].c_y + bullet[i].size);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
			}
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if ( GameStatus == TRUE){
			if (wParam == VK_UP){
				shape[0].c_y -= dv_y;
				shape[1].c_y -= dv_y;
				if (IntersectRect(&rcTemp, &rect1, &rect2)){
					shape[2].c_y -= 73;
				}
			}
			else if (wParam == VK_DOWN){
				shape[0].c_y += dv_y;
				shape[1].c_y += dv_y;
				if (IntersectRect(&rcTemp, &rect1, &rect2)){
					shape[2].c_y += 73;
				}
			}
			else if (wParam == VK_LEFT){
				shape[0].c_x -= dv_x;
				shape[1].c_x -= dv_x;
				if (IntersectRect(&rcTemp, &rect1, &rect2)){
					shape[2].c_x -= 100;
				}
			}
			else if (wParam == VK_RIGHT){
				shape[0].c_x += dv_x;
				shape[1].c_x += dv_x;
				if (IntersectRect(&rcTemp, &rect1, &rect2)){
					shape[2].c_x += 100;
				}
			}
			else if (wParam == VK_RETURN){
				Shoot = TRUE;
				if (bullet_c == 99){
					bullet_c = 0;
				}
				bullet_c++;
			}
			else if (wParam == VK_SPACE){
				if (Dir == 0){
					Dir = 1;
				}
				if (Dir == 1){
					shape[1].c_x = shape[0].c_x;
					shape[1].c_y = shape[0].c_y - 45;
					Dir = 2;
				}
				else if (Dir == 2){
					shape[1].c_x = shape[0].c_x + 45;
					shape[1].c_y = shape[0].c_y;
					Dir = 3;
				}
				else if (Dir == 3){
					shape[1].c_x = shape[0].c_x;
					shape[1].c_y = shape[0].c_y + 30;
					Dir = 4;
				}
				else if (Dir == 4){
					shape[1].c_x = shape[0].c_x - 50;
					shape[1].c_y = shape[0].c_y;
					Dir = 1;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_RBUTTONDOWN:
		if ( MR == FALSE){
			MR = TRUE;
		}
		else if ( MR == TRUE){
			MR = FALSE;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}