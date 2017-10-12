#include <Windows.h>

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
	RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;
	static char str[100];
	static int yPos, xPos;
	static int count;
	char zero = '0';
	int i, j;

	switch (uMsg) {
	case WM_CREATE:
		count = 0;
		xPos = 350;
		yPos = 250;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, xPos, yPos, str, strlen(str));
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP){
			yPos = yPos - 20;
		}
		else if (wParam == VK_DOWN){
			yPos = yPos + 20;
		}
		else if (wParam == VK_RIGHT){
			xPos = xPos + 40;
		}
		else if (wParam == VK_LEFT){
			xPos = xPos - 40;
		}
		if (yPos <= 0){
			yPos = 10;
		}
		else if (yPos >= 550) {
			yPos = 540;
		}
		else if (xPos <= 0) {
			xPos = 10;
		}
		else if (xPos >= 640) {
			xPos = 630;
		} if (wParam == VK_CONTROL){
			for (i = 0; i < count; ++i){
				if (str[i] >= 65 && str[i] <= 90){
					str[i] += 32;
				}
				else if (str[i] >= 97 && str[i] <= 122){
					str[i] -= 32;
				}
			}
		}
		else if (wParam == VK_F1){
			for (i = strlen(str) + 5; i > 4; --i){
				str[i] = str[i - 3];
			}
			str[i] = zero;
			str[i + 1] = zero;
			str[i + 2] = zero;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK){
			for (j = 0; j < count; ++j){
				str[j] = str[j + 1];
			}
			if (count < 1){
				yPos--;
				count = strlen(str) - 1;
			}
			if (count == 0 && yPos == 0)
				break;
			str[count++] = wParam;
		}
		else if (wParam == '/'){
			if (count == 10){
				yPos = yPos + 1;
				count = 0;
				count = strlen(str);
			}
		}
		hdc = GetDC(hWnd);
		str[count++] = wParam;
		str[count] = '\0';
		if (count > 99){
			count--;
		}
		xPos = rand() % 700;
		yPos = rand() % 600;
		InvalidateRect(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}