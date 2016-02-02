// iNfoFR.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "iNfoFR.h"
#include <commctrl.h>
#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
HWND				CreateListView(HWND);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	PanelLol(HWND, UINT, WPARAM, LPARAM);
void				HandleWM_NOTIFY(LPARAM);
BOOL InsertListViewItems(HWND, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_INFOFR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INFOFR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INFOFR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_INFOFR);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define IDC_MAIN_LISTVIEW11 1472
#define IDC_MAIN_LISTVIEW12 1473
#define IDC_MAIN_BUTTON1 1474

HWND CreateListView(HWND hwndParent, HMENU menu)
{
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcClient;                       // The parent window's client area.

	GetClientRect(hwndParent, &rcClient);

	// Create the list-view window in report view with label editing enabled.
	HWND hWndListView = CreateWindow(WC_LISTVIEW,
		L"",
		WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_VISIBLE | WS_BORDER | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
		0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		hwndParent,
		menu,
		hInst,
		NULL);

	return (hWndListView);
}


#define C_COLUMNS 2
// InitListViewColumns: Adds columns to a list-view control.
// hWndListView:        Handle to the list-view control. 
// Returns TRUE if successful, and FALSE otherwise. 
BOOL InitListViewColumns(HWND hWndListView, std::vector<LPWSTR> stringList)
{
	WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;
	int iCol;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Add the columns.
	for (iCol = 0; iCol < stringList.size(); iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = stringList[iCol];
		lvc.cx = 100;               // Width of column in pixels.

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

									// Load the names of the column headings from the string resources.
		//LoadString(hInst,
		//	IDS_FIRST_POOPOO + iCol,
		//	szText,
		//	sizeof(szText) / sizeof(szText[0]));

		// Insert the columns into the list view.
		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;
	}

	return TRUE;
}

// InsertListViewItems: Inserts items into a list view. 
// hWndListView:        Handle to the list-view control.
// cItems:              Number of items to insert.
// Returns TRUE if successful, and FALSE otherwise.
BOOL InsertListViewItems(HWND hWndListView, int cItems)
{
	LVITEM lvI;

	// Initialize LVITEM members that are common to all items.
	lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.state = 0;

	// Initialize LVITEM members that are different for each item.
	for (int index = 0; index < cItems; index++)
	{
		lvI.iItem = index;
		lvI.iImage = index;

		// Insert items into the list.
		if (ListView_InsertItem(hWndListView, &lvI) == -1)
			return FALSE;
	}

	return TRUE;
}

struct PETINFO
{
	TCHAR szKind[10];
	TCHAR szBreed[50];
	TCHAR szPrice[20];
};

PETINFO rgPetInfo[5] =
{
	{ TEXT("Dog"),  TEXT("Poodle"),     TEXT("$300.00") },
	{ TEXT("Cat"),  TEXT("Siamese"),    TEXT("$100.00") },
	{ TEXT("Fish"), TEXT("Angel Fish"), TEXT("$10.00") },
	{ TEXT("Bird"), TEXT("Parakeet"),   TEXT("$5.00") },
	{ TEXT("Toad"), TEXT("Woodhouse"),  TEXT("$0.25") },
};

