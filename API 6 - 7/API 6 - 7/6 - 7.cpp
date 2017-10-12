#include <Windows.h>
#include <stdio.h>
#include "resource.h"

#pragma warning (disable : 4996)

#define SIZE 100

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습 문제 6 - 7";

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

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

HBITMAP hBit_tile1, hBit_tile2, hBit_tile3, oldBit_tile1, oldBit_tile2, oldBit_tile3;
HWND Win_hWnd;
static int sel, x, y, mx, my;
static int GridX, Max_x, Size_x, GridY, Max_y, Size_y, CountGrid_x, CountGrid_y;
int Map[SIZE][SIZE];
BOOL Grid_On, Read;
FILE *fp;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hDlg = NULL;
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	Win_hWnd = hWnd;

	switch (uMsg) {
	case WM_CREATE:
		hBit_tile1 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit_tile2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBit_tile3 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3));
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_MENU_READ:
			if ( (fp = fopen("Map.txt", "r")) == NULL ){
				fprintf(stderr, "파일 읽기 에러");
				exit(1);
			}

			fscanf(fp, "%d %d", &Max_x, &Max_y);
			fscanf(fp, "%d %d", &Size_x, &Size_y);

			for (int i = 0; i < Max_y / Size_y; ++i){
				for (int j = 0; j < Max_x / Size_x; ++j){
					fscanf(fp, "%d", &Map[i][j]);
				}
			}

			Grid_On = TRUE;
			fclose(fp);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MENU_SAVE:
			if ((fp = fopen("Map.txt", "w")) == NULL){
				fprintf(stderr, "파일 읽기 에러");
				exit(1);
			}

			fprintf(fp, "%d %d\n", Max_x, Max_y);
			fprintf(fp, "%d %d\n", Size_x, Size_y);
			for (int i = 0; i < Max_y / Size_y; ++i){
				for (int j = 0; j < Max_x / Size_x; ++j){
					fprintf(fp,"%d ", Map[i][j]);
				}
				fprintf(fp, "\n");
			}

			fclose(fp);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MENU_EXIT:
			PostQuitMessage(0);
			break;
		case ID_MENU_DIALOG:
			if (!IsWindow(hDlg)){
				hDlg = CreateDialog(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DiaLogProc);
				ShowWindow(hDlg, SW_SHOW);
			}
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);

		if (Grid_On){
			for (GridX = 0; GridX <= Max_x; GridX += Size_x){
				MoveToEx(hdc, GridX, 0, NULL);
				LineTo(hdc, GridX, Max_y);
			}
			for (GridY = 0; GridY <= Max_y; GridY += Size_y){
				MoveToEx(hdc, 0, GridY, NULL);
				LineTo(hdc, Max_x, GridY);
			}
		}
		
		for (int i = 0; i < 50; ++i){
			for (int j = 0; j < 50; ++j){
				if ( Map[i][j] == 1 ){
					SelectObject(memdc, hBit_tile1);
					BitBlt(hdc, i*Size_x, j*Size_y, Size_x, Size_y, memdc, 0, 0, SRCCOPY);
				}
				else if ( Map[i][j] == 2 ){
					SelectObject(memdc, hBit_tile2);
					BitBlt(hdc, i*Size_x, j*Size_y, Size_x, Size_y, memdc, 0, 0, SRCCOPY);
				}
				else if ( Map[i][j] == 3 ){
					SelectObject(memdc, hBit_tile3);
					BitBlt(hdc, i*Size_x, j*Size_y, Size_x, Size_y, memdc, 0, 0, SRCCOPY);
				}
			}
		}
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (int i = 0; i < 50; ++i){
			for (int j = 0; j < 50; ++j){
				if (mx >= i*Size_x && mx < (i + 1)*Size_x && my >= Size_y*j && my < Size_y*(j + 1)){
					Map[i][j] = sel;
				}
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		fclose(fp);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK DiaLogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpdis;
	HDC memDC;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hBit_tile1 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit_tile2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBit_tile3 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT_X:
			Max_x = GetDlgItemInt(hDlg, IDC_EDIT_X, NULL, FALSE);
			break;
		case IDC_EDIT_Y:
			Max_y = GetDlgItemInt(hDlg, IDC_EDIT_Y, NULL, FALSE);
			break;
		case IDC_GRID_X:
			Size_x = GetDlgItemInt(hDlg, IDC_GRID_X, NULL, FALSE);
			break;
		case IDC_GRID_Y:
			Size_y = GetDlgItemInt(hDlg, IDC_GRID_Y, NULL, FALSE);
			break;
		case IDC_TILE1:
			sel = 1;
			break;
		case IDC_TILE2:
			sel = 2;
			break;
		case IDC_TILE3:
			sel = 3;
			break;
		case IDC_OK:
			Grid_On = TRUE;
			InvalidateRect(Win_hWnd, NULL, TRUE);
			break;
		case IDC_EXIT:
			DestroyWindow(hDlg);
			hDlg = NULL;
			break;
		}
		break;
	case WM_DRAWITEM:
		lpdis = (LPDRAWITEMSTRUCT)lParam;
		switch (lpdis->CtlID)
		{
		case IDC_TILE1:
			memDC = CreateCompatibleDC(lpdis->hDC);
			if ( lpdis->itemState & ODS_SELECTED){
				oldBit_tile1 = (HBITMAP)SelectObject(memDC, hBit_tile1);
			}
			else 
				oldBit_tile1 = (HBITMAP)SelectObject(memDC, hBit_tile1);
			BitBlt(lpdis->hDC, 0, 0, 300, 300, memDC, 0, 0, SRCCOPY);
			SelectObject(memDC, oldBit_tile1);
			DeleteDC(memDC);
			break;
		case IDC_TILE2:
			memDC = CreateCompatibleDC(lpdis->hDC);
			if (lpdis->itemState & ODS_SELECTED){
				oldBit_tile2 = (HBITMAP)SelectObject(memDC, hBit_tile2);
			}
			else
				oldBit_tile2 = (HBITMAP)SelectObject(memDC, hBit_tile2);
			BitBlt(lpdis->hDC, 0, 0, 300, 300, memDC, 0, 0, SRCCOPY);
			SelectObject(memDC, oldBit_tile2);
			DeleteDC(memDC);
			break;
		case IDC_TILE3:
			memDC = CreateCompatibleDC(lpdis->hDC);
			if (lpdis->itemState & ODS_SELECTED){
				oldBit_tile3 = (HBITMAP)SelectObject(memDC, hBit_tile3);
			}
			else
				oldBit_tile3 = (HBITMAP)SelectObject(memDC, hBit_tile3);
			BitBlt(lpdis->hDC, 0, 0, 300, 300, memDC, 0, 0, SRCCOPY);
			SelectObject(memDC, oldBit_tile3);
			DeleteDC(memDC);
			break;
			return TRUE;
		}
		break;
	}
	return false;
}
