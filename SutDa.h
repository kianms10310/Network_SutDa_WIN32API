#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "stdafx.h"
#include "resource.h"
#define PLAYER 2										// 플레이어 수 

TCHAR str[128];																		// 임시출력버퍼
TCHAR szIP_ADDRESS[20];																// IP 주소를 저장할 배열

POINT randPoint[PLAYER * 2];															// 화투의 x,y 좌표 저장

/*					핸들 변수										*/
 HWND hWndButton[3];																	// 버튼 HWND
 HWND hWndStatic[PLAYER];															// 돈 Static HWND
 HMENU hMenu, hMenuReady, hMenuStart, hMenuCreate, hMenuConnect, hMenuDisconnect;	// 메뉴핸들

/*					싱글플레이 변수									*/
 BOOL iStart = FALSE;																// 게임이 시작됐는지
 BOOL iEnd = TRUE;																	// 게임이 종료 됐는지
 BOOL iOpen = FALSE;																	// 패를 오픈할 것인지
 BOOL iSolo = TRUE;																	// 싱글 플레이인가

/*					네트워크 관련 변수								*/
 BOOL IsClient = FALSE;																// 클라이언트 여부
 BOOL state_ServReady = FALSE;														// 서버의 준비 상태
 BOOL state_ClntReady = FALSE;														// 클라이언트의 준비 상태
 BOOL flag_GameStart = FALSE;														// 게임중 여부
 BOOL IsTurn = FALSE;																// 자신의 차례인지 여부
 BOOL flag_Start = FALSE;															// 시작버튼을 눌렀는지 여부
 BOOL flag_End = FALSE;																// 게임의 끝의 여부
 SOCKET client_sock;																	// 클라이언트 소켓

 int sel;																			// 선택
 int rating = 1;																		// 배팅
 int pointer[20] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };	// x축 좌표를 배열에 저장
 int randX[PLAYER * 2];																// x좌표의 난수
 int Money[PLAYER] = { 0, 0 };														// 플레이어의 소지금
 int countWin[PLAYER];																// 이긴 회수
 int countLose[PLAYER];																// 패배 회수

 enum { WIN = 300, LOSE, DRAW };														//버튼 ID
 enum { PLAYER1_MONEY = 200, PLAYER2_MONEY };											//Static ID
 enum {																					
	DDANG38 = 501, SAGOOPATOO, AMHANG, GLOSSDDANG, SAGOO, DDANGJAB,
	DANG10, DANG9, DANG8, DANG7, DANG6, DANG5, DANG4, DANG3, DANG2, DANG1,
	ARRY, SNAKE, BING9, BING10, JANGSA, SAERYUCK, GAB0,
	END8, END7, END6, END5, END4, END3, END2, END1, JERO
};																					//경우의 수

void RandomXY();
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit, WPARAM wParam);
int GetUniqueNumber();
int Uped(int x1, int y1, int x2, int y2);
int Compare(int x, int y);
void resultPrint(HWND hWnd, HDC hdc, HBITMAP MyBitmap, int x, int y, WPARAM wParam);
void CheckLevel(HWND hWnd);
void Clear_SutDa();

