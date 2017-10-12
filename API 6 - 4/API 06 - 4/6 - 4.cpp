#include <Windows.h>
#include "resource.h"

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
}SHAPE;

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 6 - 4";

HWND hButton;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	hButton = hWnd;

	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		break;
	case WM_LBUTTONDOWN:
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC DLG_hdc;
	PAINTSTRUCT ps;
	static HBRUSH hBrush;
	static char name[20];
	static char color[8][20] = { { "Block" }, { "White" }, { "Cyan" }, { "Pink" },
								{ "Red" }, { "Green" }, { "Blue" }, { "Yellow" } };
	static HWND hCombo, hList;
	static int selection;
	static RECT rect;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO1);
		hList = GetDlgItem(hDlg, IDC_LIST1);
		GetClientRect(hDlg, &rect);
		for (int i = 0; i < 8; ++i)
			SendMessage(hList, LB_INSERTSTRING, i, (LPARAM)color[i]);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_COMBO1:
			if (HIWORD(wParam) == CBN_SELCHANGE){
				selection = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			}
			break;
		case IDC_LIST1:
			if (HIWORD(wParam) == CBN_SELCHANGE){
				selection = SendMessage(hList, LB_GETCURSEL, 0, 0);
			}
			break;
		case IDC_BUTTON1:
			GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
			if (strcmp(name, "")){
				SendMessage(hCombo, CB_INSERTSTRING, 0, (LPARAM)name);
			}
			SendMessage(hCombo, CB_SETCURSEL, selection, 0);
			break;
		case IDC_BUTTON2:
			GetDlgItemText(hDlg, IDC_COMBO1, name, 20);
			SetDlgItemText(hDlg, IDC_EDIT1, name);
			break;
		case IDC_BUTTON3:
			if ( selection == 0 ){
				hBrush = CreateSolidBrush(RGB(0,0,0));
			}
			else if ( selection == 1){
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
			}
			else if (selection == 2){
				hBrush = CreateSolidBrush(RGB(0, 255, 255));
			}
			else if (selection == 3){
				hBrush = CreateSolidBrush(RGB(255, 0, 255));
			}
			else if (selection == 4){
				hBrush = CreateSolidBrush(RGB(255, 0, 0));
			}
			else if (selection == 5){
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
			}
			else if (selection == 6){
				hBrush = CreateSolidBrush(RGB(255, 0, 0));
			}
			else if (selection == 7){
				hBrush = CreateSolidBrush(RGB(255, 255, 0));
			}
			InvalidateRect(hDlg, NULL, TRUE);
			break;
		case IDC_BUTTON4:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_PAINT:
		DLG_hdc = BeginPaint(hDlg, &ps);
		
		FillRect(DLG_hdc, &rect, hBrush);
		
		EndPaint(hDlg, &ps);
		break;
	}
	return false;
}