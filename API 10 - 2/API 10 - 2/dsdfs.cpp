#include<windows.h>
#include<stdio.h>
#include<time.h>
#include<process.h>
#define MAX 10
HINSTANCE g_hInst;
HWND hWnd;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void ThreadProc(POINT* nowPoint)
{
	PAINTSTRUCT ps;
	HDC hdc = NULL;
	int i;
	POINT p = *((POINT *)nowPoint);
	srand((unsigned)time(0));
	hdc = BeginPaint(hWnd, &ps);

	while (1) {
		SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
		Ellipse(hdc, p.x, p.y, p.x + 20, p.y + 20);
		Sleep(1000);
	}
	EndPaint(hWnd, &ps);
	return;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	static char szClassName[] = "API";
	static char szTitle[] = "API 연습문제";

	MSG msg;
	WNDCLASSEX wc;
	g_hInst = hInstance;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName =//MAKEINTRESOURCE(IDR_MENU1);         
		wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	hWnd = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW, 100, 0, 800, 680, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return(FALSE);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static POINT p;
	static HANDLE hThread[MAX];
	static int count;
	static RECT rectView;
	static int dir_X, dir_Y;
	int i;
	switch (uMsg)
	{
	case WM_CREATE:
		dir_X = 1;
		dir_Y = 1;
		GetClientRect(hWnd, &rectView);
		SetTimer(hWnd, 1, 50, NULL);
		break;
	case WM_LBUTTONDOWN:
		p.x = rand() % 1000;
		p.y = rand() % 800;
		break;
	case WM_TIMER:
		p.x += 20 * dir_X;
		p.y += 20 * dir_Y;
		if (p.x < rectView.left + 10 || p.x > rectView.right - 10)
			dir_X *= -1;
		if (p.y < rectView.top + 10 || p.y > rectView.bottom - 10)
			dir_Y *= -1;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		if (count < MAX)
		{
			hThread[count] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void *))ThreadProc, (POINT *)&p, 0, NULL);
			//   Sleep(500);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		for (i = 0; i<MAX; i++)
			CloseHandle(hThread[i]);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}