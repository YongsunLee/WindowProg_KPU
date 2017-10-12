#include <Windows.h>
#include <math.h>

#define IDC_BUTTON_R 100
#define IDC_BUTTON_EXIT 101
#define IDC_BUTTON_BIG 102
#define IDC_BUTTON_MID 103
#define IDC_BUTTON_SMA 104
#define IDC_SCR_1 105
#define IDC_SCR_2 106

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
}SHAPE;

SHAPE shape[5];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습 문제 7 - 6";

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
	static HWND hButton[2], hRadio[3], hScroll[2];
	static int scro1, scro2, TempPos;
	static double seta[4], speed1, speed2, move_x[4], move_y[4];


	switch (uMsg) {
	case WM_CREATE:
		hButton[0] = CreateWindow("Button", "Reverse", WS_CHILD | WS_VISIBLE | WS_BORDER, 570, 500, 100, 30, hWnd, (HMENU)IDC_BUTTON_R, g_hinst, NULL);
		hButton[1] = CreateWindow("Button", "EXIT", WS_CHILD | WS_VISIBLE | WS_BORDER, 680, 500, 100, 30, hWnd, (HMENU)IDC_BUTTON_EXIT, g_hinst, NULL);
		hRadio[0] = CreateWindow("Button", "Big", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600 , 390, 100, 30, hWnd, (HMENU)IDC_BUTTON_BIG, g_hinst, NULL);
		hRadio[1] = CreateWindow("Button", "Midium", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600, 420, 100, 30, hWnd, (HMENU)IDC_BUTTON_MID, g_hinst, NULL);
		hRadio[2] = CreateWindow("Button", "Small", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 600, 450, 100, 30, hWnd, (HMENU)IDC_BUTTON_SMA, g_hinst, NULL);
		hScroll[0] = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 570, 10, 200, 20, hWnd, (HMENU)IDC_SCR_1, g_hinst, NULL);
		hScroll[1] = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 570, 40, 200, 20, hWnd, (HMENU)IDC_SCR_2, g_hinst, NULL);
		
		SetScrollRange(hScroll[0], SB_CTL, 0,  255, TRUE);
		SetScrollPos(hScroll[0], SB_CTL, 0, TRUE);

		SetScrollRange(hScroll[1], SB_CTL, 0, 255, TRUE);
		SetScrollPos(hScroll[1], SB_CTL, 0, TRUE);
		
		shape[0].c_x = 280;
		shape[0].c_y = 280;
		shape[0].size = 150;
		shape[1].c_x = 280;
		shape[1].c_y = 280;
		shape[1].size = 60;

		shape[2].c_x = 130;
		shape[2].c_y = 280;
		shape[2].size = 20;
		shape[3].c_x = 220;
		shape[3].c_y = 280;
		shape[3].size = 20;

		speed1 = 0.01;
		speed2 = 0.03;

		SetTimer(hWnd, 0, 100, NULL);
		break;
	case WM_TIMER:
		switch (LOWORD(wParam))
		{
		case 0:
			for (int i = 2; i < 4; ++i){
				move_x[i] = (shape[i].c_x + shape[i].size) * sin(seta[i]);
				move_y[i] = (shape[i].c_y + shape[i].size) * cos(seta[i]);
				if (i == 2){
					seta[i] += speed1;
				}
				else if (i == 3){
					seta[i] += speed2;
				}
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_HSCROLL:
		if ((HWND)lParam == hScroll[0]) TempPos = scro1;
		if ((HWND)lParam == hScroll[1]) TempPos = scro2;
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			TempPos = max(0, TempPos - 1);
			break;
		case SB_LINERIGHT:
			TempPos = min(255, TempPos + 1);
			break;
		case SB_PAGELEFT:
			TempPos = max(0, TempPos - 10);
			break;
		case SB_PAGERIGHT:
			TempPos = min(255, TempPos + 10);
			break;
		case SB_THUMBTRACK:
			TempPos = HIWORD(wParam);
			break;
		}

		if ((HWND)lParam == hScroll[0]) scro1 = TempPos;
		if ((HWND)lParam == hScroll[1]) scro2 = TempPos;
		SetScrollPos((HWND)lParam, SB_CTL, TempPos, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// 뒷 배경 원
		Ellipse(hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size
			, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);

		Ellipse(hdc, shape[1].c_x - shape[1].size, shape[1].c_y - shape[1].size
			, shape[1].c_x + shape[1].size, shape[1].c_y + shape[1].size);

		//  움직이는 원
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, (shape[2].c_x + move_x[2]) - shape[2].size, (shape[2].c_y + move_y[2]) - shape[2].size,
			(shape[2].c_x + move_x[2]) + shape[2].size, (shape[2].c_y + move_y[2]) + shape[2].size);
		SelectObject(hdc, oldBrush);

		hBrush = CreateSolidBrush(RGB(255, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, (shape[3].c_x + move_x[3]) - shape[3].size, (shape[3].c_y + move_y[3]) - shape[3].size,
			(shape[3].c_x + move_x[3]) + shape[3].size, (shape[3].c_y + move_y[3]) + shape[3].size);
		SelectObject(hdc, oldBrush);


		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}