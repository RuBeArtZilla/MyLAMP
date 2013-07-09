// mylamp_updater.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

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

INT_PTR CALLBACK Updater::SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

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

