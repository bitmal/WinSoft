// WinSoft.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "winsoft.h"

#include <cmath>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWindow;									// window
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

bool isRunning = true;

bool mouseDown = false;
int mouseDownX = -1;
int mouseDownY = -1;
int mouseX = -1;
int mouseY = -1;
float boxNormX = -1;
float boxNormY = -1;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void CreateConsole();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	CreateConsole();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINSOFT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);	
	
    
	// Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINSOFT));
	
	// create the bitmap backbuffer for rendering (hCompatBuffer)
	// "memory/compatible device contexts"
	HDC hMemDC = NULL;
	HDC hClient = NULL;
	HBITMAP map = NULL;
	RECT clientRect;

	hClient = GetDC(hWindow);

	GetClientRect(hWindow, &clientRect);

	const DWORD WIDTH = clientRect.right+1;
	const DWORD HEIGHT = clientRect.bottom+1;

	hMemDC = CreateCompatibleDC(hClient);

	BITMAPINFOHEADER header;
	ZeroMemory(&header, sizeof(BITMAPINFOHEADER));
	header.biBitCount = 32;
	header.biCompression = BI_RGB;
	header.biWidth = WIDTH;
	header.biHeight = HEIGHT;
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biPlanes = 1;
	BITMAPINFO info;
	ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader = header;

	PVOID surfaceData = NULL;

	HBITMAP hDibSurface = CreateDIBSection(hMemDC, &info, DIB_RGB_COLORS, &surfaceData, NULL, 0);
	SelectObject(hMemDC, hDibSurface);

	WinSoft::Surface surface {};
	surface._data = surfaceData;
	surface._rect = WinSoft::Rect{ WinSoft::Point{ 0, 0 }, WinSoft::Point{ (float)WIDTH, (float)HEIGHT } };

    // Main message loop:
	MSG msg;

	float boxWidth = 200;
	float boxHeight = 200;
	WinSoft::Point point = {400, 200};
	WinSoft::Rect r {};
	r._bottomLeft = point;
	r._topRight = WinSoft::Point{point._x + boxWidth, point._y+boxHeight};
	WinSoft::Color32 innerColor{1, 1, 0, 1};
	WinSoft::ColorBorder border{};
	border._color = {1, 0, 0, 1};	
	border._type = WinSoft::ColorBorder::INSET;
	border._width = 50;

    while (isRunning)
    {
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
			TranslateMessage(&msg);
            DispatchMessage(&msg);
        }	

		WinSoft::RefreshSurface(surface, WinSoft::Color32 { 0, 0, 0, 0 });
		//WinSoft::DrawLine(WinSoft::Point{(float)WIDTH/4-1, (float)HEIGHT/2-1}, WinSoft::Point{(float)x, (float)y}, WinSoft::Color32{1, 0, 0, 1}, surface);

		int transMouseY = HEIGHT - mouseY - 1;
		int transMouseX = mouseX;		

		bool inBounds = false;

		int bX = (int)r._bottomLeft._x;
		int tX = (int)r._topRight._x;
		int bY = (int)r._bottomLeft._y;
		int tY = (int)r._topRight._y;

		if ((transMouseX >= bX && transMouseX <= tX) &&
			(transMouseY >= bY && transMouseY <= tY))
		{
			inBounds = true;
		}

		if (inBounds)
		{
			WinSoft::PColor32 pcol = 0;
			WinSoft::PColor(border._color, pcol);

			WinSoft::PColor32 colorMask = 0xAAAAAAAA;
			pcol |= colorMask;

			WinSoft::FColor(pcol, border._color);			

			if (mouseDown)
			{
				if (boxNormX < 0 || boxNormY < 0)
				{
					boxNormX = (transMouseX-r._bottomLeft._x)/boxWidth;
					boxNormY = (transMouseY-r._bottomLeft._y)/boxHeight;

					WinSoft::PColor(innerColor, pcol);
					colorMask = 0xABCDEFFF;
					pcol |= colorMask;
					WinSoft::FColor(pcol, innerColor);
				}

				point._x = transMouseX - (boxNormX*boxWidth);
				point._y = transMouseY - (boxNormY*boxHeight);

				r._bottomLeft = point;
				r._topRight = WinSoft::Point{point._x + boxWidth, point._y + boxHeight};
			}
			else
			{
				innerColor = WinSoft::Color32{1, 1, 0, 1};
			}
		}
		else
		{
			innerColor = WinSoft::Color32{1, 1, 0, 1};
			border._color = WinSoft::Color32{1, 0, 0, 1};			
		}

		WinSoft::FillRect(r, &border, innerColor, surface);

		// render
		SetDIBits(hMemDC, hDibSurface, 0, WIDTH*HEIGHT, NULL, &info, DIB_RGB_COLORS);
		BitBlt(hClient, 0, 0, WIDTH, HEIGHT, hMemDC, 0, 0, SRCCOPY);

		// block the thread for framerate
		Sleep(10);
    }

	DeleteObject(hDibSurface);
	DeleteDC(hMemDC);
	ReleaseDC(hWindow, hClient);

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINSOFT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINSOFT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = hWindow = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
    
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		mouseDown = true;
		mouseDownX = x;
		mouseDownY = y;
	}
		break;
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		mouseX = x;
		mouseY = y;
	}
		break;
	case WM_LBUTTONUP:
	{
		mouseDown = false;
		mouseDownX = -1;
		mouseDownY = -1;
		boxNormX = -1;
		boxNormY = -1;
	}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);	
		isRunning = false;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

void CreateConsole()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
}
