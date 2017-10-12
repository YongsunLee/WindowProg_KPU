#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

#pragma warning (disable : 4996)

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4);

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
	FILE *fp;
	HDC hdc;
	PAINTSTRUCT ps;
	OPENFILENAME OFN;
	OPENFILENAME SFN;
	static int yPos, count;
	static char str[10][100] = {0, };
	static SIZE size;
	char strFile[100], lpstrFile[100] = "";
	char filter[] = "모든 파일(*.*)\0*.*\0텍스트 파일\0*.txt;*.doc\0";
	int answer;
	int write_count = 0;

	switch (uMsg) {
	case WM_CREATE:
		yPos = 0;
		count = 0;
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_FILE_OPEN:
			memset(str, 0, sizeof(str));
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 100;
			OFN.lpstrInitialDir = ".";
			if (GetOpenFileName(&OFN) != 0){
				wsprintf(strFile, "%s 파일을 여시겠습니까?", OFN.lpstrFile);
				MessageBox(hWnd, strFile, "열기 선택", MB_OK);
				
				fp = fopen(OFN.lpstrFile, "r");
				yPos = 0;
				while ( yPos < 10 && fgets(str[yPos], sizeof(str[0]), fp)){
					str[yPos][strlen(str[yPos])] = '\0';
					count = strlen(str[yPos]);

					if (str[yPos][strlen(str[yPos])-1] == 10){
						yPos++;
						count = 0;
					}
				}
				fclose(fp);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case ID_FIEL_OPEN_NEW:
			answer = MessageBox(hWnd, "새로운 작업을 시작합니다.", "새 문서 선택", MB_OKCANCEL);
			if ( answer == IDOK){
				memset(str, 0, sizeof(str));
				yPos = 0;
				count = 0;

				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case ID_SAVE:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hWnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lpstrFile;
			SFN.nMaxFile = 256;
			SFN.lpstrInitialDir = ".";
			if (GetSaveFileName(&SFN) != 0){
				wsprintf(strFile, "%s 파일에 저장하시겠습니까?", SFN.lpstrFile);
				MessageBox(hWnd, strFile, "저장하기 선택", MB_OK);

				fp = fopen(SFN.lpstrFile, "w");
				while ( write_count <= yPos && write_count < 100 ){
					fputs(str[write_count], fp);
					write_count++;
				}
				fclose(fp);
			}
			break;
		case ID_EXIT:
			answer = MessageBox(hWnd, "종료하시겠습니까?", "끝내기", MB_OKCANCEL);
			if (answer == IDOK){
				PostQuitMessage(0);
			}
			break;
		default:
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetTextExtentPoint(hdc, str[yPos], strlen(str[yPos]), &size);
		for (int i = 0; i <= yPos; ++i){
			TextOut(hdc, 0, i*20, str[i], strlen(str[i])-1);
		}
		SetCaretPos(size.cx, (yPos)*20);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK){
			if (count == 0 && yPos == 0)
				break;
			count--;
			if (count < 0){
				yPos--;
				count = strlen(str[yPos]);
			}
		}
		else if (wParam == VK_RETURN){
			str[yPos][count] = 10;
			count = 0;
			yPos = yPos + 1;
		}
		else{
			if (count >= 98){
				str[yPos][count] = 10;
				yPos++;
				count = 0;
			}
			str[yPos][count++] = wParam;
		}
		str[yPos][count] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}