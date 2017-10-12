#include <Windows.h>
#include <stdio.h>

#pragma warning (disable : 4996)

#define IDC_BUTTON 100
#define IDC_EDIT1 101
#define IDC_EDIT2 102
#define IDC_COMBO_YEAR 103
#define IDC_COMBO_MON 104
#define IDC_COMBO_DAY 105

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
	static HWND hButton, hEdit[2], hCombo[3];
	static char str[10], str2[10], year[10], mon[10], day[10];
	static int sel, num1, num2;
	static BOOL Y, M, D;

	switch (uMsg) {
	case WM_CREATE:
		hCombo[0] = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | EN_VSCROLL | ES_AUTOHSCROLL, 50, 100, 200, 300, hWnd, (HMENU)IDC_COMBO_YEAR, g_hinst, NULL);
		hCombo[1] = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | EN_VSCROLL | ES_AUTOHSCROLL, 250, 100, 200, 300, hWnd, (HMENU)IDC_COMBO_MON, g_hinst, NULL);
		hCombo[2] = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | EN_VSCROLL | ES_AUTOHSCROLL, 450, 100, 200, 300, hWnd, (HMENU)IDC_COMBO_DAY, g_hinst, NULL);
		hEdit[0] = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 100,200,200,25, hWnd, (HMENU)IDC_EDIT1, g_hinst, NULL);
		hEdit[1] = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 350,200,200,25, hWnd, (HMENU)IDC_EDIT2, g_hinst, NULL);
		hButton = CreateWindow("button", "OK", WS_CHILD | WS_VISIBLE | WS_BORDER, 280,300,100,25, hWnd, (HMENU)IDC_BUTTON, g_hinst, NULL);
		for (int i = 1990; i < 2014; ++i){
			sprintf(str, "%d", i);
			SendMessage(hCombo[0], CB_ADDSTRING, 0, (LPARAM)str);
		}
		for (int i = 1; i <= 12; ++i){
			sprintf(str, "%d", i);
			SendMessage(hCombo[1], CB_ADDSTRING, 0, (LPARAM)str);
		}
		for (int i = 1; i <= 31; ++i){
			sprintf(str, "%d", i);
			SendMessage(hCombo[2], CB_ADDSTRING, 0, (LPARAM)str);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_COMBO_YEAR:
			sel = SendMessage(hCombo[0], CB_GETCURSEL, 0, 0);
			SendMessage(hCombo[0], CB_GETLBTEXT, sel, (LPARAM)year);
			strncpy(str, year, 2);
			num1 = atoi(str);
			num2 = atoi(year);
			num1 *= 100;
			num2 -= num1;
			sprintf(year, "%d", num2);
			Y = TRUE;
			break;
		case IDC_COMBO_MON:
			sel = SendMessage(hCombo[1], CB_GETCURSEL, 0, 0);
			SendMessage(hCombo[1], CB_GETLBTEXT, sel, (LPARAM)mon);
			num1 = atoi(mon);
			num2 = SendMessage(hCombo[2], CB_GETCOUNT, 0,0);
			if (num1 == 2){
				for (num2 = 28; num2 < 32; ++num2){
					SendMessage(hCombo[2], CB_DELETESTRING, num2, 0);
				}
			}
			else if ( num2 < 31 ){
				for (int i = 29; i < 32; ++i){
					sprintf(str, "%d", i);
					SendMessage(hCombo[2], CB_ADDSTRING, 0, (LPARAM)str);
				}
			}
			if (num1 < 10){
				sprintf(mon, "0%d", num1);
			}
			else 
				sprintf(mon, "%d", num1);
			M = TRUE;
			break;
		case IDC_COMBO_DAY:
			sel = SendMessage(hCombo[2], CB_GETCURSEL, 0, 0);
			SendMessage(hCombo[2], CB_GETLBTEXT, sel, (LPARAM)day);
			num1 = atoi(day);
			if (num1 < 10){
				sprintf(day, "0%d", num1);
			}
			else
				sprintf(day, "%d", num1);
			D = TRUE;

			if (Y && M && D){
				sprintf(str, "%s%s%s", year, mon, day);
				SetDlgItemText(hWnd, IDC_EDIT1, str);
			}
			break;
		case IDC_BUTTON:
			hdc = GetDC(hWnd);
			GetDlgItemText(hWnd, IDC_EDIT1, str, 100);
			if (strcmp(str, ""))
				TextOut(hdc, 100, 400, str, strlen(str));
			GetDlgItemText(hWnd, IDC_EDIT2, str2, 100);
			if (strcmp(str2, ""))
				TextOut(hdc, 300, 400, str2, strlen(str2));
			ReleaseDC(hWnd, hdc);
			break;
		}
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*

7 - 7

2개의 차일드 윈도우를 좌 우에 만든다.
각각의 차일드 윈도우에 1개의 도형이 그려진다.

버튼 1 : 1번 도형이 앞으로 이동
버튼 2 : 2번 도형이 앞으로 이동
버튼 3 : 1번 도형이 제자리에서 점프
버튼 4 : 2번 도형이 제자리에서 점프
버튼 5 : 모든 도형이 정지
버튼 6 : 프로그램 종료

*/