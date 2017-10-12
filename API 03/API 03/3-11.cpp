#include <Windows.h>
#include <math.h>

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
	int coll;
	BOOL Check;
}SHAPE;

float LengthPts(int, int, int, int);
BOOL InCircle(int x, int y, int mx, int my, float rad);

SHAPE rect[1];
SHAPE circle[1];
SHAPE block[50];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 3 - 11";

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
	static int endX, endY, oldX, oldY;
	static int Block_x, Block_y;
	static int b_count, rb_count;
	static int speedX, speedY;
	static BOOL Drag, Check, GameStatus;
	static char GameMessage[100];

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		SetTimer(hWnd, 1, 100, NULL);
		SetTimer(hWnd, 2, 5000, NULL);
		SetTimer(hWnd, 3, 30000, NULL);
		oldX = rect[0].c_x = 400;
		oldY = rect[0].c_y = 530;
		rect[0].size = 10;
		circle[0].c_x = 400;
		circle[0].c_y = 510;
		circle[0].size = 10;
		circle[0].draw = TRUE;
		Drag = FALSE;
		Check = FALSE;
		GameStatus = TRUE;
		Block_x = 77;
		Block_y = 33;
		speedX = 10;
		speedY = 10;
		rb_count = 0;
		b_count = 50;
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
			for (int i = 0; i < 50; ++i){
				if (block[i].draw == TRUE){
					if (((circle[0].c_x + circle[0].size) >= block[i].c_x - 30 && (circle[0].c_x + circle[0].size) <= block[i].c_x + 30) &&
						((circle[0].c_y + circle[0].size) >= block[i].c_y - 20 && (circle[0].c_y + circle[0].size) <= block[i].c_y + 20) ||
						((circle[0].c_x - circle[0].size) <= block[i].c_x + 30 && (circle[0].c_x - circle[0].size) >= block[i].c_x - 30) &&
						((circle[0].c_y - circle[0].size) <= block[i].c_y + 20 && (circle[0].c_y - circle[0].size) >= block[i].c_y - 20)){
						speedY *= -1;
						block[i].coll += 1;
					}
				}
				if (block[i].coll == 1){
					block[i].r = 255;
					block[i].g = 0;
					block[i].b = 0;
				}
				else if (block[i].coll == 2){
					block[i].draw = FALSE;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 1:
			for (int i = 0; i < 50; ++i){
				if (block[i].Check == FALSE){
					block[i].Check = TRUE;
					block[i].draw = TRUE;
					block[i].c_x = Block_x;
					block[i].c_y = Block_y;
					Block_x += 70;
					block[i].r = 0;
					block[i].g = 255;
					block[i].b = 0;
					if ((i + 1) % 10 == 0){
						Block_y += 50;
						Block_x = 77;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 2:
			speedX *= 2;
			speedY *= 2;
			if (speedX >= 60){
				speedX = speedX / 4;
				speedY = speedY / 4;
			}
			break;
		case 3:
			KillTimer(hWnd, 0);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			GameStatus = FALSE;
			for (int i = 0; i < 50; ++i){
				if (block[i].r == 255){
					rb_count++;
				}
				if (block[i].draw == FALSE){
					b_count--;
					rb_count--;
				}
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 50; ++i){
			if (block[i].draw == TRUE){
				hBrush = CreateSolidBrush(RGB(block[i].r, block[i].g, block[i].b));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, block[i].c_x - 30, block[i].c_y - 20, block[i].c_x + 30, block[i].c_y + 20);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}

		Rectangle(hdc, rect[0].c_x - 30, rect[0].c_y - rect[0].size, rect[0].c_x + 30, rect[0].c_y + rect[0].size);

		if (circle[0].draw == TRUE){
			Ellipse(hdc, circle[0].c_x - circle[0].size, circle[0].c_y - circle[0].size
				, circle[0].c_x + circle[0].size, circle[0].c_y + circle[0].size);
		}

		if (circle[0].c_y >= 550){
			circle[0].draw = TRUE;
			circle[0].c_x = 400;
			circle[0].c_y = 250;
		}

		if (GameStatus == FALSE){
			wsprintf(GameMessage, "게임이 종료되었습니다! %d %d", b_count, rb_count);
			TextOut(hdc, 330, 220, GameMessage, strlen(GameMessage));
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

			Rectangle(hdc, rect[0].c_x - 30, rect[0].c_y - rect[0].size, rect[0].c_x + 30, rect[0].c_y + rect[0].size);

			Rectangle(hdc, endX - 30, endY - rect[0].size, endX + 30, endY + rect[0].size);

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
			speedX *= 2;
			speedY *= 2;
		}
		else if (wParam == '-'){
			speedX /= 2;
			speedY /= 2;
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
