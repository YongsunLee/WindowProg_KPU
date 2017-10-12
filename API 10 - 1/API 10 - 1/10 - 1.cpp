#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <time.h>

#define THREAD_NUM 100

struct Ell{
	POINT p;
	int count;
};

HINSTANCE g_hinst;
HWND hWnd;
LPCTSTR lpszClass = "연습 문제 10 - 1";

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

void ThreadProc(Ell *nowshape)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int count = 0;
	Ell shape = *((Ell *)nowshape);
	srand((unsigned)time(NULL));
	hdc = BeginPaint(hWnd, &ps);

	while (1){
		if ( shape.count != 10){
			SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
			shape.count++;
		}
		Ellipse(hdc, shape.p.x, shape.p.y, shape.p.x + 60, shape.p.y + 60);
		Sleep(1000);
	}
	EndPaint(hWnd, &ps);
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThread[THREAD_NUM];
	static int count;
	static Ell shape;
	
	switch (uMsg) {
	case WM_CREATE:
		srand((unsigned)time(NULL));
		shape.p.x = rand() % 700;
		shape.p.y = rand() % 500;
		break;
	case WM_LBUTTONDOWN:
		
		shape.p.x = rand() % 700;
		shape.p.y = rand() % 500;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		if ( count < THREAD_NUM){
			hThread[count] = (HANDLE)_beginthreadex(NULL, 0, 
												(unsigned int (__stdcall*)(void *))ThreadProc, 
												(Ell *)&shape, 0, NULL);
		}
		break;
	case WM_DESTROY:
		for (int i = 0; i < THREAD_NUM; ++i)
			CloseHandle(hThread[i]);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

