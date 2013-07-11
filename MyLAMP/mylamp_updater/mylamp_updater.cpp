// mylamp_updater.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "../mylamp_core/mylamp_lib.h"

#define UPDATER_ITEM_NAME L"Components"
mylamp::Component * pComponent;

class Updater: public mylamp::Component
{
private:
	bool b_init;
	bool b_load;
public:
	Updater();
	virtual bool IsInit(){return b_init;};
	virtual bool IsLoad(){return b_load;};

	virtual bool Load(){return (b_load = true);};
	virtual ~Updater(){};
	virtual mylamp::COMPONENT_INFO GetInfo();
	virtual UINT64 GetCoreMinVersion(){return 1;};
	virtual settings_items GetSettingsItems(); 	
	virtual bool CheckSelectedItem(StringVector svReversedItem);
	virtual INT_PTR CALLBACK SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
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
	//Components* pComponents = GetComponents();

	return (INT_PTR)FALSE;
};
	


MYLAMP_HEADER_API mylamp::Component* RegComponent()
{
	pComponent = new Updater();
	return pComponent;
}; 

MYLAMP_HEADER_API void FreeComponent()
{
	delete pComponent;
};

