#include <Windows.h>
#include <time.h>


typedef struct{
	BOOL draw;
	int type;
	POINT point[2];
	//int x, y;
	int direct;
	DWORD color;
}SHAPE;

SHAPE shape[100] = {};

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
	static int r, g, b;
	static int i, j;
	static int sec;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		SetTimer(hWnd, 1, 200, NULL);
		srand(time(NULL));
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			for (i = 0; i < 100; ++i){
				if (shape[i].draw == TRUE){
					switch (shape[i].direct)
					{
					case 1:
						shape[i].point[0].x++;
						shape[i].point[1].x++;
						break;
					case 2:
						shape[i].point[0].x--;
						shape[i].point[1].x--;
						break;
					case 3:
						shape[i].point[0].y++;
						shape[i].point[1].y++;
						break;
					case 4:
						shape[i].point[0].y--;
						shape[i].point[1].y--;
						break;
					}
					if (shape[i].point[0].x < 0 || shape[i].point[0].x > 800 || shape[i].point[0].y < 0 || shape[i].point[0].y > 560
						|| shape[i].point[1].x < 0 || shape[i].point[1].x > 800 || shape[i].point[1].y < 0 || shape[i].point[1].y > 560){
						shape[i].draw = FALSE;
					}
				}
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case 1:
			for (i = 0; i < 100; ++i){
				if (shape[i].draw == FALSE){
					shape[i].draw = TRUE;
					shape[i].type = rand() % 2 + 1;
					shape[i].direct = rand() % 4 + 1;
					shape[i].color = RGB(rand() % 255, rand() % 255, rand() % 255);
					shape[i].point[0].x = rand() % 700;
					shape[i].point[0].y = rand() % 500;
					shape[i].point[1].x = shape[i].point[0].x + rand() % 70;
					shape[i].point[1].y = shape[i].point[0].y + rand() % 50;
					break;
				}
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (i = 0; i < 100; ++i){
			if (shape[i].draw == TRUE){

				hBrush = CreateSolidBrush(shape[i].color);
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

				if (shape[i].type == 1){
					Ellipse(hdc, shape[i].point[0].x, shape[i].point[0].y,
						shape[i].point[1].x, shape[i].point[1].y);
				}
				else if (shape[i].type == 2){
					Rectangle(hdc, shape[i].point[0].x, shape[i].point[0].y,
						shape[i].point[1].x, shape[i].point[1].y);
				}

				if (i % 3 == 0){
					KillTimer(hWnd, 1);
					sec = rand() % 500;
					SetTimer(hWnd, 1, sec, NULL);
				}

				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0);
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*
3 -3
궤도를 따라 공전하는 원 만들기
화면을 사드운 하여 각 등분의 중앙에 빨간색 원을 그린다.
각각의 빨간색 원을 중심으로 그 주위를 임의의 반지름을 가진 원의 궤도를 그리고
그 궤도를 따라 다양한 크기와 색의 원이 다양한 타이머에 따라 회전한다. 회전 방향은 시계방향과 반시계방향이 모두 있다.
'q'를 입력하면 프로그램이 종료된다.

#include <math.h>
x = Rcos(double 세타)
y = Rsin(double 형식의 각도)
라디안이므로 360도가 아니라 2파이 즉, 3.14로 계산

시계방향 = 각도가 -
반시계 방향 =각도가 +

원점이 0,0이 아니므로 원점 좌표를 계산해서 더한후 계산한다.
*/