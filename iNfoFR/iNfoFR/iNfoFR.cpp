// iNfoFR.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "iNfoFR.h"
#include <commctrl.h>
#include <vector>
#include "readFile.h"
#include "DocumentReader.h"

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
INT_PTR CALLBACK	SearchWindow(HWND, UINT, WPARAM, LPARAM);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_INFOFR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

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
#define IDC_MAIN_EDITVALUE1 1475
#define IDC_MAIN_BUTTON2 1476
#define IDC_MAIN_TEXTDISP11 1477
#define IDC_MAIN_TEXTDISP12 1478
#define IDC_MAIN_TEXTDISP13 1479
#define IDC_MAIN_TEXTDISP21 1480
#define IDC_MAIN_TEXTDISP22 1481
#define IDC_MAIN_TEXTDISP23 1482
#define IDC_MAIN_BUTTON3DEL 1483
#define IDSC_MAIN_EDITFLOAT 2472
#define IDSC_MAIN_EDITINT 2473
#define IDSC_MAIN_EDITSTRING32 2474
#define IDSC_MAIN_EDITSTRING16 2475
#define BUTTON_APPLY 2601

int peoplesPoo = 0;

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
		WS_CHILD | LVS_REPORT | WS_VISIBLE | WS_BORDER | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
		0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		hwndParent,
		menu,
		hInst,
		NULL);
	SetWindowTheme(hWndListView, L"Explorer", NULL);
	

	return (hWndListView);
}


#define C_COLUMNS 2
// InitListViewColumns: Adds columns to a list-view control.
// hWndListView:        Handle to the list-view control. 
// Returns TRUE if successful, and FALSE otherwise. 
BOOL InitListViewColumns(HWND hWndListView, std::vector<LPWSTR> stringList)
{
	//WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;
	int iCol;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Add the columns.
	for (iCol = 0; iCol < (int)stringList.size(); iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = stringList[iCol];
		lvc.cx = 100;               // Width of column in pixels.

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

									// Load the names of the column headings from the string resources.

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
		{
			//peoplesPoo = -1;
			return FALSE;
		}
	}
	//peoplesPoo = -1;
	return TRUE;
}

IFWhole OpenedFile;
DocumentReader firstRead = DocumentReader::DocumentReader(); //Setting up the reading material.
std::vector<DocumentReader::DocumentItem> DocumentationList; //Documentation Listing.

struct PETINFO
{
	TCHAR szKind[30];
	TCHAR szBreed[50];
	TCHAR szPrice[20];
};

struct ENTRYLISTINFO {
	TCHAR szNumber[20];
	TCHAR szFirstValue[32];
	TCHAR szSecondValue[32];
	TCHAR szThirdValue[32];
};

struct PROPERTYLISTINFO {
	TCHAR szName[66];
	TCHAR szID[20];
	TCHAR szValue[32];
	TCHAR szType[10];
	TCHAR szTypeDisp[40];
};

std::vector<ENTRYLISTINFO> EntrysList;
std::vector<PROPERTYLISTINFO> PropertiesList;
int SelectedProperty;
int SelectedItem;

PETINFO rgPetInfo[5] =
{
	{ TEXT("Position X"),  TEXT("40.00000"),     TEXT("76843") },
	{ TEXT("Position Y"),  TEXT("50.00000"),    TEXT("7331") },
	{ TEXT("Position Z"), TEXT("-10.00000"), TEXT("13336") },
	{ TEXT("Name"), TEXT("mkiniko"),   TEXT("13337") },
	{ TEXT("Room #"), TEXT("37"),  TEXT("50000") },
};


