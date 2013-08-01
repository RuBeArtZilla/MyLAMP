// mylamp_cui.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#define CUI_ITEM_NAME L"ClassicUI"
#define CUI_PARENT_ITEM_NAME L"Display"
#include "../mylamp_core/mylamp_lib.cpp"

mylamp::Component * pComponent = NULL;

class ClassicUI: public mylamp::Component
{
private:
	bool b_init;
	bool b_load;
	mylamp::Component* p_parent;

public:
	ClassicUI();
	virtual bool IsInit(){return b_init;};
	virtual bool IsLoad(){return b_load;};

	virtual bool Load(Component* pParent = 0){p_parent = pParent; return (b_load = true);};
	virtual ~ClassicUI(){};
	virtual mylamp::COMPONENT_INFO GetInfo();
	virtual UINT64 GetCoreMinVersion(){return 1;};
	
	virtual settings_items GetSettingsItems(); 	
	virtual bool CheckSelectedItem(StringVector svReversedItem);

	virtual INT_PTR CALLBACK SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool AddWndProc(_WndProc wndproc){return false;};
	virtual bool DelWndProc(_WndProc wndproc){return false;};
};


//---------------------------------------------------------------------------
ClassicUI::ClassicUI()
{
	b_init = false;
	b_load = false;
}

//---------------------------------------------------------------------------
mylamp::COMPONENT_INFO ClassicUI::GetInfo()
{
	mylamp::COMPONENT_INFO ciResult = {1, 0};
	return ciResult;
}

//---------------------------------------------------------------------------
settings_items ClassicUI::GetSettingsItems() 
{
	settings_items siResult;
	siResult.items.push_back(CUI_ITEM_NAME);
	siResult.path.push_back(CUI_PARENT_ITEM_NAME);
	return siResult;
}

//---------------------------------------------------------------------------
bool ClassicUI::CheckSelectedItem(StringVector svReversedItem)
{
	//TODO: rewrite this code
	if (svReversedItem.size() != 2)
		return false;

	if (	(!svReversedItem.at(0).compare(CUI_ITEM_NAME)) &&
			(!svReversedItem.at(1).compare(CUI_PARENT_ITEM_NAME))	)
		return true;

	return false;
}

//---------------------------------------------------------------------------
INT_PTR CALLBACK ClassicUI::SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		}
		break;

	case WM_COMPONENT_UNSELECTED:
		{
			//TODO: place hide code here
		}
		break;
	}
	return (INT_PTR)FALSE;
};

//---------------------------------------------------------------------------
MYLAMP_HEADER_API mylamp::Component* RegComponent()
{
	pComponent = new ClassicUI();
	return pComponent;
}

//---------------------------------------------------------------------------
MYLAMP_HEADER_API void FreeComponent()
{
	delete pComponent;
	pComponent = NULL;
}
