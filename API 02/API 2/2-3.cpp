//		14.3.18
//	

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

//CallBack �Լ� = OS�� ����ϴ� �Լ�
// ������ �Լ��� ���α׷��� ���
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	COLORREF color;
COLORREF: DWORD;
	RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;
	int i, j;
	char temp[100];

	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (i = 1; i <= 20; ++i){
			for (j = i; j <= 20; ++j){
				rect.left = (j)* 39;
				rect.top = (i)* 28;
				rect.right = (j - 1) * 39;
				rect.bottom = (i - 1) * 28;

				wsprintf(temp, "(%d,%d)", i, j);
				DrawText(hdc, temp, strlen(temp), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ȭ�鿡 �ﰢ�� ������ ���ڸ� ����ϱ�
// ȭ���� 20x20ĭ���� ������ ����� ���� ����Ѵ�.
// DrawText �Լ��� ����Ѵ�.
// 1����� ��� ���� ���� ���� �� ���� ����Ѵ�.