struct PoopInfo {
	TCHAR szKids[30];
	TCHAR szString[20];
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

void changePropList(int item) {
	PropertiesList.clear();
	for (int i = 0; i < (int)OpenedFile.header.NumberOfProperties; ++i) {
		PROPERTYLISTINFO theInfo;
		IFPropertyValue currentProp = OpenedFile.ItemsLolz[item].TheProperties[i];
		_stprintf_s(theInfo.szID, L"%u", currentProp.ValueID);
		_stprintf_s(theInfo.szType, _T("%hs"), currentProp.valueType.c_str());
		_stprintf_s(theInfo.szName, _T("%hs"), currentProp.ValueName.c_str());
		if (currentProp.valueType == "s32") {
			_stprintf_s(theInfo.szValue, _T("%hs"), currentProp.InfoString);
			_stprintf_s(theInfo.szTypeDisp, L"String (size 32)");
		}
		else if(currentProp.valueType == "s16") {
			_stprintf_s(theInfo.szValue, _T("%hs"), currentProp.TableString);
			_stprintf_s(theInfo.szTypeDisp, L"String (size 16)");
		}
		else if (currentProp.valueType == "f4") {
			_stprintf_s(theInfo.szValue, _T("%f"), currentProp.FloatLol);
			_stprintf_s(theInfo.szTypeDisp, L"Float (size 4)");
		}
		else if (currentProp.valueType == "i4") {
			_stprintf_s(theInfo.szValue, _T("%u"), currentProp.IntegerLol);
			_stprintf_s(theInfo.szTypeDisp, L"Integer/Other (size 4)");
		}

		PropertiesList.push_back(theInfo);
		
	}
	peoplesPoo = 2;
}

void changeEntryList() {
	EntrysList.clear();
	for (int i = 0; i < (int)OpenedFile.header.NumberOfItems; ++i) {
		ENTRYLISTINFO theInfo;
		IFItem currentEnt = OpenedFile.ItemsLolz[i];
		_stprintf_s(theInfo.szNumber, L"%d", i);
		for (int j = 0; j < 3 && j < (int)OpenedFile.header.NumberOfProperties; ++j) {
			TCHAR canada[32];
			IFPropertyValue checkSam = currentEnt.TheProperties[j];
			if (checkSam.valueType == "s32") {
				_stprintf_s(canada, _T("%hs"), checkSam.InfoString);
			}
			else if (checkSam.valueType == "s16") {
				_stprintf_s(canada, _T("%hs"), checkSam.TableString);
			}
			else if (checkSam.valueType == "f4") {
				_stprintf_s(canada, _T("%f"), checkSam.FloatLol);
			}
			else if (checkSam.valueType == "i4") {
				_stprintf_s(canada, _T("%u"), checkSam.IntegerLol);
			}
			
			if (j == 0) {
				_stprintf_s(theInfo.szFirstValue, L"%s", canada);
			}
			else if (j == 1) {
				_stprintf_s(theInfo.szSecondValue, L"%s", canada);
			}
			else if (j == 2) {
				_stprintf_s(theInfo.szThirdValue, L"%s", canada);
			}
		}
		EntrysList.push_back(theInfo);
		
	}
	peoplesPoo = 1;
}

std::string MBFromW(LPCWSTR pwsz, UINT cp) {
	int cch = WideCharToMultiByte(cp, 0, pwsz, -1, 0, 0, NULL, NULL);

	char* psz = new char[cch];

	WideCharToMultiByte(cp, 0, pwsz, -1, psz, cch, NULL, NULL);

	std::string st(psz);
	delete[] psz;

	return st;
}

static WNDPROC OriginalEditCtrlProc1 = NULL;

//For use with Floats in Edit boxes.
LRESULT CALLBACK EditFloatWindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch(uIdSubclass)
	{
	case IDSC_MAIN_EDITFLOAT:
		if (uMsg == WM_CHAR)
		{
			// Make sure we only allow specific characters
			TCHAR lolText[100];
			Edit_GetText(hwnd, lolText, 100);
			int numDots, numNegative;
			numNegative = 0;
			numDots = 0;
			for (int i = 0; i < 99; ++i) {
				if (lolText[i] == '-') {
					numNegative++;
				}
				else if (lolText[i] == '.') {
					numDots++;
				}
				if ((numDots > 0 && wParam == '.') || (numNegative > 0 && wParam == '-')) {
					return 0;
				}
			}

			if (!((wParam >= '0' && wParam <= '9')
				|| wParam == '.'
				|| wParam == '-'
				|| wParam == VK_RETURN
				|| wParam == VK_DELETE
				|| wParam == VK_BACK))
			{
				return 0;
			}
		}
		break;
	case IDSC_MAIN_EDITINT:
	{
		if (uMsg == WM_CHAR)
		{
			// Make sure we only allow specific characters
			if (!((wParam >= '0' && wParam <= '9')
				|| wParam == VK_RETURN
				|| wParam == VK_DELETE
				|| wParam == VK_BACK))
			{
				return 0;
			}
		}
		break;
	}
		
	case IDSC_MAIN_EDITSTRING32:
	{
		break; 
	}
	case IDSC_MAIN_EDITSTRING16:
	{
		break; 
	}
	default:
		break;
	}
	

	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

//For use with Ints in Edit boxes.
LRESULT CALLBACK EditIntWindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if (uMsg == WM_CHAR)
	{
		// Make sure we only allow specific characters
		if (!((wParam >= '0' && wParam <= '9') || wParam == VK_RETURN|| wParam == VK_DELETE || wParam == VK_BACK) || (wParam == '.'))
		{
			return 0;
		}
	}


	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void setSubclassEdit(HWND editLolz, PROPERTYLISTINFO lolProp) {
	if (!_tcscmp(lolProp.szType, _T("f4"))) {
		RemoveWindowSubclass(editLolz, EditIntWindowProc, IDSC_MAIN_EDITINT);
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITSTRING16);
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITSTRING32);
		SetWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITFLOAT, 0);
		Edit_LimitText(editLolz, 15);
	}
	else if (!_tcscmp(lolProp.szType, _T("i4"))) {
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITFLOAT);
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITSTRING16);
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITSTRING32);
		SetWindowSubclass(editLolz, EditIntWindowProc, IDSC_MAIN_EDITINT, 1);
		Edit_LimitText(editLolz, 10);
	}
	else if (!_tcscmp(lolProp.szType, _T("s16"))) {
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITFLOAT);
		RemoveWindowSubclass(editLolz, EditIntWindowProc, IDSC_MAIN_EDITINT);
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITSTRING32);
		SetWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITSTRING16, 2);
		Edit_LimitText(editLolz, 15);
	}
	else if (!_tcscmp(lolProp.szType, _T("s32"))) {
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITSTRING16);
		RemoveWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITFLOAT);
		RemoveWindowSubclass(editLolz, EditIntWindowProc, IDSC_MAIN_EDITINT);
		SetWindowSubclass(editLolz, EditFloatWindowProc, IDSC_MAIN_EDITSTRING32, 3);
		Edit_LimitText(editLolz, 31);
	}
}