void resultPrint(HWND hWnd, HDC hdc, HBITMAP MyBitmap, int x, int y, WPARAM wParam)
{
	int pos;
	char buffer[300];
	iOpen = TRUE;							// 버튼이 눌렸기 때문에 컴퓨터의 패 공개
	DrawBitmap(hdc, x, y, MyBitmap, wParam);

	if (iSolo == TRUE)						// 싱글플레이
	{
		if (Compare(Uped(randPoint[0].x, randPoint[0].y, randPoint[1].x, randPoint[1].y), Uped(randPoint[2].x, randPoint[2].y, randPoint[3].x, randPoint[3].y))
			== sel) // 사용자의 선택 값이, return 값과 같다면 사용자의 승리.
		{
			MessageBox(hWnd, TEXT("승리"), TEXT("결과"), MB_OK);
			Money[1] += 5000 * rating;			// 사용자의 돈 증가
			Money[0] -= 5000 * rating;			// 컴퓨터의 돈 감소
			countWin[1]++;						// 사용자의 승리 증가
			countLose[0]++;						// 컴퓨터의 패배 증가
		}
		else
		{
			MessageBox(hWnd, TEXT("패배"), TEXT("결과"), MB_OK);
			Money[1] -= 5000 * rating;			// 사용자의 돈 감소
			Money[0] += 5000 * rating;			// 컴퓨터의 돈 증가
			countWin[0]++;						// 컴퓨터의 승리 증가
			countLose[1]++;						// 사용자의 패배 증가
		}
	}
	else if (flag_Start == TRUE)			// 네트워크 플레이
	{
		if (Compare(Uped(randPoint[0].x, randPoint[0].y, randPoint[1].x, randPoint[1].y), Uped(randPoint[2].x, randPoint[2].y, randPoint[3].x, randPoint[3].y)) == WIN)
		{
			MessageBox(hWnd, TEXT("승리"), TEXT("결과"), MB_OK);
			Money[1] += 5000 * rating;			// 사용자의 돈 증가
			Money[0] -= 5000 * rating;			// 컴퓨터의 돈 감소
			countWin[1]++;						// 사용자의 승리 증가
			countLose[0]++;						// 컴퓨터의 패배 증가
		}
		else if (Compare(Uped(randPoint[0].x, randPoint[0].y, randPoint[1].x, randPoint[1].y), Uped(randPoint[2].x, randPoint[2].y, randPoint[3].x, randPoint[3].y)) == LOSE)
		{
			MessageBox(hWnd, TEXT("패배"), TEXT("결과"), MB_OK);
			Money[1] -= 5000 * rating;			// 사용자의 돈 감소
			Money[0] += 5000 * rating;			// 컴퓨터의 돈 증가
			countWin[0]++;						// 컴퓨터의 승리 증가
			countLose[1]++;						// 사용자의 패배 증가
		}
		else
			MessageBox(hWnd, TEXT("무승부"), TEXT("결과"), MB_OK);
	}
	iOpen = FALSE;							// 컴퓨터의 패를 볼 수 없음.


	if (iStart == TRUE)						// 싱글 플레이
	{
		RandomXY();
		CheckLevel(hWnd);						// Level을 체크함.
		InvalidateRect(hWnd, NULL, TRUE);
	}

	if (flag_Start == TRUE && IsTurn == TRUE)	// 네트워크 플레이
	{
		if (Money[1] < 0)
		{
			Clear_SutDa();
		}
		RandomXY();
		pos = randPoint[0].x * 10000000 + randPoint[0].y * 1000000 +
			randPoint[1].x * 100000 + randPoint[1].y * 10000 +
			randPoint[2].x * 1000 + randPoint[2].y * 100 +
			randPoint[3].x * 10 + randPoint[3].y;
		_itoa(pos, buffer, 10);
		send(client_sock, buffer, 10, 0);
		InvalidateRect(hWnd, NULL, TRUE);
	}

}

int GetUniqueNumber()
{
	int i, ret;
	while (1)
	{
		ret = rand() % 20;			//0~19의 난수 발생
		for (i = 0; i<4; i++)
		{
			if (randX[i] == ret)	//만약 중복된다면 탈출!
				break;
		}
		if (i == 4)					//반복문이 끝났다면 ret을 return
			return ret;
	}
}

