// mylamp_core.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "mylamp_lib.h"
#include "mylamp_core.h"

#define MAX_LOADSTRING				100
#define SETTINGS_WINDOW_HEIGHT		450
#define SETTINGS_WINDOW_WIDTH		600
#define SETTINGS_WINDOW_TV_WIDTH	200
#define SETTINGS_WINDOW_OFFSET		4

// Global Variables:
HINSTANCE			hInst;										// current instance
TCHAR				szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR				szWindowClass[MAX_LOADSTRING];				// the main window class name
Components			components;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Preferences(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

bool				AddItemsToSettingsTree(HWND hWnd, StringVector svPath, StringVector svItems);
void				TreeExpandAllNode(HWND hWnd);
bool				ChangeSelectedItem(StringVector svReverseItem);

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	SetComponents(&components);
			
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYLAMP_CORE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYLAMP_CORE));

	// Main message loop:
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

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYLAMP_CORE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BACKGROUND)/*(COLOR_WINDOW+1)*/;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYLAMP_CORE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
	HWND hWnd, hwndTV;
	RECT rcClient;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, SETTINGS_WINDOW_WIDTH, SETTINGS_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	GetClientRect(hWnd, &rcClient); 

	InitCommonControls(); 

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
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_PREFERENCE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_OPTIONS), hWnd, Preferences);
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
		// TODO: Add any drawing code here...
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

typedef mylamp::Component* (*_RegComponent)();

// Message handler for preferences box.
INT_PTR CALLBACK Preferences(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{	
	case WM_INITDIALOG:
		{	
			HWND hWndTV = GetDlgItem(hDlg, IDC_SET_TREE);

			StringVector items, paths, paths2;
			items.push_back(L"New Item");
			items.push_back(L"New Item2");

			paths.push_back(L"New Item");
			AddItemsToSettingsTree(hWndTV, paths, items);

			paths2.push_back(L"root2");

			AddItemsToSettingsTree(hWndTV, paths2, items);

			if (!components.isLoad())
				components.Load();

			DllDetailVector* ddvCurrent = components.getDetailVector();
			DllDetailIterator ddiIterator = ddvCurrent->begin();
			
			do
			{
				settings_items si = ddiIterator->pComponent->GetSettingsItems();
				
				AddItemsToSettingsTree(hWndTV, si.path, si.items);

				ddiIterator++;
			}
			while (ddiIterator != ddvCurrent->end());
			// <TEST CODE>
			
			
			paths.push_back(L"New Item2");
			AddItemsToSettingsTree(hWndTV, paths, items);

			paths.push_back(L"New Item");
			AddItemsToSettingsTree(hWndTV, paths, items);

			TreeExpandAllNode(hWndTV);
			// </TEST CODE>

			return (INT_PTR)TRUE;
		}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	case WM_NOTIFY:
		{
			LPNMHDR pNMHDR = (LPNMHDR)lParam;
			StringVector svCurrentItem;

			if ( pNMHDR->hwndFrom == GetDlgItem(hDlg, IDC_SET_TREE) )
			{
				if ( pNMHDR->code == TVN_SELCHANGED )
				{
					LPNMTREEVIEW pNMTV = (LPNMTREEVIEW) lParam;
					TVITEM tvItem = pNMTV->itemNew;

					HTREEITEM hCurrent = tvItem.hItem; //warning: check mask; 

					do
					{
						TVITEMEX tviCurrent = { 0 };
						
						tviCurrent.hItem = hCurrent;
						tviCurrent.mask = TVIF_HANDLE | TVIF_TEXT;

						tviCurrent.pszText = new TCHAR[MAXCHAR];
						tviCurrent.cchTextMax = MAXCHAR;

						if (TreeView_GetItem(pNMHDR->hwndFrom, &tviCurrent))
							svCurrentItem.push_back(tviCurrent.pszText);
						
						delete tviCurrent.pszText;

						hCurrent = TreeView_GetParent(pNMHDR->hwndFrom, hCurrent);
					}
					while (hCurrent);
					
					ChangeSelectedItem(svCurrentItem);
					break;
				}
			}
			break;
		}
	}
	return (INT_PTR)FALSE;
}

bool ChangeSelectedItem(StringVector svReverseItem)
{
	//TODO: add code here;
	return true;
}

void TreeExpandAllNode(HWND hWnd)
{
	HTREEITEM hIterator = TreeView_GetRoot(hWnd);
	if (!hIterator)
		return;
	do
	{
		if ( TreeView_GetChild(hWnd, hIterator) )
			if ( !TreeView_Expand(hWnd, hIterator, TVE_EXPAND) )
				return;

		hIterator = TreeView_GetNextVisible(hWnd, hIterator);
	}
	while( hIterator );
}

HTREEITEM InsertItemToTree(HWND hWnd, HTREEITEM hParent, HTREEITEM hInsertAfter, tstring item)
{
	TVITEMEX tviInsert = { 0 };
	tviInsert.mask = TVIF_TEXT;	//item.copy( (LPTSTR)tviInsert.pszText, item.length(), 0);
	tviInsert.pszText = (wchar_t * ) item.c_str();

	TVINSERTSTRUCT tvInsStruct = { 0 };
	tvInsStruct.hParent = hParent;
	tvInsStruct.hInsertAfter = hInsertAfter;
	tvInsStruct.itemex = tviInsert;

	return TreeView_InsertItem(hWnd, &tvInsStruct);
}

bool AddItemsToSettingsTree(HWND hWnd, StringVector svPath, StringVector svItems)
{
	HTREEITEM hParent = NULL;
	HTREEITEM hIterator = TreeView_GetRoot(hWnd);

	if ( svItems.empty() ) 
		return false;

	// Check and Create Path
	if ( !svPath.empty() )
	{
		StringVectorIterator svIterator = svPath.begin();

		while (svIterator != svPath.end())
		{
			tstring tzCurrentName;
			do
			{
				TVITEMEX tviCurrent = {0};

				tviCurrent.hItem = hIterator;
				tviCurrent.mask = TVIF_HANDLE | TVIF_TEXT;

				if (hIterator)
				{
					tviCurrent.pszText = new TCHAR[MAXCHAR];
					tviCurrent.cchTextMax = MAXCHAR;

					if (TreeView_GetItem(hWnd, &tviCurrent)) 
						tzCurrentName = tstring(tviCurrent.pszText);
					else
						return false;
					
					delete tviCurrent.pszText;
					
					if (svIterator->compare(tzCurrentName))
						hIterator = TreeView_GetNextSibling(hWnd, hIterator);
				}
				else
				{
					hIterator = InsertItemToTree(hWnd, hParent, TVI_SORT, *svIterator);
					if (!hIterator) 
						return false;

					tzCurrentName = *svIterator;
				}
			}
			while ( svIterator->compare(tzCurrentName) );

			hParent = hIterator;

			hIterator = TreeView_GetChild(hWnd, hParent);

			svIterator++;
		}
	}

	// Insert Items
	StringVectorIterator svIterator = svItems.begin();

	while (svIterator != svItems.end())
	{
		if ( !InsertItemToTree(hWnd, hParent, TVI_SORT, *svIterator) )
			return false;

		svIterator++;
	}	

	return true;
}