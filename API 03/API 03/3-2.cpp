#include <Windows.h>

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
	static int x, y, figure;
	static int i, j, count, speed;
	HPEN hPen, oldPen;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 500, NULL);
		x = 200;
		y = 200;
		i = 10;
		count = 0;
		figure = 0;
		speed = 30;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			x += speed;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 2:
			x += speed;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 3:
			x -= speed;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 4:
			y += speed;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 5:
			y -= speed;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
			// 점프 Timer
		case 6:
			y -= i;
			if (i >= -10){
				i--;
			}
			count++;
			if (count == 21){
				KillTimer(hWnd, 6);
				SetTimer(hWnd, 2, 500, NULL);
				count = 0;	i = 10;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 7:
			x += i;
			if (i >= -10){
				i--;
			}
			count++;
			if (count == 21){
				KillTimer(hWnd, 7);
				SetTimer(hWnd, 4, 500, NULL);
				count = 0; i = 10;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 8:
			x -= i;
			if (i >= -10){
				i--;
			}
			count++;
			if (count == 21){
				KillTimer(hWnd, 8);
				SetTimer(hWnd, 5, 500, NULL);
				count = 0; i = 10;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 9:
			y += i;
			if (i >= -10){
				i--;
			}
			count++;
			if (count == 21){
				KillTimer(hWnd, 9);
				SetTimer(hWnd, 3, 500, NULL);
				count = 0; i = 10;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// 각 방향에 대한  머리 꼬리 그리기
		if (figure == 0){
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			Ellipse(hdc, x - 60, y - 20, x - 20, y + 20);
		}
		else if (figure == 1){
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, x - 60, y - 20, x - 20, y + 20);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
		}
		else if (figure == 2){
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, x - 60, y + 20, x - 20, y + 60);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			Ellipse(hdc, x - 60, y - 20, x - 20, y + 20);
		}
		else if (figure == 3){
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, x - 60, y - 60, x - 20, y - 20);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			Ellipse(hdc, x - 60, y - 20, x - 20, y + 20);
		}
		else if (figure == 4){
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			Ellipse(hdc, x - 60, y - 20, x - 20, y + 20);
		}
		// 사방 충돌 체크
		if (x <= 70){
			figure = 4;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			KillTimer(hWnd, 5);
			SetTimer(hWnd, 2, 500, NULL);
		}
		else if (x >= 740){
			figure = 1;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 4);
			KillTimer(hWnd, 5);
			SetTimer(hWnd, 3, 500, NULL);
		}
		else if (y <= 70){
			figure = 2;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 5);
			SetTimer(hWnd, 4, 500, NULL);
		}
		else if (y >= 490){
			figure = 3;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			SetTimer(hWnd, 5, 500, NULL);
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		hdc = GetDC(hWnd);
		if (wParam == VK_UP){
			figure = 3;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			SetTimer(hWnd, 5, 500, NULL);
		}
		else if (wParam == VK_DOWN){
			figure = 2;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 5);
			SetTimer(hWnd, 4, 500, NULL);
		}
		else if (wParam == VK_LEFT){
			figure = 1;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 4);
			KillTimer(hWnd, 5);
			SetTimer(hWnd, 3, 500, NULL);
		}
		else if (wParam == VK_RIGHT){
			figure = 4;
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			KillTimer(hWnd, 5);
			SetTimer(hWnd, 2, 500, NULL);
		}
		else if (wParam == VK_SPACE){
			if (figure == 4 || figure == 0){
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				SetTimer(hWnd, 6, 100, NULL);
			}
			else if (figure == 2){
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				SetTimer(hWnd, 7, 100, NULL);
			}
			else if (figure == 3){
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				SetTimer(hWnd, 8, 100, NULL);
			}
			else if (figure == 1){
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				SetTimer(hWnd, 9, 100, NULL);
			}
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_CHAR:
		if (wParam == '+'){
			speed += 10;
		}
		else if (wParam == '-'){
			speed -= 10;
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		KillTimer(hWnd, 4);
		KillTimer(hWnd, 5);
		KillTimer(hWnd, 6);
		KillTimer(hWnd, 7);
		KillTimer(hWnd, 8);
		KillTimer(hWnd, 9);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}