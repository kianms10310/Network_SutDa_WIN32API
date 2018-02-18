// Win32Project1.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "SutDa.h"

#define MAX_LOADSTRING 100
#define PORT 5412										// ��Ʈ��ȣ

// ����� ���� Windows �޽���
#define WM_CONNECT (WM_USER + 0)						
#define WM_RECEIVE (WM_USER + 1)
#define WM_ACCEPT (WM_USER + 2)

// ���� ����:
HINSTANCE hInst;																	// ���� �ν��Ͻ��Դϴ�.

TCHAR szWindowClass[MAX_LOADSTRING];												// �⺻ â Ŭ���� �̸��Դϴ�.
										//����� ��

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK IPad(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, TEXT("����"), WS_SYSMENU,
      CW_USEDEFAULT, 0, 500, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	static int OptionValue = SO_SYNCHRONOUS_NONALERT;		// �˻� �ɼ� ������ ���� ���� ������
	static SOCKET listen_sock;								// listen ����
	static POINT pMain;									//ClientArea�� x��, y������ ���� ���� �����ϴ� ����
	static RECT rtMain;									//ClientArea�� �޾ƿ��� ���� ����
	static HBITMAP MyBitmap;							//ȭ����.bmp�� Bitmap �ҷ����� ���� ����
	static RECT drawRect;

	WSADATA wsaData;										// ���� �ʱ�ȭ�� ���
	SOCKADDR_IN serveraddr, clientaddr, acceptaddr;			// ����, Ŭ���̾�Ʈ, accept �ּ� �� �����ϴ� ����ü

	int return_value;										// ��ȯ��
	char buffer[300];
	int pos;
	int addrsize;
	PAINTSTRUCT ps;
	HDC hdc;

	HBRUSH MyBrush, OldBrush;							//Brush ���� ����

	switch (message)
	{
	case WM_CREATE:
		MyBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetClientRect(hWnd, &rtMain);
		pMain.x = rtMain.right / 16;
		pMain.y = rtMain.bottom / 21;

		hWndButton[0] = CreateWindow(TEXT("button"), TEXT("WIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			pMain.x * 8, pMain.y * 12, pMain.x * 7, pMain.y * 2, hWnd, (HMENU)WIN, hInst, NULL);

		hWndButton[1] = CreateWindow(TEXT("button"), TEXT("DRAW"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			pMain.x * 8, pMain.y * 15, pMain.x * 7, pMain.y * 2, hWnd, (HMENU)DRAW, hInst, NULL);

		hWndButton[2] = CreateWindow(TEXT("button"), TEXT("LOSE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			pMain.x * 8, pMain.y * 18, pMain.x * 7, pMain.y * 2, hWnd, (HMENU)LOSE, hInst, NULL);

		hWndStatic[0] = CreateWindow(TEXT("static"), TEXT("P1 : "), WS_CHILD | WS_VISIBLE,
			pMain.x, pMain.y * 12, pMain.x * 5, pMain.y, hWnd, (HMENU)PLAYER1_MONEY, hInst, NULL);

		hWndStatic[1] = CreateWindow(TEXT("static"), TEXT("Me : "), WS_CHILD | WS_VISIBLE,
			pMain.x, pMain.y * 18, pMain.x * 5, pMain.y, hWnd, (HMENU)PLAYER2_MONEY, hInst, NULL);

		hMenu = GetMenu(hWnd);
		EnableMenuItem(hMenu, IDM_READY, MF_DISABLED);
		EnableMenuItem(hMenu, IDM_START, MF_DISABLED);
		EnableMenuItem(hMenu, IDM_DISCONNECT, MF_DISABLED);

		drawRect.left = pMain.x * 6;
		drawRect.top = pMain.y * 5;
		drawRect.right = pMain.x * 10;
		drawRect.bottom = pMain.y * 6;

		break;

	case WM_CONNECT:

		serveraddr.sin_family = AF_INET;						// TCP/IP
		serveraddr.sin_port = htons(PORT);						// ��Ʈ��ȣ
		serveraddr.sin_addr.s_addr = inet_addr(szIP_ADDRESS);	// �Է� ���� IP
		WSAAsyncSelect(client_sock, hWnd, NULL, NULL);			//Ŭ���̾�Ʈ ����

		connect(client_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

		// ���� üũ
		return_value = WSAGetLastError();

		// ������ ���� ���
		if (return_value == WSAEISCONN)
		{
			// ������ �޽��� ��� 
			WSAAsyncSelect(client_sock, hWnd, WM_RECEIVE, FD_READ | FD_OOB | FD_CLOSE);
			MessageBox(hWnd, TEXT("Connect Success!!"), TEXT("Success"), MB_OK);

			// ���� �ʱ�ȭ
			IsClient = TRUE;
			IsTurn = FALSE;
			// �޴� ��Ȱ��ȭ �� Ȱ��ȭ
			EnableMenuItem(hMenu, IDM_READY, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CROOM, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_CONNECT, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_DISCONNECT, MF_ENABLED);
		}

		// ���� �߻���
		else
		{
			MessageBox(hWnd, TEXT("Connect() Error!"), TEXT("Error"), MB_OK);
			closesocket(client_sock);
			WSACleanup();
		}
		break;

	case WM_RECEIVE:
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
			recv(client_sock, buffer, 128, 0);			// ���ۿ� �޾ƿ�.

			if (strcmp(buffer, "ready") == 0)			// ��밡 Ready�� ��.
			{
				state_ClntReady = TRUE;
				MessageBox(hWnd, TEXT("Ready"), TEXT("Notice"), MB_OK);
			}
			else if (strcmp(buffer, "start") == 0)			// ������ ������ ������
			{
				flag_GameStart = TRUE;
				flag_Start = TRUE;
				flag_End = FALSE;
				iSolo = FALSE;
				rating = 1;
				Money[0] = Money[1] = 30000;
				countWin[0] = countWin[1] = countLose[0] = countLose[1] = 0;
				MessageBox(hWnd, TEXT("Start Game!!"), TEXT("Notice"), MB_OK);
				InvalidateRect(hWnd, NULL, TRUE);
				ShowWindow(hWndButton[1], SW_HIDE);			// ��Ʈ��ũ ���ӿ����� DRAW �����
				SetWindowText(hWndButton[0], "GO");			// WIN ��ư�� GO�� �ٲٱ�
				SetWindowText(hWndButton[2], "DIE");		// LOSE ��ư�� ���̷� �ٲٱ�
			}
			else if (strcmp(buffer, "go") == 0)			// ��밡 Go��ư�� ����.
			{
				hdc = GetDC(hWnd);
				IsTurn = TRUE;
				iOpen = FALSE;
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
				ReleaseDC(hWnd, hdc);
			}
			else if (strcmp(buffer, "die") == 0)		// ��밡 die��ư�� ����.
			{
				IsTurn = TRUE;
				iOpen = FALSE;

				Money[0] -= 5000 * rating;
				Money[1] += 5000 * rating;
				countLose[0]++;
				countWin[1]++;
				RandomXY();								// �и� ����.
														// ��ǥ���� 8�ڸ� ������ ����.
				pos = randPoint[0].x * 10000000 + randPoint[0].y * 1000000 +
					randPoint[1].x * 100000 + randPoint[1].y * 10000 +
					randPoint[2].x * 1000 + randPoint[2].y * 100 +
					randPoint[3].x * 10 + randPoint[3].y;
				_itoa(pos, buffer, 10);					// 8�ڸ� �������� buffer�� �ֱ����� ���������� �ٲ�.
				send(client_sock, buffer, 10, 0);		// Send

				InvalidateRect(hWnd, NULL, TRUE);
			}
			else										// ������ ������ ���ڸ� pos ������ ����
			{
				pos = atoi(buffer);						// ���ۿ� ���� ���� ���� ���� ���� ���� ����.
				randPoint[3].x = pos / 10000000;
				pos = pos - randPoint[3].x * 10000000;

				randPoint[3].y = pos / 1000000;
				pos = pos - randPoint[3].y * 1000000;

				randPoint[2].x = pos / 100000;
				pos = pos - randPoint[2].x * 100000;

				randPoint[2].y = pos / 10000;
				pos = pos - randPoint[2].y * 10000;

				randPoint[1].x = pos / 1000;
				pos = pos - randPoint[1].x * 1000;

				randPoint[1].y = pos / 100;
				pos = pos - randPoint[1].y * 100;

				randPoint[0].x = pos / 10;
				pos = pos - randPoint[0].x * 10;

				randPoint[0].y = pos;
				iOpen = FALSE;
				InvalidateRect(hWnd, NULL, TRUE);
			}

			break;
		
		case FD_CLOSE:				// ���� �޽���
			MessageBox(hWnd, TEXT("DisConnect"), TEXT("Notice"), MB_ICONSTOP);

			// ���� CleanUP
			WSACleanup();

			// ���� �� ���� ���� �ʱ�ȭ
			IsClient = FALSE;
			//�޴� ����� �� Ȱ��ȭ
			EnableMenuItem(hMenu, IDM_START, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_READY, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_CROOM, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CONNECT, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_DISCONNECT, MF_DISABLED);

			break;
		}
		break;

		
	case WM_ACCEPT:					// �������� ���� ���� ��û�� �޾��� ���
		addrsize = sizeof(acceptaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&acceptaddr, &addrsize);
		WSAAsyncSelect(client_sock, hWnd, WM_RECEIVE, FD_WRITE | FD_READ | FD_OOB | FD_CLOSE);
		MessageBox(hWnd, TEXT("���� �Ǿ����ϴ�."), TEXT("�˸�"), MB_OK);
		EnableMenuItem(hMenu, IDM_READY, MF_ENABLED);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		
		case WIN:						// WIN ��ư�� ���� ���
			hdc = GetDC(hWnd);
			sel = WIN;
			if (iStart == TRUE && iSolo == TRUE)	// ����ڰ� ���� ������ ���� ���
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
			else if (flag_Start == TRUE && IsTurn == TRUE)	// go��ư�� ���� ���.
			{
				send(client_sock, "go", 10, 0);
				IsTurn = FALSE;
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
			}
			else if (IsTurn == FALSE)
			{
				MessageBox(hWnd, TEXT("��� ���ʰ� �ƴմϴ�."), TEXT("�˸�"), MB_OK);
			}
			else
				MessageBox(hWnd, TEXT("������ ���۵��� �ʾҽ��ϴ�."), TEXT("�˸�"), MB_OK);
			
			ReleaseDC(hWnd, hdc);

			break;
			
		case LOSE:						// LOSE ��ư�� ���� ���
			hdc = GetDC(hWnd);
			sel = LOSE;
			if (iStart == TRUE && iSolo == TRUE)	// ����ڰ� ���� ������ ���� ���
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
			else if (flag_Start == TRUE && IsTurn == TRUE)	// die��ư�� ���� ���.
			{
				IsTurn = FALSE;
				iOpen = FALSE;
				InvalidateRect(hWnd, NULL, TRUE);
				send(client_sock, "die", 10, 0);
				Money[1] -= 5000 * rating;
				Money[0] += 5000 * rating;
				countLose[1]++;
				countWin[0]++;
				if (Money[1] < 0)
				{
					Clear_SutDa();					// ���� ����
				}
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (IsTurn == FALSE)
			{
				MessageBox(hWnd, TEXT("��� ���ʰ� �ƴմϴ�."), TEXT("�˸�"), MB_OK);
			}
			else
				MessageBox(hWnd, TEXT("������ ���۵��� �ʾҽ��ϴ�."), TEXT("�˸�"), MB_OK);
			ReleaseDC(hWnd, hdc);
			break;
		case DRAW:						// DRAW ��ư�� ���� ���
			hdc = GetDC(hWnd);
			sel = DRAW;
			if (iStart == TRUE && iSolo == TRUE)	// ����ڰ� ���� ������ ���� ���
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
			else
				MessageBox(hWnd, TEXT("������ ���۵��� �ʾҽ��ϴ�."), TEXT("�˸�"), MB_OK);
			ReleaseDC(hWnd, hdc);
			break;
		
		case IDM_SOLO_START:			// �̱� �÷��� ���۸޴�
			if (iStart == FALSE)		// ������ �������� �ƴ� ���
			{
				iStart = TRUE;
				iEnd = FALSE;
				iOpen = FALSE;
				iSolo = TRUE;
				RandomXY();
				rating = 1;
				Money[0] = Money[1] = 30000;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (flag_Start == TRUE)
			{
				MessageBox(hWnd, TEXT("��Ʈ��ũ ���� ��"), TEXT("����"), MB_OK);
			}
			else
				MessageBox(hWnd, TEXT("�̱� ���� ��"), TEXT("����"), MB_OK);
			break;
		case IDM_START:					// ���� �޴� Ŭ����
			// ������ Ŭ���̾�Ʈ ��� �غ� ���¶��
			if (state_ClntReady == TRUE && state_ServReady == TRUE)
			{
				// ������ ���
				if (IsClient == FALSE)
				{
					iSolo = FALSE;
					flag_GameStart = TRUE;
					flag_Start = TRUE;
					send(client_sock, "start", 10, 0);
					IsTurn = TRUE;						//������ ���� ������ �����Ѵ�
					flag_End = FALSE;
					Money[0] = Money[1] = 30000;
					rating = 1;
					countWin[0] = countWin[1] = countLose[0] = countLose[1] = 0;
					InvalidateRect(hWnd, NULL, TRUE);
					RandomXY();
					EnableMenuItem(hMenu, IDM_READY, MF_DISABLED);
					EnableMenuItem(hMenu, IDM_START, MF_DISABLED);
					ShowWindow(hWndButton[1], SW_HIDE);			// ��Ʈ��ũ ���ӿ����� DRAW �����
					SetWindowText(hWndButton[0], "GO");			// WIN ��ư�� GO�� �ٲٱ�
					SetWindowText(hWndButton[2], "DIE");		// LOSE ��ư�� ���̷� �ٲٱ�


					pos = randPoint[0].x * 10000000 + randPoint[0].y * 1000000 +
						randPoint[1].x * 100000 + randPoint[1].y * 10000 +
						randPoint[2].x * 1000 + randPoint[2].y * 100 +
						randPoint[3].x * 10 + randPoint[3].y;
					_itoa(pos, buffer, 10);
					send(client_sock, buffer, 10, 0);

					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			break;
		case IDM_READY:					// �غ� �޴� Ŭ����
			if (IsClient == TRUE)		//Ŭ���̾�Ʈ ���
			{					
				send(client_sock, "ready", 10, 0);	//�������� �غ� �Ǿ����� �˸�
			}
			else
			{										//�������
				state_ServReady = TRUE;
				EnableMenuItem(hMenu, IDM_START, MF_ENABLED);
			}
			EnableMenuItem(hMenu, IDM_READY, MF_DISABLED);
			break;
		case IDM_DISCONNECT:			// ���� ���� ��ư ���ý�
			MessageBox(hWnd, TEXT("Disconnect"), TEXT("Notice"), MB_ICONSTOP);
			closesocket(client_sock);
			closesocket(listen_sock);
			WSACleanup();
			IsClient = FALSE;
			EnableMenuItem(hMenu, IDM_START, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_READY, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CROOM, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CONNECT, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_DISCONNECT, MF_ENABLED);
			break;
		case IDM_CROOM:					// �游��� �޴� Ŭ��

			// �̱� ���� ���� ���
			if (iStart == TRUE)
			{
				MessageBox(hWnd, TEXT("�̱� ���� ��"), TEXT("����"), MB_OK);
				break;
			}

			//���� �ʱ�ȭ
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			//���� �ɼ� ����
			setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, (char*)&OptionValue, sizeof(int));

			//���� ����(TCP)
			listen_sock = socket(AF_INET, SOCK_STREAM, 0);

			//�ּ�ü�� ����
			clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			clientaddr.sin_family = AF_INET;
			clientaddr.sin_port = htons(PORT);

			//���ε�
			return_value = bind(listen_sock, (SOCKADDR*)&clientaddr, sizeof(clientaddr));
			if (return_value == SOCKET_ERROR)
			{
				MessageBox(hWnd, TEXT("Bind() Error!"), TEXT("Error"), MB_OK);
				PostQuitMessage(0);			//������ ����(�ӽ�)-���߿� �ٲ�
			}
			//������ �޽��� ���(LISTEN ���Ͽ� ���� ��û�� ������ WM_ACCEPT �߻�)
			WSAAsyncSelect(listen_sock, hWnd, WM_ACCEPT, FD_ACCEPT);

			//1�� �����Ѵ�.
			listen(listen_sock, 1);

			//�޽��� �ڽ�!
			MessageBox(hWnd, TEXT("Create ROOM Success!!"), TEXT("Success"), MB_OK);

			//�� ����� ��ư ��Ȱ��ȭ(���� ��������Ƿ�)
			EnableMenuItem(hMenu, IDM_CROOM, MF_DISABLED);

			//�����ϱ� ��ư ��Ȱ��ȭ(������ �ǹǷ� Ŭ���̾�Ʈ�� ��ư�� ���ش�)
			EnableMenuItem(hMenu, IDM_CONNECT, MF_DISABLED);

			break;
		case IDM_CONNECT:				// �����ϱ� ��ư Ŭ��
			if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, IPad))
			{
				//���� �ʱ�ȭ
				WSAStartup(MAKEWORD(2, 2), &wsaData);

				setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, (char*)&OptionValue, sizeof(int));

				// Ŭ���̾�Ʈ ���� ����
				client_sock = socket(AF_INET, SOCK_STREAM, 0);

				// �ּ�ü�� ����
				serveraddr.sin_addr.s_addr = inet_addr(szIP_ADDRESS);		// ���̾�α׷� �Է¹��� ip�ּ�
				serveraddr.sin_family = AF_INET;						// TCP/IP
				serveraddr.sin_port = htons(PORT);						// ��Ʈ��ȣ

				// ������ �޽��� ���
				WSAAsyncSelect(client_sock, hWnd, WM_CONNECT, FD_CONNECT);

				// �����û
				connect(client_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		//�ʷ�
		MyBrush = (HBRUSH)CreateSolidBrush(RGB(105, 155, 055));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, 0, 0, rtMain.right, pMain.y * 11);

		//���
		MyBrush = (HBRUSH)CreateSolidBrush(RGB(222, 158, 023));
		(HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, 0, pMain.y * 11, pMain.x * 7, rtMain.bottom);

		//�Ķ�
		MyBrush = (HBRUSH)CreateSolidBrush(RGB(231, 244, 246));
		(HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, pMain.x * 7, pMain.y * 11, rtMain.right, rtMain.bottom);
		SetBkMode(hdc, TRANSPARENT);
		
		//���������ÿ��� �и� ������.
		if (iStart == TRUE || flag_Start == TRUE)
			DrawBitmap(hdc, pMain.x, pMain.y, MyBitmap, wParam);
		
		wsprintf(str, TEXT("P1 : %d"), Money[0]);
		SetWindowText(hWndStatic[0], str);

		wsprintf(str, TEXT("ME : %d"), Money[1]);
		SetWindowText(hWndStatic[1], str);

		wsprintf(str, TEXT("�� : %d      �� : %d"), countWin[0], countLose[0]);
		TextOut(hdc, pMain.x, pMain.y * 13, str, lstrlen(str));

		wsprintf(str, TEXT("�� : %d      �� : %d"), countWin[1], countLose[1]);
		TextOut(hdc, pMain.x, pMain.y * 19, str, lstrlen(str));

		//�̱� �÷��� �ÿ��� ���� ������.
		if (iSolo == TRUE)
		{
			wsprintf(str, TEXT("���� : x%d"), rating);
			TextOut(hdc, pMain.x * 7, pMain.y * 5, str, lstrlen(str));
		}

		SelectObject(hdc, OldBrush);
		DeleteObject(MyBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// IP �Է� ���̾� �α�
INT_PTR CALLBACK IPad(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{


	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_OK){
			GetDlgItemText(hDlg, IDC_EDIT1, szIP_ADDRESS, 20);

			EndDialog(hDlg, ID_OK);
			return TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