void ClearEditArea(HWND hWnd, HWND editLolz) {
	HWND applyButton = GetDlgItem(hWnd, IDC_MAIN_BUTTON2);
	Edit_Enable(editLolz, FALSE);
	Edit_SetText(editLolz, (LPCTSTR)L"");
	SelectedProperty = -1;
	Button_Enable(applyButton, FALSE);
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
			Bloxxer1Columns.push_back(L"First Value");
			Bloxxer1Columns.push_back(L"Second Value");
			Bloxxer1Columns.push_back(L"Third Value");
			std::vector<LPWSTR> Bloxxer2Columns;
			Bloxxer2Columns.push_back(L"Property Name");
			Bloxxer2Columns.push_back(L"Property Value");
			Bloxxer2Columns.push_back(L"Property ID#");
			Bloxxer2Columns.push_back(L"Property Type");

			RECT rcClient;
			GetClientRect(hWnd, &rcClient);

			HWND Bloxxer = CreateListView(hWnd, (HMENU)IDC_MAIN_LISTVIEW11);
			HWND Bloxxer2 = CreateListView(hWnd, (HMENU)IDC_MAIN_LISTVIEW12);
			
			SetWindowPos(Bloxxer, NULL, 0, 0, rcClient.right / 3, rcClient.bottom - 70, SWP_NOZORDER);
			SetWindowPos(Bloxxer2, NULL, rcClient.right / 3, 0, rcClient.right / 3 , rcClient.bottom - 70, SWP_NOZORDER);
			
			
			HWND hwndButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"Copy Item",      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,   // Styles 
				10,         // x position 
				(rcClient.bottom) - 60,         // y position 
				100,        // Button width
				50,        // Button height
				hWnd,     // Parent window
				(HMENU)IDC_MAIN_BUTTON1,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);      // Pointer not needed.
			
			HWND hwndApplyButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"Apply",      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_DISABLED,   // Styles 
				(rcClient.right / 3) * 2 + 50,         // x position 
				(rcClient.bottom / 2) + 30 ,         // y position 
				((rcClient.right / 3) - 100) / 2 - 5,        // Button width
				25,        // Button height
				hWnd,     // Parent window
				(HMENU)IDC_MAIN_BUTTON2,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);      // Pointer not needed.

			HWND hwndDeleteButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"Delete Item",      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,   // Styles 
				120,         // x position 
				(rcClient.bottom) - 60,         // y position 
				100,        // Button width
				50,        // Button height
				hWnd,     // Parent window
				(HMENU)IDC_MAIN_BUTTON3DEL,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);      // Pointer not needed.
			
			HWND hwndEdit = CreateWindowEx(
				0, L"EDIT",   // predefined class 
				NULL,         // no window title 
				WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL | WS_DISABLED,
				(rcClient.right / 3) * 2 + 50, rcClient.bottom / 2, (rcClient.right / 3) - 100, 25,   // set size in WM_SIZE message 
				hWnd,         // parent window 
				(HMENU)IDC_MAIN_EDITVALUE1,   // edit control ID 
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);        // pointer not needed 
			
			HWND EditDisplayType = CreateWindowEx(
				0, L"EDIT", 
				NULL, 
				WS_CHILD| WS_VISIBLE| ES_RIGHT| WS_DISABLED, // | WS_THICKFRAME
				(rcClient.right / 3) * 2 + 20, rcClient.top + 50, (rcClient.right / 3) / 2 - 25, 25,   
				hWnd,         
				(HMENU)IDC_MAIN_TEXTDISP11,    
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);
			Edit_SetText(EditDisplayType, L"Property Type: ");
			HWND EditTextType = CreateWindowEx(
				0, L"EDIT",
				NULL,
				WS_CHILD | WS_VISIBLE | ES_LEFT | WS_DISABLED, 
				(rcClient.right / 3) * 2 + (rcClient.right / 3) / 2 + 5, rcClient.top + 50, rcClient.right - 25, 25,
				hWnd,
				(HMENU)IDC_MAIN_TEXTDISP21,
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);
			
			
			if (!firstRead.GetDocumentation(DocumentationList)) {
				MessageBox(hWnd, L"Could not open set documentation", L"Error", MB_OK | MB_ICONERROR);
			}

			InitListViewColumns(Bloxxer, Bloxxer1Columns);
			InitListViewColumns(Bloxxer2, Bloxxer2Columns);
			
		}
		break;
	case WM_SIZE:
	{
		HWND hEdit, Bloxxer2, Button1Thing, EditBox1, ApplyButton, TextBox1, TextBox12, DelButton;
		RECT rcClient;

		GetClientRect(hWnd, &rcClient);

		hEdit = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW11);
		Bloxxer2 = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW12);
		Button1Thing = GetDlgItem(hWnd, IDC_MAIN_BUTTON1);
		EditBox1 = GetDlgItem(hWnd, IDC_MAIN_EDITVALUE1);
		ApplyButton = GetDlgItem(hWnd, IDC_MAIN_BUTTON2);
		TextBox1 = GetDlgItem(hWnd, IDC_MAIN_TEXTDISP11);
		TextBox12 = GetDlgItem(hWnd, IDC_MAIN_TEXTDISP21);
		DelButton = GetDlgItem(hWnd, IDC_MAIN_BUTTON3DEL);

		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right / 3, rcClient.bottom - 70, SWP_NOZORDER);
		SetWindowPos(Bloxxer2, NULL, rcClient.right / 3, 0, rcClient.right / 3, rcClient.bottom - 70, SWP_NOZORDER);
		SetWindowPos(Button1Thing, NULL, 10, (rcClient.bottom) - 60, 100, 50, SWP_NOZORDER);
		SetWindowPos(EditBox1, NULL, (rcClient.right / 3) * 2 + 30, rcClient.bottom / 2, (rcClient.right / 3) - 60, 25, SWP_NOZORDER);
		SetWindowPos(ApplyButton, NULL, (rcClient.right / 3) * 2 + 30,(rcClient.bottom / 2) + 30,((rcClient.right / 3) - 100) / 2 - 5, 25, SWP_NOZORDER);
		SetWindowPos(TextBox1, NULL,(rcClient.right / 3) * 2 + 20, rcClient.top + 50, (rcClient.right / 3) / 2 - 25, 25, SWP_NOZORDER);
		SetWindowPos(TextBox12, NULL, (rcClient.right / 3) * 2 + (rcClient.right / 3) / 2 + 5, rcClient.top + 50, rcClient.right - 25, 25, SWP_NOZORDER);
		SetWindowPos(DelButton, NULL, 120, (rcClient.bottom) - 60, 100, 50, SWP_NOZORDER);

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

				ZeroMemory(&ofn, sizeof(ofn));	// Initialize OPENFILENAME
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
					if (ReadInfoFile(OpenedFile, (LPWSTR)szFile, DocumentationList))
					{
						SelectedItem = -1;
						HWND editLolz = GetDlgItem(hWnd, IDC_MAIN_EDITVALUE1);
						ClearEditArea(hWnd, editLolz);
						TCHAR candySam[300];
						_stprintf(candySam, L"%s (iNfoFR)", (LPWSTR)szFile);
						SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)(candySam));
						HWND poopHead = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW11);
						ListView_DeleteAllItems(poopHead);
						HWND mainLolz = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW12);
						ListView_DeleteAllItems(mainLolz);
						PropertiesList.clear();
						changeEntryList();
						InsertListViewItems(poopHead, (int)OpenedFile.header.NumberOfItems);
					}
					else {
						MessageBox(hWnd, L"File Did not Open Fully", L"ERROR", MB_OK | MB_ICONERROR);
					}

				}
			}
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDC_MAIN_BUTTON2:
			{
				LPNMHDR lpnmh = (LPNMHDR)lParam;
				
					if (SelectedProperty > -1) {
						HWND editLolz = GetDlgItem(hWnd, IDC_MAIN_EDITVALUE1);
						PROPERTYLISTINFO lolProp = PropertiesList[SelectedProperty];
						HWND lolList2 = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW12);
						HWND lolList1 = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW11);

						Edit_GetText(editLolz, PropertiesList[SelectedProperty].szValue, 31);
						
						std::string newMe = MBFromW(PropertiesList[SelectedProperty].szValue, CP_ACP);
						
						if (!_tcscmp(lolProp.szType, _T("f4"))) {
							OpenedFile.ItemsLolz[SelectedItem].TheProperties[SelectedProperty].FloatLol = std::stof(newMe);
						}
						else if (!_tcscmp(lolProp.szType, _T("i4"))) {
							OpenedFile.ItemsLolz[SelectedItem].TheProperties[SelectedProperty].IntegerLol = (u32)std::stoul(newMe);
						}
						else if (!_tcscmp(lolProp.szType, _T("s16"))) {
							std::strcpy(OpenedFile.ItemsLolz[SelectedItem].TheProperties[SelectedProperty].TableString, newMe.c_str());
						}
						else if (!_tcscmp(lolProp.szType, _T("s32"))) {
								std::strcpy(OpenedFile.ItemsLolz[SelectedItem].TheProperties[SelectedProperty].InfoString, newMe.c_str());
						}

						ListView_Update(lolList2, SelectedProperty);
						
						if (SelectedProperty == 0) {
							_stprintf(EntrysList[SelectedItem].szFirstValue, L"%s", PropertiesList[SelectedProperty].szValue);
						}
						else if (SelectedProperty == 1) {
							_stprintf(EntrysList[SelectedItem].szSecondValue, L"%s", PropertiesList[SelectedProperty].szValue);
						}
						else if (SelectedProperty == 2) {
							_stprintf(EntrysList[SelectedItem].szThirdValue, L"%s", PropertiesList[SelectedProperty].szValue);
						}
						ListView_Update(lolList1, SelectedItem);
					}
					break;
				
			}
			case ID_FILE_SAVE:
			{
				OPENFILENAME ofn;
				char szFileName[MAX_PATH] = "";

				ZeroMemory(&ofn, sizeof(ofn));

				ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = L"Info Files\0*info;*table;telesa\0All\0*.*\0";
				ofn.lpstrFile = (LPWSTR)szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
				ofn.lpstrDefExt = L"";

				if (GetSaveFileName(&ofn))
				{
					WriteInfoFile(OpenedFile, (LPWSTR)szFileName);
					// Do something usefull with the filename stored in szFileName 
				}
				break;
			}
			case IDC_MAIN_BUTTON1:
			{
				HWND listLol1 = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW11);
				int lolPoo = ListView_GetSelectionMark(listLol1);
				if (lolPoo > -1) {
					IFItem copyAp = OpenedFile.ItemsLolz[lolPoo];
					OpenedFile.ItemsLolz.push_back(copyAp);
					OpenedFile.header.NumberOfItems += 1;
					ENTRYLISTINFO theInfo;
					IFItem currentEnt = copyAp;
					_stprintf_s(theInfo.szNumber, L"%d", OpenedFile.header.NumberOfItems - 1);
					for (int j = 0; j < 3 && j < (int)OpenedFile.header.NumberOfProperties; ++j) {
						TCHAR canada[32];
						IFPropertyValue checkSam = currentEnt.TheProperties[j];
						if (checkSam.valueType == "s32") {
							_stprintf_s(canada, _T("%hs"), checkSam.InfoString);
						}
						else if (checkSam.valueType == "s16") {
							_stprintf_s(canada, _T("%hs"), checkSam.TableString);
						}
						else if (checkSam.valueType == "f4") {
							_stprintf_s(canada, _T("%f"), checkSam.FloatLol);
						}
						else if (checkSam.valueType == "i4") {
							_stprintf_s(canada, _T("%u"), checkSam.IntegerLol);
						}

						if (j == 0) {
							_stprintf_s(theInfo.szFirstValue, L"%s", canada);
						}
						else if (j == 1) {
							_stprintf_s(theInfo.szSecondValue, L"%s", canada);
						}
						else if (j == 2) {
							_stprintf_s(theInfo.szThirdValue, L"%s", canada);
						}
					}
					EntrysList.push_back(theInfo);
					InsertListViewItems(listLol1, 1);
				}
				break;
			}
			case IDC_MAIN_BUTTON3DEL:
			{
				HWND listLol1 = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW11);
				int lolPoo = ListView_GetSelectionMark(listLol1);
				if (lolPoo > -1) {
					OpenedFile.ItemsLolz.erase(OpenedFile.ItemsLolz.begin() + lolPoo);
					EntrysList.erase(EntrysList.begin() + lolPoo);
					OpenedFile.header.NumberOfItems--;
					ListView_DeleteItem(listLol1, lolPoo);
					for (int i = 0; i < (int)OpenedFile.header.NumberOfItems; ++i) {
						_stprintf_s(EntrysList[i].szNumber, L"%d", i);
						ListView_Update(listLol1, i);
					}
				}
				break;
			}
			case ID_EDIT_SEARCH:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SEARCHDIALOG), hWnd, SearchWindow);
				break;
			}
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
			UINT_PTR compareRare = ((LPNMHDR)lParam)->idFrom;
			switch (plvdi->item.iSubItem)
			{
			case 0:
			{
				
				if (compareRare == 0) {
					plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szKind;
				}
				else if (compareRare == (UINT_PTR)IDC_MAIN_LISTVIEW11) {
					plvdi->item.pszText = EntrysList[plvdi->item.iItem].szNumber;
				}
				else if (compareRare == (UINT_PTR)IDC_MAIN_LISTVIEW12) {
					plvdi->item.pszText = PropertiesList[plvdi->item.iItem].szName;
				}
			}
			break;
			case 1:
			{
				if (peoplesPoo == 0) {
					plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szBreed;
				}
				else if (compareRare == (UINT_PTR)IDC_MAIN_LISTVIEW11) {
					plvdi->item.pszText = EntrysList[plvdi->item.iItem].szFirstValue;
				}
				else if (compareRare == (UINT_PTR)IDC_MAIN_LISTVIEW12) {
					plvdi->item.pszText = PropertiesList[plvdi->item.iItem].szValue;
				}
			}
			break;
			case 2:
			{
				if (peoplesPoo == 0) {
					plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szPrice;
				}
				else if (compareRare == (UINT_PTR)IDC_MAIN_LISTVIEW11) {
					plvdi->item.pszText = EntrysList[plvdi->item.iItem].szSecondValue;
				}
				else if (compareRare == (UINT_PTR)IDC_MAIN_LISTVIEW12) {
					plvdi->item.pszText = PropertiesList[plvdi->item.iItem].szID;
				}
			}
			break;
			case 3:
			{
				if (peoplesPoo == 0) {

				}
				else if (compareRare == (UINT_PTR)IDC_MAIN_LISTVIEW11) {
					plvdi->item.pszText = EntrysList[plvdi->item.iItem].szThirdValue;
				}
				else if (peoplesPoo == 2) {
					plvdi->item.pszText = PropertiesList[plvdi->item.iItem].szTypeDisp;
				}
			}
			break;

			default:
				break;
			}
			return TRUE;
		}

		case NM_RETURN:
		{
			LPNMHDR lpnmh = (LPNMHDR)lParam;
			switch (lpnmh->idFrom)
			{
			case IDC_MAIN_LISTVIEW11:
			{

				HWND lolList = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW11);
				int getTheSelected = ListView_GetSelectionMark(lolList);
				HWND editLolz = GetDlgItem(hWnd, IDC_MAIN_EDITVALUE1);
				ClearEditArea(hWnd, editLolz);
				if (getTheSelected > -1) {
					SelectedItem = getTheSelected;
					HWND mainLolz = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW12);
					ListView_DeleteAllItems(mainLolz);
					changePropList(getTheSelected);
					InsertListViewItems(mainLolz, PropertiesList.size());
				}
				break;
			}
			case IDC_MAIN_LISTVIEW12:
			{
				HWND mainLolz = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW12);
				int getTheSelected = ListView_GetSelectionMark(mainLolz);
				if (getTheSelected > -1) {
					HWND typeBox = GetDlgItem(hWnd, IDC_MAIN_TEXTDISP21);
					HWND editLolz = GetDlgItem(hWnd, IDC_MAIN_EDITVALUE1);

					PROPERTYLISTINFO lolProp = PropertiesList[getTheSelected];
					SendMessage(editLolz, WM_SETTEXT, 0, (LPARAM)lolProp.szValue);
					SelectedProperty = getTheSelected;
					setSubclassEdit(editLolz, lolProp);
					Edit_Enable(editLolz, TRUE);
					Edit_SetText(typeBox, lolProp.szTypeDisp);
					HWND applyButton = GetDlgItem(hWnd, IDC_MAIN_BUTTON2);
					Button_Enable(applyButton, TRUE);
				}
				break; 
			}
			default:
				break;
			}
			
			return TRUE;
		}
		case NM_DBLCLK:
		{
			NMITEMACTIVATE* fartMan = (NMITEMACTIVATE*)lParam;
			UINT_PTR party = ((LPNMHDR)lParam)->idFrom;
			if((UINT_PTR)IDC_MAIN_LISTVIEW11 == party && fartMan->iItem > -1)
				{
					
					HWND mainLolz = GetDlgItem(hWnd, IDC_MAIN_LISTVIEW12);
					SelectedItem = fartMan->iItem;
					ListView_DeleteAllItems(mainLolz);
					changePropList(fartMan->iItem);
					HWND editLolz = GetDlgItem(hWnd, IDC_MAIN_EDITVALUE1);
					ClearEditArea(hWnd, editLolz);
					InsertListViewItems(mainLolz, PropertiesList.size());
					
				}
			else if ((UINT_PTR)IDC_MAIN_LISTVIEW12 == party && fartMan->iItem > -1)
			{
				HWND editLolz = GetDlgItem(hWnd, IDC_MAIN_EDITVALUE1);
				HWND typeBox = GetDlgItem(hWnd, IDC_MAIN_TEXTDISP21);
				PROPERTYLISTINFO lolProp = PropertiesList[fartMan->iItem];
				SendMessage(editLolz, WM_SETTEXT, 0, (LPARAM)lolProp.szValue);
				SelectedProperty = fartMan->iItem;
				setSubclassEdit(editLolz, lolProp);
				Edit_Enable(editLolz, TRUE);
				Edit_SetText(typeBox, lolProp.szTypeDisp);
				HWND applyButton = GetDlgItem(hWnd, IDC_MAIN_BUTTON2);
				Button_Enable(applyButton, TRUE);
			}
			return TRUE;
		
		}
		
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

