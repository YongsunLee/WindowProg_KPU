#include <Windows.h>

HINSTANCE g_hinst;
LPCTSTR lpszClass = "Window program 1 - 2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int

	nCmdShow)
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
		1000,
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
	PAINTSTRUCT ps;
	HDC hdc;
	static char str[10][100];
	static char Estr[10][100];
	static int count = 0, yPos;
	static int yPosCopy = -1;
	static SIZE size;
	int i, j;

	switch (uMsg) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		count = 0;
		yPos = 0;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetTextExtentPoint(hdc, str[yPos], strlen(str[yPos]), &size);
		for (i = 0; i <= yPos; ++i)
			TextOut(hdc, 0, i * 20, str[i], strlen(str[i]));
		SetCaretPos(size.cx, yPos * 20);
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
			count = 0;
			yPos = yPos + 1;
		}
		else if (wParam == VK_ESCAPE){
			for (i = 0; i <= yPos; ++i){
				strcpy(Estr[i], str[i]);
			}
			yPosCopy = yPos;
			count = 0;
			yPos = 0;
		}
		else if (wParam == VK_TAB){
			if (yPosCopy >= 0){
				for (i = 0; i <= yPosCopy; ++i){
					strcpy(str[i], Estr[i]);
				}
				yPos = yPosCopy;
				count = strlen(str[yPos]);
				yPosCopy = -1;
			}
		}
		else if (wParam == '1' || wParam == '2' || wParam == '3' ||
			wParam == '4' || wParam == '5' || wParam == '6' ||
			wParam == '7' || wParam == '8' || wParam == '9' || wParam == '0'){
			count = 0;
			yPos = yPos + 1;
			str[yPos][count++] = wParam;
		}
		else if (wParam == 'q' || wParam == 'Q'){
			PostQuitMessage(0);
		}
		else{
			str[yPos][count++] = wParam;
			if (count > 98){
				yPos++;
				count = 0;
			}
		}
		if (yPos > 9) break;
		str[yPos][count] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

