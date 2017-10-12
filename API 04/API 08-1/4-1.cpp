#include <Windows.h>
#include <math.h>
#include "resource.h"

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

SHAPE rect[1];
SHAPE circle[1];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 4 - 1";

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static int endX, endY, oldX, oldY;
	static int speedX, speedY;
	static int GridX, GridY;
	static BOOL Drag, Check, Grid, GameStatus;

	switch (uMsg) {
	case WM_CREATE:
		oldX = rect[0].c_x = 400;
		oldY = rect[0].c_y = 530;
		rect[0].size = 10;

		circle[0].c_x = 400;
		circle[0].c_y = 510;
		circle[0].size = 10;
		circle[0].r = 255; circle[0].g = 255; circle[0].b = 255;
		circle[0].draw = TRUE;
		Drag = FALSE;
		Check = FALSE;
		speedX = 10;
		speedY = 10;

		GridX = 40;
		Grid = FALSE;

		
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_GAME_START:
			SetTimer(hWnd, 0, 100, NULL);
			break;
		case ID_GAME_END:
			KillTimer(hWnd, 0);
			break;
		case ID_SPEED_SLOW:
			speedX /= 2;
			speedY /= 2;
			break;
		case ID_SPEED_MEDIUM:
			speedX *= 2;
			speedY *= 2;
			break;
		case ID_SPEED_FAST:
			speedX *= 4;
			speedY *= 4;
			break;
		case ID_BALL_BIG:
			circle[0].size = 20;
			break;
		case ID_BALL_SMALL:
			circle[0].size = 5;
			break;
		case ID_COLOR_RED:
			circle[0].r = 255; circle[0].g = 0; circle[0].b = 0;
			break;
		case ID_COLOR_GREEN:
			circle[0].r = 0; circle[0].g = 255; circle[0].b = 0;
			break;
		case ID_COLOR_BLUE:
			circle[0].r = 0; circle[0].g = 0; circle[0].b = 255;
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
		case 0:
			circle[0].c_x -= speedX;
			circle[0].c_y -= speedY;
			if (circle[0].c_x <= 0){
				circle[0].c_x = 0;
				speedX *= -1;
			}
			if (circle[0].c_x >= 770){
				circle[0].c_x = 770;
				speedX *= -1;
			}
			if (circle[0].c_y <= 0){
				circle[0].c_y = 0;
				speedY *= -1;
			}

			if ((circle[0].c_x >= rect[0].c_x - 30 && circle[0].c_x <= rect[0].c_x + 60) &&
				(circle[0].c_y >= rect[0].c_y - 10 && circle[0].c_y <= rect[0].c_y + 10)){
				speedY *= -1;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (Grid == TRUE){
			for (GridX = 40; GridX <= 800; GridX += 40){
				for (GridY = 20; GridY <= 540; GridY += 20){
					MoveToEx(hdc, 0, GridY, NULL);
					LineTo(hdc, 800, GridY);
				}
				MoveToEx(hdc, GridX, 0, NULL);
				LineTo(hdc, GridX, 540);
			}
		}
		
		Rectangle(hdc, rect[0].c_x - 30, rect[0].c_y - rect[0].size
			, rect[0].c_x + 30, rect[0].c_y + rect[0].size);

		if (circle[0].draw == TRUE){
			hBrush = CreateSolidBrush(RGB(circle[0].r, circle[0].g, circle[0].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, circle[0].c_x - circle[0].size, circle[0].c_y - circle[0].size
				, circle[0].c_x + circle[0].size, circle[0].c_y + circle[0].size);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		if (circle[0].c_y >= 550){
			circle[0].draw = TRUE;
			circle[0].c_x = 400;
			circle[0].c_y = 250;
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		Drag = TRUE;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Drag = FALSE;
		if (Check == TRUE){
			rect[0].c_x = oldX;
			rect[0].c_y = oldY;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (Drag){
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			Rectangle(hdc, rect[0].c_x - 30, rect[0].c_y - rect[0].size
				, rect[0].c_x + 30, rect[0].c_y + rect[0].size);

			Rectangle(hdc, endX - 30, endY - rect[0].size
				, endX + 30, endY + rect[0].size);

			if (rect[0].c_y >= 530 || endY >= 530){
				rect[0].c_y = 530;
				endY = 530;
			}
			rect[0].c_x = endX; rect[0].c_y = endY;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_CHAR:
		if (wParam == '+'){
			speedX += 10;
			speedY += 10;
		}
		else if (wParam == '-'){
			speedX -= 10;
			speedY -= 10;
		}
		else if (wParam == 'm' || wParam == 'M'){
			Check = FALSE;
		}
		else if (wParam == 's' || wParam == 'S'){
			Check = TRUE;
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


/*
4 - 4

증식하는 팩맨

자신의 팩맨과 NPC팩맨이 있어야 한다.
일정영역의 위치에 들어가게 되면 NPC 팩맨이 자신을 따라오게 된다.
만약에 두개의 팩맨이 부딪히게 되면 새로운 NPC 팩맨이 생성되게 된다.

먹이는 색상이 랜덤으로 뿌려지게 되고
팩맨이 그 먹이를 먹게되면 그 색상으로 변하게 된다. (NPC도 동일)

메일 제목 : 자기 이름 연습문제 번호
akkmh@naver.com
dongminchoi90@gmail.com


*/