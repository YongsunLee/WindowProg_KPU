#include <Windows.h>
#include <math.h>
#include <mmsystem.h>
#include "resource.h"

/*
BGM 정보 : Vanilla Mood - Tales Weaver OST - 05.Reminiscence
*/

//--------------------
// 정의
#define WINDOW_COUNT 2
#define MAX_COUNT_BULLET 150
#define IDC_BUTTON_START 100
#define IDC_BUTTON_HELP 101
#define IDC_BUTTON_END 102
//--------------------

//--------------------------------------------
// 함수선언
float LengthPts(int x1, int y1, int x2, int y2);
void SetVisibleWindow(int Index);
//---------------------------------------------


//--------------
//구조체
typedef struct{
	int c_x, c_y;
	int size;
	BOOL draw;
}SHAPE;
//---------------

HINSTANCE g_hinst;
LPCTSTR lpszClass = "우주의 끝을 찾아서 ver 1.0";
LPCTSTR lpszClassMain = "Main";
LPCTSTR lpszClassGame = "Game";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcGame(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProcHelp(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

//---------전역 변수--------
HWND hGameWnds[WINDOW_COUNT];
SHAPE Bullet[MAX_COUNT_BULLET];
SHAPE MBullet[MAX_COUNT_BULLET];
SHAPE BulletX[MAX_COUNT_BULLET];
SHAPE BBullet[MAX_COUNT_BULLET];
int nShowIndex = 0, Score = 0;
HBITMAP hBit_ALL, hBit_Start, hBit_Help, hBit_End, hBit_title, hBit_BulletX, hBit_Bullet_Boss,
		hBit_Player, hBit_BG, hBit_Monster1[16], hBit_Monster2[16], hBit_Monster3[16], hBit_MonsterBoss
		, hBit_GameOver, hBit_GameClear;
HBITMAP oldBit_Start;
BOOL GameStart;
//--------------------------


//--------화면 전환 함수--------
void SetVisibleWindow(int index)
{
	static int i;
	static RECT crt;

	if (index < 0 || index >= WINDOW_COUNT)return;
	nShowIndex = index;

	for (i = 0; i < WINDOW_COUNT; ++i){
		if (i == nShowIndex){
			if (IsWindowVisible(hGameWnds[i]) != TRUE){
				ShowWindow(hGameWnds[i], SW_SHOW);
			}
			else {
				if (IsWindowVisible(hGameWnds[i]) == TRUE){
					ShowWindow(hGameWnds[i], SW_HIDE);
				}
			}
		}
	}
	GetClientRect(GetParent(hGameWnds[nShowIndex]), &crt);
	SendMessage(GetParent(hGameWnds[nShowIndex]), WM_SIZE, SIZE_RESTORED,
		MAKELPARAM((crt.right - crt.left), (crt.bottom - crt.top)));
	SetFocus(hGameWnds[nShowIndex]);
}
//------------------------------

//--WinMain---
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

	WndClass.lpfnWndProc = WndProcMain;
	WndClass.lpszClassName = lpszClassMain;

	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = WndProcGame;
	WndClass.lpszClassName = lpszClassGame;

	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass,
		lpszClass,
		WS_OVERLAPPEDWINDOW,
		100,
		50,
		600,
		800,
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
//------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT crt;

	switch (uMsg) {
	case WM_CREATE:
		hGameWnds[0] = CreateWindow(lpszClassMain, NULL, WS_CHILD | WS_CLIPCHILDREN
			, 0, 0, 0, 0, hWnd, (HMENU)0, g_hinst, NULL);

		hGameWnds[1] = CreateWindow(lpszClassGame, NULL, WS_CHILD | WS_CLIPCHILDREN
			, 0, 0, 0, 0, hWnd, (HMENU)0, g_hinst, NULL);
		SetVisibleWindow(0);
		break;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED){
			GetClientRect(hWnd, &crt);
			MoveWindow(hGameWnds[nShowIndex], 0, 0, crt.right, crt.bottom, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 메인 타이틀
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc, hMemDC;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT lpdis;

	switch (uMsg)
	{
	case WM_CREATE:
		PlaySound( "Reminiscence.wav" , NULL, SND_ASYNC);
		CreateWindow("button", "Game Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW
			, 200, 450, 216, 49, hWnd, (HMENU)IDC_BUTTON_START, g_hinst, NULL);

		CreateWindow("button", "Help", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW
			, 260, 520, 96, 45, hWnd, (HMENU)IDC_BUTTON_HELP, g_hinst, NULL);

		CreateWindow("button", "Game End", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW
			, 260, 590, 103, 48, hWnd, (HMENU)IDC_BUTTON_END, g_hinst, NULL);

		hBit_title = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit_Start = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP3));
		hBit_Help = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP4));
		hBit_End = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP5));
		break;
	case WM_DRAWITEM:
		lpdis = (LPDRAWITEMSTRUCT)lParam;
		switch (lpdis->CtlID)
		{
		case IDC_BUTTON_START:
			hMemDC = CreateCompatibleDC(lpdis->hDC);
			if (lpdis->itemState & ODS_SELECTED){
				SelectObject(hMemDC, hBit_Start);
			}
			else
				SelectObject(hMemDC, hBit_Start);
			BitBlt(lpdis->hDC, 0, 0, 216, 49, hMemDC, 0, 0, SRCCOPY);
			DeleteDC(hMemDC);
			break;
		case IDC_BUTTON_HELP:
			hMemDC = CreateCompatibleDC(lpdis->hDC);
			if (lpdis->itemState & ODS_SELECTED){
				SelectObject(hMemDC, hBit_Help);
			}
			else
				SelectObject(hMemDC, hBit_Help);
			BitBlt(lpdis->hDC, 0, 0, 96, 45, hMemDC, 0, 0, SRCCOPY);
			DeleteDC(hMemDC);
			break;
		case IDC_BUTTON_END:
			hMemDC = CreateCompatibleDC(lpdis->hDC);
			if (lpdis->itemState & ODS_SELECTED){
				SelectObject(hMemDC, hBit_End);
			}
			else
				SelectObject(hMemDC, hBit_End);
			BitBlt(lpdis->hDC, 0, 0, 103, 48, hMemDC, 0, 0, SRCCOPY);
			DeleteDC(hMemDC);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);

		SelectObject(memdc, hBit_title);
		BitBlt(hdc, 0, 0, 600, 800, memdc, 0, 0, SRCCOPY);

		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_START:
			MessageBox(hWnd, "Game Start!", "게임시작", MB_OK);
			GameStart = TRUE;
			//PlaySound(NULL, 0, 0);
			SetVisibleWindow(1);
			break;
		case IDC_BUTTON_HELP:
			DialogBox(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProcHelp);
			break;
		case IDC_BUTTON_END:
			if (MessageBox(hWnd, "Real End?", "게임종료", MB_YESNO) == IDYES){
				DestroyWindow(GetParent(hWnd));
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 게임 화면
LRESULT CALLBACK WndProcGame(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc, mem1dc, mem2dc;
	PAINTSTRUCT ps;
	// Player 변수
	static HBRUSH PB_hBrush, PB_oldBrush;
	static int player_x, player_y, Count_B, Count_BX, CoolTimer, respwan;
	static int save_x[16], save_y[16], Player_HP;
	static BOOL Shoot, Shoot_x, CoolTime, PlayerDraw;
	static char PlayerHP_str[20];

	// 몬스터 변수
	static HBRUSH MB_hBrush, MB_oldBrush;
	static int M1_x[16], M1_y[16], M2_x[16], M2_y[16], M3_x[16], M3_y[16], Boss_x, Boss_y, Boss_HP;
	static int M1_DelayTimer[16], M2_DelayTimer[16], M3_DelayTimer[16], MB_DelayTimer[16], dis, dis1, dis2, pattern, Count_MB, Count_BB;
	static BOOL Monster_Bullet, M1_pattern1, M1_pattern2, M1_pattern3, M1_pattern4, M2_pattern1, 
			M2_pattern2, M2_pattern3, M2_pattern4, M3_pattern1, M3_pattern2, M3_pattern3, M3_pattern4, 
			M1_Delay[16], M2_Delay[16], M3_Delay[16], Move_Right, Move_Left, Shoot_Boss, MB_Delay[16], Undead, BossDraw;
	static char BossHP_str[20];

	// 배경 변수
	static int BG_x, BG_y;

	// 스코어 변수
	static char Score_str[20];
	static BOOL GameOver, GameClear;

	switch (uMsg)
	{
	case WM_CREATE:
		// BitMap-------------------------------------------------------------
		hBit_BG = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBit_Player = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP6));
		for (int i = 0; i < 16; ++i){
			hBit_Monster1[i] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP7));
		}
		for (int i = 0; i < 16; ++i){
			hBit_Monster2[i] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP8));
		}
		for (int i = 0; i < 16; ++i){
			hBit_Monster3[i] = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP9));
		}
		hBit_MonsterBoss = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP10));
		hBit_BulletX = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP14));
		//for (int i = 0; i < 4; ++i){
			hBit_Bullet_Boss = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP15));
		//}
		hBit_GameOver = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP16));
		hBit_GameClear = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP17));
		//--------------------------------------------------------------------

		//Timer----------------------
		SetTimer(hWnd, 1, 100, NULL);
		SetTimer(hWnd, 2, 1000, NULL);
		//---------------------------

		// Monster1 패턴별 좌표값
		for (int i = 0; i < 16; ++i){
			// 0 1 2 3
			if (i < 4){
				M1_x[i] = 100;
				M1_y[i] = 80 - dis1;
				dis1 += 50;
			}
			// 4 5 6 7
			else if (i >= 4 && i < 8){
				M1_x[i] = 200;
				M1_y[i] = 80 - dis1;
				dis1 += 50;
			}
			// 8 9 10 11
			else if (i >= 8 && i < 12){
				M1_x[i] = 300;
				M1_y[i] = 80 - dis1;
				dis1 += 50;
			}
			// 12 13 14 15
			else if (i >= 12 && i < 16) {
				M1_x[i] = 400;
				M1_y[i] = 80 - dis1;
				dis1 += 50;
			}
		}
		//----------------------

		// Monster2 패턴별 좌표값
		for (int i = 0; i < 16; ++i){
			// 0 1 2 3
			if ( i < 4){
				M2_x[i] = 200 - dis;
				M2_y[i] = 0 - dis;
				dis += 30;
			}
			// 4 5 6 7
			else if ( i >= 4 && i < 8){
				M2_x[i] = 400 + dis;
				M2_y[i] = 0 - dis;
				dis += 30;
			}
			// 8 9 10 11
			else if ( i >= 8 && i < 12){
				M2_x[i] = 100 - dis;
				M2_y[i] = 0 - dis;
				dis += 30;
			}
			// 12 13 14 15
			else if ( i >= 12 && i < 16) {
				M2_x[i] = 500 + dis;
				M2_y[i] = 0 - dis;
				dis += 30;
			}
		}

		//Monster 3 패턴별 좌표값
		for (int i = 0; i < 16; ++i){
			// 0 1 2 3
			if (i < 4){
				M3_x[i] = 0 - dis2;
				M3_y[i] = 100;
				dis2 += 50;
			}
			// 4 5 6 7
			else if (i >= 4 && i < 8){
				M3_x[i] = 500 + dis2;
				M3_y[i] = 100;
				dis2 += 50;
			}
			// 8 9 10 11
			else if (i >= 8 && i < 12){
				M3_x[i] = 100 - dis2;
				M3_y[i] = 350;
				dis2 += 50;
			}
			// 12 13 14 15
			else if (i >= 12 && i < 16) {
				M3_x[i] = 500 + dis2;
				M3_y[i] = 350;
				dis2 += 50;
			}
		}

		//--------------------------------
		// 잡다한 변수값 초기화
		Boss_HP = 10000;
		Player_HP = 1000;
		player_x = 250;
		player_y = 520;
		Boss_x = 8;
		Boss_y = 0;
		M1_pattern1 = TRUE;
		M2_pattern1 = TRUE;
		M3_pattern1 = TRUE;
		Move_Left = TRUE;
		CoolTimer = 20;
		M1_DelayTimer[0] = 10;
		M1_DelayTimer[4] = 10;
		PlayerDraw = TRUE;
		BossDraw = TRUE;
		//---------------------------------
		break;
	case WM_TIMER:
		if (GameStart == TRUE){
			switch (wParam)
			{
			// 미사일 이동
			case 1:
				for (int i = 0; i < MAX_COUNT_BULLET; ++i){
					if (Bullet[i].draw == TRUE){
						Bullet[i].c_y -= 10;
						if (Bullet[i].c_y <= 0){
							Bullet[i].draw = FALSE;
							Bullet[i].c_x = -20;
						}
					}
					if (BulletX[i].draw == TRUE){
						BulletX[i].c_y -= 10;
						if (BulletX[i].c_y <= 0){
							BulletX[i].draw = FALSE;
							BulletX[i].c_x = -20;
						}
					}
					if (BBullet[i].draw == TRUE){
						BBullet[i].c_y += 30;
						if (BBullet[i].c_y >= 880){
							BBullet[i].draw = FALSE;
							BBullet[i].c_x = -20;
						}
					}
					if ( MBullet[i].draw == TRUE){
						if ( MBullet[i].c_x < (player_x + 40) ){
							MBullet[i].c_x += 10;
						}
						if ( MBullet[i].c_x > (player_x + 40)){
							MBullet[i].c_x -= 10;
						}
						if ( MBullet[i].c_y < (player_y + 40)){
							MBullet[i].c_y += 10;
						}
						if ( MBullet[i].c_y > (player_y + 40)){
							MBullet[i].c_y -= 10;
						}
					}
					if (LengthPts(Bullet[i].c_x, Bullet[i].c_y, Boss_x + 80, Boss_y + 115) <= 30){
						Boss_HP -= 1000;
						Bullet[i].draw = FALSE;
						Bullet[i].c_x = -20;
						Bullet[i].c_y = -20;
						if (Boss_HP <= 0){
							Score += 1000;
							BossDraw = FALSE;
						}
					}
					if (LengthPts(BulletX[i].c_x, BulletX[i].c_y, Boss_x + 80, Boss_y + 115) <= 30){
						Boss_HP -= 1000;
						BulletX[i].draw = FALSE;
						BulletX[i].c_x = -20;
						BulletX[i].c_y = -20;
						if (Boss_HP <= 0){
							Score += 1000;
							BossDraw = FALSE;
							GameClear = TRUE;
						}
					}
					if (LengthPts(BBullet[i].c_x, BBullet[i].c_y, player_x, player_y) <= 30){
						Player_HP -= 100;
						BBullet[i].draw = FALSE;
						BBullet[i].c_x = -20;
						BBullet[i].c_y = -20;
					}
					for (int j = 0; j < 16; ++j){
						if (LengthPts(Bullet[i].c_x, Bullet[i].c_y, M1_x[j], M1_y[j]) <= 22){
							M1_x[j] = 0;
							M1_y[j] = 0;
							// 0 1 2 3
							if (j < 4){
								M1_x[j] = 100;
								M1_y[j] = 80 - dis1;
								dis1 += 50;
							}
							// 4 5 6 7
							else if (j >= 4 && j < 8){
								M1_x[j] = 200;
								M1_y[j] = 80 - dis1;
								dis1 += 50;
							}
							// 8 9 10 11
							else if (j >= 8 && j < 12){
								M1_x[j] = 300;
								M1_y[j] = 80 - dis1;
								dis1 += 50;
							}
							// 12 13 14 15
							else if (j >= 12 && j < 16) {
								M1_x[j] = 400;
								M1_y[j] = 80 - dis1;
								dis1 += 50;
							}
							Bullet[i].draw = FALSE;
							Bullet[i].c_x = -20;
							Bullet[i].c_y = -20;
							Score += 50;
						}
						if (LengthPts(Bullet[i].c_x, Bullet[i].c_y, M2_x[j], M2_y[j]) <= 25){
							M2_x[j] = 0;
							M2_y[j] = 0;
							// 0 1 2 3
							if (j < 4){
								M2_x[j] = 100;
								M2_y[j] = 80 - dis;
								dis += 30;
							}
							// 4 5 6 7
							else if (j >= 4 && j < 8){
								M2_x[j] = 200;
								M2_y[j] = 80 - dis;
								dis += 30;
							}
							// 8 9 10 11
							else if (j >= 8 && j < 12){
								M2_x[j] = 300;
								M2_y[j] = 80 - dis;
								dis += 30;
							}
							// 12 13 14 15
							else if (j >= 12 && j < 16) {
								M2_x[j] = 400;
								M2_y[j] = 80 - dis;
								dis += 30;
							}
							Bullet[i].draw = FALSE;
							Bullet[i].c_x = -20;
							Bullet[i].c_y = -20;
							Score += 100;
						}
						if (LengthPts(Bullet[i].c_x, Bullet[i].c_y, M3_x[j], M3_y[j]) <= 25){
							M3_x[j] = 0;
							M3_y[j] = 0;
							// 0 1 2 3
							if (j < 4){
								M3_x[j] = 100;
								M3_y[j] = 80 - dis2;
								dis2 += 50;
							}
							// 4 5 6 7
							else if (j >= 4 && j < 8){
								M3_x[j] = 200;
								M3_y[j] = 80 - dis2;
								dis2 += 50;
							}
							// 8 9 10 11
							else if (j >= 8 && j < 12){
								M3_x[j] = 300;
								M3_y[j] = 80 - dis2;
								dis2 += 50;
							}
							// 12 13 14 15
							else if (j >= 12 && j < 16) {
								M3_x[j] = 400;
								M3_y[j] = 80 - dis2;
								dis2 += 50;
							}
							Bullet[i].draw = FALSE;
							Bullet[i].c_x = -20;
							Bullet[i].c_y = -20;
							Score += 50;
						}
						if (BulletX[i].draw){
							if (LengthPts(BulletX[i].c_x, BulletX[i].c_y, M1_x[j], M1_y[j]) <= 30){
								M1_x[j] = 0;
								M1_y[j] = 0;
								Score += 50;
							}
							if (LengthPts(BulletX[i].c_x, BulletX[i].c_y, M2_x[j], M2_y[j]) <= 30){
								M2_x[j] = 0;
								M2_y[j] = 0;
								Score += 100;
							}
							if (LengthPts(BulletX[i].c_x, BulletX[i].c_y, M3_x[j], M3_y[j]) <= 30){
								M3_x[j] = 0;
								M3_y[j] = 0;
								Score += 50;
							}
						}

						if (Undead == FALSE){
							if (LengthPts(MBullet[i].c_x, MBullet[i].c_y, player_x + 40, player_y + 40) <= 20){
								Player_HP -= 10;
								if (Player_HP <= 0){
									PlayerDraw = FALSE;
									GameOver = TRUE;
								}
								MBullet[i].draw = FALSE;
								MBullet[i].c_x = -20;
								MBullet[i].c_y = -20;
							}
						}
					}
				}
				break;
			// 몬스터 리스폰 시간측정
			case 2:
				respwan++;
				if ( respwan == 20){
					respwan = 0;
				}
				if (CoolTimer < 20){
					CoolTimer++;
				}
				if (CoolTimer == 20){
					CoolTime = TRUE;
				}
				for (int i = 0; i < 16; ++i){
					if (M1_DelayTimer[i] < 10){
						M1_DelayTimer[i]++;
					}
					if (M1_DelayTimer[i] == 10){
						M1_Delay[i] = TRUE;
					}
					if (M2_DelayTimer[i] < 10){
						M2_DelayTimer[i]++;
					}
					if (M2_DelayTimer[i] == 10){
						M2_Delay[i] = TRUE;
					}
					if (M3_DelayTimer[i] < 10){
						M3_DelayTimer[i]++;
					}
					if (M3_DelayTimer[i] == 10){
						M3_Delay[i] = TRUE;
					}
				}
				for (int i = 0; i < 4; ++i){
					if (MB_DelayTimer[i] < 10){
						MB_DelayTimer[i]++;
					}
					if (MB_DelayTimer[i] == 10){
						MB_Delay[i] = TRUE;
					}
				}
				break;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (GameStart){
			if (hBit_ALL == NULL)
				hBit_ALL = CreateCompatibleBitmap(hdc, 600, 800);

			mem1dc = CreateCompatibleDC(hdc);
			mem2dc = CreateCompatibleDC(mem1dc);

			SelectObject(mem1dc, hBit_ALL);

			// ----------------------------------------------------------------
			// Back Ground
			SelectObject(mem2dc, hBit_BG);
			BitBlt(mem1dc, BG_x, BG_y, 600, 800, mem2dc, 0, 0, SRCCOPY);

			BitBlt(mem1dc, 0, 0, 600, 800, mem2dc, 0, 800 - BG_y, SRCCOPY);

			BG_y += 3;
			if ((800 - BG_y) <= 0){
				BG_y = 0;
			}
			//-------------------------------------------------------------------


			//-------------------------------------------------------------------------------------------
			// Player
			if ( PlayerDraw ){
				SelectObject(mem2dc, hBit_Player);
				TransparentBlt(mem1dc, player_x, player_y, 80, 80, mem2dc, 0, 0, 135, 135, RGB(0, 0, 0));
			}
			//-------------------------------------------------------------------------------------------

			//-------------------------------------------------------------------------------------------
			// 몬스터 1
			if (M1_pattern1){
				for (int i = 0; i < 4; ++i){
					SelectObject(mem2dc, hBit_Monster1[i]);
					TransparentBlt(mem1dc, M1_x[i], M1_y[i], 53, 54, mem2dc, 0, 0, 53, 54, RGB(0, 0, 0));
					if (M1_y[0] < 300 || M1_y[1] < 250 || M1_y[2] < 200 || M1_y[3] < 150){
						M1_y[i] += 8;
					}
					else if(M1_y[i] >= 800){
						M1_x[i] = 100;
						M1_y[i] = 80 - dis1;
						dis1 += 50;
					}
					if (respwan == 5){
						M1_pattern2 = TRUE;
					}
				}
			}
			if (M1_pattern2){
				for (int i = 4; i < 8; ++i){
					SelectObject(mem2dc, hBit_Monster1[i]);
					TransparentBlt(mem1dc, M1_x[i], M1_y[i], 53, 54, mem2dc, 0, 0, 53, 54, RGB(0, 0, 0));
					if (M1_y[4] < 300 || M1_y[5] < 250 || M1_y[6] < 200 || M1_y[7] < 150){
						M1_y[i] += 8;
					}
					else if (M1_y[i] >= 800){
						M1_x[i] = 200;
						M1_y[i] = 80 - dis1;
						dis1 += 50;
					}
					if (respwan == 10){
						M1_pattern3 = TRUE;
					}
				}
			}
			if (M1_pattern3){
				for (int i = 8; i < 12; ++i){
					SelectObject(mem2dc, hBit_Monster1[i]);
					TransparentBlt(mem1dc, M1_x[i], M1_y[i], 53, 54, mem2dc, 0, 0, 53, 54, RGB(0, 0, 0));
					if (M1_y[8] < 300 || M1_y[9] < 250 || M1_y[10] < 200 || M1_y[11] < 150){
						M1_y[i] += 8;
					}
					else if (M1_y[i] >= 800){
						M1_x[i] = 300;
						M1_y[i] = 80 - dis1;
						dis1 += 50;
					}
					if (respwan == 15){
						M1_pattern4 = TRUE;
					}
				}
			}
			if (M1_pattern4){
				for (int i = 12; i < 16; ++i){
					SelectObject(mem2dc, hBit_Monster1[i]);
					TransparentBlt(mem1dc, M1_x[i], M1_y[i], 53, 54, mem2dc, 0, 0, 53, 54, RGB(0, 0, 0));
					if (M1_y[12] < 300 || M1_y[13] < 250 || M1_y[14] < 200 || M1_y[15] < 150){
						M1_y[i] += 8;
					}else if (M1_y[i] >= 800){
						M1_x[i] = 400;
						M1_y[i] = 80 - dis1;
						dis1 += 50;
					}
					if (respwan == 20){
						M1_pattern1 = TRUE;
						respwan = 0;
					}
				}
			}
			//몬스터 2
			if (M2_pattern1){
				for (int i = 0; i < 4; ++i){
					SelectObject(mem2dc, hBit_Monster2[i]);
					TransparentBlt(mem1dc, M2_x[i], M2_y[i], 50, 54, mem2dc, 0, 0, 50, 54, RGB(0, 0, 0));
					if (M2_x[i] < 1000 && M2_y[i] < 800){
						M2_x[i] += 8;
						M2_y[i] += 8;
					}
					else { 
						M2_x[i] = 200 - dis;
						M2_y[i] = 0 - dis;
						dis += 30;
						//M2_pattern1 = FALSE;
					}
					if (respwan == 5){
						M2_pattern2 = TRUE;
					}
				}
			}
			if (M2_pattern2){
				for (int i = 4; i < 8; ++i){
					SelectObject(mem2dc, hBit_Monster2[i]);
					TransparentBlt(mem1dc, M2_x[i], M2_y[i], 50, 54, mem2dc, 0, 0, 50, 54, RGB(0, 0, 0));
					if (M2_x[i] > -200 && M2_y[i] < 800){
						M2_x[i] -= 8;
						M2_y[i] += 8;
					}
					else{
						M2_x[i] = 400 + dis;
						M2_y[i] = 0 - dis;
						dis += 30;
						//M2_pattern2 = FALSE;
					}
					if (respwan == 10){
						M2_pattern3 = TRUE;
					}
				}
			}
			if (M2_pattern3){
				for (int i = 8; i < 12; ++i){
					SelectObject(mem2dc, hBit_Monster2[i]);
					TransparentBlt(mem1dc, M2_x[i], M2_y[i], 50, 54, mem2dc, 0, 0, 50, 54, RGB(0, 0, 0));
					if (M2_x[i] < 1000 && M2_y[i] < 800){
						M2_x[i] += 8;
						M2_y[i] += 8;
					}
					else {
						M2_x[i] = 100 - dis;
						M2_y[i] = 0 - dis;
						dis += 30;
						//M2_pattern3 = FALSE;
					}
					if (respwan == 15){
						M2_pattern4 = TRUE;
					}
				}
			}
			if (M2_pattern4){
				for (int i = 12; i < 16; ++i){
					SelectObject(mem2dc, hBit_Monster2[i]);
					TransparentBlt(mem1dc, M2_x[i], M2_y[i], 50, 54, mem2dc, 0, 0, 50, 54, RGB(0, 0, 0));
					if (M2_x[i] > -200 && M2_y[i] < 800){
						M2_x[i] -= 8;
						M2_y[i] += 8;
					}
					else {
						M2_x[i] = 200 - dis;
						M2_y[i] = 0 - dis;
						dis += 30;
						//M2_pattern4 = FALSE;
					}
					if (respwan == 20){
						M2_pattern1 = TRUE;
						respwan = 0;
					}
				}
			}
			//몬스터 3
			if (M3_pattern1){
				for (int i = 0; i < 4; ++i){
					SelectObject(mem2dc, hBit_Monster3[i]);
					TransparentBlt(mem1dc, M3_x[i], M3_y[i], 50, 50, mem2dc, 0, 0, 50, 50, RGB(0, 0, 0));
					if (M3_x[0] < 250 || M3_x[1] < 200 || M3_x[2] < 150 || M3_x[3] < 100){
						M3_x[i] += 8;
					}
					if (respwan == 5){
						M3_pattern2 = TRUE;
					}
				}
			}
			if (M3_pattern2){
				for (int i = 4; i < 8; ++i){
					SelectObject(mem2dc, hBit_Monster3[i]);
					TransparentBlt(mem1dc, M3_x[i], M3_y[i], 50, 50, mem2dc, 0, 0, 50, 50, RGB(0, 0, 0));
					if (M3_x[4] > 300 || M3_x[5] > 350 || M3_x[6] > 400 || M3_x[7] > 450){
						M3_x[i] -= 8;
					}
					if (respwan == 10){
						M3_pattern3 = TRUE;
					}
				}
			}
			if (M3_pattern3){
				for (int i = 8; i < 12; ++i){
					SelectObject(mem2dc, hBit_Monster3[i]);
					TransparentBlt(mem1dc, M3_x[i], M3_y[i], 50, 50, mem2dc, 0, 0, 50, 50, RGB(0, 0, 0));
					if (M3_x[8] < 250 || M3_x[9] < 200 || M3_x[10] < 150 || M3_x[11] < 100){
						M3_x[i] += 8;
					}
					if (respwan == 15){
						M3_pattern4 = TRUE;
					}
				}
			}
			if (M3_pattern4){
				for (int i = 12; i < 16; ++i){
					SelectObject(mem2dc, hBit_Monster3[i]);
					TransparentBlt(mem1dc, M3_x[i], M3_y[i], 50, 50, mem2dc, 0, 0, 50, 50, RGB(0, 0, 0));
					if (M3_x[12] > 300 || M3_x[13] > 350 || M3_x[14] > 400 || M3_x[15] > 450){
						M3_x[i] -= 8;
					}
					if (respwan == 20){
						M3_pattern1 = TRUE;
					}
				}
			}
			// BOSS 몬스터
			if (Score >= 1000 && BossDraw){
				SelectObject(mem2dc, hBit_MonsterBoss);
				TransparentBlt(mem1dc, Boss_x, Boss_y, 160, 115, mem2dc, 0, 0, 160, 115, RGB(0, 0, 0));
				if (Boss_x < 440 && Move_Left){
					Boss_x += 8;
					if (Boss_x >= 440){
						Move_Left = FALSE;
						Move_Right = TRUE;
					}
				}
				if (Boss_x > 0 && Move_Right){
					Boss_x -= 8;
					if (Boss_x <= 0){
						Move_Right = FALSE;
						Move_Left = TRUE;
					}
				}
			}
			//-------------------------------------------------------------------------------------------

			//-------------------------------------------------------------------------------------------
			// 미사일
			// M1과 player간의 거리가 300 이하이면 미사일을 생성
			for (int i = 0; i < 16; ++i){
				if (LengthPts(M1_x[i], M1_y[i], player_x, player_y) <= 300){
					if (M1_Delay[i] == TRUE){
						Monster_Bullet = TRUE;
						MBullet[Count_MB].draw = TRUE;
						MBullet[Count_MB].c_x = M1_x[i] + 22;
						MBullet[Count_MB].c_y = M1_y[i] + 60;
						MBullet[Count_MB].size = 5;
						Count_MB++;
						if (Count_MB >= 150){
							Count_MB = 0;
						}
						M1_Delay[i] = FALSE;
						M1_DelayTimer[i] = 0;
					}
				}
				if (LengthPts(M2_x[i], M2_y[i], player_x, player_y) <= 300){
					if (M2_Delay[i] == TRUE){
						Monster_Bullet = TRUE;
						MBullet[Count_MB].draw = TRUE;
						MBullet[Count_MB].c_x = M2_x[i] + 22;
						MBullet[Count_MB].c_y = M2_y[i] + 60;
						MBullet[Count_MB].size = 5;
						Count_MB++;
						if (Count_MB >= 150){
							Count_MB = 0;
						}
						M2_Delay[i] = FALSE;
						M2_DelayTimer[i] = 0;
					}
				}
				if (LengthPts(M3_x[i], M3_y[i], player_x, player_y) <= 300){
					if (M3_Delay[i] == TRUE){
						Monster_Bullet = TRUE;
						MBullet[Count_MB].draw = TRUE;
						MBullet[Count_MB].c_x = M3_x[i] + 22;
						MBullet[Count_MB].c_y = M3_y[i] + 60;
						MBullet[Count_MB].size = 5;
						Count_MB++;
						if (Count_MB >= 150){
							Count_MB = 0;
						}
						M3_Delay[i] = FALSE;
						M3_DelayTimer[i] = 0;
					}
				}
			}

			// 몬스터총알
			if (Monster_Bullet = TRUE){
				for (int i = 0; i < Count_MB; ++i){
					if (MBullet[i].draw == TRUE){
						MB_hBrush = CreateSolidBrush(RGB(201, 255, 102));
						MB_oldBrush = (HBRUSH)SelectObject(mem1dc, MB_hBrush);
						Ellipse(mem1dc, MBullet[i].c_x - MBullet[i].size, MBullet[i].c_y - MBullet[i].size
							, MBullet[i].c_x + MBullet[i].size, MBullet[i].c_y + MBullet[i].size);
						SelectObject(mem1dc, MB_oldBrush);
						DeleteObject(MB_hBrush);
					}
				}
			}

			// 일반 총알
			if (Shoot == TRUE){
				for (int i = 0; i < MAX_COUNT_BULLET; ++i){
					if (Bullet[i].draw == TRUE){
						PB_hBrush = CreateSolidBrush(RGB(201, 103, 255));
						PB_oldBrush = (HBRUSH)SelectObject(mem1dc, PB_hBrush);
						Ellipse(mem1dc, Bullet[i].c_x - Bullet[i].size, Bullet[i].c_y - Bullet[i].size,
							Bullet[i].c_x + Bullet[i].size, Bullet[i].c_y + Bullet[i].size);
						SelectObject(mem1dc, PB_oldBrush);
						DeleteObject(PB_hBrush);
					}
				}
			}

			// 특수 총알
			if (Shoot_x == TRUE){
				for (int i = 0; i < MAX_COUNT_BULLET; ++i){
					if (BulletX[i].draw){
						SelectObject(mem2dc, hBit_BulletX);
						TransparentBlt(mem1dc, BulletX[i].c_x, BulletX[i].c_y, 80, 80, mem2dc, 0, 0, 150, 115, RGB(0, 0, 0));
					}
				}
			}

			if ( BossDraw ){
				// Boss 총알 생성
				for (int i = 0; i < 4; ++i){
					if (MB_Delay[i]){
						Shoot_Boss = TRUE;
						BBullet[Count_BB].draw = TRUE;
						BBullet[Count_BB].c_x = (Boss_x + 80);
						BBullet[Count_BB].c_y = (Boss_y + 115);
						Count_BB++;
						if (Count_BB >= 150){
							Count_BB = 0;
						}
						MB_Delay[i] = FALSE;
						MB_DelayTimer[i] = 0;
					}
				}

				// Boss 총알
				if (Shoot_Boss == TRUE){
					for (int i = 0; i < MAX_COUNT_BULLET; ++i){
						SelectObject(mem2dc, hBit_Bullet_Boss);
						TransparentBlt(mem1dc, BBullet[i].c_x, BBullet[i].c_y, 80, 100, mem2dc, 0, 0, 223, 459, RGB(0, 0, 0));
					}
				}
			}
			//---------------------------------------------------------------------------------------------

			// Score
			SetBkMode(mem1dc, TRANSPARENT);
			SetTextColor(mem1dc, RGB(255, 255, 255));
			wsprintf(Score_str, "Score : %d", Score);
			TextOut(mem1dc, 450, 0, Score_str, strlen(Score_str));

			wsprintf(BossHP_str, "Boss : %d", Boss_HP);
			TextOut(mem1dc, 450, 50, BossHP_str, strlen(BossHP_str));

			wsprintf(PlayerHP_str, "Player : %d", Player_HP);
			TextOut(mem1dc, 450, 520, PlayerHP_str, strlen(PlayerHP_str));

			// GameOver
			if ( GameOver ){
				SelectObject(mem2dc, hBit_GameOver);
				TransparentBlt(mem1dc, 100, 200, 362, 79, mem2dc, 0, 0, 362, 79, RGB(0,0,0));
				GameStart = FALSE;
			}

			// GameClear
			if ( GameClear ){
				SelectObject(mem2dc, hBit_GameClear);
				TransparentBlt(mem1dc, 300, 300, 303, 266, mem2dc, 0,0, 303,266, RGB(0,0,0));
				GameStart = FALSE;
			}

			BitBlt(hdc, 0, 0, 600, 800, mem1dc, 0, 0, SRCCOPY);
		}

		SelectObject(mem2dc, hBit_BG);
		DeleteDC(mem2dc);
		SelectObject(mem2dc, hBit_Player);
		DeleteDC(mem2dc);
		SelectObject(mem2dc, hBit_Monster1);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, hBit_ALL);
		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam){
		case VK_UP:
			player_y -= 10;
			if ( player_y <= 0){
				player_y += 10;
			}
			break;
		case VK_DOWN:
			player_y += 10;
			if (player_y + 80 >= 800){
				player_y -= 10;
			}
			break;
		case VK_LEFT:
			player_x -= 20;
			if (player_x <= 0){
				player_x += 20;
			}
			break;
		case VK_RIGHT:
			player_x += 20;
			if (player_x + 80 >= 600){
				player_x -= 20;
			}
			break;
		case VK_ESCAPE:
			SetVisibleWindow(0);
			break;
		}
		break;

	case WM_CHAR:
		// 미사일 발사 버튼
		if (wParam == 'z'){
			Shoot = TRUE;
			Bullet[Count_B].draw = TRUE;
			Bullet[Count_B].c_x = player_x + 40;
			Bullet[Count_B].c_y = player_y - 10;
			Bullet[Count_B].size = 5;
			Count_B++;
			if (Count_B >= 150){
				Count_B = 0;
			}
		}
		if (wParam == 'x'){
			if ( CoolTime ){
				Shoot_x = TRUE;
				BulletX[Count_BX].draw = TRUE;
				BulletX[Count_BX].c_x = player_x;
				BulletX[Count_BX].c_y = player_y - 40;
				BulletX[Count_BX].size = 5;
				Count_BX++;
				CoolTime = FALSE;
				CoolTimer = 0;
				if (Count_BX >= 150){
					Count_BX = 0;
				}
			}
		}
		if (wParam == 'a'){
			if ( GameStart == TRUE){
				GameStart = FALSE;
			}
			else {
				GameStart = TRUE;
			}
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 도움말
BOOL CALLBACK DialogProcHelp(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC Dlg_hdc, Dlg_memdc;
	PAINTSTRUCT ps;
	static HBITMAP DirKey, Key_zx, Key_a;
	static HBRUSH hBursh;
	static RECT rect;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		GetClientRect(hDlg, &rect);
		DirKey = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP11));
		Key_a = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP12));
		Key_zx = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP13));
		hBursh = CreateSolidBrush(RGB(255,255,255));
		break;
	case WM_PAINT:
		Dlg_hdc = BeginPaint(hDlg, &ps);
		Dlg_memdc = CreateCompatibleDC(Dlg_hdc);

		FillRect(Dlg_hdc, &rect, hBursh);

		SelectObject(Dlg_memdc, DirKey);
		BitBlt(Dlg_hdc, 250, 80, 500, 500, Dlg_memdc, 0,0, SRCCOPY);

		SelectObject(Dlg_memdc, Key_a);
		BitBlt(Dlg_hdc, 90, 80, 200, 200, Dlg_memdc, 0,0, SRCCOPY);

		SelectObject(Dlg_memdc, Key_zx);
		BitBlt(Dlg_hdc, 70, 160, 200, 200, Dlg_memdc, 0, 0, SRCCOPY);

		DeleteDC(Dlg_memdc);
		EndPaint(hDlg, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

float LengthPts(int x1, int y1, int x2, int y2)
{
	return((sqrt((float)(x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))));
}