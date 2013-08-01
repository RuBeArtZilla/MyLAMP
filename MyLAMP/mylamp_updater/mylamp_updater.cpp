// mylamp_updater.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "../mylamp_core/mylamp_lib.cpp"
#include "../mylamp_core/ver_info.cpp"

#define UPDATER_ITEM_NAME L"Components"
#define ID_UPDATER_LISTVIEW 1280

mylamp::Component * pComponent = NULL;
//Components *		pComponents = NULL;

HWND hWndLV = NULL;
HWND		CreateListView(HWND hWnd, UINT uId, UINT x, UINT y, UINT width, UINT height);
int			SetListViewColumns(HWND hWndLV, int textMaxLen, StringVector svHeader);
BOOL WINAPI AddListViewItems(HWND hWndLV, int textMaxLen, UINT uRow, UINT uColumn, tstring tsText);

class Updater: public mylamp::Component
{
private:
	bool b_init;
	bool b_load;
	mylamp::Component* p_parent;

public:
	Updater();
	virtual bool IsInit(){return b_init;};
	virtual bool IsLoad(){return b_load;};

	virtual bool Load(Component* pParent = 0){p_parent = pParent; return (b_load = true);};
	virtual ~Updater(){};
	virtual mylamp::COMPONENT_INFO GetInfo();
	virtual UINT64 GetCoreMinVersion(){return 1;};
	
	virtual settings_items GetSettingsItems(); 	
	virtual bool CheckSelectedItem(StringVector svReversedItem);
	
	virtual INT_PTR CALLBACK SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool AddWndProc(_WndProc wndproc){return false;};
	virtual bool DelWndProc(_WndProc wndproc){return false;};
};

Updater::Updater()
{
	b_init = false;
	b_load = false;
}

mylamp::COMPONENT_INFO Updater::GetInfo()
{
	mylamp::COMPONENT_INFO ciResult = {1, 0};
	return ciResult;
}

settings_items Updater::GetSettingsItems() 
{
	settings_items siResult;
	siResult.items.push_back(UPDATER_ITEM_NAME);
	return siResult;
}

bool Updater::CheckSelectedItem(StringVector svReversedItem)
{
	if (svReversedItem.size() != 1)
		return false;

	if (!svReversedItem.begin()->compare(UPDATER_ITEM_NAME))
		return true;

	return false;
}

INT_PTR CALLBACK Updater::SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_COMPONENT_SELECTED:
		{
			//TODO: place init code here
			RECT rcl;
			GetClientRect(hWnd, &rcl);
			INT offset		= (rcl.bottom - lParam) / 2;
			INT x			= offset * 2 + wParam;
			INT y			= offset;
			INT width		= rcl.right - x - offset;
			INT height		= lParam - SETTINGS_WINDOW_BUTTON_HEIGHT;
			hWndLV = CreateListView(hWnd, ID_UPDATER_LISTVIEW, x, y, width, height);
			
			if (!hWndLV)
				break;
			
			StringVector svHeader;

			//TODO: MAKEINRESOURCE ↓ HEADERS
			svHeader.push_back(L"Name");
			svHeader.push_back(L"Version");
			svHeader.push_back(L"Module");
			svHeader.push_back(L"Description");

			SetListViewColumns(hWndLV, /*TODO: remove number*/128, svHeader);
			
			//experimental code ↓
			Components*	pComponents = (Components*)p_parent;

			DllDetailVector* pDDV = pComponents->getDetailVector();
			DllDetailIterator DDI = pDDV->begin();
			int iRowIndex = 0;
			do
			{
				tstring tsFileVersion = GetFileVersionInfo(DDI->svName, FI_FILE_VERSION);
				tstring tsInternalName = GetFileVersionInfo(DDI->svName, FI_INTERNAL_NAME);
				tstring tsProductName = GetFileVersionInfo(DDI->svName, FI_PRODUCT_NAME);
				tstring tsFileDescription = GetFileVersionInfo(DDI->svName, FI_FILE_DESCRIPTION);
				
				AddListViewItems(hWndLV, 128, iRowIndex, 0, tsProductName);
				AddListViewItems(hWndLV, 128, iRowIndex, 1, tsFileVersion);
				AddListViewItems(hWndLV, 128, iRowIndex, 2, tsInternalName);
				AddListViewItems(hWndLV, 128, iRowIndex, 3, tsFileDescription);

				iRowIndex++;
				DDI++;
			}
			while(DDI != pDDV->end());

		}
		break;

	case WM_COMPONENT_UNSELECTED:
		{
			//TODO: place hide code here
			DestroyWindow(hWndLV);
		}
		break;
	}
	return (INT_PTR)FALSE;
};
	
HWND CreateListView(HWND hWnd, UINT uId, UINT x, UINT y, UINT width, UINT height)
{
    HWND hWndLV = CreateWindow(WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | WS_VISIBLE, x, y, width, height, hWnd, (HMENU)uId, GetModuleHandle(NULL), NULL);
    ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER );
	return hWndLV;
}

int SetListViewColumns(HWND hWndLV, int textMaxLen, StringVector svHeader)
{
    RECT rcl;
    GetClientRect(hWndLV, &rcl);
   
    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_MINWIDTH;
	lvc.cx = (rcl.right - rcl.left) / svHeader.size();
	lvc.cxMin = 24;
    lvc.cchTextMax = textMaxLen;
 
	StringVectorIterator sviIterator = svHeader.begin();

	int index = -1;
 
	do
	{
		lvc.pszText = (wchar_t * ) sviIterator->c_str();
		index = ListView_InsertColumn(hWndLV, index + 1, &lvc);
        if (index == -1) break;
		sviIterator++;
	}
	while (sviIterator != svHeader.end());
 
    return index;
}

BOOL WINAPI AddListViewItems(HWND hWndLV, int textMaxLen, UINT uRow, UINT uColumn, tstring tsText)
{
	if (uColumn == 0)
	{
		LVITEM lvi = { 0 };//int iLastIndex = ListView_GetItemCount(hWndLV);
		lvi.mask = LVIF_TEXT;
		lvi.cchTextMax = textMaxLen;
		lvi.iItem = uRow;
		lvi.pszText = (wchar_t*) tsText.c_str();
		lvi.iSubItem = 0;
		return (ListView_InsertItem(hWndLV, &lvi) == -1);
	}
	else
		ListView_SetItemText(hWndLV, uRow, uColumn, (wchar_t*) tsText.c_str());
	return true;
}

MYLAMP_HEADER_API mylamp::Component* RegComponent()
{
	pComponent = new Updater();
	return pComponent;
}; 

MYLAMP_HEADER_API void FreeComponent()
{
	delete pComponent;
};

