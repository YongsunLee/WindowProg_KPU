#include <Windows.h>
#include <math.h>

typedef struct{
	int type;
	POINT point[3];
	int c_x, c_y;
	int size;
	BOOL draw;
	int r, g, b;
}SHAPE;

float LengthPts(int, int, int, int);
BOOL InCircle(int x, int y, int mx, int my, float rad);

SHAPE shape[4];

HINSTANCE g_hinst;
LPCTSTR lpszClass = "�������� 3 - 6";

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
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static int plus_x, mx, my;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 1, NULL);
		shape[0].c_x = 100;
		shape[0].c_y = 100;
		plus_x = 100;
		break;
	case WM_TIMER:
		for (int i = 0; i < 4; ++i){
			if (i == 0){
				plus_x = 0;
			}
			else if (i == 1){
				plus_x = 100;
			}
			else if (i == 2){
				plus_x = 200;
			}
			else if (i == 3){
				plus_x = 300;
			}
			shape[i].c_x = shape[0].c_x + plus_x;
			shape[i].c_y = shape[0].c_y;
			shape[i].size = 50;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 4; ++i){
			hBrush = CreateSolidBrush(RGB(shape[i].r, shape[i].g, shape[i].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, shape[i].c_x - shape[i].size, shape[i].c_y - shape[i].size,
				shape[i].c_x + shape[i].size, shape[i].c_y + shape[i].size);
			SelectObject(hdc, oldBrush);
		}

		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (int i = 0; i < 4; ++i){
			if (InCircle(shape[i].c_x, shape[i].c_y, mx, my, shape[i].size)){
				if (shape[i].draw == FALSE){
					if (i == 0){
						shape[i].r = 255;
						shape[i].g = 0;
						shape[i].b = 0;
						shape[i].draw = TRUE;
					}
					else if (i == 1){
						shape[i].r = 0;
						shape[i].g = 255;
						shape[i].b = 0;
						shape[i].draw = TRUE;
					}
					else if (i == 2){
						shape[i].r = 0;
						shape[i].g = 0;
						shape[i].b = 255;
						shape[i].draw = TRUE;
					}
					else if (i == 3){
						shape[i].r = rand() % 255;
						shape[i].g = rand() % 255;
						shape[i].b = rand() % 255;
						shape[i].draw = TRUE;
					}
				}
				else if (shape[i].draw == TRUE){
					shape[i].r = 0;
					shape[i].g = 0;
					shape[i].b = 0;
					shape[i].draw = FALSE;
				}

			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		for (int i = 0; i < 4; ++i){
			if (shape[i].draw == FALSE){
				if (i == 0 && (wParam == 'r' || wParam == 'R')){
					shape[i].r = 255;
					shape[i].g = 0;
					shape[i].b = 0;
					shape[i].draw = TRUE;
				}
				else if (i == 1 && (wParam == 'g' || wParam == 'G')){
					shape[i].r = 0;
					shape[i].g = 255;
					shape[i].b = 0;
					shape[i].draw = TRUE;
				}
				else if (i == 2 && (wParam == 'b' || wParam == 'B')){
					shape[i].r = 0;
					shape[i].g = 0;
					shape[i].b = 255;
					shape[i].draw = TRUE;
				}
				else if (i == 3 && (wParam == 'a' || wParam == 'A')){
					shape[i].r = rand() % 255;
					shape[i].g = rand() % 255;
					shape[i].b = rand() % 255;
					shape[i].draw = TRUE;
				}
			}
			else if (shape[i].draw == TRUE){
				shape[i].r = 0;
				shape[i].g = 0;
				shape[i].b = 0;
				shape[i].draw = FALSE;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

float LengthPts(int x1, int y1, int x2, int y2)
{
	return((sqrt((float)(x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my, float rad)
{
	if (LengthPts(x, y, mx, my) < rad)
		return TRUE;
	else return FALSE;
}


/*
3 - 11
�� Ƣ��� ���α׷��� �̿��Ͽ� �������� ���� �����

ȭ�� ���� ��ܿ� 5*10 ���� ������ �ִ�.
ȭ���� �ϴܿ� �ٰ� �ְ� ���콺�� �̿��Ͽ� �ٸ� �����δ�.
���콺�� ������ ���콺�� x��ġ�� �̵�

���� Ƣ��鼭 ������ �ѹ� ������ ������ ���� �ٲ��.
���� Ƣ��鼭 ������ 2�� ������ ������ ��������.
���� ���� ������ ������ ������ ������ ȭ�鿡 ����Ѵ�.
�ð��� ������ ���� �̵��ӵ��� �þ��.
������ ��� �������� ������ ����ȴ�. (Ÿ�̸� ����)

- �׽�Ʈ�� ���ؼ� 5�ʸ��� ���ӵ��� �ٲ�� ������ 30�� �Ŀ� ����ǰ� ����� ��-

*/