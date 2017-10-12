#include <Windows.h>

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 7 - 3";

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
	WndClass.lpszMenuName = NULL;
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

HWND hWndChild[4];

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rectView;

	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);

		hWndChild[0] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, rectView.right / 2 - 1, rectView.bottom / 2 - 1
			, hWnd, NULL, g_hinst, NULL);
		hWndChild[1] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rectView.right / 2 + 1, 0, rectView.right / 2 - 1, rectView.bottom / 2 - 1
			, hWnd, NULL, g_hinst, NULL);
		hWndChild[2] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, rectView.bottom / 2 + 1, rectView.right / 2 - 1, rectView.bottom / 2 - 1
			, hWnd, NULL, g_hinst, NULL);
		hWndChild[3] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rectView.right / 2 + 1, rectView.bottom / 2 + 1, rectView.right / 2 - 1, rectView.bottom / 2 - 1
			, hWnd, NULL, g_hinst, NULL);

		SetFocus(hWndChild[0]);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc[4];
	PAINTSTRUCT ps;
	static char str[2][100];
	static int count[2], yPos[2];
	static int mx, my;
	static int sel;
	static BOOL Drag;

	switch (uMsg)
	{
	case WM_CREATE:
		Drag = FALSE;
		break;
	case WM_PAINT:
		hdc[0] = BeginPaint(hWndChild[0], &ps);
		hdc[3] = BeginPaint(hWndChild[3], &ps);

		TextOut(hdc[0], 0, yPos[0], str[0], strlen(str[0]));
		TextOut(hdc[3], 0, yPos[1], str[1], strlen(str[1]));

		EndPaint(hWndChild[0], &ps);
		EndPaint(hWndChild[3], &ps);
		break;
	case WM_LBUTTONDOWN:
		Drag = TRUE;
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		
		if (hWnd == hWndChild[0]){
			sel = 1;
			SetFocus(hWnd);
		}
		else if (hWnd == hWndChild[1]){
			sel = 2;
			SetFocus(hWnd);
		}
		else if (hWnd == hWndChild[2]){
			sel = 3;
			SetFocus(hWnd);
		}
		else if (hWnd == hWndChild[3]){
			sel = 4;
			SetFocus(hWnd);
		}
		break;
	case WM_LBUTTONUP:
		Drag = FALSE;
		break;
	case WM_MOUSEMOVE:
		hdc[1] = GetDC(hWndChild[1]);
		hdc[2] = GetDC(hWndChild[2]);
		if (Drag){
			if (sel == 2){
				MoveToEx(hdc[1], mx, my, NULL);
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				LineTo(hdc[1], mx, my);
			}
			if (sel == 3){
				MoveToEx(hdc[2], mx, my, NULL);
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				LineTo(hdc[2], mx, my);
			}
		}
		ReleaseDC(hWndChild[1], hdc[1]);
		ReleaseDC(hWndChild[2], hdc[2]);
		break;
	case WM_CHAR:
		if (sel == 1){
			str[0][count[0]++] = wParam;
			str[0][count[0]] = '\0';
			sel;
		}
		if (sel == 4){
			str[1][count[1]++] = wParam;
			str[1][count[1]] = '\0';
			sel;
		}
		InvalidateRect(hWndChild[0], NULL, TRUE);
		InvalidateRect(hWndChild[3], NULL, TRUE);
		break;
	case WM_DESTROY:
		break;
	}
	return DefMDIChildProc(hWnd, uMsg, wParam, lParam);
}