#include <Windows.h>

typedef struct{
	int type;
	POINT point[3];
	BOOL draw;
	int r, g, b;
}SHAPE;

SHAPE shape[6] = {};

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
	HPEN hPen, oldPen;
	static int select;
	static int r, g, b;
	static int i;

	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_CHAR:
		hdc = GetDC(hWnd);
		hPen = CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		if (wParam == '1'){
			if (shape[0].type == 1){
				Ellipse(hdc, shape[0].point[0].x, shape[0].point[0].y,
					shape[0].point[1].x, shape[0].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (shape[0].type == 2){
				Polygon(hdc, shape[0].point, 3);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (shape[0].type == 3){
				Rectangle(hdc, shape[0].point[0].x, shape[0].point[0].y,
					shape[0].point[1].x, shape[0].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			select = 1;
		}
		else if (wParam == '2'){
			if (shape[1].type == 1){
				Ellipse(hdc, shape[1].point[0].x, shape[1].point[0].y,
					shape[1].point[1].x, shape[1].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (shape[1].type == 2){
				Polygon(hdc, shape[1].point, 3);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (shape[1].type == 3){
				Rectangle(hdc, shape[1].point[0].x, shape[1].point[0].y,
					shape[1].point[1].x, shape[1].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			select = 2;
		}
		else if (wParam == '3'){
			if (shape[2].type == 1){
				Ellipse(hdc, shape[2].point[0].x, shape[2].point[0].y,
					shape[2].point[1].x, shape[2].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (shape[2].type == 2){
				Polygon(hdc, shape[2].point, 3);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (shape[2].type == 3){
				Rectangle(hdc, shape[2].point[0].x, shape[2].point[0].y,
					shape[2].point[1].x, shape[2].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			select = 3;
		}
		else if (wParam == '4'){
			if (shape[3].type == 1){
				Ellipse(hdc, shape[3].point[0].x, shape[3].point[0].y,
					shape[3].point[1].x, shape[3].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (shape[3].type == 2){
				Polygon(hdc, shape[3].point, 3);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (shape[3].type == 3){
				Rectangle(hdc, shape[3].point[0].x, shape[3].point[0].y,
					shape[3].point[1].x, shape[3].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			select = 4;
		}
		else if (wParam == '5'){
			if (shape[4].type == 1){
				Ellipse(hdc, shape[4].point[0].x, shape[4].point[0].y,
					shape[4].point[1].x, shape[4].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (shape[4].type == 2){
				Polygon(hdc, shape[4].point, 3);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (shape[4].type == 3){
				Rectangle(hdc, shape[4].point[0].x, shape[4].point[0].y,
					shape[4].point[1].x, shape[4].point[1].y);
				SelectObject(hdc, oldPen);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			select = 5;
		}

		DeleteObject(hPen);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 5; ++i){
			if (shape[i].type == 1){
				hBrush = CreateSolidBrush(RGB(shape[i].r, shape[i].g, shape[i].b));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, shape[i].point[0].x, shape[i].point[0].y, shape[i].point[1].x, shape[i].point[1].y);
				SelectObject(hdc, oldBrush);
			}
			else if (shape[i].type == 2){
				hBrush = CreateSolidBrush(RGB(shape[i].r, shape[i].g, shape[i].b));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Polygon(hdc, shape[i].point, 3);
				SelectObject(hdc, oldBrush);
			}
			else if (shape[i].type == 3){
				hBrush = CreateSolidBrush(RGB(shape[i].r, shape[i].g, shape[i].b));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, shape[i].point[0].x, shape[i].point[0].y, shape[i].point[1].x, shape[i].point[1].y);
				SelectObject(hdc, oldBrush);
			}
		}

		//DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (shape[i].draw == FALSE){
			if (wParam == 'e' || wParam == 'E'){
				shape[i].draw = TRUE;
				shape[i].type = 1;
				shape[i].point[0].x = rand() % 700;
				shape[i].point[0].y = rand() % 560;
				shape[i].point[1].x = rand() % 700;
				shape[i].point[1].y = rand() % 560;
				shape[i].r = rand() % 255;
				shape[i].g = rand() % 255;
				shape[i].b = rand() % 255;
				++i;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == 't' || wParam == 'T'){
				shape[i].draw = TRUE;
				shape[i].type = 2;
				shape[i].point[0].x = rand() % 700;
				shape[i].point[0].y = rand() % 560;
				shape[i].point[1].x = rand() % 700;
				shape[i].point[1].y = rand() % 560;
				shape[i].point[2].x = rand() % 700;
				shape[i].point[2].y = rand() % 560;
				shape[i].r = rand() % 255;
				shape[i].g = rand() % 255;
				shape[i].b = rand() % 255;
				++i;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == 'r' || wParam == 'R'){
				shape[i].draw = TRUE;
				shape[i].type = 3;
				shape[i].point[0].x = rand() % 700;
				shape[i].point[0].y = rand() % 560;
				shape[i].point[1].x = rand() % 700;
				shape[i].point[1].y = rand() % 560;
				shape[i].r = rand() % 255;
				shape[i].g = rand() % 255;
				shape[i].b = rand() % 255;
				++i;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			if (i == 6){
				for (int j = 0; j < 5; ++j){
					shape[j] = shape[j + 1];
				}
				i = 5;
				shape[5].draw = FALSE;
			}
		}

		switch (select)
		{
		case 1:
			if (wParam == VK_UP){
				shape[0].point[0].y--;
				shape[0].point[1].y--;
				shape[0].point[2].y--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_DOWN){
				shape[0].point[0].y++;
				shape[0].point[1].y++;
				shape[0].point[2].y++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_RIGHT){
				shape[0].point[0].x++;
				shape[0].point[1].x++;
				shape[0].point[2].x++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_LEFT){
				shape[0].point[0].x--;
				shape[0].point[1].x--;
				shape[0].point[2].x--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case 2:
			if (wParam == VK_UP){
				shape[1].point[0].y--;
				shape[1].point[1].y--;
				shape[1].point[2].y--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_DOWN){
				shape[1].point[0].y++;
				shape[1].point[1].y++;
				shape[1].point[2].y++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_RIGHT){
				shape[1].point[0].x++;
				shape[1].point[1].x++;
				shape[1].point[2].x++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_LEFT){
				shape[1].point[0].x--;
				shape[1].point[1].x--;
				shape[1].point[2].x--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case 3:
			if (wParam == VK_UP){
				shape[2].point[0].y--;
				shape[2].point[1].y--;
				shape[2].point[2].y--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_DOWN){
				shape[2].point[0].y++;
				shape[2].point[1].y++;
				shape[2].point[2].y++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_RIGHT){
				shape[2].point[0].x++;
				shape[2].point[1].x++;
				shape[2].point[2].x++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_LEFT){
				shape[2].point[0].x--;
				shape[2].point[1].x--;
				shape[2].point[2].x--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case 4:
			if (wParam == VK_UP){
				shape[3].point[0].y--;
				shape[3].point[1].y--;
				shape[3].point[2].y--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_DOWN){
				shape[3].point[0].y++;
				shape[3].point[1].y++;
				shape[3].point[2].y++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_RIGHT){
				shape[3].point[0].x++;
				shape[3].point[1].x++;
				shape[3].point[2].x++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_LEFT){
				shape[3].point[0].x--;
				shape[3].point[1].x--;
				shape[3].point[2].x--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case 5:
			if (wParam == VK_UP){
				shape[4].point[0].y--;
				shape[4].point[1].y--;
				shape[4].point[2].y--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_DOWN){
				shape[4].point[0].y++;
				shape[4].point[1].y++;
				shape[4].point[2].y++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_RIGHT){
				shape[4].point[0].x++;
				shape[4].point[1].x++;
				shape[4].point[2].x++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (wParam == VK_LEFT){
				shape[4].point[0].x--;
				shape[4].point[1].x--;
				shape[4].point[2].x--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}