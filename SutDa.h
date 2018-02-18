#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "stdafx.h"
#include "resource.h"
#define PLAYER 2										// �÷��̾� �� 

TCHAR str[128];																		// �ӽ���¹���
TCHAR szIP_ADDRESS[20];																// IP �ּҸ� ������ �迭

POINT randPoint[PLAYER * 2];															// ȭ���� x,y ��ǥ ����

/*					�ڵ� ����										*/
 HWND hWndButton[3];																	// ��ư HWND
 HWND hWndStatic[PLAYER];															// �� Static HWND
 HMENU hMenu, hMenuReady, hMenuStart, hMenuCreate, hMenuConnect, hMenuDisconnect;	// �޴��ڵ�

/*					�̱��÷��� ����									*/
 BOOL iStart = FALSE;																// ������ ���۵ƴ���
 BOOL iEnd = TRUE;																	// ������ ���� �ƴ���
 BOOL iOpen = FALSE;																	// �и� ������ ������
 BOOL iSolo = TRUE;																	// �̱� �÷����ΰ�

/*					��Ʈ��ũ ���� ����								*/
 BOOL IsClient = FALSE;																// Ŭ���̾�Ʈ ����
 BOOL state_ServReady = FALSE;														// ������ �غ� ����
 BOOL state_ClntReady = FALSE;														// Ŭ���̾�Ʈ�� �غ� ����
 BOOL flag_GameStart = FALSE;														// ������ ����
 BOOL IsTurn = FALSE;																// �ڽ��� �������� ����
 BOOL flag_Start = FALSE;															// ���۹�ư�� �������� ����
 BOOL flag_End = FALSE;																// ������ ���� ����
 SOCKET client_sock;																	// Ŭ���̾�Ʈ ����

 int sel;																			// ����
 int rating = 1;																		// ����
 int pointer[20] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };	// x�� ��ǥ�� �迭�� ����
 int randX[PLAYER * 2];																// x��ǥ�� ����
 int Money[PLAYER] = { 0, 0 };														// �÷��̾��� ������
 int countWin[PLAYER];																// �̱� ȸ��
 int countLose[PLAYER];																// �й� ȸ��

 enum { WIN = 300, LOSE, DRAW };														//��ư ID
 enum { PLAYER1_MONEY = 200, PLAYER2_MONEY };											//Static ID
 enum {																					
	DDANG38 = 501, SAGOOPATOO, AMHANG, GLOSSDDANG, SAGOO, DDANGJAB,
	DANG10, DANG9, DANG8, DANG7, DANG6, DANG5, DANG4, DANG3, DANG2, DANG1,
	ARRY, SNAKE, BING9, BING10, JANGSA, SAERYUCK, GAB0,
	END8, END7, END6, END5, END4, END3, END2, END1, JERO
};																					//����� ��

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
	iOpen = TRUE;							// ��ư�� ���ȱ� ������ ��ǻ���� �� ����
	DrawBitmap(hdc, x, y, MyBitmap, wParam);

	if (iSolo == TRUE)						// �̱��÷���
	{
		if (Compare(Uped(randPoint[0].x, randPoint[0].y, randPoint[1].x, randPoint[1].y), Uped(randPoint[2].x, randPoint[2].y, randPoint[3].x, randPoint[3].y))
			== sel) // ������� ���� ����, return ���� ���ٸ� ������� �¸�.
		{
			MessageBox(hWnd, TEXT("�¸�"), TEXT("���"), MB_OK);
			Money[1] += 5000 * rating;			// ������� �� ����
			Money[0] -= 5000 * rating;			// ��ǻ���� �� ����
			countWin[1]++;						// ������� �¸� ����
			countLose[0]++;						// ��ǻ���� �й� ����
		}
		else
		{
			MessageBox(hWnd, TEXT("�й�"), TEXT("���"), MB_OK);
			Money[1] -= 5000 * rating;			// ������� �� ����
			Money[0] += 5000 * rating;			// ��ǻ���� �� ����
			countWin[0]++;						// ��ǻ���� �¸� ����
			countLose[1]++;						// ������� �й� ����
		}
	}
	else if (flag_Start == TRUE)			// ��Ʈ��ũ �÷���
	{
		if (Compare(Uped(randPoint[0].x, randPoint[0].y, randPoint[1].x, randPoint[1].y), Uped(randPoint[2].x, randPoint[2].y, randPoint[3].x, randPoint[3].y)) == WIN)
		{
			MessageBox(hWnd, TEXT("�¸�"), TEXT("���"), MB_OK);
			Money[1] += 5000 * rating;			// ������� �� ����
			Money[0] -= 5000 * rating;			// ��ǻ���� �� ����
			countWin[1]++;						// ������� �¸� ����
			countLose[0]++;						// ��ǻ���� �й� ����
		}
		else if (Compare(Uped(randPoint[0].x, randPoint[0].y, randPoint[1].x, randPoint[1].y), Uped(randPoint[2].x, randPoint[2].y, randPoint[3].x, randPoint[3].y)) == LOSE)
		{
			MessageBox(hWnd, TEXT("�й�"), TEXT("���"), MB_OK);
			Money[1] -= 5000 * rating;			// ������� �� ����
			Money[0] += 5000 * rating;			// ��ǻ���� �� ����
			countWin[0]++;						// ��ǻ���� �¸� ����
			countLose[1]++;						// ������� �й� ����
		}
		else
			MessageBox(hWnd, TEXT("���º�"), TEXT("���"), MB_OK);
	}
	iOpen = FALSE;							// ��ǻ���� �и� �� �� ����.


	if (iStart == TRUE)						// �̱� �÷���
	{
		RandomXY();
		CheckLevel(hWnd);						// Level�� üũ��.
		InvalidateRect(hWnd, NULL, TRUE);
	}

	if (flag_Start == TRUE && IsTurn == TRUE)	// ��Ʈ��ũ �÷���
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
		ret = rand() % 20;			//0~19�� ���� �߻�
		for (i = 0; i<4; i++)
		{
			if (randX[i] == ret)	//���� �ߺ��ȴٸ� Ż��!
				break;
		}
		if (i == 4)					//�ݺ����� �����ٸ� ret�� return
			return ret;
	}
}

