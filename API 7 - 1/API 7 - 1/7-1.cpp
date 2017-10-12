#include <Windows.h>
#include "resource.h"

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

HWND hWndChild[50];
int ID = 0;

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hWndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;
	
	switch (uMsg)
	{
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hWnd), 0);
		clientcreate.idFirstChild = 100;

		hWndClient = CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, g_hinst, (LPSTR)&clientcreate);
		ShowWindow(hWndClient, SW_SHOW);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEW:
			mdicreate.szClass = "Child Window";
			mdicreate.szTitle = "Title";
			mdicreate.hOwner = g_hinst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;

			hWndChild[ID] = (HWND)SendMessage(hWndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			ID++;
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefFrameProc(hWnd, hWndClient, uMsg, wParam, lParam);
}

char str[50][10][100];
int yPos[50], count[50];

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SIZE size;
	
	switch (uMsg)
	{
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		for (int j = 0; j <= ID; ++j){
			GetTextExtentPoint(hdc, str[j][yPos[j]], strlen(str[j][yPos[j]]), &size);
			for (int i = 0; i <= yPos[j]; ++i){
				if(hWnd == hWndChild[j]){
				TextOut(hdc, 0, i * 20, str[j][i], strlen(str[j][i]));
				}
			}
			SetCaretPos(size.cx, yPos[j] * 20);
		}
		
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		for (int i = 0; i <= ID; ++i){
			if (hWnd == hWndChild[i]){
				str[i][yPos[i]][count[i]++] = wParam;
				if (count[i] > 98){
					str[i][yPos[i]][count[i]] = NULL;
					yPos[i]++;
					count[i] = 0;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN){
			for (int i = 0; i <= ID; ++i){
				if (hWnd == hWndChild[i]){
					if (yPos[i] != 9){
						str[i][yPos[i]][count[i]] = NULL;
						yPos[i]++;
						count[i] = 0;
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		return 0;
	}
	return DefMDIChildProc(hWnd, uMsg, wParam, lParam);
}
