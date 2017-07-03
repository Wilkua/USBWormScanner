// USBWormScanner.cpp
// William Drescher
// Created 2013
// Updated 3 July 2017
//
// The entry point for the application. Creates the main window
// structure and regsiters it with Windows and initializes the
// scanner and VB script killer.
//

#include "stdafx.h"
#include "USBWormScanner.h"
#include "LogController.h"
#include "WormScanner.h"

using namespace std;

#define MAX_LOADSTRING 100
#define WM_NOTIFYCLICK (WM_USER + 1)

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void OnDeviceChange(HWND hwnd, WPARAM wParam, LPARAM lParam);
char FirstDriveFromMask(ULONG unitmask);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	LogController logCtrl;
	WormScanner wScanner(logCtrl);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ANTIWORM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ANTIWORM));

	wScanner.StopRunningVBS();		// stop worm from running
	wScanner.ScanAndCleanSystem();	// runs an initial system cleaning

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANTIWORM));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_ANTIWORM);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL CreateNotifyIcon(HINSTANCE hInstance, HWND hWnd)
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 100;
	nid.uVersion = NOTIFYICON_VERSION;
	nid.uCallbackMessage = WM_NOTIFYCLICK;
	nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANTIWORM));
	wcscpy_s(nid.szTip, L"AntiWorm");
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	return Shell_NotifyIcon(NIM_ADD, &nid);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 640, 480, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	CreateNotifyIcon(hInstance, hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
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
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DEVICECHANGE:
		OnDeviceChange(hWnd, wParam, lParam);
		break;
	case WM_NOTIFYCLICK:	// Notification icon clicked
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			DestroyWindow(hWnd);
			break;
		}
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

void OnDeviceChange(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
	TCHAR szMsg[80];

	switch (wParam)
	{
	case DBT_DEVICEARRIVAL:
		// Check whether a CD or DVD was inserted into a drive.
		if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
		{
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			if (lpdbv->dbcv_flags & DBTF_MEDIA)
			{
				StringCchPrintf(szMsg, sizeof(szMsg) / sizeof(szMsg[0]),
					TEXT("Drive %c: Media has arrived.\n"),
					FirstDriveFromMask(lpdbv->dbcv_unitmask));

				MessageBox(hwnd, szMsg, TEXT("WM_DEVICECHANGE"), MB_OK);
			}
		}
		break;
	case DBT_DEVICEREMOVECOMPLETE:
		// Check whether a CD or DVD was removed from a drive.
		if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
		{
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

			if (lpdbv->dbcv_flags & DBTF_MEDIA)
			{
				StringCchPrintf(szMsg, sizeof(szMsg) / sizeof(szMsg[0]),
					TEXT("Drive %c: Media was removed.\n"),
					FirstDriveFromMask(lpdbv->dbcv_unitmask));

				MessageBox(hwnd, szMsg, TEXT("WM_DEVICECHANGE"), MB_OK);
			}
		}
		break;
	default:
		if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
		{
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

			if (lpdbv->dbcv_flags & DBTF_MEDIA)
			{
				wcout << szMsg << endl;
				wcout << L"Drive ";
				cout << FirstDriveFromMask(lpdbv->dbcv_unitmask);
				wcout << L" has changed." << endl;
			}
		}
		/*Process other WM_DEVICECHANGE notifications for other
		devices or reasons.*/
		break;
	}
}

char FirstDriveFromMask(ULONG unitmask)
{
	char i;

	for (i = 0; i < 26; ++i)
	{
		if (unitmask & 0x1)
			break;
		unitmask = unitmask >> 1;
	}

	return(i + 'A');
}
