#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <time.h>

#define THREAD_NUM 100

HINSTANCE g_hinst;
HWND hWnd;
LPCTSTR lpszClass = "연습 문제 10 - 2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
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

void ThreadProc(POINT *nowshape)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT Ell = *((POINT *)nowshape);
	static int count;
	srand((unsigned)time(NULL));
	hdc = BeginPaint(hWnd, &ps);


	while (1){
		SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
		Ellipse(hdc, Ell.x, Ell.y, Ell.x + 60, Ell.y + 60);
		Sleep(1000);
	}
	EndPaint(hWnd, &ps);
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThread[THREAD_NUM];
	static POINT Ell;
	static RECT rectView;
	static int speed_x, speed_y;
	static int count;


	switch (uMsg) {
	case WM_CREATE:
		speed_x = 20;
		speed_y = 20;
		GetClientRect(hWnd, &rectView);
		SetTimer(hWnd, 0, 100, NULL);
		break;
	case WM_LBUTTONDOWN:
		// x, y 좌표 받아오기
		Ell.x = rand() % 700;
		Ell.y = rand() % 500;
		break;
	case WM_TIMER:
		// 자동이동
		Ell.x += speed_x;
		Ell.y += speed_y;
		
		// 충돌체크
		if ( Ell.x < rectView.left - 30 || Ell.x > rectView.right + 30){
			speed_x *= -1;
		}
		if ( Ell.y < rectView.top - 30 || Ell.y > rectView.bottom + 30){
			speed_y *= -1;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		// 스레드 호출
		if (count < THREAD_NUM){
			hThread[count] = (HANDLE)_beginthreadex(NULL, 0,
				(unsigned int(__stdcall*)(void *))ThreadProc,
				(POINT *)&Ell, 0, NULL);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0);
		// 스레드 지우기
		for (int i = 0; i < THREAD_NUM; ++i)
			CloseHandle(hThread[i]);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