void HandleWM_NOTIFY(LPARAM lParam)
{
	NMLVDISPINFO* plvdi;

	switch (((LPNMHDR)lParam)->code)
	{
	case LVN_GETDISPINFO:

		plvdi = (NMLVDISPINFO*)lParam;

		switch (plvdi->item.iSubItem)
		{
		case 0:
			plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szKind;
			break;

		case 1:
			plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szBreed;
			break;

		case 2:
			plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szPrice;
			break;

		default:
			break;
		}

		break;

	}
	// NOTE: In addition to setting pszText to point to the item text, you could 
	// copy the item text into pszText using StringCchCopy. For example:
	//
	// StringCchCopy(plvdi->item.pszText, 
	//                         plvdi->item.cchTextMax, 
	//                         rgPetInfo[plvdi->item.iItem].szKind);

	return;
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
	case WM_CREATE:
		{
			std::vector<LPWSTR> Bloxxer1Columns;
			Bloxxer1Columns.push_back(L"Item #");
			std::vector<LPWSTR> Bloxxer2Columns;
			Bloxxer2Columns.push_back(L"Property Name");
			Bloxxer2Columns.push_back(L"Property Value");
			Bloxxer2Columns.push_back(L"Poo Poos");

			RECT rcClient;
			GetClientRect(hWnd, &rcClient);

			HWND Bloxxer = CreateListView(hWnd, (HMENU)IDC_MAIN_LISTVIEW11);
			HWND Bloxxer2 = CreateListView(hWnd, (HMENU)IDC_MAIN_LISTVIEW12);
			SetWindowPos(Bloxxer, NULL, 0, 0, rcClient.right / 3, (rcClient.bottom / 3) * 2, SWP_NOZORDER);
			SetWindowPos(Bloxxer2, NULL, rcClient.right / 3, 0, rcClient.right / 3 , (rcClient.bottom / 3) * 2, SWP_NOZORDER);
			InsertListViewItems(Bloxxer2, 5);
			
			HWND hwndButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"OK",      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
				10,         // x position 
				(rcClient.bottom / 3) * 2 + 10,         // y position 
				100,        // Button width
				50,        // Button height
				hWnd,     // Parent window
				(HMENU)IDC_MAIN_BUTTON1,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);      // Pointer not needed.

			//HWND poopBox = CreateDialog(hInst, MAKEINTRESOURCE(IDD_FORMVIEW), hWnd, PanelLol);
			InitListViewColumns(Bloxxer, Bloxxer1Columns);
			InitListViewColumns(Bloxxer2, Bloxxer2Columns);

			
			//if (!poopBox)
			{
				//MessageBox(hWnd, L"Failed to go", L"Poo Poo", MB_OK);
			}
			
		}
		break;
	case WM_SIZE:
	{
		HWND hEdit, Bloxxer2, Button1Thing;
		RECT rcClient;

		GetClientRect(hWnd, &rcClient);

		hEdit = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW11);
		Bloxxer2 = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW12);
		Button1Thing = GetDlgItem(hWnd, IDC_MAIN_BUTTON1);

		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right / 3, (rcClient.bottom / 3) * 2, SWP_NOZORDER);
		SetWindowPos(Bloxxer2, NULL, rcClient.right / 3, 0, rcClient.right / 3 , (rcClient.bottom / 3) * 2, SWP_NOZORDER);
		SetWindowPos(Button1Thing, NULL, 10, (rcClient.bottom / 3) * 2 + 10, 100, 50, SWP_NOZORDER);

		
	}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_FILE_OPEN: {


				OPENFILENAME ofn;       // common dialog box structure
				char szFile[260];       // buffer for file name
				HANDLE hf;              // file handle

										// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = (LPWSTR)szFile;
				// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
				// use the contents of szFile to initialize itself.
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"Info Files\0*info;*table;telesa\0All\0*.*\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				// Display the Open dialog box. 
				if (GetOpenFileName(&ofn) == TRUE)
				{
					hf = CreateFile(ofn.lpstrFile,
						GENERIC_READ,
						0,
						(LPSECURITY_ATTRIBUTES)NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);
				}
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_GETDISPINFO:
		{
			NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;
			switch (plvdi->item.iSubItem)
			{
			case 0:
				// rgPetInfo is an array of PETINFO structures.
				plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szKind;
				break;

			case 1:
				plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szBreed;
				break;
			case 2:
				plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szPrice;
				break;

			default:
				break;
			}
			return TRUE;
		}
		// More notifications...
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

INT_PTR CALLBACK PanelLol(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_SIZE:
	{
		HWND hEdit;
		RECT rcClient;

		GetClientRect(hDlg, &rcClient);

		hEdit = GetDlgItem(hDlg, IDD_FORMVIEW);
		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
	}
		break;
	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			case IDOK || IDCANCEL:
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
				break;
			}
			case IDC_BUTTON_APPLY:
			{
				break;
			}
			case IDC_BUTTON_ADD_ITEM:
			{
				break;
			}
			case IDC_BUTTON_RESET:
			{
				break;
			}
			case IDC_BUTTON_COPY_ITEM:
			{
				break;
			}
			case IDC_BUTTON_REMOVE_ITEM:
			{
				break;
			}
			case IDM_ABOUT:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
				break;
			}
			case IDM_EXIT:
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			break;


		}
	
		
	}
	return (INT_PTR)FALSE;
}




