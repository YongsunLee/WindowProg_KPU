#include <Windows.h>

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 3 - 4";

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
	RECT rect_cat, rect_mouse;
	static int x1, y1, x2, y2;
	static BOOL Selection;
	static int Move_x, Move_y;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 1, NULL);
		x1 = 100;
		y1 = 100;
		x2 = 0;
		y2 = 0;
		Selection = FALSE;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (Selection == TRUE){
				if (x1 < Move_x){
					x1++;
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else if (x1 > Move_x){
					x1--;
					InvalidateRect(hWnd, NULL, TRUE);
				}
				if (y1 < Move_y){
					y1++;
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else if (y1 > Move_y){
					y1--;
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		rect_cat.left = x1 - 10;
		rect_cat.top = y1 - 10;
		rect_cat.right = x1 + 40;
		rect_cat.bottom = y1 + 10;

		DrawText(hdc, "고양이", 6, &rect_cat, DT_SINGLELINE | DT_VCENTER);
		if (Selection == TRUE){
			rect_mouse.left = x2 - 10;
			rect_mouse.top = y2 - 10;
			rect_mouse.right = x2 + 10;
			rect_mouse.bottom = y2 + 10;
			DrawText(hdc, "쥐", 2, &rect_mouse, DT_SINGLELINE | DT_VCENTER);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		Move_x = LOWORD(lParam);
		Move_y = HIWORD(lParam);
		x2 = Move_x;
		y2 = Move_y;
		Selection = TRUE;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		Move_x = LOWORD(lParam);
		Move_y = HIWORD(lParam);
		if (Selection){
			x2 = Move_x;
			y2 = Move_y;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*
3-7
풍선 바람넣기 게임 만들기

화면의 임의의 위치에 10개의 풍선을 그리고,  풍선을 위쪽은 위쪽으로 올라가고 있다.
풍선은 원과 삼각형가 직선을 이용하여 구현한다.
위의 가장자리에 도달하면 화면 밖으로 나가고 다시 아래에서 새로운 풍선이 그려진다.
마우스로 풍선 내부를 클릭하고 있으면 풍선의 크기가 커진다.
일정 크기 이상이 되면 풍선이 터진다.
풍선이 터지면 다른곳에 새로운 풍선이 그려진다.

*/