INT_PTR CALLBACK SearchWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}
	case WM_SIZE:
	{
		HWND hEdit;
		RECT rcClient;

		GetClientRect(hDlg, &rcClient);

		hEdit = GetDlgItem(hDlg, IDD_SEARCHDIALOG);
		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			case IDC_RADIO1:
			{
				// load the combobox with item list.  
				// Send a CB_ADDSTRING message to load each item
				HWND hWndComboBox = GetDlgItem(hDlg, IDC_SEARCHCOMBO1);
				//Clears all of the combo box
				int ComboCount = ComboBox_GetCount(hWndComboBox);
				while (ComboCount > -1)
				{
					ComboCount = ComboBox_DeleteString(hWndComboBox, ComboCount - 1);
				}
				TCHAR Planets[9][10] =
				{
					TEXT("Mercury"), TEXT("Venus"), TEXT("Terra"), TEXT("Mars"),
					TEXT("Jupiter"), TEXT("Saturn"), TEXT("Uranus"), TEXT("Neptune"),
					TEXT("Pluto??")
				};

				TCHAR A[16];
				int  k = 0;

				memset(&A, 0, sizeof(A));
				for (k = 0; k <= 8; k += 1)
				{
					wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Planets[k]);

					// Add string to combobox.
					SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
				}

				// Send the CB_SETCURSEL message to display an initial item 
				//  in the selection field  
				SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);
				ComboBox_Enable(hWndComboBox, TRUE);
				
				break;
			}
			case WM_DESTROY:
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
				break;
			}
			
		}
		}
	break;
	case WM_NOTIFY: {
				switch (((LPNMHDR)lParam)->code)
				{
				
				default:
					return DefWindowProc(hDlg, message, wParam, lParam);
				}
				break;
			}
	}
	

	
	
	return (INT_PTR)FALSE;
}