void RandomXY()
{
	int randNumY;
	srand((unsigned)time(NULL));

	for (int i = 0; i < 4; i++)
	{
		//GetUniqueNumber() 호출
		randX[i] = GetUniqueNumber();
		//중복되지 않은 수가 발생되었기 때문에 pointer[randX[i]] 인덱스의 값을 randPoint[i].x 값으로 가져옴.
		randPoint[i].x = pointer[randX[i]];


		randNumY = rand() % 2;
		randPoint[i].y = randNumY;
	}

	//만약 (x1,y1) 와 (x2, y2) 두 좌표가 서로 같은 경우
	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			if (randPoint[i].x == randPoint[j].x && randPoint[i].y == randPoint[j].y)
			{
				randPoint[i].y = abs(randPoint[i].y - 1);
			}
		}
	}

}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit, WPARAM wParam)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;


	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);			//API함수, 정보를 가져옴.
	bx = bit.bmWidth / 10;
	by = bit.bmHeight / 2;

	// iOpen이 TRUE인 경우, 사용자가 버튼은 눌렀고 컴퓨터의 패가 공개되어야 할 때.
	if (iOpen == TRUE)
	{
		StretchBlt(hdc, x * 1, y * 3, x * 2, y * 4, MemDC, bx * randPoint[0].x, by * randPoint[0].y, bx, by, SRCCOPY);
		StretchBlt(hdc, x * 4, y * 3, x * 2, y * 4, MemDC, bx * randPoint[1].x, by * randPoint[1].y, bx, by, SRCCOPY);
	}

	StretchBlt(hdc, x * 10, y * 3, x * 2, y * 4, MemDC, bx * randPoint[2].x, by * randPoint[2].y, bx, by, SRCCOPY);
	StretchBlt(hdc, x * 13, y * 3, x * 2, y * 4, MemDC, bx * randPoint[3].x, by * randPoint[3].y, bx, by, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

int Compare(int x, int y)
{
	if (x > y)
		return WIN;
	else if (x == y)
		return DRAW;
	else
		return LOSE;
}

int Uped(int x1, int y1, int x2, int y2)
{
	int sum = 0;
	//38광땡
	if ((x1 == 2 && y1 == 0 && x2 == 7 && y2 == 0) ||
		(x1 == 7 && y1 == 0 && x2 == 2 && y2 == 0))
	{
		return DDANG38;
	}
	//사구파토
	else if (
		(x1 == 3 && y1 == 0 && x2 == 8 && y2 == 0) ||
		(x1 == 8 && y1 == 0 && x2 == 3 && y2 == 0))
	{
		return SAGOOPATOO;
	}
	//암행어사
	else if (
		(x1 == 3 && y1 == 0 && x2 == 6 && y2 == 0) ||
		(x1 == 6 && y1 == 0 && x2 == 3 && y2 == 0))
	{
		return AMHANG;
	}
	//광땡
	else if (
		(x1 == 0 && y1 == 0 && x2 == 2 && y2 == 0) ||
		(x1 == 2 && y1 == 0 && x2 == 0 && y2 == 0) ||
		(x1 == 7 && y1 == 0 && x2 == 0 && y2 == 0) ||
		(x1 == 0 && y1 == 0 && x2 == 7 && y2 == 0))
	{
		return GLOSSDDANG;
	}
	//사구
	else if (
		(x1 == 3 && y1 == 0 && x2 == 8 && y2 == 1) ||
		(x1 == 8 && y1 == 1 && x2 == 3 && y2 == 0) ||
		(x1 == 3 && y1 == 1 && x2 == 8 && y2 == 1) ||
		(x1 == 8 && y1 == 1 && x2 == 3 && y2 == 1) ||
		(x1 == 3 && y1 == 1 && x2 == 8 && y2 == 1) ||
		(x1 == 8 && y1 == 1 && x2 == 3 && y2 == 1))
	{
		return SAGOO;
	}
	//땡잡이
	else if (
		(x1 == 6 && y1 == 0 && x2 == 2 && y2 == 0) ||
		(x1 == 2 && y1 == 0 && x2 == 6 && y2 == 0))
	{
		return DDANGJAB;
	}
	else if (x1 == x2)
	{
		switch (x1)
		{
		case 9:				//장땡
			return DANG10;
			break;
		case 8:				//구땡
			return DANG9;
			break;
		case 7:				//팔땡
			return DANG8;
			break;
		case 6:				//칠땡
			return DANG7;
			break;
		case 5:				//육땡
			return DANG6;
			break;
		case 4:				//오땡
			return DANG5;
			break;
		case 3:				//사땡
			return DANG4;
			break;
		case 2:				//삼땡
			return DANG3;
			break;
		case 1:				//이땡
			return DANG2;
			break;
		case 0:				//일땡
			return DANG1;
			break;
		}
	}
	//알리
	else if (
		(x1 == 0 && y1 == 0 && x2 == 1 && y2 == 0) ||
		(x1 == 0 && y1 == 1 && x2 == 1 && y2 == 1) ||
		(x1 == 0 && y1 == 1 && x2 == 1 && y2 == 0) ||
		(x1 == 0 && y1 == 1 && x2 == 1 && y2 == 1) ||
		(x1 == 1 && y1 == 0 && x2 == 0 && y2 == 0) ||
		(x1 == 1 && y1 == 1 && x2 == 0 && y2 == 0) ||
		(x1 == 1 && y1 == 0 && x2 == 0 && y2 == 1) ||
		(x1 == 1 && y1 == 1 && x2 == 0 && y2 == 1))
	{
		return ARRY;
	}
	//독사
	else if (
		(x1 == 0 && y1 == 0 && x2 == 3 && y2 == 0) ||
		(x1 == 0 && y1 == 0 && x2 == 3 && y2 == 1) ||
		(x1 == 0 && y1 == 1 && x2 == 3 && y2 == 0) ||
		(x1 == 0 && y1 == 1 && x2 == 3 && y2 == 1) ||
		(x1 == 3 && y1 == 0 && x2 == 0 && y2 == 0) ||
		(x1 == 3 && y1 == 1 && x2 == 0 && y2 == 0) ||
		(x1 == 3 && y1 == 0 && x2 == 0 && y2 == 1) ||
		(x1 == 3 && y1 == 1 && x2 == 0 && y2 == 1))
	{
		return SNAKE;
	}
	//구삥
	else if (
		(x1 == 0 && y1 == 0 && x2 == 8 && y2 == 0) ||
		(x1 == 0 && y1 == 0 && x2 == 8 && y2 == 1) ||
		(x1 == 0 && y1 == 1 && x2 == 8 && y2 == 0) ||
		(x1 == 0 && y1 == 1 && x2 == 8 && y2 == 1) ||
		(x1 == 8 && y1 == 0 && x2 == 0 && y2 == 0) ||
		(x1 == 8 && y1 == 1 && x2 == 0 && y2 == 0) ||
		(x1 == 8 && y1 == 0 && x2 == 0 && y2 == 1) ||
		(x1 == 8 && y1 == 1 && x2 == 0 && y2 == 1))
	{
		return  BING9;
	}
	//장삥
	else if (
		(x1 == 0 && y1 == 0 && x2 == 9 && y2 == 0) ||
		(x1 == 0 && y1 == 0 && x2 == 9 && y2 == 1) ||
		(x1 == 0 && y1 == 1 && x2 == 9 && y2 == 0) ||
		(x1 == 0 && y1 == 1 && x2 == 9 && y2 == 1) ||
		(x1 == 9 && y1 == 0 && x2 == 0 && y2 == 0) ||
		(x1 == 9 && y1 == 1 && x2 == 0 && y2 == 0) ||
		(x1 == 9 && y1 == 0 && x2 == 0 && y2 == 1) ||
		(x1 == 9 && y1 == 1 && x2 == 0 && y2 == 1))
	{
		return  BING10;
	}
	//장사
	else if (
		(x1 == 3 && y1 == 0 && x2 == 9 && y2 == 0) ||
		(x1 == 3 && y1 == 0 && x2 == 9 && y2 == 1) ||
		(x1 == 3 && y1 == 1 && x2 == 9 && y2 == 0) ||
		(x1 == 3 && y1 == 1 && x2 == 9 && y2 == 1) ||
		(x1 == 9 && y1 == 0 && x2 == 3 && y2 == 0) ||
		(x1 == 9 && y1 == 1 && x2 == 3 && y2 == 0) ||
		(x1 == 9 && y1 == 0 && x2 == 3 && y2 == 1) ||
		(x1 == 9 && y1 == 1 && x2 == 3 && y2 == 1))
	{
		return  JANGSA;
	}
	//세륙
	else if (
		(x1 == 3 && y1 == 0 && x2 == 5 && y2 == 0) ||
		(x1 == 3 && y1 == 0 && x2 == 5 && y2 == 1) ||
		(x1 == 3 && y1 == 1 && x2 == 5 && y2 == 0) ||
		(x1 == 3 && y1 == 1 && x2 == 5 && y2 == 1) ||
		(x1 == 5 && y1 == 0 && x2 == 3 && y2 == 0) ||
		(x1 == 5 && y1 == 1 && x2 == 3 && y2 == 0) ||
		(x1 == 5 && y1 == 0 && x2 == 3 && y2 == 1) ||
		(x1 == 5 && y1 == 1 && x2 == 3 && y2 == 1))
	{
		return  SAERYUCK;
	}
	//끗
	else
	{
		sum = x1 + x2 + 2;

		if (sum > 10)
			sum = sum - 10;

		switch (sum)
		{
		case 0:				//망통
			return JERO;
			break;
		case 1:				//1끗
			return END1;
			break;
		case 2:				//2끗
			return END2;
			break;
		case 3:				//3끗
			return END3;
			break;
		case 4:				//4끗
			return END4;
			break;
		case 5:				//5끗
			return END5;
			break;
		case 6:				//6끗
			return END6;
			break;
		case 7:				//7끗
			return END7;
			break;
		case 8:				//8끗
			return END8;
			break;
		case 9:				//갑오
			return  GAB0;
			break;
		}

	}
	return -1;
}



void Clear_SutDa()
{
	flag_Start = FALSE;							// 시작 X
	flag_End = TRUE;							// 끝남 O
	state_ServReady = FALSE;					// 서버 준비 X
	state_ClntReady = FALSE;					// 클라이언트 준비 X
	flag_GameStart = FALSE;						// 게임 시작 X
	IsTurn = FALSE;								// 자신의 차례 X


	/*
	EnableMenuItem(hMenu, IDM_START, MF_DISABLED);
	EnableMenuItem(hMenu, IDM_READY, MF_ENABLED);
	EnableMenuItem(hMenu, IDM_CROOM, MF_DISABLED);
	EnableMenuItem(hMenu, IDM_CONNECT, MF_DISABLED);
	EnableMenuItem(hMenu, IDM_DISCONNECT, MF_ENABLED);
	*/
	exit(0);
}

void CheckLevel(HWND hWnd)
{
	// 컴퓨터의 돈이 0보다 작아지는 경우(Level 증가)
	if (Money[0] <= 0)
	{
		rating = rating * 2;
		Money[0] = Money[1] * 2;
		countWin[0] = countLose[0] = 0;
	}
	// 사용자의 돈이 0보다 작아지는 경우.
	else if (Money[1] <= 0)
	{
		MessageBox(hWnd, TEXT("파산"), TEXT("결과"), MB_OK);
		iStart = FALSE;
		iEnd = TRUE;
	}
}