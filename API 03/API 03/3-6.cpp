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

SHAPE shape[4];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 3 - 6";

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
	static int plus_x, mx, my;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 1, NULL);
		shape[0].c_x = 100;
		shape[0].c_y = 100;
		plus_x = 100;
		break;
	case WM_TIMER:
		for (int i = 0; i < 4; ++i){
			if (i == 0){
				plus_x = 0;
			}
			else if (i == 1){
				plus_x = 100;
			}
			else if (i == 2){
				plus_x = 200;
			}
			else if (i == 3){
				plus_x = 300;
			}
			shape[i].c_x = shape[0].c_x + plus_x;
			shape[i].c_y = shape[0].c_y;
			shape[i].size = 50;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 4; ++i){
			hBrush = CreateSolidBrush(RGB(shape[i].r, shape[i].g, shape[i].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, shape[i].c_x - shape[i].size, shape[i].c_y - shape[i].size,
				shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
			SelectObject(hdc, oldBrush);
		}

		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (int i = 0; i < 4; ++i){
			if (InCircle(shape[i].c_x, shape[i].c_y, mx, my, shape[i].size)){
				if (shape[i].draw == FALSE){
					if (i == 0){
						shape[i].r = 255;
						shape[i].g = 0;
						shape[i].b = 0;
						shape[i].draw = TRUE;
					}
					else if (i == 1){
						shape[i].r = 0;
						shape[i].g = 255;
						shape[i].b = 0;
						shape[i].draw = TRUE;
					}
					else if (i == 2){
						shape[i].r = 0;
						shape[i].g = 0;
						shape[i].b = 255;
						shape[i].draw = TRUE;
					}
					else if (i == 3){
						shape[i].r = rand() % 255;
						shape[i].g = rand() % 255;
						shape[i].b = rand() % 255;
						shape[i].draw = TRUE;
					}
				}
				else if (shape[i].draw == TRUE){
					shape[i].r = 0;
					shape[i].g = 0;
					shape[i].b = 0;
					shape[i].draw = FALSE;
				}

			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		for (int i = 0; i < 4; ++i){
			if (shape[i].draw == FALSE){
				if (i == 0 && (wParam == 'r' || wParam == 'R')){
					shape[i].r = 255;
					shape[i].g = 0;
					shape[i].b = 0;
					shape[i].draw = TRUE;
				}
				else if (i == 1 && (wParam == 'g' || wParam == 'G')){
					shape[i].r = 0;
					shape[i].g = 255;
					shape[i].b = 0;
					shape[i].draw = TRUE;
				}
				else if (i == 2 && (wParam == 'b' || wParam == 'B')){
					shape[i].r = 0;
					shape[i].g = 0;
					shape[i].b = 255;
					shape[i].draw = TRUE;
				}
				else if (i == 3 && (wParam == 'a' || wParam == 'A')){
					shape[i].r = rand() % 255;
					shape[i].g = rand() % 255;
					shape[i].b = rand() % 255;
					shape[i].draw = TRUE;
				}
			}
			else if (shape[i].draw == TRUE){
				shape[i].r = 0;
				shape[i].g = 0;
				shape[i].b = 0;
				shape[i].draw = FALSE;
			}
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


/*
3 - 11
공 튀기기 프로그램을 이용하여 벽돌깨기 게임 만들기

화면 내용 상단에 5*10 개의 벽돌이 있다.
화면의 하단에 바가 있고 마우스를 이용하여 바를 움직인다.
마우스를 누르면 마우스의 x위치로 이동

공이 튀기면서 벽돌에 한번 닿으면 벽돌의 색이 바뀐다.
공이 튀기면서 벽돌에 2번 닿으면 벽돌이 없어진다.
색이 변한 벽돌의 개수와 없어진 개수를 화면에 출력한다.
시간이 지나면 공의 이동속도가 늘어난다.
벽돌이 모두 없어지면 게임이 종료된다. (타이머 종료)

- 테스트를 위해서 5초마다 공속도가 바뀌고 게임이 30초 후에 종료되게 만드는 것-

*/