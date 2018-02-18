// Win32Project1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "SutDa.h"

#define MAX_LOADSTRING 100
#define PORT 5412										// 포트번호

// 사용자 정의 Windows 메시지
#define WM_CONNECT (WM_USER + 0)						
#define WM_RECEIVE (WM_USER + 1)
#define WM_ACCEPT (WM_USER + 2)

// 전역 변수:
HINSTANCE hInst;																	// 현재 인스턴스입니다.

TCHAR szWindowClass[MAX_LOADSTRING];												// 기본 창 클래스 이름입니다.
										//경우의 수

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// 기본 메시지 루프입니다.
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

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, TEXT("섯다"), WS_SYSMENU,
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

	static int OptionValue = SO_SYNCHRONOUS_NONALERT;		// 검색 옵션 저장을 위한 버퍼 포인팅
	static SOCKET listen_sock;								// listen 소켓
	static POINT pMain;									//ClientArea를 x축, y축으로 나눈 값을 저장하는 변수
	static RECT rtMain;									//ClientArea를 받아오기 위한 변수
	static HBITMAP MyBitmap;							//화투패.bmp를 Bitmap 불러오기 위한 변수
	static RECT drawRect;

	WSADATA wsaData;										// 윈속 초기화시 사용
	SOCKADDR_IN serveraddr, clientaddr, acceptaddr;			// 서버, 클라이언트, accept 주소 값 저장하는 구조체

	int return_value;										// 반환값
	char buffer[300];
	int pos;
	int addrsize;
	PAINTSTRUCT ps;
	HDC hdc;

	HBRUSH MyBrush, OldBrush;							//Brush 설정 변수

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
		serveraddr.sin_port = htons(PORT);						// 포트번호
		serveraddr.sin_addr.s_addr = inet_addr(szIP_ADDRESS);	// 입력 받은 IP
		WSAAsyncSelect(client_sock, hWnd, NULL, NULL);			//클라이언트 소켓

		connect(client_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

		// 에러 체크
		return_value = WSAGetLastError();

		// 에러가 없을 경우
		if (return_value == WSAEISCONN)
		{
			// 윈도우 메시지 등록 
			WSAAsyncSelect(client_sock, hWnd, WM_RECEIVE, FD_READ | FD_OOB | FD_CLOSE);
			MessageBox(hWnd, TEXT("Connect Success!!"), TEXT("Success"), MB_OK);

			// 변수 초기화
			IsClient = TRUE;
			IsTurn = FALSE;
			// 메뉴 비활성화 및 활성화
			EnableMenuItem(hMenu, IDM_READY, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CROOM, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_CONNECT, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_DISCONNECT, MF_ENABLED);
		}

		// 에러 발생시
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
			recv(client_sock, buffer, 128, 0);			// 버퍼에 받아옴.

			if (strcmp(buffer, "ready") == 0)			// 상대가 Ready를 함.
			{
				state_ClntReady = TRUE;
				MessageBox(hWnd, TEXT("Ready"), TEXT("Notice"), MB_OK);
			}
			else if (strcmp(buffer, "start") == 0)			// 서버가 게임을 시작함
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
				ShowWindow(hWndButton[1], SW_HIDE);			// 네트워크 게임에서는 DRAW 지우기
				SetWindowText(hWndButton[0], "GO");			// WIN 버튼을 GO로 바꾸기
				SetWindowText(hWndButton[2], "DIE");		// LOSE 버튼을 다이로 바꾸기
			}
			else if (strcmp(buffer, "go") == 0)			// 상대가 Go버튼을 누름.
			{
				hdc = GetDC(hWnd);
				IsTurn = TRUE;
				iOpen = FALSE;
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
				ReleaseDC(hWnd, hdc);
			}
			else if (strcmp(buffer, "die") == 0)		// 상대가 die버튼을 누름.
			{
				IsTurn = TRUE;
				iOpen = FALSE;

				Money[0] -= 5000 * rating;
				Money[1] += 5000 * rating;
				countLose[0]++;
				countWin[1]++;
				RandomXY();								// 패를 섞음.
														// 좌표값을 8자리 정수로 놓음.
				pos = randPoint[0].x * 10000000 + randPoint[0].y * 1000000 +
					randPoint[1].x * 100000 + randPoint[1].y * 10000 +
					randPoint[2].x * 1000 + randPoint[2].y * 100 +
					randPoint[3].x * 10 + randPoint[3].y;
				_itoa(pos, buffer, 10);					// 8자리 정수형을 buffer에 넣기위해 문자형으로 바꿈.
				send(client_sock, buffer, 10, 0);		// Send

				InvalidateRect(hWnd, NULL, TRUE);
			}
			else										// 상대방이 선택한 문자를 pos 변수에 저장
			{
				pos = atoi(buffer);						// 버퍼에 받은 값을 나눠 몫을 각각 값에 넣음.
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
		
		case FD_CLOSE:				// 종료 메시지
			MessageBox(hWnd, TEXT("DisConnect"), TEXT("Notice"), MB_ICONSTOP);

			// 윈속 CleanUP
			WSACleanup();

			// 변수 및 각종 변수 초기화
			IsClient = FALSE;
			//메뉴 숨기기 및 활성화
			EnableMenuItem(hMenu, IDM_START, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_READY, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_CROOM, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CONNECT, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_DISCONNECT, MF_DISABLED);

			break;
		}
		break;

		
	case WM_ACCEPT:					// 상대방으로 부터 연결 요청을 받았을 경우
		addrsize = sizeof(acceptaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&acceptaddr, &addrsize);
		WSAAsyncSelect(client_sock, hWnd, WM_RECEIVE, FD_WRITE | FD_READ | FD_OOB | FD_CLOSE);
		MessageBox(hWnd, TEXT("연결 되었습니다."), TEXT("알림"), MB_OK);
		EnableMenuItem(hMenu, IDM_READY, MF_ENABLED);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		
		case WIN:						// WIN 버튼을 누른 경우
			hdc = GetDC(hWnd);
			sel = WIN;
			if (iStart == TRUE && iSolo == TRUE)	// 사용자가 게임 시작을 누른 경우
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
			else if (flag_Start == TRUE && IsTurn == TRUE)	// go버튼을 누른 경우.
			{
				send(client_sock, "go", 10, 0);
				IsTurn = FALSE;
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
			}
			else if (IsTurn == FALSE)
			{
				MessageBox(hWnd, TEXT("당신 차례가 아닙니다."), TEXT("알림"), MB_OK);
			}
			else
				MessageBox(hWnd, TEXT("게임이 시작되지 않았습니다."), TEXT("알림"), MB_OK);
			
			ReleaseDC(hWnd, hdc);

			break;
			
		case LOSE:						// LOSE 버튼을 누른 경우
			hdc = GetDC(hWnd);
			sel = LOSE;
			if (iStart == TRUE && iSolo == TRUE)	// 사용자가 게임 시작을 누른 경우
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
			else if (flag_Start == TRUE && IsTurn == TRUE)	// die버튼을 누른 경우.
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
					Clear_SutDa();					// 게임 종료
				}
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (IsTurn == FALSE)
			{
				MessageBox(hWnd, TEXT("당신 차례가 아닙니다."), TEXT("알림"), MB_OK);
			}
			else
				MessageBox(hWnd, TEXT("게임이 시작되지 않았습니다."), TEXT("알림"), MB_OK);
			ReleaseDC(hWnd, hdc);
			break;
		case DRAW:						// DRAW 버튼을 누른 경우
			hdc = GetDC(hWnd);
			sel = DRAW;
			if (iStart == TRUE && iSolo == TRUE)	// 사용자가 게임 시작을 누른 경우
				resultPrint(hWnd, hdc, MyBitmap, pMain.x, pMain.y, wParam);
			else
				MessageBox(hWnd, TEXT("게임이 시작되지 않았습니다."), TEXT("알림"), MB_OK);
			ReleaseDC(hWnd, hdc);
			break;
		
		case IDM_SOLO_START:			// 싱글 플레이 시작메뉴
			if (iStart == FALSE)		// 게임이 진행중이 아닐 경우
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
				MessageBox(hWnd, TEXT("네트워크 게임 중"), TEXT("오류"), MB_OK);
			}
			else
				MessageBox(hWnd, TEXT("싱글 게임 중"), TEXT("오류"), MB_OK);
			break;
		case IDM_START:					// 시작 메뉴 클릭시
			// 서버와 클라이언트 모두 준비 상태라면
			if (state_ClntReady == TRUE && state_ServReady == TRUE)
			{
				// 서버일 경우
				if (IsClient == FALSE)
				{
					iSolo = FALSE;
					flag_GameStart = TRUE;
					flag_Start = TRUE;
					send(client_sock, "start", 10, 0);
					IsTurn = TRUE;						//서버가 먼저 게임을 시작한다
					flag_End = FALSE;
					Money[0] = Money[1] = 30000;
					rating = 1;
					countWin[0] = countWin[1] = countLose[0] = countLose[1] = 0;
					InvalidateRect(hWnd, NULL, TRUE);
					RandomXY();
					EnableMenuItem(hMenu, IDM_READY, MF_DISABLED);
					EnableMenuItem(hMenu, IDM_START, MF_DISABLED);
					ShowWindow(hWndButton[1], SW_HIDE);			// 네트워크 게임에서는 DRAW 지우기
					SetWindowText(hWndButton[0], "GO");			// WIN 버튼을 GO로 바꾸기
					SetWindowText(hWndButton[2], "DIE");		// LOSE 버튼을 다이로 바꾸기


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
		case IDM_READY:					// 준비 메뉴 클릭시
			if (IsClient == TRUE)		//클라이언트 라면
			{					
				send(client_sock, "ready", 10, 0);	//서버에게 준비가 되었음을 알림
			}
			else
			{										//서버라면
				state_ServReady = TRUE;
				EnableMenuItem(hMenu, IDM_START, MF_ENABLED);
			}
			EnableMenuItem(hMenu, IDM_READY, MF_DISABLED);
			break;
		case IDM_DISCONNECT:			// 연결 종료 버튼 선택시
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
		case IDM_CROOM:					// 방만들기 메뉴 클릭

			// 싱글 게임 중일 경우
			if (iStart == TRUE)
			{
				MessageBox(hWnd, TEXT("싱글 게임 중"), TEXT("오류"), MB_OK);
				break;
			}

			//윈속 초기화
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			//소켓 옵션 변경
			setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, (char*)&OptionValue, sizeof(int));

			//소켓 생성(TCP)
			listen_sock = socket(AF_INET, SOCK_STREAM, 0);

			//주소체계 세팅
			clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			clientaddr.sin_family = AF_INET;
			clientaddr.sin_port = htons(PORT);

			//바인딩
			return_value = bind(listen_sock, (SOCKADDR*)&clientaddr, sizeof(clientaddr));
			if (return_value == SOCKET_ERROR)
			{
				MessageBox(hWnd, TEXT("Bind() Error!"), TEXT("Error"), MB_OK);
				PostQuitMessage(0);			//에러시 종료(임시)-나중에 바꿈
			}
			//윈도우 메시지 등록(LISTEN 소켓에 연결 요청이 들어오면 WM_ACCEPT 발생)
			WSAAsyncSelect(listen_sock, hWnd, WM_ACCEPT, FD_ACCEPT);

			//1명만 수용한다.
			listen(listen_sock, 1);

			//메시지 박스!
			MessageBox(hWnd, TEXT("Create ROOM Success!!"), TEXT("Success"), MB_OK);

			//방 만들기 버튼 비활성화(방을 만들었으므로)
			EnableMenuItem(hMenu, IDM_CROOM, MF_DISABLED);

			//연결하기 버튼 비활성화(서버가 되므로 클라이언트용 버튼은 없앤다)
			EnableMenuItem(hMenu, IDM_CONNECT, MF_DISABLED);

			break;
		case IDM_CONNECT:				// 연결하기 버튼 클릭
			if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, IPad))
			{
				//윈속 초기화
				WSAStartup(MAKEWORD(2, 2), &wsaData);

				setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, (char*)&OptionValue, sizeof(int));

				// 클라이언트 소켓 생성
				client_sock = socket(AF_INET, SOCK_STREAM, 0);

				// 주소체계 설정
				serveraddr.sin_addr.s_addr = inet_addr(szIP_ADDRESS);		// 다이얼로그로 입력받은 ip주소
				serveraddr.sin_family = AF_INET;						// TCP/IP
				serveraddr.sin_port = htons(PORT);						// 포트번호

				// 윈도우 메시지 등록
				WSAAsyncSelect(client_sock, hWnd, WM_CONNECT, FD_CONNECT);

				// 연결요청
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
		// TODO: 여기에 그리기 코드를 추가합니다.
		//초록
		MyBrush = (HBRUSH)CreateSolidBrush(RGB(105, 155, 055));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, 0, 0, rtMain.right, pMain.y * 11);

		//노랑
		MyBrush = (HBRUSH)CreateSolidBrush(RGB(222, 158, 023));
		(HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, 0, pMain.y * 11, pMain.x * 7, rtMain.bottom);

		//파랑
		MyBrush = (HBRUSH)CreateSolidBrush(RGB(231, 244, 246));
		(HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, pMain.x * 7, pMain.y * 11, rtMain.right, rtMain.bottom);
		SetBkMode(hdc, TRANSPARENT);
		
		//시작했을시에만 패를 보여줌.
		if (iStart == TRUE || flag_Start == TRUE)
			DrawBitmap(hdc, pMain.x, pMain.y, MyBitmap, wParam);
		
		wsprintf(str, TEXT("P1 : %d"), Money[0]);
		SetWindowText(hWndStatic[0], str);

		wsprintf(str, TEXT("ME : %d"), Money[1]);
		SetWindowText(hWndStatic[1], str);

		wsprintf(str, TEXT("승 : %d      패 : %d"), countWin[0], countLose[0]);
		TextOut(hdc, pMain.x, pMain.y * 13, str, lstrlen(str));

		wsprintf(str, TEXT("승 : %d      패 : %d"), countWin[1], countLose[1]);
		TextOut(hdc, pMain.x, pMain.y * 19, str, lstrlen(str));

		//싱글 플레이 시에만 배팅 보여줌.
		if (iSolo == TRUE)
		{
			wsprintf(str, TEXT("배팅 : x%d"), rating);
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

// 정보 대화 상자의 메시지 처리기입니다.
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

// IP 입력 다이얼 로그
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

