#include <Windows.h>

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
}SHAPE;

SHAPE shape[4];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습 문제 7 - 1";

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hinstance;
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = ChildWndProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window";
	RegisterClass(&WndClass);

	hWnd = CreateWindow("Window Class Name",
		"Window Class Name",
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

HWND hWndChild[4];
int speedX = 10, speedY = 10;
int BoundaryX, BoundaryY;

RECT rectView;

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR hCursor1, hCursor2;
	static BOOL split;

	switch (uMsg)
	{
	case WM_CREATE:
		split = FALSE;
		GetClientRect(hWnd, &rectView);

		hWndChild[0] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window", NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, rectView.right/2 -1, rectView.bottom/2-1
			, hWnd, NULL, g_hinst, NULL);

		shape[0].c_x = 50;
		shape[0].c_y = 50;
		
		hWndChild[1] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rectView.right/2+1, 0, rectView.right/ 2 - 1, rectView.bottom / 2 - 1
			, hWnd, NULL, g_hinst, NULL);

		shape[1].c_x = (rectView.right/2-1) + 50;
		shape[1].c_y = 50;

		hWndChild[2] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, rectView.bottom/2+1, rectView.right / 2 - 1, rectView.bottom / 2 - 1
			, hWnd, NULL, g_hinst, NULL);
		
		shape[2].c_x = 50;
		shape[2].c_y = (rectView.bottom/2-1) + 50;

		hWndChild[3] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rectView.right/2+1, rectView.bottom/2+1, rectView.right / 2 - 1, rectView.bottom / 2 - 1
			, hWnd, NULL, g_hinst, NULL);

		shape[3].c_x = (rectView.right / 2 - 1) + 50;
		shape[3].c_y = (rectView.bottom / 2 - 1) + 50;

		for (int i = 0; i < 4; ++i)
			shape[i].size = 20;

		BoundaryX = rectView.bottom / 2;
		BoundaryY = rectView.right / 2;

		hCursor1 = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
		hCursor2 = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
		break;
	case WM_LBUTTONDOWN:
		if (split == TRUE){
			SetCursor(hCursor1);
			SetCursor(hCursor2);
			SetCapture(hWnd);
		}
		break;
	case WM_LBUTTONUP:
		if (split == TRUE) ReleaseCapture();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc[4];
	PAINTSTRUCT ps;
	RECT rectView[4];
	static int mx, my, Move;

	switch (uMsg)
	{
	case WM_CREATE:
		for (int i = 0; i < 4; ++i){
			GetClientRect(hWndChild[i], &rectView[i]);
		}
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			shape[0].c_x += speedX;
			shape[0].c_y += speedY;
			if (shape[0].c_x <= 0){
				speedX *= -1;
			}
			if (shape[0].c_x >= (rectView[0].right)){
				speedX *= -1;
			}
			if (shape[0].c_y <= 0){
				speedY *= -1;
			}
			if (shape[0].c_y >= (rectView[0].bottom)){
				speedY *= -1;
			}
			InvalidateRect(hWndChild[0], NULL, TRUE);
			break;
		case 1:
			shape[1].c_x += speedX;
			shape[1].c_y += speedY;
			if (shape[1].c_x <= rectView[0].right){
				speedX *= -1;
			}
			if (shape[1].c_x >= (rectView[1].right)){
				speedX *= -1;
			}
			if (shape[1].c_y <= 0){
				speedY *= -1;
			}
			if (shape[1].c_y >= (rectView[1].bottom)){
				speedY *= -1;
			}
			InvalidateRect(hWndChild[1], NULL, TRUE);
			break;
		case 2:
			shape[2].c_x += speedX;
			shape[2].c_y += speedY;
			if (shape[2].c_x <= 0){
				speedX *= -1;
			}
			if (shape[2].c_x >= (rectView[2].right)){
				speedX *= -1;
			}
			if (shape[2].c_y <= rectView[0].bottom){
				speedY *= -1;
			}
			if (shape[2].c_y >= (rectView[2].bottom)){
				speedY *= -1;
			}
			InvalidateRect(hWndChild[2], NULL, TRUE);
			break;
		case 3:
			shape[3].c_x += speedX;
			shape[3].c_y += speedY;
			if (shape[3].c_x <= rectView[0].right){
				speedX *= -1;
			}
			if (shape[3].c_x >= (rectView[3].right)){
				speedX *= -1;
			}
			if (shape[3].c_y <= rectView[1].bottom){
				speedY *= -1;
			}
			if (shape[3].c_y >= (rectView[3].bottom)){
				speedY *= -1;
			}
			InvalidateRect(hWndChild[3], NULL, TRUE);
			break;
		}
		break;
	case WM_PAINT:
		hdc[0] = BeginPaint(hWndChild[0], &ps);
		hdc[1] = BeginPaint(hWndChild[1], &ps);
		hdc[2] = BeginPaint(hWndChild[2], &ps);
		hdc[3] = BeginPaint(hWndChild[3], &ps);
		
		Ellipse(hdc[0], shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size
			, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);

		Ellipse(hdc[1], shape[1].c_x - shape[1].size, shape[1].c_y - shape[1].size
			, shape[1].c_x + shape[1].size, shape[1].c_y + shape[1].size);

		Ellipse(hdc[2], shape[2].c_x - shape[2].size, shape[2].c_y - shape[2].size
			, shape[2].c_x + shape[2].size, shape[2].c_y + shape[2].size);

		Ellipse(hdc[3], shape[3].c_x - shape[3].size, shape[3].c_y - shape[3].size
			, shape[3].c_x + shape[3].size, shape[3].c_y + shape[3].size);

		EndPaint(hWndChild[0], &ps);
		EndPaint(hWndChild[1], &ps);
		EndPaint(hWndChild[2], &ps);
		EndPaint(hWndChild[3], &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if ( Move == 0){
			if (0 < mx || mx < rectView[0].right || 0 < my || my < rectView[0].bottom){
				SetTimer(hWndChild[0], 0, 100, NULL);
				InvalidateRect(hWndChild[0], NULL, TRUE);
				Move = 1;
				break;
			}
		}
		if ( Move == 1){
			if (rectView[0].right < mx || mx < rectView[1].right || 0 < my || my < rectView[1].bottom){
				SetTimer(hWndChild[1], 1, 100, NULL);
				InvalidateRect(hWndChild[1], NULL, TRUE);
				Move = 2;
				break;
			}
		}
		if ( Move == 2){
			if (rectView[1].right < mx || mx < rectView[2].right || rectView[1].bottom < my || my < rectView[2].bottom){
				SetTimer(hWndChild[2], 2, 100, NULL);
				InvalidateRect(hWndChild[2], NULL, TRUE);
				Move = 3;
				break;
			}
		}
		if ( Move == 3){
			if (rectView[2].right < mx || mx < rectView[3].right || rectView[1].bottom < my || my < rectView[3].bottom){
				SetTimer(hWndChild[3], 3, 100, NULL);
				InvalidateRect(hWndChild[3], NULL, TRUE);
				Move = 0;
				break;
			}
		}
		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hWnd, uMsg, wParam, lParam);
}

/*
7 - 3
4개로 분할된 윈도우에서 작업하기

실습 7 - 2를 이용하여 2개의 자식 윈도우에서는 문서 작업을, 2개의 자식 윈도우에서는 그리기 작업을
하도록 한다.

그리기는 왼쪽 마우스를 드래그 하면 자유그리기가 이루어진다.
문자쓰기는 마우스를 눌러 차일드 윈도우를 활성화 시킨후 문자를 입력한다.

특정 윈도우 활성화 (SetFocus (hWND hWnd)) 함수 사용

문자 그림
그림 문자

*/