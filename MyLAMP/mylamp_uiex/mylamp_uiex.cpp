// mylamp_uiex.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#define UIEX_ITEM_NAME L"UI Extended"
#define UIEX_PARENT_ITEM_NAME L"Display"

#define SS_UIEX_ITEM_NAME L"UI Extended"
#define SSPN_EXTEND_BORDER L"extend_frame_into_client_area"


//#include "../mylamp_core/mylamp_lib.cpp"
#include "../mylamp_core/mlp_settings_connector.cpp"

mylamp::Component * pComponent = NULL;
bool bInitUI = false;

INT_PTR MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class UIEX: public mylamp::Component
{
private:
	bool b_init;
	bool b_load;
	bool b_extend_frame;
	mylamp::Component* p_parent;

public:
	UIEX();
	virtual bool IsInit(){return b_init;};
	virtual bool IsLoad(){return b_load;};

	virtual bool Load(Component* pParent = 0);
	virtual ~UIEX(){};
	virtual mylamp::COMPONENT_INFO GetInfo();
	virtual UINT64 GetCoreMinVersion(){return 1;};
	
	virtual settings_items GetSettingsItems(); 	
	virtual bool CheckSelectedItem(StringVector svReversedItem);

	virtual INT_PTR CALLBACK SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual bool AddWndProc(_WndProc wndproc){return false;};
	virtual bool DelWndProc(_WndProc wndproc){return false;};
};

INT_PTR MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!bInitUI)
	{
		MARGINS mar = { 0, 0, 0, 100 };

		DwmExtendFrameIntoClientArea(hWnd, &mar);
		
		/*if(!AeroAutoSubclass(m_hWnd, ASC_SUBCLASS_ALL_CONTROLS, 0L))
		{
			//TODO: error
		}*/

		bInitUI = true;
	}
	return (INT_PTR)FALSE;
};

UIEX::UIEX()
{
	b_init = false;
	b_load = false;
}

bool UIEX::Load(Component* pParent)
{
	//stat_ss::saveSetting(tstring(SS_UIEX_ITEM_NAME), tstring(SSPN_EXTEND_BORDER), tstring(L"true"));
	SaveSetting(tstring(SS_UIEX_ITEM_NAME), tstring(SSPN_EXTEND_BORDER), tstring(L"true"));

	b_extend_frame = false;

	p_parent = pParent; 
	pParent->AddWndProc(MainWndProc); 
	return (b_load = true);
}

mylamp::COMPONENT_INFO UIEX::GetInfo()
{
	mylamp::COMPONENT_INFO ciResult = {1, 0};
	return ciResult;
}

settings_items UIEX::GetSettingsItems() 
{
	settings_items siResult;
	siResult.items.push_back(UIEX_ITEM_NAME);
	siResult.path.push_back(UIEX_PARENT_ITEM_NAME);
	return siResult;
}

bool UIEX::CheckSelectedItem(StringVector svReversedItem)
{
	//TODO: rewrite this code
	if (svReversedItem.size() != 2)
		return false;

	if (	(!svReversedItem.at(0).compare(UIEX_ITEM_NAME)) &&
			(!svReversedItem.at(1).compare(UIEX_PARENT_ITEM_NAME))	)
		return true;

	return false;
}

INT_PTR CALLBACK UIEX::SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

MYLAMP_HEADER_API mylamp::Component* RegComponent()
{
	pComponent = new UIEX();
	return pComponent;
}; 

MYLAMP_HEADER_API void FreeComponent()
{
	delete pComponent;
};

