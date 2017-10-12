#include <Windows.h>
#include "resource.h"

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습 문제 6 - 6";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DiaLogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	switch (uMsg) {
	case WM_CREATE:
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DiaLogProc);
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK DiaLogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hList_Name, hList_Num, hList_Sex, hList_Year, hCombo;
	static int selection, Sex;
	static char name[20], number[20];
	char sex[][20] = {"여자", "남자"};
	char year[5][20] = { "1990", "1991", "1992", "1993", "1994" };

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO_YEAR);
		hList_Name = GetDlgItem(hDlg, IDC_LIST_NAME);
		hList_Num = GetDlgItem(hDlg, IDC_LIST_NUMBER);
		hList_Sex = GetDlgItem(hDlg, IDC_LIST_SEX);
		hList_Year = GetDlgItem(hDlg, IDC_LIST_YEAR);

		for (int i = 0; i < 5; ++i){
			SendMessage(hCombo, CB_ADDSTRING, i, (LPARAM)year[i]);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_FEMALE:
			Sex = 0;
			break;
		case IDC_MALE:
			Sex = 1;
			break;
		case IDC_COMBO_YEAR:
			if ( HIWORD(wParam) == CBN_SELCHANGE){
				selection = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			}
			break;
		case IDC_NEW:
			SetDlgItemText(hDlg, IDC_EDIT_NAME, "");
			SetDlgItemText(hDlg, IDC_EDIT_NUMBER, "");
			break;
		case IDC_INSERT:
			GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
			if ( strcmp(name, "")){
				SendMessage(hList_Name, LB_ADDSTRING, 0, (LPARAM)name);
			}
			GetDlgItemText(hDlg, IDC_EDIT_NUMBER, number, 20);
			if ( strcmp(number, "")){
				SendMessage(hList_Num, LB_ADDSTRING, 0, (LPARAM)number);
			}
			SendMessage(hList_Sex, LB_ADDSTRING, 0, (LPARAM)sex[Sex]);
			SendMessage(hList_Year, LB_ADDSTRING, 0, (LPARAM)year[selection]);
			break;
		case IDC_DELETE:
			SendMessage(hList_Name, LB_DELETESTRING, selection, 0);
			SendMessage(hList_Num, LB_DELETESTRING, selection, 0);
			SendMessage(hList_Sex, LB_DELETESTRING, selection, 0);
			SendMessage(hList_Year, LB_DELETESTRING, selection, 0);
			break;
		case IDC_EXIT:
			EndDialog(hDlg,  0);
			PostQuitMessage(0);
			break;
		case IDC_LIST_NAME:
			if (HIWORD(wParam) == LBN_SELCHANGE){
				selection = SendMessage(hList_Name, LB_GETCURSEL, 0, 0);
			}
			break;
		case IDC_LIST_NUMBER:
			if (HIWORD(wParam) == LBN_SELCHANGE){
				selection = SendMessage(hList_Num, LB_GETCURSEL, 0, 0);
			}
			break;
		case IDC_LIST_SEX:
			if (HIWORD(wParam) == LBN_SELCHANGE){
				selection = SendMessage(hList_Sex, LB_GETCURSEL, 0, 0);
			}
			break;
		case IDC_LIST_YEAR:
			if (HIWORD(wParam) == LBN_SELCHANGE){
				selection = SendMessage(hList_Year, LB_GETCURSEL, 0, 0);
			}
			break;
		}
		break;
	}
	return false;
}