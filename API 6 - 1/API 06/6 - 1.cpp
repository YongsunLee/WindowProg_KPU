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

SHAPE shape[2];
static BOOL Win_Ball, DLG_Ball;

HINSTANCE g_hinst;
LPCTSTR lpszClass = "연습문제 6 - 1";

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
	RECT rect;
	static int speedX, speedY;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 100, NULL);
		shape[0].c_x = 100;
		shape[0].c_y = 100;
		shape[0].size = 50;
		GetClientRect(hWnd, &rect);
		speedX = 10;
		speedY = 10;
		break;
	case WM_LBUTTONDOWN:
		DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			if (Win_Ball == TRUE){
				shape[0].c_x -= speedX;
				shape[0].c_y -= speedY;
				if ( shape[0].c_x >= 770 ){
					speedX *= -1;
				}
				if ( shape[0].c_x <= 0 ){
					speedX *= -1;
				}
				if ( shape[0].c_y <= 0 ){
					speedY *= -1;
				}
				if ( shape[0].c_y >= 500 ){
					speedY *= -1;
				}
			}
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (Win_Ball == TRUE){
			Ellipse(hdc, shape[0].c_x - shape[0].size, shape[0].c_y - shape[0].size
				, shape[0].c_x + shape[0].size, shape[0].c_y + shape[0].size);
		}

		EndPaint(hWnd, &ps);
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
	static int speedX, speedY;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, 0, 100, NULL);
		shape[1].c_x = 50;
		shape[1].c_y = 50;
		shape[1].size = 10;
		speedX = 10;
		speedY = 10;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			if (DLG_Ball == TRUE){
				shape[1].c_x -= speedX;
				shape[1].c_y -= speedY;
				if (shape[1].c_x >= 450){
					speedX *= -1;
				}
				if (shape[1].c_x <= 0){
					speedX *= -1;
				}
				if (shape[1].c_y <= 0){
					speedY *= -1;
				}
				if (shape[1].c_y >= 300){
					speedY *= -1;
				}
			}
			break;
		default:
			break;
		}
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_WIN_START:
			Win_Ball = TRUE;
			break;
		case IDC_WIN_END:
			Win_Ball = FALSE;
			break;
		case IDC_EXIT:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;
		case IDC_DLG_START:
			DLG_Ball = TRUE;
			break;
		case IDC_DLG_STOP:
			DLG_Ball = FALSE;
			break;
		case ID_DLG_EXIT:
			EndDialog(hDlg, 0);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		DLG_hdc = BeginPaint(hDlg, &ps);

		if (DLG_Ball == TRUE){
			Ellipse(DLG_hdc, shape[1].c_x - shape[1].size, shape[1].c_y - shape[1].size
				, shape[1].c_x + shape[1].size, shape[1].c_y + shape[1].size);
		}

		EndPaint(hDlg, &ps);
		break;
	}
	return false;
}

/*
6 - 3

다이얼로그 박스에는 화면을 설정하고 오른쪽에는 아래의 버튼을 만든다.
	버튼
	1. 점프
	2. 앞
	3. 뒤
	4. 위
	5. 아래
	
	체크 박스
	6. 그리드

	라디오 버튼 - 비트맵 사용 (원, 사각형으로 해도됨)
	7. 캐릭터 1
	8. 캐릭터 2

	라디오 버튼
	9. 확대 캐릭터 확대
	10. 축소 캐릭터 축소
	11. 캐릭터가 현 위치에서 지그재그로 이동 (아래 오른쪽)
	12. 캐릭터가 정지
	13. 
	14. 프로그램 종료


*/