void RandomXY()
{
	int randNumY;
	srand((unsigned)time(NULL));

	for (int i = 0; i < 4; i++)
	{
		//GetUniqueNumber() ȣ��
		randX[i] = GetUniqueNumber();
		//�ߺ����� ���� ���� �߻��Ǿ��� ������ pointer[randX[i]] �ε����� ���� randPoint[i].x ������ ������.
		randPoint[i].x = pointer[randX[i]];


		randNumY = rand() % 2;
		randPoint[i].y = randNumY;
	}

	//���� (x1,y1) �� (x2, y2) �� ��ǥ�� ���� ���� ���
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

	GetObject(hBit, sizeof(BITMAP), &bit);			//API�Լ�, ������ ������.
	bx = bit.bmWidth / 10;
	by = bit.bmHeight / 2;

	// iOpen�� TRUE�� ���, ����ڰ� ��ư�� ������ ��ǻ���� �а� �����Ǿ�� �� ��.
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
	//38����
	if ((x1 == 2 && y1 == 0 && x2 == 7 && y2 == 0) ||
		(x1 == 7 && y1 == 0 && x2 == 2 && y2 == 0))
	{
		return DDANG38;
	}
	//�籸����
	else if (
		(x1 == 3 && y1 == 0 && x2 == 8 && y2 == 0) ||
		(x1 == 8 && y1 == 0 && x2 == 3 && y2 == 0))
	{
		return SAGOOPATOO;
	}
	//������
	else if (
		(x1 == 3 && y1 == 0 && x2 == 6 && y2 == 0) ||
		(x1 == 6 && y1 == 0 && x2 == 3 && y2 == 0))
	{
		return AMHANG;
	}
	//����
	else if (
		(x1 == 0 && y1 == 0 && x2 == 2 && y2 == 0) ||
		(x1 == 2 && y1 == 0 && x2 == 0 && y2 == 0) ||
		(x1 == 7 && y1 == 0 && x2 == 0 && y2 == 0) ||
		(x1 == 0 && y1 == 0 && x2 == 7 && y2 == 0))
	{
		return GLOSSDDANG;
	}
	//�籸
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
	//������
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
		case 9:				//�嶯
			return DANG10;
			break;
		case 8:				//����
			return DANG9;
			break;
		case 7:				//�ȶ�
			return DANG8;
			break;
		case 6:				//ĥ��
			return DANG7;
			break;
		case 5:				//����
			return DANG6;
			break;
		case 4:				//����
			return DANG5;
			break;
		case 3:				//�綯
			return DANG4;
			break;
		case 2:				//�ﶯ
			return DANG3;
			break;
		case 1:				//�̶�
			return DANG2;
			break;
		case 0:				//�϶�
			return DANG1;
			break;
		}
	}
	//�˸�
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
	//����
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
	//����
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
	//���
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
	//���
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
	//����
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
	//��
	else
	{
		sum = x1 + x2 + 2;

		if (sum > 10)
			sum = sum - 10;

		switch (sum)
		{
		case 0:				//����
			return JERO;
			break;
		case 1:				//1��
			return END1;
			break;
		case 2:				//2��
			return END2;
			break;
		case 3:				//3��
			return END3;
			break;
		case 4:				//4��
			return END4;
			break;
		case 5:				//5��
			return END5;
			break;
		case 6:				//6��
			return END6;
			break;
		case 7:				//7��
			return END7;
			break;
		case 8:				//8��
			return END8;
			break;
		case 9:				//����
			return  GAB0;
			break;
		}

	}
	return -1;
}



void Clear_SutDa()
{
	flag_Start = FALSE;							// ���� X
	flag_End = TRUE;							// ���� O
	state_ServReady = FALSE;					// ���� �غ� X
	state_ClntReady = FALSE;					// Ŭ���̾�Ʈ �غ� X
	flag_GameStart = FALSE;						// ���� ���� X
	IsTurn = FALSE;								// �ڽ��� ���� X


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
	// ��ǻ���� ���� 0���� �۾����� ���(Level ����)
	if (Money[0] <= 0)
	{
		rating = rating * 2;
		Money[0] = Money[1] * 2;
		countWin[0] = countLose[0] = 0;
	}
	// ������� ���� 0���� �۾����� ���.
	else if (Money[1] <= 0)
	{
		MessageBox(hWnd, TEXT("�Ļ�"), TEXT("���"), MB_OK);
		iStart = FALSE;
		iEnd = TRUE;
	}
}