#include <Windows.h>
#include <math.h>

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL eat_PC;
	BOOL eat_NPC;
	BOOL eat_NPC1;
	BOOL draw;
	int r, g, b;
}SHAPE;

float LengthPts(int, int, int, int);
BOOL InCircle(int x, int y, int mx, int my, float rad);

SHAPE PC_man[2];
SHAPE NPC_man[4];
SHAPE Food[20];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 4 - 4";

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
	static int shape;
	static int GridX, GridY;
	static int direct, c_X, c_Y, NPC_cx, NPC_cy, NPC_rad, rad, c_rad;
	static int NPC_cx1, NPC_cy1, NPC_rad1;

	switch (uMsg) {
	case WM_CREATE:
		// 먹이 생성
		SetTimer(hWnd, 0, 100, NULL);
		// NPC 이동
		SetTimer(hWnd, 1, 100, NULL);
		// 팩맨 모양
		SetTimer(hWnd, 2, 200, NULL);
		// 팩맨 이동
		SetTimer(hWnd, 3, 200, NULL);
		// 초기 팩맨 좌표
		PC_man[0].point[0].x = 30; PC_man[0].point[0].y = 30;
		PC_man[0].point[1].x = 70; PC_man[0].point[1].y = 70;
		PC_man[0].point[2].x = 70; PC_man[0].point[2].y = 20;
		PC_man[0].point[3].x = 70; PC_man[0].point[3].y = 60;

		PC_man[1].point[0].x = 30; PC_man[1].point[1].x = 70;
		PC_man[1].point[0].y = 30; PC_man[1].point[1].y = 70;
		PC_man[1].point[2].x = 70; PC_man[1].point[2].y = 30;
		PC_man[1].point[3].x = 70; PC_man[1].point[3].y = 40;
		PC_man[0].r = 255; PC_man[0].g = 255; PC_man[0].b = 255;

		NPC_man[0].point[0].x = 680; NPC_man[0].point[0].y = 480;
		NPC_man[0].point[1].x = 720; NPC_man[0].point[1].y = 520;
		NPC_man[0].point[2].x = 680; NPC_man[0].point[2].y = 510;
		NPC_man[0].point[3].x = 680; NPC_man[0].point[3].y = 480;

		NPC_man[1].point[0].x = 680; NPC_man[1].point[0].y = 480;
		NPC_man[1].point[1].x = 720; NPC_man[1].point[1].y = 520;
		NPC_man[1].point[2].x = 680; NPC_man[1].point[2].y = 500;
		NPC_man[1].point[3].x = 680; NPC_man[1].point[3].y = 490;
		NPC_man[0].r = 255; NPC_man[0].g = 255; NPC_man[0].b = 255;

		shape = 1;
		NPC_man[2].draw = FALSE;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			for (int i = 0; i < 20; ++i){
				if (Food[i].draw == FALSE){
					Food[i].eat_PC = FALSE;
					Food[i].eat_NPC = FALSE;
					Food[i].eat_NPC1 = FALSE;
					Food[i].draw = TRUE;
					Food[i].c_x = rand()%15 * 50;
					Food[i].c_y = rand()%12 * 50;
					Food[i].size = 10;
					Food[i].r = rand() % 255; Food[i].g = rand() % 255; Food[i].b = rand() % 255;
				}

				// PC 먹이 충돌
				c_X = (PC_man[0].point[1].x + PC_man[0].point[0].x) / 2;
				c_Y = (PC_man[0].point[1].y + PC_man[0].point[0].y) / 2;

				rad = LengthPts(c_X, c_Y, PC_man[0].point[1].x, PC_man[0].point[1].y);

				if (InCircle(c_X, c_Y, Food[i].c_x, Food[i].c_y, rad)){
					Food[i].eat_PC = TRUE;
				}

				if (Food[i].eat_PC == TRUE){
					Food[i].draw = FALSE;
					PC_man[0].r = Food[i].r, PC_man[0].g = Food[i].g, PC_man[0].b = Food[i].b;
					PC_man[1].r = Food[i].r, PC_man[1].g = Food[i].g, PC_man[1].b = Food[i].b;
				}

				// NPC1 먹이 충돌
				NPC_cx = (NPC_man[0].point[1].x + NPC_man[0].point[0].x)/2;
				NPC_cy = (NPC_man[0].point[1].y + NPC_man[0].point[0].y)/2;
				
				NPC_rad = LengthPts(NPC_cx, NPC_cy, NPC_man[0].point[0].x, NPC_man[0].point[0].y);

				if (InCircle(NPC_cx,NPC_cy, Food[i].c_x, Food[i].c_y, NPC_rad)){
					Food[i].eat_NPC = TRUE;
				}

				if (Food[i].eat_NPC == TRUE){
					Food[i].draw = FALSE;
					NPC_man[0].r = Food[i].r, NPC_man[0].g = Food[i].g, NPC_man[0].b = Food[i].b;
					NPC_man[1].r = Food[i].r, NPC_man[1].g = Food[i].g, NPC_man[1].b = Food[i].b;
				}

				// NPC2 먹이 충돌
				NPC_cx1 = (NPC_man[2].point[1].x + NPC_man[2].point[0].x) / 2;
				NPC_cy1 = (NPC_man[2].point[1].y + NPC_man[2].point[0].y) / 2;

				NPC_rad1 = LengthPts(NPC_cx1, NPC_cy1, NPC_man[0].point[0].x, NPC_man[0].point[0].y);

				if (InCircle(NPC_cx1, NPC_cy1, Food[i].c_x, Food[i].c_y, NPC_rad)){
					Food[i].eat_NPC1 = TRUE;
				}

				if (Food[i].eat_NPC1 == TRUE){
					Food[i].draw = FALSE;
					NPC_man[2].r = Food[i].r, NPC_man[2].g = Food[i].g, NPC_man[2].b = Food[i].b;
					NPC_man[3].r = Food[i].r, NPC_man[3].g = Food[i].g, NPC_man[3].b = Food[i].b;
				}

				// NPC PC 충돌
				if (NPC_man[2].draw == FALSE){
					if (InCircle(c_X, c_Y, NPC_cx, NPC_cy, rad)){
						NPC_man[2].draw = TRUE;
						NPC_man[3].draw = TRUE;

						// NPC 2 초기좌표
						NPC_man[2].point[0].x = NPC_man[0].point[0].x; NPC_man[2].point[0].y = NPC_man[0].point[0].y;
						NPC_man[2].point[1].x = NPC_man[0].point[1].x; NPC_man[2].point[1].y = NPC_man[0].point[1].y;
						NPC_man[2].point[2].x = NPC_man[0].point[2].x; NPC_man[2].point[2].y = NPC_man[0].point[2].y;
						NPC_man[2].point[3].x = NPC_man[0].point[3].x; NPC_man[2].point[3].y = NPC_man[0].point[3].y;

						NPC_man[3].point[0].x = NPC_man[1].point[0].x; NPC_man[3].point[0].y = NPC_man[1].point[0].y;
						NPC_man[3].point[1].x = NPC_man[1].point[1].x; NPC_man[3].point[1].y = NPC_man[1].point[1].y;
						NPC_man[3].point[2].x = NPC_man[1].point[2].x; NPC_man[3].point[2].y = NPC_man[1].point[2].y;
						NPC_man[3].point[3].x = NPC_man[1].point[3].x; NPC_man[3].point[3].y = NPC_man[1].point[3].y;
						NPC_man[2].r = 255; NPC_man[2].g = 255; NPC_man[2].b = 255;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 1:
			// NPC 1
			if (PC_man[0].point[0].x >= 300){
				if (NPC_man[0].point[0].x >= PC_man[0].point[0].x){
					for (int i = 0; i < 4; ++i){
						NPC_man[0].point[i].x -= 5;
						NPC_man[1].point[i].x -= 5;
					}
				}
				else if (NPC_man[0].point[0].x <= PC_man[0].point[0].x){
					for (int i = 0; i < 4; ++i){
						NPC_man[0].point[i].x += 5;
						NPC_man[1].point[i].x += 5;
					}
				}
				if (NPC_man[0].point[0].y >= PC_man[0].point[0].y){
					for (int i = 0; i < 4; ++i){
						NPC_man[0].point[i].y -= 5;
						NPC_man[1].point[i].y -= 5;
					}
				}
				else if (NPC_man[0].point[0].y <= PC_man[0].point[0].y){
					for (int i = 0; i < 4; ++i){
						NPC_man[0].point[i].y += 5;
						NPC_man[1].point[i].y += 5;
					}
				}
			}

			// NPC 2
			if (NPC_man[3].draw == TRUE){
				if (PC_man[0].point[0].y >= 280){
					if (NPC_man[2].point[0].x >= PC_man[0].point[0].x){
						for (int i = 0; i < 4; ++i){
							NPC_man[2].point[i].x -= 5;
							NPC_man[3].point[i].x -= 5;
						}
					}
					else if (NPC_man[2].point[0].x <= PC_man[0].point[0].x){
						for (int i = 0; i < 4; ++i){
							NPC_man[2].point[i].x += 5;
							NPC_man[3].point[i].x += 5;
						}
					}
					if (NPC_man[2].point[0].y >= PC_man[0].point[0].y){
						for (int i = 0; i < 4; ++i){
							NPC_man[2].point[i].y -= 5;
							NPC_man[3].point[i].y -= 5;
						}
					}
					else if (NPC_man[2].point[0].y <= PC_man[0].point[0].y){
						for (int i = 0; i < 4; ++i){
							NPC_man[2].point[i].y += 5;
							NPC_man[3].point[i].y += 5;
						}
					}
				}
			}			
			break;
		case 2:
			if (shape == 1) {
				shape = 2;
			}
			else if (shape == 2) {
				shape = 1;
			}
			break;
		case 3:
			// 오른쪽
			if ( direct == 1){
				for (int i = 0; i < 4; ++i){
					PC_man[0].point[i].x += 25;
					PC_man[1].point[i].x += 25;
				}
			}
			// 왼쪽
			else if (direct == 2){
				for (int i = 0; i < 4; ++i){
					PC_man[0].point[i].x -= 25;
					PC_man[1].point[i].x -= 25;
				}
			}
			// 위
			else if (direct == 3){
				for (int i = 0; i < 4; ++i){
					PC_man[0].point[i].y -= 25;
					PC_man[1].point[i].y -= 25;
				}
			}
			// 아래
			else if (direct == 4){
				for (int i = 0; i < 4; ++i){
					PC_man[0].point[i].y += 25;
					PC_man[1].point[i].y += 25;
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

		//그리드 그리기
		for (GridX = 25; GridX <= 800; GridX += 25){
			for (GridY = 25; GridY <= 560; GridY += 25){
				MoveToEx(hdc, 0, GridY, NULL);
				LineTo(hdc, 800, GridY);
			}
			MoveToEx(hdc, GridX, 0, NULL);
			LineTo(hdc, GridX, 560);
		}

		// 음식 그리기
		for (int i = 0; i < 20; ++i){
			if (Food[i].draw == TRUE){
				hBrush = CreateSolidBrush(RGB(Food[i].r, Food[i].g, Food[i].b));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, Food[i].c_x - Food[i].size, Food[i].c_y - Food[i].size
							,Food[i].c_x + Food[i].size, Food[i].c_y + Food[i].size);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}

		// 팩맨 그리기, shape로 입 크기 조절
		if (shape == 1){
			hBrush = CreateSolidBrush(RGB(PC_man[0].r, PC_man[0].g, PC_man[0].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Pie(hdc, PC_man[0].point[0].x, PC_man[0].point[0].y, PC_man[0].point[1].x, PC_man[0].point[1].y
				, PC_man[0].point[2].x, PC_man[0].point[2].y, PC_man[0].point[3].x, PC_man[0].point[3].y);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			
			// NPC 1
			hBrush = CreateSolidBrush(RGB(NPC_man[0].r, NPC_man[0].g, NPC_man[0].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Pie(hdc, NPC_man[0].point[0].x, NPC_man[0].point[0].y, NPC_man[0].point[1].x, NPC_man[0].point[1].y
				, NPC_man[0].point[2].x, NPC_man[0].point[2].y, NPC_man[0].point[3].x, NPC_man[0].point[3].y);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			// NPC 2
			if (NPC_man[2].draw == TRUE){
				hBrush = CreateSolidBrush(RGB(NPC_man[2].r, NPC_man[2].g, NPC_man[2].b));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Pie(hdc, NPC_man[2].point[0].x, NPC_man[2].point[0].y, NPC_man[2].point[1].x, NPC_man[2].point[1].y
					, NPC_man[2].point[2].x, NPC_man[2].point[2].y, NPC_man[2].point[3].x, NPC_man[2].point[3].y);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}

		}
		else if (shape == 2){
			hBrush = CreateSolidBrush(RGB(PC_man[0].r, PC_man[0].g, PC_man[0].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Pie(hdc, PC_man[1].point[0].x, PC_man[1].point[0].y, PC_man[1].point[1].x, PC_man[1].point[1].y
				, PC_man[1].point[2].x, PC_man[1].point[2].y, PC_man[1].point[3].x, PC_man[1].point[3].y);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			
			hBrush = CreateSolidBrush(RGB(NPC_man[0].r, NPC_man[0].g, NPC_man[0].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Pie(hdc, NPC_man[1].point[0].x, NPC_man[1].point[0].y, NPC_man[1].point[1].x, NPC_man[1].point[1].y
				, NPC_man[1].point[2].x, NPC_man[1].point[2].y, NPC_man[1].point[3].x, NPC_man[1].point[3].y);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			if (NPC_man[3].draw == TRUE){
				hBrush = CreateSolidBrush(RGB(NPC_man[2].r, NPC_man[2].g, NPC_man[2].b));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Pie(hdc, NPC_man[3].point[0].x, NPC_man[3].point[0].y, NPC_man[3].point[1].x, NPC_man[3].point[1].y
					, NPC_man[3].point[2].x, NPC_man[3].point[2].y, NPC_man[3].point[3].x, NPC_man[3].point[3].y);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}
		
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		// 오른쪽
		if (wParam == VK_RIGHT){
			// 팩맨 모양
			PC_man[0].point[2].x = PC_man[0].point[1].x;
			PC_man[0].point[2].y = PC_man[0].point[1].y - 50;
			PC_man[0].point[3].x = PC_man[0].point[1].x;
			PC_man[0].point[3].y = PC_man[0].point[1].y - 10;

			PC_man[1].point[2].x = PC_man[1].point[1].x;
			PC_man[1].point[2].y = PC_man[1].point[1].y - 40;
			PC_man[1].point[3].x = PC_man[1].point[1].x;
			PC_man[1].point[3].y = PC_man[1].point[1].y - 20;

			// 이동방향
			direct = 1;
		}
		// 왼쪽
		else if (wParam == VK_LEFT){
			PC_man[0].point[2].x = PC_man[0].point[0].x;
			PC_man[0].point[2].y = PC_man[0].point[0].y + 50;
			PC_man[0].point[3].x = PC_man[0].point[0].x;
			PC_man[0].point[3].y = PC_man[0].point[0].y + 10;

			PC_man[1].point[2].x = PC_man[1].point[0].x;
			PC_man[1].point[2].y = PC_man[1].point[0].y + 40;
			PC_man[1].point[3].x = PC_man[1].point[0].x;
			PC_man[1].point[3].y = PC_man[1].point[0].y + 20;

			//방향
			direct = 2;
		}
		// 위
		else if (wParam == VK_UP){
			PC_man[0].point[2].x = PC_man[0].point[1].x - 50;
			PC_man[0].point[2].y = PC_man[0].point[0].y;
			PC_man[0].point[3].x = PC_man[0].point[1].x - 10;
			PC_man[0].point[3].y = PC_man[0].point[0].y;

			PC_man[1].point[2].x = PC_man[1].point[1].x;
			PC_man[1].point[2].y = PC_man[1].point[0].y - 40;
			PC_man[1].point[3].x = PC_man[1].point[1].x;
			PC_man[1].point[3].y = PC_man[1].point[0].y - 20;

			direct = 3;
		}
		// 아래
		else if (wParam == VK_DOWN){
			PC_man[0].point[2].x = PC_man[0].point[0].x+50;
			PC_man[0].point[2].y = PC_man[0].point[1].y;
			PC_man[0].point[3].x = PC_man[0].point[0].x+10;
			PC_man[0].point[3].y = PC_man[0].point[1].y;

			PC_man[1].point[2].x = PC_man[1].point[0].x+40;
			PC_man[1].point[2].y = PC_man[1].point[1].y;
			PC_man[1].point[3].x = PC_man[1].point[0].x+20;
			PC_man[1].point[3].y = PC_man[1].point[1].y;

			direct = 4;
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
BOOL InCircle(int x, int y, int mx, int my, float rad)
{
	if (LengthPts(x, y, mx, my) < rad)
		return TRUE;
	else return FALSE;
}
